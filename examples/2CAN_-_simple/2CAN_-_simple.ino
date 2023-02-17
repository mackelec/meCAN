
#include "meClock_F446.h" //---- using external 8MHz crystal
#include <meCAN.h>

meCAN1 can1;
meCAN2 can2;

#define LED PC13   // QFP64 socket PCB
HardwareSerial Serial1(PA10, PA9);



void setup() 
{
  pinMode(LED,OUTPUT);
  Serial1.begin(115200);
  bool success;
  success = can1.begin(125,PA11,PA12,STD_ID_LEN);
  success = can2.begin(125,PB12,PB13,STD_ID_LEN);
}

void loop() 
{
  digitalToggle(LED);
  Serial1.println("loop testing - meCAN 1-simple");
  readCAN1();
  sendCAN1();
  delay(500);
  readCAN2();
  sendCAN2();
  delay(500);
}


void readCAN1()
{
  uint8_t rxdata[8];
  can1.rxMsgLen = can1.receive(can1.id, can1.fltIdx, rxdata);
  if (can1.rxMsgLen<1) return;
  reportCAN(1,can1.id,can1.rxMsgLen,rxdata);
}

void readCAN2()
{
  uint8_t rxdata[8];
  can2.rxMsgLen = can2.receive(can2.id, can2.fltIdx, rxdata);
  if (can2.rxMsgLen<1) {return;}
  reportCAN(2,can2.id,can2.rxMsgLen,rxdata);
}

void sendCAN1()
{
  unsigned char out[8] = {8, 7, 6, 5, 4, 3, 2, 1};
  bool result = can1.transmit(200,out,8);
  Serial1 << "CAN1 Transmit = " << result << endl;
}

void sendCAN2()
{
  unsigned char out[8] = {9, 10, 11, 12, 13, 14, 15, 16};
  bool result = can2.transmit(400,out,8);
  Serial1 << "CAN2 Transmit = " << result << endl;
}

void reportCAN(int canport,int id,int len,uint8_t data[])
{
  Serial1.print("CAN port = ");
  Serial1.print(canport);
  Serial1.print(" Rx <id> ");
  Serial1.println(id);
  for (int i=len-1;i>-1;i--)
  {
    Serial1.print(data[i]);Serial1.print(" ");
  }
  Serial1.println();
}
