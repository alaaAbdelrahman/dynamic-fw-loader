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
 * File Name : r_c3sap_sys_depend.c
 * Description : System layer API (Environment dependent part)
 ******************************************************************************/

/*===========================================================================*/
/* Includes                                                                  */
/*===========================================================================*/
#include "r_typedefs.h"
#include "r_config.h"
#include "r_bsp_api.h"
#include "r_constants.h"
#include "r_crc32_api.h"
#include "r_fw_download.h"
#include "r_sys_sap.h"
#include "r_uif_sap_if.h"
#include "r_c3sap_sys.h"
#include "r_c3sap_sys_depend.h"
#include "r_c3sap_sys_if.h"
#include "r_c3sap_g3_if.h"
#include "r_c3sap_sys_thread.h"
#include "r_app_timer.h"
volatile uint8_t T1_SemaphoreWait = 0;
/*===========================================================================*/
/* Macro definitions                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Typedef definitions                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Variables                                                                 */
/*===========================================================================*/
static const uint32_t r_sys_baud_table[] = 
{
    9600,
    19200,
    38400,
    57600,
    115200,
    230400,
    300000,
    375000,
    460800,
    500000,
    750000,
    937500,
    1000000
};

static r_result_t   sys_timer_status;
static volatile uint32_t sys_signal=0uL;


/*===========================================================================*/
/* Function prototype                                                        */
/*===========================================================================*/
static r_uif_baud_t sys_convert_baud(uint8_t sys_baud);

/*===========================================================================*/
/* Functions                                                                 */
/*===========================================================================*/
/******************************************************************************
* Function Name:R_SYS_Init
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_SYS_Init(r_sys_boot_info_t *pboot_info, r_sys_callback_t *pcallbacks)
{
    r_result_t status = R_RESULT_SUCCESS;
    
    if ( NULL == pboot_info )
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    
    if ( (R_SYS_BAUD_MAX <= pboot_info->dl_baud) || (R_SYS_BAUD_MAX <= pboot_info->cmd_baud) )
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    
    sys_init_sub(pcallbacks);
    
    R_UIF_Deinit(R_C3SAP_BLOCKID_UARTIF_0);
    
    R_CRC_InitCRC32();
    R_THREAD_Deinit();
    R_THREAD_Init();
    
    sys_clear_signal( R_SYS_SIGNAL_CONFIRM );


    if(NULL != pboot_info->p_fw_plc)
    {
        //status = R_FW_Download(pboot_info);
    }
    else
    {
        //R_BSP_Cpx3Reset();
    }
    if ( R_RESULT_SUCCESS == status )
    {
        status = R_UIF_Init( R_C3SAP_BLOCKID_UARTIF_0, sys_convert_baud(pboot_info->cmd_baud) );
    }
    
    if ( R_RESULT_SUCCESS == status )
    {
        /* wait boot up indication */
        //status = sys_wait_signal( R_SYS_SIGNAL_CONFIRM, pboot_info->dl_timeout );
    }
    
    return status;
}
/******************************************************************************
   End of function  R_SYS_Init
******************************************************************************/

/******************************************************************************
* Function Name:R_SYS_Deinit
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_SYS_Deinit(void)
{
    r_result_t ret;
    
    R_THREAD_Deinit();
    
    ret = R_UIF_Deinit(R_C3SAP_BLOCKID_UARTIF_0);
    sys_deinit_sub();
    
    return ret;

}
/******************************************************************************
   End of function  R_SYS_Deinit
******************************************************************************/


