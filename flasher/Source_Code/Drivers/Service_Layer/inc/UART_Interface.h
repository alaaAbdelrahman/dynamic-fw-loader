
#ifndef UART_DRV_H_
#define UART_DRV_H_


#include "config.h"                                      /*check it with ENG.ESmail*/ 
#if (MicroController == Micro_V94XX)
#include "V94XX_UART.h"
#include "V94XX_CLK.h"
#include "V94XX_CORTEX.h"
#else
#include "v85xx_uart.h"
#include "v85xx_clk.h"
#include "v85xx_cortex.h"
#endif
#include "UART_Config.h"

#define BAUDRATE_4800   4800
enum
{
    EVEN_FLAG_UART_NUM_0=0x01,
    EVEN_FLAG_UART_NUM_1=0x02,
    EVEN_FLAG_UART_NUM_2=0x04,
    EVEN_FLAG_UART_NUM_3=0x08,
    EVEN_FLAG_UART_NUM_4=0x10,
    EVEN_FLAG_UART_NUM_5=0x20,
};

void S_USCI_A0_ISR(void);
void S_USCI_A4_ISR(uint8_t rx_byte);
void S_USCI_A5_ISR(uint8_t rx_byte);
void S_USCI_A2_ISR(uint8_t rx_byte);
void S_UART_SEND_BYTE(UART_TypeDef*uart_num ,uint8_t u8byte) ;
void S_UART_SEND_FRAME(UART_TypeDef*uart_num ,uint8_t* u8byte, uint16_t size);
uint8_t S_UART_INIT (UART_TypeDef*uart_num ,uint32_t baud_rate,uint8_t uiDataBit,uint8_t uiParity);
uint8_t S_UART_RECEIVE_BYTE(UART_TypeDef*uart_num );
void S_UART_DISABLE (UART_TypeDef*uart_num );
uint16_t S_ReceiveFrameUSCI_A3(uint8_t* puiBuff, uint16_t uiMaxSize);
uint16_t S_ReceiveFrameUSCI_A0(uint8_t* puiBuff, uint16_t uiMaxSize);
void S_SendFrameUSCI_A0(uint8_t* puiBuff, uint16_t uiSize);
uint8_t S_UART_RX_FRAME( UART_TypeDef*uart_num  , uint8_t* data_buf , uint8_t size);
uint8_t S_UART_Clear_RX_BUFFER (UART_TypeDef*uart_num);
static uint8_t S_UART_CHECK_PARITY_BYTE(uint8_t u8byte) ;
uint8_t get_even_flag_uart(UART_TypeDef *uart_num);

//=============================================================================
// FUNCTION
//=============================================================================
// Name           : uart_debug_byte
// Description    : enable  uart debug port, then send uart byte then disable this uart debug port
//-----------------------------------------------------------------------------
// Author         : Ibrahem Mostafa
// Date           : 16/02/2020
//=============================================================================
void uart_debug_byte (uint8_t send_Byte, uint8_t delay_ms);
#endif
