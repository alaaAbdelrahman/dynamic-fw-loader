#include "config.h"
#if (MicroController == Micro_V94XX)

/**
  ******************************************************************************
  * @file    lib_rtc.c 
  * @author  Application Team
  * @version V1.5.0
  * @date    2022-09-20
  * @brief   RTC library.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "V94XX_RTC.h"
#include "V94XX.h"
#include "V94XX_CLK.h"


#define RTCPWD_KEY    0x5AA55AA5
#define RTCCE_SETKEY  0xA55AA55B
#define RTCCE_CLRKEY  0xA55AA55A

//ATClockSource
#define RTC_ACCTRL_ADCSEL_Pos         (3U)                                     
#define RTC_ACCTRL_ADCSEL_Msk         (0x1U << RTC_ACCTRL_ADCSEL_Pos)          /*!< 0x00000008 */
#define RTC_ACCTRL_ADCSEL              RTC_ACCTRL_ADCSEL_Msk                   /*!< ADC source select register */ 
#define RTC_ACCTRL_ACCLK_Pos          (4U)                                     
#define RTC_ACCTRL_ACCLK_Msk          (0x3U << RTC_ACCTRL_ACCLK_Pos)           /*!< 0x00000030 */
#define RTC_ACCTRL_ACCLK               RTC_ACCTRL_ACCLK_Msk                    /*!< Auto-trigger clock source selection */ 
#define RTC_ACCTRL_ACCLK_0            (0x0U << RTC_ACCTRL_ACCLK_Pos)
#define RTC_ACCTRL_ACCLK_1            (0x1U << RTC_ACCTRL_ACCLK_Pos)
#define RTC_ACCTRL_ACCLK_2            (0x2U << RTC_ACCTRL_ACCLK_Pos)
#define RTC_ACCTRL_ACCLK_3            (0x3U << RTC_ACCTRL_ACCLK_Pos)
#define RTC_ACCTRL_ACDEL_Pos          (6U)                                     
#define RTC_ACCTRL_ACDEL_Msk          (0x3U << RTC_ACCTRL_ACDEL_Pos)           /*!< 0x000000C0 */
#define RTC_ACCTRL_ACDEL               RTC_ACCTRL_ACDEL_Msk                    /*!< Auto-calibration delay period selection, before doing auto-calibration, a specified delay will be applied to ensure the main power is stable */ 
#define RTC_ACCTRL_ACDEL_0            (0x0U << RTC_ACCTRL_ACDEL_Pos) 
#define RTC_ACCTRL_ACDEL_1            (0x1U << RTC_ACCTRL_ACDEL_Pos)
#define RTC_ACCTRL_ACDEL_2            (0x2U << RTC_ACCTRL_ACDEL_Pos)
#define RTC_ACCTRL_ACDEL_3            (0x3U << RTC_ACCTRL_ACDEL_Pos)
#define RTC_ACCTRL_ACPER_Pos          (8U)                                     
#define RTC_ACCTRL_ACPER_Msk          (0x3FU << RTC_ACCTRL_ACPER_Pos)          /*!< 0x00003F00 */
#define RTC_ACCTRL_ACPER               RTC_ACCTRL_ACPER_Msk                    /*!< Auto trigger period control register, the actual period is controlled by (ACPER + 1)*ACCLK.  For example, when ACCLK is set to 2 (1 minute), and ACPER is set to 5, then the auto trigger period is (5+1)*1 minute = 6 minutes */ 
//ATClockSource
#define RTC_ATCS_DISABLE      RTC_ACCTRL_ACCLK_0
#define RTC_ATCS_SEC          RTC_ACCTRL_ACCLK_1
#define RTC_ATCS_MIN          RTC_ACCTRL_ACCLK_2
#define RTC_ATCS_HOUR         RTC_ACCTRL_ACCLK_3

#define IS_RTC_AUTOCAL_ATCS(__ATCS__)  (((__ATCS__) == RTC_ATCS_DISABLE) ||\
                                        ((__ATCS__) == RTC_ATCS_SEC)     ||\
                                        ((__ATCS__) == RTC_ATCS_MIN)     ||\
                                        ((__ATCS__) == RTC_ATCS_HOUR))

void RTC_TrigSourceConfig(uint32_t TrigSource, uint32_t Period);
uint32_t RTC_AutoCalCmdUnlock(uint32_t NewState);
void RTC_StartAutoCalManual(void);
void RTC_WaitForAutoCalManual(void);

