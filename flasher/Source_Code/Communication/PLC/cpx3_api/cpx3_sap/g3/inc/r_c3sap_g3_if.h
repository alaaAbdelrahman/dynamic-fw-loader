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
 * File Name : r_c3sap_g3_if.h
 * Description : G3 layer Internal API
 ******************************************************************************/

#ifndef R_C3SAP_G3_IF_H
#define R_C3SAP_G3_IF_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_g3_api.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_G3_SET_PKT_HEAD(c, a, p)  ((uint8_t)((c) << 7) | ((uint8_t)((a) << 4) | (uint8_t)(p)))
#define R_G3_GET_PKT_IDC(a)         ((uint8_t)((a) >> 7))
#define R_G3_GET_PKT_IDA(a)         ((uint8_t)((a) >> 4) & 0x07u)
#define R_G3_GET_PKT_IDP(a)         ((uint8_t)((a) & 0x0Fu))

#define R_G3CORE_AVOID_ERROR  (4u)

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
void       R_G3CTRL_Init (uint8_t * preq_bin, uint16_t req_buff_size, void * pcb_str, uint16_t cb_buff_size);
void       R_G3CTRL_Deinit (void);
r_result_t R_G3CTRL_RegistCb (uint8_t ch, uint8_t g3mode, r_g3_callback_t * pcallbacks);
r_result_t R_G3CTRL_SendCmd (uint8_t * buffer, uint32_t size);
r_result_t R_G3CTRL_ReceiveCmd (uint8_t * pbuffer);
uint8_t    R_G3CTRL_GetG3Mode (uint8_t ch);
r_result_t R_G3CTRL_SemaphoreWait (void);
void       R_G3CTRL_SemaphoreRelease (void);
void       R_EAP_SapInit (uint8_t * preq_bin, uint16_t req_buff_size, void * pcb_str, uint16_t cb_buff_size);
r_result_t R_EAP_RegistCb (uint8_t ch, void * pcallbacks);
r_result_t R_EAP_ClearCb (uint8_t ch);
void       R_EAP_BaseCb (uint8_t * buff);
void       R_ADP_SapInit (uint8_t * preq_bin, uint16_t req_buff_size, void * pcb_str, uint16_t cb_buff_size);
r_result_t R_ADP_RegistCb (uint8_t ch, void * pcallbacks);
r_result_t R_ADP_ClearCb (uint8_t ch);
void       R_ADP_BaseCb (uint8_t * buff);
void       R_G3MAC_SapInit (uint8_t * preq_bin, uint16_t req_buff_size, void * pcb_str, uint16_t cb_buff_size);
r_result_t R_G3MAC_RegistCb (uint8_t ch, void * pcallbacks);
r_result_t R_G3MAC_ClearCb (uint8_t ch);
void       R_G3MAC_BaseCb (uint8_t * buff);
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
void       R_G3RFMAC_SapInit (uint8_t * preq_bin, uint16_t req_buff_size, void * pcb_str, uint16_t cb_buff_size);
r_result_t R_G3RFMAC_RegistCb (uint8_t ch, void * pcallbacks);
r_result_t R_G3RFMAC_ClearCb (uint8_t ch);
void       R_G3RFMAC_BaseCb (uint8_t * buff);
#endif /* R_DEFINE_DISABLE_HYBRID && R_DEFINE_OPEN_ARM */
 
#endif /* R_C3SAP_G3_IF_H */

