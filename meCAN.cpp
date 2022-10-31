

#include "meCAN.h"


bool meCAN1::begin(int brp,int Rx,int Tx,idtype addrType)
{

  uint32_t btr;
  
  if (_SERIES == 0)  return false;
  if (_SERIES == PROC_F1)
  {
    /*
     *  http://www.bittiming.can-wiki.info/
     * 
     *  use clock rate of 32Mhz 
     */
    switch (brp)
    {
      case 50:
        btr = 0x001e0027;
        break;
      case 125:
        btr = 0x001e000f;
        break;
      case 250:
        btr = 0x001e0007;
        break;
      case 500:
        btr = 0x001e0003;
        break;
      case 1000:
        btr = 0x001e0001;
        break;     
    }
    //btr = (3 << 20) | (12 << 16) | (15 << 0);
    
    switch (Rx)
    {
      //-------  TX pins are sset according to RX pin
      
      case PA11:
        MMIO32(apb2enr) |= (1 << 2) | (1 << 0); // enable gpioA = b2 and afio = b0 clks
        MMIO32(mapr) &= 0xffff9fff;             // CAN map to default pins, PA11/12
        MMIO32(crhA) &= 0xFFF00FFF;             // clear control bits for pins 11 & 12 of Port A
        MMIO32(crhA) |= 0b0100 << 12;        // pin11 for rx, b0100 = b01xx, floating, bxx00 input
        periphBit(odrA, 11) = 0;           //
        MMIO32(crhA) |= 0b1001 << 16;           // PA12 set for AF-PP-fast
        break;
      case PB8:
        MMIO32(apb2enr) |= (1 << 3) | (1 << 0); // enable gpioB = b3 and afio = b0 clks
        MMIO32(mapr) |= (2 << 13);              // alt func, CAN remap to B9+B8 
        MMIO32(crhB) &= 0xFFFFFF00;             // clear control bits for pins 8 & 9 of Port B
        MMIO32(crhB) |= 0b0100;              // pin8 for rx, b0100 = b01xx, floating, bxx00 input
        periphBit(odrB, 8) = 0;            // set input will pullup resistor for single wire with pullup mode
        MMIO32(crhB) |= 0b1001 << 4;            // set output
        break;
    }
    periphBit(apb1enr, 25) = 1;      // enable CAN1
     
  }

        //---------------
        //     L4 series
        //---------------
  
  else if(_SERIES == PROC_L4)
  {
   
    /*
     *  http://www.bittiming.can-wiki.info/
     * 
     *  use clock rate of 80Mhz
     */
    switch (brp)
    {
      case 50:
        btr = 0x001c0063;
        break;
      case 125:
        btr = 0x001c0027;
        break;
      case 250:
        btr = 0x001c0013;
        break;
      case 500:
        btr = 0x001c0009;
        break;
      case 1000:
        btr = 0x001c0004;
        break;     
    }

    // enable CAN1 clk
        periphBit(apb1enr1,25)=1;
    
    switch (Rx)
    {
      case PA11:                // AF9
          // enable PortA clk
        periphBit(ahb2enr,0)=1;
          // setup PA11 for rx and A12 as Tx, as alternate mode
        _CLEAR(moderA,22);
        _SET(moderA,23);
          // set pin modes as high speed
        _CLEAR(ospeedrA,22);
        _SET(ospeedrA,23); 
          // set pin modes as alternate mode 9 (pins PA11)
        MMIO32(afrhA) &= 0xFFFF0FFF; // clear nibble
        MMIO32(afrhA) |= (0x9 << 12);   // pin PA11
        break;

      case PB8:                 // AF9                          wary of boot loading
        periphBit(ahb2enr,1)=1;     // enable port B clk
        _CLEAR(moderB,16);          // setup PB8 for AF mode  (0b10)
        _SET(moderB,17);
        _CLEAR(ospeedrB,16);        // High Speed mode (0b10)
        _SET(ospeedrB,17);
        MMIO32(afrhB) &= 0xFFFFFFF0; // clear nibble
        MMIO32(afrhB) |= (0x9 << 0);   // set to AF9
        break;

      case PB12:                // AF10
        periphBit(ahb2enr,1)=1;     // enable port B clk
        _CLEAR(moderB,24);          // setup PB12 for AF mode  (0b10)
        _SET(moderB,25);
        _CLEAR(ospeedrB,24);        // High Speed mode (0b10)
        _SET(ospeedrB,25);
        MMIO32(afrhB) &= 0xFFF0FFFF; // clear nibble
        MMIO32(afrhB) |= (0xA << 16);   // set to AF10
        break;
        
      case PB5:                 // AF3
        periphBit(ahb2enr,1)=1;     // enable port B clk
        _CLEAR(moderB,10);          // setup for AF mode  (0b10)
        _SET(moderB,11);
        _CLEAR(ospeedrB,10);        // High Speed mode (0b10)
        _SET(ospeedrB,11);
        MMIO32(afrlB) &= 0xFF0FFFFF; // clear nibble
        MMIO32(afrlB) |= (0x3 << 20);   // set to AF3
        break;

      case PD0:                 // AF9
        periphBit(ahb2enr,3)=1;     // enable port D clk
        _CLEAR(moderD,0);          // setup for AF mode  (0b10)
        _SET(moderD,1);
        _CLEAR(ospeedrD,0);        // High Speed mode (0b10)
        _SET(ospeedrD,1);
        MMIO32(afrlD) &= 0xFFFFFFF0; // clear nibble
        MMIO32(afrlD) |= (0x9 << 0);   // set to AF3
        break;
        
      default:
        // error
        break;
    }
    switch (Tx)
    {
      case PA12:                // AF9
          // enable PortA clk
        periphBit(ahb2enr,0)=1;
          // setup  A12 as Tx, as alternate mode
        _CLEAR(moderA,24);
        _SET(moderA,25);
          // set pin modes as high speed
        _CLEAR(ospeedrA,24);
        _SET(ospeedrA,25); 
           // set pin modes as alternate mode 9 (pins PA12)
        MMIO32(afrhA) &= 0xFF0FFFFF; // clear nibble
        MMIO32(afrhA) |= (0x9 << 16);   // pin PA12
        break;

      case PB9:                 // AF9
        periphBit(ahb2enr,1)=1;     // enable port B clk
        _CLEAR(moderB,18);          // setup for AF mode  (0b10)
        _SET(moderB,19);
        _CLEAR(ospeedrB,18);        // High Speed mode (0b10)
        _SET(ospeedrB,19);
        MMIO32(afrhB) &= 0xFFFFFF0F; // clear nibble
        MMIO32(afrhB) |= (0x9 << 4);   // set to AF9
        break;

      case PB13:                // AF10
        periphBit(ahb2enr,1)=1;     // enable port B clk
        _CLEAR(moderB,26);          // setup for AF mode  (0b10)
        _SET(moderB,27);
        _CLEAR(ospeedrB,26);        // High Speed mode (0b10)
        _SET(ospeedrB,27);
        MMIO32(afrhB) &= 0xFF0FFFFF; // clear nibble
        MMIO32(afrhB) |= (0xA << 20);   // set to AF10
        break;
      
      case PB6:                 // AF8
        periphBit(ahb2enr,1)=1;     // enable port B clk
        _CLEAR(moderB,12);          // setup for AF mode  (0b10)
        _SET(moderB,13);
        _CLEAR(ospeedrB,12);        // High Speed mode (0b10)
        _SET(ospeedrB,13);
        MMIO32(afrlB) &= 0xF0FFFFFF; // clear nibble
        MMIO32(afrlB) |= (0x8 << 24);   // set to AF8
        break;

      case PD1:                 // AF9
        periphBit(ahb2enr,3)=1;     // enable port D clk
        _CLEAR(moderD,2);          // setup for AF mode  (0b10)
        _SET(moderD,3);
        _CLEAR(ospeedrD,2);        // High Speed mode (0b10)
        _SET(ospeedrD,3);
        MMIO32(afrlD) &= 0xFFFFFF0F; // clear nibble
        MMIO32(afrlD) |= (0x9 << 4);   // set to AF3
      default:
        // error
        break;
    }
  }

//---------------
//     F4 series
//---------------

  
  else if(_SERIES == PROC_F4)
  {
    
    /*
     *  http://www.bittiming.can-wiki.info/
     * 
     *  use clock rate of 45Mhz
     */
    switch (brp)
    {
      case 50:
        btr = 0x001e0031;
        break;
      case 125:
        btr = 0x001e0013;
        break;
      case 250:
        btr = 0x001e0009;
        break;
      case 500:
        btr = 0x001e0004;
        break;
      case 1000:
        btr = 0x001b0002;
        break;     
    }
  }     //------  END of Series Types

  
  if (!setBTR(btr)) return false;
  filterInit(); 
  return true;
}


