
#include <fstream>
#include <ros/ros.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
using namespace std;

mavros_msgs::State current_state;

void state_cb(const mavros_msgs::State::ConstPtr& msg){
    current_state = *msg;
}

int main(int argc, char **argv)
{   //Variables
    ros::init(argc, argv, "mode");
    ros::NodeHandle nh;
    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("mavros/state", 10, state_cb);
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");

    //Connection check
    ros::Rate rate(20.0);
    while(ros::ok() && !current_state.connected){
    ros::spinOnce();
    rate.sleep();
    }
    ROS_INFO("Successfully Connected");

    //Mode Setting
    mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = argv[1];

    if( set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent){
        ROS_INFO("mode enabled");
    }

    return 0;
}
