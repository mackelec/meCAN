#include <PString.h>
#include <Streaming.h>
#include <meCAN.h>

meCAN1 can1;
meCAN2 can2;
meCAN1 canData;

#define LED     PC13 

void setup() 
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  
  can1.begin(125,PA11,PA12,STD_ID_LEN);
  can2.begin(125,PB12,PB13,STD_ID_LEN);
  can1.attachInterrupt(can1ISR);
}

void loop()
{
  readCAN2();
  delay(200);
}

void can1ISR() 
{
  readCAN1();
}



uint8_t theNumber=33;

void readCAN1()
{
  can1.rxMsgLen = can1.receive(can1.id, can1.fltIdx, can1.rxData.bytes);
  //rxMsgLen = CAN1receive(id, fltIdx, rxData.bytes);
  Serial << "Can len = " << can1.rxMsgLen << endl;
  if (can1.rxMsgLen<1) return;
  uint8_t c;
  theNumber = can2.rxData.bytes[0];
  if (can1.id != 100) reportCAN(&can1);
  sendCAN1();
}

void sendCAN1()
{
  unsigned char out[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  out[0] = theNumber;
  bool result = can1.transmit(102,out,8);
}

void readCAN2()
{
  can2.rxMsgLen = can2.receive(can2.id, can2.fltIdx, can2.rxData.bytes);
  if (can2.rxMsgLen<1) return;
  uint8_t c;
  theNumber = can2.rxData.bytes[0];
  if (can2.id == 102)
  {
    theNumber = can2.rxData.bytes[0] + 1; 
    digitalToggle(LED);
  }
  if (can1.id != 101) reportCAN(&can2);
  sendCAN2();
}

void sendCAN2()
{
  unsigned char out[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  out[0] = theNumber;
  bool result = can2.transmit(100,out,8);
}

void reportCAN(meCAN1 *can)
{
  Serial << "CAN1 Rx <id> " << can->id << "  [" << _HEX(can->id) << "]  " << endl ;
  for (int i=0;i<can->rxMsgLen;i++)
  {
    Serial << _HEX(can->rxData.bytes[i]) << " " ;
  }
  Serial << endl;
}

void reportCAN(meCAN2 *can)
{
  Serial << "CAN2 Rx <id> " << can->id << "  [" << _HEX(can->id) << "]  " << endl ;
  for (int i=0;i<can->rxMsgLen;i++)
  {
    Serial << _HEX(can->rxData.bytes[i]) << " " ;
  }
  Serial << endl;
}
