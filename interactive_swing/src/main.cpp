#include <Arduino.h>

#include "AccelStepper.h"
#include "Pinmap.h"


#define NUM_OF_MOTION 3
#define PI 3.141592
#define NUM_OF_STEPS 400
#define FPS 10
<<<<<<< HEAD
#define ENDURANCE 30.0 // MEAN 5.4 DEGREE

=======
#define ENDURANCE 20 // MEAN 5.4 DEGREE
>>>>>>> 4eece6d29e13472589ca9a18046fb5ffc8d659b5
AccelStepper master(AccelStepper::DRIVER,step_inside,dir_inside); //step, dir
void motor_ready(AccelStepper* motor);

float make_position_within_range(float position,float range_max,float range_min);

float initial_pos_encoder,pos_encoder;
float getAverageRead(uint32_t anal_in,int num);

//SETUP

int global_speed;
void setup()
{
  Serial.begin(9600);
  motor_ready(&master);
  initial_pos_encoder = getAverageRead(enc_inside,10); //READ INITIAL POSITION FROM MAGNETIC ENCODER
  
}


//LOOP
float num_of_steps_for_encoder_pos,num_of_steps_for_master;

void loop(){
    //analogWrite(enc_outside,0);
    master.runSpeed();
    pos_encoder= getAverageRead(enc_inside,10); // 0~1023

    //encoder pos(0~1023) to steps(0~399)
    num_of_steps_for_encoder_pos = (pos_encoder-initial_pos_encoder)/1024.0*400;
    
    //make pos wintin range 0~399
    num_of_steps_for_encoder_pos = make_position_within_range(num_of_steps_for_encoder_pos,NUM_OF_STEPS,0);
    num_of_steps_for_master = make_position_within_range(master.currentPosition(),NUM_OF_STEPS,0);

    //USE POLLING(recognize desync with encoder and master)
       
    
    if(abs(num_of_steps_for_master-num_of_steps_for_encoder_pos ) > ENDURANCE &&
       abs(num_of_steps_for_master-num_of_steps_for_encoder_pos )< 400-ENDURANCE ){
        //should synchronize encoder and master
        master.setCurrentPosition(0);
        global_speed=-global_speed; 
        master.setSpeed(global_speed);
        initial_pos_encoder = getAverageRead(enc_inside,10);
        //analogWrite(enc_outside,255);
        //Serial.println(abs(num_of_steps_for_master-num_of_steps_for_encoder_pos ));
    }
    
    
    

}


float make_position_within_range(float position,float range_max,float range_min){
    while(position<range_min ||position>=range_max){
      if(position<range_min)
        position=position+(range_max-range_min);
      else if(position>=range_max)
        position= position-(range_max-range_min);
    }
    if(abs(position-range_max) <5)//if postion is near 400 just turn into 0
      position= range_min;
  return position;
}

void motor_ready(AccelStepper* motor){
  motor->moveTo(400);
  motor->setAcceleration(200);
  motor->setMinPulseWidth(20);
  motor->setMaxSpeed(2000);//initialize stepper
  motor->setCurrentPosition(0);

  motor->setSpeed(500);
  global_speed =motor->speed(); 
}

float getAverageRead(uint32_t anal_in,int num){
  float aver;
  for(int i=0; i<num; i++)
    aver+=analogRead(anal_in);  
  return aver/num;
}