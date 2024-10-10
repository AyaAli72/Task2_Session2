#include<Arduino.h>
#ifndef CURRENTSENSOR_H
#define CURRENTSENSOR_H
#define VIN A1
#define vcc 5.0

class CurrentSensor {
  public:
    uint8_t current[4] = {0,0,0,0};
    uint8_t getCurrent();
    uint8_t Return_current();
    
};
#endif
