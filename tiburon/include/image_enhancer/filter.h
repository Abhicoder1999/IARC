#ifndef IMAGE_ENHANCER
#define IMAGE_ENHANCER
#include <iostream>
#include <limits.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <cmath>
#include <queue>
#include <deque>
#include <vector>
#include <algorithm>

#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/core/core.hpp>

#define PI 3.14159265

using namespace cv;
using namespace std;

class ImageEnhancer
{
  public:
    ImageEnhancer(int p,int m);
    ~ImageEnhancer();
    void errorMsg(int);

    // Mat arr[10];
    int counter;

    // queue<Mat> buff;
    deque<Mat> buff;
    vector<Mat> median_buffer,min_buffer; //Mat new_src;
    int param, min_param, target_param; // not in use now
    vector<int> pixelB, pixelG, pixelR; // not in use now

    Mat glareRemovalMedian(Mat src);
    void glareRemoval(Mat src);
    Mat glareRemovalMinMedian(Mat src);
    void put_(std::vector<int> &v, int val);


};
#endif

// color_enhancer -----------------------------------------------------------

Mat color_enhancer(Mat src);
Mat contrast_enhancer(Mat src);

// end ----------------------------------------------------------------------
