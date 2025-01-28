#include "ActuatorCanBusController.h"

ActuatorCanBusController::ActuatorCanBusController() {}

void ActuatorCanBusController::setup(ros::NodeHandle *n, ros::Publisher *sent_messages) {
    this->sent_messages = sent_messages;
}

void ActuatorCanBusController::handleStateMessage(std_msgs::Int8MultiArrayPtr states) {
    can_msgs::Frame f = builder.buildFrame(states);

    sent_messages->publish(f);
}