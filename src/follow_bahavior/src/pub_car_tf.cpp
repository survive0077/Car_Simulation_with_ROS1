#include"ros/ros.h"
#include"tf2_ros/transform_broadcaster.h"
#include"geometry_msgs/TransformStamped.h"
#include"geometry_msgs/PoseStamped.h"
#include"tf2/LinearMath/Quaternion.h"
#include<iostream>

// 全局变量，小车的名字
std::string robot_name;

void callback(const geometry_msgs::PoseStamped::ConstPtr &pose)
{
    // 静态变量，每次调用使用同一个pub对象，防止重复新建
    static tf2_ros::TransformBroadcaster pub;
    geometry_msgs::TransformStamped tfs;
    tfs.header.frame_id = "gazebo";
    tfs.header.stamp = ros::Time::now();
    tfs.child_frame_id = robot_name;
    // 坐标系偏移量设置，小车为二维平面运动
    tfs.transform.translation.x = pose->pose.position.x;
    tfs.transform.translation.y = pose->pose.position.y;
    tfs.transform.translation.z = 0.0;
    // 四元数设置
    tfs.transform.rotation.x = pose->pose.orientation.x;
    tfs.transform.rotation.y = pose->pose.orientation.y;
    tfs.transform.rotation.z = pose->pose.orientation.z;
    tfs.transform.rotation.w = pose->pose.orientation.w;

    pub.sendTransform(tfs);
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "pub_dynamic");
    ros::NodeHandle nh;
    if (argc != 2)
    {
        ROS_ERROR("Please input a robot name");
    }
    else
    {
        robot_name = argv[1];
    }
    
    ros::Subscriber sub = nh.subscribe<geometry_msgs::PoseStamped>("true_pose", 100, callback);

    ros::spin();
    return 0;
}

