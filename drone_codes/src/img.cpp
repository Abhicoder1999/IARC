#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>


int main(int argc, char **argv)
{
	ros::init(argc, argv, "img");
	ros::NodeHandle nh;

    return 0;
}
