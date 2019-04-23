#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <math.h>
#include <stdlib.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/Quaternion.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Point.h>


using namespace ros;

double amclX = 0;
double amclY = 0;
double amclTheta = 0;
double avgPoseX = 0;
double cloudCount = 0;
double cloudAddX = 0;
double cloudAddY = 0;
double avgPoseY = 0;

void amclReceived(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg){

    amclX = msg->pose.pose.position.x;
    amclY = msg->pose.pose.position.y;
    amclTheta = msg->pose.pose.orientation.w;

    ROS_INFO_STREAM("amcl_pose is (" << amclX << "," << amclY << ")");



}

void cloudReceived(const geometry_msgs::PoseArray::ConstPtr& msg){

    for(int c = 0; c < msg->poses.size(); c++){

        cloudAddX += msg->poses[c].position.x;
        cloudAddY += msg->poses[c].position.y;
        cloudCount++;

    }

    avgPoseX = cloudAddX/cloudCount;
    avgPoseY = cloudAddY/cloudCount;
    ROS_INFO_STREAM("Avg from particle cloud (" << avgPoseX << "," << avgPoseY << ")");


}

void calcAvg(){
    ROS_INFO_STREAM("Difference between average and amcl_pose is (" << fabs(avgPoseX - amclX) << "," << fabs(avgPoseY - amclY) << ")");

}



int main(int argc, char**argv){
    init(argc,argv,"averagepose");
    NodeHandle nh;


    Subscriber acmlSub = nh.subscribe("/amcl_pose", 2000, &amclReceived);

    Subscriber cloudSub = nh.subscribe("/particlecloud", 2000, &cloudReceived);

    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base",true);
    ROS_INFO_STREAM("Waiting for server to be available...");


    while (!ac.waitForServer()) {
    }
    ROS_INFO_STREAM("done!");

    move_base_msgs::MoveBaseGoal goal;

    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = Time::now();
    goal.target_pose.pose.orientation.w = 1;



    Rate rate(1);

    spinOnce();



    while(ok()){

        calcAvg();
        Duration(2).sleep();
        goal.target_pose.pose.position.x = rand() % 2 + -2;
        goal.target_pose.pose.position.y = rand() % 2 + -2;
        ac.sendGoal(goal);
        ac.waitForResult();
        calcAvg();
        if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
            spinOnce();
            //ROS_INFO_STREAM("Success");
        }

        calcAvg();
        spinOnce();


    }

}