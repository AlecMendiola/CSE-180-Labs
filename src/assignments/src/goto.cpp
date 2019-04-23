#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2_msgs/TFMessage.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <math.h>

using namespace ros;

float closestOb = 10000;

float currX = 0;
float currY = 0;
float currTheta = 0;
float goalX = 5;
float goalY = 5;
double roll =0;
double yaw = 0;
double pitch = 0;
int goalAchieved = 0;
float angleToPoint = 0;
tf2::Quaternion q;
int firstRun = 0;
bool turned = 0;

void scanMessageReceived(const sensor_msgs::LaserScan &msg)
{
    // ROS_INFO_STREAM("Received scan. Finding closest obstacle");
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
    //ROS_INFO_STREAM("Closest obstacle at distance (m)" << closest);
    // ROS_INFO_STREAM("Index: " << closestIndex);
    closestOb = closest;
}

void intBaseReceived(const nav_msgs::Odometry::ConstPtr &msgP){

//  ROS_INFO_STREAM("Recieved Position");

currX = msgP->pose.pose.position.x;
currY = msgP->pose.pose.position.y;
tf2::fromMsg(msgP->pose.pose.orientation, q);


// ROS_INFO_STREAM("(X,Y,Theta) is (" << currX << "," << currY << "," << currTheta << ")");

}

