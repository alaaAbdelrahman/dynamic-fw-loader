
/*******************************************************************************
**                                                                            **
**  PROJECT   :                                                               **
**                                                                            **
**  File Name : HW_Init.h                                                     **
**                                                                            **
**  File Type : header file for the HW_Init                                   **
**                                                                            **
**  AUTHOR    : Mohamed Mahmoud Zakaria                                       **
**                                                                            **
**  PURPOSE   : It contains the prototypes of HW_Init                         **
**                                                                            **
**  History   : May 30, 2011                                                  **
**                                                                            **
**  Version   : 1.0                                                           **
*******************************************************************************/


/* To eliminate including the file more than one time */
#ifndef METER_INIT_H
#define METER_INIT_H


/********************************** Headers ***********************************/

#include "config.h"      /* Header file contains the standard types used */
#include "CLK_Interface.h"
#include "GPIO_Interface.h"
#include "SPI_Interface.h"
#include "I2C_Interface.h"
#include "ADC_Interface.h"
#include "RTC_Interface.h"
#include "UART_Interface.h"

#if (MicroController == Micro_V94XX)
#include "V94XX_PMU.h"
#include "V94XX_ANA.h"
#include "V94XX_CORTEX.h"
#else
#include "v85xx_pmu.h"
#include "v85xx_ana.h"
#include "v85xx_cortex.h"
#endif


#include "basic_srv.h"
#include "core_cm0.h"
/******************************************************************************/



//#define GAIN_CALIB_CONST  ((double)2147483648)
//#define STD_MTR_CURR      ((float)50000000) /* in micro ampere*/
#define STD_MTR_PF        ((double)1000)
#define PULSE_COEFF     32

/*******************************************************************************
                           Functions prototypes
 ******************************************************************************/

/*------This is a list of functions provide for upper layer modules.----------*/

void vMC_DriversInit(void);               //PINS
void vMeter_Init(void);
void IRQ_init(void);
void detatch_sensor_init(void);

/*------------------------------------------------------------------------------
              End function prototypes
 -----------------------------------------------------------------------------*/





#endif  /*end 0f if METER_INIT_H defined */
/*******************************************************************************
                                 End of file
*******************************************************************************/
