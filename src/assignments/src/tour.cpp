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

int count = 1;
bool goalGot = 0;
double x;
double y;
bool firstRun = 0;



void goalReceived(const std_msgs::Bool::ConstPtr& msg)
{

    goalGot = msg->data;
    //ROS_INFO_STREAM("Tour: Has Goal Been Achieved? " << goalGot);


    



}



int main(int argc, char **argv)
{
    init(argc, argv, "tour");
    NodeHandle nh;

    double position[] = {1,2,5,6,0,7};
    int npositions = 3; 

    Subscriber goalSub = nh.subscribe<std_msgs::Bool>("goalStream", 4000, &goalReceived);
    Publisher pointPub = nh.advertise<geometry_msgs::Vector3>("points", 5000);

    Rate rate(1);

    geometry_msgs::Vector3 pointSend;

    spinOnce();


    while(ok()){
        Duration(3).sleep();


            if(count == 1 && firstRun == 0){
                pointSend.x = position[0];
                pointSend.y = position[1];
                pointPub.publish(pointSend);
                firstRun == 1;


                
            }

            spinOnce();


         while(count < npositions + 1){
            Duration(3).sleep();

             spinOnce();


            if(goalGot == 1){
            count = count + 2;
            //ROS_INFO_STREAM("Tour: Count is " << count);
            pointSend.x = position[count - 1];            
            pointSend.y = position[count];
            pointPub.publish(pointSend);
            spinOnce();

                 }
              }



    }


}
