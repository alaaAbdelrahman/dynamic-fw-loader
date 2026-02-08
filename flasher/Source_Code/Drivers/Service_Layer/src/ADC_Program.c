#include "ADC_Interface.h"
#include "time32.h"
extern uint16_t battery_percentage_fraction;
float  Glob_f32_Vbatresistor;


/*************************************************************
**************************************************************
**      Function Name  : S_ADC_Init                         **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): None                               **
**      Return value   : None                               **
**      Description    : Function to init ADC               **
**************************************************************
*************************************************************/
  
void S_ADC_Init()
{
  ADCInitType ADC_InitStruct; 
  /* ADC initialization */
  S_ADC_DeInit();
  ADC_InitStruct.TrigMode = ADC_TRIGMODE_MANUAL;
  ADC_InitStruct.ConvMode = ADC_CONVMODE_SINGLECHANNEL;
  ADC_InitStruct.Channel = ADC_CHANNEL9;
  ADC_InitStruct.ClockDivider = ADC_CLKDIV_4;
  ADC_InitStruct.ClockSource = ADC_CLKSRC_RCH;
  ADC_Init(&ADC_InitStruct);
#if (defined(EM122U_BOARD) || defined(EM122_BOARD)|| defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD)|| defined(EM210_BOARD) || defined(MTR_THREE_PH))
   /* Res division x1/4 */
  S_ADC_RESDivisionCmd(DISABLE);
#else
  S_ADC_RESDivisionCmd(ENABLE);
#endif
  
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////






/*************************************************************
**************************************************************
**      Function Name  : NVR_GetADCVoltageParameter         **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint32_t  &   NVR_ADCVOLPARA       **
**      Return value   : uint32_t                           **
**      Description    : Function to Get ADC Volatge        **
**************************************************************
*************************************************************/
/**
  * @breif  Get the parameters of ADC voltage measuring.
  * @note   Voltage(unit:V) = aParameter*ADC_DATA + bParameter
  *             ADC_DATA: ADC channel original data
  *             aParameter/bParameter: Get from this function
  * @param  [in]Mode:
  *                ADC_3V_EXTERNAL_NODIV
  *                ADC_3V_EXTERNAL_RESDIV
  *                ADC_3V_EXTERNAL_CAPDIV
  *                ADC_3V_VDD_RESDIV
  *                ADC_3V_VDD_CAPDIV
  *                ADC_3V_BATRTC_RESDIV
  *                ADC_3V_BATRTC_CAPDIV
  *                ADC_5V_EXTERNAL_NODIV
  *                ADC_5V_EXTERNAL_RESDIV
  *                ADC_5V_EXTERNAL_CAPDIV
  *                ADC_5V_VDD_RESDIV
  *                ADC_5V_VDD_CAPDIV
  *                ADC_5V_BATRTC_RESDIV
  *                ADC_5V_BATRTC_CAPDIV
  * @param  [out]Parameter: The parameters get from NVR
  * @retval 0: Function succeeded.
            1: Function failed(Checksum error). 
  */
uint32_t NVR_GetADCVoltageParameter(uint32_t Mode, NVR_ADCVOLPARA *Parameter)
{
  uint32_t checksum;
  uint32_t i;
  int32_t tmp_int;

/*RmvCodCmntA_K*/

  /*----- Power supply: 5V -----*/
  if (0x100UL & Mode)
  {
            /*RmvCodCmntA_K*/  }
  /*----- Power supply: 3.3V -----*/
  else
  {
    checksum = 0UL;
    for (i=0; i<14; i++)
      checksum += *(NVR_3VPARA_BASEADDR1+i);
    checksum = ~(checksum);
    if (checksum != *(NVR_3VPARA_BASEADDR1+i))     /* Checksum1 error */
    {
      checksum = 0UL;
      for (i=0; i<14; i++)
        checksum += *(NVR_3VPARA_BASEADDR2+i);
      checksum = ~(checksum); 
      if (checksum != *(NVR_3VPARA_BASEADDR2+i))   /* Checksum2 error */ 
      {
        return 1;
      }  
      else
      {
        tmp_int = (int32_t)*(NVR_3VPARA_BASEADDR2+2*(Mode));
        Parameter->aParameter = (float)(tmp_int / 100000000.0);
        tmp_int = (int32_t)*(NVR_3VPARA_BASEADDR2+2*(Mode)+1);
        Parameter->bParameter = (float)(tmp_int / 100000000.0);
        return 0;
      } 
    }
    else
    {
      tmp_int = (int32_t)*(NVR_3VPARA_BASEADDR1+2*(Mode));
      Parameter->aParameter = (float)(tmp_int / 100000000.0);
      tmp_int = (int32_t)*(NVR_3VPARA_BASEADDR1+2*(Mode)+1);
      Parameter->bParameter = (float)(tmp_int / 100000000.0);
      return 0;
    }
  }
  return 0;
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////





/*************************************************************
**************************************************************
**      Function Name  : ADC_CalculateVoltage               **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint32_t & int16_t  & float        **
**      Return value   : uint32_t                           **
**      Description    : Function to calculate ADC Volatge  **
**************************************************************
*************************************************************/
uint32_t ADC_CalculateVoltage(uint32_t Mode, int16_t adc_data, float *Voltage)
{
  NVR_ADCVOLPARA parameter;

  /* Check parameters */
  
  if (NVR_GetADCVoltageParameter(Mode, &parameter))
  {
    if ((Mode&0xFUL) > 2UL) /* VDD or BATRTC channel */
    {

              /*RmvCodCmntA_K*/
    }
    else /* External channel */
    {
      if (Mode & 0x100UL)  /* Power supply: 5V */
      {
                /*RmvCodCmntA_K*/
      }
      else                 /* Power supply: 3.3V */
      {
      	
      	
      	
      	
      	
      	
      	
      	
      	
      	
      	
      	
                 /*RmvCodCmntA_K*/

            *Voltage = (float)(0.00016425*(float)adc_data + 0.03739179+.004);
            #if (defined(EM122U_BOARD) || defined(EM122_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD) || defined(EM130_BOARD)|| defined(EM210_BOARD) || defined(MTR_THREE_PH)) 
                *Voltage = ((float) *Voltage )/2.0;
            #endif
      }
    }
  }
  else
  {
#if (defined(EM122U_BOARD) || defined(EM122_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD) || defined(EM130_BOARD) || defined(EM210_BOARD) || defined(MTR_THREE_PH)) 
    *Voltage = ((float)(parameter.aParameter*(float)adc_data + parameter.bParameter))*(float)2.085;
#else
    if (Glob_f32_Vbatresistor > .032)
      Glob_f32_Vbatresistor = .025;
   *Voltage =  ((float)(0.00016425*(float)adc_data + 0.03739179 +.004)*2+Glob_f32_Vbatresistor)/2;
#endif
  }
  return 0;
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////




/*************************************************************
**************************************************************
**      Function Name  : ADC_CalculateVoltage               **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint16_t *                         **
**      Return value   : None                               **
**      Description    : Function to Get battery Volatge    **
**************************************************************
*************************************************************/
void S_ADC_Get_Vbat(  uint16_t * ret_val)
{
  int32_t adc_data ; 
  NVR_BATMEARES NVR_BATOffsetStruct;
  float  Vbat;


     /*RmvCodCmntA_K*/
    /* Get NVR BAT Offset information */
    S_NVR_GetBATOffset(&NVR_BATOffsetStruct) ;

    Glob_f32_Vbatresistor = NVR_BATOffsetStruct.BATRESResult;
    if (Glob_f32_Vbatresistor == 0)
      Glob_f32_Vbatresistor = .03;
    if (Glob_f32_Vbatresistor < 0)
      Glob_f32_Vbatresistor = 0;

    /* Starts a manual ADC conversion */
  S_ADC_StartManual();
  /* Wait until maual triger complete*/
  S_ADC_WaitForManual();
  
 adc_data = (int16_t)S_ADC_GetADCConversionValue(ADC_CHANNEL9);
   /* ADC resource release */
 // ADC_DeInit();
 /* Calculate the voltage of channel 3 */
#if (defined(EM122U_BOARD) || defined(EM122_BOARD)|| defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD) || defined(MTR_THREE_PH)) 
 ADC_CalculateVoltage(ADC_3V_EXTERNAL_NODIV, adc_data, &Vbat);
#else
 ADC_CalculateVoltage(ADC_3V_EXTERNAL_RESDIV, adc_data, &Vbat);
#endif
 
 if(adc_data>20)
 {
#if (defined(EM122U_BOARD) || defined(EM122_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM210_BOARD) || defined(MTR_THREE_PH))
   *ret_val = (uint16_t)(Vbat *20) ;
#else
   *ret_val = (uint16_t)(Vbat *200) ;
#endif
 }
 else
 {
   *ret_val =0;
 }
 
   battery_percentage_fraction = (uint16_t)(Vbat *200);
 if (battery_percentage_fraction < 180 || battery_percentage_fraction > 400)
   battery_percentage_fraction = 0;
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////







#if 0 //

/*************************************************************
**************************************************************
**      Function Name  : S_ADC_Get_VSuper_Cap               **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint16_t *                         **
**      Return value   : None                               **
**      Description    : Function to Get battery Volatge    **
**************************************************************
*************************************************************/
void S_ADC_Get_VSuper_Cap(  uint16_t * ret_val)
{
  volatile float  Vbat;
  int32_t data;
  uint16_t timeOut_ADC;


  /* ADC initialization */
  ADC_DeInit();
  timeOut_ADC = 0;
  while ((ANA->ADCCTRL & ANA_ADCCTRL_MTRIG)&& (timeOut_ADC++ < 5000 ) );

  ANA->ADCCTRL = 0x06C00102;

  /* Enable division 
  [in]Division
                ADC_BAT_CAPDIV  (Cap division 1/4)
                ADC_BAT_RESDIV  (Resistance division 1/4)
  */
  ANA->REG1 |= ADC_BAT_CAPDIV;


  /* Enable ADC */
  ana_reg3_tmp |= ANA_REG3_ADCPDN;  
  ANA->REG3 = ana_reg3_tmp;
  

  /* Start a manual ADC conversion */
  ADC_StartManual();

  /* Waiting last operation done */
  ADC_WaitForManual();
  
  /*read data from channel 2*/
  data = ANA->ADCDATA2;
 
  /* Calculate the voltage of BAT2 */
  
    *ret_val = (uint16_t) ( 0.014107*(float)data  - 0.699515 );

  /* ADC resource release */
  ADC_DeInit();
   
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////
#endif


#ifdef SUPERCAP_FEATURE


void SuperCap_Monitoring(void)
{
#ifdef  EM130_BOARD
  static ADC_CalResType SuperCapReading;
  
  ADC_GetBAT2Voltage_Normal(ADC_BAT_RESDIV,&SuperCapReading);
 
   ctrl_sys_new_var.SuperCap_Value = (uint32_t) (SuperCapReading.BAT2Voltage*100);
   
#elif defined EM330_BOARD


int32_t adc_data ; 
  NVR_BATMEARES NVR_BATOffsetStruct;
  float  Vbat;


 
     /*RmvCodCmntA_K*/
    /* Get NVR BAT Offset information */
    S_NVR_GetBATOffset(&NVR_BATOffsetStruct) ;

    Glob_f32_Vbatresistor = NVR_BATOffsetStruct.BATRESResult;
    if (Glob_f32_Vbatresistor == 0)
      Glob_f32_Vbatresistor = .03;
    if (Glob_f32_Vbatresistor < 0)
      Glob_f32_Vbatresistor = 0;

    /* Starts a manual ADC conversion */
  S_ADC_StartManual();
  /* Wait until maual triger complete*/
  S_ADC_WaitForManual();
  
 adc_data = (int16_t)S_ADC_GetADCConversionValue(ADC_CHANNEL11);
   /* ADC resource release */

 ADC_CalculateVoltage(ADC_3V_EXTERNAL_NODIV, adc_data, &Vbat);

 
 if(adc_data>20)
 {
   ctrl_sys_new_var.SuperCap_Value = (uint16_t)(Vbat *200) ;
 }
 else
 {
   ctrl_sys_new_var.SuperCap_Value =0;
 }


#endif

}

#endif




/*************************************************************
**************************************************************
**      Function Name  : S_ADC_Get_VoltBat_1                **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 13/12/2023                         **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint16_t *                         **
**      Return value   : None                               **
**      Description    : Function to Get battery Volatge    **
**************************************************************
*************************************************************/
void S_ADC_Get_VoltBat_1(  uint16_t * ret_val)
{
  volatile float  Vbat;
  //static uint16_t VoltBat_1_percentage =0; 
  int32_t VoltBat_1_data;
  uint16_t VoltBat_1_timeOut_ADC;

  
  /* ADC initialization */
  ADC_DeInit();
  VoltBat_1_timeOut_ADC=0;
  while ((ANA->ADCCTRL & ANA_ADCCTRL_MTRIG)&& (VoltBat_1_timeOut_ADC++ < 5000 ));
  ANA->ADCCTRL = 0x06C00101;
  
  /* Enable division */
  ANA->REG1 |= ADC_BAT_CAPDIV;
  /* Enable ADC */
  ana_reg3_tmp |= ANA_REG3_ADCPDN;  
  ANA->REG3 = ana_reg3_tmp;
  
  /* Start a manual ADC conversion */
  ADC_StartManual();
  /* Waiting last operation done */
  ADC_WaitForManual();
  
  VoltBat_1_data = ANA->ADCDATA1;
 
  /* Calculate the voltage of VDD */

    Vbat = (0.00014107*(float)VoltBat_1_data - 0.00699515) ;
    *ret_val = (uint16_t) ( (float)Vbat * 100);
  /* ADC resource release */
  ADC_DeInit();

   
 if (*ret_val < 180 || *ret_val > 400)
   *ret_val = 0;   
   
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////

