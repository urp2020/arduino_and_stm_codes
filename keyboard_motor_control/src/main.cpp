#include <Arduino.h>
#include "AccelStepper.h"

#define STEP 5

AccelStepper stepper1(AccelStepper::DRIVER,16,17);
AccelStepper stepper2(AccelStepper::DRIVER,18,19);

int stepper1_speed=0,stepper2_speed=0;
int stepper1_dir=1, stepper2_dir=1;
int max_angle_difference = 0;
void motor_ready(AccelStepper* motor);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  motor_ready(& stepper1);
  motor_ready(& stepper2);

  while(!Serial.available()){
    delay(100);
    //Serial.println("wait..");
  }

  max_angle_difference = Serial.readString().toInt();
  Serial.println(max_angle_difference);
  
}

void loop() {
  // put your main code here, to run repeatedly:
   if(Serial.available() > 0){
     char c = Serial.read();
     switch(c){
      case 'u':
        stepper1_speed= stepper1_speed+STEP;
        break;
      case 'd':
        stepper1_speed= stepper1_speed-STEP;
        break;
      case 'f':
        stepper1_dir= -stepper1_dir;
        break;
      case 'w':
        stepper2_speed= stepper2_speed+STEP;
        break;
      case 's':
        stepper2_speed= stepper2_speed-STEP;
        break;
      case 'a':
        stepper2_dir= -stepper2_dir;
        break;
     }
     stepper1.setSpeed(stepper1_speed*stepper1_dir);
     stepper2.setSpeed(stepper2_speed*stepper2_dir);
   }

    if(abs(stepper1.currentPosition()-stepper2.currentPosition())>=max_angle_difference){
      /*
      if(stepper1.speed()>stepper2.speed() ){
        stepper2.setSpeed(stepper1.speed());
      }
      else{
        stepper1.setSpeed(stepper2.speed());
      }
      */
      stepper1.setSpeed(0);
      stepper2.setSpeed(0);
    }
    stepper1.runSpeed();
    stepper2.runSpeed();
    

}

void motor_ready(AccelStepper* motor){
  motor->moveTo(400);
  motor->setAcceleration(200);
  motor->setMinPulseWidth(20);
  motor->setMaxSpeed(2000);//initialize stepper
  motor->setCurrentPosition(0);
}