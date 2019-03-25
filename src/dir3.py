#!/usr/bin/env python3
import rospy
import keyboard
from std_msgs.msg import Int32

def talker():
	pub = rospy.Publisher('direction', Int32, queue_size=10)
	rospy.init_node('iarc', anonymous=True)
	rate = rospy.Rate(10) # 10hz
	while not rospy.is_shutdown():
		if keyboard.is_pressed('b'):
			dir_no=8
		else:
			dir_no=6
		pub.publish(dir_no)
		rate.sleep()

if __name__ == '__main__':
	try:
		talker()
	except rospy.ROSInterruptException:
		pass


