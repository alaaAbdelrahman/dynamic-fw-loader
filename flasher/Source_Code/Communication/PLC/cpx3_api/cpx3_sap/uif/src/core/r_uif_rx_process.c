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
* File Name   : r_uif_rx_process.c
*    @version
*        $Rev: 9082 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2022-03-14 10:22:21 +0900#$
* Description :
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_uif_rx_process.h"

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
static void uif_rxmem_flush (uint8_t port_id, r_uif_rx_ringbuff_info_t * prx_buff);
static void uif_rx_sync_process (r_uif_rx_core_buff_t * prx);
static r_sys_status_t uif_rx_hdlc_process (r_uif_rx_core_buff_t * prx);


/*===========================================================================*/
/* FUNCTION declared                                                         */
/*===========================================================================*/

/******************************************************************************
* Function Name:R_UIF_RxCyclicMain
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_sys_status_t R_UIF_RxCyclicMain (uint8_t port_id)
{
    r_uif_rx_core_buff_t * prx = uif_rx_get_core_buff_p (port_id);

    if (NULL == prx)
    {
        return R_SYS_STATUS_INVALID_PARAMETER;
    }


    uif_rx_sync_process (prx);

    uif_rx_hdlc_process (prx);


    return R_SYS_STATUS_SUCCESS;
} /* R_UIF_RxCyclicMain */
/******************************************************************************
   End of function  R_UIF_RxCyclicMain
******************************************************************************/

/******************************************************************************
* Function Name:R_UIF_CbRxReceiveCmd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_sys_status_t R_UIF_CbRxReceiveCmd (uint8_t port_id, uint8_t src_type_id, uint8_t * paddress, uint32_t length)
{
    r_uif_rx_core_buff_t * prx = uif_rx_get_core_buff_p (port_id);
    r_uif_lbuff_rx_t *     prx_lbuff;
    r_sys_status_t         ret = R_SYS_STATUS_INVALID_PARAMETER;

    if (
        (NULL == prx) ||
        (length > 0xFFFFu)
        )
    {
        return ret;
    }

    prx_lbuff = (r_uif_lbuff_rx_t *)uif_cmn_get_lbuff ((r_uif_lbuff_t *)&prx->data.rx_lbuff, R_UIF_LBUFF_WAIT_RELEASE);

    if (NULL != prx_lbuff)
    {
        ret = uif_cmn_release_lbuff ((r_uif_lbuff_t *)&prx->data.rx_lbuff, src_type_id, paddress, (uint16_t)length);
        if (R_SYS_STATUS_SUCCESS == ret)
        {
            R_UIF_STATS_INC (prx, R_UIF_STATS_RX_OUT_COMMAND_SUCCESS);
        }
    }

    return ret;
} /* R_UIF_CbRxReceiveCmd */
/******************************************************************************
   End of function  R_UIF_CbRxReceiveCmd
******************************************************************************/

/*===========================================================================*/
/* Sync Detect Process                                                       */
/*===========================================================================*/

