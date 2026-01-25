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
 * File Name : r_c3sap_g3rfmac.c
 * Description : G3 MAC layer API
 ******************************************************************************/
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
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

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct
{
#ifndef R_DEFINE_DISABLE_UMAC
    r_g3_rfmac_callback_t   cb[R_G3_CH_MAX];
#endif
    uint8_t *               preq_bin;
    r_g3rfmac_cb_str_t    * pcb_str;
    uint16_t                req_buff_size;
    uint16_t                cb_buff_size;
} r_g3rfmac_info_t;

/******************************************************************************
Private global variables and functions
******************************************************************************/
static r_result_t g3rfmac_check_param (uint8_t ch, uint16_t func_id, void * preq, uint8_t * preq_buff);
static r_result_t g3rfmac_process_request (uint8_t ch, uint16_t func_id, void * preq);

static r_g3rfmac_info_t g3rfmac_info;

/* PRQA S 2814 EOF */
/*===========================================================================*/
/* Functions                                                                 */
/*===========================================================================*/
#ifndef R_DEFINE_DISABLE_UMAC
/******************************************************************************
* Function Name:R_G3RFMAC_McpsDataReq
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_G3RFMAC_McpsDataReq (uint8_t ch, r_g3rfmac_mcps_data_req_t * preq)
{
    r_result_t status;

    status = g3rfmac_process_request (ch, R_G3_FUNCID_RF_MCPS_DATA_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_G3RFMAC_McpsDataReq
******************************************************************************/

/******************************************************************************
* Function Name:R_G3RFMAC_MlmeResetReq
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_G3RFMAC_MlmeResetReq (uint8_t ch, r_g3rfmac_mlme_reset_req_t * preq)
{
    r_result_t status;

    status = g3rfmac_process_request (ch, R_G3_FUNCID_RF_MLME_RESET_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeResetReq
******************************************************************************/
#endif /* #ifndef R_DEFINE_DISABLE_UMAC */

/******************************************************************************
* Function Name:R_G3RFMAC_MlmeGetReq
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_G3RFMAC_MlmeGetReq (uint8_t ch, r_g3rfmac_mlme_get_req_t * preq)
{
    r_result_t status;

    status = g3rfmac_process_request (ch, R_G3_FUNCID_RF_MLME_GET_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeGetReq
******************************************************************************/

/******************************************************************************
* Function Name:R_G3RFMAC_MlmeSetReq
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_G3RFMAC_MlmeSetReq (uint8_t ch, r_g3rfmac_mlme_set_req_t * preq)
{
    r_result_t status;

    status = g3rfmac_process_request (ch, R_G3_FUNCID_RF_MLME_SET_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeSetReq
******************************************************************************/

#ifndef R_DEFINE_DISABLE_UMAC
/******************************************************************************
* Function Name:R_G3RFMAC_MlmeScanReq
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_G3RFMAC_MlmeScanReq (uint8_t ch, r_g3rfmac_mlme_scan_req_t * preq)
{
    r_result_t status;

    status = g3rfmac_process_request (ch, R_G3_FUNCID_RF_MLME_SCAN_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeScanReq
******************************************************************************/

/******************************************************************************
* Function Name:R_G3RFMAC_MlmeStartReq
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_G3RFMAC_MlmeStartReq (uint8_t ch, r_g3rfmac_mlme_start_req_t * preq)
{
    r_result_t status;

    status = g3rfmac_process_request (ch, R_G3_FUNCID_RF_MLME_START_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeStartReq
******************************************************************************/

/******************************************************************************
* Function Name:R_G3RFMAC_RegistCb
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_G3RFMAC_RegistCb (uint8_t ch, void * pcallbacks)
{
    if (NULL == pcallbacks)
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    g3rfmac_info.cb[ch] = *(r_g3_rfmac_callback_t*)pcallbacks;

    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_G3RFMAC_RegistCb
******************************************************************************/

/******************************************************************************
* Function Name:R_G3RFMAC_ClearCb
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_G3RFMAC_ClearCb (uint8_t ch)
{
    R_memset (&g3rfmac_info.cb[ch], 0, sizeof (r_g3_rfmac_callback_t));
    
    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_G3RFMAC_ClearCb
******************************************************************************/

