#include <mbed.h>
#include "step_listener.h"
//#include Pinmap_v2.h" // for new pcb (with magnetic encoder)
#include "Pinmap_v1.h"  // for old pcb (with current sensor)

InterruptIn step(step_outside);
InterruptIn dir(dir_outside);

DigitalOut stepIn(step_inside);
DigitalOut dirIn(dir_inside);

int main(){
    StepListener driver(
        &step, &dir,
        &stepIn, &dirIn
    ); 
    driver.readyToListen();
    while(1){

    }
}