#include <ros/ros.h>
#include <std_msgs/String.h>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <iostream>
#include <vector>
#include <string>
typedef unsigned char BYTE;
using namespace std;
using namespace cv;

const int IMG_WIDTH = 648;
const int IMG_HEIGHT = 488;

Mat frontFinal;

static inline bool is_base64(BYTE c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

vector<BYTE> base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  BYTE char_array_4[4], char_array_3[3];
  std::vector<BYTE> ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
          ret.push_back(char_array_3[i]);
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
  }

  return ret;
}
void callback(const std_msgs::String::ConstPtr& msg)
{
 ROS_INFO("entered call back");
 string s = msg->data;
 /*int n = s.length();
 char buffer[n+1];
 strcpy(buffer, s.c_str());

char* camImages;

    camImages = strtok(buffer, "!");
    camImages = strtok(NULL, "!");
    char* frontimg = camImages;
    string fimg(frontimg);

*/

vector<BYTE> decodedData_ = base64_decode(s);
Mat front_initial= imdecode(decodedData_, CV_LOAD_IMAGE_COLOR);
Mat channels_f[3];
split(front_initial,channels_f);
vector<Mat> frontimage;
frontimage.push_back(channels_f[0]);
frontimage.push_back(channels_f[1]);
frontimage.push_back(channels_f[2]);

merge(frontimage, frontFinal);
imshow("Front", frontFinal);
waitKey(1);

}

int main(int argc, char **argv)
{
    ROS_INFO("entered main");
    ros::init(argc,argv,"iarc");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher frontImagePub;
    sensor_msgs::ImagePtr frontMsg;
    ros::Subscriber sub = nh.subscribe("/images",1,callback);
    frontImagePub = it.advertise("/camera/image", 1);
    

    while(ros::ok())
    {
       frontMsg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frontFinal).toImageMsg();
       frontImagePub.publish(frontMsg);
       ros::spinOnce();
    }
}