bool meCAN1::setBTR(uint32_t btr)
{
  uint32_t start;
  //  set ABOM 
  periphBit(mcr1,6)=1;
    // exit sleep
  periphBit(mcr1, 1) = 0; 
    //  go into Init mode 
  INRQ = 1;
  start = millis();
  while(1)
  {
    if (INAK == 1) break;
	if (millis() - start > 10) return false;
  }
  MMIO32(btr1) = btr; 
    // go into normal mode  
  INRQ = 0;
  start = millis();
  while(INAK)
  {
	if (millis() - start > 10) return false;
  }
  return true;
}

void meCAN1::filterInit()
{
  FINIT = 1;                                // FINIT  'init' filter mode ]
  periphBit(fa1r, 0) = 0;                       // de-activate filter 'bank'
  periphBit(fs1r, 0) = 0;                       // fsc filter scale reg,  0 => 2ea. 16b
  periphBit(fm1r, 0) = 0;                       // fbm list mode = 1, 0 = mask
 
  MMIO32(fr1) = 0L;
  MMIO32(fr2) = 0L;
  periphBit(fa1r, 0) = 1;                          // activate filter 'bank'
  FINIT = 0;                         
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
  periphBit(ier1, fmpie0) = 1U; // set fifo RX int enable request
  MMIO32(iser0) = 1UL << 20;
}
void meCAN1::disableInterrupt()
{
  periphBit(ier1, fmpie0) = 0U;
  MMIO32(iser0) = 1UL << 20;
}

