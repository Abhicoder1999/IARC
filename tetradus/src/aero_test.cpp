	/***************************************************************************
 *                       Varanon Austin Pukasamsombut					   *
 *																		   *
 *                  AERO = Autonomous Exploration RObot                    *
 *                                 Test                                    *
 *                                                                         *
 * This code is designed to make a Quadcopter (with APM 2.6) autonomously  *
 * takeoff in ALT_HOLD mode, hover in the air for 20 seconds, and then     *
 * land by changing to LAND mode.                                          * 
 ***************************************************************************/

//Standard Include
#include "ros/ros.h"
#include <cstdlib>

//Include Message Types Used
#include <mavros_msgs/OverrideRCIn.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/VFR_HUD.h>
#include <mavros_msgs/RCIn.h>

#define mavros mavros_msgs

//Define Radio Channels to Numbers [May differ depending on RC Transmitter]
#define ROLL 0
#define PITCH 1
#define THROTTLE 2
#define YAW 3
#define LEFT_TRIGGER 5

//Define Checks
#define PRELIMINARY_CHECK 1
#define LAND_CHECK 2

//Define RC Throttle Values [Differs for all controllers]
#define MIDDLE 1500  //PWM Value with Stick at Middle 
#define RISING 1800 //PWM Value with Stick Slightly Above Middle
#define RELEASE 1000 //PWM Value when Stick is completely Lowered
#define NO_RC 900	 //PWM Value when no RC Controller has been connected.

#define HOV_TIME 5  //hovering time in seconds

//Class Define (Maybe Move to Separate File Later)
class Receiver
{
    public:
    	//Ctor initializes looping checks and vfr_ctr
    	Receiver();
    	
        //Callbacks
        void stateCallback(const mavros_msgs::State::ConstPtr& msg);
        void vfrCallback(const mavros_msgs::VFR_HUD::ConstPtr& msg);
        void rcCallback(const mavros_msgs::RCIn::ConstPtr& msg);
        
        int vfr_ctr;  //needed for ascertaining ground altitude wrt mean sea level
        
        //Looping Checks
        bool state_finished; //If False, then runs stateCallback on spin.
        bool vfr_finished;
        bool rc_finished;
        bool terminate; //Release All Channels and Terminate Program

        //Miscellaneous Variables
        int state_check;
        int vfr_check;
        int rc_check_ch; //Channel to Check
        int rc_check_val; //Value to Check
        float gnd_alt;  //to store ground altitude wrt mean sea level
};

Receiver::Receiver(void)
{
    state_finished = true;
    vfr_finished = true;
    rc_finished = true;
    terminate = false;
    vfr_ctr=0;
}

void Receiver::stateCallback(const mavros_msgs::State::ConstPtr& msg)
{
    if(terminate) return;
    if(state_finished) return;
    //ROS_INFO("ENTER: State Callback");

    bool check1, check2;

    if(state_check == PRELIMINARY_CHECK)
    {
    	 if(msg->armed)
        {
            ROS_INFO("Completed: ARMING");
            check2 = true;
        }
        else
        {
            ROS_INFO("Waiting: ARMING");
            check2 = false;
            system("rosrun mavros mavsafety arm");
        }
    
        if(msg->mode == "ALT_HOLD") 
        {
            ROS_INFO("Mode changed: ALT_HOLD");
            check1 = true;
        }
        else 
        {
            ROS_INFO("Waiting: ALT_HOLD");
            check1 = false;
            system("rosrun mavros mavsys mode -c ALT_HOLD");
        }
       
    
        if(check1 && check2) 
        {
            ROS_INFO("Completed: Preliminary Check");
            state_finished = true;
        }
    }//End Preliminary Check

    if(state_check == LAND_CHECK)
    {
        if(msg->mode == "LAND") 
        {
            ROS_INFO("Mode changed: LAND");
            state_finished = true;
        }
        else 
        {
            ROS_INFO("Waiting: LAND");
            system("rosrun mavros mavsys mode -c LAND");
        }
    }//End Landing Check
   
}
    
void Receiver::vfrCallback(const mavros_msgs::VFR_HUD::ConstPtr& msg)
{   vfr_ctr++;
    if(vfr_ctr==1)
    	gnd_alt=msg->altitude;
    if(terminate) return;
    if(vfr_finished) return;
   
    //ROS_INFO("Enter: VFR Callback");
    
    if(vfr_check == RISING)
    {
        if(msg->altitude-gnd_alt < 20)
        {
            ROS_INFO("Ascending...");
            vfr_finished = false;
        }
        else //Rises until the Altitude Goes Above 2.6
        {
            ROS_INFO("Completed: RISING");
            vfr_finished = true;
        }
    }
    if(vfr_check == RELEASE)
    {
        if(msg->altitude-gnd_alt > 0)
        {
            ROS_INFO("Descending...");
            vfr_finished = false;
        }
        else //Rises until the Altitude Goes Above 2.6
        {
            vfr_finished = true;
        }
    }
}


void Receiver::rcCallback(const mavros_msgs::RCIn::ConstPtr& msg)
{
    

    //Terminates all Programs if Left Trigger Moves: "Forced Landing" [Differs for All Controllers]
    if(msg->channels[LEFT_TRIGGER] > 1200) terminate = true;

    if(rc_finished) return;
    //ROS_INFO("Enter: RC Callback");
    if(msg->channels[rc_check_ch] <= (rc_check_val + 7)
            && msg->channels[rc_check_ch] >= (rc_check_val - 7)) //RC Values Fluctuates, so +-7 takes that into account.
    {
        rc_finished = true; //If RC_Value is close to Required Value.
        ROS_INFO("Completed: RC Change");
    }
    else 
        rc_finished = false;   
}


