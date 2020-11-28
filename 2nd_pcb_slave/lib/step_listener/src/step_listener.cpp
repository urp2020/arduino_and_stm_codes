#include "step_listener.h"


StepListener::StepListener(InterruptIn* step, InterruptIn* dir, 
                    DigitalOut* stepIn, DigitalOut* dirIn){
    //Initialize member pins
    //Interrupt Service Rountines will be registered in StepListener::readyToListen function
    this->step = step;
    this->dir = dir;
    this->stepIn = stepIn;
    this->dirIn = dirIn;

}
void StepListener::readyToListen(){
    //Please Call once in setup routines

    //Interrrupt Service Routines are registered
    //C++ labmda expression is used -> see here : https://modoocode.com/196


    /*
    //Meausre stepSpeed per tickDuration
    speedometer.attach([this](void)->void{
        this->lastStepTicksPerDuration = this->stepTicks;
        this->stepTicks = 0;
    }, tickDuration);

    
    */
    stepIn->write(0);
    dirIn->write(0);

    this->step->mode(PullDown);
    this->step->rise( [this](void)->void{
        this->stepIn->write(1);
        //added by taeyun
    } );
    this->step->fall( [this](void)->void{this->stepIn->write(0);} );


    this->dir->mode(PullDown);
    this->dir->rise( [this](void)->void{this->dirIn->write(1);} );
    this->dir->fall( [this](void)->void{this->dirIn->write(0);} );


}
/*
double StepListener::currentSpeedStepsPerSeconds(){
    return this->lastStepTicksPerDuration/tickDuration;
}
*/