/******************************************************************************
* Function Name:uif_rx_sync_process
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void uif_rx_sync_process (r_uif_rx_core_buff_t * prx)
{
    r_uif_rx_ringbuff_info_t * prx_buff;
    uint16_t                   i;
    uint16_t                   as_valid_len;
    uint16_t                   tmp_async_wp;
    uint16_t                   tmp_rp;
    uint8_t                    tmp_rd;
    r_uif_core_buff_t        * pcore;

    if (NULL == prx)
    {
        return;
    }

    prx_buff     = &prx->data.rx_rbuff_info;
    if (prx_buff->uif_sync_wp != prx_buff->uif_sync_rp)
    {
        return;
    }

    tmp_async_wp = prx_buff->uif_async_wp;
    as_valid_len = GET_VALID_LEN (tmp_async_wp, prx_buff->uif_sync_wp_pre, prx_buff->uif_buff_len);

    if (
        (0u == as_valid_len) &&
        (R_UIF_RXSTATE_SYNC == prx_buff->rx_state)
        )
    {
        if (R_UIF_ChkTimeoutMsec (prx->data.rx_rbuff_info.uif_rx_sync_valid_time, R_UIF_RX_UART_TTL_MSEC) == R_TRUE)
        {
            uif_rxmem_flush (prx->port_id, prx_buff);
            R_UIF_STATS_INC (prx, R_UIF_STATS_RX_IN_TIMEOUT);
            return;
        }
    }

    while (
        (as_valid_len) &&
        (prx_buff->uif_sync_wp == prx_buff->uif_sync_rp)
        )
    {
        tmp_rp = prx_buff->uif_sync_wp_pre;
        if (prx_buff->uif_async_full)
        {
            prx->data.rx_log.flush_cnt++;
            uif_rxmem_flush (prx->port_id, prx_buff);
            R_UIF_STATS_INC (prx, R_UIF_STATS_RX_IN_BUFF_OVERFLOW);
            break;
        }
        else if (R_UIF_RXSTATE_SYNC == prx_buff->rx_state)
        {
            if (R_UIF_ChkTimeoutMsec (prx->data.rx_rbuff_info.uif_rx_sync_valid_time, R_UIF_RX_UART_TTL_MSEC) == R_FALSE)
            {
                uint16_t pay_len = GET_VALID_LEN (prx_buff->uif_sync_wp_pre, prx_buff->uif_sync_wp, prx_buff->uif_buff_len);
                for (i = 0u; i < as_valid_len; i++)
                {
                    tmp_rd = prx_buff->psync_buff[tmp_rp];
                    pcore = uif_get_core_buff_p (prx->port_id);
                    if (R_UIF_HDLC_SCODE == tmp_rd)
                    {
                        if (
                            (((uint16_t)(pay_len + i) + 1u) >= R_UIF_HDLC_PACK_MIN) &&
                            (((uint16_t)(pay_len + i) + 1u) < (R_UIF_HDLC_HEADER_LEN + (pcore->uif_command_len_max * 2)))
                            )
                        {
                            /* Sync Detected */
                            prx_buff->rx_state    = R_UIF_RXSTATE_ASYNC;
                            tmp_rp                = FWD_PTR (tmp_rp, 1u, prx_buff->uif_buff_len);
                            prx_buff->uif_sync_wp = prx_buff->uif_sync_wp_pre = tmp_rp;
                        }
                        else
                        {
                            prx_buff->uif_sync_rp = prx_buff->uif_sync_wp = prx_buff->uif_sync_wp_pre = tmp_rp;
                            prx_buff->rx_state    = R_UIF_RXSTATE_ASYNC;
                            prx->data.rx_log.fwd_cnt++;
                            R_UIF_STATS_INC (prx, R_UIF_STATS_RX_IN_SYNC_BREAK);

                        }
                        break;
                    }
                    else if (((uint16_t)(pay_len + i) + 1u) >= (R_UIF_HDLC_HEADER_LEN + (pcore->uif_command_len_max * 2)))
                    {
                        prx_buff->uif_sync_rp = prx_buff->uif_sync_wp = prx_buff->uif_sync_wp_pre = tmp_rp;
                        prx_buff->rx_state    = R_UIF_RXSTATE_ASYNC;
                        R_UIF_STATS_INC (prx, R_UIF_STATS_RX_IN_LENGTH_ERROR);
                    }
                    else
                    {
                        /* Do Nothing */
                    }
                    tmp_rp = FWD_PTR (tmp_rp, 1u, prx_buff->uif_buff_len);
                }
                if (i == as_valid_len)
                {
                    prx_buff->uif_sync_wp_pre = tmp_rp;
                }
                prx->data.rx_rbuff_info.uif_rx_sync_valid_time = R_UIF_GetCurrentTimeMsec ();
            }
            else
            {
                /* Sync Broken */
                uif_rxmem_flush (prx->port_id, prx_buff);
                R_UIF_STATS_INC (prx, R_UIF_STATS_RX_IN_TIMEOUT);
            }
        }
        else
        {
            for (i = 0u; i < as_valid_len; i++)
            {
                tmp_rd = prx_buff->psync_buff[tmp_rp];
                if (R_UIF_HDLC_SCODE == tmp_rd)
                {
                    /* Sync Detected */
                    prx_buff->rx_state = R_UIF_RXSTATE_SYNC;
                    prx->data.rx_rbuff_info.uif_rx_sync_valid_time = R_UIF_GetCurrentTimeMsec ();
                    break;
                }
                R_UIF_STATS_INC (prx, R_UIF_STATS_RX_IN_DISCARD);
                tmp_rp = FWD_PTR (tmp_rp, 1u, prx_buff->uif_buff_len);
            }
            prx_buff->uif_sync_rp = prx_buff->uif_sync_wp = prx_buff->uif_sync_wp_pre = tmp_rp;
            if (R_UIF_RXSTATE_SYNC == prx_buff->rx_state)
            {
                prx_buff->uif_sync_wp_pre = FWD_PTR (prx_buff->uif_sync_wp_pre, 1u, prx_buff->uif_buff_len);
            }

        }
        as_valid_len = GET_VALID_LEN (tmp_async_wp, prx_buff->uif_sync_wp_pre, prx_buff->uif_buff_len);
    }
    return;
} /* uif_rx_sync_process */
/******************************************************************************
   End of function  uif_rx_sync_process
******************************************************************************/

