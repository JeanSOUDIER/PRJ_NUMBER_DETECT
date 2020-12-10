Brief project description
======


This is a school electrical engineering project (end-year).

It consists into:
  * Detecting a list of hand-written digits and a whiteboard, and recognize them.
  * Send those digits through Bluetooth to a robot arm mounted on a TurtleBot that will move itself to another whiteboard to write the data it received.

Material used:
======

  * NVIDIA Jetson Nano.
  * Raspberry Pi 3B+.
  * Raspberry Pi Camera Module V2.
  * TurtleBot Waffle Pi.
  * Robot arm WidowXL.
  * HC05 (x2)


Advanced project description:
======

> TO BE ADDED


Repository organization
======

 CAO:
----------
  CAD files for several 3D-printed parts.

Driver:
----------

  Configuration files for the Jetson Nano and Raspberry Pi.

PRJ_AUTOM:
----------

  Matlab files that simulate the Turtlebot movements.

PRJ_MOBI:
----------

  Matlab files that simulate the Turtlebot power consumption (overestimated).

PROG:
----------

  Main programming files.

### BN_robot

  Every file related to the Turtlebot and robot arm control. 
It is meant to be executed on the Raspberry Pi that controls the robot.

### CodeArmWidowXL/ros

  Sample files to move the robot arm through ROS on an Arduino.


### CodeOpenCR/RC100_modif

  Sample files to control the TurtleBot through ROS on an Arduino.


### NeuralNetwork_Renderer

  Convolutional Neural Network rendering interface. Also contains any neural network- and image processing-related files.
It is meant to be executed on the Jetson Nano that hosts the digit-detection and -recognition processes.

Authors
======


  * DUHAMEL Erwan (erwanduhamel@outlook.com)
  * SOUDIER Jean (jean.soudier@insa-strasbourg.com)