/******************************************************************************
* Function Name:R_G3RFMAC_BaseCb
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_G3RFMAC_BaseCb (uint8_t * pbuff)
{
    uint8_t    idc;
    uint8_t    idp;
    uint16_t   funcId;
    uint16_t   length = 0;
    uint16_t   max_len = g3rfmac_info.cb_buff_size;
    r_result_t status;
    r_g3_rfmac_callback_t *pCb;
    
    if ((NULL == pbuff) || (NULL == g3rfmac_info.pcb_str))
    {
        return;
    }
    idc    = R_G3_GET_PKT_IDC (pbuff[0]);
    idp    = (pbuff[0] & 0x0F);
    funcId = ( (((uint16_t)pbuff[0]<<8u) | pbuff[1] ) & 0x3FFFu );
    pCb = &g3rfmac_info.cb[idc];
    
    switch( idp )
    {
        case R_G3_UNITID_G3RFMAC:
        case R_G3_UNITID_RFLMAC:
        {
            status = R_G3RFMAC_CbBin2Str (funcId, &pbuff[2], max_len, g3rfmac_info.pcb_str, &length);
        }
        break;
        default:
        {
            status = R_RESULT_FAILED;
        }
        break;
    }
    
    if( (R_RESULT_SUCCESS == status) && (0 != length) )
    {
        switch (funcId)
        {
            case R_G3_FUNCID_RF_MCPS_DATA_CNF:
            {
                if (NULL != pCb->R_G3RFMAC_McpsDataCnf)
                {
                    pCb->R_G3RFMAC_McpsDataCnf (&g3rfmac_info.pcb_str->rf_mcps_data_cnf);
                }
            }
            break;
            case R_G3_FUNCID_RF_MLME_RESET_CNF:
            {
                if (NULL != pCb->R_G3RFMAC_MlmeResetCnf)
                {
                    pCb->R_G3RFMAC_MlmeResetCnf (&g3rfmac_info.pcb_str->rf_mlme_reset_cnf);
                }
            }
            break;
            case R_G3_FUNCID_RF_MLME_GET_CNF:
            {
                if (NULL != pCb->R_G3RFMAC_MlmeGetCnf)
                {
                    pCb->R_G3RFMAC_MlmeGetCnf (&g3rfmac_info.pcb_str->rf_mlme_get_cnf);
                }
            }
            break;
            case R_G3_FUNCID_RF_MLME_SET_CNF:
            {
                if (NULL != pCb->R_G3RFMAC_MlmeSetCnf)
                {
                    pCb->R_G3RFMAC_MlmeSetCnf (&g3rfmac_info.pcb_str->rf_mlme_set_cnf);
                }
            }
            break;
            case R_G3_FUNCID_RF_MLME_SCAN_CNF:
            {
                if (NULL != pCb->R_G3RFMAC_MlmeScanCnf)
                {
                    pCb->R_G3RFMAC_MlmeScanCnf (&g3rfmac_info.pcb_str->rf_mlme_scan_cnf);
                }
            }
            break;
            case R_G3_FUNCID_RF_MLME_START_CNF:
            {
                if (NULL != pCb->R_G3RFMAC_MlmeStartCnf)
                {
                    pCb->R_G3RFMAC_MlmeStartCnf (&g3rfmac_info.pcb_str->rf_mlme_start_cnf);
                }
            }
            break;
            case R_G3_FUNCID_RF_MCPS_DATA_IND:
            {
                if (NULL != pCb->R_G3RFMAC_McpsDataInd)
                {
                    pCb->R_G3RFMAC_McpsDataInd (&g3rfmac_info.pcb_str->rf_mcps_data_ind);
                }
            }
            break;
            case R_G3_FUNCID_RF_MLME_BEACON_NOTIFY_IND:
            {
                if (NULL != pCb->R_G3RFMAC_MlmeBeaconNotifyInd)
                {
                    pCb->R_G3RFMAC_MlmeBeaconNotifyInd (&g3rfmac_info.pcb_str->rf_mlme_beacon_notify_ind);
                }
            }
            break;
            case R_G3_FUNCID_RF_MLME_COMM_STATUS_IND:
            {
                if (NULL != pCb->R_G3RFMAC_MlmeCommStatusInd)
                {
                    pCb->R_G3RFMAC_MlmeCommStatusInd (&g3rfmac_info.pcb_str->rf_mlme_comm_status_ind);
                }
            }
            break;
            case R_G3_FUNCID_RF_MLME_FRAMECOUNT_IND:
            {
                if (NULL != pCb->R_G3RFMAC_MlmeFrameCountInd)
                {
                    pCb->R_G3RFMAC_MlmeFrameCountInd (&g3rfmac_info.pcb_str->rf_mlme_frame_count_ind);
                }
            }
            break;
            case R_G3_FUNCID_RF_MLME_WARNING_IND:
            {
                if (NULL != pCb->R_G3RFMAC_MlmeWarningInd)
                {
                    pCb->R_G3RFMAC_MlmeWarningInd (&g3rfmac_info.pcb_str->rf_mlme_warning_ind);
                }
            }
            break;
            case R_G3_FUNCID_RF_LML_DATA_IND:
            case R_G3_FUNCID_RF_MCPS_LML_DATA_IND:
            {
                if (NULL != pCb->R_RFLMAC_LmlDataInd)
                {
                    pCb->R_RFLMAC_LmlDataInd (&g3rfmac_info.pcb_str->rf_lml_data_ind);
                }
            }
            break;
            default:
            {
                /* do nothing */
            }
            break;
        }
    }
} /* R_G3RFMAC_BaseCb */
/******************************************************************************
   End of function  R_G3RFMAC_BaseCb
******************************************************************************/
#endif /* #ifndef R_DEFINE_DISABLE_UMAC */

