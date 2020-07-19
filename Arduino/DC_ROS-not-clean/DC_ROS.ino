
 #include <ros.h>
 #include <geometry_msgs/Point32.h>
 #include<geometry_msgs/Twist.h>
// --stepper definitons
#define TYPE DOUBLE
#define PI 3.14
#include <AFMotor.h>
#include <Encoder.h>

//----- DC Motor definitions------------
AF_DCMotor motorL(4);
AF_DCMotor motorR(3);

//-------------Encoder definitions
Encoder R_enc(19, 18);
Encoder L_enc(21, 20);
long RoldPosition  = -999;
long LoldPosition  = -999;
 //-----------------------------------------Robot parameters definition------------ 
 #define L 0.33
 #define R 0.05
//--------------------------------Motors VARS-----------------------------------
float vel=0.0; //as Twist msgs depend on Vector3 which is float64
float omega=0.0;
float VR,VL;
//-------------------pot vars---------
int i=0;


//-----------------------------------------------------------------------------------------
ros::NodeHandle  nh;

//----------
//------------------------------Publish init----------------------------------------------
geometry_msgs::Point32 Point_msg;
ros::Publisher enc_pub("/encoder", &Point_msg);
//--------------------------------------------------

//-----------------------------------DC Motors Callback subscribers
void motors_cb(const geometry_msgs::Twist& msg)
{

    vel=msg.linear.x;
    omega=msg.angular.z;
    
    VR=(2*vel+omega*L)/(2*R);// take care of it 
    VL=(2*vel-omega*L)/(2*R);
    
    
       motorR.run(FORWARD);
       motorR.setSpeed(VR);
      //---left---
      motorL.run(FORWARD);
      motorL.setSpeed(VL);
      
 //-------------negative part     
    if (VR<0)
    {
          motorR.run(BACKWARD);
          motorR.setSpeed(abs(VR));
          //----
    }
    if (VL<0){
          motorL.run(BACKWARD);
          motorL.setSpeed(abs(VL));
    }




}
//--------------------subscribers---------------------------
ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &motors_cb);

//-------------
 void setup() 
{
      Serial.begin (57600);    
      //-----------------------------
       // turn on motor
      motorR.setSpeed(200);
      motorR.run(RELEASE);
     
      
      motorL.setSpeed(200);
      motorL.run(RELEASE);
      //---------------------------potintiometer ---------
      pinMode(A8,OUTPUT);
      pinMode(A10,INPUT);
      pinMode(A12,OUTPUT);
      digitalWrite(A8,HIGH);
      digitalWrite(A12,LOW);
      
      //---------------------------ROS Setup
      nh.advertise(enc_pub);
      nh.subscribe(sub);
      
 } 

 void loop() { 
      
   //----Right Encoder
     long RnewPosition = R_enc.read();
        if (RnewPosition != RoldPosition) {
          RoldPosition = RnewPosition;
           
           
          Serial.println(RnewPosition);
        } 
        
  //----left encoder

      long LnewPosition = L_enc.read();
        if (LnewPosition != LoldPosition) {
          LoldPosition = LnewPosition;
          Serial.println(LnewPosition);
        }  
//-------end of encoder

//--just for testing motors
//i=map(analogRead(A10),0,1023,0,255);
//i=78;
//Serial.println("Pot value:");
//Serial.println(i);

     /* motorR.run(FORWARD);
       motorR.setSpeed(i);
      //---left---
      motorL.run(FORWARD);
      motorL.setSpeed(i);
      */
      //----------


//-----------------------ROS publishing  
        Point_msg.x=RnewPosition;
        Point_msg.y=LnewPosition;
        enc_pub.publish(&Point_msg);
//-------------        
      nh.spinOnce();
      delay(10);
 }
