#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>


using namespace ros;





double currX = 0;
double currY = 0;
double currTheta = 0;
int count = 0;
int failCount = 0;
bool firstRun = 0;
const double distanceTolerance = 0.2;
double distanceErrorX = 0;
double distanceErrorY = 0;






void amclReceived(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg){

    currX = msg->pose.pose.position.x;
    currY = msg->pose.pose.position.y;
    currTheta = msg->pose.pose.orientation.w;

    //ROS_INFO_STREAM("Received Updated Location Data");
    //ROS_INFO_STREAM("Current Location is (" << currX << "," << currY << ")");



}

bool inRange(double low, double high, double x) 
{ 
    return ((x-high)*(x-low) <= 0); 
} 

int main(int argc,char **argv) {

    init(argc,argv,"navigate");
    NodeHandle nh;


    Publisher mbnPub = nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1000);
    Subscriber acmlSub = nh.subscribe("/amcl_pose", 2000, &amclReceived);

    geometry_msgs::PoseStamped goal;

    goal.header.frame_id = "map";
    goal.header.stamp = Time::now();

    Rate rate(1);




    while (ok()) {

    Duration(5).sleep();


        if(count == 0 && firstRun == 0){
            goal.pose.position.x = -5;
            goal.pose.position.y = -5;
            goal.pose.orientation.w = 1;

            ROS_INFO_STREAM("Publishing 1st goal of (-5,-5)");

            mbnPub.publish(goal);

            firstRun = 1;

            spinOnce();

        }

        if(count == 0 && firstRun == 1){
            
            spinOnce();

            if(inRange(-4.5, -5.5, currX) && inRange(-4.5,-5.5, currY)){
                ROS_INFO_STREAM("Reached 1st Goal!");
                ROS_INFO_STREAM("Current Position is (" << currX << "," << currY << ")");

                Duration(5).sleep();

                count++;
                firstRun = 0;

            }

        }
    
        if(count == 1 && firstRun == 0){
            goal.pose.position.x = -5;
            goal.pose.position.y = 5;
            goal.pose.orientation.w = 1;

            ROS_INFO_STREAM("Publishing 2nd goal of (-5,5)");

            mbnPub.publish(goal);

            firstRun = 1;

            spinOnce();

        }

        if(count == 1 && firstRun == 1){
            
            spinOnce();

            


            if(inRange(-4.5, -5.5, currX) && inRange(4.5,5.5, currY)){
                ROS_INFO_STREAM("Reached 2nd Goal!");
                ROS_INFO_STREAM("Current Position is (" << currX << "," << currY << ")");

                Duration(5).sleep();


                count++;
                firstRun = 0;

            }

        }


        if(count == 2 && firstRun == 0){
            goal.pose.position.x = 5;
            goal.pose.position.y = 5;
            goal.pose.orientation.w = 1;

            ROS_INFO_STREAM("Publishing 3rd goal of (5,5)");

            mbnPub.publish(goal);

            firstRun = 1;

            spinOnce();

        }

        if(count == 2 && firstRun == 1){
            
            spinOnce();

            


            if(inRange(4.5, 5.5, currX) && inRange(4.5,5.5, currY)){
                ROS_INFO_STREAM("Reached 3rd Goal!");
                ROS_INFO_STREAM("Current Position is (" << currX << "," << currY << ")");


                Duration(5).sleep();


                count++;
                firstRun = 0;

            }

        }


        if(count == 3 && firstRun == 0){
            goal.pose.position.x = 5;
            goal.pose.position.y = -5;
            goal.pose.orientation.w = 1;

            ROS_INFO_STREAM("Publishing 4th goal of (5,-5)");

            mbnPub.publish(goal);

            firstRun = 1;

            spinOnce();

        }

        if(count == 3 && firstRun == 1){
            
            spinOnce();

            


            if(inRange(4.5, 5.5, currX) && inRange(-4.5,-5.5, currY)){
                ROS_INFO_STREAM("Reached 4th Goal!");
                ROS_INFO_STREAM("Current Position is (" << currX << "," << currY << ")");
                ROS_INFO_STREAM("All 4 Points Reached!");

                Duration(5).sleep();


                count++;
                firstRun = 0;

            }

        }


    
    //return 0;    
}
}

/*
Write a note navigate (in a file navigate.cpp) that sends the robot through the following poses (x,y,theta):

-5,-5,0

-5,5,0

5,5,0

5,-5,0
*/