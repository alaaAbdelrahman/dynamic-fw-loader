

/*************************************************************
**************************************************************
**      File Name      : GPIO Interface.h                   **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 15/3/2023                          **
**      Description    : this file include the declaration  **
**      of Functions to GPIO in Service layer               **
**************************************************************
*************************************************************/






#ifndef GPIO_Interface_H
#define GPIO_Interface_H
/*************************************************************
**************************************************************
**                   Include section                        **
**************************************************************
**************************************************************
*************************************************************/
#include "config.h"

#if (MicroController == Micro_V94XX)
#include "V94XX_PMU.h"
#include "V94XX_LCD.h"
#include "V94XX_CORTEX.h "
#include "V94XX_GPIO.h"

#else 

#include "v85xx_pmu.h"
#include "v85xx_lcd.h"
#include "v85xx_cortex.h "
#include "v85xx_gpio.h"

#endif


#include "basic_srv.h"
#include "GPIO_Config.h"













  /*************************************************************
  **************************************************************
  **                   Functions Declaration                  **
  **************************************************************
  *************************************************************/


void S_GPIO_Set_Bits(void *Port,uint16_t PIN);
void S_GPIO_Clear_Bits(void *Port,uint16_t PIN);
uint8_t S_GPIO_Get_Bits(void *Port,uint16_t PIN);
void S_GPIO_Toggle_Bits(void *Port,uint16_t PIN);

void S_GpioInit_PIN(void *Port,uint16_t PIN,uint16_t Mode );


void S_GpioInit_ALL_Modules(void);
void S_GpioWakeUpPinsInit(void);
void S_GpioEnableRfidBoard(void);
void S_GpioDisableRfidBoard(void);
void S_GpioI2cInitPinsOpLow(void);
void S_GpioShutdownPWD(void);
void S_GpioControlRelay(uint8_t RelayNumber, uint8_t ControlAction);
void S_GpioInitCalibPinINPUT(void);


void Dot_Matrix_GPIO_INIT(void);

#endif /*GPIO_Interface_H*/
