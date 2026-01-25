#include "config.h"
#if (MicroController == Micro_V94XX)

/**
* @file    v85xx_isr.c
* @author  weaktea
* @version V1.0
* @date    2016/07/05
* @brief   Main Interrupt Service Routines.
******************************************************************************/


#include "meter_init.h"

#include "V94XX_WDT.h"
#include "V94XX_ISR.h"
#include "V94XX_TMR.h"
#include "V94XX_DMA.h"
#include "RTC_Interface.h"
#include "UART_Interface.h"
//#include "Meter_Interface.h"
//#include "Power_modes.h"
#include "V94XX_PMU.h"
#include "task_priority.h"
//#include "iec62056_21_slave.h"
#include "Timer_Interface.h"


#include "V94XX_FLASH.h"  
#include "V94XX_ISO7816.h"
//#include "V9381.h"
#include "SystemTick_Interface.h"
//#include "self_test.h"
#ifdef EM210_BOARD
#include "V9340.h"
#endif
#ifdef PLC_MODEM
//#include "r_typedefs.h"
//#include "iec62056_21_slave.h"
#include "r_typedefs.h"
#include "r_config.h"
#include "r_bsp_api.h"
#include "r_byte_swap.h"
#include "r_memory_api.h"
#include "r_queue_api.h"

#include "r_app_timer.h"

/* g3 part */
#include "r_c3sap_api.h"

/* app part */
#include "r_demo_app.h"
#include "r_demo_app_eap.h"
#include "r_demo_app_thread.h"
#include "r_demo_api.h"

#include "r_ipv6_headers.h"
#include "r_udp_headers.h"
#include "r_demo_tools.h"
#include "r_demo_api.h"
#include "r_demo_common.h"
#include "r_demo_config.h"
#include "r_demo_statistics.h"
#include "r_demo_ib_access.h"
#include "r_demo_app_ver.h"
#include "r_demo_parameters.h"
#include "r_demo_os_wrapper.h"
#include "app_process_thread.h"
extern uint16_t DMA_The_last_Index;
extern void(*TMR0_CallBack)(void);
extern void(*TMR1_CallBack)(void);
extern void(*TMR2_CallBack)(void);
extern void(*TX1_CallBack)(void);
extern void(*RX1_CallBack)(uint8_t);
extern void(*TX3_CallBack)(void);
extern void(*RX3_CallBack)(uint8_t);
extern uint8_t uart_buffer_send_flag;
extern volatile uint8_t tx1;
#endif
#ifdef PLC_MODEM
uint8_t test = 0;
extern uint8_t flag_framend;
extern uint8_t* remaining_addr;
extern uint32_t remaining;
extern uint8_t bsrc_rcv[1536];
extern uint8_t DMA_Buffer[1536];
extern uint16_t DMA_last_index;
#endif
//extern uint16_t DMA_The_last_Index;
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
  //you must open calling it in bootloader code first,as I removed jumping as it isn't used

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('m',10);
#endif
  CORTEX_NVIC_SystemReset(15);
  while(1)
  {}
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('H',10);
#endif
  /* system soft reset */
  CORTEX_NVIC_SystemReset(16);
  while(1)
  {}
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{  //you must open calling it in bootloader code first,as I removed jumping as it isn't used

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte (')',10);
#endif
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{  //you must open calling it in bootloader code first,as I removed jumping as it isn't used

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('b',10);
#endif
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{  //you must open calling it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('~',10);
#endif
}

/**
  * @brief  This function handles PMU interrupt request.
  * @param  None
  * @retval None
  */
void PMU_IRQHandler(void)
{

}
//uint64_t seconds_debug = 0;

/**
  * @brief  This function handles RTC interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
#ifdef DEBUG_LOW_POWER
  uart_debug_byte ('t',0);
#endif
   if (RTC_GetINTStatus(RTC_INTSTS_WKUSEC))
  {
    RTC_ClearINTStatus(RTC_INTSTS_WKUSEC);
  }
  RTC_ClearINTStatus(0xffffffff);
}

/**
  * @brief  This function handles U32K0 interrupt request.
  * @param  None
  * @retval None
  */
void U32K0_IRQHandler(void)
{  //you must open calling it in bootloader code first,as I removed jumping as it isn't used

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('v',0);
#endif
}

/**
  * @brief  This function handles U32K1 interrupt request.
  * @param  None
  * @retval None
  */
void U32K1_IRQHandler(void)
{  //you must open calling it in bootloader code first,as I removed jumping as it isn't used

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('c',0);
#endif
}

/**
  * @brief  This function handles I2C interrupt request.
  * @param  None
  * @retval None
  */
void I2C_IRQHandler(void)
{  //you must open calling it in bootloader code first,as I removed jumping as it isn't used

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('i',0);
#endif
}

/**
  * @brief  This function handles SPI1 interrupt request.
  * @param  None
  * @retval None
  */
void SPI1_IRQHandler(void)
{//you must open calling it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('s',0);
#endif
}

/**
  * @brief  This function handles UART0 interrupt request.
  * @param  None
  * @retval None
  */



uint8_t uart_receive_byte(UART_TypeDef *uart_num)
{
  uint8_t data;

  data = UART_ReceiveData(uart_num);

  if (get_even_flag_uart(uart_num))
  {
    data = data & 0x7f;
  }
  return data;
}

void UART0_IRQHandler(void)
{
  UART_ClearINTStatus(UART0, UART_INTSTS_RX);
  UART_ClearINTStatus(UART0, 0xffffffff /*UART_INTSTS_Msk*/ /*UART_INTSTS_RX*/);
  UART_ClearINTStatus(UART0,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART0,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART0,  UART_INTSTS_RX );

}

/**
  * @brief  This function handles UART1 interrupt request.
  * @param  None
  * @retval None
  */
void UART1_IRQHandler(void)
{
    UART_ClearINTStatus(UART1, UART_INTSTS_RX);



}

/**
  * @brief  This function handles UART2 interrupt request.
  * @param  None
  * @retval None
  */
void UART2_IRQHandler(void)
{
    UART_ClearINTStatus(UART2, UART_INTSTS_RX);
    
  UART_ClearINTStatus(UART2, 0xffffffff /*UART_INTSTS_Msk*/ /*UART_INTSTS_RX*/);
  UART_ClearINTStatus(UART2,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART2,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART2,  UART_INTSTS_RX );
}

/**
  * @brief  This function handles UART3 interrupt request.
  * @param  None
  * @retval None
  */
#if 0                                                               /*ADDCode_K*/  
volatile uint32_t how_manyBytesRxTouart3 = 0;
uint8_t hh=0,hhh=0;

void UART3_IRQHandler(void)
{
  //static uint8_t ii=0,arr2[12]={0};

  UART_ClearINTStatus(UART3, UART_INTSTS_RX);

}
#endif

/**
  * @brief  This function handles UART4 interrupt request.
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void)
{
  UART_ClearINTStatus(UART4, UART_INTSTS_RX);
  UART_ClearINTStatus(UART4, 0xffffffff /*UART_INTSTS_Msk*/ /*UART_INTSTS_RX*/);
  UART_ClearINTStatus(UART4,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART4,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART4,  UART_INTSTS_RX );

}