/**
  * @brief  RTC registers write protection control. 
  * @param  NewState:
  *             ENABLE      
  *             DISABLE
  * @retval None
  */
void RTC_WriteProtection(uint32_t NewState)
{
  /* Check parameters */
  assert_parameters(IS_FUNCTIONAL_STATE(NewState));
  
  /* Enable RTC Write-Protection */
  if (NewState != DISABLE)
  {
    RTC->PWD = RTCPWD_KEY;
    RTC->CE = RTCCE_CLRKEY;
  }
  /* Disable RTC Write-Protection */
  else
  {
    RTC->PWD = RTCPWD_KEY;
    RTC->CE = RTCCE_SETKEY;       
  }
}

/**
  * @brief  Wait until the RTC registers (be W/R protected) are synchronized
  *         with RTC APB clock.
  *
  * @note   The RTC Resynchronization mode is write protected, use the 
  *         RTC_WriteProtection(DISABLE) before calling this function.
  *           Write-Operation process as follows: 
  *             1. RTC_WriteProtection(DISABLE);  
  *             2. RTC Registers write operation(only first write-operation be 
  *                valid on the same register).
  *             3. RTC_WriteProtection(ENABLE); 
  *             4. RTC_WaitForSynchro();  Wait until the RTC registers be 
  *                synchronized by calling this function.
  * @retval None
  */
void RTC_WaitForSynchro(void)
{
  uint32_t timeout = 0 ;
  while ((RTC->CE & RTC_CE_BSY)&&(timeout++ <200)) 
  {
  }
}

/**
  * @brief      Write RTC registers(continuous/be write-protected). 
  * @param[in]  StartAddr   the start address of registers be written
  * @param[in]  wBuffer     pointer to write
  * @param[in]  Len         number of registers be written
  * @retval     None
  */
void RTC_WriteRegisters(uint32_t StartAddr, const uint32_t *wBuffer, uint8_t Len)
{
  uint8_t cnt;
  
  /* Parameter check */
  assert_parameters(IS_RTC_REGOP_STARTADDR(StartAddr));

  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();  
  
  /* Disable RTC Registers write-protection */
  RTC_WriteProtection(DISABLE);
  
  /* Write registers */
  for (cnt=0; cnt<Len; cnt++)
  {
    *(volatile uint32_t *)(StartAddr) = *(wBuffer++);
    StartAddr += 4;
  }
  
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
}

/**
  * @brief      Read RTC registers(continuous/be read-protected). 
  * @param[in]  StartAddr   the start address of registers be read
  * @param[out] rBuffer     pointer to read
  * @param[in]  Len         number of registers be read
  * @retval     None
  */
void RTC_ReadRegisters(uint32_t StartAddr, uint32_t *rBuffer, uint8_t Len)
{
  __IO uint32_t tmp;
  uint8_t cnt;
  
  /* Parameter check */
  assert_parameters(IS_RTC_REGOP_STARTADDR(StartAddr));

  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
  /* Dummy read-operation to RTC->LOAD */
  tmp = RTC->LOAD;
  tmp += 1; 
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
  /* Read registers */
  for (cnt=0; cnt<Len; cnt++)
  {
    *(rBuffer++) = *(volatile uint32_t *)(StartAddr);
    StartAddr += 4;
  }
}

/**
  * @brief  Set RTC current time.
  * @param  sTime: Pointer to Time structure
  * @retval None
  */
void RTC_SetTime(RTC_TimeTypeDef *sTime)
{
  /* Parameter check */
  assert_parameters(IS_RTC_TIME_YEAR(sTime->Year));
  assert_parameters(IS_RTC_TIME_MONTH(sTime->Month));
  assert_parameters(IS_RTC_TIME_DATE(sTime->Date)); 
  assert_parameters(IS_RTC_TIME_WEEKDAY(sTime->WeekDay)); 
  assert_parameters(IS_RTC_TIME_HOURS(sTime->Hours)); 
  assert_parameters(IS_RTC_TIME_MINS(sTime->Minutes)); 
  assert_parameters(IS_RTC_TIME_SECS(sTime->Seconds)); 

  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
  /* Disable RTC Registers write-protection */
  RTC_WriteProtection(DISABLE);
  
  /* Write RTC time registers */
  RTC->SEC  = sTime->Seconds;
  RTC->MIN  = sTime->Minutes;
  RTC->HOUR = sTime->Hours;
  RTC->DAY  = sTime->Date;
  RTC->WEEK = sTime->WeekDay;
  RTC->MON  = sTime->Month;
  RTC->YEAR = sTime->Year;
  
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
}

