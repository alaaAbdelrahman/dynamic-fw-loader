
/*******************************************************************************
**                                                                            **
**  PROJECT   :                                                               **
**                                                                            **
**  File Name : HW_Init.c                                                     **
**                                                                            **
**  File Type : C-file for Delays.                                            **
**                                                                            **
**  AUTHOR    : Mohamed Mahmoud Zakaria                                       **
**                                                                            **
**  PURPOSE   : This file contains hardware initialization.                   **
**                                                                            **
**  H/W       : Micro Controller: STM8L16x                                    **
**                                                                            **
**  S/W       : IAR Systems IDE & Compiler                                    **
**                                                                            **
**  History   : May 30, 2011                                                  **
**                                                                            **
**  Version   : 1.0                                                           **
*******************************************************************************/





/********************************** Headers ***********************************/
#include "meter_init.h"
//#include "Meter_Interface.h"
#include "file_sys.h"
//#include "Power_modes.h"
//#include "ADE7953.h"
#include "OS_Itf.h"
#include "Flash.h"
#include "Flash_app.h"
//#include "V9261f.h"
//#include "TarrifRegisters.h"
#ifdef EM210_BOARD
#include "V9240_Interface.h"
#endif
#ifdef STS_ENABLE
#include "STS_Module.h"
#endif
#ifdef GROUP_METERING
#include "GM_File_Sys_Manager.h"
uint32_t Tmp_Serial_buff[50]={0x00000065,0x00000064,0x00000066,0x00000067,0x00000068,0x00000001,0x00000002,0x0000006A,0x00000009,0x0000000A,
                              0x0000000B,0x000000C,0x0000000D,0x0000000E,0x0000000F,0x00000010,0x00000011,0x00000012,0x00000013,0x00000014,
                              0x00000015,0x0000016,0x0000017,0x00000018,0x00000019,0x0000001A,0x0000001B,0x0000001C,0x0000001D,0x0000001E,
                              0x0000001F,0x0000020,0x00000021,0x00000022,0x00000023,0x00000024,0x00000025,0x00000026,0x00000027,0x00000028,
                              0x00000029,0x000002A,0x0000002B,0x0000002C,0x0000002D,0x0000002E,0x0000002F,0x00000030,0x00000031,0x00000032
                             };
#endif
/*----------------------------------------------------------------------------*/

/*============================================================================================
 *         Local Macros declaration
 * ===========================================================================================*/
#define RETRY_INIT_NUM      3
/*********************************************************************************************/
volatile uint16_t eeprom_load_errors_register=0; // indicate if we load wrong data from the eeprom 
/*******************************************************************************
  Function Name   :  vMC_DriversInit

  Parameters(in)  :  None

  Parameters(out) :  None

  Return value    :  None

  Sych/Asynch     :  Synchronous

  Reentrancy      :  None Reentrant

  Description     :  This function is responsible of starting the initialization
                     of All used drivers.
*******************************************************************************/

void vMC_DriversInit(void)
{
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('e',5);
#endif

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('t',5);
#endif
#ifdef EM130_BOARD
  S_SPI1_INIT();
#endif
#if defined (MTR_THREE_PH) && !defined(MTR_TWO_PH)
  S_SPI1_INIT();
  S_SPI2_INIT();
#endif
  S_I2C_INIT(0xa0, EEPROM_I2C);
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('o',5);
#endif
  S_ADC_Init();
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('h',5);
#endif
#ifdef GPRS_DEBUG
  S_UART_INIT (OPTICAL_UART_PORT_NUMBER ,'5',UART_WORDLEN_8B,UART_PARITY_NONE);
#else
  S_UART_INIT (OPTICAL_UART_PORT_NUMBER ,'0',UART_WORDLEN_8B,UART_PARITY_EVEN);
#ifdef RS485_ENABLE
  S_UART_INIT (RS_UART_PORT_NUMBER ,'0',UART_WORDLEN_8B,UART_PARITY_EVEN);
#endif
#if defined (DLMS_UI) && !defined (RS485_ENABLE)
S_UART_INIT (RS_UART_PORT_NUMBER ,'6',UART_WORDLEN_8B,UART_PARITY_NONE);
#endif
#ifdef HDLC_PROTOCOL
  S_UART_INIT (CONNECTED_METER_MODEM ,'0',UART_WORDLEN_8B,UART_PARITY_EVEN);   //back to RS_UART_PORT_NUMBER
#endif

#ifdef EM210_BOARD                 //two phase board
  S_UART_INIT (V9240_UART_NUMBER ,'6',UART_WORDLEN_8B,UART_PARITY_EVEN);
#endif
#endif


#ifdef WIFI_ESP32_ENABLE
S_UART_INIT (S_UART2 ,'7',UART_WORDLEN_8B,UART_PARITY_EVEN);
#endif
  
#ifdef GPRS_FEATURE
  //S_UART_INIT (S_UART0 ,'5',UART_WORDLEN_8B,UART_PARITY_NONE);
#endif
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('w',5);
#endif
#ifdef MTR_SINGLE_PH
  if(PMU_GetPowerStatus(PMU_PWRSTS_VDCINDROP) == 0 )
  {
#ifdef V9261f_ENABLE
    #if !defined(EM110_BOARD) && !defined (EM330_BOARD) && !defined(EM130_BOARD) && !defined(EM210_BOARD)
    S_UART_INIT (S_UART3 ,'4',UART_WORDLEN_9B,UART_PARITY_ODD);
    #else
    S_UART_INIT (S_UART2 ,'4',UART_WORDLEN_9B,UART_PARITY_ODD);
    #endif
#else
#if (defined(EM122_BOARD) || defined(EM110_BOARD)||defined (EM330_BOARD)|| defined(EM122U_BOARD))
    S_UART_INIT (S_UART2 ,'4',UART_WORDLEN_9B,UART_PARITY_ODD);
#else
#if (MicroController == Micro_V94XX)
#else
   S_UART_INIT (S_UART3 ,'5',UART_WORDLEN_9B,UART_PARITY_ODD);  
#endif
   
#endif
#endif
  }
#endif
  /* TODO: AHMED HASSAN ... Note that Lcd_init() is called two times ..
  one time in DriverInit and other time in meterinit ... We need to call
  it once */
#ifndef DOT_MATRIX_LCD_ENABLE
  lcd_HQ30774_init( 1, LCD_FRQ_256H , LCD_DRV_150 , LCD_VLCD_0);//lcd_HQ30774_init();
#endif
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('G',0);
#endif
  
}/*end function*/



