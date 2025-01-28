#ifndef MOTOR_CAN_BUS_CONTROLLER_H
#define MOTOR_CAN_BUS_CONTROLLER_H

#include "ros/ros.h"
#include "../drive_motor/MotorMessageBuilder.h"

class MotorCanBusController {
    public:
        MotorCanBusController();

        void setup(ros::NodeHandle *n, ros::Publisher *sent_messages);

        void handleCanBusMessage(can_msgs::FramePtr f);
        void handleVelocityMessage(can_transporter::MotorVelocityPtr mv);
    private:
        MotorMessageBuilder builder;
        ros::Publisher *sent_messages;

        ros::Publisher voltagePublisher;
        ros::Publisher errorPublisher;
        ros::Publisher currentPublisher;
        ros::Publisher tempPublisher;

        int frontLeftId = 0;
        int frontRightId = 1;
        int backLeftId = 2;
        int backRightId = 3;
};

#endif