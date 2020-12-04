#include "step_listener_arduino.h"

StepListener::StepListener(uint8_t step, uint8_t dir, 
    uint8_t stepIn, uint8_t dirIn):step(step),dir(dir),stepIn(stepIn),dirIn(dirIn){

    pinMode(stepIn,OUTPUT);
    pinMode(dirIn,OUTPUT);

    pinMode(step,INPUT_PULLDOWN);
    pinMode(dir,INPUT_PULLDOWN);
}

void StepListener::readyToListen(){
    attachInterrupt(step, [this](void)->void{digitalWrite(stepIn,LOW);}, FALLING);
    attachInterrupt(step, [this](void)->void{digitalWrite(stepIn,HIGH);}, RISING);

    attachInterrupt(dir, [this](void)->void{digitalWrite(dirIn,LOW);}, FALLING);
    attachInterrupt(dir, [this](void)->void{digitalWrite(dirIn,HIGH);}, RISING);
    
}