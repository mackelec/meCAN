#pragma once


#include <arduino.h>

#include <PString.h>

#include <Streaming.h>


#ifdef STM32F1xx
  #define _SERIES 1
#elif STM32F4xx
  #define _SERIES 4
#elif STM32L4xx
  #define _SERIES 14
#else
  #define _SERIES 0
#endif

#define PROC_F1 1
#define PROC_L4 14
#define PROC_F4 4


#ifdef STM32F1xx
  constexpr static uint32_t gpioABase = 0x40010800UL; // port A
#elif STM32F4xx
  constexpr static uint32_t gpioABase = 0x40020000UL; // port A
#elif STM32L4xx
  constexpr static uint32_t gpioABase = 0x48000000UL; // port A
#else
  #define _SERIES 0
#endif

constexpr static uint32_t gpioBBase = gpioABase + 0x400; // port B
constexpr static uint32_t gpioCBase = gpioABase + 0x800; // port C
constexpr static uint32_t gpioDBase = gpioABase + 0xC00; // port D
constexpr static uint32_t gpioEBase = gpioABase + 0x1000; // port E

//---- F1    GPIO/AFIO Regs

constexpr static uint32_t afioBase = 0x40010000UL;
constexpr static uint32_t mapr = afioBase + 0x004; // alternate pin function mapping

// Clock
constexpr static uint32_t rcc = 0x40021000UL;
constexpr static uint32_t rccBase = 0x40021000UL;
  //---- F1
constexpr static uint32_t apb1enr = rccBase + 0x01c;
constexpr static uint32_t apb2enr = rccBase + 0x018;
   //--- L4
constexpr static uint32_t ahb1enr = rccBase + 0x048;
constexpr static uint32_t ahb2enr = rccBase + 0x04C;
constexpr static uint32_t apb1enr1 = rccBase + 0x058;
constexpr static uint32_t apb1enr2 = rccBase + 0x05C;


constexpr static uint32_t moderA   = gpioABase;           // mode register
constexpr static uint32_t ospeedrA = gpioABase + 0x008; 
constexpr static uint32_t afrlA    = gpioABase + 0x020;   // alternate function
constexpr static uint32_t afrhA    = gpioABase + 0x024;

constexpr static uint32_t moderB   = gpioBBase;           // mode register
constexpr static uint32_t ospeedrB = gpioBBase + 0x008; 
constexpr static uint32_t afrlB    = gpioBBase + 0x020;   // alternate function
constexpr static uint32_t afrhB    = gpioBBase + 0x024;

constexpr static uint32_t moderC   = gpioCBase;           // mode register
constexpr static uint32_t ospeedrC = gpioCBase + 0x008; 
constexpr static uint32_t afrlC    = gpioCBase + 0x020;   // alternate function
constexpr static uint32_t afrhC    = gpioCBase + 0x024;

constexpr static uint32_t moderD   = gpioDBase;           // mode register
constexpr static uint32_t ospeedrD = gpioDBase + 0x008; 
constexpr static uint32_t afrlD    = gpioDBase + 0x020;   // alternate function
constexpr static uint32_t afrhD    = gpioDBase + 0x024;

constexpr static uint32_t moderE   = gpioEBase;           // mode register
constexpr static uint32_t ospeedrE = gpioEBase + 0x008; 
constexpr static uint32_t afrlE    = gpioEBase + 0x020;   // alternate function
constexpr static uint32_t afrhE    = gpioEBase + 0x024;
 
//  --- F1 series
constexpr static uint32_t crlA = gpioABase ; // cntrL reg 
constexpr static uint32_t crhA = gpioABase + 0x004; // cntrH reg 
constexpr static uint32_t odrA = gpioABase + 0x00c; // output data reg

constexpr static uint32_t crlB = gpioBBase ; // cntrL reg 
constexpr static uint32_t crhB = gpioBBase + 0x004; // cntrH reg 
constexpr static uint32_t odrB = gpioBBase + 0x00c; // output data reg

constexpr static uint32_t crlC = gpioCBase ; // cntrL reg 
constexpr static uint32_t crhC = gpioCBase + 0x004; // cntrH reg 
constexpr static uint32_t odrC = gpioCBase + 0x00c; // output data reg

constexpr static uint32_t crlD = gpioDBase ; // cntrL reg 
constexpr static uint32_t crhD = gpioDBase + 0x004; // cntrH reg 
constexpr static uint32_t odrD = gpioDBase + 0x00c; // output data reg

constexpr static uint32_t crlE = gpioEBase ; // cntrL reg 
constexpr static uint32_t crhE = gpioEBase + 0x004; // cntrH reg 
constexpr static uint32_t odrE = gpioEBase + 0x00c; // output data reg


  
//  ------------------   CAN1  ------------------

