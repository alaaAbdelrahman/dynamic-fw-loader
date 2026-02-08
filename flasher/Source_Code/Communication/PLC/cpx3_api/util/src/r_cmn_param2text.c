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
* http://www.renesas.com/disclaimer *
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_cmn_param2text.c
* Version      : 1.0
* Description  : Convert structure to text for log output
******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 01.04.2019 1.00     First Release
******************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"
#include "r_cmn_param2text.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define MAX_DIGITS          (12)

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
/* functions */

/* variables */
uint32_t g_strtext_mode = 0;

/******************************************************************************
* Function Name:log_param_loop_printf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint32_t R_CMN_Param2TextLoopPrint( uint32_t num, uint8_t *p_data, uint8_t space, char* p_out_text )
{
    uint32_t length = 0;
    uint32_t i;

    if( p_data != NULL )
    {
        for( i = 0; i < num; i++ )
        {
            length += sprintf(&p_out_text[length], "%02X", p_data[i]);
            if( space == R_TRUE )
            {
                length += sprintf(&p_out_text[length], " ");
            }
        }
    }
    else
    {
        length += sprintf(&p_out_text[length], "NULL");
    }

    return length;
}
/******************************************************************************
   End of function  log_param_loop_printf
******************************************************************************/


/******************************************************************************
* Function Name:R_CMN_Param2TextSetMode
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_CMN_Param2TextSetMode( uint32_t mode )
{
    g_strtext_mode = mode;
    
    return;
}
/******************************************************************************
   End of function  R_CMN_Param2TextSetMode
******************************************************************************/


/******************************************************************************
* Function Name:R_CMN_Param2TextGetMode
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint32_t R_CMN_Param2TextGetMode( void )
{
    return g_strtext_mode;
}
/******************************************************************************
   End of function  R_CMN_Param2TextGetMode
******************************************************************************/

/******************************************************************************
* Function Name:R_CMN_Param2TextClearMode
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_CMN_Param2TextClearMode( uint32_t mode )
{
    g_strtext_mode &= ~mode;
    
    return;
}
/******************************************************************************
   End of function  R_CMN_Param2TextClearMode
******************************************************************************/

/******************************************************************************
* Function Name:R_CMN_Param2TextResult
* Description :
* Arguments :
* Return Value :
******************************************************************************/
const char* R_CMN_Param2TextResult( e_result_t result )
{
    const char *p_ret;
    
    switch (result)
    {
        ENUM_CASE(R_RESULT, SUCCESS)                     // @suppress("Switch statement")    // @suppress("Case statement braces")
        ENUM_CASE(R_RESULT, FAILED)                      // @suppress("Switch statement")
        ENUM_CASE(R_RESULT, BAD_INPUT_ARGUMENTS)         // @suppress("Switch statement")
        ENUM_CASE(R_RESULT, ILLEGAL_NULL_POINTER)        // @suppress("Switch statement")
        ENUM_CASE(R_RESULT, INVALID_REQUEST)             // @suppress("Switch statement")
        ENUM_CASE(R_RESULT, TIMEOUT)                     // @suppress("Switch statement")
        ENUM_CASE(R_RESULT, ABORTED)                     // @suppress("Switch statement")
        ENUM_CASE(R_RESULT, UNKNOWN)                     // @suppress("Switch statement")

        default:
        {
            p_ret = "FATAL UNDEFINED ERROR";
        }
        break;
    }
    
    return p_ret;
}
/******************************************************************************
   End of function  R_CMN_Param2TextResult
******************************************************************************/

/******************************************************************************
* Function Name: R_CMN_Param2TextDacimal
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint32_t R_CMN_Param2TextDacimal( uint32_t val, uint32_t dev, uint8_t dig )
{
    uint32_t dec=0;
    uint32_t rem = val%dev;
    uint8_t i;
    
    for(i=0;i<dig;i++)
    {
        dec*=10;
        dev/=10;
        if( 0 != dev )
        {
            dec += (rem/dev);
            rem = rem%dev;
        }
    }
    
    return dec;
}
/******************************************************************************
   End of function  R_CMN_Param2TextDacimal
******************************************************************************/
