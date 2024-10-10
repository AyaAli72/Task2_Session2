#include "softStart.h"
#include "Arduino.h"
float exponent = 0;

int dir = up; // direction of increasing the speed
long int time1[8], time2[8], speed_counter[8] = {0};
//speed_counter is a counter of steps taken by the motor to reach the wanted speed

void soft_start_initial_value() {
  //needed to check time step
  for (char i = 0; i < 8; i++) {
    time1[i] = millis();
    time2[i] = millis();
  }
}
void soft_start( char type, int motor_speed, int *motor_controlled_speed, char i) {
  //check if TIME_STEP has been passed or not
  if ( (time2[i] - time1[i]) >= TIME_STEP) {
    time1[i] = millis(); //update time1 for the motor
    speed_counter[i]++; //update the number of stages
    //give the motor the next stage speed
    switch (type) {
      case up:
        *motor_controlled_speed = SOFT_START_UP;
        break;
      case down:
        *motor_controlled_speed = SOFT_START_DOWN;
        break;
    }
    //if speed difference <= 30 no more soft start needed
    if (abs(motor_speed - *motor_controlled_speed) <= STEP_SPEED) {
      *motor_controlled_speed = motor_speed;
      speed_counter[i] = 0; // set stage count back to 0
    }
  }
  time2[i] = millis(); //update time2 for the motor
}


void motor_soft_start (Servo motor_num, int motor_speed, int *motor_controlled_speed, char i , float EXP) {
  exponent = EXP ;
  //motor speed limits
  if (motor_speed < 1100 || motor_speed > 1900)
    motor_speed = 1500;
  if ( motor_speed == MOT_ZERO_SPEED ) {
    *motor_controlled_speed = MOT_ZERO_SPEED;//stop speed
    speed_counter[i] = 0;
  }
  //if changing direction => set speed to (stop speed) first
  else if ( (motor_speed > MOT_ZERO_SPEED) && (*motor_controlled_speed < MOT_ZERO_SPEED) ) {
    *motor_controlled_speed = MOT_ZERO_SPEED;
    speed_counter[i] = 0;
  }
  //lowering the speed doesnt require soft start
  else if ( (motor_speed > MOT_ZERO_SPEED) && (motor_speed < *motor_controlled_speed)) {
    *motor_controlled_speed = motor_speed;
    speed_counter[i] = TIME_STEP_RETURN_UP;
    Serial.print("(no soft start) TIME_STEP_RETURN_UP : ");
    Serial.print(TIME_STEP_RETURN_UP);
    Serial.print("    , Speed counter : ");
    Serial.print(speed_counter[i]);
    Serial.print("    , Exp : ");
    Serial.println(EXPONENT);
  }
  //if changing direction => set speed to (stop speed) first
  else if ( (motor_speed < MOT_ZERO_SPEED) && (*motor_controlled_speed > MOT_ZERO_SPEED) ) {
    *motor_controlled_speed = MOT_ZERO_SPEED;
    speed_counter[i] = 0;
  }
  //lowering the speed doesnt require soft start
  else if ((motor_speed < MOT_ZERO_SPEED) && (motor_speed > *motor_controlled_speed)) {
    *motor_controlled_speed = motor_speed;
    speed_counter[i] = TIME_STEP_RETURN_DOWN;
    //    Serial.print("(no soft start) TIME_STEP_RETURN_DOWN : ");
    //     Serial.print(TIME_STEP_RETURN_DOWN);
    //     Serial.print("    , Speed counter : ");
    //     Serial.print(speed_counter[i]);
    //     Serial.print("    , Exp : ");
    //     Serial.println(EXPONENT);
  }
  // soft start up
  else if ( (motor_speed > MOT_ZERO_SPEED) && (*motor_controlled_speed >= MOT_ZERO_SPEED) && (*motor_controlled_speed < motor_speed) ) {
    if (speed_counter[i] == 0) {
      speed_counter[i] = TIME_STEP_RETURN_UP;//update the count of speed change
    }
    //     Serial.print("TIME_STEP_RETURN_UP : ");
    //    Serial.print(TIME_STEP_RETURN_UP);
    //    Serial.print("    , Speed counter : ");
    //     Serial.print(speed_counter[i]);
    //     Serial.print("    , Exp : ");
    //     Serial.println(EXPONENT);
    soft_start(up, motor_speed, motor_controlled_speed, i);//control speed rate of change
    dir = up;
  }

  // soft start down
  else if ( (motor_speed < MOT_ZERO_SPEED) && (*motor_controlled_speed <= MOT_ZERO_SPEED) && (*motor_controlled_speed > motor_speed) ) {
    if (speed_counter[i] == 0) {
      speed_counter[i] = TIME_STEP_RETURN_DOWN;//update the count of speed change
    }
    //    Serial.print("TIME_STEP_RETURN_DOWN : ");
    //     Serial.print(TIME_STEP_RETURN_DOWN);
    //     Serial.print("    , Speed counter : ");
    //     Serial.print(speed_counter[i]);
    //     Serial.print("    , Exp : ");
    //     Serial.println(EXPONENT);
    soft_start(down, motor_speed, motor_controlled_speed, i);//control speed rate of change
    dir = down;
  }

  //Serial.println(*motor_controlled_speed);

  //set controlled speed to the motor
  motor_num.writeMicroseconds(*motor_controlled_speed);

}
