#include "CurrentSensor.h"

uint8_t CurrentSensor::getCurrent() {
  double voltage =   (5.0 / 1023.0) * analogRead(VIN); // Read the voltage from sensor
  voltage =  voltage - (vcc * 0.5) + 0.007 ;// 0.007 is a value to make voltage zero when there is no current
  current[0] = round(voltage / 0.04);//Factor differs from a model to another 
}
uint8_t CurrentSensor::Return_current(){
 getCurrent();
 return this->current;
}
