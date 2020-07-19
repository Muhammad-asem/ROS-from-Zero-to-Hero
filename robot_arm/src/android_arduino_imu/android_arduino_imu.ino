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
#include <sensor_msgs/Imu.h>

ros::NodeHandle  nh;

Servo base_servo,sh_servo,el_servo;

void servo_cb( const sensor_msgs::Imu& cmd_msg){
  //base_servo.write((cmd_msg.linear_acceleration.x)*(57296 / 1000)+90); //set servo angle, should be from 0-180  
  
  
  digitalWrite(13, HIGH-digitalRead(13));  //toggle led  
}


ros::Subscriber<sensor_msgs::Imu> sub("/imu",servo_cb);
//ros::Subscriber sub = nh.subscribe("/phone1/android/imu", 1000,servo_cb);

void setup(){
  //Serial.begin(9600);
  pinMode(13, OUTPUT);

  nh.initNode();
  nh.subscribe(sub);
  
  base_servo.attach(9); //attach it to pin 9
  sh_servo.attach(10); //attach it to pin 10
  el_servo.attach(11); //attach it to pin 11
}

void loop(){
  nh.spinOnce();
  delay(500);
}
