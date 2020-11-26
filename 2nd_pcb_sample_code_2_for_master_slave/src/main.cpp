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

AccelStepper stepper1(AccelStepper::DRIVER,13,11); //step, dir

char position_string[precision]; // read text file float number here.
int initial_position;


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

  stepper1.moveTo(400);
  stepper1.setAcceleration(200);
  stepper1.setMinPulseWidth(20);
  stepper1.setMaxSpeed(2000);//initialize stepper
  stepper1.setCurrentPosition(0);

  initial_position = analogRead(A5); //READ INITIAL POSITION FROM MAGNETIC ENCODER
}


//LOOP
char read_from_text;
int idx=0;
float target_position;
void loop(){
  initial_position = analogRead(A5);
  analogWrite(A2,initial_position/4); // READ position and pass the value to
  
  if (current_motion.available()){

    read_from_text = current_motion.read();
    position_string[idx] = read_from_text;
    idx++;

    if(read_from_text == '\n'){
      target_position = atof(position_string);
      memset(position_string,0,precision);
      idx=0;
      Serial.println((target_position));
      stepper1.moveTo(target_position/360.0*NUM_OF_STEPS);
      stepper1.setSpeed((target_position/360.0*NUM_OF_STEPS-stepper1.currentPosition())*FPS );
      stepper1.runSpeedToPosition();

    }
    
  }
  else{
    //empty array
    target_position = atof(position_string);
    memset(position_string,0,precision);
    idx=0;
    //let stepper go
    Serial.println((target_position));
    stepper1.moveTo(target_position/360.0*NUM_OF_STEPS);
    stepper1.setSpeed((target_position/360.0*NUM_OF_STEPS-stepper1.currentPosition())*FPS );
    stepper1.runSpeedToPosition();

    current_motion.seek(0);
  }
    // close the file:
  
  // put your main code here, to run repeatedly:*/
}