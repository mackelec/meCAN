
#include <MEcan.h>

meCAN1 can1;

#define LED PC13   // QFP64 socket PCB
HardwareSerial Serial1(PA10, PA9);



void setup() 
{
  pinMode(LED,OUTPUT);
  Serial1.begin(115200);
  bool success = can1.begin(125,PA11,PA12,STD_ID_LEN);
}

void loop() 
{
  digitalToggle(LED);
  Serial1.println("loop testing - meCAN 1-simple");
  readCAN1();
  sendCAN1();
  delay(1000);
}


void readCAN1()
{
  uint8_t rxdata[8];
  can1.rxMsgLen = can1.receive(can1.id, can1.fltIdx, rxdata);
  if (can1.rxMsgLen<1) return;
  reportCAN(1,can1.id,can1.rxMsgLen,rxdata);
}

void sendCAN1()
{
  unsigned char out[8] = {8, 7, 6, 5, 4, 3, 2, 1};
  bool result = can1.transmit(200,out,8);
  Serial1.print("CAN1 Transmit = "); Serial1.println(result);
  
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
