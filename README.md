# meCAN - Version 2023

## Arduino CAN Library for stm32 bxCAN

meCAN is an Arduino library that provides an easy-to-use interface for communicating over the Controller Area Network (CAN) bus. It has been designed to work with several STM32 microcontroller families, including:

 - STM32F0xx 
 - STM32F1xx 
 - STM32F4xx 
 - STM32L4xx.

The library allows you to control up to two CAN ports, with plans to support three ports in the future. It also supports receive interrupts on all ports and list and mask filters on all ports.

####  meFDCAN -  Arduino CAN library for the STM32G4xx series.  https://github.com/mackelec/meFDCAN
- The STM32G4xx utilises the FDCAN peripheral as opposed to the bxCAN which the STM32F series use.  
- meFDCAN provide simple control up to three CAN ports (STM32G473).

#### meCANbuffer - https://github.com/mackelec/meCANbuffer
- A circurlar FIFO buffer that can be used for Received CAN data.
#### External crystals - SystemClock_Config  https://github.com/mackelec/STM32-External-Crystals---SystemClock_Config

####  Plenty of meCAN Examples.  Enjoy

```
#include <meCAN.h>

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
  Serial1 << "CAN1 Transmit = " << result << endl;
  
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


```
