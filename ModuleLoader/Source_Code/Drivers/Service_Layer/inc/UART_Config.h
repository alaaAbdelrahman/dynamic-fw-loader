
#ifndef UART_Config_H_
#define UART_Config_H_


extern volatile uint8_t OPTRXBUF , RSRXBUF;
extern volatile uint8_t Boot_loader_flag ;

#define S_UART_DISABLE_TX(uart_num)                                             UART_Cmd(uart_num,UART_MODE_TX,DISABLE)
#define S_UART_DISABLE_RX(uart_num)                                             UART_Cmd(uart_num,UART_MODE_RX,DISABLE)
#define S_UART_ENABLE_TX(uart_num)                                              UART_Cmd(uart_num,UART_MODE_TX,ENABLE)
#define S_UART_ENABLE_RX(uart_num)                                              UART_Cmd(uart_num,UART_MODE_RX,ENABLE)
#define S_UART_INTConfig(UART_NUM, UART_INT_RX, STATE)                          UART_INTConfig(UART_NUM, UART_INT_RX, STATE)


#define S_UART_SendData(UART_NUM , DATA)                                        UART_SendData(UART_NUM , DATA)
#define S_UART_GetFlag(UART_NUM , UART_FLAG_TXDONE)                             UART_GetFlag(UART_NUM , UART_FLAG_TXDONE)
#define S_UART_ClearFlag(UART_NUM , UART_FLAG_TXDONE)                           UART_ClearFlag(UART_NUM , UART_FLAG_TXDONE)
#define S_UART_ReceiveData(UART_NUM)                                            UART_ReceiveData(UART_NUM)
#define S_CORTEX_SetPriority_ClearPending_EN_IRQ(IRQ, Periority)                CORTEX_SetPriority_ClearPending_EnableIRQ(IRQ, Periority)


#define S_UART0                                                                 UART0
#define S_UART1                                                                 UART1
#define S_UART2                                                                 UART2
#define S_UART3                                                                 UART3
#define S_UART4                                                                 UART4
#define S_UART5                                                                 UART5 
#define S_UART_FLAG_RXFULL                                                      UART_FLAG_RXFULL  
#define S_UART_DEBUG                                                            UART1
#define S_UART_OPTICAL                                                          UART5
#define S_UART_FIRSTBIT_LSB                                                     UART_FIRSTBIT_LSB
#define S_UART_MODE_TX                                                          UART_MODE_TX
#define S_UART_MODE_RX                                                          UART_MODE_RX
#define S_UART_WORDLEN_9B                                                       UART_WORDLEN_9B
#define S_UART_WORDLEN_8B                                                       UART_WORDLEN_8B
#define S_UART_PARITY_NONE                                                      UART_PARITY_NONE
#define S_UART0_IRQn                                                            UART0_IRQn     
#define S_UART1_IRQn                                                            UART1_IRQn
#define S_UART2_IRQn                                                            UART2_IRQn
#define S_UART3_IRQn                                                            UART3_IRQn
#define S_UART4_IRQn                                                            UART4_IRQn
#define S_UART5_IRQn                                                            UART5_IRQn
#define S_UART_INT_RX                                                           UART_INT_RX



//#ifdef GPRS_FEATURE
#if (MicroController == Micro_V94XX)
#define MODEM_UART_PORT                               S_UART2
#else
#define MODEM_UART_PORT                               S_UART0
#endif
//#endif

#define UART_V9340        S_UART0



// UART1  not used
//UART3   not available in V94XX version || V8500 use it for Metering comunication 


/*! Initialize UART port.*/
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) || defined (EM130_BOARD) || defined (EM210_BOARD)||defined (EM330_BOARD))
#define CONNECTED_METER_MODEM           S_UART5
#define OPTICAL_UART_PORT_NUMBER        S_UART4

#ifdef EM_DL130_BOARD
#define RS_UART_PORT_NUMBER             S_UART2
#else
#define RS_UART_PORT_NUMBER             S_UART5
#endif

#else

#define OPTICAL_UART_PORT_NUMBER        S_UART5
#define RS_UART_PORT_NUMBER             S_UART5
#endif




#endif /* UART_Config_H_ */
