
#include <ros.h>
#include <geometry_msgs/Point32.h> //For sending encoder msg
#include<geometry_msgs/Twist.h> //For cmd_vel subscription
#include <AFMotor.h> //Adafruit mototr library
#include <Encoder.h> //Encoder library

//----- DC Motor definitions------------
AF_DCMotor motorL(4);  //left motor goes for pin 4
AF_DCMotor motorR(3);  //right motor goes for pin 3

//-------------Encoder definitions
Encoder R_enc(19, 18); //right motor encoder goes for pins 19 18
Encoder L_enc(21, 20); //left motor encoder goes for pins 21 20
long RoldPosition  = -999;
long LoldPosition  = -999;
 //-----------------------------------------Robot parameters definition------------ 
#define L 0.33
#define R 0.05
//--------------------------------Motors VARS-----------------------------------
// initializing variables
float vel=0.0; //as Twist msgs depend on Vector3 which is float64
float omega=0.0;
float VR,VL;
//-----------------------------------------------------------------------------------------
ros::NodeHandle  nh;
//------------------------------Publish init----------------------------------------------
geometry_msgs::Point32 Point_msg;

ros::Publisher enc_pub("/encoder", &Point_msg); 


//-----------------------------------DC Motors Callback subscribers

void motors_cb(const geometry_msgs::Twist& msg)
{
 
    vel=msg.linear.x;    
    omega=msg.angular.z;  
    
     
    VR=(2*vel+omega*L)/(2*R); 
    VL=(2*vel-omega*L)/(2*R); 

    //-----right motor------

    if (VR<0)
    {
       motorR.run(BACKWARD);
       motorR.setSpeed(abs(VR));   
    }

    else 
    {
      
      motorR.run(FORWARD);  
      motorR.setSpeed(VR); 
        
    }

    //-----left motor------

     if (VL<0)
    {
       motorL.run(BACKWARD);
       motorL.setSpeed(abs(VL));   
    }

    else 
    {
      
      motorL.run(FORWARD);  
      motorL.setSpeed(VL); 
        
    }



}

//--------------------subscribers---------------------------
ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &motors_cb);



 void setup() 
{
      Serial.begin (57600);    
      //-----------------------------
       // turn on motor
      motorR.setSpeed(200);
      motorR.run(RELEASE);
     
      
      motorL.setSpeed(200);
      motorL.run(RELEASE);

     //---------------------------ROS Setup
      nh.advertise(enc_pub);  
      nh.subscribe(sub);      
      }


 void loop() { 
      
   //Right Encoder
     long RnewPosition = R_enc.read();
     if (RnewPosition != RoldPosition) {
          RoldPosition = RnewPosition; 

          
          Serial.println(RnewPosition);
        } 
        
  //----left encoder
  long LnewPosition = L_enc.read();
  if (LnewPosition != LoldPosition) {
      LoldPosition = LnewPosition; //update positions
      Serial.println(LnewPosition);
      }  
//-------end of encoder

//-----------------------ROS publishing  
        Point_msg.x=RnewPosition;
        Point_msg.y=LnewPosition;
        enc_pub.publish(&Point_msg);
//-------------        
      nh.spinOnce(); 
      delay(10);
 }
