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
 * File Name : r_c3sap_adp.c
 * Description : G3 ADP layer API
 ******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "r_c3sap_config.h"
#include "r_c3sap_g3_if.h"
#include "r_c3sap_g3_convert.h"
#include "r_g3_cmd.h"
#include "r_c3sap_g3_buffsize.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_ADP_CB_BUFF_SIZE (R_MEMORY_POOL_SIZE_CPX_SAP - R_C3SAP_MAX_REQBIN_SIZE)

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct
{
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    r_g3_hyadp_callback_t cb[R_G3_CH_MAX];
#else
    r_g3_adp_callback_t cb[R_G3_CH_MAX];
#endif
    uint8_t *preq_bin;
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    r_hyadp_cb_str_t *pcb_str;
#else
    r_adp_cb_str_t *pcb_str;
#endif
    uint16_t req_buff_size;
    uint16_t cb_buff_size;
} r_adp_info_t;






///////////////////////////////
extern uint8_t get_handle_state;
/////////////////////////////////
/******************************************************************************
Private global variables and functions
******************************************************************************/
static r_result_t adp_check_param(uint8_t ch, uint16_t func_id, uint8_t *preq_buff);
static r_result_t adp_process_request(uint8_t ch, uint16_t func_id, void *preq);
static r_result_t adp_process_request_without_payload(uint8_t ch, uint16_t func_id);

/******************************************************************************
Private global variables and functions
******************************************************************************/
static r_adp_info_t adp_info;

/* PRQA S 2814 EOF */
/*===========================================================================*/
/* Functions                                                                 */
/*===========================================================================*/

/******************************************************************************
 * Function Name:R_ADP_AdpdDataReq
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_ADP_AdpdDataReq(uint8_t ch, r_adp_adpd_data_req_t *preq)
{
    r_result_t status;

    status = adp_process_request(ch, R_G3_FUNCID_ADPD_DATA_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_ADP_AdpdDataReq
******************************************************************************/

/******************************************************************************
 * Function Name:R_ADP_AdpmResetReq
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_ADP_AdpmResetReq(uint8_t ch)
{
    r_result_t status;

    status = adp_process_request_without_payload(ch, R_G3_FUNCID_ADPM_RESET_REQ);

    return status;
}
/******************************************************************************
   End of function  R_ADP_AdpmResetReq
******************************************************************************/

/******************************************************************************
 * Function Name:R_ADP_AdpmDiscoveryReq
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_ADP_AdpmDiscoveryReq(uint8_t ch, r_adp_adpm_discovery_req_t *preq)
{
    r_result_t status;

    status = adp_process_request(ch, R_G3_FUNCID_ADPM_DISCOVERY_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_ADP_AdpmDiscoveryReq
******************************************************************************/

/******************************************************************************
 * Function Name:R_ADP_AdpmNetworkStartReq
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_ADP_AdpmNetworkStartReq(uint8_t ch, r_adp_adpm_network_start_req_t *preq)
{
    r_result_t status;

    status = adp_process_request(ch, R_G3_FUNCID_ADPM_NETWORK_START_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkStartReq
******************************************************************************/

/******************************************************************************
 * Function Name:R_ADP_AdpmNetworkJoinReq
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_ADP_AdpmNetworkJoinReq(uint8_t ch, r_adp_adpm_network_join_req_t *preq)
{
    r_result_t status;

    status = adp_process_request(ch, R_G3_FUNCID_ADPM_NETWORK_JOIN_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkJoinReq
******************************************************************************/

/******************************************************************************
 * Function Name:R_ADP_AdpmNetworkLeaveReq
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_ADP_AdpmNetworkLeaveReq(uint8_t ch)
{
    r_result_t status;

    status = adp_process_request_without_payload(ch, R_G3_FUNCID_ADPM_NETWORK_LEAVE_REQ);

    return status;
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkLeaveReq
******************************************************************************/

/******************************************************************************
 * Function Name:R_ADP_AdpmGetReq
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_ADP_AdpmGetReq(uint8_t ch, r_adp_adpm_get_req_t *preq)
{
    r_result_t status;

    status = adp_process_request(ch, R_G3_FUNCID_ADPM_GET_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_ADP_AdpmGetReq
******************************************************************************/

/******************************************************************************
 * Function Name:R_ADP_AdpmSetReq
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_ADP_AdpmSetReq(uint8_t ch, r_adp_adpm_set_req_t *preq)
{
    r_result_t status;

    status = adp_process_request(ch, R_G3_FUNCID_ADPM_SET_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_ADP_AdpmSetReq
******************************************************************************/

