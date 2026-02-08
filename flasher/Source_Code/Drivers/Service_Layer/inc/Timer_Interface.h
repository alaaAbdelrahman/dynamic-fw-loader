
#ifndef TMR_DRV_H_
#define TMR_DRV_H_


#include "config.h"
#if (MicroController == Micro_V94XX)
#include "V94XX_TMR.h"
#include "V94XX_CLK.h"
#include "V94XX_CORTEX.h"

#else
#include "v85xx_tmr.h"
#include "v85xx_clk.h"
#include "v85xx_cortex.h"
#endif
#include "Timer_Config.h"

/**
  * @brief  TMR initialization.
  * @param  TMRx: 
                TMR0 ~ TMR3
                EXTGT:
                    TMR_EXTGT_DISABLE
                    TMR_EXTGT_ENABLE
                interval_ns: overflow interval, the peroid in nano seconds
  * @retval None
  */
void S_Timer_Internal_INIT (TMR_TypeDef *TMRx, uint32_t EXTGT, uint32_t interval_ns);
void S_Timer1_CONFIG(uint32_t interval_ns);
void S_Timer2_CONFIG(uint32_t interval_ns);
#endif
