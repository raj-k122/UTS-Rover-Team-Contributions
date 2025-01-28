#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <Adafruit_PWMServoDriver.h>
#include <Servo.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int64.h>
#include <string.h>

#define SERVOMIN 70
#define SERVOMAX 500
#define SERVOHOLD 1500

#define servoHIP 15
#define servoSHOULDER 11
#define servoELBOW 8
#define servoWRIST 7
#define servoSWIVEL 3
#define servoGRIP 0

#define I2CAdd 0x40

char bufferArray[100];

ros::NodeHandle  nh;
/* Create an instance of the library */
Adafruit_PWMServoDriver Driver = Adafruit_PWMServoDriver(0x40); 
Servo servo;

int limiterFunction(int inputInt){
    if(inputInt > 490){
        inputInt = 490;
    }
    if(inputInt < 115){
        inputInt = 115;
    }
    return inputInt;
    
}
int packetServo = 0;
int packetHold11, packetHold8;
std_msgs::String cmd_msgs;
ros::Publisher feedback_pub("feedback_servo", &cmd_msgs);

int holdPos(int packetInt, int servoIndex){
  packetHold11 = packetInt;
  Driver.setPWM(servoIndex, 0, packetHold11);
}

void addingPos(int servoIndex){
  int current_pos = Driver.getPWM(servoIndex);
  for(int i=current_pos; i<=(current_pos + 10); i++){
    //packetInt = limiterFunction(packetInt);
    Driver.setPWM(servoIndex, 0, i);
  }
}

void subtractPos(int servoIndex){
  int current_pos = Driver.getPWM(servoIndex);
  for(int i=current_pos; i>=(current_pos - 10); i--){
    //packetInt = limiterFunction(packetInt);
    Driver.setPWM(servoIndex, 0, i);
  }
}


void Actuator(int motorIndex, bool checker, int motorDir){
  if(checker == false){
     for(int i=0; i<20; i++){
       Driver.writeMicroseconds(motorIndex, motorDir);
     }
     checker = true;
  }
  Driver.writeMicroseconds(motorIndex, SERVOHOLD);
}

void InitialPosition(){
  Driver.setPWM(servoSHOULDER,0, 450);
  delay(500);
  Driver.setPWM(servoELBOW,0, 150);
  delay(500);
  Driver.setPWM(servoWRIST, 0, 150);
  Driver.setPWM(servoSWIVEL, 0, 450);
  Driver.setPWM(servoGRIP, 0, 160);
}

void setServoValue(std_msgs::String input){
    int motor0pos = Driver.getPWM(0);
    int motor3pos = Driver.getPWM(3);
    int motor7pos = Driver.getPWM(7);
    int motor8pos = Driver.getPWM(8);
    int motor11pos = Driver.getPWM(11);
    String packetString = input.data;
    packetString.toCharArray(bufferArray, 100);
    cmd_msgs.data = bufferArray;
    bool hasRun = false;
    if(nh.connected()){
      if(packetString == ">"){
        Actuator(servoHIP,hasRun, 1000);
      }
      else if(packetString == "<"){
        Actuator(servoHIP,hasRun, 2000);
      }
      else if(packetString == "1"){
        for (int a = motor11pos; a<(motor11pos + 30); a++){
          Driver.setPWM(servoSHOULDER,0, a);
        }
        //subtractPos(8);
        Driver.setPWM(servoELBOW,0, motor8pos);
        Driver.setPWM(servoWRIST, 0, motor7pos);
      }
      else if(packetString == "2"){
        for (int a = motor11pos; a>(motor11pos - 30); a--){
          Driver.setPWM(servoSHOULDER,0, a);
        }
        //subtractPos(8);
        Driver.setPWM(servoELBOW,0, motor8pos);
        Driver.setPWM(servoWRIST, 0, motor7pos);
      }
      else if(packetString == "3"){
        for (int a = motor8pos; a<(motor8pos + 30); a++){
          Driver.setPWM(servoELBOW,0, a);
        }
        Driver.setPWM(servoSHOULDER,0, motor11pos);
        Driver.setPWM(servoWRIST, 0, motor7pos);
      }
      else if(packetString == "4"){
        for (int a = motor8pos; a>(motor8pos - 30); a--){
          Driver.setPWM(servoELBOW,0, a);
        }
        Driver.setPWM(servoSHOULDER,0, motor11pos);
        Driver.setPWM(servoWRIST, 0, motor7pos);
        
      }
      else if(packetString == "5"){
        for (int a = motor7pos; a<(motor7pos + 30); a++){
          Driver.setPWM(servoWRIST, 0, a);
        }
        Driver.setPWM(servoSHOULDER,0, motor11pos);
        Driver.setPWM(servoELBOW,0, motor8pos);
      }
      else if(packetString == "6"){
        for (int a = motor7pos; a>(motor7pos - 30); a--){
          Driver.setPWM(servoWRIST, 0, a);
        }
        Driver.setPWM(servoSHOULDER,0, motor11pos);
        Driver.setPWM(servoELBOW,0, motor8pos);
      }

      else if(packetString == "7"){
        for (int a = motor3pos; a<(motor3pos + 30); a++){
          Driver.setPWM(servoSWIVEL, 0, a);
        }
      }
      else if(packetString == "8"){
        for (int a = motor3pos; a>(motor3pos - 30); a--){
          Driver.setPWM(servoSWIVEL, 0, a);
        }
      }

      else if(packetString == "9"){
        for (int a = motor0pos; a<(motor0pos + 30); a++){
          Driver.setPWM(servoGRIP, 0, a);
        }
      }
      else if(packetString == "0"){
        for (int a = motor0pos; a>(motor0pos - 30); a--){
          Driver.setPWM(servoGRIP, 0, a);
        }
      }
      else{
        Driver.writeMicroseconds(servoHIP,SERVOHOLD);
        Driver.setPWM(servoSHOULDER,0, motor11pos);
        Driver.setPWM(servoWRIST, 0, motor7pos);
        Driver.setPWM(servoELBOW,0, motor8pos);
      }
    }

    feedback_pub.publish(&cmd_msgs); 
    
}

ros::Subscriber<std_msgs::String> sub_servo("servo", &setServoValue);

void setup(){
    nh.initNode();
    nh.advertise(feedback_pub);
    nh.subscribe(sub_servo);
    Driver.wakeup();
    Driver.begin();
    Driver.setPWMFreq(50);
    InitialPosition();
    
}

void loop(){
    
    nh.spinOnce();
    delay(1);
}