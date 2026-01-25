
#ifndef CLK_Interface_H_
#define CLK_Interface_H_

#include "config.h"
#if (MicroController == Micro_V94XX)
#include "V94XX_CLK.h"
#include "V94XX_PMU.h"
#else
#include "v85xx_clk.h"
#include "v85xx_pmu.h"
#endif


#include "CLK_Config.h"
/*************************************************************
**************************************************************
**                   Include section                        **
**************************************************************
**************************************************************
*************************************************************/

void S_CLK_Init(unsigned char freq) ;

#endif /* CLK_Interface_H_ */
