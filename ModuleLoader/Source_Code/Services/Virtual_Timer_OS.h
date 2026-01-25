
#ifndef VIRTUAL_TIMER_H_  /* To eliminate including the file more than one time */
#define VIRTUAL_TIMER_H_

/********************************** Headers ***********************************/
#include "config.h"      /* Header file contains the standard types used */

/********************************** Macros ***********************************/

typedef enum {
  FLASH_VIRTUAL_TIMER,
  UART5_VIRTUAL_TIMER,
  MAX_VIRTUAL_TIMERS_NUM,
}Virtual_tmr_types;

/********************************** Datatypes ***********************************/
typedef void(*Ptr2Func_type)() ;

typedef struct 
{       
  uint32_t TimeOut_Value;
  uint32_t Counter;
  Ptr2Func_type Callback_Func;
  uint8_t is_Enabled;
  uint8_t is_TimeOut;
}Virtual_Tmr_Info_str;

/********************************** functions ***********************************/
void Virtual_Timer_Init();

uint8_t Virtual_Timer_Create (Virtual_tmr_types Timer_Name , uint32_t threshold_Value , Ptr2Func_type Callback_func);

uint8_t Virtual_Timer_Start (Virtual_tmr_types Timer_Name);

uint8_t Virtual_Timer_Stop  (Virtual_tmr_types Timer_Name);

uint8_t Virtual_Timer_is_timeOut (Virtual_tmr_types Timer_Name);

uint8_t Virtual_Timer_is_Enabled (Virtual_tmr_types Timer_Name);

void Virtual_Timer_task();

#endif  /*end of if VIRTUAL_TIMER_H_ defined */

/*******************************************************************************
                             End of file
*******************************************************************************/
