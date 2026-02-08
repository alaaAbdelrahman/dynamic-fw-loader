#include "Virtual_timer_OS.h"
#include <string.h>
#include "Timer_Interface.h"

#include "v85xx_cortex.h"

Virtual_Tmr_Info_str Virt_Tmr_Info_Array[MAX_VIRTUAL_TIMERS_NUM];


/********************************** functions ***********************************/
void Virtual_Timer_Init()
{
  memset (Virt_Tmr_Info_Array , 0 , sizeof(Virt_Tmr_Info_Array) );
  S_Timer_Internal_INIT (TMR3, TMR_EXTGT_DISABLE, 1000);
  TMR_INTConfig(TMR3, ENABLE);
  CORTEX_SetPriority_ClearPending_EnableIRQ(TMR3_IRQn, 0);
  S_ENABLE_TIMER(TMR3);
}

uint8_t Virtual_Timer_Create (Virtual_tmr_types Timer_Name , uint32_t threshold_Value , Ptr2Func_type Callback_func)
{
  if (Timer_Name >= MAX_VIRTUAL_TIMERS_NUM )
    return false;
  
  Virt_Tmr_Info_Array[Timer_Name].is_Enabled = false;
  Virt_Tmr_Info_Array[Timer_Name].TimeOut_Value = threshold_Value;
  Virt_Tmr_Info_Array[Timer_Name].Counter = 0;
  Virt_Tmr_Info_Array[Timer_Name].is_TimeOut = false;
  Virt_Tmr_Info_Array[Timer_Name].Callback_Func = Callback_func;
  return true;
}

uint8_t Virtual_Timer_Start (Virtual_tmr_types Timer_Name)
{
  if (Timer_Name >= MAX_VIRTUAL_TIMERS_NUM) 
    return false;
  
  Virt_Tmr_Info_Array[Timer_Name].is_Enabled = true;
  return true;
}

uint8_t Virtual_Timer_Stop  (Virtual_tmr_types Timer_Name)
{
  if ( Timer_Name >= MAX_VIRTUAL_TIMERS_NUM )
    return false;
  
  Virt_Tmr_Info_Array[Timer_Name].is_Enabled = false;
  return true;
}

uint8_t Virtual_Timer_is_timeOut (Virtual_tmr_types Timer_Name)
{
  if ( Timer_Name >= MAX_VIRTUAL_TIMERS_NUM )
    return false;
  
  if (Virt_Tmr_Info_Array[Timer_Name].is_TimeOut == true)
    return true;
  else
    return false;
}

uint8_t Virtual_Timer_is_Enabled (Virtual_tmr_types Timer_Name)
{
  if ( Timer_Name >= MAX_VIRTUAL_TIMERS_NUM )
    return false;
  
  return Virt_Tmr_Info_Array[Timer_Name].is_Enabled;
}
void Virtual_Timer_task()
{
  uint8_t Timer_Name = 0 ;
  
  for( Timer_Name = 0 ; Timer_Name < MAX_VIRTUAL_TIMERS_NUM ; Timer_Name ++ )
  {
    if (Virt_Tmr_Info_Array[Timer_Name].is_Enabled == true)
    {
      if (Virt_Tmr_Info_Array[Timer_Name].Counter >= Virt_Tmr_Info_Array[Timer_Name].TimeOut_Value)
      {
        Virt_Tmr_Info_Array[Timer_Name].is_TimeOut = true ;
        Virt_Tmr_Info_Array[Timer_Name].is_Enabled = false;
        if (Virt_Tmr_Info_Array[Timer_Name].Callback_Func != 0)
          Virt_Tmr_Info_Array[Timer_Name].Callback_Func ();
      }
      else
        Virt_Tmr_Info_Array[Timer_Name].Counter++;
    }
  }
}