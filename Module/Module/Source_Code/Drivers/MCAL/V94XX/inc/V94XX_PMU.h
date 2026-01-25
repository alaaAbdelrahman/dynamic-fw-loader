/**
  ******************************************************************************
  * @file    V94XX_PMU.h 
  * @author  Application Team
  * @version V1.5.0
  * @date    2022-09-20
  * @brief   PMU library.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef __V94XX_PMU_H
#define __V94XX_PMU_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "V94XX.h"

/**
  * Deep-sleep low-power configuration
*/
typedef struct
{
  uint32_t COMP1Power;           /* Comparator 1 power control */
  uint32_t COMP2Power;           /* Comparator 2 power control */
  uint32_t TADCPower;            /* Tiny ADC power control */
  uint32_t BGPPower;          /* ADCBGP power control */
  uint32_t LCDPower;             /* LCD controller power control */
  uint32_t AVCCPower;        /* VDCIN detector control */
  uint32_t VDDDetector;          /* VDD detector control */
  uint32_t AHBPeriphralDisable;  /* AHB Periphral clock disable selection */
  uint32_t APBPeriphralDisable;  /* APB Periphral clock disable selection */
} PMU_LowPWRTypeDef; 


/* COMP1Power */
#define PMU_COMP1PWR_ON         (ANA_REG3_CMP1PDN)
#define PMU_COMP1PWR_OFF        (0)
#define IS_PMU_COMP1PWR(__COMP1PWR__)  (((__COMP1PWR__) == PMU_COMP1PWR_ON) ||\
                                        ((__COMP1PWR__) == PMU_COMP1PWR_OFF))
/* COMP2Power */
#define PMU_COMP2PWR_ON         (ANA_REG3_CMP2PDN)
#define PMU_COMP2PWR_OFF        (0)
#define IS_PMU_COMP2PWR(__COMP2PWR__)  (((__COMP2PWR__) == PMU_COMP2PWR_ON) ||\
                                        ((__COMP2PWR__) == PMU_COMP2PWR_OFF))
/* TADCPower */
#define PMU_TADCPWR_ON          (ANA_REGF_PDNADT)
#define PMU_TADCPWR_OFF         (0)
#define IS_PMU_TADCPWR(__TADCPWR__)  (((__TADCPWR__) == PMU_TADCPWR_ON) ||\
                                      ((__TADCPWR__) == PMU_TADCPWR_OFF))
/* ADCBGPPower */
#define PMU_ADCBGPPWR_ON           (0)
#define PMU_ADCBGPPWR_OFF          (ANA_REG3_ADCBGPPD)
#define IS_PMU_ADCBGPPWR(__ADCBGPPWR__)  (((__ADCBGPPWR__) == PMU_ADCBGPPWR_ON) ||\

/*ADDCode_K*/  
/* BGPPower */
#define PMU_BGPPWR_ON           (0)
#define PMU_BGPPWR_OFF          (ANA_REG3_BGPPD)
#define IS_PMU_BGPPWR(__BGPPWR__)  (((__BGPPWR__) == PMU_BGPPWR_ON) ||\

/*ADDCode_K*/  
/* AVCCPower */
#define PMU_AVCCPWR_ON         (0)
#define PMU_AVCCPWR_OFF        (ANA_REG8_PD_AVCCLDO)
#define IS_PMU_AVCCPWR(__AVCCPWR__)  (((__AVCCPWR__) == PMU_AVCCPWR_ON) ||\
                                        ((__AVCCPWR__) == PMU_AVCCPWR_OFF))                                   ((__BGPPWR__) == PMU_BGPPWR_OFF))                                    ((__ADCBGPPWR__) == PMU_ADCBGPPWR_OFF))
/* LCDPower */
#define PMU_LCDPWER_ON          (LCD_CTRL_EN)
#define PMU_LCDPWER_OFF         (0)
#define IS_PMU_LCDPWER(__LCDPWER__)  (((__LCDPWER__) == PMU_LCDPWER_ON) ||\
                                      ((__LCDPWER__) == PMU_LCDPWER_OFF))
/* VDCINDetector */
#define PMU_VDCINDET_ENABLE     (0)
#define PMU_VDCINDET_DISABLE    (ANA_REGA_PD_VDCINDET)
#define IS_PMU_VDCINDET(__VDCINDET__)  (((__VDCINDET__) == PMU_VDCINDET_ENABLE) ||\
                                        ((__VDCINDET__) == PMU_VDCINDET_DISABLE)) 