/*------------------------------------------------------------------------------
                       END Function vMC_DriversInit
------------------------------------------------------------------------------*/


/*******************************************************************************
  Function Name   :  vMeter_Init

  Parameters(in)  :  None

  Parameters(out) :  None

  Return value    :  None

  Sych/Asynch     :  Synchronous

  Reentrancy      :  None Reentrant

  Description     :  This function is responsible of Meter initialization

*******************************************************************************/
void vMeter_Init(void)
{
  uint8_t try_counter=0;
  //eeprom_test_flag = 0;
  do{
#if defined(DOT_MATRIX_LCD_ENABLE)
    Dot_Clear_Screen();
#endif
    eeprom_load_errors_register = 0;
    file_sys_init();
#ifdef DLMS_ENABLED
    /**** Init Dlms modules ****/
    /**** Launch OS Scheduler ****/
    OS_gTimerInitialize();
    /**** Init dlms cosem stack ****/
#endif
#if defined (DLMS_ENABLED) || defined (HDLC_PROTOCOL)
    Dal_Sys_gInitialize();
#endif
#ifdef DLMS_ENABLED 
#ifdef TARRIF_REGISTERS_ENABLE
   // TarrifRegisters_gInit(); TODO
#endif
    #ifdef PROFILE_DLMS_ENABLE

   // Profile_gInit();  TODO
#endif
#endif
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('/',5);
#endif
//    vDelay_SW(80000);
    ctrl_init();
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('*',5);
#endif
//    vDelay_SW(80000);
    mtr_init();
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('=',5);
#endif
//    vDelay_SW(80000);
    trf_pymt_init();
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('-',5);
#endif
//    vDelay_SW(80000);
    comm_init();
#ifdef DEBUG_HANG_LOOPING
    uart_debug_byte ('^',5);
#else
//   vDelay_SW(80000);
#endif
#ifdef DEBUG_LOW_POWER
    uart_debug_byte ('w',0);
#endif
    if (0)
    {
      CORTEX_NVIC_SystemReset(18);     //MCU reset    
    }
    display_init();

    if(eeprom_load_errors_register)
      Delay_ms(1000);
    //eeprom_load_errors_register =0;   //for debug to avoid the error must be removed 
    //is_Flash_work =1;
  }
  while((eeprom_load_errors_register || is_Flash_work == false) && ++try_counter < 10);
  try_counter = 0;

  if (eeprom_load_errors_register & DISPLAY_ERR_LOAD_FLAG)
  {
    eeprom_load_errors_register&=~DISPLAY_ERR_LOAD_FLAG;
    display_load_default_data();
  }
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('&',1);
#endif
  mtr_load_backup();
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('<',1);
#endif
  ctrl_load_backup();
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('>',1);
#endif
  trf_load_backup();
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('$',1);
#endif
  Flash_app_load_backup();
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('.',1);
#endif
#ifdef STS_ENABLE
 STS_load_backup();
#endif
#ifdef V9261f_ENABLE
  if (eeprom_load_errors_register & ADE_ERR_CAL_LOAD_FLAG)
  {
    eeprom_load_errors_register &= ~ADE_ERR_CAL_LOAD_FLAG;
    V9261f_Set_Default_Param();
  }
#endif
#ifdef GROUP_METERING
GM_SAVE_Serial(Tmp_Serial_buff,sizeof(Tmp_Serial_buff));
GM_SET_MeterSections(Tmp_Serial_buff);
#endif
}
