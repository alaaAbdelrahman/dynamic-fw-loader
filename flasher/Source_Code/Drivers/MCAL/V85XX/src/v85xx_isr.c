#include "config.h"
#if (MicroController == Micro_V85XX)
/**
* @file    v85xx_isr.c
* @author  weaktea
* @version V1.0
* @date    2016/07/05
* @brief   Main Interrupt Service Routines.
******************************************************************************/
#include "meter_init.h"
#include "Dlms_Hdlc.h"
#include "v85xx_wdt.h"
#include "v85xx_isr.h"
#include "v85xx_tmr.h"
#include "RTC_Interface.h"
#include "UART_Interface.h"
#include "Meter_Interface.h"
#include "ctrl.h"
#include "Power_modes.h"
#include "v85xx_pmu.h"
#include "task_priority.h"
#include "TI_CC1120.h"
#include "modem.h"
#include "iec62056_21_slave.h"
#include "display.h"
#include "Timer_Interface.h"
#include "V9261f.h"
#include "V9360.h"
#include "ymodem.h"
#include "v85xx_flash.h"  
#include "v85xx_iso7816.h"
#include "SystemTick_Interface.h"
#include "self_test.h"

uint8_t rtc_flag_counter = 0;
#if defined (IEC_62056_21_SLAVE) && defined (RS485_NetworkDiscovery_Feature)
extern uint8_t SCAN_MODE_Flag;
#endif
volatile uint16_t counter_tmr1 = 0;
volatile uint16_t count1_tmr1 = 0;
volatile uint8_t IRQ0STA = (uint8_t)0;
volatile uint8_t seconds = 0;
uint8_t line_period = LINEPERIOD;
uint8_t Check_var = 0;	//todo_mai checkfor its effect , change its scope
uint16_t rx_index_7816 = 0;
uint8_t rx_buffer_7816[500];

#define RTC_TELL_MTR_NEW_ENERGY()  mtr_drv_event(MTR_NEW_ENERGY)/*!< Publish new energy event.*/        
#define CTRL_UP_PRESSED_ISR()  ctrl_up_pressed_isr()/*!< Publish new energy event.*/  
#define CTRL_DOWN_PRESSED_ISR()  ctrl_down_pressed_isr()/*!< Publish new energy event.*/  
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

  if ( !S_GPIO_Get_Bits(SW_PORT, TERMINAL_SW_PIN)||!S_GPIO_Get_Bits(SW_PORT, TELT_SW_PIN))
  {
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('h',10);
#endif
    Delay_ms(120);
    ctrl_rtc_isr();
    if (!S_GPIO_Get_Bits(SW_PORT, TELT_SW_PIN))
    {
      ctrl_power_down_event |= CVR_OPN_MASK;
      PMU_WakeUpPinConfig(TELT_SW_PIN , IOA_DISABLE);
    }

    if (!S_GPIO_Get_Bits(SW_PORT, TERMINAL_SW_PIN))
    {
      ctrl_power_down_event |= TRMNAL_OPN_MASK;
      PMU_WakeUpPinConfig(TERMINAL_SW_PIN , IOA_DISABLE);
    }
    GPIOBToF_ResetBits(I2C_EXTRA_PORT,I2C_EXTRA_PIN);
    CTRL_SW_STATE_STORE(&ctrl_power_down_event);
 
 /*RmvCodCmntA_K*/
 
  }
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
#ifdef ASHNTTI_PROJECT
  if (PMU_GetIOAINTStatus(SW_SCROLL_UP_PIN)) 
  {  
    PMU_WakeUpPinConfig(SW_SCROLL_UP_PIN , IOA_DISABLE);
    PMU_ClearIOAINTStatus(SW_SCROLL_UP_PIN);
    if (S_GPIO_Get_Bits(SW_PORT, SW_SCROLL_UP_PIN))
    CTRL_UP_PRESSED_ISR();
  }
  
  if (PMU_GetIOAINTStatus(SW_SCROLL_DOWN_PIN))  
  { 
    PMU_WakeUpPinConfig(SW_SCROLL_DOWN_PIN , IOA_DISABLE);
    PMU_ClearIOAINTStatus(SW_SCROLL_DOWN_PIN);
    if (S_GPIO_Get_Bits(SW_PORT, SW_SCROLL_DOWN_PIN))
    CTRL_DOWN_PRESSED_ISR();  
  }
