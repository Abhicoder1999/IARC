#!/usr/bin/env python
# license removed for brevity
import rospy
import mavros
from geometry_msgs.msg import Twist
from sensor_msgs.msg import NavSatFix
import math
Kp=2.5
pub = rospy.Publisher('/Quad1/mavros/setpoint_velocity/cmd_vel_unstamped', Twist, queue_size=10)
#Twist vel_sent=NULL
#NavSatFix vel_sent=None
def callback(data):
	print("Entered callback")
	global curr_lon,curr_alt,radius,lon,alt
	curr_alt=data.altitude-603.46
	curr_lon=data.longitude
	print(curr_alt)
	print(curr_lon)
	x=(curr_lon-lon)*111132.954*math.cos(math.radians(lat))
	z=(curr_alt-alt)
	
	theta=math.atan2(z,x)
	print("Theta: ",math.degrees(theta))
	print("z: ",z)
	print("x: ",x)
	vel=Kp#*((y**2 + x**2)**0.5)
	if x==0 and y==0:
		xvel=0
		zvel=0
	else:
		zvel=vel*math.cos(theta)
		xvel=-vel*math.sin(theta)
		
		    	
	vel_sent=Twist()
	vel_sent.linear.x=xvel
	vel_sent.linear.z=zvel
	vel_sent.linear.y=0
	vel_sent.angular.x=0
	vel_sent.angular.y=0
	vel_sent.angular.z=0

	radius=(float(x)**2 + z**2)**0.5
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
		alt=input("Enter centre point altitude: ")
		lon=input("Enter centre point longitude: ")
		lat=input("Enter centre point latitude: ")
		#talker()
		listener()

	except rospy.ROSInterruptException:
        	pass
