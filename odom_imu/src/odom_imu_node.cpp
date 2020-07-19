#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include<sensor_msgs/Imu.h>
#include <geometry_msgs/Point32.h>
/*#define R 0.05
#define N 30.0
#define L 0.15
double Dc=0.0;
double RtickOld=0.0;
double RtickNew=0.0;
double LtickOld=0.0;
double LtickNew=0.0;
*/
static double ax=0.0,ay=0.0,az=0.0,dt=0.0,wz=0.0;// It can be without static as it is a global variable
double x = 0.0;
double y = 0.0;
double z = 0.0;

double vx = 0.0;
double vy = 0.0;
double vz = 0.0;

geometry_msgs::Quaternion qtn ;
void Imu_cb(const sensor_msgs::Imu& msg)
{
/*
  RtickNew=(double)msg.x;//encoder
LtickNew=(double)msg.y;
Dc=((2*3.14*R*(RtickNew-RtickOld)/N)+((2*3.14*R*(LtickNew-LtickOld)/N)))/2;
*/
ax=msg.linear_acceleration.x;
ay=msg.linear_acceleration.y;
az=msg.linear_acceleration.z;
qtn=msg.orientation;
wz=msg.angular_velocity.z;

}

int main(int argc, char** argv){
  ros::init(argc, argv, "odometry_Imu_publisher");
  ros::NodeHandle n;
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom_Imu", 50);
  tf::TransformBroadcaster odom_broadcaster;
  ros::Subscriber sub = n.subscribe("/phone1/android/imu", 1000, Imu_cb);
// Here I need to Insert the subscriber part that subscibes to arduino topics and get the encoder reading then pose rate from pose


  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  ros::Rate r(10.0);
//-----------------------------------While loop--------------------------------
      while(n.ok()){

    ros::spinOnce();               // check for incoming messages

    current_time = ros::Time::now();
/*
    //compute odometry in a typical way given the velocities of the robot
    dt = (current_time - last_time).toSec();
    vx+=ax*dt;//do i need runge_kutta for integration
    vy+=ay*dt;
    //vz+=az*dt;
   //double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
    //double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
    //double delta_th = vth * dt;

    x += vx*dt;
    y += vy*dt;
    //z += vz*dt;
// here i can change the algorithm as i know the distance but not the velocity so i can divide instead of multiply and also i have to consider the
    //sampling and data input from arduino to C++ node
    //since all odometry is 6DOF we'll need a quaternion created from yaw

*/

    /***
    //the orientation does not matter till now I can get it from the compasss then
**/

    //first, we'll publish the transform over tf
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link_imu";

    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = qtn;


    //***********************************************************
    //Causes Error we have to define the
//*******************************************************************


    //send the transform
    odom_broadcaster.sendTransform(odom_trans);

    //next, we'll publish the odometry message over ROS
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";

    //set the position
    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0.0; //--------------------->> enable
    odom.pose.pose.orientation = qtn;

    //set the velocity
    odom.child_frame_id = "base_link_imu";// what if many sensor is on thr base_link
    odom.twist.twist.linear.x = 0.0;//vx;
    odom.twist.twist.linear.y = 0.0;//vy;
    odom.twist.twist.linear.z=0.0;//vz;
    odom.twist.twist.angular.z =0.0;// wz;  //take care

    //publish the message
    odom_pub.publish(odom);

    last_time = current_time;
    r.sleep();
  }
}
