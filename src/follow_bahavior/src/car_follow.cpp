#include"ros/ros.h"
#include"tf2_ros/buffer.h"
#include"tf2_ros/transform_listener.h"
#include"geometry_msgs/PoseStamped.h"
#include"tf2_geometry_msgs/tf2_geometry_msgs.h"
#include"geometry_msgs/TransformStamped.h"
#include"geometry_msgs/Twist.h"
#include<iostream>
#include"follow_bahavior/model.h"



int main(int argc, char *argv[])
{
    ros::init(argc, argv, "car_follow");
    ros::NodeHandle nh;
    tf2_ros::Buffer buffer;
    tf2_ros::TransformListener sub(buffer);
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 100);
    // 设置父节点和子节点
    std::string father = argv[1];
    std::string child = argv[2];
    // 发布频率，Hz
    int rate_Hz = 10;
    ros::Rate rate(rate_Hz);
    // 后车初始速度为0，若为GM模型则必须不为0否则无法启动！
    double v_after = 0;
    // 前车速度
    double v_front;
    
    while (ros::ok())
    {
        try
        {
            // 计算坐标系间的相对关系，传入参数顺序：目标坐标系，源坐标系，时间
            geometry_msgs::TransformStamped tfs;
            tfs = buffer.lookupTransform(father, child, ros::Time(0));
            // ROS_INFO("father: %s, child: %s, translation: (%.2f,%.2f,%.2f), rotation: (%.2f,%.2f,%.2f,%.2f)",
            //         tfs.header.frame_id.c_str(),
            //         tfs.child_frame_id.c_str(),
            //         tfs.transform.translation.x,
            //         tfs.transform.translation.y,
            //         tfs.transform.translation.z,
            //         tfs.transform.rotation.x,
            //         tfs.transform.rotation.y,
            //         tfs.transform.rotation.z,
            //         tfs.transform.rotation.w);
            // 从参数服务器获取前车速度
            nh.getParam("/" + child + "/vel", v_front);
            ROS_INFO("The speed of %s is %.2f", child.c_str(), v_front);
            geometry_msgs::Twist twist;
            // 小车只有线速度的xy和角速度的z，为简化问题不考虑麦克纳姆轮的y
            double s = sqrt(pow(tfs.transform.translation.x, 2) + pow(tfs.transform.translation.y, 2));
            // GM模型
            //v_after = model::GM_Model(s, v_front, v_after, 1, 1, rate_Hz); 
            // IDM模型
            v_after = model::IDM_Model(s, v_front, v_after, rate_Hz);

            twist.linear.x = v_after;
            twist.angular.z = 4 * atan2(tfs.transform.translation.y, tfs.transform.translation.x);
            // 向参数服务器载入当前车下一秒的速度
            nh.setParam("/" + father + "/vel", v_after);
            pub.publish(twist);
        }
        catch(const std::exception& e)
        {
            ROS_WARN("%s", e.what());
        }
        rate.sleep();
        ros::spinOnce();        
    }
    
    

    return 0;
}
