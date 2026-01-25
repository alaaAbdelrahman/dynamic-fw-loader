/******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
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
*
* Copyright (C) 2015-2017 Renesas Electronics Corporation. All rights reserved.
******************************************************************************/

/******************************************************************************
* File Name     : r_led_api.c
* Device(s)     : RX631
* Tool-chain    : KPIT GNURX-ELF 16.01
* H/W platform  : G-CPX / EU-CPX2 / G-CPX3
* Description   : Sample software
******************************************************************************/

/*******************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : 09.08.2013 1.00    First Release
 ******************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "r_config.h"
#include "r_bsp_api.h"
#include "r_app_timer.h"
#include "r_led_api.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/
#define MAX_USED_LED_TIMER  (R_BSP_LED_NUM)
/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

/******************************************************************************
* Static variables
******************************************************************************/
static uint8_t                  led_timer_assign = R_FALSE;
static r_timer_oneshot_table_t  timer_table[MAX_USED_LED_TIMER];
static uint8_t                  active_check[(MAX_USED_LED_TIMER / 8u) + 1u];
static uint32_t                 toggle_time[MAX_USED_LED_TIMER];

/******************************************************************************
* Local function headers
******************************************************************************/
static r_bsp_led_t led_convert_port(uint8_t led_id);
static void led_toggle_timing(uint8_t led_id);

/******************************************************************************
* Extern variables
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
Function implementations
******************************************************************************/

/***********************************************************************
* Function Name     : R_LED_Start
* Description       : This function blink the LED
* Argument          : led_id : Control LED number
*                     toggle_msec : Blinking cycle. When it is 0, always turn on the LED
* Return Value      : R_RESULT_FAILED or R_RESULT_SUCCESS
***********************************************************************/
r_result_t R_LED_Start(uint8_t led_id, uint32_t toggle_msec)
{
    r_bsp_led_t led_port = led_convert_port( led_id );
    
    if( R_BSP_LED_NUM == led_port )
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    
    if( R_FALSE == led_timer_assign )
    {
        if (R_TIMER_AssignOneShotTimer(R_TIMER_ID_LED, timer_table, MAX_USED_LED_TIMER, active_check, sizeof(active_check), &led_toggle_timing) != R_RESULT_SUCCESS)
        {
           return R_RESULT_FAILED;
        }
        led_timer_assign = R_TRUE;
    }
    
    if( R_TIMER_TimerOneShotOff( R_TIMER_ID_LED, led_id ) != R_RESULT_SUCCESS )
    {
       return R_RESULT_FAILED;
    }
    
    if( 0 != toggle_msec )
    {
        toggle_time[led_id] = toggle_msec;
        
        if( R_TIMER_TimerOneShotOn( R_TIMER_ID_LED, toggle_msec, led_id ) != R_RESULT_SUCCESS )
        {
           return R_RESULT_FAILED;
        }
    }
    
    R_BSP_LedOn( led_port );
    
    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_LED_Start
******************************************************************************/

/***********************************************************************
* Function Name     : R_LED_Stop
* Description       : This function turn off the LED
* Argument          : led_port : Control LED port
* Return Value      : R_RESULT_FAILED or R_RESULT_SUCCESS
***********************************************************************/
r_result_t R_LED_Stop(uint8_t led_id)
{
    r_bsp_led_t led_port = led_convert_port( led_id );
    
    if( R_BSP_LED_NUM == led_port )
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    
    if( R_TIMER_TimerOneShotOff( R_TIMER_ID_LED, led_id ) != R_RESULT_SUCCESS )
    {
       return R_RESULT_FAILED;
    }
    
    R_BSP_LedOff( led_port );
    
    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_LED_Stop
******************************************************************************/

/******************************************************************************
* Function Name:led_convert_port
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
static r_bsp_led_t led_convert_port(uint8_t led_id)
{
    r_bsp_led_t led_port;

    switch( led_id )
    {
        case 0:
            led_port = R_BSP_LED_4;
            break;
        case 1:
            led_port = R_BSP_LED_5;
            break;
        default:
            led_port = R_BSP_LED_NUM;
            break;
    }
    
    return led_port;
}
/******************************************************************************
   End of function  led_convert_port
******************************************************************************/

/******************************************************************************
* Function Name:led_toggle_timing
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
static void led_toggle_timing(uint8_t led_id)
{
    r_bsp_led_t led_port = led_convert_port( led_id );
    
    R_BSP_ToggleLed( led_port );
    
    R_TIMER_TimerOneShotOn( R_TIMER_ID_LED, toggle_time[led_id] , led_id );
}
/******************************************************************************
   End of function  led_toggle_timing
******************************************************************************/

