#include <PString.h>
#include <Streaming.h>

#include "meCAN.h"
#include "canPins.h"
#include "canBR.h"

bool meCAN1::begin(int brp,int Rx,int Tx,idtype addrType)
{

  uint32_t btr;
  __HAL_RCC_CAN1_CLK_ENABLE();

  #ifdef STM32F0xx
    #ifdef PA11_R
      if (Rx == PA11 || Tx == PA12)
      {
        __HAL_REMAP_PIN_ENABLE(HAL_REMAP_PA11_PA12);
      }
    #endif
  #endif
  
  bool success;
  success = setup_meCANpin(Rx);
  Serial1 << "Rx pin = " << success << endl;
  if (!success) return false;
  success = setup_meCANpin(Tx);
  Serial1 << "Tx pin = " << success << endl;
  if (!success) return false;
  btr = meCanBTR(brp);
  
  if (!setBTR(btr)) return false;
  filterInit(); 
  return true;
}


bool meCAN1::setBTR(uint32_t btr)
{
  uint32_t start;
  uint32_t _btr = btr;

  if (contributed_BTR !=0) _btr = contributed_BTR;
  //  set ABOM 
  // Disable ABOM
  CAN1->MCR &= ~CAN_MCR_ABOM;
    // exit sleep
  CAN1->MCR &= ~CAN_MCR_SLEEP;
    //  go into Init mode 
  CAN1->MCR |= CAN_MCR_INRQ; // Request initialization
  Serial1 << "CAN1->MCR = " << CAN1->MCR << endl;
  start = millis();
  while(1)
  {
    if ((CAN1->MSR & CAN_MSR_INAK) == CAN_MSR_INAK) break;
    if (millis() - start > 10) {return false;}
  }
  CAN1->BTR = _btr;
    // go into normal mode  
  // Leave the initialization mode and enter normal mode
  CAN1->MCR &= ~CAN_MCR_INRQ;
  start = millis();
  while ((CAN1->MSR & CAN_MSR_INAK) == CAN_MSR_INAK)
  {
  if (millis() - start > 10) {return false;}
  }
  return true;
}

void meCAN1::filterInit()
{
  // FINIT  'init' filter mode 
  SETBIT(CAN1->FMR,0);    // FINIT = 1
  
  CLEARBIT(CAN1->FA1R,0);      // de-activate filter 'bank'
  CLEARBIT(CAN1->FS1R,0);      // fsc filter scale reg,  0 => 2ea. 16b
  CLEARBIT(CAN1->FM1R,0);      // fbm list mode = 1, 0 = mask
  
  // Clear filter bank 1 and 2
  uint32_t *_FR1 = (uint32_t *)(CAN1_BASE + 0x240);
  uint32_t *_FR2 = (uint32_t *)(CAN1_BASE + 0x244);
  
  *_FR1 = 0L;
  *_FR2 = 0L;
  
  SETBIT(CAN1->FA1R,0);     // activate filter 'bank'
  CLEARBIT(CAN1->FMR,0);    // FINIT = 0
}



void meCAN1::filterList16Init(int bank, int idA, int idB, int idC, int idD)
{
  filter16Init(bank, 1, idA, idB, idC, idD);
}

void meCAN1::filter16Init(int bank, int mode, int a, int b, int c, int d)
{
  FINIT = 1;                                       // FINIT  'init' filter mode ]
  periphBit(fa1r, bank) = 0;                       // de-activate filter 'bank'
  periphBit(fs1r, bank) = 0;                       // fsc filter scale reg,  0 => 2ea. 16b
  periphBit(fm1r, bank) = mode;                    // fbm list mode = 1, 0 = mask
  MMIO32(fr1 + (8 * bank)) = (b << 21) | (a << 5); // fltr1,2 of flt bank n  OR  flt/mask 1 in mask mode
  MMIO32(fr2 + (8 * bank)) = (d << 21) | (c << 5); // fltr3,4 of flt bank n  OR  flt/mask 2 in mask mode
  periphBit(fa1r, bank) = 1;                       // activate this filter ]
  FINIT = 0;                                       // ~FINIT  'active' filter mode ]
}

