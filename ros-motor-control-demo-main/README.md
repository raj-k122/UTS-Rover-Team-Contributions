# ROS Motor Control demo

This branch of the code allows you to control a standard servo with the "j" and "k" keys of the keyboard through ROS. 

## Arduino code
Low level code to control the servo through the arduino. Creates a subscriber to the topic "servo". 

## Modified Teleop Twist node
A pre-built node that you can install. It handles characters from your keyboard and publishes them to the topic "cmd_vel".

Run with command: "rosrun teleop_twist_keyboard teleop_twist_keyboard.py"

## Convert Twist node
This node subscribes to "cmd_vel" and converts its Twist message into a character value and publishes them to the topic "servo"

Run with command: "rosrun convert_twist convert_twist.py"

## Rosserial node
Rosserial allows communication between the arduino and ROS. 

Run with command: "rosrun rosserial_python serial_node.py {add location of your arduino USB port} 57600"