constexpr static uint32_t CANBase = 0x40006400UL;
constexpr static uint32_t mcr1 = CANBase ;
constexpr static uint32_t msr1 = CANBase + 0x04;
constexpr static uint32_t tsr1 = CANBase + 0x08;
constexpr static uint32_t can1_rf0r = CANBase + 0x0C;
constexpr static uint32_t can1_rf1r = CANBase + 0x10;
constexpr static uint32_t ier1 = CANBase + 0x14;
constexpr static uint32_t esr1 = CANBase + 0x18;
constexpr static uint32_t btr1 = CANBase + 0x1c;

constexpr static uint32_t fmr = CANBase + 0x200;
constexpr static uint32_t fm1r = CANBase + 0x204;
constexpr static uint32_t fs1r = CANBase + 0x20C;
constexpr static uint32_t ffa1r = CANBase + 0x214;
constexpr static uint32_t fa1r = CANBase + 0x21C;
constexpr static uint32_t fr1 = CANBase + 0x240;   // id/mask acceptance reg1
constexpr static uint32_t fr2 = CANBase + 0x244;   // id/mask acceptance reg2

constexpr static uint32_t tsr = CANBase + 0x008;  // tx status
constexpr static uint32_t ti0r = CANBase + 0x180;  // tx mailbox id
constexpr static uint32_t tdt0r = CANBase + 0x184; // tx data len and time stamp
constexpr static uint32_t tdl0r = CANBase + 0x188; // tx mailbox data[3:0]
constexpr static uint32_t tdh0r = CANBase + 0x18C; // tx mailbox data[7:4]

constexpr static uint32_t ri0r = CANBase + 0x1B0;  // rx fifo id reg
constexpr static uint32_t rdt0r = CANBase + 0x1B4; // fifo data len and time stamp
constexpr static uint32_t rdl0r = CANBase + 0x1B8; // rx fifo data low
constexpr static uint32_t rdh0r = CANBase + 0x1BC; // rx fifo data high



//  ------------------   CAN 2  ------------------

constexpr static uint32_t CAN2Base = 0x40006800;
constexpr static uint32_t mcr2 = CAN2Base + 0x000;  // master cntrl
constexpr static uint32_t msr2 = CAN2Base + 0x004;  // rx status
constexpr static uint32_t tsr2 = CAN2Base + 0x008;  // tx status
constexpr static uint32_t rf0r2 = CAN2Base + 0x0C;  // receive FIFO
constexpr static uint32_t ier2 = CAN2Base + 0x14;
constexpr static uint32_t btr2 = CAN2Base + 0x01C; // bit timing and rate
constexpr static uint32_t ti0r2 = CAN2Base + 0x180;  // tx mailbox id
constexpr static uint32_t tdt0r2 = CAN2Base + 0x184; // tx data len and time stamp
constexpr static uint32_t tdl0r2 = CAN2Base + 0x188; // tx mailbox data[3:0]
constexpr static uint32_t tdh0r2 = CAN2Base + 0x18C; // tx mailbox data[7:4]
constexpr static uint32_t ri0r2 = CAN2Base + 0x1B0; // rx FIFO id
constexpr static uint32_t rdt0r2 = CAN2Base + 0x1B4; // rx FIFO data
constexpr static uint32_t rdl0r2 = CAN2Base + 0x1B8; // rx FIFO data low
constexpr static uint32_t rdh0r2 = CAN2Base + 0x1BC; // rx FIFO data high
constexpr static uint32_t esr2 = CAN2Base + 0x018; // esr

/*
 * 
 *      System Control Space Base Address
 * 
 */

constexpr static uint32_t scsBase = 0xE000E000UL;        // System Control Space Base Address
constexpr static uint32_t nvicBase = scsBase + 0x0100UL; // NVIC Base Address
constexpr static uint32_t iser0 = nvicBase + 0x000;       //  NVIC interrupt set (enable)
constexpr static uint32_t iser1 = nvicBase + 0x004;
constexpr static uint32_t iser2 = nvicBase + 0x008;
constexpr static uint32_t icer = nvicBase + 0x080;       // NVIC interrupt clear (disable)

constexpr static uint32_t scbBase = scsBase + 0x0D00UL;
constexpr static uint32_t vtor = scbBase + 0x008;

#define MMIO32(x) (*(volatile uint32_t *)(x))
#define MMIO16(x) (*(volatile uint16_t *)(x))
#define MMIO8(x) (*(volatile uint8_t *)(x))

static inline volatile uint32_t &periphBit(uint32_t addr, int bitNum) // peripheral bit tool
{
  return MMIO32(0x42000000 + ((addr & 0xFFFFFFF) << 5) + (bitNum << 2)); // uses bit band memory
}


#define fmpie0 1 // rx interrupt enable on rx msg pending bit
#define INRQ      periphBit(mcr1, 0)
#define INAK      periphBit(msr1, 0)
#define FINIT     periphBit(fmr,  0)

#define INRQ2      periphBit(mcr2, 0)
#define INAK2      periphBit(msr2, 0)

#define SETBIT(port,_bit)     port |= 1UL << _bit
#define CLEARBIT(port,_bit)   port &= ~(1UL << _bit)

