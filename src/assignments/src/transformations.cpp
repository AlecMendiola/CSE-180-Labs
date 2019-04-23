#include <ros/ros.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2_msgs/TFMessage.h>
#include <tf/transform_listener.h>
#include <tf2_ros/transform_listener.h>
#include <tf2/LinearMath/Transform.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/LinearMath/Vector3.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
using namespace ros;
using namespace std;




int main(int argc, char **argv)
{
    init(argc, argv, "transformations");
    NodeHandle nh;

    tf2_ros::Buffer buffer;
    tf2_ros::TransformListener listener(buffer);

    Rate rate(1);

    geometry_msgs::TransformStamped transformStamped;
    tf2::Quaternion q;
    tf2::Quaternion q1;
    tf2::Quaternion q2;
    tf2::Quaternion q3;
    tf2::Matrix3x3 m1;
    tf2::Matrix3x3 m2;
    tf2::Matrix3x3 m3;




    while(ok()){

        try{
            transformStamped = buffer.lookupTransform("front_sonar", "back_sonar", Time(0));

        }

        catch(tf2::TransformException &ex){
            ROS_WARN("%s", ex.what());
            Duration(1.0).sleep();
            continue;
        }



        //getting the geometry msgs quaternion to a tf2 quaternion

        tf2::fromMsg(transformStamped.transform.rotation, q);
    


        //getting w,x,y,z & making them a-d to make it easier to use the formula for the book to get a rotation matrix

        double a = transformStamped.transform.rotation.w;
        double b = transformStamped.transform.rotation.x;
        double c = transformStamped.transform.rotation.y;
        double d = transformStamped.transform.rotation.z;
        






        //uses aforementioned formula as well as using the .translation.x,y,z as those are the px, py, pz

        ROS_INFO_STREAM("Transformation Matrix of " << transformStamped.header.frame_id << " and " << transformStamped.child_frame_id <<  endl << 2*(a*a + b*b)-1 << "\t" << 2*(b*c - a*d) <<"\t" << 2*(b * d + a *c)<<"\t" << transformStamped.transform.translation.x << endl << 2*(b*c+a*d) << "\t" << 2*(a * a+ c * c)-1 << "\t" << 2*(c*d - a*b) << "\t" << transformStamped.transform.translation.y << endl << 2*(b*d - a*c) << "\t" << 2*(c*d + a*b) << "\t" << 2 * (a*a + d*d) - 1 << "\t" << transformStamped.transform.translation.z << endl << "0\t0\t0\t1" << endl);





        //Goal of Part 2:multiply parent frontsonar child baselink by parent baselink child odom 



        try{
            transformStamped = buffer.lookupTransform("front_sonar", "base_link", Time(0));
        }




        catch (tf2::TransformException &ex){
            ROS_WARN("%s", ex.what());
            Duration(1.0).sleep();
            continue;
        }




            //setting variables for the px,py,pz as we will need them for a calculation later

            double changeAx = transformStamped.transform.translation.x;
            double changeAy = transformStamped.transform.translation.y;
            double changeAz = transformStamped.transform.translation.z;


            tf2::Vector3 o1(changeAx,changeAy,changeAz);

            //because for the calculation we need them bundled together we're putting them in a vector3



            tf2::fromMsg(transformStamped.transform.rotation,q1);

            //getting the tf2 quaternion of front-sonar base-link

            m1.setRotation(q1);

            //setting up the 3x3 matrix for front-sonar-base-link




                try{

            transformStamped = buffer.lookupTransform("pioneer/base_link", "pioneer/odom", Time(0));
        }




        catch (tf2::TransformException &ex)
        {
            ROS_WARN("%s", ex.what());
            Duration(1.0).sleep();
            continue;
        }





        tf2::fromMsg(transformStamped.transform.rotation,q2);

        //getting tf2 quaternion for baselink-odom

        m2.setRotation(q2);

        //setting up 3x3 matrix for baselink-odom

        m3 = m1 * m2;

        //to get the rotation of front-sonar-odom we needed to multiply the previous two rotation matrices

        m3.getRotation(q3);

        //getting the quaternion of the newly formed 3x3 matrix of front-sonar-odom

        b = q3.getX();
        c = q3.getY();
        d = q3.getZ();
        a = q3.getW();

        //setting a-d as the new matrix's x-w so we could print it out easier



        double changeBx = transformStamped.transform.translation.x;
        double changeBy = transformStamped.transform.translation.y;
        double changeBz = transformStamped.transform.translation.z;

        //getting px, py, pz of the second transformation matrix as we'll need to use it to compute the last matrix's px, py ,pz


        tf2::Vector3 o2(changeBx,changeBy,changeBz);

        //bundling them into a vec3 so we can multiply it


    

        tf2::Vector3 o3 = m1 * o2 + o1;

        //in accordance to the formula in the book to get the px, py, pz of front-sonar-odom we need the rotation matrix of front-sonar-baselink multiplied by baselink-odom's px, py, pz and front-sonar-baselink px, py, pz added to it



        ROS_INFO_STREAM("Transformation Matrix of front_sensor and odom" <<  endl << 2*(a*a + b*b)-1 << "\t" << 2*(b*c - a*d) <<"\t" << 2*(b * d + a *c)<<"\t" << o3.getX() << endl << 2*(b*c+a*d) << "\t   " << 2*(a * a+ c * c)-1 << "\t" << 2*(c*d - a*b) << "\t" << o3.getY() << endl << 2*(b*d - a*c) << "\t" << 2*(c*d + a*b) << "\t" << 2 * (a*a + d*d) - 1 << "\t" << o3.getZ() << endl << "0\t0\t0\t1" << endl);


        //printing out transformation matrix of front-sonar-odom




    }

}