#include "ros/ros.h"
#include "can_msgs/Frame.h"
#include "std_msgs/Int8MultiArray.h"

#include "canbus/CanBusController.h"


CanBusController controller;

void registerCANFrame(can_msgs::FramePtr f) {
    controller.handleReceivedFrame(f);
}

void handleMotorVelocity(can_transporter::MotorVelocityPtr mv) {
    controller.handleMotorMessage(mv);
}

void handleActuatorMessage(std_msgs::Int8MultiArrayPtr actuatorStates) {
    if(actuatorStates->data.size() != 4) {
        ROS_ERROR("Incorrect size for the actuator state array (%li != 4)", actuatorStates->data.size());
        return;
    }

    // controller.handleActuatorMessage(actuatorStates);
}

int main(int argc, char **argv) { 
    ros::init(argc, argv, "can_transporter");

    ros::NodeHandle n;

    controller.setup(&n);

    // ros::Subscriber canReceiver = n.subscribe("/received_messages", 1000, registerCANFrame);
    ros::Subscriber motorReceiver = n.subscribe("/motor_velocities", 100, handleMotorVelocity);
    ros::Subscriber actuatorStateReceiver = n.subscribe("/desired_actuator_states", 100, handleActuatorMessage);
    
    ros::spin();

    return 0;
}