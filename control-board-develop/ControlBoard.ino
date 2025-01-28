#include <ros.h>
#include <std_msgs/Int8MultiArray.h>

#define backRightF 4
#define backRightB 5
#define frontRightF 6
#define frontRightB 7
#define backLeftF 10
#define backLeftB 11
#define frontLeftF 12
#define frontLeftB 13

#define middleF 8
#define middleB 9

#define scoopElbowUp 30
#define scoopElbowDown 31
#define scoopWristUp 32
#define scoopWristDown 33

std_msgs::Int8MultiArray msg;
ros::Publisher Actuator("/rover/control/board", &msg);

ros::NodeHandle  nh;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(backLeftF, INPUT_PULLUP);
  pinMode(backLeftB, INPUT_PULLUP);
  pinMode(backRightF, INPUT_PULLUP);
  pinMode(backRightB, INPUT_PULLUP);
  pinMode(frontLeftF, INPUT_PULLUP);
  pinMode(frontLeftB, INPUT_PULLUP);
  pinMode(frontRightF, INPUT_PULLUP);
  pinMode(frontRightB, INPUT_PULLUP);

  pinMode(middleF, INPUT_PULLUP);
  pinMode(middleB, INPUT_PULLUP);

  pinMode(scoopElbowUp, INPUT_PULLUP);
  pinMode(scoopElbowDown, INPUT_PULLUP);
  pinMode(scoopWristUp, INPUT_PULLUP);
  pinMode(scoopWristDown, INPUT_PULLUP);

  nh.initNode();
  nh.advertise(Actuator);

  // {-1, 0, 0, 0};

}

//write function that takes all the inputs from digitalRead()
// and returns out an array with 4 values

int processorFunction(int blF, int blB){
  int readValF = digitalRead(blF);
  int readValB = digitalRead(blB);
  int returnVal;
  int diff = readValF - readValB;
  if(diff == 0){
    returnVal = 0;
  }
  else if (diff == -1){
    returnVal = 1;
  }
  else if (diff == 1){
    returnVal = -1;
  }

  return returnVal;
  
}

void loop() {
  
  int8_t backLeft = processorFunction(backLeftF, backLeftB);
  int8_t backRight = processorFunction(backRightF, backRightB);
  int8_t frontLeft = processorFunction(frontLeftF, frontLeftB);
  int8_t frontRight = processorFunction(frontRightF, frontRightB);
  int8_t middleActuator = processorFunction(middleF, middleB);
  int8_t scoopElbow = processorFunction(scoopElbowUp, scoopElbowDown);
  int8_t scoopWrist = processorFunction(scoopWristUp, scoopWristDown);
  
  
  int8_t value[7] = {backLeft, backRight, frontLeft, frontRight, middleActuator, scoopElbow, scoopWrist};
  msg.data = value;
  msg.data_length = 7;
  Actuator.publish(&msg);
  nh.spinOnce();
  delay(100);
  

}