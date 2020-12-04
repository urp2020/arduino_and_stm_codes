#include <Arduino.h>

#include "AccelStepper.h"
#include "Pinmap.h"


#define NUM_OF_MOTION 3
#define PI 3.141592
#define NUM_OF_STEPS 400
#define FPS 10
#define ENDURANCE 5 // MEAN 4.5 DEGREE

AccelStepper master(AccelStepper::DRIVER,step_inside,dir_inside); //step, dir
void motor_ready(AccelStepper* motor);

float make_position_within_range(float position,float range_max,float range_min);

float initial_pos_encoder,pos_encoder;

//SETUP


void setup()
{
  Serial.begin(9600);
  motor_ready(&master);
  initial_pos_encoder = analogRead(enc_inside); //READ INITIAL POSITION FROM MAGNETIC ENCODER
}


//LOOP
float num_of_steps_for_encoder_pos,num_of_steps_for_master;
void loop(){

    master.run();
    pos_encoder= analogRead(enc_inside); // 0~1023

    //encoder pos(0~1023) to steps(0~399)
    num_of_steps_for_encoder_pos = (pos_encoder-initial_pos_encoder)/1024.0*400;
    
    //make pos wintin range 0~399
    num_of_steps_for_encoder_pos = make_position_within_range(num_of_steps_for_encoder_pos,NUM_OF_STEPS,0);
    num_of_steps_for_master = make_position_within_range(master.currentPosition(),NUM_OF_STEPS,0);

    //USE POLLING
    if(abs(master.currentPosition()-num_of_steps_for_encoder_pos ) > ENDURANCE){
        master.setSpeed(-master.speed());
    }
    

}


float make_position_within_range(float position,float range_max,float range_min){
    while(position<range_min ||position>=range_max){
      if(position<range_min)
        position=position+(range_max-range_min);
      if(position>=range_max)
        position= position-(range_max-range_min);
    }
  return position;
}

void motor_ready(AccelStepper* motor){
  motor->moveTo(400);
  motor->setAcceleration(200);
  motor->setMinPulseWidth(20);
  motor->setMaxSpeed(2000);//initialize stepper
  motor->setCurrentPosition(0);

  motor->setSpeed(200);
}