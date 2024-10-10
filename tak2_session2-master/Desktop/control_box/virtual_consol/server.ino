#include <UIPEthernet.h>
EthernetUDP udp;
uint8_t frame[] = {95,118,255,255,99,255,127,79,100,0,30,30,10,0,8,9,18,50};
IPAddress box(192, 168, 1, 7);

void setup() {
  Serial.begin(115200);
  uint8_t mac[6] = { 0x01, 0x02, 0x03, 0x04, 0x02, 0x05 };
  Ethernet.begin(mac, IPAddress(192, 168, 1, 9));
  
}
    
void loop() {
  
  udp.begin(7000);
  int success;
  String message = "";
    success = udp.parsePacket();
   
    if (success)
  {     uint8_t* msg = (uint8_t*)malloc(success+1);
        int len = udp.read(msg,success+1);
        msg[len]=0;
        Serial.print("received: '");
        for(int i=0 ;i<10;i++){
          Serial.print(msg[i]);
          Serial.print("\t");
        }
        Serial.println();
        free(msg);
        }

    udp.flush();
    int size=udp.beginPacket(box, 7000);
    size=udp.write(frame,18);
    //Serial.println(size);
    udp.endPacket();
    udp.stop();

}
