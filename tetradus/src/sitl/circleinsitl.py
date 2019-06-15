#!/usr/bin/env python
# license removed for brevity
import rospy
import mavros
from geometry_msgs.msg import Twist
from sensor_msgs.msg import NavSatFix
import math
Kp=5
pub = rospy.Publisher('/Quad1/mavros/setpoint_velocity/cmd_vel_unstamped', Twist, queue_size=10)
#Twist vel_sent=NULL
#NavSatFix vel_sent=None
def callback(data):
	print("Entered callback")
	global curr_lat,curr_lon,radius,lat,lon
	curr_lat=data.latitude
	curr_lon=data.longitude
	print(curr_lat)
	print(curr_lon)
	x=(curr_lon-lon)
	y=(curr_lat-lat)
	theta=math.atan2(y,x)
	vel=Kp#*((y**2 + x**2)**0.5)
	if x==0 and y==0:
		xvel=0
		yvel=0
	else:
		yvel=vel*math.cos(theta)
		xvel=-vel*math.sin(theta)
		    	
	vel_sent=Twist()
	vel_sent.linear.x=xvel
	vel_sent.linear.y=yvel
	vel_sent.linear.z=0
	vel_sent.angular.x=0
	vel_sent.angular.y=0
	vel_sent.angular.z=0

	radius=(float(x)**2 + y**2)**0.5
	print("Radius: ",radius)	
	rospy.loginfo(vel_sent)
	pub.publish(vel_sent)
	
	
	

def listener():
	print("Entered listener")
	rospy.init_node('sitl', anonymous=True)
	rospy.Subscriber("/Quad1/mavros/global_position/global", NavSatFix, callback)
	rospy.spin()


if __name__ == '__main__':
	
	try:
		lat=input("Enter centre point latitude: ")
		lon=input("Enter centre point longitude: ")
		#talker()
		listener()

	except rospy.ROSInterruptException:
        	pass
