import sys
sys.path.remove('/opt/ros/kinetic/lib/python2.7/dist-packages')
import cv2
sys.path.append('/opt/ros/kinetic/lib/python2.7/dist-packages')


import cv2
import os
from glare_filter import *

vd = cv2.VideoCapture("./video6.avi")
#object created accpets 2 inputs: GlareFilter(median_param=8, minimum =4)
g_filter = GlareFilter(8,4)
g_filter2 = GlareFilter()
while vd.isOpened():
    res,frame=vd.read()
    cv2.imshow("original",frame)
    k=cv2.waitKey(10)
    if(k==27):
        break
    cv2.imshow("final_Min_median",g_filter.min_median_filter(frame))
    cv2.imshow("final_median",g_filter2.median_filter(frame))
