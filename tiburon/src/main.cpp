#include "image_enhancer/filter.h"

int main(int argc, char const *argv[]) {
  cout<<"started\n";
  ImageEnhancer *glare = new ImageEnhancer(10,4);
  Mat fin_frame,frame;
  VideoCapture cap("/home/Documents/catkin_ws/src/image_enhancer/robosub_gate.avi"); // for color enhancment test
  // VideoCapture cap("/home/devid/Videos/video3.avi");  // for glare test
  while (cap.isOpened()) {
    cap>>frame;
    imshow("original",frame);
    waitKey(30);
    // // glare removal
    // fin_frame = glare->glareRemovalMedian(frame);
    // imshow("glareRemoval",fin_frame);
    // for color enhancement
    imshow("contrast_increased",contrast_enhancer(frame));
    imshow("only streched",color_enhancer(frame));
    imshow("streched then contrast increased",contrast_enhancer(color_enhancer(frame)));

    waitKey(1);
  }
  return 0;
}
