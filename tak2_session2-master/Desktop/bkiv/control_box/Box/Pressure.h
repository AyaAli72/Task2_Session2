#ifndef PRESSURE_H
#define PRESSURE_H


#define D1_SEQUENCE 0x0F40      // pressure measurement sequence
#define D2_SEQUENCE 0x0F20      // temperature measurement sequence
#define WORD1_SEQUENCE 0x1D50   // word1 sequence
#define WORD2_SEQUENCE 0x1D60   // word2 sequence
#define WORD3_SEQUENCE 0x1D90   // word3 sequence
#define WORD4_SEQUENCE 0x1DA0   // word4 sequence
#define RESET_SEQUENCE 0x155540 // reset sequence

#define CONVERSION_DELAY_MS 35
#define COMM_FREQUENCY 500000

#define MCLK_PIN 2 // MCLK Pin
class PressureSensor{
public:
void pressureInit();
uint16_t readData(const uint16_t command, const unsigned long recvDelay = 0);
uint16_t pressureLoop();
//float getDepth(float pressure);
};
#endif