void meCAN1::filter32Init(int bank, int mode, int a, int b) //32b filters
{
    FINIT = 1;               // FINIT  'init' filter mode ]
    periphBit(fa1r, bank) = 0;          // de-activate filter 'bank'
    periphBit(fs1r, bank) = 1;          // fsc filter scale reg,  0 => 2ea. 16b,  1=>32b
    periphBit(fm1r, bank) = mode;       // fbm list mode = 1, 0 = mask
    MMIO32(fr1 + (8 * bank)) = a << 21; // the RXID/MASK to match ]
    MMIO32(fr2 + (8 * bank)) = b << 21; // must replace a mask of zeros so that everything isn't passed
    periphBit(fa1r, bank) = 1;          // activate this filter ]
    FINIT = 0;               // ~FINIT  'active' filter mode ]
}


void meCAN1::enableInterrupt()
{
  
  #ifndef STM32F0xx
    NVIC_Set_Priority(20,2); //interrupt_prioity
  #else 
    NVIC_Set_Priority(30,2); //interrupt_prioity
  #endif
  uint32_t *_ISER = (uint32_t *)(NVIC_BASE);
  //---  set the fmpie0 bit, bit 1 to enable fifo interupt

  SETBIT(CAN1->IER,1); 
  #ifndef STM32F0xx
    SETBIT(*_ISER,20);
  #else 
    SETBIT(*_ISER,30);
  #endif
}
void meCAN1::disableInterrupt()
{
  uint32_t *_ISER = (uint32_t *)(NVIC_BASE);
  CLEARBIT(CAN1->IER,1); 
  SETBIT(*_ISER,20);
}


void meCAN1::attachInterrupt(void func()) // copy IRQ table to SRAM, point VTOR reg to it, set IRQ addr to user ISR
{
    #ifndef STM32F0xx
    static uint8_t newTbl[0xF0] __attribute__((aligned(0x100)));
    uint8_t *pNewTbl = newTbl;
    int origTbl = SCB->VTOR;  //MMIO32(vtor);
    for (int j = 0; j < 0x3c; j++) // table length = 60 integers
        MMIO32((pNewTbl + (j << 2))) = MMIO32((origTbl + (j << 2)));

    uint32_t canVectTblAdr = reinterpret_cast<uint32_t>(pNewTbl) + (36 << 2); // calc new ISR addr in new vector tbl
    MMIO32(canVectTblAdr) = reinterpret_cast<uint32_t>(func);                 // set new CAN/USB ISR jump addr into new table
    //MMIO32(vtor) = reinterpret_cast<uint32_t>(pNewTbl); 
    SCB->VTOR = reinterpret_cast<uint32_t>(pNewTbl);
    // load vtor register with new tbl location
    enableInterrupt();
    #endif
}

bool meCAN1::transmit(int txId, const void *ptr, unsigned int len)
{
    uint32_t tsr_reg = CAN1->TSR;
    uint32_t *_TIOR = (uint32_t *)(CAN1_BASE + 0x180);
    uint32_t *_TDT0R = (uint32_t *)(CAN1_BASE + 0x184);
    uint32_t *_TDL0R = (uint32_t *)(CAN1_BASE + 0x188);
    uint32_t *_TDH0R = (uint32_t *)(CAN1_BASE + 0x18C);


    if (!(tsr_reg & (1 << 26))) // tx mailbox 0 not ready
    {
        return false;
    }

    if (_extIDs)
        *_TIOR = (txId << 3) + 0b100; // set 29b extended ID
    else
        *_TIOR = (txId << 21) + 0b000; // set 12b standard ID

    *_TDT0R = len;
    // this assumes that misaligned word access works
    *_TDL0R = ((const uint32_t *)ptr)[0];
    *_TDH0R = ((const uint32_t *)ptr)[1];

    *_TIOR |= (1 << 0); // tx request
    return true;
}