/******************************************************************************
 * Function Name:R_ADP_AdpmRouteDiscoveryReq
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_ADP_AdpmRouteDiscoveryReq(uint8_t ch, r_adp_adpm_route_disc_req_t *preq)
{
    r_result_t status;

    status = adp_process_request(ch, R_G3_FUNCID_ADPM_ROUTE_DISCOVERY_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_ADP_AdpmRouteDiscoveryReq
******************************************************************************/

/******************************************************************************
 * Function Name:R_ADP_AdpmPathDiscoveryReq
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_ADP_AdpmPathDiscoveryReq(uint8_t ch, r_adp_adpm_path_discovery_req_t *preq)
{
    r_result_t status;

    status = adp_process_request(ch, R_G3_FUNCID_ADPM_PATH_DISCOVERY_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_ADP_AdpmPathDiscoveryReq
******************************************************************************/

/******************************************************************************
 * Function Name:R_ADP_AdpmLbpReq
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_ADP_AdpmLbpReq(uint8_t ch, r_adp_adpm_lbp_req_t *preq)
{
    r_result_t status;

    status = adp_process_request(ch, R_G3_FUNCID_ADPM_LBP_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_ADP_AdpmLbpReq
******************************************************************************/

/******************************************************************************
 * Function Name:R_ADP_RegistCb
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_ADP_RegistCb(uint8_t ch, void *pcallbacks)
{
    if (NULL == pcallbacks)
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    adp_info.cb[ch] = *(r_g3_hyadp_callback_t *)pcallbacks;
#else
    adp_info.cb[ch] = *(r_g3_adp_callback_t *)pcallbacks;
#endif
    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_ADP_RegistCb
******************************************************************************/

/******************************************************************************
 * Function Name:R_ADP_ClearCb
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_ADP_ClearCb(uint8_t ch)
{
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    R_memset(&adp_info.cb[ch], 0, sizeof(r_g3_hyadp_callback_t));
#else
    R_memset(&adp_info.cb[ch], 0, sizeof(r_g3_adp_callback_t));
#endif

    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_ADP_ClearCb
******************************************************************************/

