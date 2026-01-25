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
* File Name   : r_uif_buff_declared.c
*    @version
*        $Rev: 9083 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2022-03-14 10:26:21 +0900#$
* Description :
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_uif_buff_declared.h"
#include "r_uif_thread_if.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_UIF_CORE_MAX_BAUDRATE     (1000000u)

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static r_uif_core_buff_t * ps_uif_core_buff[R_UIF_CORE_STATIC_BUFF_NUM] = {0u};

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/*===========================================================================*/
/* FUNCTION declared                                                         */
/*===========================================================================*/

/******************************************************************************
* Function Name:uif_set_core_buff_p
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static r_sys_status_t uif_set_core_buff_p (uint8_t port_id, r_uif_memsetup_info_t * pinfo, void ** ppend)
{
    void *                 ptr;
    r_uif_core_buff_t *    pcore;
    r_uif_rx_core_buff_t * prx           = ZERO;
    r_uif_tx_core_buff_t * ptx;
    uint32_t               tmp_tx_ttl_msec;
    uint32_t *             prx_linear_buff = ZERO;
    uint32_t *             ptx_linear_buff;
    uint8_t *              prx_ring_buff = ZERO;
    uint8_t *              ptx_ring_buff;

    if (
        ((((NULL == pinfo) || (NULL == pinfo->pstart)) ||
          (port_id >= R_UIF_CORE_STATIC_BUFF_NUM))) ||
        (((0u == pinfo->size_info.rx_buff_size_byte) && (0u == pinfo->size_info.tx_buff_size_byte)) ||
         (NULL == ppend))

        )
    {
        return R_SYS_STATUS_INVALID_PARAMETER;
    }

    ptr   = pinfo->pstart;
    ptr   = (void *)ALIGN_4BYTE (ptr);

    pcore = (r_uif_core_buff_t *)ptr;
    ptr   = (r_uif_core_buff_t *)&pcore[1];

    if (((r_uintptr_t)ptr - (r_uintptr_t)pinfo->pstart) > pinfo->alloc_byte_size)
    {
        return R_SYS_STATUS_INSUFFICIENT_MEMSIZE;
    }

    if (pinfo->size_info.rx_buff_size_byte)
    {
        if (pinfo->size_info.rx_buff_size_byte <= (R_UIF_HDLC_HEADER_LEN + (pinfo->size_info.command_len_max * 2)))
        {
            return R_SYS_STATUS_INVALID_PARAMETER;
        }

        ptr           = (void *)ALIGN_4BYTE (ptr);
        prx           = (r_uif_rx_core_buff_t *)ptr;
        ptr           = (r_uif_rx_core_buff_t *)&prx[1];

        ptr             = (void *)ALIGN_4BYTE (ptr);
        prx_linear_buff = (uint32_t *)ptr;
        ptr             = (uint32_t *)&prx_linear_buff[1 + (((R_UIF_HDLC_HEADER_LEN + pinfo->size_info.command_len_max) + 3u) / 4u)];

        prx_ring_buff = (uint8_t *)ptr;
        ptr           = (uint8_t *)&prx_ring_buff[pinfo->size_info.rx_buff_size_byte];    /* PRQA S 2824 */

        if (((r_uintptr_t)ptr - (r_uintptr_t)pinfo->pstart) > pinfo->alloc_byte_size)
        {
            return R_SYS_STATUS_INSUFFICIENT_MEMSIZE;
        }
    }

