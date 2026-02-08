/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : r_stdio_api.c
*    @version
*        $Rev: 10117 $
*    @last editor
*        $Author: a5089763 $
*    @date  
*        $Date:: 2022-05-12 13:13:22 +0900#$
* Description : 
******************************************************************************/
/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include <stdarg.h>

#include "r_typedefs.h"
#include "r_bsp_api.h"
#include "r_stdio_api.h"
#ifdef R_DEFINE_ENABLE_RTT_VIEWER
#include"common_utils.h"
#endif

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_STDIO_MAX_BUF_LEN         (256u)   /* Max bytes to be printfed */
#define R_STDIO_MAX_DEBUG_BUF_LEN   (128u)   /* Max bytes to be printfed */

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct
{
    uint8_t*        pdata;          // Pointer to buffer to store the data
    uint16_t        cnt;            // Counts the received data
    r_boolean_t     on;             // Flag of UART Rx on
} r_stdio_uart_rx_t;

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
/******************************************************************************
* Static variables
******************************************************************************/
static          uint8_t             uart_tx_buffer[R_STDIO_MAX_BUF_LEN];
static          r_stdio_uart_rx_t   uart_rx_params;
static          r_stdio_output_t    s_out_type = R_STDIO_OUTPUT_STDOUT;
volatile r_boolean_t         tx_busy_flag = R_FALSE;

/******************************************************************************
* Local function headers
******************************************************************************/
void stdio_rx_byte_handle(uint8_t rx_byte);
void stdio_tx_finish_handle(void);
/******************************************************************************
* Extern variables
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
Function implementations
******************************************************************************/

/******************************************************************************
* Function Name:R_STDIO_Init
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_STDIO_Init(r_stdio_output_t out_type)
{
    r_result_t ret = R_RESULT_UNKNOWN;

#ifdef R_DEFINE_ENABLE_RTT_VIEWER
    s_out_type = out_type;
    return R_RESULT_SUCCESS;
#else

    switch (out_type)
    {
    case R_STDIO_OUTPUT_STDOUT:
        ret = R_RESULT_FAILED;
        break;
    case R_STDIO_OUTPUT_UART_MCU_HOST:
        ret = R_BSP_ConfigureUart(R_BSP_UART_MCU_HOST, 115200u, stdio_tx_finish_handle, stdio_rx_byte_handle);
        break;
    case R_STDIO_OUTPUT_UART_MCU_DEBUG:
        ret = R_BSP_ConfigureUart(R_BSP_UART_MCU_DEBUG, 115200u, stdio_tx_finish_handle, stdio_rx_byte_handle);
        break;
    default:
        ret = R_RESULT_BAD_INPUT_ARGUMENTS;
        break;
    }

    if (R_RESULT_SUCCESS == ret)
    {
        s_out_type = out_type;
    }

    return ret;

#endif
}
/******************************************************************************
   End of function  R_STDIO_Init
******************************************************************************/

/******************************************************************************
#ifdef __PLC__DEBUG__
* Function Name:R_STDIO_Printf
#endif
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
int32_t R_STDIO_Printf(const char* pformat, ...)
{
  int32_t string_length = 0;
    va_list argument_list;
    
    
    switch (s_out_type)
    {
    case R_STDIO_OUTPUT_STDOUT:
    string_length = 0;
    break;
    case R_STDIO_OUTPUT_UART_MCU_HOST:
    case R_STDIO_OUTPUT_UART_MCU_DEBUG:
        /* Wait for previous transmission to finish */
        while (R_TRUE == tx_busy_flag)
        {
            /* do nothing */
        }

        tx_busy_flag = R_TRUE;

        /* Init argument list */
        va_start(argument_list, pformat);

        /* Convert string to buffer */
        string_length = vsprintf((char*)uart_tx_buffer, pformat, argument_list);
        if (string_length > (int32_t)R_STDIO_MAX_BUF_LEN)
        {
            string_length = -1; /* error */
            tx_busy_flag = R_FALSE;
        }
        else
        {
#ifdef R_DEFINE_ENABLE_RTT_VIEWER
            //   APP_PRINT (pformat);
            SEGGER_RTT_Write(SEGGER_INDEX, uart_tx_buffer, (uint32_t)string_length);
            tx_busy_flag = R_FALSE;
#else
            /* Send buffer */
            R_BSP_UartSend(R_BSP_UART_MCU_HOST, uart_tx_buffer, (uint32_t)string_length);
#endif
        }

        /* Perform any cleanup necessary so that the function can return */
        va_end(argument_list);
        break;
    default:
        string_length = 0;
        break;
    }


    return string_length;
}
/******************************************************************************
#ifdef __PLC__DEBUG__
   End of function  R_STDIO_Printf
#endif
******************************************************************************/