/******************************************************************************
* Function Name:R_SYS_SendCmd
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_SYS_SendCmd(st_sys_send_cmd_t *p_cmd)
{
    r_result_t ret;
    
    /* Check Parameters */
    ret = sys_check_block_id(p_cmd->dst_block_id);
    
    if (R_RESULT_SUCCESS == ret)
    {
        ret = sys_check_block_id(p_cmd->src_block_id);
    }
    
    if ( NULL == p_cmd->pbuffer )
    {
        ret = R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    
    if (R_RESULT_SUCCESS == ret)
    {
        switch (p_cmd->dst_block_id)
        {
            case R_C3SAP_BLOCKID_UARTIF_0:
            case R_C3SAP_BLOCKID_UARTIF_1:
                if( R_UIF_GetTxState(p_cmd->dst_block_id) == R_UIF_STATE_IDLE )
                {
                    ret = R_UIF_SendCmd(p_cmd->dst_block_id, p_cmd->src_block_id, p_cmd->pbuffer, p_cmd->size ,p_cmd->reserved);
                }
                else
                {
                    ret = R_RESULT_FAILED;
                }
                break;
            default:
            case R_C3SAP_BLOCKID_SYSTEM:
            case R_C3SAP_BLOCKID_G3:
            case R_C3SAP_BLOCKID_PRIME:
                break;
        }
    }
    
    return ret;
}
/******************************************************************************
   End of function  R_SYS_SendCmd
******************************************************************************/

/******************************************************************************
* Function Name:R_SYS_ReceiveCmd
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_SYS_ReceiveCmd(st_sys_recv_cmd_t *p_cmd)
{
    r_result_t ret;

    /* Check Parameters */
    ret = sys_check_block_id(p_cmd->dst_block_id);
    
    if ( R_RESULT_SUCCESS == ret )
    {
        ret = sys_check_block_id(p_cmd->src_block_id);
    }
    
    if ( NULL == p_cmd->pbuffer )
    {
        ret = R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    
    if ( R_RESULT_SUCCESS == ret )
    {
        switch( p_cmd->dst_block_id )
        {
            case R_C3SAP_BLOCKID_SYSTEM:
                {
                    sys_base_cb( p_cmd->pbuffer );
                }
                ret = R_RESULT_SUCCESS;
                break;
            case R_C3SAP_BLOCKID_G3:
                ret = R_G3CTRL_ReceiveCmd( p_cmd->pbuffer );
                break;
            default:
                ret = R_RESULT_BAD_INPUT_ARGUMENTS;
                break;
        }
    }
    
    return ret;
}
/******************************************************************************
   End of function  R_SYS_ReceiveCmd
******************************************************************************/

/******************************************************************************
* Function Name:R_SYS_GetSystemTimeMsec
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_SYS_GetSystemTimeMsec(uint32_t *psys_time)
{
    r_result_t ret = R_RESULT_SUCCESS;
    
    /* Check Parameters */
    if (R_TIMER_GetExpireTickCount(psys_time, 0u) != R_RESULT_SUCCESS)
    {
        ret = R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    return ret;
}
/******************************************************************************
   End of function  R_SYS_GetSystemTimeMsec
******************************************************************************/


/******************************************************************************
* Function Name:R_SYS_GetDirectBaud
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
uint32_t R_SYS_GetDirectBaud(uint8_t baud)
{
    return r_sys_baud_table[baud];
}
/******************************************************************************
   End of function  R_SYS_GetDirectBaud
******************************************************************************/

/******************************************************************************
* Function Name:R_SYS_TimeOutEvent
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
void R_SYS_TimeOutEvent(uint8_t handle)
{
    UNUSED(handle);
    
    sys_timer_status = R_RESULT_TIMEOUT;
    sys_set_signal(R_SYS_SIGNAL_CONFIRM);

}
/******************************************************************************
   End of function  R_SYS_TimeOutEvent
******************************************************************************/

/******************************************************************************
* Function Name:R_SYS_SemaphoreWait
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_SYS_SemaphoreWait(void)
{
    T1_SemaphoreWait = 1;
    /* Do Nothing */
    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_SYS_SemaphoreWait
******************************************************************************/

