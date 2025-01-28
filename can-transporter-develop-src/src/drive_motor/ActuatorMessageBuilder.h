#ifndef ACTUATOR_MESSAGE_BUILDER_H
#define ACTUATOR_MESSAGE_BUILDER_H

#include "can_msgs/Frame.h"
#include "std_msgs/Int8MultiArray.h"

class ActuatorMessageBuilder {
    public:
        ActuatorMessageBuilder();

        can_msgs::Frame buildFrame(std_msgs::Int8MultiArrayPtr states);
};

#endif