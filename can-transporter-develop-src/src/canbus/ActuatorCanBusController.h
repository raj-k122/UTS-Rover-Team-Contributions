#ifndef ACTUATOR_CAN_BUS_CONTROLLER_H
#define ACTUATOR_CAN_BUS_CONTROLLER_H

#include "ros/ros.h"
#include "can_msgs/Frame.h"
#include "std_msgs/Int8MultiArray.h"
#include "../drive_motor/ActuatorMessageBuilder.h"

class ActuatorCanBusController {
    public:
        ActuatorCanBusController();

        void setup(ros::NodeHandle *n, ros::Publisher *sent_messages);
        void handleStateMessage(std_msgs::Int8MultiArrayPtr states);

    private:
        ActuatorMessageBuilder builder;
        ros::Publisher *sent_messages;
};

#endif