#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Point32.h>
#define R 0.05
#define N 725.0
#define L 0.33
double Dc=0.0;
double RtickOld=0.0;
double RtickNew=0.0;
double LtickOld=0.0;
double LtickNew=0.0;
double x = 0.0;
double y = 0.0;
double th = 0.0;

double vx = 0.0;
double vy = 0.0;
double vth = 0.0;

void encoder_cb(const geometry_msgs::Point32& msg)
{
RtickNew=(double)msg.x;//right encoder msg
LtickNew=(double)msg.y;
Dc=((2*3.14*R*(RtickNew-RtickOld)/N)+((2*3.14*R*(LtickNew-LtickOld)/N)))/2;
x+=Dc*cos(th);
y+=Dc*sin(th);
th+=((2*3.14*R*(RtickNew-RtickOld)/N)-((2*3.14*R*(LtickNew-LtickOld)/N)))/L;
RtickOld=RtickNew;
LtickOld=LtickNew;
}

int main(int argc, char** argv){
  ros::init(argc, argv, "odometry_publisher");
  ros::NodeHandle n;
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
  tf::TransformBroadcaster trans_broadcaster;
  ros::Subscriber sub = n.subscribe("/encoder", 1000, encoder_cb);

  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  ros::Rate r(10.0);
//-----------------------------------While loop--------------------------------
      while(n.ok()){

    ros::spinOnce(); // check for incoming messages

    current_time = ros::Time::now();

    //---odom calculation------
    double dt = (current_time - last_time).toSec();
    vx=Dc*cos(th)/dt;
    vy=Dc*sin(th)/dt;
    vth=(((2*3.14*R*(RtickNew-RtickOld)/N)-((2*3.14*R*(LtickNew-LtickOld)/N)))/L)/dt;

    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

    //first,---------- TRANSFORMATION broadcasting over tf
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "chassis";

    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;

    //send the transform
    trans_broadcaster.sendTransform(odom_trans);

    //next, ------------------ODOMETRY--------------- message over ROS
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";
    odom.child_frame_id = "chassis";
    //set the position
    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odom_quat;

    //set the velocity
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = vy;
    odom.twist.twist.angular.z = vth;

    //send THE ODOM message
    odom_pub.publish(odom);

    last_time = current_time;
    r.sleep();
  }
}
