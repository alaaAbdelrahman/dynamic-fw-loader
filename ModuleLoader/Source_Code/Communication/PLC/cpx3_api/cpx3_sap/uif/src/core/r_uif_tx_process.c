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
* File Name   : r_uif_tx_process.c
*    @version
*        $Rev: 8957 $
*    @last editor
*        $Author: a5034286 $
*    @date
*        $Date:: 2022-03-08 18:38:35 +0900#$
* Description :
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_uif_tx_process.h"


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
static void uif_tx_rcv_process (r_uif_tx_core_buff_t * ptx);
static r_sys_status_t uif_tx_send_process (r_uif_tx_core_buff_t * ptx);


/*===========================================================================*/
/* FUNCTION declared                                                         */
/*===========================================================================*/

/******************************************************************************
* Function Name:R_UIF_TxReqMain
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_sys_status_t R_UIF_TxReqMain (uint8_t port_id, uint8_t src_type_id, uint32_t tx_length, uint8_t * ptx_data, uint8_t secFlg)
{
    r_uif_tx_core_buff_t * ptx = uif_tx_get_core_buff_p (port_id);
    r_uif_lbuff_tx_t *     ptx_lbuff;
    uint16_t               length;

    if (
        ((tx_length <= R_UIF_COMMAND_LEN_MIN) ||
         (NULL == ptx)) ||
        ((NULL == ptx_data) ||
         (tx_length > 0xFFFFu))
        )
    {
        R_UIF_STATS_INC (ptx, R_UIF_STATS_TX_IN_RCV_ERROR);
        return R_SYS_STATUS_INVALID_PARAMETER;
    }

    ptx_lbuff = (r_uif_lbuff_tx_t *)uif_cmn_get_lbuff ((r_uif_lbuff_t *)&ptx->data.tx_lbuff, R_UIF_LBUFF_INVALID);

    if (NULL == ptx_lbuff)
    {
        return R_SYS_STATUS_OUT_OF_CAPS;
    }

    length    = uif_hdlc_enc (ptx->port_id, &ptx->data.tx_key.key, ptx_data, (uint16_t)tx_length, src_type_id, (uint8_t *)ptx_lbuff->lbuff_long, secFlg);
    if (length)
    {
        ptx_lbuff->cmn.phead               = (uint8_t *)ptx_lbuff->lbuff_long;
        ptx_lbuff->cmn.length              = length;
        ptx_lbuff->cmn.type_id             = src_type_id;
        ptx_lbuff->cmn.lbuff_state         = R_UIF_LBUFF_VALID;
        R_UIF_STATS_INC (ptx, R_UIF_STATS_TX_IN_RCV_COMMAND);
        return R_SYS_STATUS_SUCCESS;
    }

    return R_SYS_STATUS_CRC_ERROR;


} /* R_UIF_TxReqMain */
/******************************************************************************
   End of function  R_UIF_TxReqMain
******************************************************************************/

/******************************************************************************
* Function Name:R_UIF_TxCyclicMain
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_sys_status_t R_UIF_TxCyclicMain (uint8_t port_id)
{
    r_uif_tx_core_buff_t * ptx = uif_tx_get_core_buff_p (port_id);

    if (NULL == ptx)
    {
        return R_SYS_STATUS_INVALID_PARAMETER;
    }


    uif_tx_rcv_process (ptx);

    return uif_tx_send_process (ptx);
}
/******************************************************************************
   End of function  R_UIF_TxCyclicMain
******************************************************************************/

/******************************************************************************
* Function Name:uif_tx_rcv_process
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void uif_tx_rcv_process (r_uif_tx_core_buff_t * ptx)
{
    r_uif_lbuff_tx_t *         ptx_lbuff;
    r_uif_tx_ringbuff_info_t * ptx_buff;
    uint16_t                   free_len;
    uint16_t                   i;
    r_uif_core_buff_t *        pcore;
    uint16_t                   lbuff_len;

    if (NULL == ptx)
    {
        return;
    }
    ptx_buff  = &ptx->data.tx_rbuff_info;
    if (0u == ptx_buff->uif_buff_len)
    {
        return;
    }

    ptx_lbuff = (r_uif_lbuff_tx_t *)uif_cmn_get_lbuff ((r_uif_lbuff_t *)&ptx->data.tx_lbuff, R_UIF_LBUFF_VALID);
    if (NULL == ptx_lbuff)
    {
        return;
    }

    free_len  = GET_FREE_LEN (ptx_buff->uif_sync_wp, ptx_buff->uif_sync_rp, ptx_buff->uif_buff_len);

    if (free_len > ptx_lbuff->cmn.length)
    {
        uint8_t * pi = (uint8_t *)ptx_lbuff->lbuff_long;
        uint8_t * po = ptx_buff->psync_buff;
        uint16_t  rlen;
        uint16_t  wp;
        wp   = ptx_buff->uif_sync_wp;
        rlen = ptx_buff->uif_buff_len;
        if (NULL != po)
        {
            for (i = 0u; i < ptx_lbuff->cmn.length; i++)
            {
                po[wp] = *(pi++);
                wp     = FWD_PTR (wp, 1u, rlen);
            }
            ptx_buff->uif_sync_wp = wp;
            pcore = uif_get_core_buff_p (ptx->port_id);
            lbuff_len = (R_UIF_HDLC_HEADER_LEN + (pcore->uif_command_len_max * 2) + 3) & 0xFFFCu;
            R_memset (&ptx_lbuff->cmn, 0, sizeof(r_uif_lbuff_cmn_t)); /* ptx_lbuff->cmn.lbuff_state = R_UIF_LBUFF_INVALID */
            R_memset (ptx_lbuff->lbuff_long, 0, lbuff_len);
        }
    }

    return;
} /* uif_tx_rcv_process */
/******************************************************************************
   End of function  uif_tx_rcv_process
******************************************************************************/