/**
  * @brief  Get RTC current time.
  * @param  gTime: Pointer to Time structure
  * @retval None
*/
void RTC_GetTime(RTC_TimeTypeDef *gTime)
{
  __IO uint32_t dummy_data = 0;

  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
  /* Dummy read-operation to RTC->LOAD register */
  dummy_data = RTC->LOAD;
  dummy_data += 1;
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
  /* Read RTC time registers */
  gTime->Seconds  = RTC->SEC;
  gTime->Minutes  = RTC->MIN;
  gTime->Hours    = RTC->HOUR;
  gTime->Date     = RTC->DAY;
  gTime->WeekDay  = RTC->WEEK;
  gTime->Month    = RTC->MON;
  gTime->Year     = RTC->YEAR;
}

/**
  * @brief  Interrupt configure.
  * @param  INTMask: can use the ��|�� operator
                RTC_INT_CEILLE
                RTC_INT_WKUCNT
                RTC_INT_MIDNIGHT
                RTC_INT_WKUHOUR 
                RTC_INT_WKUMIN 
                RTC_INT_WKUSEC
                RTC_INT_TIMEILLE 
            NewState:
                ENABLE
                DISABLE
  * @retval None
  */
void RTC_INTConfig(uint32_t INTMask, uint32_t NewState)
{
  uint32_t tmp;
  
  /* Parameter check */
  assert_parameters(IS_RTC_INT(INTMask));  
  assert_parameters(IS_FUNCTIONAL_STATE(NewState));
  
  tmp = RTC->INTEN;
  tmp &= ~(0x1UL);
  
  if (NewState == ENABLE)
    tmp |= INTMask;
  else
    tmp &= ~INTMask;
  
  RTC->INTEN = tmp;
}

/**
  * @brief  Get interrupt status.
  * @param  INTMask:
                RTC_INTSTS_CEILLE
                RTC_INTSTS_WKUCNT
                RTC_INTSTS_MIDNIGHT
                RTC_INTSTS_WKUHOUR 
                RTC_INTSTS_WKUMIN 
                RTC_INTSTS_WKUSEC
                RTC_INTSTS_TIMEILLE 
    * @retval 1: status set
              0: status reset.
  */
