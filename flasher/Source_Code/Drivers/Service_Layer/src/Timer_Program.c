#include "Timer_Interface.h"

volatile uint32_t timer1_tick_ms = 0;
/**
  * @brief  TMR initialization.
  * @param  TMRx: 
                TMR0 ~ TMR3
                EXTGT:
                    S_TMR_EXTGT_DISABLE
                    TMR_EXTGT_ENABLE
                interval_us: overflow interval, the peroid in micro seconds
  * @retval None
  */
void S_Timer_Internal_INIT (TMR_TypeDef *TMRx, uint32_t EXTGT, uint32_t interval_us)
{
  TMR_InitType TMR_InitStruct;
  uint32_t OneSecond_nano = 1000000;
  /* Timer initialization:
       - Clock source: internal clock(APB clock 13107200Hz)  */
  S_TMR_DeInit(TMRx);
  TMR_InitStruct.ClockSource = S_TMR_CLKSRC_INTERNAL;
  TMR_InitStruct.EXTGT = EXTGT;
  if (interval_us == 0)
    interval_us = 1;//noto divide by zero, make it 1 , but note here is wrong argument
  OneSecond_nano = OneSecond_nano/interval_us;
  TMR_InitStruct.Period = S_CLK_GetPCLKFreq()/OneSecond_nano - 1;
  S_TMR_Init(TMRx, &TMR_InitStruct);
}

void S_Timer1_CONFIG(uint32_t interval_us)
{
  S_Timer_Internal_INIT(S_TMR1 , S_TMR_EXTGT_DISABLE, interval_us); 
  S_ENABLE_TIMER_1_INT; 
  S_ENABLE_TIMER(S_TMR1);
}

void S_Timer2_CONFIG(uint32_t interval_us)
{
  S_Timer_Internal_INIT(S_TMR2 , S_TMR_EXTGT_DISABLE, interval_us); 
{S_TMR_INTConfig(S_TMR2, ENABLE);S_CORTEX_SetPriority_ClearPending_EnableIRQ(TMR2_IRQn, 0);}
S_ENABLE_TIMER(S_TMR2);
}