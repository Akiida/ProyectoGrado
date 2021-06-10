#!/usr/bin/env python
import numpy as np
import datetime
import time
import serial
import threading 
import logging
from os import system
import rospy
from std_msgs.msg import String

import tf
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point, Pose, Quaternion, Twist, Vector3



def talker():

	pub = rospy.Publisher('/cmd_vel', Twist, queue_size=1)
	rospy.init_node('Velocidades', anonymous=True)
	
	move_cmd = Twist()
	move_cmd.linear.x = 0.1
	move_cmd.angular.z = 0

	now = rospy.Time.now()
	rate = rospy.Rate(10)

	while not rospy.is_shutdown():
		pub.publish(move_cmd)
		rate.sleep()

if __name__ == '__main__':
	try:
		talker()
	except rospy.ROSInterruptException:
		pass
