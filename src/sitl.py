#!/usr/bin/env python
# license removed for brevity
import rospy
import mavros
from geometry_msgs.msg import Twist
from sensor_msgs.msg import NavSatFix

lat=0
lon=0
Kp=10000
pub = rospy.Publisher('/Quad1/mavros/setpoint_velocity/cmd_vel_unstamped', Twist, queue_size=10)
#Twist vel_sent=NULL
#NavSatFix vel_sent=None
def callback(data):
	print("Entered callback")
	global curr_lat,curr_lon,lat,lon
	curr_lat=data.latitude
	curr_lon=data.longitude
	print(curr_lat)
	print(curr_lon)
	x=lon-curr_lon
	y=lat-curr_lat
	vel=Kp*((y**2 + x**2)**0.5)
	if x==0 and y==0:
		xvel=0
		yvel=0
	else:
		yvel=vel*y/((y**2 + x**2)**0.5)
		xvel=vel*x/((y**2 + x**2)**0.5)
		    	
	vel_sent=Twist()
	vel_sent.linear.x=xvel
	vel_sent.linear.y=yvel
	vel_sent.linear.z=0
	vel_sent.angular.x=0
	vel_sent.angular.y=0
	vel_sent.angular.z=0
		
	rospy.loginfo(vel_sent)
	pub.publish(vel_sent)
	
	
def callbacksec(data):
	global lat,lon
	lat=data.latitude
	lon=data.longitude
	

def listener():
	print("Entered listener")
	rospy.init_node('sitl', anonymous=True)
	rospy.Subscriber("/Quad1/mavros/global_position/global", NavSatFix, callback)
	rospy.Subscriber("/Quad2/mavros/global_position/global", NavSatFix, callbacksec)
	rospy.spin()

if __name__ == '__main__':
	
	try:
		#lat=input("Enter latitude: ")
		#lon=input("Enter longitude: ")
		#curr_lat=lat
		#curr_lon=lon
		#talker()
		listener()

	except rospy.ROSInterruptException:
        	pass
