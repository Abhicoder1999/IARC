
#include <fstream>
#include <ros/ros.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/CommandTOL.h>
#include <geometry_msgs/PoseStamped.h>
using namespace std;

geometry_msgs::PoseStamped current_position;

void local_position(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
        current_position = *msg;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "set_point");
    ros::NodeHandle nh;

    ros::Subscriber pose_sub = nh.subscribe<geometry_msgs::PoseStamped>("mavros/local_position/pose", 10, local_position);
    ros::Publisher pose_pub = nh.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_raw/local",1000);


    return 0;
}