/* VDDDetector */
#define PMU_VDDDET_ENABLE       (0)
#define PMU_VDDDET_DISABLE      (ANA_REG9_PDDET)
#define IS_PMU_VDDDET(__VDDDET__)  (((__VDDDET__) == PMU_VDDDET_ENABLE) ||\
                                    ((__VDDDET__) == PMU_VDDDET_DISABLE))

/* APBPeriphralDisable */
#define PMU_APB_ALL       (MISC2_PCLKEN_DMA     \
                          |MISC2_PCLKEN_I2C     \
                          |MISC2_PCLKEN_UART0   \
                          |MISC2_PCLKEN_UART1   \
                          |MISC2_PCLKEN_UART2   \
                          |MISC2_PCLKEN_UART4   \
                          |MISC2_PCLKEN_UART5   \
                          |MISC2_PCLKEN_ISO78160\
                          |MISC2_PCLKEN_ISO78161\
                          |MISC2_PCLKEN_TIMER   \
                          |MISC2_PCLKEN_MISC    \
                          |MISC2_PCLKEN_U32K0   \
                          |MISC2_PCLKEN_U32K1   \
                          |MISC2_PCLKEN_SPI)   
#define PMU_APB_DMA        MISC2_PCLKEN_DMA       
#define PMU_APB_I2C        MISC2_PCLKEN_I2C        
#define PMU_APB_UART0      MISC2_PCLKEN_UART0     
#define PMU_APB_UART1      MISC2_PCLKEN_UART1     
#define PMU_APB_UART2      MISC2_PCLKEN_UART2       
#define PMU_APB_UART4      MISC2_PCLKEN_UART4     
#define PMU_APB_UART5      MISC2_PCLKEN_UART5     
#define PMU_APB_ISO78160   MISC2_PCLKEN_ISO78160     
#define PMU_APB_ISO78161   MISC2_PCLKEN_ISO78161       
#define PMU_APB_TIMER      MISC2_PCLKEN_TIMER     
#define PMU_APB_MISC       MISC2_PCLKEN_MISC                         
#define PMU_APB_U32K0      MISC2_PCLKEN_U32K0     
#define PMU_APB_U32K1      MISC2_PCLKEN_U32K1 
#define PMU_APB_SPI        MISC2_PCLKEN_SPI
/* AHBPeriphralDisable */
#define PMU_AHB_ALL       (MISC2_HCLKEN_DMA     \
                          |MISC2_HCLKEN_GPIO    \
                          |MISC2_HCLKEN_LCD     \
                          |MISC2_HCLKEN_CRYPT)
#define PMU_AHB_DMA        MISC2_HCLKEN_DMA
#define PMU_AHB_GPIO       MISC2_HCLKEN_GPIO
#define PMU_AHB_LCD        MISC2_HCLKEN_LCD
#define PMU_AHB_CRYPT      MISC2_HCLKEN_CRYPT   
   
//PMU interrupt
#define PMU_INT_IOAEN   PMU_CONTROL_INT_IOA_EN
#define PMU_INT_32K     PMU_CONTROL_INT_32K_EN
#define PMU_INT_6M      PMU_CONTROL_INT_6M_EN
#define PMU_INT_Msk     (PMU_INT_IOAEN  \
                         |PMU_INT_32K \
                         |PMU_INT_6M)
#define IS_PMU_INT(__INT__)  ((((__INT__)&PMU_INT_Msk) != 0U) &&\
                              (((__INT__)&(~PMU_INT_Msk)) == 0U))
   
//INTStatus
#define PMU_INTSTS_32K      PMU_STS_INT_32K
#define PMU_INTSTS_6M       PMU_STS_INT_6M
#define PMU_INTSTS_EXTRST   PMU_STS_EXTRST
#define PMU_INTSTS_PORST    PMU_STS_PORST
#define PMU_INTSTS_DPORST   PMU_STS_DPORST
#define PMU_INTSTS_Msk      (PMU_INTSTS_32K    \
                            |PMU_INTSTS_6M     \
                            |PMU_INTSTS_EXTRST \
                            |PMU_INTSTS_PORST  \
                            |PMU_INTSTS_DPORST)
