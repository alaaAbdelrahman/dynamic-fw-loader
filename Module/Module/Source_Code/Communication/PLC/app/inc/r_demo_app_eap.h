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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : r_demo_eap.h
*    @version
*        $Rev: 9602 $
*    @last editor
*        $Author: a0202438 $
*    @date
*        $Date:: 2022-04-08 13:53:43 +0900#$
* Description :
******************************************************************************/

#ifndef R_CERT_APP_EAP_H
#define R_CERT_APP_EAP_H

/******************************************************************************
Macro definitions
******************************************************************************/
#if (defined R_RL78_PLC) || (defined __VANGO__)
#define R_DEMO_CLIENT_NUM_MAX    (8u)
#define R_DEMO_CLIENT_B_NUM_MAX  (2u)
#else
#define R_DEMO_CLIENT_NUM_MAX    (128u)
#define R_DEMO_CLIENT_B_NUM_MAX  (16u)
#endif

/******************************************************************************
Typedef definitions
******************************************************************************/


typedef enum
{
    R_DEMO_CSTATE_INVALID = 0,
    R_DEMO_CSTATE_VALID,
    R_DEMO_CSTATE_JOINED,
    R_DEMO_CSTATE_LEAVED,
} r_demo_client_state_t;


typedef union
{
    r_eap_clientinfo_table_t     cTable[R_DEMO_CLIENT_NUM_MAX];
} r_demo_client_table_t;

typedef struct
{
    r_demo_client_table_t tbl;
    uint16_t              joinedNum;
    uint16_t              networkAddrCnt;
} r_demo_client_info_t;


/******************************************************************************
Functions prototype
******************************************************************************/

void R_LBS_ShowDeviceList (void);
void R_LBS_KickDeviceNo (uint8_t no);
void R_LBS_RemoveKeyDev(uint8_t no, uint8_t index);

void R_DEMO_AppHandleEapNewDeviceInd (const r_eap_eapm_newdevice_ind_t * ind);
void R_DEMO_AppHandleEapNwkLeaveInd (const r_eap_eapm_network_leave_ind_t * ind);
void R_DEMO_AppHandleEapNwkJoinInd (const r_eap_eapm_network_join_ind_t * ind);
void R_DEMO_AppHandleEapEapKeyInd(const r_eap_eapm_eap_key_ind_t * ind);


r_result_t R_DEMO_EapInit (uint8_t chId);
r_result_t R_DEMO_EapmStart (uint8_t chId, r_eap_eapm_start_cnf_t ** eStaCfm);
r_result_t R_DEMO_EapmNetwork (uint8_t chId, const r_eap_eapm_network_req_t * eNwkReq, r_eap_eapm_network_cnf_t ** eNwkCfm);
r_result_t R_DEMO_EapmSet (uint8_t chId, const r_eap_eapm_set_req_t * setReq, r_eap_eapm_set_cnf_t ** setCfm);
r_result_t R_DEMO_EapmGet (uint8_t chId, const r_eap_eapm_get_req_t * getReq, r_eap_eapm_get_cnf_t ** getCfm);
r_result_t R_DEMO_EapmSetClientinfo (uint8_t chId, const r_eap_eapm_set_clientinfo_req_t * setReq, r_eap_eapm_set_clientinfo_cnf_t ** setCfm);
r_result_t R_DEMO_EapmReset (uint8_t chId, r_eap_eapm_reset_cnf_t ** cnf);
r_eap_status_t R_DEMO_EapmGetWrap (uint8_t chId, uint8_t id, uint16_t index, uint8_t * val);
r_eap_status_t R_DEMO_EapmSetWrap (uint8_t chId, uint8_t id, uint16_t index, uint8_t * val);
r_eap_status_t R_DEMO_EapmSetClientinfoWrap (uint8_t chId, r_eap_clientinfo_table_t * pTbl);

r_result_t set_eap_ib_entry (uint8_t chId, const r_eap_ib_id_t attributeId);
r_result_t get_eap_ib_entry (uint8_t chId, const r_eap_ib_id_t attributeId);


#endif /* R_CERT_APP_EAP_H */

