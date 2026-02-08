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
* File Name   : r_demo_tools.c
*    @version
*        $Rev: 11089 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2022-07-12 11:09:31 +0900#$
* Description :
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include <stdlib.h>
#include "r_typedefs.h"
#include "r_stdio_api.h"
#include "r_byte_swap.h"
#include "r_c3sap_api.h"
#include "r_demo_tools.h"
#include "r_demo_app.h"

#include "r_led_api.h"
#include "r_bsp_api.h"

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
Exported global variables (to be accessed by other files)
******************************************************************************/
/******************************************************************************
Private global variables and functions
******************************************************************************/
/******************************************************************************
Functions
******************************************************************************/



/******************************************************************************
* Function Name: hex_string_to_uint32
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint32_t hex_string_to_uint32 (uint8_t * uartRxBuffer)
{
    /* Terminate string */
    uartRxBuffer[10] = '\0';

    /* Convert to integer */
    return strtoul ((char const *)uartRxBuffer, NULL, 16);
}
/******************************************************************************
   End of function  hex_string_to_uint32
******************************************************************************/


/******************************************************************************
* Function Name: hex_string_to_uint16
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint16_t hex_string_to_uint16 (uint8_t * uartRxBuffer)
{
    /* Terminate string */
    uartRxBuffer[6] = '\0';

    /* Convert to integer */
    return (uint16_t)strtoul ((char const *)uartRxBuffer, NULL, 16);
}
/******************************************************************************
   End of function  hex_string_to_uint16
******************************************************************************/


/******************************************************************************
* Function Name: hex_string_to_uint8
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint8_t hex_string_to_uint8 (uint8_t * uartRxBuffer)
{
    /* Terminate string */
    uartRxBuffer[4] = '\0';

    /* Convert to integer */
    return (uint8_t)strtoul ((char const *)uartRxBuffer, NULL, 16);
}
/******************************************************************************
   End of function  hex_string_to_uint8
******************************************************************************/


/******************************************************************************
* Function Name: hex_string_to_uint64
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint64_t hex_string_to_uint64 (uint8_t * uartRxBuffer)
{
    uint8_t  firstPart[9];
    uint8_t  secondPart[9];
    uint8_t  i;
    uint32_t firstPartConv;
    uint32_t secondPartConv;

    /* Prepare buffer */
    firstPart[8] = '\0';
    secondPart[8] = '\0';

    /* Copy to structure */
    for (i = 0; i < 8; i++)
    {
        firstPart[i] = uartRxBuffer[i + 2];
        secondPart[i] = uartRxBuffer[i + 10];
    }

    /* Convert to integer */
    firstPartConv  = strtoul ((char const *)firstPart, NULL, 16);
    secondPartConv = strtoul ((char const *)secondPart, NULL, 16);

    return (((uint64_t)firstPartConv) << 32) + (uint64_t)secondPartConv;
} /* hex_string_to_uint64 */
/******************************************************************************
   End of function  hex_string_to_uint64
******************************************************************************/

/******************************************************************************
* Function Name: hex_string_128bit_to_uint8_arr
* Description :
* Arguments : uartRxBuffer consists of 34 uint8_t elements and starts with '0x'
* Return Value :
******************************************************************************/
void hex_string_128bit_to_uint8_arr (uint8_t * uartRxBuffer, uint8_t * parr)
{
    uint8_t i, j;
    uint8_t tempStr[11];
    uint32_t tempVal;
    if (NULL == parr)
    {
        return;
    }
    for (i=0; i<4; ++i)
    {
        tempStr[0] = '0';
        tempStr[1] = 'x';
        tempStr[10] = '\0';
        for (j=0; j<8; ++j)
        {
            tempStr[j+2] = uartRxBuffer[i*8+2 + j];
        }

        tempVal = hex_string_to_uint32(tempStr);

        R_BYTE_UInt32ToArr(tempVal, parr+i*4);
    }
} /* hex_string_128bit_to_uint8_arr */
/******************************************************************************
   End of function  hex_string_128bit_to_uint8_arr
******************************************************************************/


/******************************************************************************
* Function Name: R_DEMO_SoftReset
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_DEMO_SoftReset (void)
{
    R_BSP_SoftReset ();
}
/******************************************************************************
   End of function  R_DEMO_SoftReset
******************************************************************************/


/******************************************************************************
* Function Name: R_DEMO_HndFatalError
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_DEMO_HndFatalError (void)
{
    uint8_t buffer[1];

#ifdef __PLC__DEBUG__
    R_STDIO_Printf ("\nOperation failed. -> Press Enter to reset device!");
#endif
    R_DEMO_LED(R_DEMO_G3_USE_SECONDARY_CH, R_DEMO_LED_ALERT);
    R_STDIO_Gets ((char *)buffer);
    R_DEMO_SoftReset ();
}
/******************************************************************************
   End of function R_DEMO_HndFatalError
******************************************************************************/

/******************************************************************************
* Function Name: R_DEMO_HndWaitEnter
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_DEMO_HndWaitEnter(void)
{
    uint8_t buffer[2];
#ifdef __PLC__DEBUG__
    R_STDIO_Printf("\n-----------------Press Enter to continue---------------------");
#endif
    R_STDIO_Gets((char *)buffer);
}
/******************************************************************************
End of function R_DEMO_HndWaitEnter
******************************************************************************/

/******************************************************************************
* Function Name: R_DEMO_LED
* Description :
* Arguments :
* Return Value :
******************************************************************************/
#if  defined (R_DEFINE_OPEN_RX)
void R_DEMO_LED (uint8_t ch_id, r_demo_led_mode_t mode)
{
    UNUSED (ch_id);
    UNUSED (mode);
}
#else
void R_DEMO_LED (uint8_t ch_id, r_demo_led_mode_t mode)
{
    switch (mode)
    {
        case R_DEMO_LED_STOP:
            R_LED_Stop (ch_id);
            break;

        case R_DEMO_LED_ALERT:
            R_LED_Start (ch_id, 100u);
            break;

        case R_DEMO_LED_BOOT:
            R_LED_Start (ch_id, 1000u);
            break;

        case R_DEMO_LED_IDLE:
            R_LED_Start (ch_id, 0u);
            break;

        default:
            break;
    } /* switch */

    return;
} /* R_DEMO_LED */
#endif /* ifdef R_SYNERGY_PLC */
/******************************************************************************
End of function  R_DEMO_LED
******************************************************************************/


