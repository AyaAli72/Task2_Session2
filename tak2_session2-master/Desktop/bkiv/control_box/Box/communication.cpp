#include "communication.h"

//initiatize the ethertnet module
void CommunicationClient::Init() {
  Ethernet.begin(this->mac, this->BOXIP);
}
//setting ips of box and console
void CommunicationClient::ips_setting(IPAddress Box, IPAddress Console) {

}
// receiving function
void CommunicationClient::receiveData(uint8_t *acc_frame, uint8_t *thrus_frame) {
  this->udp.begin(this->BOXPort);
  int success;
  String message = "";
//  do
//  {
    success = udp.parsePacket();

//  }
//  while (!success);
  Serial.println(success);
  if (success)
  { uint8_t* msg = (uint8_t*)malloc(success + 1);
    int len = udp.read(msg, success + 1);
    msg[len] = 0;
    Serial.print("received: '");
    Serial.println(msg[0]);
    free(msg);
  }

  udp.flush();

}

void CommunicationClient::sendData(int *imu_frame, int imu_size, uint8_t current, int current_size, uint16_t pressure) {
  prep_to_send(imu_frame,  imu_size, current, current_size, pressure);
//  Serial.println("here ");
  //  Serial.println(data_sent[4]);
  udp.beginPacket(IPAddress(192, 168, 1, 9), 7000);
  int size = udp.write(data_sent, 10);
  Serial.println(size);
  udp.endPacket();

  udp.stop();
}
void CommunicationClient::prep_to_send(int *imu_frame, int imu_size, uint8_t current, int current_size, uint16_t pressure) {
  int x = 0;
  for (int j = 0; j < imu_size; j++) {
    this->data_sent[x++] = lowByte(imu_frame[j]);
    this->data_sent[x++] = highByte(imu_frame[j]);
  }

  this->data_sent[x++] = current;


  this->data_sent[x++] = lowByte(pressure);
  this->data_sent[x++] = highByte(pressure);
}
