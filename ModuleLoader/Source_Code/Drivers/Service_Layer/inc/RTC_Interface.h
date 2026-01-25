
#ifndef RTC_DRV_H_
#define RTC_DRV_H_

#include "config.h"

#if (MicroController == Micro_V94XX)
#include "V94XX_RTC.h"
#include "V94XX_CLK.h"
#include "V94XX_CORTEX.h"
#include "V94XX_PMU.h"

#else
#include "v85xx_rtc.h"
#include "v85xx_clk.h"
#include "v85xx_cortex.h"
#include "v85xx_pmu.h"

#endif
#include "basic_srv.h"
#include "RTC_Config.h"




void S_RTC_C_getCalendarTime(RTC_TimeTypeDef *gTime);
void S_RTC_C_Update(RTC_TimeTypeDef *sTime);
void S_RTC_C_UpdateTime(rtc_t_t *sTime);
void S_RTC_C_UpdateDate(rtc_d_t *sDate);

void S_RTC_C_SetRTCSEC (uint8_t uisec);
void S_RTC_C_SetRTCMIN (uint8_t uimin);
void S_RTC_C_SetRTCHOUR (uint8_t uihour);
void S_RTC_C_SetRTCDAY (uint8_t uiday);
void S_RTC_C_SetRTCDOW (uint8_t uidow);
void S_RTC_C_SetRTCMON (uint8_t uimon);
void S_RTC_C_SetRTCYEAR (uint8_t uiyear);
uint8_t S_RTC_C_GetRTCYEAR();
uint8_t S_RTC_C_GetRTCMON();
uint8_t S_RTC_C_GetRTCDAY();
uint8_t S_RTC_C_GetRTCDOW();
uint8_t S_RTC_C_GetRTCHOUR();
uint8_t S_RTC_C_GetRTCMIN();
uint8_t S_RTC_C_GetRTCSEC();
void S_RTC_init(void);




#endif
