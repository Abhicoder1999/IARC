#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <curses.h>

using namespace std;


int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "dir");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<std_msgs::Int32>("direction", 1000);
  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    
    std_msgs::Int32 msg;
    cin>>msg.data;
    

    ROS_INFO("%d", msg.data);

    chatter_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
    
  }


  return 0;
}




