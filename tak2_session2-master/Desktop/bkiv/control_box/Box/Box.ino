#include "TorpedoMPU.h"
#include "Pressure.h"
#include "Communication.h"
#include "CurrentSensor.h"
#include"Output_signals.h"
#include "softStart.h"

Thrusters thrusters;
Tools tools;
TorpedoMPU tmpu;
CurrentSensor CurrentS;
uint8_t box[] = {192, 168, 1, 7};
uint8_t console[] = {192, 168, 1, 9};
uint16_t imu[3] = { 1666, 898, 255 };
uint8_t current[2] = { 99, 20};
uint16_t pressure = 1024;
CommunicationClient client(18, 10, box, console);
PressureSensor pressurS;

uint8_t frame[] = {0, 1, 1, 1, 0, 1, 1, 0, 127 , 100 , 255 , 50 , 100 , 255 , 120, 255 , 33, 20, 40, 40, 20, 40, 30, 40};
uint8_t tool[] = {0, 255, 0, 255, 255, 255, 255, 255};

void setup() {
  Serial.begin(115200);
  client.Init();
  soft_start_initial_value();
  tools.init();

  thrusters.init();
  tmpu.start();
  tmpu.check();

  pressurS.pressureInit();

  thrusters.thrustersFrame = frame;
  tools.tool_frame = tool;

}

void loop() {

  tmpu.calculate();
  tools.apply_signal();
  thrusters.prep_And_apply_signal();
  //  Serial.print ("preassure: ");
  //  Serial.println (pressurS.pressureLoop());
  client.receiveData(tools.tool_frame, thrusters.thrustersFrame);
  client.sendData(tmpu.communication_data(), 3, CurrentS.Return_current(), 1, pressurS.pressureLoop());
  // delay(3000);
  tools.apply_signal();
  thrusters.prep_And_apply_signal();
}
