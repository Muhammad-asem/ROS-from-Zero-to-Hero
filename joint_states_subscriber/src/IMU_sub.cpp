#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include<std_msgs/Float32.h>
#include<geometry_msgs/Vector3.h>

sensor_msgs::Imu topublish;
void chatterCallback(const sensor_msgs::Imu& msg)
{
  ROS_INFO("I heard: [%f]", msg.linear_acceleration.z);
  topublish=msg;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "IMU_sub");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/phone1/android/imu", 1000, chatterCallback);
ros::Rate loop_rate(10);
  ros::Publisher IMU_pub =nh.advertise<geometry_msgs::Vector3>("IMU/clone",1000);
  //ros::spin();

  while (ros::ok())
   {
     if(topublish.header.seq!=0){
     IMU_pub.publish(topublish.linear_acceleration);
     }
     ros::spinOnce();
     loop_rate.sleep();

   }

  return 0;
}
