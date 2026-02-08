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
* File Name   : r_uif_data_manage.c
*    @version
*        $Rev: 3635 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2017-12-08 14:18:23 +0900#$
* Description :
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_uif_base.h"
#include "r_uif_buff_declared.h"
#include "r_uif_thread_if.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
* Function Name:uif_cmn_get_lbuff
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void *uif_cmn_get_lbuff (r_uif_lbuff_t * plbuff, r_uif_lbuff_state_t lbuff_state)
{
    void * pret = NULL;

    if ((NULL != plbuff) && (plbuff->cmn.lbuff_state == lbuff_state))
    {
        pret = plbuff;
    }
    return pret;
}
/******************************************************************************
   End of function  uif_cmn_get_lbuff
******************************************************************************/

/******************************************************************************
* Function Name:uif_cmn_release_lbuff
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_sys_status_t uif_cmn_release_lbuff (r_uif_lbuff_t * plbuff, uint8_t src_type_id, uint8_t * paddress, uint16_t length)
{
    if (((NULL                      != plbuff) &&
         ((R_UIF_LBUFF_WAIT_RELEASE == plbuff->cmn.lbuff_state) &&
          (src_type_id              == plbuff->cmn.type_id))) &&
        ((paddress                  == plbuff->cmn.phead) &&
         (length                     == plbuff->cmn.length))
        )
    {
        plbuff->cmn.lbuff_state = R_UIF_LBUFF_INVALID;
        return R_SYS_STATUS_SUCCESS;
    }
    else
    {
        return R_SYS_STATUS_INVALID_PARAMETER;
    }
}
/******************************************************************************
   End of function  uif_cmn_release_lbuff
******************************************************************************/

/******************************************************************************
* Function Name:uif_rx_receive_cmd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_sys_status_t uif_rx_receive_cmd (uint8_t port_id, r_uif_lbuff_t * plbuff, uint8_t type_id, uint8_t * pbuffer, uint16_t size)
{
    r_sys_status_t ret;
    if (
        ((NULL == plbuff) ||
         (NULL == pbuffer)) ||
        (ZERO == size)
        )
    {
        return R_SYS_STATUS_INVALID_PARAMETER;
    }
    ret = R_UIF_ReceiveCmd (port_id, type_id, pbuffer, size);
    if (R_SYS_STATUS_SUCCESS == ret)
    {
        plbuff->cmn.phead               = pbuffer;
        plbuff->cmn.length              = size;
        plbuff->cmn.type_id             = type_id;
        plbuff->cmn.lbuff_state         = R_UIF_LBUFF_WAIT_RELEASE;
    }
    return ret;
} /* uif_rx_receive_cmd */
/******************************************************************************
   End of function  uif_rx_receive_cmd
******************************************************************************/

/******************************************************************************
* Function Name:R_UIF_GetCurrentTimeMsec
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint32_t R_UIF_GetCurrentTimeMsec (void)
{
    uint32_t now = 0uL;
    R_SYS_GetSystemTimeMsec (&now);
    return now;
}
/******************************************************************************
   End of function  R_UIF_GetCurrentTimeMsec
******************************************************************************/

/******************************************************************************
* Function Name:R_UIF_ChkTimeoutMsec
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint8_t R_UIF_ChkTimeoutMsec (uint32_t valid_time_msec, uint32_t ttl_msec)
{
    uint32_t now = 0uL;
    uint64_t res;
    R_SYS_GetSystemTimeMsec (&now);

    res = now;
    if (now < valid_time_msec)
    {
        res = (uint64_t)(res + 0x100000000uLL);
    }
    if ((res - valid_time_msec) < ttl_msec)
    {
        return R_FALSE;
    }
    return R_TRUE;
}
/******************************************************************************
   End of function  R_UIF_ChkTimeoutMsec
******************************************************************************/


/******************************************************************************
* Function Name:R_UIF_InitStatistics
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_UIF_InitStatistics (r_uif_stats_info_t * pinfo)
{
    if (
        ((NULL == pinfo) ||
         (NULL == pinfo->pstart)) ||
        (pinfo->length > R_UIF_MAX_STATS_LENGTH)
        )
    {
        return;
    }

    R_memset (pinfo->pstart, 0, pinfo->length);

    return;
}
/******************************************************************************
   End of function  R_UIF_InitStatistics
******************************************************************************/

/******************************************************************************
* Function Name:R_UIF_StatisticsIncrement
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_UIF_StatisticsIncrement (r_uif_stats_info_t ** ppinfo, uint8_t index, uint16_t offset)
{
    r_uif_stats_info_t * pinfo;

    if ((NULL == ppinfo) || (index >= R_UIF_STATS_GROUP_END))
    {
        return;
    }

    pinfo = &(*(r_uif_stats_info_t **)ppinfo)[index];
    if (
        ((NULL == pinfo) ||
         (NULL == pinfo->pstart)) ||
        ((pinfo->length > R_UIF_MAX_STATS_LENGTH) ||
         (offset >= (pinfo->length >> 2)))
        )
    {
        return;
    }

    pinfo->pstart[offset]++;

    return;
} /* R_UIF_StatisticsIncrement */
/******************************************************************************
   End of function  R_UIF_StatisticsIncrement
******************************************************************************/


