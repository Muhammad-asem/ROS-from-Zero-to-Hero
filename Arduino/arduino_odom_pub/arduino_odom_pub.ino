/* 
 * rosserial Planar Odometry Example
 */

#include <ros.h>
#include <ros/time.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include<nav_msgs/Odometry.h>

ros::NodeHandle  nh;

nav_msgs::Odometry odom;
ros::Publisher odom_pub( "odom", &odom);
/////////////////////---------------------------
geometry_msgs::TransformStamped t;
tf::TransformBroadcaster broadcaster; //check this well

double x = 1.0;
double y = 0.0;
double theta = 1.57;


//--------------------------------
//ros::Time current_time,last_time;
//ros::Time current_time= ros::Time::now();
//ros::Time last_time= ros::Time::now();
//-----------------------
char base_link_frame[] = "/base_link";
char odom_frame[] = "/odom";

void setup()
{
  //nh.initNode();  //TAKE CARE OF THIS I HAVE COMMENTED IT 
  
  broadcaster.init(nh);
  nh.advertise(odom_pub);
}

void loop()
{  
    
    //current_time=nh.now();
  
  // drive in a circle velocities and displacemet
  double dx = 0.2;
  double dtheta = 0.18;
  x += cos(theta)*dx*0.1;
  y += sin(theta)*dx*0.1;
  theta += dtheta*0.1;
  if(theta > 3.14)
    theta=-3.14;
    
  // tf odom->base_link
  t.header.frame_id = odom_frame;
  t.child_frame_id = base_link_frame;
  
  t.transform.translation.x = x;
  t.transform.translation.y = y;
  //take care , need we to write Z?
  
  t.transform.rotation = tf::createQuaternionFromYaw(theta);
  t.header.stamp = nh.now();
  
  broadcaster.sendTransform(t);

//nav_msgs::Odometry odom;
    /*odom.header.stamp = nh.now();//current_time;
    odom.header.frame_id = "odom";

    //set the position
    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = tf::createQuaternionFromYaw(theta);

    //set the velocity
    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = dx;
    odom.twist.twist.linear.y = dx;
    odom.twist.twist.linear.z =0.0;
    odom.twist.twist.angular.z = dtheta;

    //publish the message
    odom_pub.publish(&odom);

    //last_time = current_time;////////   */
  nh.spinOnce();
  delay(10);
}
