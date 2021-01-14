#include "ros/ros.h"
#include "std_msgs/Int32.h"
//#include <curses.h>

using namespace std;


int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "dir2");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<std_msgs::Int32>("direction", 1000);
  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    
    std_msgs::Int32 msg;
   // msg.data=getch();
    ROS_INFO("%d", msg.data);
    chatter_pub.publish(msg);
    //while(getch());
    msg.data=0
    ROS_INFO("%d", msg.data);
   // chatter_pub.publish(msg);
  }


  return 0;
}


