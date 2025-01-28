#ifndef MOTOR_MESSAGE_BUILDER_H
#define MOTOR_MESSAGE_BUILDER_H

#include "ros/ros.h"
#include "can_msgs/Frame.h"
#include "can_transporter/MotorVelocity.h"
#include "can_transporter/MotorTemperature.h"
#include "can_transporter/MotorBusVoltage.h"
#include "can_transporter/MotorOutputCurrent.h"
#include "can_transporter/MotorError.h"
#include "talon.h"
#include <string>
#include <tuple>

#define TALON 0x02040000

class MotorMessageBuilder {
    public:
        MotorMessageBuilder(int,int,int,int);
        MotorMessageBuilder();

        struct MotorFrames {
            can_msgs::Frame frontLeft;
            can_msgs::Frame frontRight;
            can_msgs::Frame backLeft;
            can_msgs::Frame backRight;
        };

        enum MotorStatus {
            OUTPUT_CURRENT = STATUS_02,
            CENTER = STATUS_03,
            VOLTAGE_TEMP = STATUS_04,
            LOOP_ERROR = STATUS_13,
            UNKNOWN = 0x0
        };

        MotorFrames buildFrames(can_transporter::MotorVelocityPtr velocityMessage);

        MotorStatus getStatusType(can_msgs::FramePtr f);

        can_transporter::MotorOutputCurrent convertStatus2(can_msgs::FramePtr f);
        std::tuple<can_transporter::MotorTemperature, can_transporter::MotorBusVoltage> convertStatus4(can_msgs::FramePtr f);
        can_transporter::MotorError convertStatus13(can_msgs::FramePtr f);

        
    private:
        int frontLeftId ;
        int frontRightId;
        int backLeftId;
        int backRightId;

        can_msgs::Frame buildFrame(int id, float velocity);

};

#endif