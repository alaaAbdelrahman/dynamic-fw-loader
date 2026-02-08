#ifndef _SYS_TICK_INTERFACE_H__
#define _SYS_TICK_INTERFACE_H__

#include "basic_srv.h"
#include "CLK_Interface.h"

#if (MicroController == Micro_V94XX)
#include "V94XX_CORTEX.h"

#else
#include "v85xx_cortex.h"

#endif
#include "SystemTick_Config.h"



/************************************************************************************************************************
*************************************************************************************************************************
                              **                  Functions Declaration                        **
*************************************************************************************************************************
*************************************************************************************************************************
************************************************************************************************************************/
uint8_t S_Start_Systick_Time_Measure(uint16_t time_res, TIME_UNIT unit);
uint8_t S_Stop_Systick_Time_Measure(void);
uint8_t S_Capture_Systick_Time_Point(uint8_t volatile *index);
uint8_t S_Get_Systick_Time_Measure(uint8_t start_index, uint8_t stop_index, SYS_TICK_TIME_STD *res);
uint32_t S_Get_SysTick_Counts(void);
void S_SysTick_ISR_Cb(void);


#endif /* _SYS_TICK_INTERFACE_H__ */
