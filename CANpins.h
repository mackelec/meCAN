#pragma once
#include <arduino.h>

#include <PString.h>
#include <Streaming.h>

uint16_t      pin2PortPin(int pin);
GPIO_TypeDef * pin2Port(int pin);
bool          clockEnable(GPIO_TypeDef *port);

typedef struct PinInfo
{
  int pinNumber;   //  eg, PA11
  uint32_t alternate_function;  // eg, GPIO_AF4_CAN
  int portpin;    // 11 (from A11
  int portNum;    // eg, 1=port A, 2 = port B
  int gpioInitPin; // eg,  GPIO_PIN_11
  GPIO_TypeDef *port; // eg, GPIOA
} PinInfo;

const PinInfo pin_info[]  =
{
//----------------------- STM32F0
#ifdef STM32F0
  #ifdef PA11 
    {PA11, GPIO_AF4_CAN, 11, 1, GPIO_PIN_11,GPIOA}, // PA11 - RX
  #endif
  #ifdef PA12 
    {PA12, GPIO_AF4_CAN, 12, 1, GPIO_PIN_12,GPIOA}, // PA12 - TX
  #endif
  #ifdef PB8 
    {PB8, GPIO_AF4_CAN, 8, 2, GPIO_PIN_8,GPIOB}, // PB8 - RX
  #endif
  #ifdef PB9 
    {PB9, GPIO_AF4_CAN, 9, 2, GPIO_PIN_9,GPIOB}, // PB9 - TX
  #endif
#endif
//-------------------------STM32F1
#ifdef STM32F1
  #ifdef AFIO_CAN1_1
    #ifdef PA11
    {PA11, AFIO_CAN1_1, 11, 1, GPIO_PIN_11,GPIOA}, // PA11 - RX
    #endif
    #ifdef PA12
    {PA12, AFIO_CAN1_1, 12, 1, GPIO_PIN_12,GPIOA}, // PA12 - TX
    #endif
  #endif
  
  #ifdef AFIO_CAN1_2
    #ifdef PB8
    {PB8, AFIO_CAN1_2, 8, 2, GPIO_PIN_8,GPIOB}, // PB8 - RX
    #endif
    #ifdef PB9
    {PB9, AFIO_CAN1_2, 9, 2, GPIO_PIN_9,GPIOB}, // PB9 - TX
    #endif
  #endif
  
  #ifdef AFIO_CAN1_3
    #ifdef PD0
    {PD0, AFIO_CAN1_3, 0, 4, GPIO_PIN_0,GPIOD}, // PD0 - RX
    #endif
    #ifdef PD1
    {PD1, AFIO_CAN1_3, 1, 4, GPIO_PIN_1,GPIOD}, // PD1 - RX
    #endif
  #endif
  
  #ifdef AFIO_CAN2_ENABLE
    #ifdef PB5
    {PB5, AFIO_CAN2_ENABLE, 5, 2, GPIO_PIN_5,GPIOB}, // PB5 - RX
    #endif
    #ifdef PB6
    {PB6, AFIO_CAN2_ENABLE, 6, 2, GPIO_PIN_6,GPIOB}, // PB6 - TX
    #endif
  #endif

  #ifdef AFIO_CAN2_DISABLE
    #ifdef PB12
    {PB12, GPIO_AF4_CAN, 12, 2, GPIO_PIN_12, GPIOB}, // PB12 - RX
    #endif
    #ifdef PB13
    {PB13, GPIO_AF4_CAN, 13, 2, GPIO_PIN_13, GPIOB}, // PB13 - TX
    #endif
  #endif
#endif

// ======================= STM32F4 =====================

#ifdef STM32F4
  #ifdef GPIO_AF11_CAN3
    #ifdef PA8
    {PA8, GPIO_AF11_CAN3, 8, 1, GPIO_PIN_8, GPIOA}, // PA8 - RX
    #endif
    #ifdef PB3
    {PB3, GPIO_AF11_CAN3, 3, 2, GPIO_PIN_3, GPIOB}, // PB3 - RX
    #endif
    #ifdef PA15
    {PA15, GPIO_AF11_CAN3, 15, 1, GPIO_PIN_15, GPIOA}, // PA15 - TX
    #endif
    #ifdef PB4
    {PB4, GPIO_AF11_CAN3, 4, 2, GPIO_PIN_4, GPIOB}, // PB4 - TX
    #endif
  #endif

  #ifdef GPIO_AF9_CAN1
    #ifdef PA11
    {PA11, GPIO_AF9_CAN1, 11, 1, GPIO_PIN_11, GPIOA}, // PA11 - RX
    #endif
    #ifdef PA12
    {PA12, GPIO_AF9_CAN1, 12, 1, GPIO_PIN_12, GPIOA}, // PA12 - TX
    #endif
    #ifdef PD0
    {PD0, GPIO_AF9_CAN1, 0, 4, GPIO_PIN_0, GPIOD}, // PD0 - RD
    #endif
    #ifdef PD1
    {PD1, GPIO_AF9_CAN1, 1, 4, GPIO_PIN_1, GPIOD}, // PD1 - TD
    #endif
    #ifdef PG0
    {PG0, GPIO_AF9_CAN1, 0, 7, GPIO_PIN_0, GPIOG}, // PG0 - RX
    #endif
    #ifdef PG1
    {PG1, GPIO_AF9_CAN1, 1, 7, GPIO_PIN_1, GPIOG}, // PG1 - RX
    #endif
    #ifndef   STM32F413xx
      #ifdef PB8
      {PB8, GPIO_AF9_CAN1, 8, 2, GPIO_PIN_8, GPIOB},      // PB8    - RX
      #endif
      #ifdef PB9
      {PB9, GPIO_AF9_CAN1, 9, 2, GPIO_PIN_9, GPIOB},      // PB9    - TX
      #endif
    #endif
  #endif
  #ifdef GPIO_AF9_CAN2
    #ifdef PA11
    {PA11, GPIO_AF9_CAN2, 11, 1, GPIO_PIN_11, GPIOA}, // PA11 - RX
    #endif
    #ifdef PB12
    {PB12, GPIO_AF9_CAN2, 12, 2, GPIO_PIN_12, GPIOB}, // PB12 - RX
    #endif
    #ifdef PG11
    {PG11, GPIO_AF9_CAN2, 11, 7, GPIO_PIN_11, GPIOG}, // PG11 - RX
    #endif
    #ifdef PB6
    {PB6, GPIO_AF9_CAN2, 6, 2, GPIO_PIN_6, GPIOB},    // PB6  - TX
    #endif
    #ifdef PB13
    {PB13, GPIO_AF9_CAN2, 13, 2, GPIO_PIN_13, GPIOB}, // PB13 - TX
    #endif
    #ifdef PG12
    {PG12, GPIO_AF9_CAN2, 12, 7, GPIO_PIN_12, GPIOG}, // PG12 - TX
    #endif
  #endif
  #ifdef GPIO_AF8_CAN1
    #ifdef PB8
    {PB8, GPIO_AF8_CAN1, 8, 2, GPIO_PIN_8, GPIOB},    // PB8  - RX
    #endif
    #ifdef PB9
    {PB9, GPIO_AF8_CAN1, 9, 2, GPIO_PIN_9, GPIOB},    // PB9  - TX
    #endif
  #endif
#endif

// ---------------------- STM32L4

#ifdef STM32L4
  #ifdef GPIO_AF9_CAN1
    // group all the entries with GPIO_AF9_CAN1
    #ifdef PA11 
      {PA11, GPIO_AF9_CAN1, 11, 1, GPIO_PIN_11,GPIOA}, // PA11
    #endif
    #ifdef PB8
      {PB8, GPIO_AF9_CAN1, 8, 2, GPIO_PIN_8, GPIOB}, // PB8
    #endif
    #ifdef PD0
      {PD0, GPIO_AF9_CAN1, 0, 4, GPIO_PIN_0, GPIOD}, // PD0
    #endif
    #ifdef PA12
      {PA12, GPIO_AF9_CAN1, 12, 1, GPIO_PIN_12,GPIOA}, // PA12
    #endif
    #ifdef PB9
      {PB9, GPIO_AF9_CAN1, 9, 2, GPIO_PIN_9, GPIOB}, // PB9
    #endif
    #ifdef PD1
      {PD1, GPIO_AF9_CAN1, 1, 4, GPIO_PIN_1, GPIOD}, // PD1
    #endif
  #endif
  
  #ifndef STM32L496xx
    #ifdef GPIO_AF10_CAN1
      // group all the entries with GPIO_AF10_CAN2
      #ifdef PB12
        {PB12, GPIO_AF10_CAN1, 12, 2, GPIO_PIN_12, GPIOB}, // PB12 - RX
      #endif
      #ifdef PB13
        {PB13, GPIO_AF10_CAN1, 13, 2, GPIO_PIN_13, GPIOB}, // PB13 - TX
      #endif
    #endif
    #ifdef GPIO_AF3_CAN1
      #ifdef PB5
        {PB5, GPIO_AF3_CAN1, 5, 2, GPIO_PIN_5, GPIOB}, // PB5 - RX
      #endif
    #endif
    #ifdef GPIO_AF8_CAN1
      #ifdef PB6
        {PB6, GPIO_AF8_CAN1, 6, 2, GPIO_PIN_6, GPIOB}, // PB6 - TX
      #endif
    #endif
  #endif
  #ifdef STM32L496xx
    #ifdef GPIO_AF3_CAN2
      // group all the entries with GPIO_AF3_CAN2
      #ifdef PB5
      {PB5, GPIO_AF3_CAN2, 5, 2, GPIO_PIN_5, GPIOB}, // PB5 - RX
      #endif
    #endif
    
    #ifdef GPIO_AF10_CAN2
      // group all the entries with GPIO_AF10_CAN2
      #ifdef PB12
      {PB12, GPIO_AF10_CAN2, 12, 2, GPIO_PIN_12, GPIOB}, // PB12 - RX
      #endif
      #ifdef PB13
      {PB13, GPIO_AF10_CAN2, 13, 2, GPIO_PIN_13, GPIOB}, // PB13 - TX
      #endif
    #endif
    
    #ifdef GPIO_AF8_CAN2
      // group all the entries with GPIO_AF8_CAN2
      #ifdef PB6
      {PB6, GPIO_AF8_CAN2, 6, 2, GPIO_PIN_6, GPIOB}, // PB6 - TX
      #endif
    #endif
  #endif
#endif

#ifdef STM32G4
  #ifdef GPIO_AF9_FDCAN1
    #ifdef PA11 
      {PA11, GPIO_AF9_FDCAN1, 11, 1, GPIO_PIN_11, GPIOA}, // PA11 - RX
    #endif
    #ifdef PB8 
      {PB8, GPIO_AF9_FDCAN1,  8, 2, GPIO_PIN_8,  GPIOB}, // PB8 - RX
    #endif
    #ifdef PD0 
      {PD0, GPIO_AF9_FDCAN1,  0, 4, GPIO_PIN_0,  GPIOD}, // PD0 - RX
    #endif
    #ifdef PA12 
      {PA12, GPIO_AF9_FDCAN1, 12, 1, GPIO_PIN_12, GPIOA}, // PA12 - TX
    #endif
    #ifdef PB9 
      {PB9, GPIO_AF9_FDCAN1,  9, 2, GPIO_PIN_9,  GPIOB}, // PB9 - TX
    #endif
    #ifdef PD1 
      {PD1, GPIO_AF9_FDCAN1,  1, 4, GPIO_PIN_1,  GPIOD}, // PD1 - TX
    #endif
  #endif
  
  #ifdef GPIO_AF9_FDCAN2
    #ifdef PB5 
      {PB5, GPIO_AF9_FDCAN2,  5, 2, GPIO_PIN_5,  GPIOB},
    #endif
    #ifdef PB12 
      {PB12, GPIO_AF9_FDCAN2, 12, 2, GPIO_PIN_12, GPIOB},
    #endif
    #ifdef PB6 
      {PB6, GPIO_AF9_FDCAN2,  6, 2, GPIO_PIN_6,  GPIOB},
    #endif
    #ifdef PB13 
      {PB13, GPIO_AF9_FDCAN2, 13, 2, GPIO_PIN_13, GPIOB},
    #endif
  #endif
  
  #ifdef GPIO_AF11_FDCAN3
    #ifdef PA8 
      {PA8, GPIO_AF11_FDCAN3,  8, 1, GPIO_PIN_8,  GPIOA},
    #endif
    #ifdef PB3 
      {PB3, GPIO_AF11_FDCAN3,  3, 2, GPIO_PIN_3,  GPIOB},
    #endif
    #ifdef PA15 
      {PA15, GPIO_AF11_FDCAN3, 15, 1, GPIO_PIN_15, GPIOA},
    #endif
    #ifdef PB4
      {PB4, GPIO_AF11_FDCAN3,  4, 2, GPIO_PIN_4,  GPIOB},
    #endif
  #endif
#endif
{-1,0, 0, 1, GPIO_PIN_0,GPIOA}
};