/******************************************************************************
* Function Name:uif_tx_send_process
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static r_sys_status_t uif_tx_send_process (r_uif_tx_core_buff_t * ptx)
{
    r_uif_tx_ringbuff_info_t * ptx_buff;
    r_uif_lbuff_tx_t *         ptx_lbuff;
    uint16_t                   lin_valid_len;
    r_sys_status_t             status = R_SYS_STATUS_SUCCESS;

    if (NULL == ptx)
    {
        return R_SYS_STATUS_INVALID_PARAMETER;
    }

    ptx_buff = &ptx->data.tx_rbuff_info;

    if (R_UIF_UART_ACTIVE == ptx_buff->uart_info.state)
    {
        if (R_UIF_ChkTimeoutMsec (ptx_buff->uart_info.uart_valid_time, ptx->uifTxTtlMsec) == R_FALSE)
        {
            return R_SYS_STATUS_INVALID_STATUS;
        }
        R_UIF_STATS_INC (ptx, R_UIF_STATS_TX_OUT_TIMEOUT);
        uif_tx_release_sendbuff (ptx);
        ptx_buff->uart_info.state = R_UIF_UART_FREE;
    }
    else if (R_UIF_UART_BUSY == ptx_buff->uart_info.state)
    {
        if (R_UIF_ChkTimeoutMsec (ptx_buff->uart_info.uart_valid_time, R_UIF_TX_UART_TTL_BUSYWAIT_MSEC) == R_FALSE)
        {
            return R_SYS_STATUS_INVALID_STATUS;
        }
        ptx_buff->uart_info.state = R_UIF_UART_FREE;
    }
    else
    {
        /* Do nothing */
    }

    if (0u == ptx_buff->uif_buff_len)
    {
        /* Liner buff mode */
        ptx_lbuff = (r_uif_lbuff_tx_t *)uif_cmn_get_lbuff ((r_uif_lbuff_t *)&ptx->data.tx_lbuff, R_UIF_LBUFF_VALID);
        if (NULL == ptx_lbuff)
        {
            return R_SYS_STATUS_OUT_OF_CAPS;
        }
        else if (ptx_lbuff->cmn.length)
        {
            ptx_lbuff->cmn.lbuff_state          = R_UIF_LBUFF_WAIT_RELEASE;
            ptx_buff->uart_info.state           = R_UIF_UART_ACTIVE;
            status = R_UIF_DrvUartSend (ptx->port_id, (uint8_t *)ptx_lbuff->lbuff_long, ptx_lbuff->cmn.length);

            if (R_SYS_STATUS_SUCCESS == status)
            {
                ptx_buff->uart_info.uart_valid_time = R_UIF_GetCurrentTimeMsec ();
                R_UIF_STATS_INC (ptx, R_UIF_STATS_TX_OUT_COMMAND_REQD);
            }
            else
            {
                uif_tx_release_sendbuff(ptx);
		        ptx_buff->uart_info.state = R_UIF_UART_FREE;
                R_UIF_STATS_INC (ptx, R_UIF_STATS_TX_DRV_ERROR);
            }
        }
        else
        {
            /* Do Nothing */
        }
    }
    else
    {
        /* Ring buff mode */
        lin_valid_len = GET_VALID_LEN_linear (ptx_buff->uif_sync_wp, ptx_buff->uif_sync_rp, ptx_buff->uif_buff_len);

        if (lin_valid_len)
        {
            ptx_buff->uart_info.state           = R_UIF_UART_ACTIVE;
            status = R_UIF_DrvUartSend (ptx->port_id, &ptx_buff->psync_buff[ptx_buff->uif_sync_rp], lin_valid_len);

            if (R_SYS_STATUS_SUCCESS == status)
            {
                ptx_buff->uif_sync_rp_pre           = FWD_PTR (ptx_buff->uif_sync_rp_pre, lin_valid_len, ptx_buff->uif_buff_len);
                ptx_buff->uart_info.uart_valid_time = R_UIF_GetCurrentTimeMsec ();
                R_UIF_STATS_INC (ptx, R_UIF_STATS_TX_OUT_COMMAND_REQD);
            }
            else
            {
                ptx_buff->uart_info.state           = R_UIF_UART_BUSY;
                ptx_buff->uart_info.uart_valid_time = R_UIF_GetCurrentTimeMsec ();
                R_UIF_STATS_INC (ptx, R_UIF_STATS_TX_DRV_ERROR);
            }

        }
    }

    return status;
} /* uif_tx_send_process */
/******************************************************************************
   End of function  uif_tx_send_process
******************************************************************************/

/******************************************************************************
* Function Name:uif_tx_release_sendbuff
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void uif_tx_release_sendbuff (r_uif_tx_core_buff_t * ptx)
{
    r_uif_tx_ringbuff_info_t * ptx_buff;

    if (NULL == ptx)
    {
        return;
    }

    ptx_buff = &ptx->data.tx_rbuff_info;
    if (0u == ptx_buff->uif_buff_len)
    {
        ptx->data.tx_lbuff.cmn.lbuff_state = R_UIF_LBUFF_INVALID;
    }
    else
    {
        ptx_buff->uif_sync_rp = ptx_buff->uif_sync_rp_pre;
    }
    return;
} /* uif_tx_release_sendbuff */
/******************************************************************************
   End of function  uif_tx_release_sendbuff
******************************************************************************/