int meCAN1::receive(volatile int &id, volatile int &fltrIdx, volatile uint8_t pData[])
{
    int len = -1;
    //uint32_t *_RF0R = (uint32_t *)(CAN1_BASE + 0x0C);
    uint32_t *_RI0R = (uint32_t *)(CAN1_BASE + 0x1B0);
    uint32_t *_RDT0R = (uint32_t *)(CAN1_BASE + 0x1B4);
    uint32_t *_RDL0R = (uint32_t *)(CAN1_BASE + 0x1B8);
    uint32_t *_RDH0R = (uint32_t *)(CAN1_BASE + 0x1BC);


    if (CAN1->RF0R & (3 << 0)) // num of msgs pending
    {
        if (_rxExtended)
            id = (*_RI0R >> 3); // extended id
        else
            id = (*_RI0R >> 21);          // std id
        len = *_RDT0R & 0x0F;             // fifo data len and time stamp
        fltrIdx = (*_RDT0R >> 8) & 0xff;  // filter match index. Index accumalates from start of bank
        ((uint32_t *)pData)[0] = *_RDL0R; // 4 low rx bytes
        ((uint32_t *)pData)[1] = *_RDH0R; // another 4 bytes
        CAN1->RF0R |= (1 << 5);                 // release the mailbox
    }
    return len;
}



void meCAN1::filterMask16Init(int bank, int idA, int maskA, int idB, int maskB) // 16b mask filters
{
    filter16Init(bank, 0, idA, maskA, idB, maskB); // fltr 1,2 of flt bank n
}

void meCAN1::filterMask32Init(int bank, int id, int mask) //32b filters
{
    filter32Init(bank, 0, id, mask);
}



//============================================  CAN 2  ==========================================




bool meCAN2::begin(int brp,int Rx,int Tx,idtype addrType)
{

  uint32_t btr;
  GPIO_InitTypeDef gpio_init;
  
  if (_SERIES == 0)  return false;
  #ifdef __HAL_RCC_CAN2_CLK_ENABLE
    __HAL_RCC_CAN2_CLK_ENABLE();
  #endif
  bool success;
  success = setup_meCANpin(Rx);
  if (!success) return false;
  success = setup_meCANpin(Tx);
  if (!success) return false;
  btr = meCanBTR(brp);
  if (!setBTR(btr))
  { 
    return false;
  }
  filterInit(); 
  return true;
}

bool meCAN2::setBTR(uint32_t btr)
{
#ifdef CAN2
  uint32_t start;
  uint32_t _btr = btr;

  if (contributed_BTR !=0) _btr = contributed_BTR;
  //  set ABOM 
  // Disable ABOM
  CAN2->MCR &= ~CAN_MCR_ABOM;
    // exit sleep
  CAN2->MCR &= ~CAN_MCR_SLEEP;
    //  go into Init mode 
  CAN2->MCR |= CAN_MCR_INRQ; // Request initialization
  start = millis();
  while(1)
  {
    if ((CAN2->MSR & CAN_MSR_INAK) == CAN_MSR_INAK) break;
    if (millis() - start > 10) {return false;}
  }
  CAN2->BTR = _btr;
    // go into normal mode  
  // Leave the initialization mode and enter normal mode
  CAN2->MCR &= ~CAN_MCR_INRQ;
  start = millis();
  while ((CAN2->MSR & CAN_MSR_INAK) == CAN_MSR_INAK)
  {
    if (millis() - start > 10) {return false;}
  }
#endif
  return true;
}