bool clockEnable(int port)
{
  switch (port)
  {
    case 1: 
      #ifdef __HAL_RCC_GPIOA_CLK_ENABLE
      __HAL_RCC_GPIOA_CLK_ENABLE();
      return true;
      #endif
      break;
    case 2: 
      #ifdef __HAL_RCC_GPIOB_CLK_ENABLE
      __HAL_RCC_GPIOB_CLK_ENABLE();
      return true;
      #endif
      break;
    case 3: 
      #ifdef __HAL_RCC_GPIOC_CLK_ENABLE
      __HAL_RCC_GPIOC_CLK_ENABLE();
      return true;
      #endif
      break;
    case 4: 
      #ifdef __HAL_RCC_GPIOD_CLK_ENABLE
      __HAL_RCC_GPIOD_CLK_ENABLE();
      return true;
      #endif
      break;
    case 5: 
      #ifdef __HAL_RCC_GPIOE_CLK_ENABLE
      __HAL_RCC_GPIOE_CLK_ENABLE();
      return true;
      #endif
      break; 
    case 6: 
      #ifdef __HAL_RCC_GPIOF_CLK_ENABLE
      __HAL_RCC_GPIOF_CLK_ENABLE();
      return true;
      #endif
      break;
    case 7: 
      #ifdef __HAL_RCC_GPIOG_CLK_ENABLE
      __HAL_RCC_GPIOG_CLK_ENABLE();
      return true;
      #endif
      break;
  }
}


bool setup_meCANpin(int _pin)
{
  bool success = false;
  GPIO_InitTypeDef gpio_init;
  gpio_init.Mode = GPIO_MODE_AF_PP;
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;

  for (int i = 0; i < sizeof(pin_info) / sizeof(PinInfo); i++)
  {
    if (_pin == pin_info[i].pinNumber)
    {
      PinInfo info = pin_info[i];
      bool pass = clockEnable(info.portNum);
      gpio_init.Pin = info.gpioInitPin;   //info.pin;
      gpio_init.Alternate = info.alternate_function;
      HAL_GPIO_Init(info.port, &gpio_init);
      return true;;
    }
  }
  return success;
}
