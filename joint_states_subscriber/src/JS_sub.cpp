#include "ros/ros.h"
#include "sensor_msgs/JointState.h"

void chatterCallback(const sensor_msgs::JointState& msg)
{
  ROS_INFO("I heard: [%f]", msg.position[0]);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "JS_sub");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/joint_states", 1000, chatterCallback);

  ros::spin();

  return 0;
}