void meCAN1::attachInterrupt(void func()) // copy IRQ table to SRAM, point VTOR reg to it, set IRQ addr to user ISR
{
    static uint8_t newTbl[0xF0] __attribute__((aligned(0x100)));
    uint8_t *pNewTbl = newTbl;
    int origTbl = MMIO32(vtor);
    for (int j = 0; j < 0x3c; j++) // table length = 60 integers
        MMIO32((pNewTbl + (j << 2))) = MMIO32((origTbl + (j << 2)));

    uint32_t canVectTblAdr = reinterpret_cast<uint32_t>(pNewTbl) + (36 << 2); // calc new ISR addr in new vector tbl
    MMIO32(canVectTblAdr) = reinterpret_cast<uint32_t>(func);                 // set new CAN/USB ISR jump addr into new table
    MMIO32(vtor) = reinterpret_cast<uint32_t>(pNewTbl);                       // load vtor register with new tbl location
    enableInterrupt();
}

bool meCAN1::transmit(int txId, const void *ptr, unsigned int len)
{

    if (periphBit(tsr, 26) == 0) // tx mailbox 0 not ready)
    {
           return false;
    }

    if (_extIDs)
        MMIO32(ti0r) = (txId << 3)  + 0b100; // // set 29b extended ID.
    else
        MMIO32(ti0r) = (txId << 21) + 0b000; //12b std id

    MMIO32(tdt0r) = (len << 0);
    // this assumes that misaligned word access works
    MMIO32(tdl0r) = ((const uint32_t *)ptr)[0];
    MMIO32(tdh0r) = ((const uint32_t *)ptr)[1];

    periphBit(ti0r, 0) = 1; // tx request
    return true;
}

