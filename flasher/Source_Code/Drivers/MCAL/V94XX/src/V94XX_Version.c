#include "config.h"
#if (MicroController == Micro_V94XX)

/**
*******************************************************************************
  * @file    lib_version.c
  * @author  Application Team
  * @version V2.0.7
  * @date    2022-09-20
  * @brief   Version library.
*******************************************************************************/
#include "V94XX_Version.h"

#define Target_DriveVersion  DRIVER_VERSION(2, 0)

/**
  * @brief  Get Target driver's current version.
  * @param  None
  * @retval Version value
  *           Bit[15:8] : Major version
  *           Bit[7:0]  : Minor version 
  */
uint16_t Target_GetDriveVersion(void)
{
  return (Target_DriveVersion);
}

#endif  /* MicroController */
/*********************************** END OF FILE ******************************/
