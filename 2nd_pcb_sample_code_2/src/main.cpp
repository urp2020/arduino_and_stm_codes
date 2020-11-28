#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "AccelStepper.h"
#include "Pinmap.h"


#define NUM_OF_MOTION 3
#define PI 3.141592
#define NUM_OF_STEPS 400
#define FPS 10
#define precision 25 //text_file number(string) maimum length 3.1415921

int motion_index = 0; //change motion index for changing motion
char *MOTIONS[NUM_OF_MOTION] = {"motion1.txt", "motion2.txt", "motion3.txt"};
SDLib::File current_motion;

AccelStepper master(AccelStepper::DRIVER,step_inside,dir_inside); //step, dir

char position_string[precision]; // read text file float number here.
int initial_position;

void motor_ready(AccelStepper* motor);
void move_to_target_position(AccelStepper* master,float master_target);
void change_SD_file(SDLib::File* _current_motion);
float target_position_abs_position(float target);


//SETUP
void setup()
{
  pinMode(A2,OUTPUT);
  Serial.begin(9600);
  while (!Serial){;}
  Serial.print("Initializing SD card...");
  if (!SD.begin(sd_CS))
  {                                                                                                                                                                                                                                                                                                                                 
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  current_motion = SD.open(MOTIONS[motion_index]);

  motor_ready(&master);

  initial_position = analogRead(enc_inside); //READ INITIAL POSITION FROM MAGNETIC ENCODER
}


//LOOP
char read_from_text;
int idx=0;
float target_position;
float abs_target;
float encoder_pos;
int temp;

void loop(){
  
  if (current_motion.available()){

    read_from_text = current_motion.read();
    position_string[idx] = read_from_text;
    idx++;

    if(read_from_text == '\n'){
      target_position = atof(position_string);
      abs_target = target_position_abs_position(target_position);
      memset(position_string,0,precision);
      idx=0;
      move_to_target_position(&master,target_position);

    }
    
  }
  else{
    current_motion.seek(0);
  }
    // close the file:
  
  // put your main code here, to run repeatedly:*/
}

void move_to_target_position(AccelStepper* master,float master_target){

  master->moveTo(master_target/360.0*NUM_OF_STEPS);
  master->setSpeed((master_target/360.0*NUM_OF_STEPS-master->currentPosition())*FPS );

  while(master->distanceToGo()!=0){
    master->runSpeedToPosition();
  }
}

void change_SD_file(SDLib::File* _current_motion){//if there did not reach target, then change file
  _current_motion->close();// close file
  motion_index= (motion_index+1)%NUM_OF_MOTION; //motion index update
  *(_current_motion) = SD.open(MOTIONS[motion_index]); //open new motion
  Serial.println((*_current_motion).name());

  initial_position = analogRead(enc_inside);
  
  master.setCurrentPosition(0);
  
}
float target_position_abs_position(float target){
    while(target<0 ||target>400){
      if(target<0)
        target=target+400;
      if(target>400)
        target= target-400;
    }
  return target;
}

void motor_ready(AccelStepper* motor){
  motor->moveTo(400);
  motor->setAcceleration(200);
  motor->setMinPulseWidth(20);
  motor->setMaxSpeed(2000);//initialize stepper
  motor->setCurrentPosition(0);
}