int meCAN1::receive(volatile int &id, volatile int &fltrIdx, volatile uint8_t pData[])
{
  int len = -1;
  if (MMIO32(can1_rf0r) & (3 << 0)) // num of msgs pending
    {
        if (_rxExtended)
            id = (MMIO32(ri0r) >> 3); // extended id
        else
            id = (MMIO32(ri0r) >> 21);          // std id
        len = MMIO32(rdt0r) & 0x0F;             // fifo data len and time stamp
        fltrIdx = (MMIO32(rdt0r) >> 8) & 0xff;  // filter match index. Index accumalates from start of bank
        ((uint32_t *)pData)[0] = MMIO32(rdl0r); // 4 low rx bytes
        ((uint32_t *)pData)[1] = MMIO32(rdh0r); // another 4 bytes
        periphBit(can1_rf0r, 5) = 1;                 // release the mailbox
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




void meCAN2::begin(int brp,int Rx,int Tx,idtype addrType)
{

  uint32_t btr;
  
  if (_SERIES == 0)  return;
  if (_SERIES == PROC_F1)
  {
    /*
     *  http://www.bittiming.can-wiki.info/
     * 
     *  use clock rate of 32Mhz 
     */
    switch (brp)
    {
      case 50:
        btr = 0x001e0027;
        break;
      case 125:
        btr = 0x001e000f;
        break;
      case 250:
        btr = 0x001e0007;
        break;
      case 500:
        btr = 0x001e0003;
        break;
      case 1000:
        btr = 0x001e0001;
        break;       
    }
    switch (Rx)
    {
      //-------  TX pins are sset according to RX pin
      case PB12:
        //MMIO32(apb2enr) |= (1 << 2) | (1 << 0); // enable gpioA = b2 and afio = b0 clks
        
        periphBit(apb2enr,0)=1;             // enable Alt Function Clk 
        periphBit(apb2enr,3)=1;             // enable PortB clk
        periphBit(mapr, 22) = 0;            // Clr remap of CAN2
          // setup B12 for rx and B13 as Tx
        MMIO32(crhB) &= 0xFF00FFFF;
        MMIO32(crhB) |= 0b0100 << 16;
        MMIO32(crhB) |= 0x900000;
        periphBit(odrB, 12) = 0;
        break;
        
//      case PB8:
//        MMIO32(apb2enr) |= (1 << 3) | (1 << 0); // enable gpioB = b3 and afio = b0 clks
//        MMIO32(mapr) |= (2 << 13);              // alt func, CAN remap to B9+B8 
//        MMIO32(crhB) &= 0xFFFFFF00;             // clear control bits for pins 8 & 9 of Port B
//        MMIO32(crhB) |= 0b0100;              // pin8 for rx, b0100 = b01xx, floating, bxx00 input
//        periphBit(odrB, 8) = 0;            // set input will pullup resistor for single wire with pullup mode
//        MMIO32(crhB) |= 0b1001 << 4;            // set output
//        break;

        default:
          return;
          break;
    }
    periphBit(apb1enr, 26) = 1;      // enable CAN2
     
  }
  else if(_SERIES == PROC_L4)
  {
   
    /*
     *  http://www.bittiming.can-wiki.info/
     * 
     *  use clock rate of 80Mhz
     */
    switch (brp)
    {
      case 50:
        btr = 0x001c0063;
        break;
      case 125:
        btr = 0x001c0027;
        break;
      case 250:
        btr = 0x001c0013;
        break;
      case 500:
        btr = 0x001c0009;
        break;
      case 1000:
        btr = 0x001c0004;
        break;     
    }
    
    switch (Rx)
    {
      case PA11:
          // enable PortA clk
        periphBit(ahb2enr,0)=1;
          // enable CAN1 clk
        periphBit(apb1enr1,25)=1;
          // setup PA11 for rx and A12 as Tx, as alternate mode
        _CLEAR(moderA,22);
        _SET(moderA,23);
          // set pin modes as high speed
        _CLEAR(ospeedrA,22);
        _SET(ospeedrA,23); 
          // set pin modes as alternate mode 9 (pins PA11)
        MMIO32(afrhA) &= 0xFFF0FFFF; // clear nibble
        MMIO32(afrhA) |= (0x9 << 12);   // pin PA11
        break;
      
      default:
        // error
        break;
    }
    switch (Tx)
    {
      case PA12:
          // enable PortA clk
        periphBit(ahb2enr,0)=1;
          // setup  A12 as Tx, as alternate mode
        _CLEAR(moderA,24);
        _SET(moderA,25);
          // set pin modes as high speed
        _CLEAR(ospeedrA,24);
        _SET(ospeedrA,25); 
           // set pin modes as alternate mode 9 (pins PA12)
        MMIO32(afrhA) &= 0xFF0FFFFF; // clear nibble
        MMIO32(afrhA) |= (0x9 << 16);   // pin PA12
        break;
      
      default:
        // error
        break;
    }
  }
  else if(_SERIES == PROC_F4)
  {
    
    /*
     *  http://www.bittiming.can-wiki.info/
     * 
     *  use clock rate of 45Mhz
     */
    switch (brp)
    {
      case 50:
        btr = 0x001e0031;
        break;
      case 125:
        btr = 0x001e0013;
        break;
      case 250:
        btr = 0x001e0009;
        break;
      case 500:
        btr = 0x001e0004;
        break;
      case 1000:
        btr = 0x001b0002;
        break;     
    }
  }

  setBTR(btr);
  filterInit(); 
  
}


void meCAN2::setBTR(uint32_t btr)
{
    //  set ABOM 
  periphBit(mcr2,6)=1;
    // exit sleep
  periphBit(mcr2, 1) = 0; 
    //  go into Init mode 
  INRQ2 = 1;
  while(1)
  {
    if (INAK2 == 1) break;
  }
  MMIO32(btr2) = btr; 
    // go into normal mode
  INRQ2 = 0;
  while(INAK2);
}


void meCAN2::filterInit()
{
  FINIT = 1;                                // FINIT  'init' filter mode ]
  periphBit(fa1r, 14) = 0;                       // de-activate filter 'bank'
  periphBit(fs1r, 14) = 0;                       // fsc filter scale reg,  0 => 2ea. 16b
  periphBit(fm1r, 14) = 0;                       // fbm list mode = 1, 0 = mask
  
  MMIO32(fr1+8*14) = 0L;
  MMIO32(fr1+8*14+4) = 0L;
  periphBit(fa1r, 14) = 1;                       // activate filter 'bank'
  FINIT = 0;
  MMIO32(esr2) = 0L;    
  periphBit(rf0r2,5) = 1;   // clr mailbox                    
}

int meCAN2::receive(volatile int &id, volatile int &fltrIdx, volatile uint8_t pData[])
{
  int len = -1;
  if (MMIO32(rf0r2) & (3 << 0)) // num of msgs pending
    {
      if (_rxExtended)
          id = (MMIO32(ri0r2) >> 3); // extended id
      else
          id = (MMIO32(ri0r2) >> 21);          // std id
      len = MMIO32(rdt0r2) & 0x0F;             // fifo data len and time stamp
      fltrIdx = (MMIO32(rdt0r2) >> 8) & 0xff;  // filter match index. Index accumalates from start of bank
      ((uint32_t *)pData)[0] = MMIO32(rdl0r2); // 4 low rx bytes
      ((uint32_t *)pData)[1] = MMIO32(rdh0r2); // another 4 bytes
      periphBit(rf0r2, 5) = 1;                 // release the mailbox
    }
    return len;
}


bool meCAN2::transmit(int txId, const void *ptr, unsigned int len)
{

    if (periphBit(tsr2, 26) == 0) // tx mailbox 0 not ready)
    {
           return false;
    }

    if (_extIDs)
        MMIO32(ti0r2) = (txId << 3)  + 0b100; // // set 29b extended ID.
    else
        MMIO32(ti0r2) = (txId << 21) + 0b000; //12b std id

    MMIO32(tdt0r2) = (len << 0);
    // this assumes that misaligned word access works
    MMIO32(tdl0r2) = ((const uint32_t *)ptr)[0];
    MMIO32(tdh0r2) = ((const uint32_t *)ptr)[1];

    periphBit(ti0r2, 0) = 1; // tx request
    return true;
}

void meCAN2::enableInterrupt()
{
  periphBit(ier2, fmpie0) = 1U; // set fifo RX int enable request
  MMIO32(iser2) = 1UL << 0;
}
void meCAN2::disableInterrupt()
{
  periphBit(ier2, fmpie0) = 0U;
  MMIO32(iser2) = 1UL << 0;
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



void display32bit(String des,uint32_t value)
{
  char ds[100];
  uint32_t v1 = value;
  
  char hd[20];
  char a;
  int p=0;
  int c=0;
  int cc=0;
  for (int i=31;i>=0;i--)
  {
    //bool b = (bool) (value & (1<<i));
    bool b = (bool) bitRead(value,i);
    ds[p] = b?49:48;
    //a = b?48:49;
    //Serial << a;
    p++;
    c++;
    cc++;
    if (c>3 && i>0)
    {
      ds[p] = '_';
      p++;
      c=0;
    }
    if (cc>7 && i>0)
    {
      ds[p] = '_';
      p++;
      ds[p] = '_';
      p++;
      cc=0;
    }
  }
  ds[p]=0;
  
  String t = String(des);
  t = String(t + "          ");
  t = t.substring(0,9);
  t = String(t + " = ");
  String L = String(ds);
  Serial.print(t);
  Serial.print(L);
  String v = String(v1);
  v = String("     " + v);
  Serial.println(v);
}