void meCAN2::filterInit()
{
#ifdef CAN2  
  // FINIT  'init' filter mode 
  SETBIT(CAN1->FMR,0);    // FINIT = 1
  
  CLEARBIT(CAN1->FA1R,14);      // de-activate filter 'bank'
  CLEARBIT(CAN1->FS1R,14);      // fsc filter scale reg,  0 => 2ea. 16b
  CLEARBIT(CAN1->FM1R,14);      // fbm list mode = 1, 0 = mask
  
  // Clear filter bank 1 and 2
  uint32_t *_FR1 = (uint32_t *)(CAN1_BASE + 0x240 + 8*14);
  uint32_t *_FR2 = (uint32_t *)(CAN1_BASE + 0x244 + 8*14);
  
  *_FR1 = 0L;
  *_FR2 = 0L;

  CAN1->FMR |= (0b001110 << 8); 
  SETBIT(CAN1->FA1R,14);     // activate filter 'bank'
  CLEARBIT(CAN1->FMR,0);    // FINIT = 0
  CAN2->ESR = 0;
  SETBIT(CAN2->RF0R,5);
#endif
}

void meCAN2::filter16Init(int bank, int mode, int a, int b, int c, int d)
{
  FINIT = 1;                                       // FINIT  'init' filter mode ]
  periphBit(fa1r, bank) = 0;                       // de-activate filter 'bank'
  periphBit(fs1r, bank) = 0;                       // fsc filter scale reg,  0 => 2ea. 16b
  periphBit(fm1r, bank) = mode;                    // fbm list mode = 1, 0 = mask
  MMIO32(fr1 + (8 * bank)) = (b << 21) | (a << 5); // fltr1,2 of flt bank n  OR  flt/mask 1 in mask mode
  MMIO32(fr2 + (8 * bank)) = (d << 21) | (c << 5); // fltr3,4 of flt bank n  OR  flt/mask 2 in mask mode
  periphBit(fa1r, bank) = 1;                       // activate this filter ]
  FINIT = 0;                                       // ~FINIT  'active' filter mode ]
}

int meCAN2::receive(volatile int &id, volatile int &fltrIdx, volatile uint8_t pData[])
{
#ifdef CAN2    
    int len = -1;
    //uint32_t *_RF0R = (uint32_t *)(CAN1_BASE + 0x0C);
    uint32_t *_RI0R = (uint32_t *)(CAN2_BASE + 0x1B0);
    uint32_t *_RDT0R = (uint32_t *)(CAN2_BASE + 0x1B4);
    uint32_t *_RDL0R = (uint32_t *)(CAN2_BASE + 0x1B8);
    uint32_t *_RDH0R = (uint32_t *)(CAN2_BASE + 0x1BC);

    if (CAN2->RF0R & (3 << 0)) // num of msgs pending
    {
        if (_rxExtended)
            id = (*_RI0R >> 3); // extended id
        else
            id = (*_RI0R >> 21);          // std id
        len = *_RDT0R & 0x0F;             // fifo data len and time stamp
        fltrIdx = (*_RDT0R >> 8) & 0xff;  // filter match index. Index accumalates from start of bank
        ((uint32_t *)pData)[0] = *_RDL0R; // 4 low rx bytes
        ((uint32_t *)pData)[1] = *_RDH0R; // another 4 bytes
        CAN2->RF0R |= (1 << 5);                 // release the mailbox
    }
    return len;
#endif
}



bool meCAN2::transmit(int txId, const void *ptr, unsigned int len)
{
#ifdef CAN2    
    uint32_t tsr_reg = CAN2->TSR;
    uint32_t *_TIOR = (uint32_t *)(CAN2_BASE + 0x180);
    uint32_t *_TDT0R = (uint32_t *)(CAN2_BASE + 0x184);
    uint32_t *_TDL0R = (uint32_t *)(CAN2_BASE + 0x188);
    uint32_t *_TDH0R = (uint32_t *)(CAN2_BASE + 0x18C);

    if (!(tsr_reg & (1 << 26))) // tx mailbox 0 not ready
    {
        return false;
    }
    if (_extIDs)
        *_TIOR = (txId << 3) + 0b100; // set 29b extended ID
    else
        *_TIOR = (txId << 21) + 0b000; // set 12b standard ID

    *_TDT0R = len;
    // this assumes that misaligned word access works
    *_TDL0R = ((const uint32_t *)ptr)[0];
    *_TDH0R = ((const uint32_t *)ptr)[1];

    *_TIOR |= (1 << 0); // tx request
#endif
    return true;
}


