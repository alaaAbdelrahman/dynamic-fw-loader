#include "config.h"
#include "UART_Interface.h"
#include "basic_srv.h"
#include "GPIO_Interface.h"
//#include "Power_modes.h"
#include "Timer_Interface.h"
#ifdef EM210_BOARD
#include "V9340.h"
#endif
#ifdef DLMS_UI
#include "UI.h"
#endif
#ifdef CONSOL_FEATURE
#include "Consol_Interface.h"
#endif
//#include "iec62056_21_master.h"
#define USCI_A0RxBufferSize 5
#define USCI_A3RxBufferSize 5
extern uint8_t SCAN_MODE_Flag;
volatile uint8_t OPTRXBUF, RSRXBUF; // OPTRXBUF is buff for optic , RSRXBUF is buff for rs
///#if defined (DLMS_ENABLED) || defined (HDLC_PROTOCOL)

uint8_t USCI_A3RxBuffer[USCI_A3RxBufferSize];
uint8_t USCI_A0RxBuffer[USCI_A0RxBufferSize];
volatile uint16_t USCI_A3RxIndex = 0;
volatile uint16_t USCI_A0RxIndex = 0;
//#endif
uint8_t clear_even_parity_flag=0;
uint8_t UartParityTemp = 0 ;
extern uint8_t iec62056_21_lcd_icon_flg;
//////extern unsigned char RxFlag;

//=============================================================================
//                 LOCAL FUNCTIONS DEFINITION
//=============================================================================
//=============================================================================
// FUNCTION
//=============================================================================
// Name           : USCI_A3_ISR
// Description    : this function for optic
//-----------------------------------------------------------------------------
// Author         : HB
// Date           : 16/02/2011
//=============================================================================

//=============================================================================
//                 LOCAL VARIABLES DEFINITION
//=============================================================================



