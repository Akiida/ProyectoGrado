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


def callback(msg):
	global WheelVelRef

	a=0.145
	b=0.115
	R=0.04

	T = np.array([[1, -1, -(a+b)],
			[1, 1, (a+b)],
			[1, 1, -(a+b)], 
			[1, -1, (a+b)]])*(1/R)

	VelocidadesRelativas = np.array([[float(msg.linear.x)],
			[float(msg.linear.y)],
			[float(msg.angular.z)]])
	Wd = np.dot(T, VelocidadesRelativas) 
	Wd = Wd * 1/(2*3.141592653589793238)
	for k in range (0,4):
		if float(Wd[k]) >= 0:
			WheelVelRef[k] = int(50 + float(Wd[k])*50)
		else:
			WheelVelRef[k] = int(-float(Wd[k])*50)


def LecuraSerial():

	global	WheelVel, pub
	WheelVel = [0,0,0,0]
	while not rospy.is_shutdown():
		entrada = []
		STM.flushInput() 
		for i in STM.readline().decode():
			entrada.append(ord(i))
		try:
			for j in range (0,4):
				if float(entrada[j]) >= 50:
					WheelVel[j] = ((float(entrada[j])-50)*2)/100
				else:
					WheelVel[j] = (float(entrada[j])*-2)/100
			time.sleep(0.1)
			mensaje = str(WheelVel[3]) + " " + str(WheelVel[2]) + " " + str(WheelVel[1]) + " " + str(WheelVel[0])
			pub.publish(mensaje)
		except:
			WheelVel = [0,0,0,0]

	pass

def EscrituraSerial():

	global WheelVelRef
	while not rospy.is_shutdown():
		STM.write(chr(WheelVelRef[3]).encode())
		STM.write(chr(WheelVelRef[2]).encode())
		STM.write(chr(WheelVelRef[1]).encode())
		STM.write(chr(WheelVelRef[0]).encode())
		STM.write(chr(95).encode())
		time.sleep(0.1)
	pass


def is_any_thread_alive(threads):
	return True in [t.is_alive() for t in threads]

if __name__ == "__main__":

	global WheelVelRef, pub
	pub = rospy.Publisher('/encoders', String, queue_size=10)
	rospy.Subscriber("/cmd_vel", Twist, callback)
	rospy.init_node('control', anonymous=True)
	WheelVelRef = [0,0,0,0]
	STM = serial.Serial('/dev/ttyUSB0',9600, timeout=1)
	t1 = threading.Thread(target=LecuraSerial)
	t2 = threading.Thread(target=EscrituraSerial)
	t1.daemon = True
	t2.daemon = True
	t1.start()
	t2.start()

	while is_any_thread_alive([t1,t2]):
		time.sleep(0)

	time.sleep(0.1)
	STM.write(chr(0).encode())
	STM.write(chr(0).encode())
	STM.write(chr(0).encode())
	STM.write(chr(0).encode())
	STM.write(chr(95).encode())
	time.sleep(0.1)