/******************************************************************************
 * Function Name:R_ADP_BaseCb
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_ADP_BaseCb(uint8_t *pbuff)
{
    uint8_t idc;
    uint8_t idp;
    uint16_t funcId;
    uint16_t length = 0;
    uint16_t max_len = adp_info.cb_buff_size;
    r_result_t status;
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    r_g3_hyadp_callback_t *pCb;
#else
    r_g3_adp_callback_t *pCb;
#endif

    if ((NULL == pbuff) || (NULL == adp_info.pcb_str))
    {
        return;
    }
    idc = R_G3_GET_PKT_IDC(pbuff[0]);
    idp = (pbuff[0] & 0x0F);
    funcId = ((((uint16_t)pbuff[0] << 8u) | pbuff[1]) & 0x3FFFu);
    pCb = &adp_info.cb[idc];

    switch (idp)
    {
    case R_G3_UNITID_G3ADP:
    {
        status = R_ADP_CbBin2Str(funcId, &pbuff[2], max_len, adp_info.pcb_str, &length);
    }
    break;
    case R_G3_UNITID_G3MAC:
    {
        status = R_G3MAC_CbBin2Str(funcId, &pbuff[2], max_len, adp_info.pcb_str, &length);
    }
    break;
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    case R_G3_UNITID_G3RFMAC:
    case R_G3_UNITID_RFLMAC:
    {
        status = R_G3RFMAC_CbBin2Str(funcId, &pbuff[2], max_len, adp_info.pcb_str, &length);
    }
    break;
#endif
    default:
    {
        status = R_RESULT_FAILED;
    }
    break;
    }

    if (R_RESULT_SUCCESS == status)
    {
        switch (funcId)
        {
        case R_G3_FUNCID_ADPD_DATA_CNF:
        {
            if (NULL != pCb->R_ADP_AdpdDataCnf)
            {
                pCb->R_ADP_AdpdDataCnf(&adp_info.pcb_str->adpd_data_cnf);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_RESET_CNF:
        {
            if (NULL != pCb->R_ADP_AdpmResetCnf)
            {
                pCb->R_ADP_AdpmResetCnf(&adp_info.pcb_str->adpm_reset_cnf);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_DISCOVERY_CNF:
        {
            if (NULL != pCb->R_ADP_AdpmDiscoveryCnf)
            {
                pCb->R_ADP_AdpmDiscoveryCnf(&adp_info.pcb_str->adpm_discovery_cnf);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_NETWORK_START_CNF:
        {
            if (NULL != pCb->R_ADP_AdpmNetworkStartCnf)
            {
                pCb->R_ADP_AdpmNetworkStartCnf(&adp_info.pcb_str->adpm_network_start_cnf);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_NETWORK_JOIN_CNF:
        {
            if (NULL != pCb->R_ADP_AdpmNetworkJoinCnf)
            {
                pCb->R_ADP_AdpmNetworkJoinCnf(&adp_info.pcb_str->adpm_network_join_cnf);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_NETWORK_LEAVE_CNF:
        {
            if (NULL != pCb->R_ADP_AdpmNetworkLeaveCnf)
            {
                pCb->R_ADP_AdpmNetworkLeaveCnf(&adp_info.pcb_str->adpm_network_leave_cnf);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_GET_CNF:
        {
            if (NULL != pCb->R_ADP_AdpmGetCnf)
            {
                pCb->R_ADP_AdpmGetCnf(&adp_info.pcb_str->adpm_get_cnf);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_SET_CNF:
        {
            if (NULL != pCb->R_ADP_AdpmSetCnf)
            {
                pCb->R_ADP_AdpmSetCnf(&adp_info.pcb_str->adpm_set_cnf);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_ROUTE_DISCOVERY_CNF:
        {
            if (NULL != pCb->R_ADP_AdpmRouteDiscoveryCnf)
            {
                pCb->R_ADP_AdpmRouteDiscoveryCnf(&adp_info.pcb_str->adpm_route_discovery_cnf);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_PATH_DISCOVERY_CNF:
        {
            if (NULL != pCb->R_ADP_AdpmPathDiscoveryCnf)
            {
                pCb->R_ADP_AdpmPathDiscoveryCnf(&adp_info.pcb_str->adpm_path_discovery_cnf);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_LBP_CNF:
        {
            if (NULL != pCb->R_ADP_AdpmLbpCnf)
            {
                pCb->R_ADP_AdpmLbpCnf(&adp_info.pcb_str->adpm_lbp_cnf);
            }
        }
        break;
        case R_G3_FUNCID_ADPD_DATA_IND:
        {
            if (NULL != pCb->R_ADP_AdpdDataInd)
            {
                pCb->R_ADP_AdpdDataInd(&adp_info.pcb_str->adpd_data_ind);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_NETWORK_LEAVE_IND:
        {
            if (NULL != pCb->R_ADP_AdpmNetworkLeaveInd)
            {
                pCb->R_ADP_AdpmNetworkLeaveInd();
            }
        }
        break;
        case R_G3_FUNCID_ADPM_PATH_DISCOVERY_IND:
        {
            if (NULL != pCb->R_ADP_AdpmPathDiscoveryInd)
            {
                pCb->R_ADP_AdpmPathDiscoveryInd(&adp_info.pcb_str->adpm_path_discovery_ind);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_LBP_IND:
        {
            if (NULL != pCb->R_ADP_AdpmLbpInd)
            {
                pCb->R_ADP_AdpmLbpInd(&adp_info.pcb_str->adpm_lbp_ind);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_NETWORK_STATUS_IND:
        {
            if (NULL != pCb->R_ADP_AdpmNetworkStatusInd)
            {
                pCb->R_ADP_AdpmNetworkStatusInd(&adp_info.pcb_str->adpm_network_status_ind);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_BUFFER_IND:
        {
            if (NULL != pCb->R_ADP_AdpmBufferInd)
            {
                pCb->R_ADP_AdpmBufferInd(&adp_info.pcb_str->adpm_buffer_ind);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_KEY_STATE_IND:
        {
            if (NULL != pCb->R_ADP_AdpmKeyStateInd)
            {
                pCb->R_ADP_AdpmKeyStateInd(&adp_info.pcb_str->adpm_key_state_ind);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_ROUTE_ERROR_IND:
        {
            if (NULL != pCb->R_ADP_AdpmRouteErrorInd)
            {
                pCb->R_ADP_AdpmRouteErrorInd(&adp_info.pcb_str->adpm_route_error_ind);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_EAP_KEY_IND:
        {
            if (NULL != pCb->R_ADP_AdpmEapKeyInd)
            {
                pCb->R_ADP_AdpmEapKeyInd(&adp_info.pcb_str->adpm_eap_key_ind);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_FRAME_COUNTER_IND:
        {
            if (NULL != pCb->R_ADP_AdpmFrameCounterInd)
            {
                pCb->R_ADP_AdpmFrameCounterInd(&adp_info.pcb_str->adpm_frame_counter_ind);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_ROUTE_UPDATE_IND:
        {
            if (NULL != pCb->R_ADP_AdpmRouteUpdateInd)
            {
                pCb->R_ADP_AdpmRouteUpdateInd(&adp_info.pcb_str->adpm_route_update_ind);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_LOAD_SEQ_NUM_IND:
        {
            if (NULL != pCb->R_ADP_AdpmLoadSeqNumInd)
            {
                pCb->R_ADP_AdpmLoadSeqNumInd(&adp_info.pcb_str->adpm_load_seq_num_ind);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_RREP_IND:
        {
            if (NULL != pCb->R_ADP_AdpmRrepInd)
            {
                pCb->R_ADP_AdpmRrepInd(&adp_info.pcb_str->adpm_rrep_ind);
            }
        }
        break;
        case R_G3_FUNCID_ADPM_BEACON_NOTIFY_IND:
        {
            if (NULL != pCb->R_ADP_AdpmBeaconNotifyInd)
            {
                pCb->R_ADP_AdpmBeaconNotifyInd(&adp_info.pcb_str->adpm_bcn_notify_ind);
            }
        }
        break;
        case R_G3_FUNCID_MLME_GET_CNF:
        {
            if (NULL != pCb->R_G3MAC_MlmeGetCnf)
            {
                pCb->R_G3MAC_MlmeGetCnf(&adp_info.pcb_str->mlme_get_cnf);
            }
        }
        break;
        case R_G3_FUNCID_MLME_SET_CNF:
        {
            if (NULL != pCb->R_G3MAC_MlmeSetCnf)
            {
                pCb->R_G3MAC_MlmeSetCnf(&adp_info.pcb_str->mlme_set_cnf);
            }
        }
        break;
        case R_G3_FUNCID_MLME_TMR_RECEIVE_IND:
        {
            if (NULL != pCb->R_G3MAC_MlmeTmrReceiveInd)
            {
                pCb->R_G3MAC_MlmeTmrReceiveInd(&adp_info.pcb_str->mlme_tmr_receve_ind);
            }
        }
        break;
        case R_G3_FUNCID_MLME_TMR_TRANSMIT_IND:
        {
            if (NULL != pCb->R_G3MAC_MlmeTmrTransmitInd)
            {
                pCb->R_G3MAC_MlmeTmrTransmitInd(&adp_info.pcb_str->mlme_tmr_transmit_ind);
            }
        }
        break;
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
        case R_G3_FUNCID_RF_MLME_GET_CNF:
        {
            if (NULL != pCb->R_G3RFMAC_MlmeGetCnf)
            {
                pCb->R_G3RFMAC_MlmeGetCnf(&adp_info.pcb_str->rf_mlme_get_cnf);
            }
        }
        break;
        case R_G3_FUNCID_RF_MLME_SET_CNF:
        {
            if (NULL != pCb->R_G3RFMAC_MlmeSetCnf)
            {
                pCb->R_G3RFMAC_MlmeSetCnf(&adp_info.pcb_str->rf_mlme_set_cnf);
            }
        }
        break;
        case R_G3_FUNCID_RF_LML_DATA_IND:
        case R_G3_FUNCID_RF_MCPS_LML_DATA_IND:
        {
            if (NULL != pCb->R_RFLMAC_LmlDataInd)
            {
                pCb->R_RFLMAC_LmlDataInd(&adp_info.pcb_str->rf_lml_data_ind);
            }
        }
        break;
#endif

        default:
        {
            /* do nothing */
        }
        break;
        }
    }

} /* R_ADP_BaseCb */
/******************************************************************************
   End of function  R_ADP_BaseCb
******************************************************************************/

