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
 * File Name : r_c3sap_g3_depend.c
 * Description : G3 layer API (Environment dependent part)
 ******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "r_constants.h"
#include "r_config.h"
#include "r_bsp_api.h"
#include "r_c3sap_g3.h"
#include "r_c3sap_g3_if.h"
#include "r_c3sap_g3_depend.h"
#include "r_c3sap_sys_if.h"

#include "r_app_timer.h"
#include "../../cpx3_fw_new_v1/cpx3_switch.h"
extern uint8_t flag_G3_Init_Send;
extern uint8_t cpx3_current_task;
extern uint8_t start_config_set;
/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static r_result_t g3ctrl_timer_status = R_RESULT_SUCCESS;

uint32_t g3ctrl_signal = 0uL;

/*===========================================================================*/
/* Functions                                                                 */
/*===========================================================================*/

/******************************************************************************
 * Function Name:R_G3CTRL_SemaphoreWait
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_G3CTRL_SemaphoreWait(void)
{
    return R_SYS_SemaphoreWait();
}
/******************************************************************************
   End of function  R_G3CTRL_SemaphoreWait
******************************************************************************/

/******************************************************************************
 * Function Name:R_G3CTRL_SemaphoreRelease
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_G3CTRL_SemaphoreRelease(void)
{
    R_SYS_SemaphoreRelease();
}
/******************************************************************************
   End of function  R_G3CTRL_SemaphoreRelease
******************************************************************************/

/******************************************************************************
 * Function Name:R_G3CTRL_TimeOutEvent
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_G3CTRL_TimeOutEvent(uint8_t handle)
{
    UNUSED(handle);

    g3ctrl_timer_status = R_RESULT_TIMEOUT;
    g3ctrl_set_signal(R_G3CTRL_SIGNAL_CONFIRM);
}

/******************************************************************************
   End of function  R_G3CTRL_TimeOutEvent
******************************************************************************/

/******************************************************************************
 * Function Name:g3ctrl_os_res_init
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void g3ctrl_os_res_init(void)
{
    /* Do Nothing */
}
/******************************************************************************
   End of function  g3ctrl_os_res_init
******************************************************************************/

/******************************************************************************
 * Function Name:g3ctrl_os_res_deinit
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void g3ctrl_os_res_deinit(void)
{
    /* Do Nothing */
}
/******************************************************************************
   End of function  g3ctrl_os_res_deinit
******************************************************************************/

/******************************************************************************
 * Function Name:g3ctrl_clear_signal
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t g3ctrl_clear_signal(uint32_t signal)
{
    g3ctrl_signal &= (~signal);
    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  g3ctrl_clear_signal
******************************************************************************/

/******************************************************************************
 * Function Name:g3ctrl_set_signal
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t g3ctrl_set_signal(uint32_t signal)
{
    g3ctrl_signal |= signal;
    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  g3ctrl_set_signal
******************************************************************************/

/******************************************************************************
 * Function Name:g3ctrl_wait_signal
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t g3ctrl_wait_signal(uint32_t signal, uint32_t time_out)
{
    g3ctrl_timer_status = R_RESULT_SUCCESS;

    /* Start Timeout Timer */
    R_TIMER_TimerOneShotOn(R_TIMER_ID_CPX, time_out, R_HANDLE_G3_REQUEST);

    while ((g3ctrl_signal & signal) == 0uL)
    {
        R_BSP_NOP();
    }

    R_TIMER_TimerOneShotOff(R_TIMER_ID_CPX, R_HANDLE_G3_REQUEST);

    return g3ctrl_timer_status;
} /* g3ctrl_wait_signal */
/******************************************************************************
   End of function  g3ctrl_wait_signal
******************************************************************************/

r_result_t monitor_SendingAdpInitCommands(uint32_t signal, uint32_t time_out)
{
    g3ctrl_timer_status = R_RESULT_SUCCESS;

    flag_G3_Init_Send = 0;
    /* Start Timeout Timer */
    R_TIMER_TimerOneShotOn(R_TIMER_ID_CPX, time_out, R_HANDLE_G3_REQUEST);

    if ((g3ctrl_signal & signal) == 0uL)
    { // BUSY

        g3ctrl_timer_status = R_RESULT_DEVICE_BUSY;
    }
    else
    {
        cpx3_current_task = CPX3_APP_RESET;
        start_config_set = 0;
        flag_G3_Init_Send = 1;
        g3ctrl_timer_status = R_RESULT_SUCCESS;
    }

    R_TIMER_TimerOneShotOff(R_TIMER_ID_CPX, R_HANDLE_G3_REQUEST);

    return g3ctrl_timer_status;
} /* g3ctrl_wait_signal */
/******************************************************************************
   End of function  g3ctrl_wait_signal
******************************************************************************/
