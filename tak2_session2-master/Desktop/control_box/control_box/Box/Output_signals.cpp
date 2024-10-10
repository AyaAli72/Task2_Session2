#include "Output_signals.h"
#include "softStart.h"

Servo Motors[motorsCount];
int motor_controlled_speed[motorsCount] = {MOT_ZERO_SPEED, MOT_ZERO_SPEED, MOT_ZERO_SPEED, MOT_ZERO_SPEED, MOT_ZERO_SPEED, MOT_ZERO_SPEED, MOT_ZERO_SPEED, MOT_ZERO_SPEED};

void Thrusters::init(){
  for(int i = 0 ; i < motorsCount ; i++){  
    Motors[i].attach(motorPins[i]);
    Motors[i].writeMicroseconds(MOT_ZERO_SPEED);//stop speed 1500
  }
delay(2000);
}

void Tools::init(){
  for(int i = 0 ; i < toolsCount ; i++){
    pinMode(toolPins[i] , OUTPUT); //set tool pins to be Output pins 
  }
}

//set signals to tools
void Tools::apply_signal(){
  for(int i = 0 ; i < toolsCount ; i++){
    analogWrite(toolPins[i] , this->tool_frame[i]);
  }
  
}

//set signals to Motors
void Thrusters::apply_signal(int *thrustersSpeeds ,float *exponants){
   for(int i = 0 ; i < motorsCount ; i++){
    //call soft start to control motors speed 
    motor_soft_start (Motors[i], thrustersSpeeds[i] ,(motor_controlled_speed + i), i ,exponants[i]);
  }
}


//prepare motor speeds and exponants to be sent to motors
void Thrusters::prep_And_apply_signal(){
   float exponants[motorsCount];
   int thrustersSpeeds[motorsCount];
    
   for(int i = 0 ; i < motorsCount ; i++){
     int dir = (this->thrustersFrame[i] == 1)? 1 : -1;
     int speedChange = map(this->thrustersFrame[i+8],0,255,0,400);
     int speedValue = 1500 + dir * speedChange;
     thrustersSpeeds[i] = speedValue;
     exponants[i] = (int)thrustersFrame[i+16] / 10.0 ; 
   }
   
 this->apply_signal(thrustersSpeeds ,exponants);
}
