#include "MotorMessageBuilder.h"

#include <algorithm>

MotorMessageBuilder::MotorMessageBuilder(int frontLeftId, int frontRightId, int backLeftId, int backRightId) {
    this->frontLeftId = frontLeftId;
    this->frontRightId = frontRightId;
    this->backLeftId = backLeftId;
    this->backRightId = backRightId;
}

MotorMessageBuilder::MotorMessageBuilder() : MotorMessageBuilder(0, 1, 2, 3) {}

MotorMessageBuilder::MotorFrames MotorMessageBuilder::buildFrames(can_transporter::MotorVelocityPtr velocityMessage) {
    MotorMessageBuilder::MotorFrames frames;

    frames.frontLeft = buildFrame(frontLeftId, velocityMessage->frontLeft);
    frames.frontRight = buildFrame(frontRightId, -velocityMessage->frontRight);
    frames.backLeft = buildFrame(backLeftId, velocityMessage->backLeft);
    frames.backRight = buildFrame(backRightId, -velocityMessage->backRight);

    return frames;
}

can_msgs::Frame MotorMessageBuilder::buildFrame(int id, float velocity) {
    int32_t speed = std::min(std::max(-static_cast<int32_t>((velocity)*1023), -1023), 1023);

    can_msgs::Frame f;

    uint arbId = 0x00;
    arbId = (TALON | id);
    
    f.id = CONTROL_3 | arbId;
    f.dlc = 8;
    f.is_error = false;
    f.is_rtr = false;
    f.is_extended = true;
    for(int i = 0; i < 8; i++) f.data[i] = 0;

    ROS_INFO("Frame %x", arbId);

    f.data[0] = (unsigned char) (speed >> 16);
    f.data[1] = (unsigned char) (speed >> 8);
    f.data[2] = (unsigned char) (speed >> 0);

    return f;
}

MotorMessageBuilder::MotorStatus MotorMessageBuilder::getStatusType(can_msgs::FramePtr f) {
    unsigned char deviceNumber = (unsigned char)(f->id >> 0);
    deviceNumber <<= 2;
    deviceNumber >>= 2;

    int32_t commandId = (f->id&0x0007FFC0);
    
    if((int) deviceNumber == 2) {
        switch(commandId) {
            case STATUS_02:
                return MotorStatus::OUTPUT_CURRENT;
            case STATUS_03:
                return MotorStatus::CENTER;
            case STATUS_04:
                return MotorStatus::VOLTAGE_TEMP;
            case STATUS_13:
                return MotorStatus::LOOP_ERROR;
            default:
                return MotorStatus::UNKNOWN;
        }
    }else {
        return MotorStatus::UNKNOWN;
    }
}

can_transporter::MotorOutputCurrent MotorMessageBuilder::convertStatus2(can_msgs::FramePtr f) {
    can_transporter::MotorOutputCurrent output;
    unsigned char H = (unsigned char)(f->data[5]);
    unsigned char L = (unsigned char)(f->data[6]);

    H &= 0xFF;
    L &= 0xC0;
    int32_t raw = 0;
    raw |= H;

    raw <<= 8;
    raw |= L;
    raw >>= 6;
    
    output.motorId = ((unsigned char)(f->id >> 24)) & 0x0F;
    output.current = 0.125 * raw + 0.0;
    return output;
}

std::tuple<can_transporter::MotorTemperature, can_transporter::MotorBusVoltage> MotorMessageBuilder::convertStatus4(can_msgs::FramePtr f) {
    can_transporter::MotorTemperature temp;
    can_transporter::MotorBusVoltage voltage;

    unsigned char L = (unsigned char)(f->data[0]);
    int32_t raw = 0;
    raw |= L;

    voltage.voltage = 0.05 * raw + 4.0;

    L = (unsigned char)(f->data[7]);
    L &= 0x3F;
    raw = 0;
    raw |= L;
    temp.temp = (float)raw;

    int motorId = ((unsigned char)(f->id >> 24)) & 0x0F;
    voltage.motorId = motorId;
    temp.motorId = motorId;

    return {temp, voltage};
}

can_transporter::MotorError MotorMessageBuilder::convertStatus13(can_msgs::FramePtr f) {
    can_transporter::MotorError output;

    unsigned char H = (unsigned char)(f->data[0]);
    unsigned char M = (unsigned char)(f->data[1]);
    unsigned char L = (unsigned char)(f->data[2]);
    int32_t error = 0;
    error |= H;
    error <<= 8;
    error |= M;
    error <<= 8;
    error |= L;
    error <<= 8;
    error >>= 8;

    output.motorId = ((unsigned char)(f->id >> 24)) & 0x0F;
    output.error = (float)error * 360.0/6045.0;
    return output;
}