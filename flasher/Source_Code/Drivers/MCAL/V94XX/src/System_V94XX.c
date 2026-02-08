#include "config.h"
#if (MicroController == Micro_V94XX)

/**
  ******************************************************************************
  * @file    system_target.h
  * @author  Application Team
  * @version V1.5.0
  * @date    2022-09-20
  * @brief   system source file.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "V94XX.h"
#include "V94XX_RTC.h"
/**
  * @brief  Setup the microcontroller system
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
#if 0
  uint32_t i, Count, Value, Address, Checksum;
  
  Count = *(__IO uint32_t *)NVR_REGINFOCOUNT1;
  Checksum = Count;
  Checksum = ~Checksum;
  if(Checksum != *(__IO uint32_t *)(NVR_REGINFOCOUNT1 + 4))
  {
    Count = *(__IO uint32_t *)(NVR_REGINFOCOUNT1 + NVR_REGINFOBAKOFFSET);
    Checksum = Count;
    Checksum = ~Checksum;
    if(Checksum != *(__IO uint32_t *)(NVR_REGINFOCOUNT1 + NVR_REGINFOBAKOFFSET + 4))
    {
      while(1);
    }
  }
  
  for(i=0; i<Count; i++)
  {
    Address = *(__IO uint32_t *)(NVR_REGINFOCOUNT1 + 8+ i*12);
    Value = *(__IO uint32_t *)(NVR_REGINFOCOUNT1 + 12 + i*12);
    Checksum = Address + Value;
    Checksum = ~Checksum;
    if(Checksum != *(__IO uint32_t *)(NVR_REGINFOCOUNT1 + 16 + i*12))
    {
      Address = *(__IO uint32_t *)(NVR_REGINFOCOUNT1 + NVR_REGINFOBAKOFFSET + 8 + i*12);
      Value = *(__IO uint32_t *)(NVR_REGINFOCOUNT1 + NVR_REGINFOBAKOFFSET + 12 + i*12);
      Checksum = Address + Value;
      Checksum = ~Checksum;
      if(Checksum != *(__IO uint32_t *)(NVR_REGINFOCOUNT1 + NVR_REGINFOBAKOFFSET + 16 + i*12))
      {
        while(1);
      }
    }
    if((Address>=0x40014800) && (Address<=0x40015000))
    {
      RTC_WriteRegisters(Address, &Value, 1);
    }
    else
    {
      *(__IO uint32_t *)(Address) = Value;
    }
  }
  
  ANA->REG8 |= BIT7;
#endif
}

/**
  * @brief  Initializes registers.
  * @param  None
  * @retval None
  */
void SystemUpdate(void)
{
 
}

#endif /* MicroController */
/*********************************** END OF FILE ******************************/
