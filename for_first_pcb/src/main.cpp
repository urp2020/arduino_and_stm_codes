#include <mbed.h>
#include "step_listener.h"
#include "def_pins.h"

InterruptIn step(external_step);
InterruptIn dir(external_dir);

DigitalOut stepIn(internal_step);
DigitalOut dirIn(internal_dir);

int main(){
    StepListener driver(
        &step, &dir,
        &stepIn, &dirIn
    ); 
    driver.readyToListen();
    while(1){

    }
}