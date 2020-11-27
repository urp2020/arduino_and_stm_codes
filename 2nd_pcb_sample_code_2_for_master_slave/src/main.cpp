#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "AccelStepper.h"
#include "Pinmap.h"


#define NUM_OF_MOTION 2
#define PI 3.141592
#define NUM_OF_STEPS 400
#define FPS 10
#define precision 25 //text_file number(string) maimum length 3.1415921

int motion_index = 0; //change motion index for changing motion
char *MOTIONS[NUM_OF_MOTION] = {"motion1.txt", "motion2.txt"};
SDLib::File current_motion;

AccelStepper master(AccelStepper::DRIVER,step_inside,dir_inside); //step, dir
AccelStepper slave(AccelStepper::DRIVER,step_outside,dir_outside);

char position_string[precision]; // read text file float number here.
int initial_position_mas,initial_position_slave;


void motor_ready(AccelStepper* motor);
void move_to_target_position(AccelStepper* master,float master_target,AccelStepper* slave,float slave_target);
void change_SD_file(SDLib::File* _current_motion);
float target_position_abs_position(float target);

//SETUP
void setup()
{
  pinMode(A2,OUTPUT);
  Serial.begin(9600);
  while (!Serial){;}
  Serial.print("Initializing SD card...");
  while(!SD.begin(sd_CS))
  {                                                                                                                                                                                                                                                                                                                                 
    Serial.println("initialization failed!");
  }
  Serial.println("initialization done.");

  current_motion = SD.open(MOTIONS[motion_index]);

  motor_ready(&master);
  motor_ready(&slave);

  initial_position_mas = analogRead(enc_inside); //READ INITIAL POSITION FROM MAGNETIC ENCODER
  //initial_position_slave = analogRead(enc_outside);
  //may be calculated for a time, and get average.
}


//LOOP
char read_from_text;
int idx=0;
float target_position_master,target_position_slave;
float abs_target_mas;
float encoder_pos;
int temp;
void loop(){

  if (current_motion.available()){

    read_from_text = current_motion.read();
    position_string[idx] = read_from_text;
    idx++;

    if(read_from_text == '\t'){ // TAB IS NEEDEDS
      target_position_master = atof(position_string);
      //update slave_target when it meets '\t'
      memset(position_string,0,precision);
      idx=0;

    }
    if(read_from_text == '\n'){
      target_position_slave = atof(position_string); //update master_target when it meets '\n'
      //Serial.println(target_position_slave);
        abs_target_mas = target_position_abs_position(target_position_slave);
      analogWrite(enc_outside,(int) target_position_slave%255);
      memset(position_string,0,precision);
      idx=0;
      move_to_target_position(&master, target_position_master, &slave, target_position_slave );// moving

    }
  }
  else{
    current_motion.close();
    current_motion = SD.open(MOTIONS[motion_index]);
    //current_motion.seek(0);
  }
    

  
  //at the end of the while loop (reading one line && moving), determine whether we should change motion or not.
  temp= analogRead(enc_inside);
  encoder_pos = (temp -initial_position_mas ) >0 ? (temp -initial_position_mas ): (temp -initial_position_mas )+1024;
  //we should change 0~1024 to 0~399
  encoder_pos = encoder_pos/1024.0*NUM_OF_STEPS; //ENCODER POSITION 
  


  if(current_motion.position() == 0){// IF NO FILE CHANGE, AT THE END OF MOTION FILE, WE SHOULD INITIALIZE ITS POSITION
    initial_position_mas = analogRead(enc_inside);
    //initial_position_slave = analogRead(enc_outside);

    master.setCurrentPosition(0);
    slave.setCurrentPosition(0);
    //change_SD_file(&current_motion);

  }


}

void motor_ready(AccelStepper* motor){
  motor->moveTo(400);
  motor->setAcceleration(200);
  motor->setMinPulseWidth(20);
  motor->setMaxSpeed(2000);//initialize stepper
  motor->setCurrentPosition(0);
}

void move_to_target_position(AccelStepper* master,float master_target,AccelStepper* slave,float slave_target){

  master->moveTo(master_target/360.0*NUM_OF_STEPS);
  master->setSpeed((master_target/360.0*NUM_OF_STEPS-master->currentPosition())*FPS );

  slave->moveTo(slave_target/360.0*NUM_OF_STEPS);
  slave->setSpeed((slave_target/360.0*NUM_OF_STEPS-slave->currentPosition())*FPS );

  while(master->distanceToGo()!=0 || slave->distanceToGo()!=0){
    master->runSpeedToPosition();
    slave->runSpeedToPosition();
  }
}

void change_SD_file(SDLib::File* _current_motion){//if there did not reach target, then change file
  _current_motion->close();// close file
  motion_index= (motion_index+1)%NUM_OF_MOTION; //motion index update
  *(_current_motion) = SD.open(MOTIONS[motion_index]); //open new motion
  Serial.println((*_current_motion).name());

  initial_position_mas = analogRead(enc_inside);
  //initial_position_slave = analogRead(enc_outside);

  master.setCurrentPosition(0);
  slave.setCurrentPosition(0);

}
float target_position_abs_position(float target){
    while(target<0 ||target>=400){
      if(target<0)
        target=target+400;
      if(target>400)
        target= target-400;
    }
  return target;
}

/*
   HAVE TO DO
   1. COMPARE MAG ENCODER, TARGET_POS AND DIFFRENT? THEN CHANGE FILE(CHANGE FILE CODE IS DONE)
   2. IT SHOULD BE CONSIDERED THAT SLAVE MOTOR EMIT IT'S ENC_OUTSIDE 0~255(ANALOG_WRITE), SO RESOLUTION IS VERY LOW. NOT 0~1023
   + AT THE END OF MOTION WE SHOULD MANIPULTE INITIAL POSITION OF ENC, AND motor
   3. MAKE SLAVE CODE. JUST 'STEP_LISTENER' AND 'ENCODER PART OUT NEEDED' ONLY(NOT SUBSTRACT INITIAL ENCODER. JUST SEND IT)

  make sd card file motion1, motion2, motion3

*/