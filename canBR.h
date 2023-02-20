
#include <arduino.h>

/*
 *  http://www.bittiming.can-wiki.info/
 */

typedef struct btrSet
{
  int canclock; // Mhz, clock speed of CAN peripheral
  int kBPS;     // kHz, CAN bus bits/Second
  uint32_t BTR; // BTR register value
};

const btrSet meBTRset[] = 
{
  #ifdef STM32F0
    //    Clock = 48Mhz
    {48,10,0x001c012b},
    {48,20,0x001c0095},
    {48,50,0x001c003b},
    {48,100,0x001c001d},
    {48,125,0x001c0017},
    {48,250,0x001c000b},
    {48,500,0x001c0005},
    {48,1000,0x001c0002},
  #endif
  #ifdef STM32F1
    //    Clock = 32Mhz
    {32,10,0x001c00c7},
    {32,20,0x001c0063},
    {32,50,0x001c0027},
    {32,100,0x001c0013},
    {32,125,0x001c000f},
    {32,250,0x001c0007},
    {32,500,0x001c0003},
    {32,1000,0x001c0001},
  #endif
  #ifdef STM32F4
    #ifdef STM32F413xx
      // Clock = 50Mhz
      {50,10,0x000701f3},
      {50,20,0x000700f9},
      {50,50,0x00070063},
      {50,100,0x00070031},
      {50,125,0x001c0018},
      {50,250,0x00070013},
      {50,500,0x00070009},
      {50,1000,0x00070004},
    #else
      //  Clock = 45Mhz for  F446,
      {45,10,0x001e00f9},
      {45,20,0x001e007c},
      {45,50,0x001e0031},
      {45,100,0x001e0018},
      {45,125,0x001e0013},
      {45,250,0x001e0009},
      {45,500,0x001e0004},
      {45,750,0x001b0003},
      {45,1000,0x001b0002},
    #endif
  #endif
  #ifdef STM32L4
    //    Clock = 80Mhz
    {80,10,0x001c01f3},
    {80,20,0x001c00f9},
    {80,50,0x001c0063},
    {80,100,0x001c0031},
    {80,125,0x001c0027},
    {80,250,0x001c0013},
    {80,500,0x001c0009},
    {80,1000,0x001c0004},
  #endif
  {0,0,0}
};





static int meCanClock()
{
  int canClock;
  uint32_t pclk1_frequency = HAL_RCC_GetPCLK1Freq();
  canClock = (int)pclk1_frequency/1000000;
  return canClock;
}

uint32_t meCanBTR(int bps)
{
  switch (bps)
  {
    case 10:
    case 20:
    case 50:
    case 100:
    case 125:
    case 250:
    case 500:
    case 1000:
      break;
    default:
      return 0;break;
  }
  int canClock=meCanClock();
  Serial1 << "Can Clock = " << canClock << endl;
  for (int i =0;i< (sizeof(meBTRset)/sizeof(meBTRset[0]));i++)
  {
    btrSet b = meBTRset[i];
    if (canClock != b.canclock) continue;
    if (bps != b.kBPS) continue;
    return b.BTR;
  }
  return 0;
}
