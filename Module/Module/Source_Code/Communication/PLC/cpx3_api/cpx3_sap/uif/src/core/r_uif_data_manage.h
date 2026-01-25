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
* File Name   : r_uif_data_manage.h
*    @version
*        $Rev: 6428 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2021-04-01 10:05:31 +0900#$
* Description :
******************************************************************************/

#ifndef R_UIF_DATA_MANAGE_H
#define R_UIF_DATA_MANAGE_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_uif_base.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_UIF_ASYNC_BUFF_LEN    (64u)
#define R_UIF_MAX_STATS_LENGTH  (1024u)

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum
{
    R_UIF_RXSTATE_ASYNC = 0,
    R_UIF_RXSTATE_SYNC,
    R_UIF_RXSTATE_END,
} r_uif_rxstate_t;

typedef enum
{
    R_UIF_LBUFF_INVALID = 0,
    R_UIF_LBUFF_VALID,
    R_UIF_LBUFF_WAIT_RELEASE,
    R_UIF_LBUFF_END,
} r_uif_lbuff_state_t;

typedef enum
{
    R_UIF_UART_FREE = 0,
    R_UIF_UART_ACTIVE,
    R_UIF_UART_BUSY,
    R_UIF_UART_END,
} r_uif_uart_state_t;


/*===========================================================================*/
/* TYPE declared                                                             */
/*===========================================================================*/

__packed typedef struct
{
    uint32_t        uif_rx_sync_valid_time;
    uint8_t *       psync_buff;

/*  uint8_t                     asyncBuff[R_UIF_ASYNC_BUFF_LEN]; */
    uint16_t        uif_buff_len;
    uint16_t        uif_async_wp;

/*  uint16_t                    uifAsyncRp; */
    uint16_t        uif_async_full;
    uint16_t        uif_sync_wp;
    uint16_t        uif_sync_wp_pre;
    uint16_t        uif_sync_rp;
    r_uif_rxstate_t rx_state;
} r_uif_rx_ringbuff_info_t;



__packed typedef struct
{
    uint8_t  state;
    uint32_t uart_valid_time;
} r_uif_uart_info_t;

__packed typedef struct
{
    uint8_t *         psync_buff;
    uint16_t          uif_buff_len;
    uint16_t          uif_sync_wp;
    uint16_t          uif_sync_rp_pre;
    uint16_t          uif_sync_rp;
    r_uif_uart_info_t uart_info;
} r_uif_tx_ringbuff_info_t;

__packed typedef struct
{
    uint8_t *           phead;
    uint16_t            length;
    r_uif_lbuff_state_t lbuff_state;
    uint8_t             type_id;
} r_uif_lbuff_cmn_t;

__packed typedef struct
{
    r_uif_lbuff_cmn_t cmn;
    uint32_t *        lbuff_long;
} r_uif_lbuff_t;

__packed typedef struct
{
    r_uif_lbuff_cmn_t cmn;
    uint32_t *        lbuff_long;
} r_uif_lbuff_rx_t;


__packed typedef struct
{
    r_uif_lbuff_cmn_t cmn;
    uint32_t *        lbuff_long;
} r_uif_lbuff_tx_t;

__packed typedef struct
{
    uint32_t flush_cnt;
    uint32_t fwd_cnt;
    uint32_t triple_cnt;
} r_uif_log_rx_t;


__packed typedef struct
{
    uint32_t * pstart;
    uint32_t   length;
} r_uif_stats_info_t;


__packed typedef struct
{
    uint8_t validFlg;
    uint8_t data[16u];
} r_uif_key_t;


__packed typedef struct
{
    r_uif_key_t key;
    uint8_t     updateFlg;
} r_uif_key_info_t;


/*===========================================================================*/
/* BUFF declared                                                             */
/*===========================================================================*/

__packed typedef struct
{
    r_uif_lbuff_rx_t         rx_lbuff;
    r_uif_rx_ringbuff_info_t rx_rbuff_info;
    r_uif_key_info_t         rx_key;
    r_uif_log_rx_t           rx_log;
} r_uif_rx_data_t;


__packed typedef struct
{
/*  r_uif_uart_info_t           info; */
    r_uif_lbuff_tx_t         tx_lbuff;
    r_uif_tx_ringbuff_info_t tx_rbuff_info;
    r_uif_key_info_t         tx_key;
} r_uif_tx_data_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
void *uif_cmn_get_lbuff (r_uif_lbuff_t * plbuff, r_uif_lbuff_state_t lbuff_state);
r_sys_status_t uif_cmn_release_lbuff (r_uif_lbuff_t * plbuff, uint8_t src_type_id, uint8_t * paddress, uint16_t length);

r_sys_status_t uif_rx_receive_cmd (uint8_t port_id, r_uif_lbuff_t * plbuff, uint8_t type_id, uint8_t * pbuffer, uint16_t size);

uint32_t R_UIF_GetCurrentTimeMsec (void);
uint8_t R_UIF_ChkTimeoutMsec (uint32_t validTimeMsec, uint32_t ttlMsec);


void R_UIF_InitStatistics (r_uif_stats_info_t * pinfo);
void R_UIF_StatisticsIncrement (r_uif_stats_info_t ** ppinfo, uint8_t index, uint16_t offset);


#endif /* R_UIF_DATA_MANAGE_H */

