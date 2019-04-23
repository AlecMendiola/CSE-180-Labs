#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include <nav_msgs/Odometry.h>
#include <math.h>

using namespace ros;


//target pose
float targetX = 1;
float targetY = 1 ;
float targetTheta = 0;

//global variables at the state of the robot
float currentX;
float currentY;
float currentTheta;


void intBaseReceived(const nav_msgs::Odometry::ConstPtr&msg)
{
    // ROS_INFO_STREAM("Received Base X position" << msg->pose.pose.position.x);
    // ROS_INFO_STREAM("Received Base Y position" << msg->pose.pose.position.y);
    // ROS_INFO_STREAM("Received Base Z position" << msg->pose.pose.position.z);
    currentX = msg->pose.pose.position.x;
    currentY = msg->pose.pose.position.y;
    currentTheta = msg->pose.pose.position.z;

}





int main(int argc, char**argv){
    init(argc,argv,"gotoposition");
    NodeHandle nh;

    Publisher movePub = nh.advertise<geometry_msgs::Twist>("/pioneer/cmd_vel", 1000);
    Subscriber poseSub = nh.subscribe<nav_msgs::Odometry>("/base_pose_ground_truth", 1000, &intBaseReceived);
    Rate rate(1);
    geometry_msgs::Twist cmd;

    ROS_INFO_STREAM("Goal target is (" << targetX << "," << targetY << "," << targetTheta << ")");

    int count = 0;

    while(ok()){
        ROS_INFO_STREAM("Current position is (" << currentX << "," << currentY << "," << currentTheta << ")");

        if(count = 0){
        cmd.angular.z = M_PI/4;
        movePub.publish(cmd);
        rate.sleep();
        count++;

        }

        cmd.angular.z = 0;
        movePub.publish(cmd);



        // cmd.linear.x = targetX;
        // cmd.linear.y = targetY;


        
        







        // while(currentX != targetX + 0.5 || currentX != targetX -0.5 || currentX != targetX){
        //     if(currentX > targetX){
        //         cmd.linear.x = -0.2;
        //         movePub.publish(cmd);
        //         rate.sleep();

        //     }
        //     if(currentX < targetX){
        //         cmd.linear.x = 0.2;
        //         movePub.publish(cmd);
        //         rate.sleep();
        //     }



        // }

        // if(currentX == targetX + 0.5 || currentX == targetX - 0.5 || currentX == targetX){
        //     ROS_INFO_STREAM("WE ARE AT X");
        //     cmd.linear.x = 0;
        //     movePub.publish(cmd);
        //     rate.sleep();
        //     }




        // if(currentX != targetX && currentX < targetX){
        //     cmd.linear.x = 0.1;
        //     movePub.publish(cmd);
        //     rate.sleep();

        // }


        // if(currentX = targetX){
        //     cmd.linear.x = 0;
        //     movePub.publish(cmd);
        //     rate.sleep();
        // }

        // if(currentY != targetY && currentY > targetY){
        //     cmd.linear.y = -0.5;
        //     movePub.publish(cmd);

        // }
        // if(currentY != targetY && currentY < targetY){
        //     cmd.linear.y = 0.5;
        //     movePub.publish(cmd);

        // }

        spinOnce();
        rate.sleep();

    }

}