/******************************************************************************
 * Function Name:R_ADP_SapInit
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_ADP_SapInit(uint8_t *preq_bin, uint16_t req_buff_size, void *pcb_str, uint16_t cb_buff_size)
{
    R_memset((void *)&adp_info, 0, sizeof(r_adp_info_t));

    if ((NULL != preq_bin) && (NULL != pcb_str))
    {
        adp_info.preq_bin = preq_bin;
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
        adp_info.pcb_str = (r_hyadp_cb_str_t *)pcb_str;
#else
        adp_info.pcb_str = (r_adp_cb_str_t *)pcb_str;
#endif
        adp_info.req_buff_size = req_buff_size;
        adp_info.cb_buff_size = cb_buff_size;
    }
}
/******************************************************************************
   End of function  R_ADP_SapInit
******************************************************************************/

/******************************************************************************
 * Function Name:adp_check_param
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
static r_result_t adp_check_param(uint8_t ch, uint16_t func_id, uint8_t *preq_buff)
{
    uint8_t g3mode = (R_G3CTRL_GetG3Mode(ch) & 0x0F);

    if (R_G3_CH_MAX <= ch)
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    if (R_G3_FUNCID_ADPM_LBP_REQ == func_id)
    {
        if (R_G3_UNITID_G3ADP != g3mode)
        {
            return R_RESULT_INVALID_REQUEST;
        }
    }
    else
    {
        if ((R_G3_UNITID_G3EAP != g3mode) && (R_G3_UNITID_G3ADP != g3mode))
        {
            return R_RESULT_INVALID_REQUEST;
        }
    }
    if (NULL == preq_buff)
    {
        return R_RESULT_INVALID_REQUEST;
    }

    return R_RESULT_SUCCESS;
} /* adp_check_param */
/******************************************************************************
   End of function  adp_check_param
******************************************************************************/