/**
  * @brief  This function handles UART5 interrupt request.
  * @param  None
  * @retval None
  */
#pragma optimize=z none no_code_motion no_cse
void UART5_IRQHandler(void)
{
  UART_ClearINTStatus(UART5, 0xffffffff /*UART_INTSTS_Msk*/ /*UART_INTSTS_RX*/);
  UART_ClearINTStatus(UART5,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART5,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART5,  UART_INTSTS_RX );
}

#if 0
/**
  * @brief  This function handles ISO78160 interrupt request.
  * @param  None
  * @retval None
  */
void ISO78160_IRQHandler(void)
{//you must open calling it in bootloader code first,as I removed jumping as it isn't used
ARM_ISO78160_IRQHandler();
}
#endif
/**
  * @brief  This function handles ISO78161 interrupt request.
  * @param  None
  * @retval None
  */
void ISO7816_IRQHandler(void)
{//you must open calling it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('0',0);
#endif
  ARM_ISO78160_IRQHandler();
}


/**
  * @brief  This function handles TMR0 interrupt request.
  * @param  None
  * @retval None
  */
//uint64_t val[30]={0},vfg[30]={0};uint16_t bb=0;uint64_t debug_timer0 = 0,temp0=0,temp1=0,temp2=0,energ_acc=0;
void TMR0_IRQHandler(void)
{
    if (TMR_GetINTStatus(TMR0))
  {
    TMR_ClearINTStatus(TMR0);
  }
}

/**
  * @brief  This function handles TMR1 interrupt request.
  * @param  None
  * @retval None
  */
void TMR1_IRQHandler(void)
{
  if (TMR_GetINTStatus(TMR1))
  {
    TMR_ClearINTStatus(TMR1);
#ifndef PLC_MODEM
    //PHDOWN_chk_event = SET;
    //counter_tmr1 ++;
    //count1_tmr1++;
#else
//    if(test == 1)
//      S_GPIO_Set_Bits(TMPR_LED_PORT, TMPR_LED_PIN);

      app_process_thread();
    
//    if(test == 1)
//      S_GPIO_Clear_Bits(TMPR_LED_PORT, TMPR_LED_PIN);
#endif
  }
  #ifdef DEBUG_HANG_LOOPING
    static uint32_t cv=0;
    if (cv++ >= 100)
    uart_debug_byte ('"',0);
#endif
}
/**
  * @brief  This function handles TMR2 interrupt request.
  * @param  None
  * @retval None
  */
   volatile uint32_t counter_1ms=0;