/******************************************************************************
* Function Name:R_SYS_SemaphoreRelease
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
void R_SYS_SemaphoreRelease(void)
{
  T1_SemaphoreWait = 0;
    /* Do Nothing */
}
/******************************************************************************
   End of function  R_SYS_SemaphoreRelease
******************************************************************************/

/******************************************************************************
* Function Name:sys_set_signal
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t sys_set_signal( uint32_t signal )
{
    sys_signal |= signal;
    
    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  sys_set_signal
******************************************************************************/

/******************************************************************************
* Function Name:sys_clear_signal
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t sys_clear_signal( uint32_t signal )
{
    sys_signal &= (~signal);
    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  sys_clear_signal
******************************************************************************/

/******************************************************************************
* Function Name:sys_wait_signal
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t sys_wait_signal( uint32_t signal, uint32_t time_out )
{
    sys_timer_status = R_RESULT_SUCCESS;
    
    R_TIMER_TimerOneShotOn( R_TIMER_ID_CPX,time_out, R_HANDLE_SYS_REQUEST );
    
    while( (sys_signal & signal) == 0uL )
    {
        R_BSP_NOP();
    }
    
    R_TIMER_TimerOneShotOff( R_TIMER_ID_CPX, R_HANDLE_SYS_REQUEST );
    
    return sys_timer_status;
}
/******************************************************************************
   End of function  sys_wait_signal
******************************************************************************/

/******************************************************************************
* Function Name:sys_convert_baud
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
static r_uif_baud_t sys_convert_baud(uint8_t sys_baud)
{
    r_uif_baud_t uif_baud;
    
    switch(sys_baud)
    {
#ifdef R_SYS_BAUD_9600
    case  R_SYS_BAUD_9600:
        uif_baud = R_UIF_BAUD_9600;
        break;
#endif
#ifdef R_SYS_BAUD_19200
    case  R_SYS_BAUD_19200:
        uif_baud = R_UIF_BAUD_19200;
        break;
#endif
#ifdef R_SYS_BAUD_38400
    case  R_SYS_BAUD_38400:
        uif_baud = R_UIF_BAUD_38400;
        break;
#endif
#ifdef R_SYS_BAUD_57600
    case  R_SYS_BAUD_57600:
        uif_baud = R_UIF_BAUD_57600;
        break;
#endif
#ifdef R_SYS_BAUD_115200
    case  R_SYS_BAUD_115200:
        uif_baud = R_UIF_BAUD_115200;
        break;
#endif
#ifdef R_SYS_BAUD_230400
    case  R_SYS_BAUD_230400:
        uif_baud = R_UIF_BAUD_230400;
        break;
#endif
#ifdef R_SYS_BAUD_300000
    case  R_SYS_BAUD_300000:
        uif_baud = R_UIF_BAUD_300000;
        break;
#endif
#ifdef R_SYS_BAUD_375000
    case  R_SYS_BAUD_375000:
        uif_baud = R_UIF_BAUD_375000;
        break;
#endif
#ifdef R_SYS_BAUD_460800
    case  R_SYS_BAUD_460800:
        uif_baud = R_UIF_BAUD_460800;
        break;
#endif
#ifdef R_SYS_BAUD_500000
    case  R_SYS_BAUD_500000:
        uif_baud = R_UIF_BAUD_500000;
        break;
#endif
#ifdef R_SYS_BAUD_750000
    case  R_SYS_BAUD_750000:
        uif_baud = R_UIF_BAUD_750000;
        break;
#endif
#ifdef R_SYS_BAUD_937500
    case  R_SYS_BAUD_937500:
        uif_baud = R_UIF_BAUD_937500;
        break;
#endif
#ifdef R_SYS_BAUD_1000000
    case  R_SYS_BAUD_1000000:
        uif_baud = R_UIF_BAUD_1000000;
        break;
#endif
    default:
        //uif_baud = R_UIF_BAUD_MAX;
        break;
    }
    
    return uif_baud;
}
/******************************************************************************
   End of function  sys_convert_baud
******************************************************************************/



