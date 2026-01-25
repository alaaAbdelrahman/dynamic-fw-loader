/**
  ******************************************************************************
  * @file    V94XX_CodeRAM.h 
  * @author  Application Team
  * @version V1.5.0
  * @date    2022-09-20
  * @brief   Codes executed in SRAM.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#ifndef __V94XX_CODERAM_H
#define __V94XX_CODERAM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "V94XX.h"   

#ifndef __GNUC__

#ifdef __ICCARM__          /* EWARM */
  #define __RAM_FUNC       __ramfunc
#endif

#ifdef __CC_ARM            /* MDK-ARM */
  #define __RAM_FUNC       __attribute__((used))
#endif

/* Exported Functions ------------------------------------------------------- */

__RAM_FUNC void PMU_EnterIdle_FlashDSTB(void);

#endif /* __GNUC__ */

#ifdef __cplusplus
}
#endif

#endif /* __V94XX_CODERAM_H */

/*********************************** END OF FILE ******************************/
