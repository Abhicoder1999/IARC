#include<image_enhancer/filter.h>

ImageEnhancer::ImageEnhancer(int p = 10, int m = 4){
  // cout<<"constructed\n";
  target_param = p;
  param = 0;
  min_param = m;
  counter = 0;
}

ImageEnhancer::~ImageEnhancer(){
  std::cout << "destructed" << '\n';
}

Mat ImageEnhancer::glareRemovalMinMedian(Mat src){
  Mat fin_src = src.clone();
  return fin_src;

}


// fps drop :(
Mat ImageEnhancer::glareRemovalMedian(Mat src){
  // cout<<"median\n";
  if(buff.size()<8){
    buff.push_back(src.clone());
    return src;
  }
  else{
    buff.pop_front();
    buff.push_back(src.clone());
  }
  Mat fin_im = src.clone();

    for(int x=0; x<src.cols; x++){
      for(int y=0; y<src.rows; y++){
        std::vector<int> b,g,r;
        for(int i=0; i<buff.size(); i++){
          Vec3b bgr = buff[i].at<Vec3b>(y, x);
          put_(b,bgr[0]);
          put_(g,bgr[1]);
          put_(r,bgr[2]);
        }
        Vec3b &new_bgr = fin_im.at<Vec3b>(y,x);
        new_bgr[0] = b[4];
        new_bgr[1] = g[4];
        new_bgr[2] = r[4];
      }
    }
    return fin_im;
}

void ImageEnhancer::put_(std::vector<int> &v, int val){
  for(int i=0; i<v.size(); i++){
    if(val<=v[i]){
      v.insert(v.begin()+i, val); return;
    }
  }
  v.push_back(val);
}


void ImageEnhancer::errorMsg(int flag = 0){
  cout<<"error occured\n";
}


void ImageEnhancer::glareRemoval(Mat src){

}

// color enhancment functions --------------------------------------------------

// working fine
Mat color_enhancer(Mat src1){
  Mat src = src1.clone();

  int maxp[3] = {0,0,0}, minp[3] = {255,255,255};
  for(int x=0; x<src.cols; x++){
    for(int y=0; y<src.rows; y++){
      Vec3b bgrPixel = src.at<Vec3b>(y, x);
      for(int i=0; i<3; i++){
        if(bgrPixel[i]>maxp[i]) maxp[i] = bgrPixel[i];
        if(bgrPixel[i]<minp[i]) minp[i] = bgrPixel[i];
      }
    }
  }
  int max_range[3] = {255, 255, 255}; // use to give color range
  for(int x=0; x<src.cols; x++){
    for(int y=0; y<src.rows; y++){
      Vec3b &bgrPixel = src.at<Vec3b>(y, x);
      for(int i=0; i<3; i++)
      bgrPixel[i] = ((bgrPixel[i]-(int)minp[i])*max_range[i])/((int)maxp[i] - (int)minp[i]);
      // bgrPixel[i] = ((bgrPixel[i]-(int)minp[i])*255)/((int)maxp[i] - (int)minp[i]);
    }
  }
return src;
}

// improve contrast bt CLAHE
Mat contrast_enhancer(Mat src){
  cv::Mat bgr_image = src.clone();
  cv::Mat lab_image;
  cv::cvtColor(bgr_image, lab_image, CV_BGR2Lab);

  // Extract the L channel
  std::vector<cv::Mat> lab_planes(3);
  cv::split(lab_image, lab_planes);  // now we have the L image in lab_planes[0]

  // apply the CLAHE algorithm to the L channel
  cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
  clahe->setClipLimit(4);
  cv::Mat dst;
  clahe->apply(lab_planes[0], dst);

  // Merge the the color planes back into an Lab image
  dst.copyTo(lab_planes[0]);
  cv::merge(lab_planes, lab_image);

 // convert back to RGB
 cv::Mat image_clahe;
 cv::cvtColor(lab_image, image_clahe, CV_Lab2BGR);

  return image_clahe;
}
