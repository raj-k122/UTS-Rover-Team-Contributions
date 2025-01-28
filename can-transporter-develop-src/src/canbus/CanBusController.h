#ifndef CAN_BUS_CONTROLLER_H
#define CAN_BUS_CONTROLLER_H

#include "ros/ros.h"
#include "can_msgs/Frame.h"
#include "std_msgs/Int8MultiArray.h"
#include "MotorCanBusController.h"
#include "ActuatorCanBusController.h"

class CanBusController {
    public:
        CanBusController();

        void setup(ros::NodeHandle *n);

        void handleReceivedFrame(can_msgs::FramePtr f);
        void handleMotorMessage(can_transporter::MotorVelocityPtr mv);
        void handleActuatorMessage(std_msgs::Int8MultiArrayPtr states);
    
    private:
        ros::NodeHandle *n;
        ros::Publisher sent_messages;

        MotorCanBusController motorController;
        ActuatorCanBusController actuatorController;

};

#endif