#ifndef SOFT_START_H
#define SOFT_START_H
 
#include <Servo.h>
extern float exponent;        //need to be defined at ".ino" file 

#define TIME_STEP 80.0        // time difference to use soft start

#define EXPONENT exponent     // affect rising rate for the motor
#define STEP_SPEED 30         // maximum speed difference to not use soft start 
#define MOT_MAX_SPEED 1900    // motors max speed 
#define MOT_MIN_SPEED 1100    // motors max speed in opposite direction
#define MOT_ZERO_SPEED 1500   // motors stop speed 
#define MOT_SPEED_DIFF 400.0  // difference between any max speed and the stop speed (1900-1500)
//calculate 
#define TIME_STEP_RETURN_UP (-log(-((*motor_controlled_speed) - MOT_MAX_SPEED) / MOT_SPEED_DIFF))*1000 / (TIME_STEP*EXPONENT)
#define TIME_STEP_RETURN_DOWN (-log(((*motor_controlled_speed) - MOT_MIN_SPEED) / MOT_SPEED_DIFF))*1000 / (TIME_STEP*EXPONENT)
//speed change vaules for speeds higher than 1500
#define SOFT_START_UP (MOT_MAX_SPEED - (MOT_SPEED_DIFF * exp(-EXPONENT *TIME_STEP * speed_counter[i] / 1000.0)))
//speed change vaules for speeds Lower than 1500
#define SOFT_START_DOWN (MOT_MIN_SPEED + (MOT_SPEED_DIFF * exp(-EXPONENT *TIME_STEP * speed_counter[i] / 1000.0)))
#define up 0
#define down 1



void soft_start_initial_value(); //to be called in setup(initiate time values) 
void soft_start( char type, int motor_speed, int *motor_controlled_speed, char i);//contorl soft start stages(speed changes)
void motor_soft_start (Servo motor_num, int motor_speed, int *motor_controlled_speed, char i , float EXP); //control speed coming form console

#endif