#endif
#if defined (V9203_ENABLE) || defined (ADE7953_ENABLE)
  if (PMU_GetIOAINTStatus(MTR_IRQ_PIN))
  {   
    PMU_ClearIOAINTStatus(MTR_IRQ_PIN);
#ifdef V9203_ENABLE 
    PMU_WakeUpPinConfig(MTR_IRQ_PIN,IOA_DISABLE);
    V9203_IRQ_isr();
#elif defined (ADE7953_ENABLE)
    ade7953_isr();
#endif
  }
#endif
#ifdef V9360_ENABLE
   if (PMU_GetIOAINTStatus(GPIO_Pin_7))
  {   
    PMU_ClearIOAINTStatus(GPIO_Pin_7);
    PMU_WakeUpPinConfig(GPIO_Pin_7,IOA_DISABLE);
    PWR_down_detect |= VDCIN_PWR_DOWN;
    task_priority_set(PRIORITY_FOR_METER_TASK);
  }
#endif
  if (PMU_GetIOAINTStatus(TELT_SW_PIN) || PMU_GetIOAINTStatus(TERMINAL_SW_PIN))
  {
    GPIO_InitType GPIO_InitStruct;
    uint8_t counter = 0;
    /* Configure EEPORM WP, output high CMOS mode */
    GPIOBToF_SetBits(EEPROM_WP_PORT,EEPROM_WP_PIN);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUTPUT_CMOS;
    GPIO_InitStruct.GPIO_Pin =EEPROM_WP_PIN;
    GPIOBToF_Init(EEPROM_WP_PORT, &GPIO_InitStruct);
    
    /* Configure i2c extra, output low  mode */
    GPIOBToF_ResetBits(I2C_EXTRA_PORT,I2C_EXTRA_PIN);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUTPUT_CMOS;
    GPIO_InitStruct.GPIO_Pin = I2C_EXTRA_PIN;
    GPIOBToF_Init(I2C_EXTRA_PORT, &GPIO_InitStruct);
    Delay_ms(120);
    
    ctrl_rtc_isr();
    counter = 0;
    while (!S_GPIO_Get_Bits(SW_PORT, TELT_SW_PIN) && ++counter < 3 );
    if (counter == 3)
    {
      ctrl_power_down_event |= CVR_OPN_MASK;
      PMU_WakeUpPinConfig(TELT_SW_PIN , IOA_DISABLE);
      PMU_ClearIOAINTStatus(TELT_SW_PIN);
    }

    counter = 0;
    while (!S_GPIO_Get_Bits(SW_PORT, TERMINAL_SW_PIN) && ++counter < 3 );
    if (counter == 3)
    {
      ctrl_power_down_event |= TRMNAL_OPN_MASK;
      PMU_WakeUpPinConfig(TERMINAL_SW_PIN , IOA_DISABLE);
      PMU_ClearIOAINTStatus(TERMINAL_SW_PIN);
    }

    GPIOBToF_ResetBits(I2C_EXTRA_PORT,I2C_EXTRA_PIN);
#ifdef DEBUG_LOW_POWER
  uart_debug_byte ('T',0);
#endif
    CTRL_SW_STATE_STORE(&ctrl_power_down_event);

    /*RmvCodCmntA_K*/
  }
#ifdef DEBUG_LOW_POWER
  if (PMU_GetINTStatus(PMU_INTSTS_32K|PMU_INTSTS_6M|PMU_INTSTS_PORST|PMU_INTSTS_EXTRST|PMU_INTSTS_DPORST))
  {
    if ( PMU_GetINTStatus(PMU_INTSTS_32K) )   uart_debug_byte ('y',0);
        if ( PMU_GetINTStatus(PMU_INTSTS_6M) ) uart_debug_byte ('k',0);
            if ( PMU_GetINTStatus(PMU_INTSTS_PORST) ) uart_debug_byte ('f',0);
                if ( PMU_GetINTStatus(PMU_INTSTS_EXTRST) ) uart_debug_byte ('z',0);
                    if ( PMU_GetINTStatus(PMU_INTSTS_DPORST) ) uart_debug_byte ('o',0);
  }
if (PMU_GetIOAINTStatus(GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15))
  {
    uart_debug_byte ('G',0);
  }
