import cv2
import numpy as np

class GlareFilter:
    def __init__(self,p=8,m=4):
        self.param = p
        self.min_param = m
        self.buff_param = []
        self.buff_min = []

    def median_filter(self,src):
        if(len(self.buff_param)<self.param):
            self.buff_param.append(src)
        else:
            self.buff_param.pop(0)
            self.buff_param.append(src)
        avg_img = np.median(self.buff_param[0:len(self.buff_param)],axis=0).astype(np.uint8)
        return avg_img

    def min_median_filter(self,src):
        if(len(self.buff_min)<self.min_param):
            self.buff_min.append(src)
        else:
            self.buff_min.pop(0)
            self.buff_min.append(src)
        min_img = self.buff_min[0]
        for i in range(1,len(self.buff_min)):
            min_img = np.minimum(min_img,self.buff_min[i])
        return self.median_filter(min_img)