uint8_t RTC_GetINTStatus(uint32_t FlagMask)
{
  /* Parameter check */
  assert_parameters(IS_RTC_INTFLAGR(FlagMask));

  if (RTC->INTSTS&FlagMask)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
  * @brief  Clear interrupt status.
  * @param  INTMask: can use the ��|�� operator
                RTC_INTSTS_CEILLE
                RTC_INTSTS_WKUCNT
                RTC_INTSTS_MIDNIGHT
                RTC_INTSTS_WKUHOUR 
                RTC_INTSTS_WKUMIN 
                RTC_INTSTS_WKUSEC
                RTC_INTSTS_TIMEILLE 
  * @retval  None
  */
void RTC_ClearINTStatus(uint32_t FlagMask)
{
  /* Parameter check */
  assert_parameters(IS_RTC_INTFLAGC(FlagMask));
  
  RTC->INTSTS = FlagMask;
}
//add by A_E--->
/**
  * @brief  Fills each RTCAC_InitStruct member with its default value.
  * @param  RTCAC_InitStruct: pointer to an RTC_AutCalType structure which will be initialized.
  * @retval None
  */
void RTC_AutoCalStructInit(RTC_AutCalType *RTCAC_InitStruct)
{
  /*------------ Reset RTC AutCal init structure parameters values -----------*/
  /* Initialize the ADCSource member */ 
  RTCAC_InitStruct->ADCSource = RTC_ADCS_DATA;
  /* Initialize the ATClockSource member */ 
  RTCAC_InitStruct->ATClockSource = RTC_ATCS_DISABLE;
  /* Initialize the ATDelay member */ 
  RTCAC_InitStruct->ATDelay = RTC_ATDELAY_15MS;
  /* Initialize the Period member */ 
  RTCAC_InitStruct->Period = 0;
}

/**
  * @brief  Auto calibration initialization.
  * @param  InitStruct: pointer to AutoCal_InitType Auto calibration configuration.
  *           ATDelay:
  *               RTC_ATDELAY_15MS
  *               RTC_ATDELAY_31MS
  *               RTC_ATDELAY_62MS
  *               RTC_ATDELAY_125MS
  *           ATClockSource:
  *               RTC_ATCS_DISABLE
  *               RTC_ATCS_SEC
  *               RTC_ATCS_MIN
  *               RTC_ATCS_HOUR
  *           ADCSource:
  *               RTC_ADCS_DATA
  *               RTC_ADCS_PORT
  *           Period: 0 ~ 63
  * @note   Auto trigger period is (Period+1)*1, unit is set by ATClockSource.
  *         Auto trigger function is not valid when ATClockSource is RTC_ATCS_DISABLE.
  * @retval None
  */
void RTC_AutoCalInit(RTC_AutCalType *InitStruct)
{
  uint32_t tmp;
  
  /* Parameter check */
  assert_param(IS_RTC_AUTOCAL_ATDLY(InitStruct->ATDelay));
  assert_param(IS_RTC_AUTOCAL_ATCS(InitStruct->ATClockSource));
  assert_param(IS_RTC_AUTOCAL_ADCSRC(InitStruct->ADCSource));
  assert_param(IS_RTC_AUTOCAL_PERIOD(InitStruct->Period));
  
  tmp = RTC->ACCTRL;
  tmp &= ~(RTC_ACCTRL_ACPER\
          |RTC_ACCTRL_ACDEL\
          |RTC_ACCTRL_ACCLK\
          |RTC_ACCTRL_ADCSEL);
  tmp |= (InitStruct->ADCSource\
         |InitStruct->ATClockSource\
         |InitStruct->ATDelay\
         |((InitStruct->Period << RTC_ACCTRL_ACPER_Pos) & RTC_ACCTRL_ACPER));
    
  /* Disable RTC Registers write-protection */
  RTC_WriteProtection(DISABLE);
  RTC->ACCTRL = tmp;
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
}

/**
  * @brief  ADC Auto-calibration enable control.
  * @param  NewState:
  *             ENABLE
  *             DISABLE
  * @retval None
  */
void RTC_AutoCalCmd(uint32_t NewState)
{
  /* Parameter check */
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
  
  /* Disable RTC Registers write-protection */
  RTC_WriteProtection(DISABLE);
  
  if (NewState != DISABLE)  RTC->ACCTRL |= RTC_ACCTRL_ACEN;
  else                      RTC->ACCTRL &= ~RTC_ACCTRL_ACEN;
  
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
}





/**
  * @brief  Get auto-calibration busy flag.
  * @param  None
  * @retval 1   flag set
  *         0   flag reset.
  */
uint8_t RTC_GetACBusyFlag(void)
{
  if (RTC->INTSTS & RTC_INTSTS_ACBSY)   return (1);
  else                                  return (0);
}


/*
  * @brief  Multi-second wake up configure.
  * @param  nPeriod��N seconds interval.
  * @note   For the first interrupt generated by calling this function, it may 
  *         have < 1 sec error if the new WKUSEC number(parameter) is not equal 
  *         to current WKUSEC number.  If the new WKUSEC is equal to current WKUSEC, 
  *         the first interrupt time may have 0~(WKUSEC +1) variation.  
  *         To avoid this problem, set an alternative parameter (like 1) by calling
  *         this function, then set the correct parameter to it.
  * @retval None
  */
void RTC_WKUSecondsConfig(uint8_t nPeriod)
{
  /* Parameter check */
  assert_parameters(IS_RTC_WKUSEC_PERIOD(nPeriod));  

  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
  /* Disable RTC Registers write-protection */
  RTC_WriteProtection(DISABLE);
  
  /* Write registers */
  RTC->WKUSEC = nPeriod - 1;
  
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();  
}

/*
  * @brief  Multi-minute wake up configure.
  * @param  nPeriod��N minute interval.
  * @note   For the first interrupt generated by calling this function, it may 
  *         have < 1 min error if the new WKUMIN number(parameter) is not equal 
  *         to current WKUMIN number.  If the new WKUMIN is equal to current WKUMIN, 
  *         the first interrupt time may have 0~(WKUMIN +1) variation.  
  *         To avoid this problem, set an alternative parameter (like 1) by calling
  *         this function, then set the correct parameter to it.
  * @retval None
  */
void RTC_WKUMinutesConfig(uint8_t nPeriod)
{
  /* Parameter check */
  assert_parameters(IS_RTC_WKUMIN_PERIOD(nPeriod)); 

  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
  /* Disable RTC Registers write-protection */
  RTC_WriteProtection(DISABLE);
  
  /* Write registers */
  RTC->WKUMIN = nPeriod - 1;
  
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();  
}

/*
  * @brief  Multi-hour wake up configure.
  * @param  nPeriod��N hour interval.
  * @note   For the first interrupt generated by calling this function, it may 
  *         have < 1 hour error if the new WKUHOUR number(parameter) is not equal 
  *         to current WKUHOUR number.  If the new WKUHOUR is equal to current WKUHOUR, 
  *         the first interrupt time may have 0~(WKUHOUR +1) variation.  
  *         To avoid this problem, set an alternative parameter (like 1) by calling
  *         this function, then set the correct parameter to it.
  * @retval None
  */
void RTC_WKUHoursConfig(uint8_t nPeriod)
{
  /* Parameter check */
  assert_parameters(IS_RTC_WKUHOUR_PERIOD(nPeriod));

  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
  /* Disable RTC Registers write-protection */
  RTC_WriteProtection(DISABLE);
  
  /* Write registers */
  RTC->WKUHOUR = nPeriod - 1;
  
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro(); 
}

/**
  * @brief  RTC counter wake up configure.
  * @param  nClock: 1 ~ 0x1000000
            CNTCLK:
                RTC_WKUCNT_RTCCLK
                RTC_WKUCNT_2048
                RTC_WKUCNT_512
                RTC_WKUCNT_128
  * @retval None
  */
void RTC_WKUCounterConfig(uint32_t nClock,uint32_t CNTCLK)
{
  /* Parameter check */
  assert_parameters(IS_RTC_WKUCNT_PERIOD(nClock));
  assert_parameters(IS_RTC_WKUCNT_CNTSEL(CNTCLK));

  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
  /* Disable RTC Registers write-protection */
  RTC_WriteProtection(DISABLE);
  
  /* Write registers */
  RTC->WKUCNT = (CNTCLK & RTC_WKUCNT_CNTSEL) | (nClock -1 );
  
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();   
}

/**
  * @brief  Gets RTC wake-up counter value.
  * @retval RTC wake-up counter value
  */
uint32_t RTC_GetWKUCounterValue(void)
{
  return RTC->WKUCNTR;
}

/**
  * @brief      RTC clock prescaler configure. 
  * @param[in]  Prescaler:
  *                     RTC_CLKDIV_1
  *                     RTC_CLKDIV_4
  * @retval     None
  */
void RTC_PrescalerConfig(uint32_t Prescaler)
{
  uint32_t tmp;
  
  /* Parameter check */
  assert_parameters(IS_RTC_CLKDIV(Prescaler));  
  
  tmp = RTC->PSCA;
  tmp &= ~RTC_PSCA_PSCA;
  tmp |= Prescaler;

  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  /* Disable RTC Registers write-protection */
  RTC_WriteProtection(DISABLE);
  RTC->PSCA = tmp;
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();   
}

/**
  * @brief  RTC PLLDIV frequency configure.
  * @param  nfrequency(HZ): the frequency of RTC PLLDIV output configuration.
  * @note   Ensure clocks be configured by calling function CLK_ClockConfig(), 
  *         get correct PCLK frequency by calling function CLK_GetPCLKFreq().
  * @retval None
  */
void RTC_PLLDIVConfig(uint32_t nfrequency)
{
  RTC->DIV = CLK_GetPCLKFreq()/2/nfrequency - 1;
}

/**
  * @brief  RTC PLLDIV output enable.
  * @param  NewState:
  *             ENABLE
  *             DISABLE
  * @retval None
  */
void RTC_PLLDIVOutputCmd(uint8_t NewState)
{
  /* Parameter check */
  assert_parameters(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState == ENABLE)   RTC->CTL |= RTC_CTL_RTCPLLOE;
  else                      RTC->CTL &= ~RTC_CTL_RTCPLLOE;
}

/**
  * @brief  Trigger a RTC Auto-calibration manually to reset the state machine.
  * @param  None
  * @retval None
  */
void RTC_Unlock(void)
{
  /*Disable automatic triggering*/
  RTC_TrigSourceConfig(RTC_ATCS_DISABLE, 0); 
  /*Enable automatic calibration*/
  RTC_AutoCalCmdUnlock(ENABLE);
  /*Manul trigger*/
  RTC_StartAutoCalManual();
  /*Wait for the manual trigger to complete*/
  RTC_WaitForAutoCalManual();
  /*Disable automatic calibration*/
  RTC_AutoCalCmdUnlock(DISABLE);
}

/**
  * @brief  ADC Auto-calibration unlock enable control.
  * @note   When DISABLE is selected, the automatic triggering of the RTC-auto-calibration must be turned off by calling 
  *         RTC_TrigSourceConfig(RTC_ATCS_DISABLE, 0) before using this function.
  * @param  NewState:
  *             ENABLE
  *             DISABLE
  * @retval 0: Function succeeded
  */
uint32_t RTC_AutoCalCmdUnlock(uint32_t NewState)
{
  uint32_t tmp;

  /* Parameter check */
  assert_parameters(IS_FUNCTIONAL_STATE(NewState));
  
  tmp = RTC->ACCTRL;
  if (NewState == DISABLE)
  {
    tmp &= ~RTC_ACCTRL_ACEN;
  }
  else
  {
    tmp |= RTC_ACCTRL_ACEN;
  }
  
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  /* Disable RTC Registers write-protection */
  RTC_WriteProtection(DISABLE);
  
  RTC->ACCTRL = tmp;
  
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  
  return 0;
}

/**
  * @brief  RTC automatic calibration auto-trigger source configure.
  * @param  TrigSource:
  *               RTC_ATCS_DISABLE
  *               RTC_ATCS_SEC
  *               RTC_ATCS_MIN
  *               RTC_ATCS_HOUR
  *         Period: 0 ~ 63         
  * @retval None
  */
void RTC_TrigSourceConfig(uint32_t TrigSource, uint32_t Period)
{
  uint32_t tmp;
  
  /* Parameter check */
  assert_parameters(IS_RTC_AUTOCAL_ATCS(TrigSource));
  assert_parameters(IS_RTC_AUTOCAL_PERIOD(Period));

  tmp = RTC->ACCTRL;
  tmp &= ~(RTC_ACCTRL_ACPER | RTC_ACCTRL_ACCLK);
  tmp |= (TrigSource | (Period << RTC_ACCTRL_ACPER_Pos));

  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  /* Disable RTC Registers write-protection */
  RTC_WriteProtection(DISABLE);
  RTC->ACCTRL = tmp;
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();  
}

/**
  * @brief  Start RTC Auto-calibration manually.
  * @param  None
  * @retval None
  */
void RTC_StartAutoCalManual(void)
{
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();
  /* Disable RTC Registers write-protection */
  RTC_WriteProtection(DISABLE);
  
  /* manual trigger Auto-calibration */
  RTC->ACCTRL |= RTC_ACCTRL_MANU; 
  
  /* Enable RTC Registers write-protection */
  RTC_WriteProtection(ENABLE);
  /* Wait until the RTC registers be synchronized */
  RTC_WaitForSynchro();  
}

/**
  * @brief  Wait until Auto-calibration manual is done.
  * @param  None
  * @retval None
  */
void RTC_WaitForAutoCalManual(void)
{
#ifdef DEBUG_HANG_LOOPING
    GPIO_InitType GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUTPUT_CMOS;
  GPIO_InitStruct.GPIO_Pin = ( GPIO_Pin_1);
  GPIOBToF_Init(GPIOE, &GPIO_InitStruct);
  GPIOBToF_ResetBits(GPIOE, GPIO_Pin_1);
#endif
  while (RTC->ACCTRL&RTC_ACCTRL_MANU)
  {
#ifdef DEBUG_HANG_LOOPING
      uart_debug_byte ('-',0);
#endif
  }
  #ifdef DEBUG_HANG_LOOPING
  GPIOBToF_SetBits(GPIOE, GPIO_Pin_1);
#endif
}

#endif  /*  MicroController  */
/*********************************** END OF FILE ******************************/