/******************************************************************************
 * Function Name:adp_process_request
 * Description : Main function for the processing of ADP requests. All commands
 *               and functions are processed in the same way.
 * Arguments : uint8_t ch: The channel used for communication
 *             uint16_t func_id: The function ID indicating the request (e.g. R_G3_FUNCID_ADPD_DATA_REQ)
 *             void *preq: A generic pointer containing information about the specific request
 * Return Value : - R_RESULT_BAD_INPUT_ARGUMENTS if the arguments are wrong or NULL
 *                - R_RESULT_SUCCESS if the conversion performed by R_ADP_ReqStr2Bin and
 *                  sending of the command succeeds.
 *                - R_RESULT_FAILED otherwise.
 ******************************************************************************/
static r_result_t adp_process_request(uint8_t ch, uint16_t func_id, void *preq)
{
    uint8_t *preq_buff = adp_info.preq_bin;
    uint16_t length = 0u;
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
/* -Wconversion false positive:
 * https://bugzilla.mozilla.org/show_bug.cgi?id=450196
 * and many others
 */
#endif
    uint16_t max_len = adp_info.req_buff_size - 2u;
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
    uint8_t cmd = (uint8_t)(func_id & 0xFFu);
    r_result_t status;

    status = adp_check_param(ch, func_id, preq_buff);
    if (R_RESULT_SUCCESS != status)
    {
        return status;
    }
    if (NULL == preq)
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    status = R_G3CTRL_SemaphoreWait();
    if (R_RESULT_SUCCESS == status)
    {
        status = R_ADP_ReqStr2Bin(func_id, preq, max_len, &preq_buff[2], &length);

        if (R_RESULT_SUCCESS == status)
        {
            preq_buff[0] = R_G3_SET_PKT_HEAD(ch, R_G3_CMDTYPE_REQ, R_G3_UNITID_G3ADP);
            preq_buff[1] = cmd;
            if (get_handle_state)
            {
                status = R_G3CTRL_SendCmd(preq_buff, length + 2u);
            }
        }

        R_G3CTRL_SemaphoreRelease();
    }

    return status;
} /* adp_process_request */
/******************************************************************************
   End of function  adp_process_request
******************************************************************************/

/******************************************************************************
 * Function Name:adp_process_request_without_payload
 * Description : Main function for the processing of ADP requests. All commands
 *               and functions are processed in the same way.
 * Arguments : uint8_t ch: The channel used for communication
 *             uint16_t func_id: The function ID indicating the request (e.g. R_G3_FUNCID_ADPD_DATA_REQ)
 * Return Value : - R_RESULT_SUCCESS if the sending of the command succeeds.
 *                - R_RESULT_FAILED otherwise.
 ******************************************************************************/
static r_result_t adp_process_request_without_payload(uint8_t ch, uint16_t func_id)
{
    uint8_t *preq_buff = adp_info.preq_bin;
    uint8_t cmd = (uint8_t)(func_id & 0xFFu);
    r_result_t status;

    /* This function shall only be called for reset and leave. */
    if ((R_G3_FUNCID_ADPM_RESET_REQ != func_id) &&
        (R_G3_FUNCID_ADPM_NETWORK_LEAVE_REQ != func_id))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    status = adp_check_param(ch, func_id, preq_buff);
    if (R_RESULT_SUCCESS != status)
    {
        return status;
    }

    status = R_G3CTRL_SemaphoreWait();
    if (R_RESULT_SUCCESS == status)
    {
        preq_buff[0] = R_G3_SET_PKT_HEAD(ch, R_G3_CMDTYPE_REQ, R_G3_UNITID_G3ADP);
        preq_buff[1] = cmd;

        status = R_G3CTRL_SendCmd(preq_buff, 2u);

        R_G3CTRL_SemaphoreRelease();
    }

    return status;
} /* adp_process_request_without_payload */
/******************************************************************************
   End of function  adp_process_request_without_payload
******************************************************************************/
