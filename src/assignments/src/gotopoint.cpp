#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2_msgs/TFMessage.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <math.h>
#include <std_msgs/Bool.h>
#include <geometry_msgs/Vector3.h>



using namespace ros;

float closestOb = 10000;

float currX = 0;
float currY = 0;
float goalX = 0;
float goalY = 0;
double roll =0;
double yaw = 0;
double pitch = 0;
float angleToPoint = 0;
tf2::Quaternion q;
bool turned = 0;
bool goalReceived = 0;
double tolerance = 0.3;
bool turnDone = 0;
int turnDoneOb = 0;
const double distanceTolerance = .75;

void scanMessageReceived(const sensor_msgs::LaserScan &msg)
{
    float closest = msg.ranges[0];
    int closestIndex = 0;
    for (int i = 1; i < msg.ranges.size(); i++)
    {
        if (msg.ranges[i] < closest)
        {
            closest = msg.ranges[i];
            closestIndex = i;
        }
    }
 
    closestOb = closest;
}

void intBaseReceived(const nav_msgs::Odometry::ConstPtr &msgP){


currX = msgP->pose.pose.position.x;
currY = msgP->pose.pose.position.y;
tf2::fromMsg(msgP->pose.pose.orientation, q);



}

void pointReceived(const geometry_msgs::Vector3::ConstPtr &msgV){


goalX = msgV->x;
goalY = msgV->y;
goalReceived = 1;


}



int main(int argc, char **argv)
{
    init(argc, argv, "gotopoint");
    NodeHandle nh;

    Subscriber subScan = nh.subscribe("/scan", 10000, &scanMessageReceived);
    Subscriber poseSub = nh.subscribe<nav_msgs::Odometry>("/odometry/filtered", 10000, &intBaseReceived);
    Subscriber pointSub = nh.subscribe<geometry_msgs::Vector3>("points", 5000, &pointReceived);

    Publisher huskyPub = nh.advertise<geometry_msgs::Twist>("/husky_velocity_controller/cmd_vel", 10000);
    Publisher goalPub = nh.advertise<std_msgs::Bool>("goalStream", 4000);


    Rate rate(1);
    geometry_msgs::Twist msg;
    std_msgs::Bool goalAchieved;
    goalAchieved.data = 0;
    spinOnce();




    while(ok()){

        Duration(.5).sleep(); //2

        spinOnce();

        ROS_INFO_STREAM("The points we're going to are (" << goalX << "," << goalY << ")");

        if(goalReceived == 1 && turnDone == 0){
                    goalAchieved.data = 0;
        goalPub.publish(goalAchieved);
        Duration(.5).sleep(); //2
        spinOnce();
            //turnDone = 0;
            Duration(.5).sleep(); //1
            spinOnce();
            tf2::Matrix3x3 o(q);
            o.getRPY(roll, pitch, yaw);

            angleToPoint = atan2((goalX - currY), (goalY - currX));

            if(goalX < currX){
             angleToPoint += M_PI;

            }

            while(yaw < angleToPoint -0.05  || yaw > angleToPoint + 0.5){
                spinOnce();
                tf2::Matrix3x3 o(q);
                o.getRPY(roll, pitch, yaw);
                msg.linear.x = 0;
                msg.linear.y = 0;
                msg.angular.z = M_PI/4;
                huskyPub.publish(msg);
                Duration(.1).sleep();
            }

            msg.linear.x = 0;
            msg.linear.y = 0;
            msg.angular.z = 0;
            huskyPub.publish(msg);
            //goalReceived = 0;
            turnDone = 1;
            //ROS_INFO_STREAM("Correct Angle Achieved!");
           // ROS_INFO_STREAM("goal Received is " << goalReceived);
            //ROS_INFO_STREAM("turnDone is " << turnDone);
            
        }

        if(goalReceived == 1 && turnDone == 1){

            while(closestOb < .9){
                ROS_INFO_STREAM("Closest Object is " << closestOb << "m away, adjusting!");
                msg.linear.x = -5;
                msg.linear.y = 0;
                msg.angular.z = 0;
                huskyPub.publish(msg);
                Duration(.3).sleep();
                msg.linear.x = 0;
                msg.linear.y = 0;
                msg.angular.z = M_PI;
                huskyPub.publish(msg);
                Duration(.3).sleep();
                msg.linear.x = 3;
                msg.linear.y = 0;
                msg.angular.z = 0;
                huskyPub.publish(msg);
                Duration(.3).sleep();
                spinOnce();

            }

            if(closestOb >= .9){
                turnDone = 0;
            }


            if((currX < goalX - distanceTolerance) || (currX > goalX + distanceTolerance) && (currY < goalY - distanceTolerance) || (currY > goalY + distanceTolerance)){
                //ROS_INFO_STREAM("Moving Forward!");
                msg.linear.x = 1;
                msg.linear.y = 0;
                msg.angular.z = 0;
                huskyPub.publish(msg);
                Duration(.3).sleep(); //.5
                spinOnce();
                
            }

                else{
                msg.linear.x = 0;
                msg.linear.y = 0;
                msg.angular.z = 0;
                huskyPub.publish(msg);
                ROS_INFO_STREAM("We've reached the point!");
                ROS_INFO_STREAM("Goal Position: (" << goalX << "," << goalY << ")");
                ROS_INFO_STREAM("Current Position: (" << currX << "," << currY << ")");
                goalReceived = 0;
                turnDone = 0;
                goalAchieved.data = 1;
                goalPub.publish(goalAchieved);
                Duration(5).sleep();
                spinOnce();


            }



        }


        /*
        goalAchieved.data = 1;
        goalPub.publish(goalAchieved);
        Duration(2).sleep();

        spinOnce();

        goalAchieved.data = 0;
        goalPub.publish(goalAchieved);


        Duration(2).sleep();
                spinOnce();
                */




        //     if(goalReceived == 1){
        //     turnDone = 0;
        //     Duration(1).sleep();
        //     spinOnce();
        //     tf2::Matrix3x3 o(q);
        //     o.getRPY(roll, pitch, yaw);

        //     angleToPoint = atan2((goalX - currY), (goalY - currX));

        //     while(yaw < angleToPoint -0.05  || yaw > angleToPoint + 0.5){
        //         spinOnce();
        //         tf2::Matrix3x3 o(q);
        //         o.getRPY(roll, pitch, yaw);
        //         msg.linear.x = 0;
        //         msg.linear.y = 0;
        //         msg.angular.z = M_PI/4;
        //         huskyPub.publish(msg);
        //         Duration(.1).sleep();
        //     }

        //     msg.linear.x = 0;
        //     msg.linear.y = 0;
        //     msg.angular.z = 0;
        //     huskyPub.publish(msg);
        //     goalReceived = 0;
        //     turnDone = 1;
            
        // }

        // if(goalReceived == 1 && turnDone == 1){

        //     while(closestOb < 1.1){
        //         msg.linear.x = -1;
        //         msg.linear.y = 0;
        //         msg.linear.z = 0;
        //         huskyPub.publish(msg);
        //         Duration(.5).sleep();
        //         msg.linear.x = 0;
        //         msg.linear.y = 0;
        //         msg.linear.z = M_PI_2;
        //         huskyPub.publish(msg);
        //         Duration(.5).sleep();

        //     }



        

}
}