/******************************************************************************
* Function Name:R_STDIO_Gets
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
uint16_t R_STDIO_Gets(char* pstr)
{
    volatile r_boolean_t on_flag;
    
    /* Reset counter */
    uart_rx_params.cnt = 0u;
    
    /* Set RX to ON */
    uart_rx_params.on = R_TRUE;
    on_flag           = R_TRUE;
    
    /* Set buffer */
    uart_rx_params.pdata = (uint8_t*) pstr;
    
    /* Wait for transmission to finish */
    while (R_TRUE == on_flag)
    {
#ifdef R_DEFINE_ENABLE_RTT_VIEWER
        uint8_t rx_byte[R_STDIO_MAX_BUF_LEN];
        uint8_t len;
        uint8_t i;
        
        len = APP_READ(&rx_byte, sizeof(rx_byte));
        
        if( len < sizeof(rx_byte) )
        {
            for(i=0;i<len;i++)
            {
                stdio_rx_byte_handle(rx_byte[i]);
            }
        }
#endif
        on_flag = uart_rx_params.on;
        R_BSP_NOP();
    }
    
    /* Terminate string. */
    *(pstr + uart_rx_params.cnt) = '\0';
    
    /* Return length */
    return uart_rx_params.cnt;
}
/******************************************************************************
   End of function  R_STDIO_Gets
******************************************************************************/

/******************************************************************************
* Function Name:stdio_tx_finish_handle
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
void stdio_tx_finish_handle(void)
{
    tx_busy_flag = R_FALSE;
}
/******************************************************************************
   End of function  stdio_tx_finish_handle
******************************************************************************/

/******************************************************************************
* Function Name:stdio_rx_byte_handle
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
void stdio_rx_byte_handle(uint8_t rx_byte)
{
    if ((R_TRUE == uart_rx_params.on) && (NULL != uart_rx_params.pdata))
    {
        uart_rx_params.pdata[uart_rx_params.cnt] = rx_byte;

        if (('\r' == rx_byte) || ('\n' == rx_byte))
        {
            /* Transmission finished */
            uart_rx_params.on = R_FALSE;
        }
        else if (ESC_CHARACTER == rx_byte) /* ESC */
        {
            /* If ESC, set counter to one */
            uart_rx_params.cnt = 1;

            /* Set zero element to ESC */
            uart_rx_params.pdata[0] = rx_byte;

            /* Transmission finished */
            uart_rx_params.on = R_FALSE;
        }
        else if (0x08 == rx_byte) /* Backspace */
        {
            if (uart_rx_params.cnt < 1)
            {
                uart_rx_params.cnt  = 0u;
            }
            else
            {
                uart_rx_params.cnt = (uint16_t)((int16_t)uart_rx_params.cnt-1);
            }
        }
        else
        {
            /* Wait for next character */
            uart_rx_params.cnt++;
        }
    }
    else
    {
        /* do nothing */
    }
}
/******************************************************************************
   End of function  stdio_rx_byte_handle
******************************************************************************/

