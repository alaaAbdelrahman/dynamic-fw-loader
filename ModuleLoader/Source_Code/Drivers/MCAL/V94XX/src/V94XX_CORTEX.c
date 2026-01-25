
#include "config.h"
#if (MicroController == Micro_V94XX)

/**
  ******************************************************************************
  * @file    lib_cortex.c 
  * @author  Application Team
  * @version V1.5.0
  * @date    2022-09-20
  * @brief   Cortex module driver.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "V94XX_CORTEX.h"
#include "core_cm0.h"
#include "V94XX.h"
#include "V94XX_GPIO.h"
/**
  * @brief  1. Clears Pending of a device specific External Interrupt.
  *         2. Sets Priority of a device specific External Interrupt.
  *         3. Enables a device specific External Interrupt. 
  * @param  IRQn: External interrupt number .
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete target Devices IRQ Channels list, please refer to target.h file)
  * @param  Priority: The preemption priority for the IRQn channel.
  *         This parameter can be a value between 0 and 3.
  *         A lower priority value indicates a higher priority
  * @retval None
  */
void CORTEX_SetPriority_ClearPending_EnableIRQ(IRQn_Type IRQn, uint32_t Priority)
{
  /* Check parameters */
  assert_parameters(IS_CORTEX_NVIC_DEVICE_IRQ(IRQn));
  assert_parameters(IS_CORTEX_NVIC_PREEMPTION_PRIORITY(Priority));

  /* Clear Pending Interrupt */
  NVIC_ClearPendingIRQ(IRQn);
  /* Set Interrupt Priority */
  NVIC_SetPriority(IRQn, Priority);
  /* Enable Interrupt in NVIC */
  NVIC_EnableIRQ(IRQn);
}

/**
  * @brief  Enables a device specific interrupt in the NVIC interrupt controller.
  * @note   To configure interrupts priority correctly before calling it.
  * @param  IRQn External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete target Devices IRQ Channels list, please refer to the appropriate CMSIS device file (target.h))
  * @retval None
  */
void CORTEX_NVIC_EnableIRQ(IRQn_Type IRQn)
{
  /* Check parameters */
  assert_parameters(IS_CORTEX_NVIC_DEVICE_IRQ(IRQn));
  /* Enable interrupt in NVIC */
  NVIC_EnableIRQ(IRQn);
}

/**
  * @brief  Disables a device specific interrupt in the NVIC interrupt controller.
  * @param  IRQn External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete target Devices IRQ Channels list, please refer to the appropriate CMSIS device file (target.h))
  * @retval None
  */
void CORTEX_NVIC_DisableIRQ(IRQn_Type IRQn)
{
  /* Check parameters */
  assert_parameters(IS_CORTEX_NVIC_DEVICE_IRQ(IRQn));
  /* Disable interrupt in NVIC */
  NVIC_DisableIRQ(IRQn); 
}

/**
  * @brief  Initiates a system reset request to reset the MCU.
  * @retval None
  */
void CORTEX_NVIC_SystemReset(uint8_t x)
{
  #if 0// RESET DEBUG
   GPIO_InitType GPIO_InitStruct;
   GPIO_InitStruct.GPIO_Mode = 0x2U;  //output pin
   GPIO_InitStruct.GPIO_Pin = 0x0001;
   GPIOBToF_Init(GPIOF, &GPIO_InitStruct);
   uint8_t ii;
  /* System Reset */
    for (ii=0;ii<x;ii++)
    {
      GPIOBToF_ResetBits(GPIOF, ((uint16_t)0x0001));
      Delay_ms(1000);
      GPIOBToF_SetBits(GPIOF,((uint16_t)0x0001));
      Delay_ms(1000);
    }
#endif
  /* System Reset */

  NVIC_SystemReset();  
}

/**
  * @brief  Gets the Pending bit of an interrupt.
  * @param  IRQn: External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete target Devices IRQ Channels list, please refer to the appropriate CMSIS device file (target.h))
  * @retval 0  Interrupt status is not pending.
            1  Interrupt status is pending.
  */
uint32_t CORTEX_NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
  /* Check parameters */
  assert_parameters(IS_CORTEX_NVIC_DEVICE_IRQ(IRQn));
  /* Get priority for Cortex-M0 system or device specific interrupts */
  return NVIC_GetPendingIRQ(IRQn);  
}

/**
  * @brief  Sets Pending bit of an external interrupt.
  * @param  IRQn External interrupt number
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete target Devices IRQ Channels list, please refer to the appropriate CMSIS device file (target.h))
  * @retval None
  */
void CORTEX_NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
  /* Check parameters */
  assert_parameters(IS_CORTEX_NVIC_DEVICE_IRQ(IRQn));
  /* Set interrupt pending */
  NVIC_SetPendingIRQ(IRQn);
}

/**
  * @brief  Clears the pending bit of an external interrupt.
  * @param  IRQn External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete target Devices IRQ Channels list, please refer to the appropriate CMSIS device file (target.h))
  * @retval None
  */
void CORTEX_NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  /* Check parameters */
  assert_parameters(IS_CORTEX_NVIC_DEVICE_IRQ(IRQn));
  /* Clear interrupt pending */
  NVIC_ClearPendingIRQ(IRQn);  
}

/**
  * @brief  Gets the priority of an interrupt.
  * @param  IRQn: External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete target Devices IRQ Channels list, please refer to the appropriate CMSIS device file (target.h))
  * @retval Interrupt Priority. Value is aligned automatically to the implemented
  *         priority bits of the microcontroller.
  */
uint32_t CORTEX_NVIC_GetPriority(IRQn_Type IRQn)
{
  /* Get priority for Cortex-M0 system or device specific interrupts */
  return NVIC_GetPriority(IRQn);
}

/**
  * @brief  Sets the priority of an interrupt.
  * @param  IRQn: External interrupt number .
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete target Devices IRQ Channels list, please refer to target.h file)
  * @param  Priority: The preemption priority for the IRQn channel.
  *         This parameter can be a value between 0 and 3.
  *         A lower priority value indicates a higher priority  
  * @retval None
  */
void CORTEX_NVIC_SetPriority(IRQn_Type IRQn, uint32_t Priority)
{
  /* Check parameters */
  assert_parameters(IS_CORTEX_NVIC_PREEMPTION_PRIORITY(Priority));
  /* Get priority for Cortex-M0 system or device specific interrupts */
  NVIC_SetPriority(IRQn, Priority);
}

/**
  * @brief  Initializes the System Timer and its interrupt, and starts the System Tick Timer.
  *         Counter is in free running mode to generate periodic interrupts.
  * @param  TicksNumb: Specifies the ticks Number of ticks between two interrupts.
  * @retval status:  - 0  Function succeeded.
  *                  - 1  Function failed.
  */
uint32_t CORTEX_SystemTick_Config(uint32_t TicksNum)
{
  return SysTick_Config(TicksNum);
}

#endif /*  MicroController  */
/*********************************** END OF FILE ******************************/
