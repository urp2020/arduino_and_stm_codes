#include <Arduino.h>
#include "AccelStepper.h"
#include "Pinmap.h"

#define PI 3.141592
#define STEPS_PER_REVOLUTION 400
#define MAX_REVOLUTION_PER_SECOND 5
#define FPS 10
#define VOLTAGE_LEVEL_PER_ENCODER_REVOLUTION 1024.0

int tolerance = 10; //steps =  9 Degree
double tolerance_sin_value = 0.2;//sin(10*PI/180);

AccelStepper master(AccelStepper::DRIVER, step_inside, dir_inside);
long current_angle;
long command_angle;
float command_speed;

double current_angle_rad;
double command_angle_rad;

void init_motor(AccelStepper* motor, long initial_position){
    motor->moveTo(STEPS_PER_REVOLUTION);
    motor->setAcceleration(STEPS_PER_REVOLUTION/2);
    motor->setMinPulseWidth(20);
    motor->setMaxSpeed(MAX_REVOLUTION_PER_SECOND*STEPS_PER_REVOLUTION);
    motor->setCurrentPosition(initial_position);
}

double get_current_angle_as_ratio(uint32_t encoder_pin){
    // 1 is 'a' revolution (360 degree)
    uint32_t voltage_level = analogRead(encoder_pin);
    return (double)(voltage_level/VOLTAGE_LEVEL_PER_ENCODER_REVOLUTION);
}
long get_current_angle_as_steps(uint32_t encoder_pin){
    return (long)(get_current_angle_as_ratio(encoder_pin)*STEPS_PER_REVOLUTION);
}
double get_current_angle_as_radian(uint32_t encoder_pin){
    return get_current_angle_as_ratio(encoder_pin)*2*PI;
}
long get_command_angle_as_steps(AccelStepper* stepper){
    long position = stepper->currentPosition();
    position = position%MAX_REVOLUTION_PER_SECOND;
    if(position < 0){
        return 0;
    }else{
        return position;
    }
}
double get_command_angle_as_radian(AccelStepper* stepper){
    return ((double)get_command_angle_as_steps(stepper))/STEPS_PER_REVOLUTION*2*PI;
}

void setup(){
    Serial.begin(9600);
    current_angle = get_current_angle_as_steps(enc_inside); //initial position
    init_motor(&master, current_angle);
    command_speed = 200;
}

void loop(){
    if(command_speed != master.speed()){
        master.setSpeed(command_speed);
    }

    master.runSpeed();
    current_angle = get_current_angle_as_steps(enc_inside);
    command_angle = get_command_angle_as_steps(&master);

    current_angle_rad = get_current_angle_as_radian(enc_inside);
    command_angle_rad = get_command_angle_as_radian(&master);

    double difference_angle_rad = command_angle_rad - current_angle_rad;
    double difference_sin_value = sin(difference_angle_rad);
    if(difference_sin_value>0){
        // command is leading, real angle is lagging
        if(abs(difference_sin_value) > tolerance_sin_value){
            // force applied
            command_speed = -command_speed; //flip
        }else{
            //pass
        }
    }else{
        // command is lagging, real angle is leading
        if(abs(difference_sin_value) > tolerance_sin_value){
            command_speed += 200; //accelerate;
        }
    }
}