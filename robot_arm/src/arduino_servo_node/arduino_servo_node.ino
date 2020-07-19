/*
 * rosserial Servo Control Example
 *
 * This sketch demonstrates the control of hobby R/C servos
 * using ROS and the arduiono
 * 
 * For the full tutorial write up, visit
 * www.ros.org/wiki/rosserial_arduino_demos
 *
 * For more information on the Arduino Servo Library
 * Checkout :
 * http://www.arduino.cc/en/Reference/Servo
 */

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <Servo.h> 
#include <ros.h>
#include<math.h>
#include <sensor_msgs/JointState.h>

ros::NodeHandle  nh;

Servo base_servo,sh_servo,el_servo;

void servo_cb( const sensor_msgs::JointState& cmd_msg){
  base_servo.write((cmd_msg.position[0])*(57296 / 1000)+90); //set servo angle, should be from 0-180  
  sh_servo.write(cmd_msg.position[1]*(57296 / 1000)+90);
  el_servo.write(cmd_msg.position[2]*(57296 / 1000)+90);
  
  digitalWrite(13, HIGH-digitalRead(13));  //toggle led  
}


ros::Subscriber<sensor_msgs::JointState> sub("/joint_states", servo_cb);

void setup(){
  pinMode(13, OUTPUT);

  nh.initNode();
  nh.subscribe(sub);
  
  base_servo.attach(9); //attach it to pin 9
  sh_servo.attach(10); //attach it to pin 10
  el_servo.attach(11); //attach it to pin 11
}

void loop(){
  nh.spinOnce();
  delay(1);
}