#endif
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
    if(!(ctrl_flags & CTRL_PWR_DWN_MASK))
    {
      rtc_flag_counter++;
      //ctrl_rtc_isr();
      //seconds_debug++;
#ifndef TRF_GRACE_48_HOURS_COMPARING_TIME
      trf_grace_elapsed_time.LPM_elapsed++;
#endif 
      seconds_wasSaved++;
      seconds_wasSaved_2++;
      Tooling_Counter++;
    }
        else
    {
#ifndef TRF_GRACE_48_HOURS_COMPARING_TIME
      trf_grace_elapsed_time.LPM_elapsed += 30;
#endif
      CTRL_SET_RTC_INT_LOW_PWR_FLAG();
    }
  }
  else
  {
#ifdef DEBUG_LOW_POWER 
    uart_debug_byte ('6',10);
#endif
  }
#ifdef ASHNTTI_PROJECT
  if((ctrl_flags & CTRL_PWR_DWN_MASK))
  {
    CTRL_SET_RTC_INT_LOW_PWR_FLAG();
  }
#endif
#if defined (IEC_62056_21_SLAVE) && defined (RS485_NetworkDiscovery_Feature)
  if(SCAN_MODE_Flag)
    S_UART_INIT((UART_TypeDef*)RS_UART_PORT_NUMBER,'0',UART_WORDLEN_8B,Dlms_NoParity);
#endif    
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
 // USCI_A0_ISR();
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('q',0);
#endif
  //OPTRXBUF=uart_receive_byte(UART0);
  uint8 byte = uart_receive_byte(UART0);

#ifdef GPRS_FEATURE
  MODEM_rx_isr(byte);
#endif

}

/**
  * @brief  This function handles UART1 interrupt request.
  * @param  None
  * @retval None
  */
void UART1_IRQHandler(void)
{
    UART_ClearINTStatus(UART1, UART_INTSTS_RX);

#ifdef DEBUG_HANG_LOOPING
    uart_debug_byte (']',0);
#endif
    
#ifdef RF_LINK_TI_CC1120_ENABLE
    TI_CC1120_rx_isr();
#else
    S_GPIO_Set_Bits(BUZZER_PORT,BUZZER_PIN);
#endif


}

/**
  * @brief  This function handles UART2 interrupt request.
  * @param  None
  * @retval None
  */
void UART2_IRQHandler(void)
{
    UART_ClearINTStatus(UART2, UART_INTSTS_RX);
    uint8 byte = uart_receive_byte(UART2);

#ifdef DEBUG_HANG_LOOPING
    uart_debug_byte ('[',0);
#endif
    S_GPIO_Set_Bits(BUZZER_PORT,BUZZER_PIN);
}

/**
  * @brief  This function handles UART3 interrupt request.
  * @param  None
  * @retval None
  */
volatile uint32_t how_manyBytesRxTouart3 = 0;
uint8_t hh=0,hhh=0;
void UART3_IRQHandler(void)
{
  //static uint8_t ii=0,arr2[12]={0};

  UART_ClearINTStatus(UART3, UART_INTSTS_RX);
    hh=UART_GetFlag(UART3,UART_FLAG_RXFULL );

  uint8 byte = uart_receive_byte(UART3);
      hhh=UART_GetFlag(UART3,UART_FLAG_RXFULL );
/*
  arr2[ii++] = byte;
  if (ii >= 8)
  {
    ii=0;
  }*/
  how_manyBytesRxTouart3++;
  if (how_manyBytesRxTouart3 > 8)
    how_manyBytesRxTouart3 = 1;
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('"',0);
#endif

}
/**
  * @brief  This function handles UART4 interrupt request.
  * @param  None
  * @retval None
  */