/*  if (pinfo->size_info.tx_buff_size_byte) */
    {
        if (
            (pinfo->size_info.tx_buff_size_byte > 0) &&
            (pinfo->size_info.tx_buff_size_byte <= (R_UIF_HDLC_HEADER_LEN + (pinfo->size_info.command_len_max * 2)))
            )
        {
            return R_SYS_STATUS_INVALID_PARAMETER;
        }

        ptr              = (void *)ALIGN_4BYTE (ptr);
        ptx              = (r_uif_tx_core_buff_t *)ptr;
        ptr              = (r_uif_tx_core_buff_t *)&ptx[1];                               /* PRQA S 2824 */

        ptr             = (void *)ALIGN_4BYTE (ptr);
        ptx_linear_buff = (uint32_t *)ptr;
        ptr             = (uint32_t *)&ptx_linear_buff[((R_UIF_HDLC_HEADER_LEN + (pinfo->size_info.command_len_max * 2)) + 3u) / 4u];

        ptx_ring_buff    = (uint8_t *)ptr;
        ptr              = (uint8_t *)&ptx_ring_buff[pinfo->size_info.tx_buff_size_byte]; /* PRQA S 2824 */

        if (((r_uintptr_t)ptr - (r_uintptr_t)pinfo->pstart) > pinfo->alloc_byte_size)
        {
            return R_SYS_STATUS_INSUFFICIENT_MEMSIZE;
        }

        /* 8 Byte2Bit, 1000 sec2msec */
        if ( (pinfo->size_info.baud_rate > R_UIF_CORE_MAX_BAUDRATE) || (pinfo->size_info.baud_rate == 0u) )
        {
            return R_SYS_STATUS_INVALID_PARAMETER;
        }
        tmp_tx_ttl_msec  =  (pinfo->size_info.tx_buff_size_byte > (R_UIF_HDLC_HEADER_LEN + (pinfo->size_info.command_len_max * 2))) ? pinfo->size_info.tx_buff_size_byte : (R_UIF_HDLC_HEADER_LEN + (pinfo->size_info.command_len_max * 2));
        tmp_tx_ttl_msec  = (uint32_t)(((tmp_tx_ttl_msec * 8) * 1000)) / pinfo->size_info.baud_rate;
        tmp_tx_ttl_msec += (uint32_t)R_UIF_TX_UART_TTL_MARGIN_MSEC;
    }

    R_memset (pcore, 0, sizeof (r_uif_core_buff_t));

    /* stats buff alloc */
    {
        uint32_t * pstats[R_UIF_STATS_GROUP_END];

        ptr       = (void *)ALIGN_4BYTE (ptr);
        pstats[0] = (uint32_t *)ptr;
        ptr       = (uint32_t *)&pstats[0][R_UIF_STATS_RXTX_END];

        if (((r_uintptr_t)ptr - (r_uintptr_t)pinfo->pstart) > pinfo->alloc_byte_size)
        {
            return R_SYS_STATUS_INSUFFICIENT_MEMSIZE;
        }

        pcore->uif_stats[0].pstart = pstats[0];
        pcore->uif_stats[0].length = sizeof (uint32_t) * R_UIF_STATS_RXTX_END;

        R_UIF_InitStatistics (&pcore->uif_stats[0]);

    }

    pcore->prx           = prx;
    pcore->ptx           = ptx;
    pcore->uif_command_len_max = pinfo->size_info.command_len_max;

    if (prx)
    {
        R_memset (pcore->prx, 0, sizeof (r_uif_rx_core_buff_t));
        pcore->prx->port_id    = port_id;
        pcore->prx->data.rx_lbuff.lbuff_long = prx_linear_buff;
        pcore->prx->data.rx_rbuff_info.psync_buff = prx_ring_buff;
        pcore->prx->data.rx_rbuff_info.uif_buff_len = pinfo->size_info.rx_buff_size_byte;
        R_memset (pcore->prx->data.rx_rbuff_info.psync_buff, 0, pcore->prx->data.rx_rbuff_info.uif_buff_len);
        pcore->prx->puif_stats = &pcore->uif_stats[0];
    }

/*  if (ptx) */
    {
        R_memset (pcore->ptx, 0, sizeof (r_uif_tx_core_buff_t));
        pcore->ptx->port_id      = port_id;
        pcore->ptx->data.tx_lbuff.lbuff_long = ptx_linear_buff;
        pcore->ptx->data.tx_rbuff_info.psync_buff = ptx_ring_buff;
        pcore->ptx->data.tx_rbuff_info.uif_buff_len = pinfo->size_info.tx_buff_size_byte;
        R_memset (pcore->ptx->data.tx_rbuff_info.psync_buff, 0, pcore->ptx->data.tx_rbuff_info.uif_buff_len);
        pcore->ptx->puif_stats   = &pcore->uif_stats[0];
        pcore->ptx->uifTxTtlMsec = tmp_tx_ttl_msec;
    }

    *ppend                    = ptr;
    ps_uif_core_buff[port_id] = pcore;
    pinfo->pout_stats         = &pcore->uif_stats[0];

    return R_SYS_STATUS_SUCCESS;
} /* uif_set_core_buff_p */
/******************************************************************************
   End of function  uif_set_core_buff_p
******************************************************************************/

