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
* File Name   : r_uif_intc.c
*    @version
*        $Rev: 3686 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2018-01-26 13:40:25 +0900#$
* Description :
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_uif_hdlc_process.h"
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


/******************************************************************************
* Function Name:R_UIF_IntRxByte
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_UIF_IntRxByte (uint8_t port_id, uint8_t rx_byte)
{
    r_uif_rx_ringbuff_info_t * prx = uif_rx_get_rbuffinfo_p (port_id);

    if (NULL == prx)
    {
        return;
    }

    if (GET_FREE_LEN (prx->uif_async_wp, prx->uif_sync_rp, prx->uif_buff_len))
    {
        prx->psync_buff[prx->uif_async_wp] = rx_byte;
        prx->uif_async_wp = FWD_PTR (prx->uif_async_wp, 1u, prx->uif_buff_len);
    }
    else
    {
        prx->uif_async_full = R_TRUE;
    }
} /* R_UIF_IntRxByte */
/******************************************************************************
   End of function  R_UIF_IntRxByte
******************************************************************************/

/******************************************************************************
* Function Name:R_UIF_IntTx
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_UIF_IntTx (uint8_t port_id)
{
    r_uif_tx_core_buff_t * ptx = uif_tx_get_core_buff_p (port_id);

    if (NULL == ptx)
    {
        return;
    }

    if (R_UIF_UART_ACTIVE == ptx->data.tx_rbuff_info.uart_info.state)
    {
        uif_tx_release_sendbuff (ptx);
        ptx->data.tx_rbuff_info.uart_info.state = R_UIF_UART_FREE;
        R_UIF_STATS_INC (ptx, R_UIF_STATS_TX_OUT_COMMAND_SUCCESS);
    }
    else
    {
        ptx->data.tx_rbuff_info.uart_info.state = R_UIF_UART_FREE;
    }
} /* R_UIF_IntTx */
/******************************************************************************
   End of function  R_UIF_IntTx
******************************************************************************/

