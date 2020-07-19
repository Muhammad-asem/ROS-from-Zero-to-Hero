#include "ros/ros.h"
#include "sensor_msgs/Image.h"

void chatterCallback(const sensor_msgs::Image msg)
{
  ROS_INFO("I heard: [%s]", msg.Image_);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("chatter", 1000, chatterCallback);

  ros::spin();

  return 0;
}
