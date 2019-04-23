#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Pose.h>

void intOdomReceived(const nav_msgs::Odometry::ConstPtr&msg){
    ROS_INFO_STREAM("Received Odom X position" << msg->pose.pose.position.x);
    ROS_INFO_STREAM("Received Base Y position" << msg->pose.pose.position.y);
}

void intBaseReceived(const nav_msgs::Odometry::ConstPtr&msg)
{
    ROS_INFO_STREAM("Received Base X position" << msg->pose.pose.position.x);
    ROS_INFO_STREAM("Received Base Y position" << msg->pose.pose.position.y);
}


using namespace ros;

int main(int argc, char **argv)
{
    init(argc, argv, "drift");
    NodeHandle nh;

    Subscriber odomSub = nh.subscribe<nav_msgs::Odometry>("/pioneer/odom", 1000, &intOdomReceived);
    Subscriber poseSub = nh.subscribe<nav_msgs::Odometry>("/base_pose_ground_truth", 1000, &intBaseReceived);

    spin();

    Publisher posePub = nh.advertise<geometry_msgs::Twist>("/posedrift", 1000);

    // Rate rate(1);

    // while(ok()){
    //     ROS_INFO_STREAM("goofy goober");
    //     rate.sleep();
    // }



}