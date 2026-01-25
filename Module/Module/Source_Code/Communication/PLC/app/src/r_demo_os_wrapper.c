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
 * Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/

/*******************************************************************************
 * File Name   : r_demo_os_wrapper.c
 *    @version
 *        $Rev: 2685 $
 *    @last editor
 *        $Author: troba $
 *    @date
 *        $Date:: 2017-10-23 23:13:54 +0900#$
 * Description :
 ******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include "r_typedefs.h"
#include "r_bsp_api.h"

/* g3 part */
#include "r_c3sap_api.h"

#include "r_demo_app.h"

#include "r_app_timer.h"
#include "r_demo_app_thread.h"

extern uint8_t config_monitor;
extern uint8_t cpx3_current_task;

extern uint8_t current_AppSetIpContent;
/******************************************************************************
 * Function Name: R_DEMO_ClearEventFlag
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_ClearEventFlag(volatile void *flags, uint32_t id)
{
    (*(uint32_t *)flags) &= (~id);
} /* R_DEMO_ClearEventFlag */
/******************************************************************************
   End of function  R_DEMO_ClearEventFlag
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_SetEventFlag
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_SetEventFlag(volatile void *flags, uint32_t id)
{
    (*(volatile uint32_t *)flags) |= id;
} /* R_DEMO_SetEventFlag */
/******************************************************************************
   End of function  R_DEMO_SetEventFlag
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_WaitEventFlag
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_WaitEventFlag(volatile void *flags, uint32_t id)
{
    r_result_t result;

    result = R_RESULT_SUCCESS;

    while (((*(volatile uint32_t *)flags) & id) == 0)
    {
        /* wait */
        if (R_TRUE == R_DEMO_WaitcnfTimeout())
        {
            /* time out detect (This function is available only for R_DEMO_AdpdData request) */
            result = R_RESULT_TIMEOUT;
            R_DEMO_ClearcnfTimeout();
            break;
        }
        // R_BSP_NOP();
    }
    (*(volatile uint32_t *)flags) &= (~id);

    return result;

} /* R_DEMO_ClearEventFlag */
/****************************************************************************** */
r_result_t CPX3_MonitorEventFlag(volatile void *flags, uint32_t id)
{
    r_result_t result = R_RESULT_SUCCESS;

    // BUSY
    if (((*(volatile uint32_t *)flags) & id) == 0)
    {
        result = R_RESULT_DEVICE_BUSY;
    }
    // FAIL
    else if (R_TRUE == R_DEMO_WaitcnfTimeout())
    {
        // TODO: Handle
        /* time out detect (This function is available only for R_DEMO_AdpdData request) */
        result = R_RESULT_TIMEOUT;
        R_DEMO_ClearcnfTimeout();
        config_monitor = 0;
        cpx3_current_task = CPX3_BOOTING_TASK;
    }
    // SUCCESS
    else
    {
        result = R_RESULT_SUCCESS;
        config_monitor = 0;
    }

    // (*(volatile uint32_t *)flags) &= (~id);

    return result;
}

/****************************************************************************** */

/******************************************************************************
   End of function  R_DEMO_WaitEventFlag
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_BusyWait
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_BusyWait(uint32_t time)
{
    R_TIMER_BusyWait(time);
} /* R_DEMO_BusyWait */
/******************************************************************************
   End of function  R_DEMO_BusyWait
******************************************************************************/
