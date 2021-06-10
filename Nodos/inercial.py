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
from sensor_msgs.msg import Imu
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point, Pose, Quaternion, Twist, Vector3

arduino = serial.Serial('/dev/ttyACM0',9600, timeout=1)


def talker():
    imu_pub = rospy.Publisher('imu/data', Imu, queue_size=1)
    rospy.init_node('imu_talker', anonymous=True)
    r = rospy.Rate(60)


    while not rospy.is_shutdown():
        entrada = arduino.readline()
        arduino.flushInput() 
        valores = entrada.split()

        imu = Imu()
        imu.header.stamp = rospy.Time.now()
        imu.header.frame_id = 'base_laser'

        imu.orientation.w = 0
        imu.orientation.x = 0
        imu.orientation.y = 0
        imu.orientation.z = 0
        try:
            imu.angular_velocity.x = 0
            imu.angular_velocity.y = 0
            imu.angular_velocity.z = float(valores[2])

            imu.linear_acceleration.x = -float(valores[0])
            imu.linear_acceleration.y = -float(valores[1])
            imu.linear_acceleration.z = 0

        except:
            imu.angular_velocity.x = 0
            imu.angular_velocity.y = 0
            imu.angular_velocity.z = 0

            imu.linear_acceleration.x = 0
            imu.linear_acceleration.y = 0
            imu.linear_acceleration.z = 0

        imu_pub.publish(imu)
        r.sleep()
if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
