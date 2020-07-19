
 #include<ros.h>
 #include<geometry_msgs/Point32.h>
 #include<geometry_msgs/Twist.h>
 //-----------------------------------------Robot parameters definition------------ 
 #define L 0.15 //wheel separation distance
 #define R 0.05 //Wheel radius
 //------------------------------------------Encoders Pin Definitions--------------
 #define RoutputA 3
 #define RoutputB 2
//-------------------------
#define LoutputA 9
#define LoutputB 8
//----------------------------------------------------------------------------------

//-----------------------------Motors PIN deinitions------------------------------
#define RPWM 5
#define RDir 6

#define LPWM 10
#define LDir 11
//----------------------------Encoder counter variable -----------------------------------------------------
int Rcounter = 0; //Right Encoder count container
int RaState;
int RaLastState;  
//-----
int Lcounter = 0; //Left Encoder count Container
int LaState;
int LaLastState;  
//--------------------------------Motors VARS-----------------------------------
float vel=0.0; // Twist msgs depend on Vector3 which is float64
float omega=0.0;
float VR,VL;
//-----------------------------------------------------------------------------------------
ros::NodeHandle  nh;

//------------------------------Encoder count Publish init----------------------------------------------
geometry_msgs::Point32 Point_msg;
ros::Publisher enc_pub("/encoder", &Point_msg);
//--------------------------------------------------
//-----------------------------------DC Motors Callback subscribers
void motors_cb(const geometry_msgs::Twist& msg)
{

vel=msg.linear.x;
omega=msg.angular.z;

VR=(2*vel+omega*L)/(2*R);// take care it 
VL=(2*vel-omega*L)/(2*R);

//--------------------------Right Motor Command------
if (VR>0)
{
  digitalWrite(RDir,HIGH);// Forward
  analogWrite(RPWM,VR);// I will change it to vel 
}
else if (VR<0)
{
  digitalWrite(RDir,LOW);// backward
  analogWrite(RPWM,VR);
}
else
{
  analogWrite(RPWM,255);// 255 means lowest velocity
}


//------------------Left Motor Command--------------------- 
if (VL>0)
{
  digitalWrite(LDir,HIGH);// Forward
  analogWrite(LPWM,VL);
}
else if (VL<0)
{
  digitalWrite(LDir,LOW);// backward
  analogWrite(LPWM,VL);
}
else
{
  analogWrite(LPWM,255);// 255 means lowest velocity
}


}
//--------------------subscribers---------------------------
ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &motors_cb);
//-------------
 void setup() 
{
Serial.begin (57600);    
//-----------------------encoder setup----------------------   
pinMode (RoutputA,INPUT);
pinMode (RoutputB,INPUT);
//-------------   
pinMode (LoutputA,INPUT);
pinMode (LoutputB,INPUT);
      
// Reads the initial state of the outputA of left and right encoder
RaLastState = digitalRead(RoutputA);
LaLastState = digitalRead(LoutputA);
//------------------------------Motor Setup-----------------
pinMode( RPWM, OUTPUT );
pinMode( RDir, OUTPUT );
digitalWrite( RPWM, LOW );
digitalWrite( RDir, LOW );
//--------------------------------------
pinMode( LPWM, OUTPUT );
pinMode( LDir, OUTPUT );
digitalWrite(LPWM, LOW );
digitalWrite(LDir, LOW );

//---------------------------ROS Setup
nh.advertise(enc_pub);
nh.subscribe(sub);
//nh.subscribe(stepsub);
 } 

 void loop() { 
   RaState = digitalRead(RoutputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (RaState != RaLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(RoutputB) != RaState) { 
       Rcounter ++;
     } else {
       Rcounter --;
     }
     Serial.print("RPosition: ");
     Serial.println(Rcounter);
   } 
   RaLastState = RaState; // Updates the previous state of the outputA with the current state
 //-----------again donkey work for Left
 
    LaState = digitalRead(LoutputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (LaState != LaLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(LoutputB) != LaState) { 
       Lcounter ++;
     } else {
       Lcounter --;
     }
     Serial.print("LPosition: ");
     Serial.println(Lcounter);
   } 
   LaLastState = LaState; // Updates the previous state of the outputA with the current state
//----------------------MOTORS Part 
//DC and Steper are in the subscribers functions..
 
//-----------------------ROS publishing  
Point_msg.x=Rcounter;
Point_msg.y=Lcounter;
enc_pub.publish(&Point_msg);
nh.spinOnce();
 }