#define IS_PMU_INTFLAGR(__INTFLAG__)  (((__INTFLAG__) == PMU_INTSTS_32K)   ||\
                                      ((__INTFLAG__) == PMU_INTSTS_6M)     ||\
                                      ((__INTFLAG__) == PMU_INTSTS_EXTRST) ||\
                                      ((__INTFLAG__) == PMU_INTSTS_PORST)  ||\
                                      ((__INTFLAG__) == PMU_INTSTS_DPORST))

#define IS_PMU_INTFLAGC(__INTFLAG__)  ((((__INTFLAG__)&PMU_INTSTS_Msk) != 0U) &&\
                                       (((__INTFLAG__)&(~PMU_INTSTS_Msk)) == 0U))
  


//Status
#define PMU_STS_32K     PMU_STS_EXIST_32K
#define PMU_STS_6M      PMU_STS_EXIST_6M
#define IS_PMU_FLAG(__FLAG__)  (((__FLAG__) == PMU_STS_32K) || ((__FLAG__) == PMU_STS_6M))

//Wakeup_Event
#define IOA_DISABLE     (0)
#define IOA_RISING      (1)
#define IOA_FALLING     (2)
#define IOA_HIGH        (3)
#define IOA_LOW         (4)
#define IOA_EDGEBOTH    (5)
#define IS_PMU_WAKEUP(__WAKEUP__)  (((__WAKEUP__) == IOA_DISABLE)     ||\
                                    ((__WAKEUP__) == IOA_RISING)  ||\
                                    ((__WAKEUP__) == IOA_FALLING) ||\
                                    ((__WAKEUP__) == IOA_HIGH)    ||\
                                    ((__WAKEUP__) == IOA_LOW)     ||\
                                    ((__WAKEUP__) == IOA_EDGEBOTH)) 

/***** Wakeup_Event (PMU_SleepWKUSRC_Config_RTC) *****/
#define PMU_RTCEVT_WKUCNT     RTC_INTSTS_INTSTS6
#define PMU_RTCEVT_MIDNIGHT   RTC_INTSTS_INTSTS5
#define PMU_RTCEVT_WKUHOUR    RTC_INTSTS_INTSTS4
#define PMU_RTCEVT_WKUMIN     RTC_INTSTS_INTSTS3
#define PMU_RTCEVT_WKUSEC     RTC_INTSTS_INTSTS2
#define PMU_RTCEVT_TIMEILLE   RTC_INTSTS_INTSTS1
#define PMU_RTCEVT_Msk         (PMU_RTCEVT_WKUCNT   \
                               |PMU_RTCEVT_MIDNIGHT \
                               |PMU_RTCEVT_WKUHOUR  \
                               |PMU_RTCEVT_WKUMIN   \
                               |PMU_RTCEVT_WKUSEC   \
                               |PMU_RTCEVT_TIMEILLE)
#define IS_PMU_RTCEVT(__RTCEVT__)  ((((__RTCEVT__)&PMU_RTCEVT_Msk) != 0U) &&\
                                    (((__RTCEVT__)&(~PMU_RTCEVT_Msk)) == 0U))


/***** BATDisc (PMU_BATDischargeConfig) *****/
#define PMU_BAT2_DISC   ANA_REG6_BAT2DISC
#define IS_PMU_BAT2DISC(__BAT2DISC__)  ((__BAT2DISC__) == PMU_BAT2_DISC)

/***** PowerThreshold (PMU_PowerAlarmTHConfig) *****/
#define PMU_PWTH_4_5      ANA_REG8_VDDPVDSEL_0
#define PMU_PWTH_4_2      ANA_REG8_VDDPVDSEL_1
#define PMU_PWTH_3_9      ANA_REG8_VDDPVDSEL_2
#define PMU_PWTH_3_6      ANA_REG8_VDDPVDSEL_3
#define PMU_PWTH_3_2      ANA_REG8_VDDPVDSEL_4
#define PMU_PWTH_2_9      ANA_REG8_VDDPVDSEL_5
#define PMU_PWTH_2_6      ANA_REG8_VDDPVDSEL_6
#define PMU_PWTH_2_3      ANA_REG8_VDDPVDSEL_7

