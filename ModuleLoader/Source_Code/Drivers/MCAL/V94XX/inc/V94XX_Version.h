/**
*******************************************************************************
* @file    V94XX_Version.h
* @author  Application Team
* @version V1.5.0
* @date    2022-09-20
* @brief   Version library.
*******************************************************************************/

#ifndef __V94XX_VERSION_H
#define __V94XX_VERSION_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "V94XX.h"
   
#define DRIVER_VERSION(major,minor) (((major) << 8) | (minor))

/* Exported Functions ------------------------------------------------------- */
   
/**
  * @brief  Get Target driver's current version.
  * @param  None
  * @retval Version value
  *           Bit[15:8] : Major version
  *           Bit[7:0]  : Minor version 
  */
uint16_t Target_GetDriveVersion(void);

#ifdef __cplusplus
}
#endif

#endif /* __V94XX_VERSION_H */

/*********************************** END OF FILE ******************************/
