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
* Copyright (C) 2015-2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/******************************************************************************
* File Name     : r_led_api.h
* Device(s)     : RX631
* Tool-chain    : KPIT GNURX-ELF 16.01
* H/W platform  : G-CPX / EU-CPX2 / G-CPX3
* Description   : Sample software
******************************************************************************/

/*
 * Prevent nested inclusions
 */
#ifndef R_LED_API_H
#define R_LED_API_H

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/***********************************************************************
* Function Name     : R_LED_Start
* Description       : This function blink the LED
* Argument          : led_id : Control LED number
*                     toggle_msec : Blinking cycle. When it is 0, always turn on the LED
* Return Value      : R_RESULT_FAILED or R_RESULT_SUCCESS
***********************************************************************/
/*!
   \fn          r_result_t R_LED_Start(uint8_t led_id, uint32_t toggle_msec)
   \brief       This function blink the LED
   \param[in]   led_id Control LED number
   \param[in]   toggle_msec Blinking cycle
   \return   Either R_RESULT_SUCCESS or R_RESULT_FAILED
 */
r_result_t R_LED_Start(uint8_t led_id, uint32_t toggle_msec);

/***********************************************************************
* Function Name     : R_LED_Stop
* Description       : This function turn off the LED
* Argument          : led_port : Control LED port
* Return Value      : R_RESULT_FAILED or R_RESULT_SUCCESS
***********************************************************************/
/*!
   \fn          result_t R_LED_Stop(uint8_t led_id);
   \brief       This function turn off the LED
   \param[in]   led_id Control LED number
   \return   Either R_RESULT_SUCCESS or R_RESULT_FAILED
 */
r_result_t R_LED_Stop(uint8_t led_id);


#endif /* R_LED_API_H */