#define IS_PMU_PWTH(__PWTH__)  (((__PWTH__) == PMU_PWTH_4_5) ||\
                                ((__PWTH__) == PMU_PWTH_4_2) ||\
                                ((__PWTH__) == PMU_PWTH_3_9) ||\
                                ((__PWTH__) == PMU_PWTH_3_6) ||\
                                ((__PWTH__) == PMU_PWTH_3_2) ||\
                                ((__PWTH__) == PMU_PWTH_2_9) ||\
                                ((__PWTH__) == PMU_PWTH_2_6) ||\
                                ((__PWTH__) == PMU_PWTH_2_3))

/***** SWTLevel *****/
#define PMU_PSWTLEVEL_3_0      (0) 
#define PMU_PSWTLEVEL_3_6       ANA_REGA_PSLSEL 
#define IS_PMU_PSWTLEVEL(__PSWTLEVEL__)  (((__PSWTLEVEL__) == PMU_PSWTLEVEL_3_0)||\
                                          ((__PSWTLEVEL__) == PMU_PSWTLEVEL_3_6)) 

/***** RTCLDOSel (PMU_RTCLDOConfig) *****/
#define PMU_RTCLDO_1_5         (0)
#define PMU_RTCLDO_1_2          ANA_REGA_RTCVSEL

/***** StatusMask (PMU_GetPowerStatus) *****/
#define PMU_PWRSTS_VDDLV         ANA_COMPOUT_AVCCLV
#define PMU_PWRSTS_VDCINDROP       ANA_COMPOUT_VDCINDROP
#define PMU_PWRSTS_VDDALARM       ANA_COMPOUT_VDDALARM

/***** Debounce (PMU_PWRDropDEBConfig) *****/
#define PMU_PWRDROP_DEB_0       ANA_CTRL_PWRDROPDEB_0
#define PMU_PWRDROP_DEB_1       ANA_CTRL_PWRDROPDEB_1
#define PMU_PWRDROP_DEB_2       ANA_CTRL_PWRDROPDEB_2
#define PMU_PWRDROP_DEB_3       ANA_CTRL_PWRDROPDEB_3
#define IS_PMU_PWRDROP_DEB(__DEB__)  (((__DEB__) == PMU_PWRDROP_DEB_0) ||\
                                      ((__DEB__) == PMU_PWRDROP_DEB_1) ||\
                                      ((__DEB__) == PMU_PWRDROP_DEB_2) ||\
                                      ((__DEB__) == PMU_PWRDROP_DEB_3))

/***** PWRState (PMU_PowerConfig) *****/
#define PMU_PWRSTA_AUTOVDD_BAT1  (0)
#define PMU_PWRSTA_SWT2BAT1  (1)
#define PMU_PWRSTA_SWT2VDD   (2)
#define IS_PMU_PWRSTA(__PWRSTA__)  (((__PWRSTA__) == PMU_PWRSTA_AUTOVDD_BAT1) ||\
                                    ((__PWRSTA__) == PMU_PWRSTA_SWT2BAT1) ||\
                                    ((__PWRSTA__) == PMU_PWRSTA_SWT2VDD))
/***** RSTSource (PMU_GetRSTSource) *****/
#define PMU_RSTSRC_EXTRST   PMU_STS_EXTRST
#define PMU_RSTSRC_PORST    PMU_STS_PORST
#define PMU_RSTSRC_DPORST   PMU_STS_DPORST
//#define PMU_RSTSRC_WDTRST   PMU_WDTSTS_WDTSTS
#define IS_PMU_RSTSRC(__RSTSRC__)  (((__RSTSRC__) == PMU_RSTSRC_EXTRST) ||\
                                    ((__RSTSRC__) == PMU_RSTSRC_PORST)  ||\
                                    ((__RSTSRC__) == PMU_RSTSRC_DPORST) )

/***** PMU_PDNDSleepConfig *****/
//VDCIN_PDNS
#define PMU_VDCINPDNS_0  (0)
#define PMU_VDCINPDNS_1  (ANA_CTRL_PDNS)
#define IS_PMU_VDCINPDNS(__VDCINPDNS__)  (((__VDCINPDNS__) == PMU_VDCINPDNS_0) ||\
                                          ((__VDCINPDNS__) == PMU_VDCINPDNS_1))
//VDD_PDNS
#define PMU_VDDPDNS_0  (0)
#define PMU_VDDPDNS_1  (ANA_CTRL_PDNS2)
#define IS_PMU_VDDPDNS(__VDDPDNS__)  (((__VDDPDNS__) == PMU_VDDPDNS_0) ||\
                                        ((__VDDPDNS__) == PMU_VDDPDNS_1))
