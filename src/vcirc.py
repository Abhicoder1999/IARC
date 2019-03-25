#!/usr/bin/env python
# license removed for brevity
import rospy
import mavros
from geometry_msgs.msg import Twist
from sensor_msgs.msg import NavSatFix
import math
Kp=5
pub = rospy.Publisher('/Quad2/mavros/setpoint_velocity/cmd_vel_unstamped', Twist, queue_size=10)
callbackctr=0
phi=0

def callback(data):
	print("Entered callback")
	global curr_lat,curr_lon,curr_alt,lat,lon,alt,callbackctr,phi
	curr_lat=data.latitude
	curr_lon=data.longitude
	curr_alt=data.altitude-603.53
	#print(curr_lat)
	#print(curr_lon)
	print("Current altitude: ",curr_alt)
	phi=math.atan2(z,(x**2 + y**2)**0.5)
	x=curr_lon-lon
	y=curr_lat-lat
	z=curr_alt-alt
	x=x/math.cos(phi)
	y=y/math.cos(phi)
	print("Height above centre: ",z)
	print("Phi: ",phi)
	theta=math.atan2(y,x)
	vel=Kp#*((y**2 + x**2)**0.5)
	if x==0 and y==0:
		xvel=0
		yvel=0
		
	else:
		yvel=vel*math.cos(theta)*math.cos(phi)
		xvel=-vel*math.sin(theta)*math.cos(phi)
	zvel=math.fabs(vel*math.cos(theta)*math.sin(phi)-vel*math.sin(theta)*math.sin(phi))
	
	if z>0:
		zvel=-zvel
		
		    	
	vel_sent=Twist()
	vel_sent.linear.x=xvel
	vel_sent.linear.y=yvel
	vel_sent.linear.z=zvel
	vel_sent.angular.x=0
	vel_sent.angular.y=0
	vel_sent.angular.z=0
	callbackctr=callbackctr+1

	radius=(float(x)**2 + y**2)**0.5
	print("Radius: ",radius)
	print("Phi: ",phi)	
	rospy.loginfo(vel_sent)
	pub.publish(vel_sent)
	callbackctr=callbackctr+1
	
	
	

def listener():
	print("Entered listener")
	rospy.init_node('sitl', anonymous=True)
	rospy.Subscriber("/Quad2/mavros/global_position/global", NavSatFix, callback)
	rospy.spin()


if __name__ == '__main__':
	
	try:
		lat=input("Enter centre point latitude: ")
		lon=input("Enter centre point longitude: ")
		alt=input("Enter centre point altitude: ")
		#talker()
		listener()

	except rospy.ROSInterruptException:
        	pass
