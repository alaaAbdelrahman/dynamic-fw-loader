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
 * File Name : r_uif_thread_if.h
 * Description : UART Interface Internal API Header
 ******************************************************************************/
#ifndef R_UIF_THREAD_IF_H
#define R_UIF_THREAD_IF_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_uif_sap_if.h"
#include "r_c3sap_sys_if.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_UIF_EventNotify(port_id, event, pparam)  /* Invalidated function */

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum
{
    R_UIF_LAYER_CORE,
    R_UIF_LAYER_IF,
}r_uif_layer_t;

typedef struct
{
    uint16_t     rx_buff_size_byte;
    uint16_t     tx_buff_size_byte;
    uint32_t     baud_rate;
    uint16_t     command_len_max;
} r_uif_memsetup_size_info_t;

typedef struct
{
    r_uif_memsetup_size_info_t size_info;
    void * pstart;
    uint32_t alloc_byte_size;
    void * pout_stats;
} r_uif_memsetup_info_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/*===========================================================================*/
/* For UIF IF API                                                            */
/*===========================================================================*/
r_sys_status_t R_UIF_ReceiveCmd (uint8_t port_id, uint8_t dst_block_id, uint8_t * pbuffer, uint32_t size);
void R_UIF_TxComplete( uint8_t port_id );
r_sys_status_t R_UIF_DrvUartSend(uint8_t port_id, uint8_t *pdata, uint16_t length);
r_sys_status_t R_UIF_DrvCrc32(uint8_t port_id, uint16_t length, uint8_t *pin, uint32_t initial,  uint8_t *pcrc_res);

/*===========================================================================*/
/* For UIF CORE API                                                          */
/*===========================================================================*/
r_sys_status_t R_UIF_InitCore (uint8_t port_id, r_uif_memsetup_info_t * pinfo, void ** ppend);
r_sys_status_t R_UIF_DeInitCore (uint8_t port_id);
r_sys_status_t R_UIF_RxCyclicMain (uint8_t port_id);
r_sys_status_t R_UIF_CbRxReceiveCmd (uint8_t port_id, uint8_t src_type_id, uint8_t * pAddress, uint32_t legth);
r_sys_status_t R_UIF_TxCyclicMain (uint8_t port_id);
r_sys_status_t R_UIF_TxReqMain (uint8_t port_id, uint8_t src_type_id, uint32_t tx_length, uint8_t * ptx_data, uint8_t secFlg);
void R_UIF_IntTx (uint8_t port_id);
void R_UIF_IntRxByte (uint8_t port_id, uint8_t rx_byte);

#endif  /* R_UIF_THREAD_IF_H */