void meCAN2::enableInterrupt()
{
  #ifdef CAN2
  NVIC_Set_Priority(64,2);
  uint32_t *_ISER2 = (uint32_t *)(NVIC_BASE+8);
  //---  set the fmpie0 bit, bit 1 to enable fifo interupt

  SETBIT(CAN2->IER,1); 
  SETBIT(*_ISER2,0);
  #endif
}
void meCAN2::disableInterrupt()
{
  #ifdef CAN2
  uint32_t *_ISER = (uint32_t *)(NVIC_BASE);
  CLEARBIT(CAN2->IER,1); 
  SETBIT(*_ISER,21);
  #endif
}

void meCAN2::attachInterrupt(void func()) // copy IRQ table to SRAM, point VTOR reg to it, set IRQ addr to user ISR
{
    static uint8_t newTbl[0xF0] __attribute__((aligned(0x100)));
    uint8_t *pNewTbl = newTbl;
    int origTbl = MMIO32(vtor);
    for (int j = 0; j < 0x3c; j++) // table length = 60 integers
        MMIO32((pNewTbl + (j << 2))) = MMIO32((origTbl + (j << 2)));

    //uint32_t canVectTblAdr = reinterpret_cast<uint32_t>(pNewTbl) + (36 << 2); // calc new ISR addr in new vector tbl
    uint32_t canVectTblAdr = reinterpret_cast<uint32_t>(pNewTbl) + (320U); // calc new ISR addr in new vector tbl
    MMIO32(canVectTblAdr) = reinterpret_cast<uint32_t>(func);                 // set new CAN/USB ISR jump addr into new table
    MMIO32(vtor) = reinterpret_cast<uint32_t>(pNewTbl);                       // load vtor register with new tbl location
    enableInterrupt();
}

void meCAN2::filterList16Init(int bank, int idA, int idB, int idC, int idD)
{
  filter16Init(bank, 1, idA, idB, idC, idD);
}
void meCAN2::filterMask16Init(int bank, int idA, int maskA, int idB, int maskB) // 16b mask filters
{
    filter16Init(bank, 0, idA, maskA, idB, maskB); // fltr 1,2 of flt bank n
}

void NVIC_Set_Priority(uint32_t IRQn, uint32_t priority)
{
    /* Check if the IRQn is valid */
    if (IRQn < 0)
    {
        /* Set the priority for the Cortex-M4 system exceptions */
        SCB->SHP[((uint32_t)(IRQn) & 0xF) - 4] = (priority & 0xF) << 4;
    }
    else
    {
        /* Set the priority for the device-specific peripherals */
        NVIC->IP[IRQn] = (priority & 0xFF) << 4;
    }
}


//void display32bit(String des,uint32_t value)
//{
//  char ds[100];
//  uint32_t v1 = value;
//  
//  char hd[20];
//  char a;
//  int p=0;
//  int c=0;
//  int cc=0;
//  for (int i=31;i>=0;i--)
//  {
//    //bool b = (bool) (value & (1<<i));
//    bool b = (bool) bitRead(value,i);
//    ds[p] = b?49:48;
//    //a = b?48:49;
//    //Serial << a;
//    p++;
//    c++;
//    cc++;
//    if (c>3 && i>0)
//    {
//      ds[p] = '_';
//      p++;
//      c=0;
//    }
//    if (cc>7 && i>0)
//    {
//      ds[p] = '_';
//      p++;
//      ds[p] = '_';
//      p++;
//      cc=0;
//    }
//  }
//  ds[p]=0;
//  
//  String t = String(des);
//  t = String(t + "          ");
//  t = t.substring(0,9);
//  t = String(t + " = ");
//  String L = String(ds);
//  Serial.print(t);
//  Serial.print(L);
//  String v = String(v1);
//  v = String("     " + v);
//  Serial.println(v);
//}
