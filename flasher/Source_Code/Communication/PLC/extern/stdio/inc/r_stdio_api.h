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
* File Name   : r_stdio_api.h
*    @version
*        $Rev: 8968 $
*    @last editor
*        $Author: a5035756 $
*    @date
*        $Date:: 2022-03-09 11:29:12 +0900#$
* Description :
******************************************************************************/

/*
 * Prevent nested inclusions
 */
#ifndef R_STDIO_API_H
#define R_STDIO_API_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define ESC_CHARACTER            (0x1B) /* Escape character as HEX value */
#define R_UART_RX_BUFFER_LENGTH  (256)

/* For host operation, use default I/O functions. */

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

typedef enum
{
    R_STDIO_OUTPUT_STDOUT,
    R_STDIO_OUTPUT_UART_MCU_HOST,    // UART between RX and host
    R_STDIO_OUTPUT_UART_MCU_DEBUG,
}r_stdio_output_t;

/***********************************************************************
* Function Name     : R_STDIO_Init
* Description       : Initiates the STDIO by configuring STDOUT or UART
* Argument          : out_type r_stdio_output_t
* Return Value      : R_RESULT_FAILED when error else R_RESULT_SUCCESS
***********************************************************************/
/*!
   \fn          r_result_t R_STDIO_Init(void);
   \brief       Initiates the STDIO by configuring STDOUT or UART
   \return      R_RESULT_FAILED when error else R_RESULT_SUCCESS
 */
r_result_t R_STDIO_Init(r_stdio_output_t out_type);

/***********************************************************************
* Function Name     : R_STDIO_Printf
* Description       : printf implementation using UART Tx
* Argument          : const char* pformat, ... List of arguments
* Return Value      : The number of bytes printed
***********************************************************************/
/*!
   \fn          int32_t R_STDIO_Printf(const char* pformat, ...);
   \brief       printf implementation using UART Tx
   \return      The number of bytes printed
 */
int32_t R_STDIO_Printf (const char * pformat, ...);

/***********************************************************************
* Function Name     : R_STDIO_Gets
* Description       : gets implementation using UART Rx
* Argument          : char* pstr Memory location where
* Return Value      : The number of bytes read
***********************************************************************/
/*!
   \fn          uint16_t R_STDIO_Gets(uint8_t* pstr);
   \brief       gets implementation using UART Rx
   \return      The number of bytes read
 */
uint16_t R_STDIO_Gets (char * pstr);

#endif /* R_STDIO_API_H */

