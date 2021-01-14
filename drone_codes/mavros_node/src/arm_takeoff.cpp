#include <fstream>
#include <ros/ros.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/CommandTOL.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
using namespace std;


int main(int argc, char **argv)
{
    ros::init(argc, argv, "arm_takeoff");
    ros::NodeHandle nh;
    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");
    ros::ServiceClient takeoff_client = nh.serviceClient<mavros_msgs::CommandTOL>("/mavros/cmd/takeoff");

    //Arming the Drone
    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value = true;

    if( arming_client.call(arm_cmd) && arm_cmd.response.success)
        ROS_INFO("Vehicle armed");
    else
        ROS_INFO("Arming Failed");
    // Takeoff

    mavros_msgs::CommandTOL apm_takeoff;
    apm_takeoff.request.altitude = 40;
    apm_takeoff.request.latitude = 0;
    apm_takeoff.request.longitude = 0;
    apm_takeoff.request.min_pitch = 0;
    apm_takeoff.request.yaw = 0;

    if(takeoff_client.call(apm_takeoff) && apm_takeoff.response.success)
       ROS_INFO("Takeoff Successful");
    else
       ROS_INFO("Failed Takeoff");

    return 0;
}
