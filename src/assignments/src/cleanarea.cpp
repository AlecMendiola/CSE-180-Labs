#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/Quaternion.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

using namespace ros;

double currX = 0;
double currY = 0;
double currTheta = 0;
int count = 0;
int failCount = 0;


void amclReceived(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg){

    currX = msg->pose.pose.position.x;
    currY = msg->pose.pose.position.y;
    currTheta = msg->pose.pose.orientation.w;


}

bool inRange(double low, double high, double x) 
{ 
    return ((x-high)*(x-low) <= 0); 
} 

int main(int argc,char **argv) {

    init(argc,argv,"cleanarea");
    NodeHandle nh;

    Subscriber acmlSub = nh.subscribe("/amcl_pose", 2000, &amclReceived);


    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>
	ac("move_base",true);
    ROS_INFO_STREAM("Waiting for server to be available...");
    while (!ac.waitForServer()) {
    }
    ROS_INFO_STREAM("done!");

    move_base_msgs::MoveBaseGoal goal;

    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = Time::now();

    geometry_msgs::Quaternion q;

    tf2::Quaternion qf;




    
    

    while(ok()){
        Duration(2).sleep();

        if(count == 0){
            goal.target_pose.pose.position.x = -2;
            goal.target_pose.pose.position.y = 1;
            goal.target_pose.pose.orientation.w = 1.0;
            ROS_INFO_STREAM("Clearing Obstacle");
            ac.sendGoal(goal);
            ac.waitForResult();

            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
                spinOnce();
                ROS_INFO_STREAM("Success, obstacle cleared");
                count++;
        }

            else{
                ROS_INFO_STREAM("Failure");
                failCount++;
            if(failCount > 2){
                ROS_INFO_STREAM("Failed 3 times, moving to next point");
                failCount = 0;
                count++;
            }
        }
    }
        if(count == 1){
            goal.target_pose.pose.position.x = 4.2; //4.3
            goal.target_pose.pose.position.y = 0;
            goal.target_pose.pose.orientation.w = 1.0;
            ROS_INFO_STREAM("Going to goal of (4,0)");
            ac.sendGoal(goal);
            ac.waitForResult();

            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
                spinOnce();
	            ROS_INFO_STREAM("Success, current position is (" << currX << "," << currY << ")");
                count++;
            }

            else{
	        ROS_INFO_STREAM("Failure");
            failCount++;
            if(failCount > 2){
                ROS_INFO_STREAM("Failed 3 times, moving to next point");
                failCount = 0;
                count++;
            }
            }
        }

        if(count == 2){
            qf.setRPY(0.0,0.0,M_PI_2);
            qf.normalize();
            tf2::convert(qf, q);

            goal.target_pose.pose.position.x = 8.8; //8.8
            goal.target_pose.pose.position.y = 0;
            goal.target_pose.pose.orientation = q;
            ROS_INFO_STREAM("Going to goal of (9,0)");
            ac.sendGoal(goal);
            ac.waitForResult();

            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
                spinOnce();
	            ROS_INFO_STREAM("Success, current position is (" << currX << "," << currY << ")");
                count++;
                }

            else{
	            ROS_INFO_STREAM("Failure");
                failCount++;
            if(failCount > 2){
                ROS_INFO_STREAM("Failed 3 times, moving to next point");
                failCount = 0;
                count++;
            }
                }
        }

        if(count == 3){
            qf.setRPY(0.0,0.0,M_PI);
            qf.normalize();
            tf2::convert(qf, q);
            goal.target_pose.pose.position.x = 8.8; //8.8
            goal.target_pose.pose.position.y = 2;
            goal.target_pose.pose.orientation = q;
            ROS_INFO_STREAM("Going to goal of (9,2)");
            ac.sendGoal(goal);
            ac.waitForResult();

            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
                spinOnce();
	            ROS_INFO_STREAM("Success, current position is (" << currX << "," << currY << ")");
                count++;
            }

            else{
	            ROS_INFO_STREAM("Failure");
                failCount++;
            if(failCount > 2){
                ROS_INFO_STREAM("Failed 3 times, moving to next point");
                failCount = 0;
                count++;
            }
                }
            }
            
        if(count == 4){
            qf.setRPY(0.0,0.0,M_PI_2);
            qf.normalize();
            tf2::convert(qf, q);
            goal.target_pose.pose.position.x = 4;
            goal.target_pose.pose.position.y = 2;
            goal.target_pose.pose.orientation = q;
            ROS_INFO_STREAM("Going to goal of (4,2)");
            ac.sendGoal(goal);
            ac.waitForResult();

            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
                spinOnce();
	            ROS_INFO_STREAM("Success, current position is (" << currX << "," << currY << ")");
            count++;
        }

            else{
	            ROS_INFO_STREAM("Failure");
                failCount++;
            if(failCount > 2){
                ROS_INFO_STREAM("Failed 3 times, moving to next point");
                failCount = 0;
                count++;
            }
        }
        }

        if(count == 5){
            goal.target_pose.pose.position.x = 4;
            goal.target_pose.pose.position.y = 4;
            goal.target_pose.pose.orientation.w = 1.0;
            ROS_INFO_STREAM("Going to goal of (4,4)");
            ac.sendGoal(goal);
            ac.waitForResult();

            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
                spinOnce();
	            ROS_INFO_STREAM("Success, current position is (" << currX << "," << currY << ")");
                count++;
                }

            else{
                ROS_INFO_STREAM("Failure");
                failCount++;
            if(failCount > 2){
                ROS_INFO_STREAM("Failed 3 times, moving to next point");
                failCount = 0;
                count++;
            }
            }
        }

        if(count == 6){
            qf.setRPY(0.0,0.0,M_PI_2);
            qf.normalize();
            tf2::convert(qf, q);
            goal.target_pose.pose.position.x = 8.7; //8.8
            goal.target_pose.pose.position.y = 4;
            goal.target_pose.pose.orientation = q;
            ROS_INFO_STREAM("Going to goal of (9,4)");
            ac.sendGoal(goal);
            ac.waitForResult();

            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
                spinOnce();
	            ROS_INFO_STREAM("Success, current position is (" << currX << "," << currY << ")");
                count++;
                }

            else{
                ROS_INFO_STREAM("Failure");
                failCount++;
            if(failCount > 2){
                ROS_INFO_STREAM("Failed 3 times, moving to next point");
                failCount = 0;
                count++;
            }
                }
        }

        if(count == 7){
            qf.setRPY(0.0,0.0,M_PI);
            qf.normalize();
            tf2::convert(qf, q);
            goal.target_pose.pose.position.x = 8.7; //8.8
            goal.target_pose.pose.position.y = 6;
            goal.target_pose.pose.orientation = q;
            ROS_INFO_STREAM("Going to goal of (9,6)");
            ac.sendGoal(goal);
            ac.waitForResult();

            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
                spinOnce();
	            ROS_INFO_STREAM("Success, current position is (" << currX << "," << currY << ")");
                count++;
                }

            else{
                ROS_INFO_STREAM("Failure");
                failCount++;
            if(failCount > 2){
                ROS_INFO_STREAM("Failed 3 times, moving to next point");
                failCount = 0;
                count++;
            }
            }
        }

        if(count == 8){
            qf.setRPY(0.0,0.0,M_PI);
            qf.normalize();
            tf2::convert(qf, q);
            goal.target_pose.pose.position.x = 4;
            goal.target_pose.pose.position.y = 6;
            goal.target_pose.pose.orientation = q;
            ROS_INFO_STREAM("Going to final goal of (4,6)");
            ac.sendGoal(goal);
            ac.waitForResult();

            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
                spinOnce();
	            ROS_INFO_STREAM("Success, current position is (" << currX << "," << currY << ")");
                count++;
                }

            else{
                ROS_INFO_STREAM("Failure");
                failCount++;
            if(failCount > 2){
                ROS_INFO_STREAM("Failed 3 times, moving to next point");
                failCount = 0;
                count++;
            }
             }
        }

        if(count == 9){
            ROS_INFO_STREAM("All points reached");
            shutdown(); //clean way to kill program
        }




    }
    



    
    return 0;    
}