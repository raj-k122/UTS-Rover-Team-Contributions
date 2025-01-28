#!/usr/bin/env python
from std_msgs.msg import String
import rospy
from geometry_msgs.msg import Twist

class ConvertTwist():
    def __init__(self):
        
        rospy.init_node('convert_twist')
        
        self.ros_pub = rospy.Publisher("/servo", String, queue_size=1)
        rospy.loginfo("> Publisher is working")

        self.ros_sub = rospy.Subscriber("/cmd_vel", Twist, self.get_character)
        rospy.loginfo("> Subscriber is working")

        rospy.spin()
    
    def get_character(self, message):
        output = ""
        maz = message.angular.z
        max = message.angular.x
        may = message.angular.y
        laz = message.linear.z
        lax = message.linear.x
        lay = message.linear.y

        if(maz == 1 and max == 0 and may == 0): 
            output = "j"
        elif(maz == -1 and max == 0 and may == 0): 
            output = "l"
        
        elif(laz == 0.5 and lax == 0 and lay == 0):
            output = ">"
        elif(laz == -0.5 and lax == 0 and lay == 0):
            output = "<" 

             
        rospy.loginfo("Message output: %s", output)
        self.ros_pub.publish(output)



if __name__ == "__main__":
    ConvertTwist()

    
    