#define _SET(add,_bit)        MMIO32(add) |= 1UL << _bit
#define _CLEAR(add,_bit)      MMIO32(add) &= ~(1UL << _bit)

#ifndef PD0
  #define PD0 48
#endif

#ifndef PD1
  #define PD1 49
#endif

enum BitRate : uint16_t
{
  BR125K = 125,
  BR250K = 250,
  BR500K = 500, // 500K and faster requires good electical design practice
  BR1M = 1000
};

enum idtype : bool
{
  STD_ID_LEN,
  EXT_ID_LEN
};

union MSG {
  uint8_t bytes[8] = {0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff};
  int16_t int16[4];
  int32_t int32[2];
  int64_t int64;
};

void display32bit(String des,uint32_t value);

class meCAN1
{
private:
  idtype _extIDs = STD_ID_LEN;
  idtype _rxExtended;
  bool setBTR(uint32_t btr);
  void filterInit(void);
  void filter16Init(int bank, int mode, int a = 0, int b = 0, int c = 0, int d = 0); 
  void filter32Init(int bank, int mode, int a, int b);   
protected:
public:
  bool begin(int brp = 125, int Rx=PA11,int Tx=PA12,idtype addrType = STD_ID_LEN);
  void enableInterrupt(void);
  void disableInterrupt(void);
  void filterList16Init(int bank, int idA = 0, int idB = 0, int idC = 0, int idD = 0);
  void filterList32Init(int bank, int idA = 0, int idB = 0); 
  void attachInterrupt(void func());
  bool transmit(int txId, const void *ptr, unsigned int len);
  int receive(volatile int &id, volatile int &fltrIdx, volatile uint8_t pData[]);
  void filterMask16Init(int bank, int idA = 0, int maskA = 0, int idB = 0, int maskB = 0x7ff);
  void filterMask32Init(int bank, int id = 0, int mask = 0);
  bool getSilentMode() { return MMIO32(btr1) >> 31; }
  void setAutoTxRetry(bool val = true) { periphBit(mcr1, 4) = !val; } 
  void setSilentMode(bool val) { periphBit(btr1, 31) = val; }
  idtype getIDType() { return _extIDs; }
  idtype getRxIDType() { return _rxExtended; }
  uint8_t getRxMsgFifo0Cnt() {return MMIO32(can1_rf0r) & (3 << 0);} //num of msgs
  uint8_t getRxMsgFifo0Full() {return MMIO32(can1_rf0r) & (1 << 3);}
  uint8_t getRxMsgFifo0Overflow() {return MMIO32(can1_rf0r) & (1 << 4);} // b4
  volatile int rxMsgLen = -1; // CAN parms
  volatile int id, fltIdx;
  volatile MSG rxData;  
  ~meCAN1() {}
};



/**********************************************************
 * 
 * 
 *                         CAN 2
 * 
 * 
 ***********************************************************/



class meCAN2
{
private:
  idtype _extIDs = STD_ID_LEN;
  idtype _rxExtended;
  void setBTR(uint32_t btr);
  void filterInit(void);
  void filter16Init(int bank, int mode, int a = 0, int b = 0, int c = 0, int d = 0); 
  void filter32Init(int bank, int mode, int a, int b);   
protected:
public:
  void begin(int brp = 125, int Rx=PB12,int Tx=PB13,idtype addrType = STD_ID_LEN);
  void enableInterrupt(void);
  void disableInterrupt(void);
  void filterList16Init(int bank, int idA = 0, int idB = 0, int idC = 0, int idD = 0);
  void filterList32Init(int bank, int idA = 0, int idB = 0); 
  void attachInterrupt(void func());
  bool transmit(int txId, const void *ptr, unsigned int len);
  int receive(volatile int &id, volatile int &fltrIdx, volatile uint8_t pData[]);
  void filterMask16Init(int bank, int idA = 0, int maskA = 0, int idB = 0, int maskB = 0x7ff);
  void filterMask32Init(int bank, int id = 0, int mask = 0);
  bool getSilentMode() { return MMIO32(btr1) >> 31; }
  void setAutoTxRetry(bool val = true) { periphBit(mcr1, 4) = !val; } 
  void setSilentMode(bool val) { periphBit(btr1, 31) = val; }
  idtype getIDType() { return _extIDs; }
  idtype getRxIDType() { return _rxExtended; }
  uint8_t getRxMsgFifo0Cnt() {return MMIO32(can1_rf0r) & (3 << 0);} //num of msgs
  uint8_t getRxMsgFifo0Full() {return MMIO32(can1_rf0r) & (1 << 3);}
  uint8_t getRxMsgFifo0Overflow() {return MMIO32(can1_rf0r) & (1 << 4);} // b4
  volatile int rxMsgLen = -1; // CAN parms
  volatile int id, fltIdx;
  volatile MSG rxData;  
  ~meCAN2() {}
};
