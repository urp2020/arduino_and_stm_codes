#ifndef STEP_LISTENER
#define STEP_LISTENER

#include "mbed.h"

#define DEFAULT_TICK_DURATION 1 // 1 second

class StepListener{
    private:
        InterruptIn* step;
        InterruptIn* dir;


        DigitalOut* stepIn;
        DigitalOut* dirIn;

        /*
        static const int tickDuration = DEFAULT_TICK_DURATION;
        unsigned int stepTicks = 0;
        unsigned int lastStepTicksPerDuration = 0;
        Ticker speedometer;
        
        */

        //below is taeyun's code

    public:
        
        StepListener(InterruptIn* step, InterruptIn* dir, 
                    DigitalOut* stepIn, DigitalOut* dirIn
                    );

        void readyToListen();
};

#endif 




