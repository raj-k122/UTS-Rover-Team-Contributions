#include "CanBusController.h"

CanBusController::CanBusController() {}

void CanBusController::setup(ros::NodeHandle *n) {
    this->n = n;

    ROS_INFO("Setup Started");

    sent_messages = n->advertise<can_msgs::Frame>("sent_messages", 100);

    ROS_INFO("Calling setup for motor controller");
    motorController.setup(n, &sent_messages);
    actuatorController.setup(n, &sent_messages);

}

void CanBusController::handleReceivedFrame(can_msgs::FramePtr f) {
    // TODO: Handle received frame
    // This isn't necessary to drive the rover, in theory
    // we only need to send messages, the statuses themselves are
    // not required and only provide feedback about the current state of the motor

    // What needs to be done:
    // - Get Frame ID
    uint32_t id = f->id;
    // - Check to see if message is from TalonFX (i.e. ID is 0x020400xx)
    if((id & 0xFFFFFF00) == 0x02040000){
        // MotorCanBusController::handleCanBusMessage(f)
    }
    else{
        // - If not, do nothing for now
    }
}

void CanBusController::handleActuatorMessage(std_msgs::Int8MultiArrayPtr states) {
    actuatorController.handleStateMessage(states);
}

void CanBusController::handleMotorMessage(can_transporter::MotorVelocityPtr mv) {
    motorController.handleVelocityMessage(mv);
}
