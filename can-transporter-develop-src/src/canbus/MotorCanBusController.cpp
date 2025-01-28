#include "MotorCanBusController.h"

MotorCanBusController::MotorCanBusController() {}

void MotorCanBusController::setup(ros::NodeHandle *n, ros::Publisher *sent_messages) {
    this->sent_messages = sent_messages;

    n->getParam("motor/front_left_id", frontLeftId);
    n->getParam("motor/front_right_id", frontRightId);
    n->getParam("motor/back_left_id", backLeftId);
    n->getParam("motor/back_right_id", backRightId);
    
    ROS_INFO("Setting up status publishers");

    voltagePublisher = n->advertise<can_transporter::MotorBusVoltage>("can_transporter/motor_bus_voltage", 10);
    errorPublisher = n->advertise<can_transporter::MotorError>("can_transporter/motor_loop_error", 10);
    currentPublisher = n->advertise<can_transporter::MotorOutputCurrent>("can_transporter/motor_output_current", 10);
    tempPublisher = n->advertise<can_transporter::MotorTemperature>("can_transporter/motor_temperature", 10);

    builder = MotorMessageBuilder(frontLeftId, frontRightId, backLeftId, backRightId);
}

void MotorCanBusController::handleCanBusMessage(can_msgs::FramePtr f) { 
    // We assume this is a message from the motors as others should have been filtered out in the main controller
    MotorMessageBuilder::MotorStatus statusType = builder.getStatusType(f);

    if(statusType == MotorMessageBuilder::MotorStatus::OUTPUT_CURRENT) {
        can_transporter::MotorOutputCurrent current = builder.convertStatus2(f);
        currentPublisher.publish(current);
    }else if(statusType == MotorMessageBuilder::MotorStatus::VOLTAGE_TEMP) {
        auto tuple = builder.convertStatus4(f);
        tempPublisher.publish(std::get<0>(tuple));
        voltagePublisher.publish(std::get<1>(tuple));
    }else if(statusType == MotorMessageBuilder::MotorStatus::LOOP_ERROR) {
        can_transporter::MotorError error = builder.convertStatus13(f);
        errorPublisher.publish(error);
    }else {
        ROS_ERROR("Unknown Status Packet");
    }
}

void MotorCanBusController::handleVelocityMessage(can_transporter::MotorVelocityPtr mv) {
    ROS_INFO("Handling motor velocity message...");
    MotorMessageBuilder::MotorFrames frames = builder.buildFrames(mv);

    can_msgs::Frame enableFrame;
    enableFrame.id = 0x000401bf;
    enableFrame.dlc = 8;
    enableFrame.is_extended = true;
    enableFrame.is_error = false;
    enableFrame.is_rtr = false;

    for(int i = 0; i < 8; i++) enableFrame.data[i] = 0;
    enableFrame.data[0] = 1;

    ROS_INFO("Front Left:\n");
    for(int i = 0; i < 8; i++) {
        ROS_INFO("%d", frames.frontLeft.data[i]);
    }

    sent_messages->publish(enableFrame);
    sent_messages->publish(frames.frontLeft);
    sent_messages->publish(frames.frontRight);
    sent_messages->publish(frames.backLeft);
    sent_messages->publish(frames.backRight);
}