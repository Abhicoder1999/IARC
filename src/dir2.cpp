#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <curses.h>

using namespace std;


int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "dir2");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<std_msgs::Int32>("direction", 1000);
  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    std_msgs::Int32 msg;
    timeout(-1);
    msg.data=getch();
    
    cout<<"data: "<<msg.data<<'\n';
    
    chatter_pub.publish(msg);
     
    timeout(1);
    while(getch()!=msg.data);
    cout<<"Came out of while"<<'\n';
    msg.data=0;
    
    cout<<"data: "<<msg.data<<'\n';
    
    chatter_pub.publish(msg);
    
  }


  return 0;
}