/******************************************************************************
* Function Name:uif_rxmem_flush
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void uif_rxmem_flush (uint8_t port_id, r_uif_rx_ringbuff_info_t * prx_buff)
{
    UNUSED (port_id);

    if (NULL != prx_buff)
    {
        uint16_t tmp_async_wp = prx_buff->uif_async_wp;
        prx_buff->uif_sync_rp    = prx_buff->uif_sync_wp = prx_buff->uif_sync_wp_pre = tmp_async_wp;
        prx_buff->rx_state       = R_UIF_RXSTATE_ASYNC;
        prx_buff->uif_async_full = 0u;
    }
}
/******************************************************************************
   End of function  uif_rxmem_flush
******************************************************************************/

/*===========================================================================*/
/* HDLC Parse Process from RingBuffer                                        */
/*===========================================================================*/

/******************************************************************************
* Function Name:uif_rx_hdlc_process
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static r_sys_status_t uif_rx_hdlc_process (r_uif_rx_core_buff_t * prx)
{
    r_uif_rx_ringbuff_info_t * prx_buff;
    r_uif_lbuff_rx_t *         prx_lbuff;
    uint16_t                   valid_len;
    r_sys_status_t             ret;
    r_uif_core_buff_t        * pcore;

    if (NULL == prx)
    {
        return R_SYS_STATUS_INVALID_PARAMETER;
    }
    prx_lbuff = (r_uif_lbuff_rx_t *)uif_cmn_get_lbuff ((r_uif_lbuff_t *)&prx->data.rx_lbuff, R_UIF_LBUFF_INVALID);
    if (NULL == prx_lbuff)
    {
        return R_SYS_STATUS_OUT_OF_CAPS;
    }

    prx_buff  = &prx->data.rx_rbuff_info;
    valid_len = GET_VALID_LEN (prx_buff->uif_sync_wp, prx_buff->uif_sync_rp, prx_buff->uif_buff_len);

    if (valid_len)
    {
        uint8_t *           pwp      = (uint8_t *)prx_lbuff->lbuff_long;
        r_uif_hdlc_pack_t * phdlc    = (r_uif_hdlc_pack_t *)pwp;
        uint16_t            tmp_rp   = prx_buff->uif_sync_rp;
        uint16_t            length;
        uint8_t             prev_val = 0u;
        uint8_t             cur_val;

        if (
            (valid_len < R_UIF_HDLC_PACK_MIN) ||
            (R_UIF_HDLC_SCODE != prx_buff->psync_buff[tmp_rp])
            )
        {
            tmp_rp                    = FWD_PTR (tmp_rp, 1u, prx_buff->uif_buff_len);
            prx_buff->uif_sync_rp     = tmp_rp;
            prx_buff->uif_sync_wp     = tmp_rp;
            prx_buff->uif_sync_wp_pre = tmp_rp;
            prx->data.rx_log.triple_cnt++;
        }
        else
        {
            *(pwp++) = R_UIF_HDLC_SCODE;
            tmp_rp   = FWD_PTR (tmp_rp, 1u, prx_buff->uif_buff_len);
            pcore    = uif_get_core_buff_p (prx->port_id);
            while (valid_len--)
            {
                cur_val = prx_buff->psync_buff[tmp_rp];

                if (R_UIF_HDLC_SCODE == cur_val)
                {
                    *(pwp++) = cur_val;     /* PRQA S 2814 */
                    tmp_rp   = FWD_PTR (tmp_rp, 1u, prx_buff->uif_buff_len);
                    break;
                }
                else if ((R_UIF_HDLC_ECODE_H == prev_val) && (R_UIF_HDLC_ECODE_LE == cur_val))
                {
                    pwp--;
                    cur_val = R_UIF_HDLC_EDECODE_E;
                }
                else if ((R_UIF_HDLC_ECODE_H == prev_val) && (R_UIF_HDLC_ECODE_LD == cur_val))
                {
                    pwp--;
                    cur_val = R_UIF_HDLC_EDECODE_D;
                }
                else
                {
                    /* Do Nothing */
                }
                *(pwp++) = cur_val;         /* PRQA S 2814 */
                prev_val = prx_buff->psync_buff[tmp_rp];
                tmp_rp   = FWD_PTR (tmp_rp, 1u, prx_buff->uif_buff_len);
                length   = (uint16_t)((r_uintptr_t)pwp - (r_uintptr_t)phdlc);
                if (length >= (R_UIF_HDLC_HEADER_LEN + pcore->uif_command_len_max))
                {
                    break;
                }
            }


            prx_buff->uif_sync_rp = tmp_rp;
            length                = (uint16_t)((r_uintptr_t)pwp - (r_uintptr_t)phdlc);

            if (
                (length <= R_UIF_HDLC_PACK_WOESC_MIN) ||
                (length >= (R_UIF_HDLC_HEADER_LEN + pcore->uif_command_len_max))
                )
            {
                /*Fatal Error*/
                prx_buff->uif_sync_rp = prx_buff->uif_sync_wp;
                R_UIF_STATS_INC (prx, R_UIF_STATS_RX_IN_SYNC_BREAK);
            }
            else
            {
                ret = uif_hdlc_dec (prx->port_id, &prx->data.rx_key.key, (uint8_t *)phdlc, length);
                if (R_SYS_STATUS_SUCCESS == ret)
                {
                    if (phdlc->head.secFlg)
                    {
                        R_UIF_STATS_INC (prx, R_UIF_STATS_RX_IN_SECURITY_SUCCESS);
                    }

                    /* Call Receive */
                    prx_lbuff->cmn.phead = phdlc->payload;
                    ret = uif_rx_receive_cmd (prx->port_id, (r_uif_lbuff_t *)prx_lbuff, phdlc->head.type, phdlc->payload, (uint16_t)(length - R_UIF_HDLC_HEADER_LEN));
                    if (R_SYS_STATUS_SUCCESS == ret)
                    {
                        R_UIF_STATS_INC (prx, R_UIF_STATS_RX_OUT_COMMAND_REQD);
                    }
                    else
                    {
                        R_UIF_STATS_INC (prx, R_UIF_STATS_RX_OUT_COMMAND_REQD_ERROR);
                    }

                }
                else if (R_SYS_STATUS_CRC_ERROR == ret)
                {
                    R_UIF_STATS_INC (prx, R_UIF_STATS_RX_IN_CRC_ERROR);
                }
                else if (R_SYS_STATUS_SEC_ERROR == ret)
                {
                    R_UIF_STATS_INC (prx, R_UIF_STATS_RX_IN_SECURITY_ERROR);
                }
                else
                {
                    /* Do Nothing */
                }
            }

        }
    }

    return R_SYS_STATUS_SUCCESS;
} /* uif_rx_hdlc_process */
/******************************************************************************
   End of function  uif_rx_hdlc_process
******************************************************************************/