uint8 arr[10] ={0};
void UART4_IRQHandler(void)
{
  UART_ClearINTStatus(UART4, UART_INTSTS_RX);
#if (defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM122U_BOARD) || defined(EM130_BOARD) || defined(EM210_BOARD))
  
   uint8_t x= 0;
  UART_ClearINTStatus(UART4, 0xffffffff /*UART_INTSTS_Msk*/ /*UART_INTSTS_RX*/);
  UART_ClearINTStatus(UART4,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART4,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART4,  UART_INTSTS_RX );

#if defined (DEBUG_HANG_LOOPING) && !defined (ADE7953_ENABLE) && !defined (GPRS_FEATURE) && !defined (IEC_62056_21_SLAVE)
  uart_debug_byte('I',1);
#endif
#ifdef ADE7953_ENABLE
    if(mtr_cal_state() == FALSE)
#endif
    {
      //OPTRXBUF=uart_receive_byte(UART4);
      x=uart_receive_byte(UART4); 
      UART_ClearINTStatus(UART4,  UART_INTSTS_RX );
#ifdef TEST_YMODEM
      if (OPTRXBUF == '5')
      {
        UART_INTConfig ( UART4  , UART_INT_RX , DISABLE );
        Ymodem_Main_Menu();
        UART_INTConfig ( UART4  , UART_INT_RX , ENABLE );

      }
#else
          /*RmvCodCmntA_K*/
      S_USCI_A4_ISR(x);
#endif
    }
#ifdef ADE7953_ENABLE
  else
    mtr_cal_rx();
#endif
#ifdef GPRS_FEATURE
 // MODEM_rx_isr();
#endif 
    UART_ClearINTStatus(UART4,  UART_INTSTS_RX );
    x=uart_receive_byte(UART4); 
    UART_ClearINTStatus(UART4,  UART_INTSTS_RX );
    x=uart_receive_byte(UART4); 
    UART_ClearINTStatus(UART4,  UART_INTSTS_RX );
    x=uart_receive_byte(UART4);
  
  
#else

#if defined (BLUETOOTH_FEATURE)
  Bluetooth_rx_isr();
#endif
  #ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('"',0);
#endif
#endif

}

/**
  * @brief  This function handles UART5 interrupt request.
  * @param  None
  * @retval None
  */
#pragma optimize=z none no_code_motion no_cse
void UART5_IRQHandler(void)
{
  uint8_t x= 0;
  UART_ClearINTStatus(UART5, 0xffffffff /*UART_INTSTS_Msk*/ /*UART_INTSTS_RX*/);
  UART_ClearINTStatus(UART5,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART5,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART5,  UART_INTSTS_RX );

#if defined (DEBUG_HANG_LOOPING) && !defined (ADE7953_ENABLE) && !defined (GPRS_FEATURE) && !defined (IEC_62056_21_SLAVE)
  uart_debug_byte('I',1);
#endif
#ifdef ADE7953_ENABLE
    if(mtr_cal_state() == FALSE)
#endif
    {
      //OPTRXBUF=uart_receive_byte(UART5);
      x=uart_receive_byte(UART5); 
      UART_ClearINTStatus(UART5,  UART_INTSTS_RX );
#ifdef TEST_YMODEM
      if (OPTRXBUF == '5')
      {
        UART_INTConfig ( UART5  , UART_INT_RX , DISABLE );
        Ymodem_Main_Menu();
        UART_INTConfig ( UART5  , UART_INT_RX , ENABLE );

      }
#else
#ifdef DLMS_UI
      S_USCI_A2_ISR(x);
#else
	  S_USCI_A5_ISR(x);
#endif
#endif
    }
#ifdef ADE7953_ENABLE
  else
    mtr_cal_rx();
#endif
#ifdef GPRS_FEATURE
 // MODEM_rx_isr();
#endif 
    UART_ClearINTStatus(UART5,  UART_INTSTS_RX );
    x=uart_receive_byte(UART5); 
    UART_ClearINTStatus(UART5,  UART_INTSTS_RX );
    x=uart_receive_byte(UART5); 
    UART_ClearINTStatus(UART5,  UART_INTSTS_RX );
    x=uart_receive_byte(UART5); 
}

/**
  * @brief  This function handles ISO78160 interrupt request.
  * @param  None
  * @retval None
  */
void ISO78160_IRQHandler(void)
{//you must open calling it in bootloader code first,as I removed jumping as it isn't used
ARM_ISO78160_IRQHandler();
}

/**
  * @brief  This function handles ISO78161 interrupt request.
  * @param  None
  * @retval None
  */
void ISO78161_IRQHandler(void)
{//you must open calling it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('0',0);
#endif
  ARM_ISO78161_IRQHandler();
}


/**
  * @brief  This function handles TMR0 interrupt request.
  * @param  None
  * @retval None
  */
//uint64_t val[30]={0},vfg[30]={0};uint16_t bb=0;uint64_t debug_timer0 = 0,temp0=0,temp1=0,temp2=0,energ_acc=0;
#ifdef AC_BUZZER_FEATURE 
    volatile uint8_t xc = 2;
