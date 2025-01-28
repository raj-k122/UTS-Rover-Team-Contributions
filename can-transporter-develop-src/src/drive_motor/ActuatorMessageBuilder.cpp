#include "ActuatorMessageBuilder.h"

ActuatorMessageBuilder::ActuatorMessageBuilder() {}

can_msgs::Frame ActuatorMessageBuilder::buildFrame(std_msgs::Int8MultiArrayPtr states) {
    can_msgs::Frame f;

    f.id = 0x300;
    f.dlc = 8;
    f.is_error = false;
    f.is_extended = true;
    f.is_rtr = false;
    
    int i;
    for(i = 0; i < states->data.size(); i++) {
        f.data[i] = states->data[i] < 0 ? 2 : states->data[i];
    }

    for(int dlc = 8; i < dlc; i++) {
        f.data[i] = 0;
    }

    return f;
}