/******************************************************************************
* Function Name:R_G3RFMAC_SapInit
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_G3RFMAC_SapInit (uint8_t * preq_bin, uint16_t req_buff_size, void * pcb_str, uint16_t cb_buff_size)
{
    R_memset ((void *)&g3rfmac_info, 0, sizeof (r_g3rfmac_info_t));

    if ((NULL != preq_bin) && (NULL != pcb_str))
    {
        g3rfmac_info.preq_bin      = preq_bin;
        g3rfmac_info.pcb_str       = (r_g3rfmac_cb_str_t *)pcb_str;
        g3rfmac_info.req_buff_size = req_buff_size;
        g3rfmac_info.cb_buff_size  = cb_buff_size;
    }
}
/******************************************************************************
   End of function  R_G3RFMAC_SapInit
******************************************************************************/

/******************************************************************************
* Function Name:g3rfmac_check_param
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static r_result_t g3rfmac_check_param (uint8_t ch, uint16_t func_id, void * preq, uint8_t * preq_buff)
{
    uint8_t g3mode = R_G3CTRL_GetG3Mode (ch);

    if ((R_G3_CH_MAX  <= ch) || (NULL == preq))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    if (NULL == preq_buff)
    {
        return R_RESULT_INVALID_REQUEST;
    }

    if ((R_G3_FUNCID_RF_MLME_GET_REQ == func_id) ||
        (R_G3_FUNCID_RF_MLME_SET_REQ == func_id))
    {
        if ((R_G3_MODE_MAC > (g3mode&0x0F) ) || (R_G3_MODE_EAP < (g3mode&0x0F) ))
        {
            return R_RESULT_INVALID_REQUEST;
        }
    }
    else
    {
        if ( (R_G3_MODE_HYBRID_MAC != g3mode) && (R_G3_MODE_RF_MAC != g3mode) )
        {
            return R_RESULT_INVALID_REQUEST;
        }
    }

    return R_RESULT_SUCCESS;
} /* g3rfmac_check_param */
/******************************************************************************
   End of function  g3rfmac_check_param
******************************************************************************/

/******************************************************************************
* Function Name:g3rfmac_process_request
* Description : Main function for the processing of G3MAC requests. All commands
*               and functions are processed in the same way.
* Arguments : uint8_t ch: The channel used for communication
*             uint16_t func_id: The function ID indicating the request (e.g. R_G3_FUNCID_RF_MCPS_DATA_REQ)
*             void *preq: A generic pointer containing information about the specific request
* Return Value : - R_RESULT_INVALID_REQUEST if the request is not performed in g3mode
*                - R_RESULT_SUCCESS if the conversion performed by R_G3RFMAC_ReqStr2Bin and
*                  sending of the command succeeds.
*                - R_RESULT_FAILED otherwise.
******************************************************************************/
static r_result_t g3rfmac_process_request (uint8_t ch, uint16_t func_id, void * preq)
{
    uint8_t *  preq_buff = g3rfmac_info.preq_bin;
    uint16_t   length    = 0u;
    #if defined (__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wconversion"
    /* -Wconversion false positive: 
     * https://bugzilla.mozilla.org/show_bug.cgi?id=450196 
     * and many others
     */
    #endif
    uint16_t   max_len   = g3rfmac_info.req_buff_size - 2u;
    #if defined (__GNUC__)
    #pragma GCC diagnostic pop
    #endif
    uint8_t    cmd       = (uint8_t)(func_id & 0xFFu);
    r_result_t status;

    status = g3rfmac_check_param (ch, func_id, preq, preq_buff);
    if (R_RESULT_SUCCESS != status)
    {
        return status;
    }

    status = R_G3CTRL_SemaphoreWait ();
    if (R_RESULT_SUCCESS == status)
    {
        status = R_G3RFMAC_ReqStr2Bin (func_id, preq, max_len, &preq_buff[2], &length);

        if (R_RESULT_SUCCESS == status)
        {
            preq_buff[0] = R_G3_SET_PKT_HEAD (ch, R_G3_CMDTYPE_REQ, R_G3_UNITID_G3RFMAC);
            preq_buff[1] = cmd;

            status       = R_G3CTRL_SendCmd (preq_buff, length + 2u);
        }

        R_G3CTRL_SemaphoreRelease ();
    }

    return status;
} /* g3rfmac_process_request */
/******************************************************************************
   End of function  g3rfmac_process_request
******************************************************************************/
#endif

