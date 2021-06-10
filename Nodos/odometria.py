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

def callback(data):
	global Wd

	entrada = data.data.split()

	Wd	= np.array([[float(entrada[0])*2*3.141592653589793238],
					[float(entrada[1])*2*3.141592653589793238],
					[float(entrada[2])*2*3.141592653589793238], 
					[float(entrada[3])*2*3.141592653589793238]])


def calculo():
	global Wd, velX, velY, w, xr, yr, phi

	Wd	= np.array([[0],
					[0],
					[0], 
					[0]])


	a=0.145
	b=0.115
	R=0.04

	T = np.array([[1, -1, -(a+b)],
		[1, 1, (a+b)],
		[1, 1, -(a+b)], 
		[1, -1, (a+b)]])*(1/R)


	velX = 0
	velY = 0
	w = 0

	xr = 0
	yr = 0
	phi = 0

	while not rospy.is_shutdown():

		VelocidadesRelativas = np.dot(np.linalg.pinv(T),Wd)

		velX = float(VelocidadesRelativas[0])
		velY = float(VelocidadesRelativas[1])
		w = float(VelocidadesRelativas[2])

		xrp=(velX*np.cos(phi)-velY*np.sin(phi))
		yrp=(velX*np.sin(phi)+velY*np.cos(phi))

		xr=(xr+0.16*xrp)
		yr=(yr+0.16*yrp)
		phi=(phi+0.16*w)

		time.sleep(0.1)

def master():

	global velX, velY, w, xr, yr, phi

	velX = 0
	velY = 0
	w = 0

	xr = 0
	yr = 0
	phi = 0

	r = rospy.Rate(60)

	while not rospy.is_shutdown():

		current_time = rospy.Time.now()

		odom_quat = tf.transformations.quaternion_from_euler(0, 0, phi)

		odom_broadcaster.sendTransform(
			(xr, yr, 0.),
			odom_quat,
			current_time,
			"base_link",
			"odom"
		)

		odom = Odometry()
		odom.header.stamp = current_time
		odom.header.frame_id = "odom"

		odom.pose.pose = Pose(Point(xr, yr, 0.), Quaternion(*odom_quat))

		odom.child_frame_id = "base_link"
		odom.twist.twist = Twist(Vector3(velX, velY, 0), Vector3(0, 0, w))

		odom_pub.publish(odom)

		r.sleep()
	pass


def is_any_thread_alive(threads):
	return True in [t.is_alive() for t in threads]

if __name__ == "__main__":

	odom_pub = rospy.Publisher("/odom", Odometry, queue_size=1)
	odom_broadcaster = tf.TransformBroadcaster()
	rospy.init_node('odometria', anonymous=True)
	rospy.Subscriber("/encoders", String, callback)

	t1 = threading.Thread(target=master)
	t2 = threading.Thread(target=calculo)
	t1.daemon = True
	t2.daemon = True
	t1.start()
	t2.start()

	while is_any_thread_alive([t1,t2]):
		time.sleep(0)