#define IOA_MASK        (0xFBFB)
#define IS_GPIOA_PINR(__PINR__)  (((__PINR__) == GPIO_Pin_0) ||\
                                 ((__PINR__) == GPIO_Pin_1) ||\
                                 ((__PINR__) == GPIO_Pin_3) ||\
                                 ((__PINR__) == GPIO_Pin_4) ||\
                                 ((__PINR__) == GPIO_Pin_5) ||\
                                 ((__PINR__) == GPIO_Pin_6) ||\
                                 ((__PINR__) == GPIO_Pin_7) ||\
                                 ((__PINR__) == GPIO_Pin_8) ||\
                                 ((__PINR__) == GPIO_Pin_9) ||\
                                 ((__PINR__) == GPIO_Pin_11) ||\
                                 ((__PINR__) == GPIO_Pin_12) ||\
                                 ((__PINR__) == GPIO_Pin_13) ||\
                                 ((__PINR__) == GPIO_Pin_14) ||\
                                 ((__PINR__) == GPIO_Pin_15))
                                 
/* Exported Functions ------------------------------------------------------- */

uint32_t PMU_EnterDSleepMode(void);
void PMU_EnterIdleMode(void);
uint32_t PMU_EnterSleepMode(void);

void PMU_INTConfig(uint32_t INTMask, uint32_t NewState);
uint8_t PMU_GetINTStatus(uint32_t INTMask);
void PMU_ClearINTStatus(uint32_t INTMask);

uint8_t PMU_GetStatus(uint32_t Mask);
uint16_t PMU_GetIOAAllINTStatus(void);
uint16_t PMU_GetIOAINTStatus(uint16_t INTMask);
void PMU_ClearIOAINTStatus(uint16_t INTMask);

void PMU_WakeUpPinConfig(uint32_t IOAx, uint32_t Wakeup_Event);

uint8_t PMU_EnterDSleep_LowPower(PMU_LowPWRTypeDef *InitStruct);
uint8_t PMU_EnterSleep_LowPower(PMU_LowPWRTypeDef *InitStruct, uint8_t mode);
#ifndef __GNUC__
void PMU_EnterIdle_LowPower(void);
#endif
void PMU_SleepWKUSRC_Config_IOA(uint16_t IOAx, uint32_t Wakeup_Event, uint32_t Priority);
void PMU_SleepWKUSRC_Config_RTC(uint32_t Wakeup_Event, uint32_t Priority);
void PMU_DeepSleepWKUSRC_Config_IOA(uint16_t IOAx, uint32_t Wakeup_Event);
void PMU_DeepSleepWKUSRC_Config_RTC(uint32_t Wakeup_Event);
void PMU_PDNDSleepConfig(uint32_t VDCIN_PDNS, uint32_t VDD_PDNS);

/***** ADCBGP functions *****/
void PMU_ADCBGP_Cmd(uint32_t NewState);

/***** VDD functions *****/
void PMU_VDDAlarmTHConfig(uint32_t PowerThreshold);
uint8_t PMU_GetVDDALARMStatus(void);
void PMU_VDDDetectorCmd(uint32_t NewState);

/***** VDD functions *****/
void PMU_VDDOutput_Cmd(uint32_t NewState);
void PMU_VDDLVDetector_Cmd(uint32_t NewState);
uint8_t PMU_GetVDDLVStatus(void);

/***** VDCIN functions *****/
void PMU_VDCINDetector_Cmd(uint32_t NewState);
uint8_t PMU_GetVDCINDropStatus(void);

/***** BAT functions *****/
void PMU_BATDischargeConfig(uint32_t BATDisc, uint32_t NewState);

/***** Other functions *****/
uint8_t PMU_GetModeStatus(void);
void PMU_MainPowerConfig(uint32_t PWRState);
void PMU_PowerSWTLevelConfig(uint32_t SWTLevel);
uint8_t PMU_GetPowerStatus(uint32_t StatusMask);
uint8_t PMU_GetPWRSRCStatus(void);
void PMU_PWRDropDEBConfig(uint32_t Debounce);
uint8_t PMU_GetRSTSource(uint32_t RSTSource);
 
#ifdef __cplusplus
}
#endif
     
#endif /* __V94XX_PMU_H */

/*********************************** END OF FILE ******************************/
