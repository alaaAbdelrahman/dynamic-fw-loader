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
* File Name   : r_demo_tools.h
*    @version
*        $Rev: 6426 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2021-04-01 10:04:16 +0900#$
* Description :
******************************************************************************/

#ifndef R_DEMO_TOOLS_H
#define R_DEMO_TOOLS_H

/******************************************************************************
Macro definitions
******************************************************************************/
/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum
{
    R_DEMO_LED_STOP = 0,
    R_DEMO_LED_ALERT,         /* fast blink */
    R_DEMO_LED_BOOT,          /* slow blink */
    R_DEMO_LED_IDLE,          /* lighting-up */
} r_demo_led_mode_t;


/******************************************************************************
Functions prototype
******************************************************************************/
/******************************************************************************
* Function Name     : hex_string_to_uint64
* Description       : Converts string buffer to uint64_t value
* Argument          : uartRxBuffer : String buffer
* Return Value      : 64 bit unsigned integer value extracted from string buffer
******************************************************************************/
/*!
   \fn uint64_t hexStringToEUI(uint8_t* uartRxBuffer);
   \brief Converts string buffer to uint64_t value
   \param[in] uartRxBuffer : String buffer
   \return 64 bit unsigned integer value extracted from string buffer
 */
uint64_t hex_string_to_uint64 (uint8_t * uartRxBuffer);

/******************************************************************************
* Function Name     : hex_string_to_uint32
* Description       : Converts string buffer to uint32_t value
* Argument          : uartRxBuffer : String buffer
* Return Value      : 32 bit unsigned integer value extracted from string buffer
******************************************************************************/
/*!
   \fn uint32_t hex_string_to_uint32(uint8_t* uartRxBuffer);
   \brief Converts string buffer to uint32_t value
   \param[in] uartRxBuffer String buffer
   \return 32 bit unsigned integer value extracted from string buffer
 */
uint32_t hex_string_to_uint32 (uint8_t * uartRxBuffer);

/******************************************************************************
* Function Name     : hex_string_to_uint16
* Description       : Converts string buffer to uint16_t value
* Argument          : uartRxBuffer : String buffer
* Return Value      : 16 bit unsigned integer value extracted from string buffer
******************************************************************************/
/*!
   \fn uint16_t hex_string_to_uint16(uint8_t* uartRxBuffer);
   \brief Converts string buffer to uint16_t value
   \param[in] uartRxBuffer String buffer
   \return 16 bit unsigned integer value extracted from string buffer
 */
uint16_t hex_string_to_uint16 (uint8_t * uartRxBuffer);

/******************************************************************************
* Function Name     : hex_string_to_uint8
* Description       : Converts string buffer to uint8_t value
* Argument          : uartRxBuffer : String buffer
* Return Value      : 8 bit unsigned integer value extracted from string buffer
******************************************************************************/
/*!
   \fn uint8_t hex_string_to_uint8(uint8_t* uartRxBuffer);
   \brief Converts string buffer to uint8_t value
   \param[in] uartRxBuffer : String buffer
   \return 8 bit unsigned integer value extracted from string buffer
 */
uint8_t hex_string_to_uint8 (uint8_t * uartRxBuffer);

void hex_string_128bit_to_uint8_arr (uint8_t * uartRxBuffer, uint8_t * parr);

/******************************************************************************
* Function Name     : R_DEMO_SoftReset
* Description       : Soft resets MCU
* Argument          : None
* Return Value      : None
******************************************************************************/
/*!
   \fn void R_DEMO_SoftReset(void);
   \brief Soft resets MCU
 */
void R_DEMO_SoftReset (void);

/******************************************************************************
* Function Name     : R_DEMO_HndFatalError
* Description       : Prints error message and resets MCU
* Argument          : None
* Return Value      : None
******************************************************************************/
/*!
   \fn void R_DEMO_HndFatalError(void);
   \brief Prints error message and resets MCU
 */
void R_DEMO_HndFatalError (void);

/******************************************************************************
* Function Name     : R_DEMO_HndWaitEnter
* Description       : wait press any key, and return
* Argument          : None
* Return Value      : None
******************************************************************************/
/*!
\fn void R_DEMO_HndWaitEnter(void);
\brief wait press any key, and return
*/
void R_DEMO_HndWaitEnter(void);

/******************************************************************************
* Function Name: R_DEMO_LED
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_DEMO_LED (uint8_t ch_id, r_demo_led_mode_t mode);


#endif /* R_DEMO_TOOLS_H */

