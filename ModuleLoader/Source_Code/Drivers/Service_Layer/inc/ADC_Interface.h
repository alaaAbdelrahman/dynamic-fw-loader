#ifndef ADC_Interface_H_
#define ADC_Interface_H_


/*************************************************************
**************************************************************
**                   Include section                        **
**************************************************************
**************************************************************
*************************************************************/
#include "config.h"

#if (MicroController == Micro_V94XX)

#include "V94XX_ADC.h"
#include "V94XX_LoadNVR.h"
#include "V94XX_ADC_Tiny.h"
#include "V94XX_COMP.h"
#include "V94XX_GPIO.h"

#else
#include "v85xx_adc.h"
#include "v85xx_LoadNVR.h"
#include "v85xx_adc_tiny.h"
#include "v85xx_comp.h"
#include "v85xx_gpio.h"
#endif

#include "ADC_Config.h"


  /*************************************************************
  **************************************************************
  **                   enum section                           **
  **************************************************************
  *************************************************************/

#ifdef SUPERCAP_FEATURE

typedef enum
{
  Init,
  Reading_Value
}SuperCapStates_t;
#endif

  /*************************************************************
  **************************************************************
  **                   Functions Declaration                  **
  **************************************************************
  *************************************************************/
/*edit by A_K*/
uint32_t NVR_GetADCVoltageParameter(uint32_t Mode, NVR_ADCVOLPARA *Parameter);
uint32_t ADC_CalculateVoltage(uint32_t Mode, int16_t adc_data, float *Voltage);
void S_ADC_Get_Vbat(uint16_t * ret_val );
void S_ADC_Init();
/*edit by A_K*/
void S_ADC_Get_VoltBat_1(  uint16_t * ret_val);

#ifdef SUPERCAP_FEATURE
void S_ADC_Get_VSuper_Cap(  uint16_t * ret_val);
void SuperCap_Monitoring(void);
#endif



#endif /* ADC_Interface_H_ */