/*************************************************************
**************************************************************
**      Function Name  : S_UART_SEND_BYTE                   **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 4/4/2023                           **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): UART_TypeDef *  &   uint8_t        **
**      Return value   : NONE                               **
**      Description    : Function to Send one byte          **
**************************************************************
*************************************************************/
void S_UART_SEND_BYTE(UART_TypeDef *uart_num, uint8_t u8byte)
{
  uint16_t countbyte = 0;
  uint16_t timeout_max = 31000; // 61000 new value for test - 31000 old value  /*edit by A_K*/ (((uint32_t)CLK_GetHCLKFreq()/(uint32_t)13107200))*1000 +30000;
  uint32_t freq=(uint32_t)CLK_GetHCLKFreq();
 if (((freq/uart_num->BAUDDIV) + 50) >= BAUDRATE_4800) //bigger than or equal baudrate 4800
 {
  timeout_max=2200;//old 2200
 }
 if (1)
   timeout_max = 60;

  S_UART_SendData(uart_num, u8byte);

  while (!S_UART_GetFlag(uart_num, UART_FLAG_TXDONE) && ++countbyte < timeout_max);       /*check it with ENG.ESmail*/
  S_UART_ClearFlag(uart_num, UART_FLAG_TXDONE);
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////




/*************************************************************
**************************************************************
**      Function Name  : S_UART_SEND_FRAME                  **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 4/4/2023                           **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): UART_TypeDef* & uint8_t & uint16_t **
**      Return value   : NONE                               **
**      Description    : Function to Send Frame             **
**************************************************************
*************************************************************/
void S_UART_SEND_FRAME(UART_TypeDef *uart_num, uint8_t *u8byte, uint16_t size)
{
  uint16_t loop;
  // S_UART_DISABLE_RX(uart_num);
  for (loop = 0; loop < size; loop++)
  {
    if (get_even_flag_uart(uart_num)) 
    {
      u8byte[loop] = S_UART_CHECK_PARITY_BYTE(u8byte[loop]);
    }

    S_UART_SEND_BYTE(uart_num, u8byte[loop]);
  }
   // S_UART_ENABLE_RX(uart_num);
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////



/***************************************************************
****************************************************************
**      Function Name  : uart_debug_byte                      **
**      Author         : Ibrahem Mostafa                      **
**      Edit by        : Ahmed Mustafa                        **
**      Date           : 16/02/2020  updated : 30/3/2023      **
**      Reentrancy     : Non Reentrant                        **
**      Sync/Async     : Synchronous                          **
**      Parameters (in): uint8_t  &   uint8_t                 **
**      Return value   : NoNe                                 **
**      Description    : enable uart debug port,              **
**      Then send uart byte then disable this uart debug Port **
****************************************************************
***************************************************************/
void uart_debug_byte(uint8_t send_Byte, uint8_t delay_ms)
{
  if (delay_ms > 0)
    Delay_ms(delay_ms);
  S_UART_INIT(OPTICAL_UART_PORT_NUMBER, '5', S_UART_WORDLEN_8B, S_UART_PARITY_NONE);
  S_UART_SEND_BYTE(OPTICAL_UART_PORT_NUMBER, send_Byte);
  Delay_ms(10);
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////



/*************************************************************
**************************************************************
**      Function Name  : S_SendFrameUSCI_A0                 **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint8_t  &   uint16_t              **
**      Return value   : None                               **
**      Description    : Function to send USCI frame        **
**************************************************************
*************************************************************/
void S_SendFrameUSCI_A0(uint8_t *puiBuff, uint16_t uiSize)
{
  uint16_t Index;
#if (BCP_VERSION == BCP_LEBANON)
#if (defined(EM122U_BOARD) || defined(EM122_BOARD))
  S_GPIO_Clear_Bits(RS485_PORT_OUT, RS485_PIN);
#else
  S_GPIO_Clear_Bits(RS485_PORT_OUT, RS485_PIN);
#endif
#endif
  for (Index = 0; Index < uiSize; Index++)
  {
    S_UART_SEND_BYTE(S_UART0, puiBuff[Index]);
  }
#if (BCP_VERSION == BCP_LEBANON)
#if (defined(EM122U_BOARD) || defined(EM122_BOARD))
  S_GPIO_Set_Bits(RS485_PORT_OUT, RS485_PIN);
#else
  S_GPIO_Set_Bits(RS485_PORT_OUT, RS485_PIN);
#endif
#endif
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////




/*************************************************************
**************************************************************
**      Function Name  : S_UART_RECEIVE_BYTE                **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): UART_TypeDef *                     **
**      Return value   : uint8_t                            **
**      Description    : Function to Receive Byte data      **
**************************************************************
*************************************************************/
uint8_t S_UART_RECEIVE_BYTE(UART_TypeDef *uart_num)
{
  uint8_t data;

  data = S_UART_ReceiveData(uart_num);

  if (get_even_flag_uart(uart_num))
  {
    data = data & 0x7f;
  }
  return data;
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////




/*************************************************************
**************************************************************
**      Function Name  : S_UART_Clear_RX_BUFFER             **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): UART_TypeDef *                     **
**      Return value   : uint8_t                            **
**      Description    : Function to clear the RX Buffer    **
**************************************************************
*************************************************************/
uint8_t S_UART_Clear_RX_BUFFER(UART_TypeDef *uart_num)
{
  uint8_t timeout = 0;

  while ((S_UART_GetFlag(uart_num, S_UART_FLAG_RXFULL)) && (++timeout < 10))
  {
    S_UART_ReceiveData(uart_num);
  }

  if (S_UART_GetFlag(uart_num, S_UART_FLAG_RXFULL) == 1)
    return false;
  else
    return true;
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////





/*************************************************************
**************************************************************
**      Function Name  : S_UART_RX_FRAME                    **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): UART_TypeDef * & uint8_t & uint8_t **
**      Return value   : uint8_t                            **
**      Description    : Function to Receive frame          **
**************************************************************
*************************************************************/
uint8_t S_UART_RX_FRAME(UART_TypeDef *uart_num, uint8_t *data_buf, uint8_t size)
{
  uint8_t i;
  uint32_t timeout = 0;
  uint32_t timeout_max_rx = 6000; //((CLK_GetHCLKFreq()/13107200))*1000+5000;
  if (1)
    timeout_max_rx = 20;
  for (i = 0; i < size; i++)
  {
    timeout = 0;
#ifdef V9340_ENABLE
   // while ( (V9340_Rx_I_RecivedByte == 0) && ((++timeout) < timeout_max_rx));
    if(uart_num == UART_V9340)
    { data_buf[i] = V9340_RX_Buffer[i];}
      else
      {
        while ((!S_UART_GetFlag(uart_num, S_UART_FLAG_RXFULL)) && ((++timeout) < timeout_max_rx));
        data_buf[i] = S_UART_ReceiveData(uart_num);
      }
#else
    while ((!S_UART_GetFlag(uart_num, S_UART_FLAG_RXFULL)) && ((++timeout) < timeout_max_rx));
    data_buf[i] = S_UART_ReceiveData(uart_num);
#endif
    
    

    if (timeout >= timeout_max_rx)
    {
      return false;
    }
  }
#ifdef V9360_ENABLE
  Delay_us(500); //Todo
#endif
  return true;
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////





/*************************************************************
**************************************************************
**      Function Name  : NVR_GetADCVoltageParameter         **
**      Author         :  HB                                **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 16/02/2011  : updated 30/3/2023    **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint8_t *  &   uint16_t            **
**      Return value   : uint16_t                           **
**      Description    : Receive the UART data in           **
**                       the specified frame                **
**************************************************************
*************************************************************/
#ifdef DLMS_ENABLED
uint16_t S_ReceiveFrameUSCI_A0(uint8_t *puiBuff, uint16_t uiMaxSize)
{
  uint16_t i;
  uint16_t NumberOfRxData = 0;
  NumberOfRxData = USCI_A0RxIndex;
  USCI_A0RxIndex = 0;
  for (i = 0; i < NumberOfRxData; i++)
  {
    puiBuff[i] = USCI_A0RxBuffer[i];
  }
  return NumberOfRxData;
}
#endif
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////







/*************************************************************
**************************************************************
**      Function Name  : S_ReceiveFrameUSCI_A3              **
**      Author         :  HB                                **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 16/02/2011 :updated 30/3/2023      **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint8_t *  &   uint16_t            **
**      Return value   : uint16_t                           **
**      Description    : Receive the UART data              **
**                       in the specified fram              **
**************************************************************
*************************************************************/
#ifdef DLMS_ENABLED
uint16_t S_ReceiveFrameUSCI_A3(uint8_t *puiBuff, uint16_t uiMaxSize)
{

  TU16 i = 0x00;
  static TU16 USCI_A3RxReadIndex = 0;
  while ((USCI_A3RxReadIndex != USCI_A3RxIndex) && (i < uiMaxSize))
  {
    puiBuff[i++] = USCI_A3RxBuffer[USCI_A3RxReadIndex++];
    if (USCI_A3RxReadIndex >= sizeof(USCI_A3RxBuffer))
      USCI_A3RxReadIndex = 0x00;
  }
  return i;
}
#endif
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////










/****************************************************************
*****************************************************************
**      Function Name  : S_UART_INIT                           **
**      Author         :                                       **
**      Edit by        : Ahmed Mustafa                         **
**      Date           : 30/3/2023                             **
**      Reentrancy     : Non Reentrant                         **
**      Sync/Async     : Synchronous                           **
**      Parameters (in): UART_TypeDef* &uint32_t&uint8_t&uint8 **
**      Return value   : uint8_t                               **
**      Description    : Function to Init The UART             **
*****************************************************************
****************************************************************/
uint8_t S_UART_INIT(UART_TypeDef *uart_num, uint32_t baud_rate, uint8_t uiDataBit, uint8_t uiParity)
{
  UART_InitType InitStruct;
  IRQn_Type uart_irqn;
  
#ifdef CONSOL_FEATURE
  if((CONS_FactoryActivated == true)  && (uart_num == OPTICAL_UART_PORT_NUMBER))
  {
    return false;
  }
#endif
  

  UART_DeInit(uart_num);

  /* Initialize the FirstBit member */
  InitStruct.FirstBit = S_UART_FIRSTBIT_LSB;
  /* Initialize the Mode member */
  InitStruct.Mode = S_UART_MODE_TX | S_UART_MODE_RX;
  /* Initialize the Parity member */
  if(uiParity ==0)
  {
    if((uart_num==UART0))
    {
      UartParityTemp|=EVEN_FLAG_UART_NUM_0;
      clear_even_parity_flag=EVEN_FLAG_UART_NUM_0;
    }
    else if(uart_num==UART1)
    {
      UartParityTemp|=EVEN_FLAG_UART_NUM_1; 
      clear_even_parity_flag=EVEN_FLAG_UART_NUM_1;
    }
    else if(uart_num==UART2)
    {
      UartParityTemp|=EVEN_FLAG_UART_NUM_2;
      clear_even_parity_flag=EVEN_FLAG_UART_NUM_2;
    }
    #if (MicroController == Micro_V85XX)
    else if(uart_num==UART3)
    {
      UartParityTemp|=EVEN_FLAG_UART_NUM_3;
      clear_even_parity_flag=EVEN_FLAG_UART_NUM_3;
    }
    #endif
    else if(uart_num==UART4)
    {
      UartParityTemp|=EVEN_FLAG_UART_NUM_4;
      clear_even_parity_flag=EVEN_FLAG_UART_NUM_4;
    }
    else if(uart_num==UART5)
    {
      UartParityTemp|=EVEN_FLAG_UART_NUM_5;
      clear_even_parity_flag=EVEN_FLAG_UART_NUM_5;
    }
  }
  else
  {
    if((uart_num==UART0))
    {
      clear_even_parity_flag=EVEN_FLAG_UART_NUM_0;
    }
    else if(uart_num==UART1)
    {
      clear_even_parity_flag=EVEN_FLAG_UART_NUM_1;
    }
    else if(uart_num==UART2)
    {
      clear_even_parity_flag=EVEN_FLAG_UART_NUM_2;
    }
    #if (MicroController == Micro_V85XX)
    else if(uart_num==UART3)
    {
      clear_even_parity_flag=EVEN_FLAG_UART_NUM_3;
    }
    #endif
    else if(uart_num==UART4)
    {
      clear_even_parity_flag=EVEN_FLAG_UART_NUM_4;
    }
    else if(uart_num==UART5)
    {
      clear_even_parity_flag=EVEN_FLAG_UART_NUM_5;
    }
    UartParityTemp&=(~clear_even_parity_flag);
  }
  if (uiDataBit == S_UART_WORDLEN_9B)
  {
    InitStruct.Parity = uiParity;
    InitStruct.WordLen = S_UART_WORDLEN_9B;
  }
  else
  {
    InitStruct.Parity = 0;
    /* Initialize the WordLen member */
    InitStruct.WordLen = S_UART_WORDLEN_8B;
  }
  // InitStruct.Baudrate = baud_rate; //for future modification
  /* Initialize the Baudrate member */
  if (baud_rate == '0')
    InitStruct.Baudrate = 300;
  else if(baud_rate == '1')
    InitStruct.Baudrate = 600;
  else if(baud_rate == '2')
    InitStruct.Baudrate = 1200;
  else if(baud_rate == '3')
    InitStruct.Baudrate = 2400;
  else if (baud_rate == '4')
    InitStruct.Baudrate = 4800;
  else if (baud_rate == '5')
    InitStruct.Baudrate = 9600;
  else if (baud_rate == '6')
    InitStruct.Baudrate = 19200;
  else if (baud_rate == '7')
    InitStruct.Baudrate = 115200;
  else if (baud_rate == '8')
#ifndef PLC_MODEM
	InitStruct.Baudrate = 28800;
#else
    InitStruct.Baudrate = 1014706;
#endif
  else if (baud_rate == '9')
    InitStruct.Baudrate = 57600;

  else
  {
    return FALSE;
  }
  // IRQn_Type
  switch ((uint32_t)uart_num)
  {
  case (uint32_t)S_UART0:
    uart_irqn = S_UART0_IRQn;
    break;
  case (uint32_t)S_UART1:
    uart_irqn = S_UART1_IRQn;
    break;
  case (uint32_t)S_UART2:
    uart_irqn = S_UART2_IRQn;
    break;
#if (MicroController == Micro_V94XX)            /*Define new Micro*/
#else
  case (uint32_t)S_UART3:
    uart_irqn = S_UART3_IRQn;
    break;
#endif
  case (uint32_t)S_UART4:
    uart_irqn = S_UART4_IRQn;
    break;
  case (uint32_t)S_UART5:
    uart_irqn = S_UART5_IRQn;
    break;
  }
  UART_Init(uart_num, &InitStruct);

#if !(MicroController == Micro_V94XX)
#if (defined(EM122_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM122U_BOARD))
  if (uart_num != S_UART2 && uart_num != S_UART_DEBUG)
#else
  if (uart_num != S_UART3 && uart_num != S_UART_DEBUG)
#endif
#endif
    
 #ifdef EM210_BOARD
    if(uart_num == S_UART0 )
    {S_UART_INTConfig(uart_num, S_UART_INT_RX, DISABLE);  } 
    else
      {S_UART_INTConfig(uart_num, S_UART_INT_RX, ENABLE); }
#else
S_UART_INTConfig(uart_num, S_UART_INT_RX, ENABLE);  
#endif

#if (defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD)|| defined(EM122U_BOARD))
  if (uart_num == RS_UART_PORT_NUMBER)
  {
#if (!defined (EM110_BOARD))
    S_UART_ENABLE_TX(uart_num);
    S_UART_ENABLE_RX(uart_num);
    
#else
    S_UART_DISABLE_TX(uart_num);
    S_UART_DISABLE_RX(uart_num);
    S_UART_INTConfig(uart_num, S_UART_INT_RX, DISABLE);
#endif
  }
  else
    S_UART_ENABLE_TX(uart_num);
    S_UART_ENABLE_RX(uart_num);
    
#else
      S_UART_ENABLE_TX(uart_num);
      S_UART_ENABLE_RX(uart_num);
    
#endif

S_CORTEX_SetPriority_ClearPending_EN_IRQ((IRQn_Type)uart_irqn, 0);
  
  return TRUE;
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////






/*************************************************************
**************************************************************
**      Function Name  : S_UART_DISABLE                     **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): UART_TypeDef *                     **
**      Return value   : None                               **
**      Description    : Function to Disable The uart       **
**************************************************************
*************************************************************/
void S_UART_DISABLE(UART_TypeDef *uart_num)
{
  S_UART_INTConfig(uart_num, UART_INT_RX, DISABLE);
   S_UART_DISABLE_TX(uart_num);
   S_UART_DISABLE_RX(uart_num);
   /*edit by A_K*/
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////






/*************************************************************
**************************************************************
**      Function Name  : S_UART_CHECK_PARITY_BYTE           **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint32_t  &   NVR_ADCVOLPARA       **
**      Return value   : uint32_t                           **
**      Description    : Function to check the parity       **
**************************************************************
*************************************************************/

uint8_t S_UART_CHECK_PARITY_BYTE(uint8_t u8byte)
{
  uint8_t index = 0;
  uint8_t flagparity = 0;
  /*check on first 7 bits */
  while (index < 7)
  {
    if (u8byte & (1 << index))
      flagparity++;
    index++;
  }

  if ((flagparity % 2))
  {
    SET_BIT(u8byte, 0x80);
  }
  return u8byte;
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////



/*************************************************************
**************************************************************
**      Function Name  : uart_tx_test                       **
**      Edit by        : Ahmed Elhossiny                    **
**      Date           : 10/10/2023                         **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint32_t  &   NVR_ADCVOLPARA       **
**      Return value   : uint32_t                           **
**      Description    : Function to get even flag the UART **
**************************************************************
*************************************************************/
uint8_t get_even_flag_uart(UART_TypeDef *uart_num)
{
  uint8_t ret=0;
  if(((UartParityTemp & EVEN_FLAG_UART_NUM_0)&&uart_num==UART0)||((UartParityTemp & EVEN_FLAG_UART_NUM_1)&&uart_num==UART1)||((UartParityTemp & EVEN_FLAG_UART_NUM_2)&&uart_num==UART2)
  #ifndef Micro_V85XX
  ||((UartParityTemp & EVEN_FLAG_UART_NUM_3)&&uart_num==UART3)
  #endif
  ||((UartParityTemp & EVEN_FLAG_UART_NUM_4)&&uart_num==UART4)||((UartParityTemp & EVEN_FLAG_UART_NUM_5)&&uart_num==UART5)) 
  {
    ret=1;
  }
  return ret;
}




/*************************************************************
**************************************************************
**      Function Name  : uart_tx_test                       **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 30/3/2023                          **
**      Reentrancy     : Non Reentrant                      **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint32_t  &   NVR_ADCVOLPARA       **
**      Return value   : uint32_t                           **
**      Description    : Function to test the UART          **
**************************************************************
*************************************************************/
#ifdef UART_TEST_DEBUG
void uart_tx_test(UART_TypeDef *uart_num)
{
  while (1)
  {
    S_UART_SEND_BYTE(uart_num, 10);
    vDelay_HW(1);
  }
}
void uart_rx_test(UART_TypeDef *uart_num)
{
  uint8_t Rx_CHAR = 0;
  Rx_CHAR = S_UART_RECEIVE_BYTE(uart_num);
}
#endif
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////

