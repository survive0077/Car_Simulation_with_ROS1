#include"ros/ros.h"
#include"geometry_msgs/Twist.h"

std::string robot_name = "car1";

void callback(const geometry_msgs::Twist::ConstPtr msg)
{
    static ros::NodeHandle nh1;
    nh1.setParam("/" + robot_name + "/vel", msg->linear.x);
}


int main(int argc, char *argv[])
{
    // 将键盘控制节点控制的头车速度载入到参数服务器中
    ros::init(argc, argv, "pub_v");
    ros::NodeHandle nh;
    static ros::Subscriber sub = nh.subscribe<geometry_msgs::Twist>("/" + robot_name + "/cmd_vel", 100, callback);
    ros::spin();
    return 0;
}
