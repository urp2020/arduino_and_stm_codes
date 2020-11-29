#include <Arduino.h>
#include "AccelStepper.h"

#define STEP_1 D9
#define DIR_1  D10

#define STEP_2 D6
#define DIR_2  D7


AccelStepper UPPER(AccelStepper::DRIVER,STEP_1,DIR_1);
AccelStepper DOWN(AccelStepper::DRIVER,STEP_2,DIR_2);

void motor_ready(AccelStepper* motor);

int speed_upper=100, speed_down=10;
int dir_upper=1, dir_down=1;

void setup() {
  // put your setup code here, to run once:
  motor_ready(&UPPER);
  motor_ready(&DOWN);
  UPPER.setSpeed(speed_upper*dir_upper);
  DOWN.setSpeed(speed_down*dir_down);
}

void loop() {
  UPPER.runSpeed();
  DOWN.runSpeed();
  // put your main code here, to run repeatedly:
}

void motor_ready(AccelStepper* motor){
  motor->moveTo(400);
  motor->setAcceleration(200);
  motor->setMinPulseWidth(20);
  motor->setMaxSpeed(2000);//initialize stepper
  motor->setCurrentPosition(0);
}