void TMR2_IRQHandler(void)
{
#ifndef PLC_MODEM
  static uint32_t counter_10ms = 0;
  counter_1ms++;
#endif
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte (';',0);
#endif
  if (TMR_GetINTStatus(TMR2))
  {
    TMR_ClearINTStatus(TMR2);
#ifndef PLC_MODEM
    //OS_Timer_iHandler();//i think we didn't need it
    if (++counter_10ms >= 100)
    {
#ifdef DLMS_ENABLED
      Dlms_Hdlc_gEndInactivityTimeOut(1);
#endif
      counter_10ms = 0;
	}
#else
//    if(test == 2)
//      S_GPIO_Set_Bits(TMPR_LED_PORT, TMPR_LED_PIN);

     uint32_t size = DMA_GetPackLenTransferred(1) + DMA_GetFrameLenTransferred(1)*256;
     while(size != DMA_The_last_Index){
       (*RX1_CallBack)(DMA_Buffer[DMA_The_last_Index]);
         DMA_The_last_Index++;
         DMA_The_last_Index = DMA_The_last_Index % 1536;
     }
      (*TMR2_CallBack)();
    
//    if(test == 2)
//      S_GPIO_Clear_Bits(TMPR_LED_PORT, TMPR_LED_PIN);
#endif
  }
#ifdef DEBUG_HANG_LOOPING
    static uint32_t cv=0;

  if (cv++ >= 100)
    uart_debug_byte ('"',0);
#endif
}
/**
  * @brief  This function handles TMR3 interrupt request.
  * @param  None
  * @retval None
  */
void TMR3_IRQHandler(void)
{

    if (TMR_GetINTStatus(TMR3))
  {
    TMR_ClearINTStatus(TMR3);
#ifdef PLC_MODEM
//    if(test == 0)
//      S_GPIO_Set_Bits(TMPR_LED_PORT, TMPR_LED_PIN);
    
   (*TMR0_CallBack)();
    
//    if(test == 0)
//      S_GPIO_Clear_Bits(TMPR_LED_PORT, TMPR_LED_PIN);
#endif
  }
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte (',',0);
#endif
}

/**
  * @brief  This function handles PWM0 interrupt request.
  * @param  None
  * @retval None
  */
void PWM0_IRQHandler(void)
{//you must open calling it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('^',0);
#endif
}

/**
  * @brief  This function handles PWM1 interrupt request.
  * @param  None
  * @retval None
  */
void PWM1_IRQHandler(void)
{//you must jump to it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('%',0);
#endif
}

/**
  * @brief  This function handles PWM2 interrupt request.
  * @param  None
  * @retval None
  */
void PWM2_IRQHandler(void)
{//you must jump to it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('$',0);
#endif
}

/**
  * @brief  This function handles PWM3 interrupt request.
  * @param  None
  * @retval None
  */
void PWM3_IRQHandler(void)
{//you must jump to it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('#',0);
#endif
}

/**
  * @brief  This function handles DMA interrupt request.
  * @param  None
  * @retval None
  */
void DMA_IRQHandler(void)
{//you must jump to it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
      uart_debug_byte ('@',0);
#endif
#ifdef PLC_MODEM
  if (DMA_GetINTStatus(0x400))
  {
    flag_framend = 1;
    DMA_ClearINTStatus(0x400);
    UART_ClearFlag(UART1, 0x80);
    DMA_DeInit(2);
    NVIC_DisableIRQ(DMA_IRQn);
    if(remaining && remaining_addr){
      Send_DMA_UART(remaining, remaining_addr);
    }
    else{
      (*TX1_CallBack)();      /*edited*/
    }
  }
#endif
}

/**
  * @brief  This function handles FLASH interrupt request.
  * @param  None
  * @retval None
  */
void FLASH_IRQHandler(void)
{//you must jump to it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
      uart_debug_byte ('!',0);
#endif
     /* if (FLASH_GetINTStatus(FLASH_INT_CS))
      {
        FLASH_ClearINTStatus(FLASH_INT_CS);
      }*/
}

/**
  * @brief  This function handles ANA interrupt request.
  * @param  None
  * @retval None
  */
void ANA_IRQHandler(void)
{
  if (ANA_GetINTStatus(ANA_INT_VDDALARM))
  {
    ANA_ClearINTStatus(ANA_INT_VDDALARM);
  }
  
}
#if 0
/**
  * @brief  This function handles SPI2 interrupt request.
  * @param  None
  * @retval None
  */
void SPI2_IRQHandler(void)
{//you must jump to it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('~',0);
#endif
}
#endif

#endif /* MicroController */
/******************* (C) COPYRIGHT Vango Technologies, Inc *****END OF FILE****/
