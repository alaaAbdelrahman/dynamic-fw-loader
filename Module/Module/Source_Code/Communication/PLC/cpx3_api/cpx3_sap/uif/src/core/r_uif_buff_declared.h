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
* File Name   : r_uif_buff_declared.h
*    @version
*        $Rev: 6428 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2021-04-01 10:05:31 +0900#$
* Description :
******************************************************************************/

#ifndef R_UIF_BUFF_DECLARED_H
#define R_UIF_BUFF_DECLARED_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_uif_base.h"
#include "r_uif_data_manage.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct
{
    r_uif_stats_info_t * puif_stats;
    uint8_t              port_id;
    r_uif_rx_data_t      data;
} r_uif_rx_core_buff_t;

typedef struct
{
    r_uif_stats_info_t * puif_stats;
    uint32_t             uifTxTtlMsec;
    uint8_t              port_id;
    r_uif_tx_data_t      data;
} r_uif_tx_core_buff_t;


/*===========================================================================*/
/* BUFF declared                                                             */
/*===========================================================================*/
typedef struct
{
    r_uif_stats_info_t     uif_stats[R_UIF_STATS_GROUP_END];
    r_uif_rx_core_buff_t * prx;
    r_uif_tx_core_buff_t * ptx;
    uint16_t               uif_command_len_max;
} r_uif_core_buff_t;

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
r_uif_core_buff_t *uif_get_core_buff_p (uint8_t port_id);
r_uif_rx_core_buff_t *uif_rx_get_core_buff_p (uint8_t port_id);
r_uif_tx_core_buff_t *uif_tx_get_core_buff_p (uint8_t port_id);
r_uif_rx_ringbuff_info_t *uif_rx_get_rbuffinfo_p (uint8_t port_id);
r_uif_tx_ringbuff_info_t *uif_tx_get_rbuffinfo_p (uint8_t port_id);

#endif /* R_UIF_BUFF_DECLARED_H */