#endif
void TMR0_IRQHandler(void)
{
#ifdef AC_BUZZER_FEATURE 
  static uint8_t counters=0;
  static uint32_t fp = 0;
#endif
    if (TMR_GetINTStatus(TMR0))
  {
    TMR_ClearINTStatus(TMR0);
#ifdef AC_BUZZER_FEATURE 
    if (++counters == 100)
    {
#endif
      ctrl_tmr_isr();
      timer1_tick_ms ++;
#ifdef AC_BUZZER_FEATURE 
      counters = 0 ;
    }
    if (buzzer_enable == 1)
    {
      if ((ctrl_high_freq && timer1_tick_ms%2) ||  ctrl_high_freq ==0)
      {
        if (fp > xc)
          GPIOBToF_ResetBits(BUZZER_PORT, BUZZER_PIN);
        else
          GPIOBToF_SetBits(BUZZER_PORT, BUZZER_PIN);
        fp++;
        if (fp > 2*xc+1)
          fp = 0 ;
      }
    }
    else
       GPIOBToF_ResetBits(BUZZER_PORT, BUZZER_PIN);
#endif
#if defined (DEBUG_LOW_POWER) || defined (DEBUG_HANG_LOOPING)
    static uint16 f=0;
      
    if (f>=450)
    {
      uart_debug_byte ('O',0);
      f=0;
    }
    else
      f++;
#endif
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
    PHDOWN_chk_event = SET;
    counter_tmr1 ++;
    count1_tmr1++;
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
  static uint32_t counter_10ms = 0;
  counter_1ms++;
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte (';',0);
#endif
  if (TMR_GetINTStatus(TMR2))
  {
    TMR_ClearINTStatus(TMR2);
    //OS_Timer_iHandler();//i think we didn't need it
    if (++counter_10ms >= 100)
    {
#ifdef DLMS_ENABLED
      Dlms_Hdlc_gEndInactivityTimeOut(1);
      Dlms_Hdlc_gEndInactivityTimeOut(0);
#endif
      counter_10ms = 0;
    }
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
    if(!PMU_GetPowerStatus(PMU_PWRSTS_VDDALARM))
    { 
#ifdef DEBUG_LOW_POWER
      uart_debug_byte ('R',0);
#endif
      PWR_down_detect &= ~VDDALARM_PWR_DOWN;
    }
    else
    {
#ifdef DEBUG_LOW_POWER
      uart_debug_byte ('Q',0);
#endif
      PWR_down_detect |= VDDALARM_PWR_DOWN;
      task_priority_set(PRIORITY_FOR_METER_TASK);
    }
  }
  if (ANA_GetINTStatus(ANA_INT_VDCINDROP))
  {
    ANA_ClearINTStatus(ANA_INT_VDCINDROP);
    if(!PMU_GetPowerStatus(PMU_PWRSTS_VDCINDROP))
    { 
#ifdef DEBUG_LOW_POWER
      uart_debug_byte ('N',0);
#endif
      PWR_down_detect &= ~VDCIN_PWR_DOWN;
    }
    else
    {
#ifdef DEBUG_LOW_POWER
      uart_debug_byte ('V',0);      
#endif
      PWR_down_detect |= VDCIN_PWR_DOWN;
      task_priority_set(PRIORITY_FOR_METER_TASK);
    }
   }
  if(PWR_down_detect & (VDCIN_PWR_DOWN|VDDALARM_PWR_DOWN))
    ctrl_flags |= CTRL_PWR_DWN_MASK;
  else
    ctrl_flags &= ~CTRL_PWR_DWN_MASK;

#if 1
  if (ANA_GetINTStatus(ANA_INT_VDCINDROP))
  {
    /* Power up condition */
    ANA_ClearINTStatus(ANA_INT_VDCINDROP);
    PWR_down_detect &= ~VDCIN_PWR_DOWN;
  }
  else
  {
    /* Power down condition */
    ANA_ClearINTStatus(ANA_INT_VDCINDROP);
    PWR_down_detect |= VDCIN_PWR_DOWN;
    task_priority_set(PRIORITY_FOR_METER_TASK);
  }
#endif
  
}

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

#endif  /* MicroController */
/******************* (C) COPYRIGHT Vango Technologies, Inc *****END OF FILE****/