int main(int argc, char **argv)
{
    init(argc, argv, "goto");
    NodeHandle nh;
    Subscriber subScan = nh.subscribe("/scan", 10000, &scanMessageReceived);
    Subscriber poseSub = nh.subscribe<nav_msgs::Odometry>("/odometry/filtered", 10000, &intBaseReceived);
    Publisher pub = nh.advertise<geometry_msgs::Twist>("/husky_velocity_controller/cmd_vel", 10000);

    Rate rate(5);
    geometry_msgs::Twist msg;



    while(ok()){
        //need a drift b/c rotates mess up the odometry/filtered



        tf2::Matrix3x3 o(q);
        o.getRPY(roll, pitch, yaw);

        if(firstRun < 1){
        Duration(2).sleep();

        }




        if(firstRun < 2){
            Duration(1).sleep();
            spinOnce();
            tf2::Matrix3x3 o(q);
            o.getRPY(roll, pitch, yaw);
            // ROS_INFO_STREAM("FIRST RUN START:");
            // ROS_INFO_STREAM("current yaw is " << yaw);
            // ROS_INFO_STREAM("first loop curr x & yeah is " <<currX <<" " << currY);
            angleToPoint = atan2((5 - currY), (5 - currX));

            while(yaw < angleToPoint -0.05  || yaw > angleToPoint +0.5  && firstRun == 1){
                spinOnce();
                tf2::Matrix3x3 o(q);
                o.getRPY(roll, pitch, yaw);
                msg.linear.x = 0;
                msg.linear.y = 0;
                msg.angular.z = M_PI/4;
                pub.publish(msg);
                // ROS_INFO_STREAM("the current yaw is " << yaw);
                // ROS_INFO_STREAM("Goal Angle is " << angleToPoint);
                Duration(.1).sleep();
            }

            msg.linear.x = 0;
            msg.linear.y = 0;
            msg.angular.z = 0;
            pub.publish(msg);
            // ROS_INFO_STREAM("Correct Angle Reached!");
            firstRun++;
            // ROS_INFO_STREAM("FIRST RUN DONE");
            
        }





        while(currX < 4.75  && currY < 4.75  && firstRun  == 2){

        

        if(closestOb < 1.1)
        {
            
            ROS_INFO_STREAM("Closest Object is " << closestOb);
            
            msg.linear.x = 0;
            msg.linear.y = 0;
            msg.angular.z =  M_PI/2;
            pub.publish(msg);
            Duration(1).sleep();

            msg.linear.x = .5;
            msg.linear.y = 0;
            msg.angular.z = 0;
            pub.publish(msg);
            Duration(1).sleep();

            turned = 1;

            spinOnce();

            
        }

        if(turned == 1 && closestOb > 1.1){

            spinOnce();

            ROS_INFO_STREAM("Correcting Angle after a Jam");
            tf2::Matrix3x3 o(q);
            o.getRPY(roll, pitch, yaw);

            angleToPoint = atan2((5 - currY), (5 - currX));

            // ROS_INFO_STREAM("The current x and y is {" << currX << "," << currY << "}");

            // ROS_INFO_STREAM("Current yaw is " << yaw);

            // ROS_INFO_STREAM("This is the angle to point after jam" << angleToPoint);

            // ROS_INFO_STREAM("Correct Angle Should be " <<angleToPoint);
            // ROS_INFO_STREAM("Current angle is " << yaw);

            while(yaw < angleToPoint -0.5  || yaw > angleToPoint + 0.5){
                tf2::Matrix3x3 o(q);
                o.getRPY(roll, pitch, yaw);
                msg.linear.x = 0;
                msg.linear.y = 0;
                msg.angular.z = M_PI/4;
                pub.publish(msg);
                // ROS_INFO_STREAM("the current yaw is " << yaw);
                // ROS_INFO_STREAM("Goal Angle is " << angleToPoint);
                Duration(.1).sleep();
                spinOnce();
            }

            msg.linear.x = 0;
            msg.linear.y = 0;
            msg.angular.z = 0;
            pub.publish(msg);
            // ROS_INFO_STREAM("Correct Angle Reached!");


            turned = 0;
            spinOnce();
            
        }


            msg.linear.x = .5;
            msg.linear.y = 0;
            msg.angular.z = 0;
            pub.publish(msg);
            Duration(.1).sleep();
            msg.linear.x = 0;
            msg.linear.y = 0;
            msg.angular.z = 0;
            pub.publish(msg);
            ROS_INFO_STREAM("Current X, Y is (" << currX << "," << currY << ")" );
            Duration(.1).sleep();
            spinOnce();



        }

        if(currY > 4.75  && currX > 4.75){
            msg.linear.x = 0;
            msg.linear.y = 0;
            msg.angular.z = 0;
            pub.publish(msg);
            Duration(.5).sleep();
            goalAchieved++;
        }

        if(goalAchieved == 2 && currY >= 4.5 && currX >= 4.5){
        msg.linear.x = 0;
        msg.linear.y = 0;
        msg.angular.z = 0;
        pub.publish(msg);
        Duration(.5).sleep();

        ROS_INFO_STREAM("GOAL REACHED! Final Position (" << currX << "," << currY << ")");
        exit(0);

        }

        else{
            spinOnce();
             tf2::Matrix3x3 o(q);
            o.getRPY(roll, pitch, yaw);
            // ROS_INFO_STREAM("yaw is " << yaw);
            angleToPoint = atan2((5 - currY), (5 - currX));
            ROS_INFO_STREAM("Course Correcting");


            while(yaw < angleToPoint -0.1  || yaw > angleToPoint + 0.1  && firstRun == 2){
                spinOnce();
                tf2::Matrix3x3 o(q);
                o.getRPY(roll, pitch, yaw);
                msg.linear.x = 0;
                msg.linear.y = 0;
                msg.angular.z = M_PI/8;
                pub.publish(msg);
                Duration(.01).sleep();
            }

            msg.linear.x = 0;
            msg.linear.y = 0;
            msg.angular.z = 0;
            pub.publish(msg);
            Duration(.01).sleep();
            msg.linear.x = 2;
            msg.linear.y = 0;
            msg.angular.z = 0;
            pub.publish(msg);
            Duration(1).sleep();
            msg.linear.x = 0;
            msg.linear.y = 0;
            msg.angular.z = 0;
            pub.publish(msg);
            Duration(.01).sleep();
            spinOnce();
            ROS_INFO_STREAM("Current X, Y is (" << currX << "," << currY << ")");


        }


        spinOnce();
    }
}