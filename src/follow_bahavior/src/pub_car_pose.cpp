#include"ros/ros.h"
#include<iostream>
#include"gazebo_msgs/GetModelState.h"
#include"geometry_msgs/PoseStamped.h"

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "true_pose_publisher");
    ros::NodeHandle nh;
    // 获取小车名字，argv第2个
    std::string robot_name = argv[1];
    nh.param<std::string>("robot_name", robot_name);
    // 用于发布小车真实坐标
    ros::Publisher pose_publisher=nh.advertise<geometry_msgs::PoseStamped>("true_pose",10);
    // 用于获得真实坐标
    ros::ServiceClient client=nh.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
    // 发布序号
    int seq = 0;
    ros::Rate rate(10);
    while(ros::ok())
    {
        gazebo_msgs::GetModelState srv;
        // 指定要获取的小车在gazebo中的名字
        srv.request.model_name = robot_name;
        if (client.call(srv))
        {
            static geometry_msgs::PoseStamped pose;
            pose.header.frame_id = "gazebo";
            pose.header.seq = seq++;
            pose.header.stamp = ros::Time::now();
            pose.pose = srv.response.pose;
            pose_publisher.publish(pose);
        }
        else
        {
            ROS_WARN("Failed to call service /gazebo/get_model_state");
        }
        rate.sleep();
    }

    return 0;
}