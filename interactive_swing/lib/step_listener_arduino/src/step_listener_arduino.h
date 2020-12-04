#ifndef STEP_LISTENER_ARDUINO
#define STEP_LISTENER_ARDUINO

#include "Arduino.h"

class StepListener{
    private:
        uint8_t step;
        uint8_t dir;

        uint8_t stepIn;
        uint8_t dirIn;


    public:
        StepListener(uint8_t step, uint8_t dir,
                    uint8_t stepIn, uint8_t dirIn);

        void readyToListen();
};




#endif