/***********************MAIN PROGRAM BEGINS HERE****************************/

int main(int argc, char **argv)
{
    ros::init(argc, argv, "aero_test"); //Initialization
    ros::NodeHandle nh; //Way to Connect Node to Master

    ROS_INFO("AERO Program: Test");

    //Setting Up All Subscribers and Publishers

    Receiver receiver;
    ros::Subscriber state_sub = nh.subscribe("/mavros/state", 1, &Receiver::stateCallback, &receiver);
    ros::Subscriber vfr_sub = nh.subscribe("/mavros/vfr_hud", 1, &Receiver::vfrCallback, &receiver);
    ros::Subscriber rc_sub = nh.subscribe("/mavros/rc/in", 1, &Receiver::rcCallback, &receiver);

   
    
    

    ros::Publisher rc_message = nh.advertise<mavros_msgs::OverrideRCIn>("/mavros/rc/override", 1, true);
    mavros_msgs::OverrideRCIn rc_command;
	
    //ROS_INFO("Subscribers and Publishers set");  
    //First Arm and Set to Altitude_Hold Mode
    ROS_INFO("Commencing: Preliminary Setup");    
    
    system("rosrun mavros mavsys mode -c ALT_HOLD");
    system("rosrun mavros mavparam set SYSID_MYGCS 1");
    system("rosrun mavros mavsafety arm");
    
    receiver.state_check = PRELIMINARY_CHECK;
    receiver.state_finished = false; //Calls State Callback
    while((!receiver.state_finished) && (ros::ok()) && (!receiver.terminate))
     {   ros::spinOnce();
     	 }

    ROS_INFO("Commencing: Take Off");
    
    //Then Push Throttle Up to Fly, Leaving All Others to RC Controller
    
    for(int i=0; i < 8; i++) rc_command.channels[i] = 0;//Releases all Channels First
    rc_command.channels[THROTTLE] = RISING; //Ascending Throttle

    receiver.rc_check_ch = THROTTLE;
    receiver.rc_check_val = RISING;
    receiver.rc_finished = false;
    while((!receiver.rc_finished) && (ros::ok()) && (!receiver.terminate)) 
    {
        rc_message.publish(rc_command);
    //   ROS_INFO("in loop");
        ros::spinOnce();
        
    }

    ROS_INFO("Completed: Take Off");
   
    //Continues until Altitude Reaches Certain Point (+ 1m)

    ROS_INFO("Commencing: Ascent");

    //receiver.landing = false;
    //receiver.alt_ground = 0.00;

    receiver.vfr_check = RISING;
    receiver.vfr_finished = false;
    while((!receiver.vfr_finished) && (ros::ok()) && (!receiver.terminate)) 
    {    ros::spinOnce();
        rc_message.publish(rc_command);
}
    ROS_INFO("Completed: Ascent");

    //Then Sets Throttle to Middle Value and Keeps Position for Set Time
    
    ROS_INFO("Commencing: Hover");

    rc_command.channels[THROTTLE] = MIDDLE; //Keeps Altitude
    
    receiver.rc_check_ch = THROTTLE;
    receiver.rc_check_val = MIDDLE;
    receiver.rc_finished = false;
    while((!receiver.rc_finished) && (ros::ok()) && (!receiver.terminate))
    {
        ros::spinOnce();
        rc_message.publish(rc_command);
    }

    ros::Time begin = ros::Time::now();
    double secs = 0; //Set to Hover for 20 seconds
    while((secs < HOV_TIME) && (ros::ok()) && (!receiver.terminate))
    {
        ros::spinOnce();
        ros::Duration waiting = ros::Time::now() - begin;
        secs = waiting.toSec();
        rc_message.publish(rc_command);
        ROS_INFO("Waiting: Hover");
    }

    ROS_INFO("Completed: Hover");

    //Changes mode to LAND and Begins Landing
    
    ROS_INFO("Commencing: Landing");
   
    if((!receiver.terminate) && (ros::ok())) 
        system("rosrun mavros mavsys mode -c LAND");    
    receiver.state_check = LAND_CHECK;
    receiver.state_finished = false; //Calls State Callback
    while((!receiver.state_finished) && (ros::ok()) && (!receiver.terminate))
        ros::spinOnce();

    ROS_INFO("Commencing: Throttle Release");

    //Releases RC Override Controls and Exits Program
   
    for(int i=0; i < 8; i++) rc_command.channels[i] = 0;

    receiver.rc_check_ch = THROTTLE;
    receiver.rc_check_val = RELEASE;
    receiver.rc_finished = false;
    while((!receiver.rc_finished) && (ros::ok())) 
    {
        ros::spinOnce();
        rc_message.publish(rc_command);
    }
     receiver.vfr_check = RELEASE;
    receiver.vfr_finished = false;
    while((!receiver.vfr_finished) && (ros::ok()) && (!receiver.terminate)) 
    {    ros::spinOnce();
        rc_message.publish(rc_command);
    }
    ROS_INFO("Completed: Landing");

    if(!receiver.terminate)
    {
        system("rosrun mavros mavsafety disarm");
        ROS_INFO("Completed: DISARMED");
        ROS_INFO("Program Completed");
    }
    if(receiver.terminate)
        ROS_INFO("FORCED TERMINATION INVOKED");

    

    return 0;
}
