
 #include <ros.h>
 #include <geometry_msgs/Point32.h>
 #include<geometry_msgs/Twist.h>
 #include <Stepper.h>
 //-----------------------------------------Robot parameters definition------------ 
 #define L 0.15
 #define R 0.05
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
//---------------------------------------------------------------------------------
 int Rcounter = 0; 
 int RaState;
 int RaLastState;  
//-----

int Lcounter = 0; 
int LaState;
int LaLastState;  
//--------------------------------Motors VARS-----------------------------------
float vel=0.0; //as Twist msgs depend on Vector3 which is float64
float omega=0.0;
float VR,VL;
//----------------------------------Stepper Motors -----------------------------------------
float Svel=0.0,Somega=0.0;


//---right stepper----

const int RstepsPerRevolution = 200;
Stepper RmyStepper(RstepsPerRevolution,12,13);
int RmotorSpeed=0;
int RstepCount = 0; // number of steps the motor has taken

//----left stepper----
/*const int LstepsPerRevolution = 200;
Stepper LmyStepper(LstepsPerRevolution, 4, 5, 6, 7);
int LmotorSpeed=0;
int LstepCount = 0; // number of steps the motor has taken
*/
//-----------------------------------------------------------------------------------------
ros::NodeHandle  nh;

//----------
//------------------------------Publish init----------------------------------------------
geometry_msgs::Point32 Point_msg;
ros::Publisher enc_pub("/encoder", &Point_msg);
//--------------------------------------------------

// THERE IS A PROBLEM IN STEPPER FUNCTION THAT RESETS OR DELAYS AND INTERFER ROS EXECUTION
//------------------------stepper funvtion------------------------------------ 
/*void stepper_cb(const geometry_msgs::Twist& msg)
{
  Svel=msg.linear.x; //stepper Velocity 
Somega=msg.angular.z; //stepper Omega 
  
  
  RmotorSpeed=(2*Svel+Somega*L)/(2*R);
if (RmotorSpeed > 0) {
    RmyStepper.setSpeed((int)msg.linear.x);
    // step 1/100 of a revolution:
    RmyStepper.step(RstepsPerRevolution / 100);
}
//---------------------------------------------

  LmotorSpeed=(2*Svel-Somega*L)/(2*R);
if (LmotorSpeed > 0) {
    LmyStepper.setSpeed(LmotorSpeed);
    // step 1/100 of a revolution:
    LmyStepper.step(LstepsPerRevolution / 100);


}


}
*/
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

//--------------Stepper part inside the callback function
 /*RmotorSpeed=(2*vel+omega*L)/(2*R);
//if (RmotorSpeed > 0) {
    RmyStepper.setSpeed((int)vel);
    // step 1/100 of a revolution:
    RmyStepper.step(RstepsPerRevolution / 100);*/
//}
//---------------------------------------------

//disable it now for the sake of expirements
 /* LmotorSpeed=(2*Svel-Somega*L)/(2*R);
if (LmotorSpeed > 0) {
    LmyStepper.setSpeed(LmotorSpeed);
    // step 1/100 of a revolution:
    LmyStepper.step(LstepsPerRevolution / 100);


}
*/

}
//--------------------subscribers---------------------------
ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &motors_cb);
//ros::Subscriber<geometry_msgs::Twist> stepsub("/cmd_vel", &stepper_cb);
//stepper will also use the same topic with the same object if sub and it will le
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