/******************************************************************************
* Function Name:uif_get_core_buff_p
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_uif_core_buff_t *uif_get_core_buff_p (uint8_t port_id)
{
    if (
        (port_id >= R_UIF_CORE_STATIC_BUFF_NUM)
        )
    {
        return NULL;
    }

    return ps_uif_core_buff[port_id];
}
/******************************************************************************
   End of function  uif_get_core_buff_p
******************************************************************************/

/******************************************************************************
* Function Name:R_UIF_InitCore
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_sys_status_t R_UIF_InitCore (uint8_t port_id, r_uif_memsetup_info_t * pinfo, void ** ppend)
{
    r_sys_status_t ret = uif_set_core_buff_p (port_id, pinfo, ppend);
    return ret;
}
/******************************************************************************
   End of function  R_UIF_InitCore
******************************************************************************/

/******************************************************************************
* Function Name:R_UIF_DeInitCore
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_sys_status_t R_UIF_DeInitCore (uint8_t port_id)
{

    if (
        (port_id >= R_UIF_CORE_STATIC_BUFF_NUM)
        )
    {
        return R_SYS_STATUS_INVALID_PARAMETER;
    }
    ps_uif_core_buff[port_id] = NULL;

    return R_SYS_STATUS_SUCCESS;
}
/******************************************************************************
   End of function  R_UIF_DeInitCore
******************************************************************************/

/*===========================================================================*/
/* For RX Only                                                               */
/*===========================================================================*/
/******************************************************************************
* Function Name:uif_rx_get_core_buff_p
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_uif_rx_core_buff_t *uif_rx_get_core_buff_p (uint8_t port_id)
{
    if (
        ((port_id >= R_UIF_CORE_STATIC_BUFF_NUM) ||
         (NULL == ps_uif_core_buff[port_id])) ||
        (NULL == ps_uif_core_buff[port_id]->prx)
        )
    {
        return NULL;
    }

    return ps_uif_core_buff[port_id]->prx;
}
/******************************************************************************
   End of function  uif_rx_get_core_buff_p
******************************************************************************/

/******************************************************************************
* Function Name:uif_rx_get_rbuffinfo_p
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_uif_rx_ringbuff_info_t *uif_rx_get_rbuffinfo_p (uint8_t port_id)
{
    if (
        ((port_id >= R_UIF_CORE_STATIC_BUFF_NUM) ||
         (NULL == ps_uif_core_buff[port_id])) ||
        (NULL == ps_uif_core_buff[port_id]->prx)
        )
    {
        return NULL;
    }

    return &ps_uif_core_buff[port_id]->prx->data.rx_rbuff_info;
}
/******************************************************************************
   End of function  uif_rx_get_rbuffinfo_p
******************************************************************************/

/*===========================================================================*/
/* For TX Only                                                               */
/*===========================================================================*/

/******************************************************************************
* Function Name:uif_tx_get_core_buff_p
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_uif_tx_core_buff_t *uif_tx_get_core_buff_p (uint8_t port_id)
{
    if (
        ((port_id >= R_UIF_CORE_STATIC_BUFF_NUM) ||
         (NULL == ps_uif_core_buff[port_id])) ||
        (NULL == ps_uif_core_buff[port_id]->ptx)
        )
    {
        return NULL;
    }

    return ps_uif_core_buff[port_id]->ptx;
}
/******************************************************************************
   End of function  uif_tx_get_core_buff_p
******************************************************************************/

/******************************************************************************
* Function Name:uif_tx_get_rbuffinfo_p
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_uif_tx_ringbuff_info_t *uif_tx_get_rbuffinfo_p (uint8_t port_id)
{
    if (
        ((port_id >= R_UIF_CORE_STATIC_BUFF_NUM) ||
         (NULL == ps_uif_core_buff[port_id])) ||
        (NULL == ps_uif_core_buff[port_id]->ptx)
        )
    {
        return NULL;
    }

    return &ps_uif_core_buff[port_id]->ptx->data.tx_rbuff_info;
}
/******************************************************************************
   End of function  uif_tx_get_rbuffinfo_p
******************************************************************************/

