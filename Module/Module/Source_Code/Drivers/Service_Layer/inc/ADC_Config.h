
#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_


/* ADC Voltage Parameters */
typedef struct
{
  float aParameter;
  float bParameter;
} NVR_ADCVOLPARA;




#define S_ADC_RESDivisionCmd(NewState)                   ADC_RESDivisionCmd(NewState)
#define S_ADC_DeInit()                                   ADC_DeInit()
#define S_NVR_GetBATOffset(AddressOfStruct)              NVR_GetBATOffset(AddressOfStruct)
#define S_ADC_StartManual()                              ADC_StartManual()
#define S_ADC_WaitForManual()                            ADC_WaitForManual()
#define S_ADC_GetADCConversionValue(ADC_CHANNEL)         ADC_GetADCConversionValue(ADC_CHANNEL)

#define Charging_Time_Required                           21600 //6hours = 21600s
#define minium_SuperCapVolt                              350  
/*edit by A_K*/
/*edit by A_K*/

/********** NVR Address **********/
//ADC Voltage Parameters
#define NVR_3VPARA_BASEADDR1          (__IO uint32_t *)(0x40400)
#define NVR_3VPARA_BASEADDR2          (__IO uint32_t *)(0x40440)
#define NVR_5VPARA_BASEADDR1          (__IO uint32_t *)(0x40480)
#define NVR_5VPARA_BASEADDR2          (__IO uint32_t *)(0x404C0)
/****************************************************************/

#if (defined(EM122U_BOARD) || defined(EM122_BOARD)|| defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD) || defined(MTR_THREE_PH))
#define S_ADC_ENABLE()   {/*RmvCodCmntA_K*/ ADC_Cmd(ENABLE);}
#else
#define S_ADC_ENABLE()   {/*RmvCodCmntA_K*/ ADC_RESDivisionCmd(ENABLE);ADC_Cmd(ENABLE);}
#endif
#define S_ADC_DISABLE()  {/*RmvCodCmntA_K*/ADC_RESDivisionCmd(DISABLE);ADC_Cmd(DISABLE);}

#endif /* ADC_CONFIG_H_ */

