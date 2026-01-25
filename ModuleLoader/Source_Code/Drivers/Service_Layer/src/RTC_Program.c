#include "RTC_Interface.h"



/*************************************************************
**************************************************************
**      Function Name  : Delayms_Normal                     **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): None                               **
**      Return value   : None                               **
**      Description    : Function Delay 1 ms                **
**************************************************************
*************************************************************/
void Delayms_Normal(void)
{
  uint8 i,j;
    for(j=0;j<50;j++)          
    {
      for(i=0;i<20;i++)
      {
      }
    }

}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////






/*************************************************************
**************************************************************
**      Function Name  : Delayms_Wakeup                     **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): None                               **
**      Return value   : None                               **
**      Description    : Function Delay   1ms               **
**************************************************************
*************************************************************/
void Delayms_Wakeup(void)
{
  uint8 i,j;
    for(j=0;j<50;j++)       
    {
      for(i=0;i<2;i++)
      {
      }
    }

}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////







/*************************************************************
**************************************************************
**      Function Name  : CalRTC_REG                         **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint32_t  & int8_t & float         **
**      Return value   : uint8_t                            **
**      Description    : Function to Calculate RTC          **
**************************************************************
*************************************************************/
uint8_t CalRTC_REG(uint32_t PCLK,int8_t mode,float tmpval) 
{
  int8_t ktemp1, ktemp2, ktemp3, ktemp4;
  int16_t ack_used;
  int16_t PPMx10,ACP4,ACP5,ACP6;
  float Ti;
  uint32_t cal_value, div_value;
  uint8_t cnt=0;
  /*RmvCodCmntA_K*/ 
    /* Get section x temperature */
    ktemp1 = (int8_t)(RTC->ACKTEMP & 0xFF);
    ktemp2 = (int8_t)((RTC->ACKTEMP >> 8) & 0xFF);
    ktemp3 = (int8_t)((RTC->ACKTEMP >> 16) & 0xFF);
    ktemp4 = (int8_t)((RTC->ACKTEMP >> 24) & 0xFF);
  
    /* Calculate temperature(float) */
    Ti = RTC->ACTI / 256.0;
  
    /* Get ACK(K parameter) */
    if (tmpval < (float)ktemp1)
      ack_used = RTC->ACK1;
    else if ((tmpval >= (float)ktemp1) && (tmpval < (float)ktemp2))
      ack_used = RTC->ACK2;
    else if ((tmpval >= (float)ktemp2) && (tmpval < (float)ktemp3))
      ack_used = RTC->ACK3;
    else if ((tmpval >= (float)ktemp3) && (tmpval < (float)ktemp4))
      ack_used = RTC->ACK4;
    else  
      ack_used = RTC->ACK5;

    ACP4 = RTC->ACP4;
     ACP5 = RTC->ACP5;
      ACP6 = RTC->ACP6;
    
    /* Calculate 10*PPM */

        /*RmvCodCmntA_K*/ 
      PPMx10 = (int16_t)((((float)ack_used)*(tmpval-Ti)*(tmpval-Ti)/65536.0 + ACP4)*10);
      if(PPMx10 >=0)
    {
             PPMx10 = PPMx10 + 5;
             PPMx10 = PPMx10 /10;

    }
    else
    {
            PPMx10 = PPMx10 - 5;
            PPMx10 = PPMx10 /10;
    }
      

    /* Calculate the value of RTC_CAL, RTC_DIV */
    cal_value = (uint32_t)(((PPMx10*ACP5/65536)) + 1);

    div_value = (uint32_t)(PCLK/2 - (int16_t)((PPMx10*ACP6)>>12) - 1);   

    /* Write RTC_CAL/RTC_DIV register */
    RTC->DIV = div_value;
	while (RTC->CE & S_RTC_CE_BSY)
	{
        if(mode)
        	{
        	  Delayms_Normal();
        	}
		else
			{
				Delayms_Wakeup();
			}
		
		  cnt++;
		  PMU->WDTCLR = S_WDTCLR_KEY; //watch dog
		  if(cnt >10)
		  	{
			  	return rtcerr;
		  	}
	}
	RTC->PWD = S_RTCPWD_KEY;
	RTC->CE = S_RTCCE_SETKEY;   
	RTC->CAL = cal_value;
	RTC->PWD = S_RTCPWD_KEY;
	RTC->CE = S_RTCCE_CLRKEY;
    cnt = 0;
	while (RTC->CE & S_RTC_CE_BSY)
	{
		if(mode)
        	{
        	  Delayms_Normal();
        	}
		else
			{
				Delayms_Wakeup();
			}
		  cnt++;
		  PMU->WDTCLR = S_WDTCLR_KEY; //watch dog
		  if(cnt >10)
		  	{
			  	return rtcerr;
		  	}
	}
	return succ;
    
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////






/***************************************************************
****************************************************************
**      Function Name  : Read_Temperature_Normal              **
**      Author         :                                      **
**      Edit by        : Ahmed Mustafa                        **
**      Date           : 30/3/2023                            **
**      Reentrancy     : Non Reentrant                        **
**      Sync/Async     : Synchronous                          **
**      Parameters (in): float *                              **
**      Return value   : uint8_t                              **
**      Description    : Function to Read Temp in normal mode **
****************************************************************
***************************************************************/
uint8_t Read_Temperature_Normal(float* result) 
{
 
  uint8_t i,cnt;
  //uint32_t retval;
  int32_t value;
  int32_t tmp1,tmp2,tmp3;
  int16_t temp_org;

  uint32_t real_temp, mea_temp;
  uint32_t rtc_data1, rtc_data2, rtc_data3, rtc_data4;
  uint32_t checksum;
   int16_t TempDelta;
  
  
  /* ADC initialization */
    if (ANA->COMPOUT & S_ANA_COMPOUT_AVCCLV)
	{
    	return avccerr;
	}
    if(ANA->REG3 & S_ANA_REG3_RCHPD)
    {
            return rcherr;
    }

	cnt=0;
   while (ANA->ADCCTRL & S_ANA_ADCCTRL_MTRIG)
	  {
          PMU->WDTCLR = S_WDTCLR_KEY; //watch dog
		  Delayms_Normal();
		  cnt++;
		  if(cnt >10)
		  	{
			  	return adcerr;
		  	}
	  }
   ANA->ADCCTRL = 0;

	ANA->ADCCTRL =  S_ANA_ADCCTRL_CLKDIV_1 + S_ANA_ADCCTRL_CICSKIP_3 + S_ANA_ADCCTRL_DSRSEL_512 + 0x0a;
   
   ANA->REG3 |= S_ANA_REG3_ADCPDN;
  
  
  for(i=0;i<3;i++)   /*remove first 3 adc data*/
  	{
        cnt=0;
		ANA->ADCCTRL |= S_ANA_ADCCTRL_MTRIG;
       /* Waiting Manual ADC conversion done */
		while (ANA->ADCCTRL & S_ANA_ADCCTRL_MTRIG)
	  {
		  Delayms_Normal();
		  cnt++;
		  PMU->WDTCLR = S_WDTCLR_KEY; //watch dog
		  if(cnt >10)
		  	{
			  	return adcerr;
		  	}
	  }
  	}
  value = 0; 
  for( i = 0; i < 7; i++)  /*get the average value of 7 adc data*/
  {
    cnt=0;
	ANA->ADCCTRL |= S_ANA_ADCCTRL_MTRIG;
    /* Waiting Manual ADC conversion done */
	while (ANA->ADCCTRL & S_ANA_ADCCTRL_MTRIG)
	  {
		  Delayms_Normal();
		  cnt++;
		  PMU->WDTCLR = S_WDTCLR_KEY; //watch dog
		  if(cnt >10)
		  	{
			  	return adcerr;
		  	}
	  }
    value+= ANA->ADCDATAA;
	}
  value = value/7;
  
  //value = 0x45E1;
  ANA->ADCCTRL = 0;
  ANA->REG3 &= ~S_ANA_REG3_ADCPDN;

  /* Calculate temperature */
  real_temp = *(S_NVR_REALTEMP1);
  mea_temp  = *(S_NVR_MEATEMP1);
  /* Calculate checksum1 */
  checksum = ~(real_temp + mea_temp);
  if (checksum == (*(S_NVR_TEMP_CHECKSUM1))) //checksum1 true
  {
    TempDelta = (int16_t)real_temp - (int16_t)mea_temp;
  }
  else
  {
    real_temp = *(S_NVR_REALTEMP2);
    mea_temp  = *(S_NVR_MEATEMP2);
    /* Calculate checksum2 */  
    checksum = ~(real_temp + mea_temp);
    if (checksum == (*(S_NVR_TEMP_CHECKSUM2))) //checksum2 true 
    {
      TempDelta = (int16_t)real_temp - (int16_t)mea_temp;
    }
    else
    {
      return nvrerr;
    }
  }


  rtc_data1 = *(S_NVR_RTC1_P1_P0);
  rtc_data2 = *(S_NVR_RTC1_P2);
  rtc_data3 = *(S_NVR_RTC1_P5_P4);
  rtc_data4 = *(S_NVR_RTC1_P7_P6);
  /* Calculate checksum1 */
  checksum = ~(rtc_data1 + rtc_data2 + rtc_data3 + rtc_data4);
  if (checksum == (*(S_NVR_RTC1_PCHECHSUM))) //checksum1 true
  {
    /* Get information */
    tmp1 = (int16_t)(rtc_data1);
    tmp2 = (int16_t)(rtc_data1 >> 16);
    tmp3 = (int32_t)((int32_t)rtc_data2 + (((int32_t)TempDelta)*256));
  }
  else
  {
    rtc_data1 = *(S_NVR_RTC2_P1_P0);
    rtc_data2 = *(S_NVR_RTC2_P2);
    rtc_data3 = *(S_NVR_RTC2_P5_P4);
    rtc_data4 = *(S_NVR_RTC2_P7_P6);
    /* Calculate checksum2 */
    checksum = ~(rtc_data1 + rtc_data2 + rtc_data3 + rtc_data4);
    if (checksum == (*(S_NVR_RTC2_PCHECHSUM))) //checksum2 true 
    {
      tmp1 = (int16_t)(rtc_data1);
      tmp2 = (int16_t)(rtc_data1 >> 16);
      tmp3 = (int32_t)((int32_t)rtc_data2 + (((int32_t)TempDelta)*256));
    }
    else
    {
      /* Get information */
      return nvrerr;
    }
  }



	//tmp1=RTC->ACP0;
   // tmp2=RTC->ACP1;
  //tmp3=RTC->ACP2;
	temp_org = (int16_t)(((tmp1 * ((value*value)>>16)) + tmp2*value + tmp3) >> 8);
    *result = temp_org/256.0;
	return succ;
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////






/***************************************************************
****************************************************************
**      Function Name  : Read_Temperature_Wakeup              **
**      Author         :                                      **
**      Edit by        : Ahmed Mustafa                        **
**      Date           : 30/3/2023                            **
**      Reentrancy     : Non Reentrant                        **
**      Sync/Async     : Synchronous                          **
**      Parameters (in): uint8_t  &   float *                 **
**      Return value   : uint8_t                              **
**      Description    : Function to Read Temp in WakeUP mode **
****************************************************************
***************************************************************/
uint8_t Read_Temperature_Wakeup(uint8_t ADC_CLKSRC,float* result)
{
     int16_t value;
     uint8_t cnt,i;
	 int32_t tmp1,tmp2,tmp3;
     int16_t temp_org;
      uint32_t real_temp, mea_temp;
     uint32_t rtc_data1, rtc_data2, rtc_data3, rtc_data4;
      uint32_t checksum;
       int16_t TempDelta;
        if (ANA->COMPOUT & S_ANA_COMPOUT_AVCCLV)
    	{
        	return avccerr;
    	}
        
		cnt = 0;
      while (ANA->ADCCTRL & S_ANA_ADCCTRL_MTRIG)
	  {
		  Delayms_Wakeup();
		  cnt++;
		  PMU->WDTCLR = S_WDTCLR_KEY; //watch dog
		  if(cnt >10)
		  	{
			  	return adcerr;
		  	}
	  }
		
	   ANA->ADCCTRL = 0;
       if(ADC_CLKSRC == 0)
	   	{
		        if(ANA->REG3 & S_ANA_REG3_RCHPD)
                {
                    return rcherr;
                }
                ANA->ADCCTRL =  S_ANA_ADCCTRL_CLKDIV_1 + S_ANA_ADCCTRL_CICSKIP_3 + S_ANA_ADCCTRL_DSRSEL_64 + 0x0a;
	   	}
	   else
	   	{
		   	    if((ANA->REG3 & S_ANA_REG3_PLLLPDN) == 0)
                {
                    return plllerr;
                }
                if((ANA->REG9 & S_ANA_REG9_PLLLSEL) <= 1 ||  (ANA->REG9 & S_ANA_REG9_PLLLSEL) == 7)
                {
                    return plllerr;
                }
                //if((PMU->CONTROL & PMU_CONTROL_PLLL_SEL) == 1)
               // {
               //     return plllerr;
              //  }
                if ((ANA->COMPOUT & S_ANA_COMPOUT_LOCKL) == 0)
                {
                     Delayms_Wakeup();
                      if ((ANA->COMPOUT & S_ANA_COMPOUT_LOCKL) == 0)
                      {
                          return plllerr;
                      }
                }

                ANA->ADCCTRL =  S_ANA_ADCCTRL_CLKDIV_1 + S_ANA_ADCCTRL_CICSKIP_3 + S_ANA_ADCCTRL_DSRSEL_64 +S_ANA_ADCCTRL_CLKSEL + 0x0a;
	   	}
	   ANA->REG3 |= S_ANA_REG3_ADCPDN;
  
	  /*---------- Get ADC data ----------*/
     for(i=0;i<4;i++)
     	{
     	   ANA->ADCCTRL |= S_ANA_ADCCTRL_MTRIG;
		    /* Waiting Manual ADC conversion done */
			cnt = 0;
			while (ANA->ADCCTRL & S_ANA_ADCCTRL_MTRIG)
			  {
				  Delayms_Wakeup();
				  cnt++;
				  PMU->WDTCLR = S_WDTCLR_KEY; //watch dog
				  if(cnt >10)
				  	{
					  	return adcerr;
				  	}
			  }
			  if(i == 3)   /*remove first 3 adc data*/
			  	{
				  	value = ANA->ADCDATAA;
			  	}
		      
     	}
     
       // ANA->ADCCTRL = 0;
	   //ANA->REG3 &= ~S_ANA_REG3_ADCPDN;

      /* Calculate temperature */
  real_temp = *(S_NVR_REALTEMP1);
  mea_temp  = *(S_NVR_MEATEMP1);
  /* Calculate checksum1 */
  checksum = ~(real_temp + mea_temp);
  if (checksum == (*(S_NVR_TEMP_CHECKSUM1))) //checksum1 true
  {
    TempDelta = (int16_t)real_temp - (int16_t)mea_temp;
  }
  else
  {
    real_temp = *(S_NVR_REALTEMP2);
    mea_temp  = *(S_NVR_MEATEMP2);
    /* Calculate checksum2 */  
    checksum = ~(real_temp + mea_temp);
    if (checksum == (*(S_NVR_TEMP_CHECKSUM2))) //checksum2 true 
    {
      TempDelta = (int16_t)real_temp - (int16_t)mea_temp;
    }
    else
    {
      //TempDelta = 0;
      return nvrerr;
    }
  }

  rtc_data1 = *(S_NVR_RTC1_P1_P0);
  rtc_data2 = *(S_NVR_RTC1_P2);
  rtc_data3 = *(S_NVR_RTC1_P5_P4);
  rtc_data4 = *(S_NVR_RTC1_P7_P6);
  /* Calculate checksum1 */
  checksum = ~(rtc_data1 + rtc_data2 + rtc_data3 + rtc_data4);
  if (checksum == (*(S_NVR_RTC1_PCHECHSUM))) //checksum1 true
  {
    /* Get information */
    tmp1 = (int16_t)(rtc_data1);
    tmp2 = (int16_t)(rtc_data1 >> 16);
    tmp3 = (int32_t)((int32_t)rtc_data2 + (((int32_t)TempDelta)*256));
  }
  else
  {
    rtc_data1 = *(S_NVR_RTC2_P1_P0);
    rtc_data2 = *(S_NVR_RTC2_P2);
    rtc_data3 = *(S_NVR_RTC2_P5_P4);
    rtc_data4 = *(S_NVR_RTC2_P7_P6);
    /* Calculate checksum2 */
    checksum = ~(rtc_data1 + rtc_data2 + rtc_data3 + rtc_data4);
    if (checksum == (*(S_NVR_RTC2_PCHECHSUM))) //checksum2 true 
    {
      tmp1 = (int16_t)(rtc_data1);
      tmp2 = (int16_t)(rtc_data1 >> 16);
      tmp3 = (int32_t)((int32_t)rtc_data2 + (((int32_t)TempDelta)*256));
    }
    else
    {
      /* Get information */
      return nvrerr;
    }
  }



//     tmp1=RTC->ACP0;
//    tmp2=RTC->ACP1;
//	tmp3=RTC->ACP2;
	temp_org = (int16_t)(((tmp1 * ((value*value)>>16)) + tmp2*value + tmp3) >> 8);
   *result = temp_org/256.0;  
        
        if(ADC_CLKSRC == 0)
	   	{
		   //	ANA->ADCCTRL = ANA_ADCCTRL_MCH + S_ANA_ADCCTRL_CLKDIV_1 + S_ANA_ADCCTRL_CICSKIP_3 + S_ANA_ADCCTRL_DSRSEL_64 + 0;
            	ANA->ADCCTRL = S_ANA_ADCCTRL_CLKDIV_1 + S_ANA_ADCCTRL_CICSKIP_3 + S_ANA_ADCCTRL_DSRSEL_64 + 0;   //ADC????
	   	}
	   else
	   	{
		   //	ANA->ADCCTRL = ANA_ADCCTRL_MCH + S_ANA_ADCCTRL_CLKDIV_1 + S_ANA_ADCCTRL_CICSKIP_3 + S_ANA_ADCCTRL_DSRSEL_64 +S_ANA_ADCCTRL_CLKSEL + 0;
           	ANA->ADCCTRL =  S_ANA_ADCCTRL_CLKDIV_1 + S_ANA_ADCCTRL_CICSKIP_3 + S_ANA_ADCCTRL_DSRSEL_64 +S_ANA_ADCCTRL_CLKSEL + 0;
	   	}
  
        ANA->ADCCTRL |= S_ANA_ADCCTRL_MTRIG;
    /* Waiting Manual ADC conversion done */
	cnt = 0;
	while (ANA->ADCCTRL & S_ANA_ADCCTRL_MTRIG)
	  {
		  Delayms_Wakeup();
		  cnt++;
		  PMU->WDTCLR = S_WDTCLR_KEY; //watch dog
		  if(cnt >10)
		  	{
			  	return adcerr;
		  	}
	  
	  }
       
       ANA->ADCCTRL = 0;
	   ANA->REG3 &= ~S_ANA_REG3_ADCPDN;
       	return succ;
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////









/*************************************************************
**************************************************************
**      Function Name  : CalRTC_Wakeup                      **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint32_t  &   uint8_t              **
**      Return value   : uint8_t                            **
**      Description    : Function Calculate RTC In WakeUp   **
**************************************************************
*************************************************************/
uint8_t CalRTC_Wakeup(uint32_t PCLK,uint8_t ADC_CLKSRC)
{
        float tmpval;
        uint8_t status_read;
        uint8_t status_cal;
        status_read = Read_Temperature_Wakeup(ADC_CLKSRC,&tmpval);
        if (status_read == 0) 
        	{
			  status_cal = CalRTC_REG(PCLK,0,tmpval);
              if( status_cal == 0)
					{
						return succ;
					}
				else
					{
						return status_cal;
					}
        	}
		else
			{
				return status_read;
			}
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////







/*************************************************************
**************************************************************
**      Function Name  : CalRTC_Normal                      **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint32_t                           **
**      Return value   : uint8_t                            **
**      Description    : Function to Calculate RTC in Normal**
**************************************************************
*************************************************************/
uint8_t CalRTC_Normal(uint32_t PCLK)
{
       float tmpval;
        uint8_t status_read;
        uint8_t status_cal;
        status_read = Read_Temperature_Normal(&tmpval);
	   if ( status_read == 0) 
        	{
				status_cal = CalRTC_REG(PCLK,1,tmpval);
              if( status_cal == 0)
					{
						return succ;
					}
				else
					{
						return status_cal;
					}
        	}
	   else
			{
				return status_read;
			}
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////






/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_getCalendarTime            **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): RTC_TimeTypeDef *                  **
**      Return value   : None                               **
**      Description    : Function to get the Calendar Time  **
**************************************************************
*************************************************************/
void S_RTC_C_getCalendarTime(RTC_TimeTypeDef *gTime)
{
   __IO uint32_t tmp;
  
  /* Dummy read-operation to RTC->LOAD register */
  tmp = RTC->LOAD;
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
  /* Read RTC time registers */
  gTime->Seconds  = BCDToDecimal((uint8_t)RTC->SEC);
  gTime->Minutes  = BCDToDecimal((uint8_t)RTC->MIN); 
  gTime->Hours    = BCDToDecimal((uint8_t)RTC->HOUR); 
  gTime->Date     = BCDToDecimal((uint8_t)RTC->DAY); 
  gTime->WeekDay  = BCDToDecimal((uint8_t)RTC->WEEK); 
  gTime->Month    = BCDToDecimal((uint8_t)RTC->MON); 
  gTime->Year     = BCDToDecimal((uint8_t)RTC->YEAR);

}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////





/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_Update                     **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): RTC_TimeTypeDef *                  **
**      Return value   : None                               **
**      Description    : Function to Update the RTC         **
**************************************************************
*************************************************************/
void S_RTC_C_Update(RTC_TimeTypeDef *sTime)
{
  /*
  assert_param(IS_RTC_TIME_HOURS(DecimalToBCD(sTime->Hours))); 
  assert_param(IS_RTC_TIME_MINS(DecimalToBCD(sTime->Minutes))); 
  assert_param(IS_RTC_TIME_SECS(DecimalToBCD(sTime->Seconds))); 
  assert_param(IS_RTC_TIME_YEAR(DecimalToBCD(sDate->Year)));
  assert_param(IS_RTC_TIME_MONTH(DecimalToBCD(sDate->Month)));
  assert_param(IS_RTC_TIME_DATE(DecimalToBCD(sDate->Date))); 
  assert_param(IS_RTC_TIME_WEEKDAY(DecimalToBCD(sDate->WeekDay))); 

   */
  
  /* Disable RTC Registers write-protection */
  RTC_WriteProtection(DISABLE);
  
  /* Write RTC time registers */
  RTC->SEC  = DecimalToBCD(sTime->Seconds) ;
  RTC->MIN  = DecimalToBCD(sTime->Minutes) ; 
  RTC->HOUR = DecimalToBCD(sTime->Hours) ; 
  RTC->DAY  = DecimalToBCD(sTime->Date) ; 
  RTC->WEEK = DecimalToBCD(sTime->WeekDay) ; 
  RTC->MON  = DecimalToBCD(sTime->Month) ; 
  RTC->YEAR = DecimalToBCD(sTime->Year) ; 
  
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////






/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_UpdateTime                 **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): rtc_t_t *                          **
**      Return value   : None                               **
**      Description    : Function to Update the time        **
**************************************************************
*************************************************************/
void S_RTC_C_UpdateTime(rtc_t_t *sTime)
{ 
  assert_param(IS_RTC_TIME_HOURS(DecimalToBCD(sTime->Hours))); 
  assert_param(IS_RTC_TIME_MINS(DecimalToBCD(sTime->Minutes))); 
  assert_param(IS_RTC_TIME_SECS(DecimalToBCD(sTime->Seconds))); 
  /* Disable RTC Registers write-protection */
  RTC_WriteProtection(DISABLE);
  
  /* Write RTC time registers */
  RTC->SEC  = DecimalToBCD(sTime->Seconds) ;
  RTC->MIN  = DecimalToBCD(sTime->Minutes) ; 
  RTC->HOUR = DecimalToBCD(sTime->Hours) ; 
  
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////





/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_UpdateDate                 **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): rtc_d_t *                          **
**      Return value   : None                               **
**      Description    : Function to Update the Date        **
**************************************************************
*************************************************************/
void S_RTC_C_UpdateDate(rtc_d_t *sDate)
{ 
  assert_param(IS_RTC_TIME_YEAR(DecimalToBCD(sDate->Year)));
  assert_param(IS_RTC_TIME_MONTH(DecimalToBCD(sDate->Month)));
  assert_param(IS_RTC_TIME_DATE(DecimalToBCD(sDate->Date))); 
  assert_param(IS_RTC_TIME_WEEKDAY(DecimalToBCD(sDate->WeekDay))); 
  /* Disable RTC Registers write-protection */
  RTC_WriteProtection(DISABLE);
  
  /* Write RTC time registers */
  RTC->DAY  = DecimalToBCD(sDate->Date) ; 
  RTC->WEEK = DecimalToBCD(sDate->WeekDay) ; 
  RTC->MON  = DecimalToBCD(sDate->Month) ; 
  RTC->YEAR = DecimalToBCD(sDate->Year) ; 
  
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////





/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_SetRTCSEC                  **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint8_t                            **
**      Return value   : None                               **
**      Description    : Function to Set the second         **
**************************************************************
*************************************************************/
void S_RTC_C_SetRTCSEC (uint8_t uisec)
{
  assert_param(IS_RTC_TIME_SECS(DecimalToBCD(sTime->Seconds))); 
  RTC_WriteProtection(DISABLE);
  
  /* Write RTC time registers */
  RTC->SEC  = DecimalToBCD(uisec);
  
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////





/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_SetRTCMIN                  **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint8_t                            **
**      Return value   : None                               **
**      Description    : Function to Set the RTC Minutes    **
**************************************************************
*************************************************************/
void S_RTC_C_SetRTCMIN (uint8_t uimin)
{
  assert_param(IS_RTC_TIME_MINS(DecimalToBCD(sTime->Minutes))); 
  RTC_WriteProtection(DISABLE);
 
  RTC->MIN  = DecimalToBCD(uimin);

  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////




/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_SetRTCHOUR                 **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint8_t                            **
**      Return value   : uint32_t                           **
**      Description    : Function to Set the Hour           **
**************************************************************
*************************************************************/
void S_RTC_C_SetRTCHOUR (uint8_t uihour)
{
  assert_param(IS_RTC_TIME_HOURS(DecimalToBCD(sTime->Hours))); 
   RTC_WriteProtection(DISABLE);
  
  RTC->HOUR = DecimalToBCD(uihour);
  
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////






/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_SetRTCDAY                    **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint8_t                            **
**      Return value   : None                               **
**      Description    : Function to Set the day            **
**************************************************************
*************************************************************/
void S_RTC_C_SetRTCDAY (uint8_t uiday)
{
  assert_param(IS_RTC_TIME_DATE(DecimalToBCD(sDate->Date))); 
  RTC_WriteProtection(DISABLE);
  
  RTC->DAY  = DecimalToBCD(uiday);
 
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////







/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_SetRTCDOW                  **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint8_t                            **
**      Return value   : None                               **
**      Description    : Function to Set The day of Week    **
**************************************************************
*************************************************************/
void S_RTC_C_SetRTCDOW (uint8_t uidow)
{
  assert_param(IS_RTC_TIME_WEEKDAY(DecimalToBCD(sDate->WeekDay))); 
  RTC_WriteProtection(DISABLE);
  
  RTC->WEEK = DecimalToBCD(uidow);

  
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////









/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_SetRTCMON                  **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint8_t                            **
**      Return value   : None                               **
**      Description    : Function to Set the month          **
**************************************************************
*************************************************************/
void S_RTC_C_SetRTCMON (uint8_t uimon)
{
  assert_param(IS_RTC_TIME_MONTH(DecimalToBCD(sDate->Month)));
  RTC_WriteProtection(DISABLE);
  
  RTC->MON  = DecimalToBCD(uimon);
  
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////





/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_SetRTCYEAR                 **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint8_t                            **
**      Return value   : None                               **
**      Description    : Function to Set the year           **
**************************************************************
*************************************************************/
void S_RTC_C_SetRTCYEAR (uint8_t uiyear)
{
  assert_param(IS_RTC_TIME_YEAR(DecimalToBCD(sDate->Year)));
  RTC_WriteProtection(DISABLE);
  
  RTC->YEAR = DecimalToBCD(uiyear);
  
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////






/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_GetRTCYEAR                 **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): None                               **
**      Return value   : uint8_t                            **
**      Description    : Function to Get the year           **
**************************************************************
*************************************************************/
uint8_t S_RTC_C_GetRTCYEAR()
 {
   __IO uint32_t tmp;
    uint8_t val ;
  
  /* Dummy read-operation to RTC->LOAD register */
  tmp = RTC->LOAD;
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
  val = BCDToDecimal((uint8_t)RTC->YEAR) ;
  
  return   val; 

 }
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////







/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_GetRTCMON                  **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): none                               **
**      Return value   : uint8_t                            **
**      Description    : Function to Get The Month          **
**************************************************************
*************************************************************/
uint8_t S_RTC_C_GetRTCMON()
 {
   __IO uint32_t tmp;
   
   uint8_t val ;
  
  /* Dummy read-operation to RTC->LOAD register */
  tmp = RTC->LOAD;
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
  val = BCDToDecimal((uint8_t) RTC->MON) ;
  
  return   val; 
 
 }
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////









/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_GetRTCDAY                  **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): none                               **
**      Return value   : uint8_t                            **
**      Description    : Function to Get The Day            **
**************************************************************
*************************************************************/
uint8_t S_RTC_C_GetRTCDAY()
 {
   __IO uint32_t tmp;
   uint8_t val ;
  
  /* Dummy read-operation to RTC->LOAD register */
  tmp = RTC->LOAD;
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
  val = BCDToDecimal((uint8_t) RTC->DAY) ;
  
  return   val;   
 
 }
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////










/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_GetRTCDOW                  **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): none                               **
**      Return value   : uint8_t                            **
**      Description    : Function to Get The day of Week    **
**************************************************************
*************************************************************/
uint8_t S_RTC_C_GetRTCDOW()
 {
   __IO uint32_t tmp;
   
   uint8_t val ;
  
  /* Dummy read-operation to RTC->LOAD register */
  tmp = RTC->LOAD;
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
  val = BCDToDecimal((uint8_t) RTC->WEEK) ;
  
  return   val;   

 }
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////




/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_GetRTCHOUR                 **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): none                               **
**      Return value   : uint8_t                            **
**      Description    : Function to Get The Hour           **
**************************************************************
*************************************************************/

uint8_t S_RTC_C_GetRTCHOUR()
 {
   __IO uint32_t tmp;
   uint8_t val ;
  
  /* Dummy read-operation to RTC->LOAD register */
  tmp = RTC->LOAD;
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
  val = BCDToDecimal((uint8_t) RTC->HOUR) ;
  
  return   val;   

 }
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////






/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_GetRTCMIN                  **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): none                               **
**      Return value   : uint8_t                            **
**      Description    : Function to Get The Min            **
**************************************************************
*************************************************************/
uint8_t S_RTC_C_GetRTCMIN()
 {
   __IO uint32_t tmp;
   uint8_t val ;
  
  /* Dummy read-operation to RTC->LOAD register */
  tmp = RTC->LOAD;
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
  val = BCDToDecimal((uint8_t)RTC->MIN) ;
  
  return   val;   

 }
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////






/*************************************************************
**************************************************************
**      Function Name  : S_RTC_C_GetRTCSEC                  **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): none                               **
**      Return value   : uint8_t                            **
**      Description    : Function to Get The Second         **
**************************************************************
*************************************************************/
uint8_t S_RTC_C_GetRTCSEC()
 {
   __IO uint32_t tmp;
   uint8_t val ;
   
  /* Dummy read-operation to RTC->LOAD register */
  tmp = RTC->LOAD;
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro(); 
  
  val = BCDToDecimal((uint8_t)RTC->SEC) ;
  
  return   val; 
 }
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////







/*************************************************************
**************************************************************
**      Function Name  : RTC_cal_update                     **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): none                               **
**      Return value   : None                               **
**      Description    : Function to Update                 **
**************************************************************
*************************************************************/
void RTC_cal_update(void)
{
   uint32_t rtc_read_cal_val=RESET;
   RTC_WriteProtection(DISABLE);
   rtc_read_cal_val=RTC->CAL;
   if(!((rtc_read_cal_val>=CAL_MIN_VAL)&&(rtc_read_cal_val<=CAL_MAX_VAL)))
   {   
     RTC->CAL=CAL_MEAN_VAL;
   } 
   RTC_WriteProtection(ENABLE);     
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////




/*************************************************************
**************************************************************
**      Function Name  : S_RTC_init                         **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): none                               **
**      Return value   : None                               **
**      Description    : Function to Init RTC               **
**************************************************************
*************************************************************/
 void S_RTC_init(void)
 {
  // RTC_cal_update();                 //open it if we use an old version of VANGO
  // CalRTC_Normal(S_CLK_GetPCLKFreq());
   RTC_WKUSecondsConfig(1);
   S_RTC_ENABLE_INT();
   // PMU_SleepWKUSRC_Config_RTC(PMU_RTCEVT_WKUSEC, 0);
 }
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////


