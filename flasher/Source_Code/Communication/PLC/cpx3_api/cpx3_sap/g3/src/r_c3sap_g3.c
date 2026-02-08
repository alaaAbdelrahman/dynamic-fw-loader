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
 * File Name : r_c3sap_g3.c
 * Description : G3 layer API
 ******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "r_c3sap_config.h"
#include "r_c3sap_sys_if.h"
#include "r_c3sap_g3.h"
#include "r_c3sap_g3_if.h"
#include "r_c3sap_g3_convert.h"
#include "r_g3_cmd.h"
#include "r_c3sap_g3_buffsize.h"
/******************************************************************************
Macro definitions
******************************************************************************/

////////////////////////////////////////////////////////////

extern uint8_t config_monitor;
uint8_t flag_G3_Init_Send = 1;
///////////////////////////////////////////////////////////////
/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct
{
    void (*R_G3_SetConfigCnf)(const r_g3_set_config_cnf_t *pcnf);
    void (*R_G3_GetConfigCnf)(const r_g3_get_config_cnf_t *pcnf);
    void (*R_G3_GetInfoCnf)(const r_g3_get_info_cnf_t *pcnf);
    void (*R_G3_ClearInfoCnf)(const r_g3_clear_info_cnf_t *pcnf);
    void (*R_G3_DumpCnf)(const r_g3_dump_cnf_t *pcnf);
    void (*R_G3_DumpAbortCnf)(const r_g3_dump_abort_cnf_t *pcnf);
    void (*R_G3_DumpInd)(const r_g3_dump_ind_t *pind);
    void (*R_G3_EventInd)(const r_g3_event_ind_t *pind);
} r_g3ctrl_callback_t;

typedef struct
{
    uint8_t g3mode[R_G3_CH_MAX];
    r_g3ctrl_callback_t cb[R_G3_CH_MAX];
    uint8_t *preq_bin;
    r_g3_cb_str_t *pcb_str;
    uint16_t req_buff_size;
    uint16_t cb_buff_size;
} r_g3_info_t;

/******************************************************************************
Private global variables and functions
******************************************************************************/
static void g3ctrl_base_cb(uint8_t *pbuff);
static r_result_t g3ctrl_check_param(uint8_t ch, uint8_t *preq_buff);
static r_result_t g3ctrl_process_request(uint8_t ch, uint16_t func_id, void *preq);
static r_result_t g3ctrl_process_request_without_payload(uint8_t ch, uint16_t func_id);

/******************************************************************************
Private global variables and functions
******************************************************************************/
static r_g3_info_t g3_info;

/* PRQA S 2814 EOF */
/*===========================================================================*/
/* Functions                                                                 */
/*===========================================================================*/

/******************************************************************************
 * Function Name:R_G3_Init
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_G3_Init(uint8_t ch, r_g3_callback_t *pcallbacks, r_g3_init_req_t *preq, uint16_t time_out)
{
    uint8_t *preq_buff = g3_info.preq_bin;
    uint16_t length = 0u;
    r_result_t status;

    status = g3ctrl_check_param(ch, preq_buff);
    if (R_RESULT_SUCCESS != status)
    {
        return status;
    }
    if ((NULL == pcallbacks) || (NULL == preq))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    if (NULL == g3_info.pcb_str)
    {
        return R_RESULT_INVALID_REQUEST;
    }

    status = R_G3CTRL_SemaphoreWait();
    if (R_RESULT_SUCCESS == status)
    {
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
/* -Wconversion false positive:
 * https://bugzilla.mozilla.org/show_bug.cgi?id=450196
 * and many others
 */
#endif
        status = R_G3CTRL_ReqStr2Bin(R_G3_FUNCID_G3_INIT_REQ, preq, g3_info.req_buff_size - 2u, &preq_buff[2], &length);
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

        if ((R_RESULT_SUCCESS == status) && (0u != length))
        {
            preq_buff[0] = R_G3_SET_PKT_HEAD(ch, R_G3_CMDTYPE_REQ, R_G3_UNITID_G3CTRL);
            preq_buff[1] = R_G3_CMDID_G3_INIT;

            /* Perform Single Send Communication and then wait for confiramtion on sending state */
            if (flag_G3_Init_Send)
            {
                g3ctrl_clear_signal(R_G3CTRL_SIGNAL_CONFIRM);
                status = R_G3CTRL_SendCmd(preq_buff, length + 2u);
            }

            if (R_RESULT_SUCCESS == status)
            {   // Send Success 
                // Wait for Confirm that Send process success from CPX3 Module
                status = monitor_SendingAdpInitCommands(R_G3CTRL_SIGNAL_CONFIRM, time_out);
                if (R_RESULT_SUCCESS == status)
                {
                    /*Send Success & Confirmed */
                    if (NULL != g3_info.pcb_str)
                    {
                        status = (r_result_t)g3_info.pcb_str->init.status;
                    }

                    if (R_RESULT_SUCCESS == status)
                    {
                        status = R_G3CTRL_RegistCb(ch, preq->g3mode, pcallbacks);
                    }
                }
            }
        }

        R_G3CTRL_SemaphoreRelease();
    }

    return status;
} /* R_G3_Init */
/******************************************************************************
   End of function  R_G3_Init
******************************************************************************/

/******************************************************************************
 * Function Name:R_G3_Deinit
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_G3_Deinit(uint8_t ch, uint16_t time_out)
{
    uint8_t *preq_buff = g3_info.preq_bin;
    r_result_t status;

    status = g3ctrl_check_param(ch, preq_buff);
    if (R_RESULT_SUCCESS != status)
    {
        return status;
    }
    if (NULL == g3_info.pcb_str)
    {
        return R_RESULT_INVALID_REQUEST;
    }

    status = R_G3CTRL_SemaphoreWait();
    if (R_RESULT_SUCCESS == status)
    {
        preq_buff[0] = R_G3_SET_PKT_HEAD(ch, R_G3_CMDTYPE_REQ, R_G3_UNITID_G3CTRL);
        preq_buff[1] = R_G3_CMDID_G3_DEINIT;

        g3ctrl_clear_signal(R_G3CTRL_SIGNAL_CONFIRM);

        status = R_G3CTRL_SendCmd(preq_buff, 2u);
        if (R_RESULT_SUCCESS == status)
        {
            /* wait confirm */
            status = g3ctrl_wait_signal(R_G3CTRL_SIGNAL_CONFIRM, time_out);
            if (R_RESULT_SUCCESS == status)
            {

                if (NULL != g3_info.pcb_str)
                {
                    status = (r_result_t)g3_info.pcb_str->deinit.status;
                }
            }
        }

        g3_info.g3mode[ch] = R_G3_UNITID_MAX;

        /* Clear registed callback */
        R_memset(&g3_info.cb[ch], 0, sizeof(r_g3ctrl_callback_t));
        R_EAP_ClearCb(ch);
        R_ADP_ClearCb(ch);
#ifndef R_DEFINE_DISABLE_UMAC
        R_G3MAC_ClearCb(ch);
#endif
        R_G3CTRL_SemaphoreRelease();
    }

    return status;
} /* R_G3_Deinit */
/******************************************************************************
   End of function  R_G3_Deinit
******************************************************************************/

/******************************************************************************
 * Function Name:R_G3_SetConfigReq
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_G3_SetConfigReq(uint8_t ch, r_g3_set_config_req_t *preq)
{
    r_result_t status;

    status = g3ctrl_process_request(ch, R_G3_FUNCID_G3_SETCONFIG_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_G3_SetConfigReq
******************************************************************************/

/******************************************************************************
 * Function Name:R_G3_GetConfigReq
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_G3_GetConfigReq(uint8_t ch)
{
    r_result_t status;

    status = g3ctrl_process_request_without_payload(ch, R_G3_FUNCID_G3_GETCONFIG_REQ);

    return status;
}
/******************************************************************************
   End of function  R_G3_GetConfigReq
******************************************************************************/

/******************************************************************************
 * Function Name:R_G3_GetInfoReq
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_G3_GetInfoReq(uint8_t ch, r_g3_get_info_req_t *preq)
{
    r_result_t status;

    status = g3ctrl_process_request(ch, R_G3_FUNCID_G3_GETINFO_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_G3_GetInfoReq
******************************************************************************/

/******************************************************************************
 * Function Name:R_G3_ClearInfoReq
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_G3_ClearInfoReq(uint8_t ch, r_g3_clear_info_req_t *preq)
{
    r_result_t status;

    status = g3ctrl_process_request(ch, R_G3_FUNCID_G3_CLEARINFO_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_G3_ClearInfoReq
******************************************************************************/

/******************************************************************************
 * Function Name:R_G3_DumpReq
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_G3_DumpReq(uint8_t ch, r_g3_dump_req_t *preq)
{
    r_result_t status;

    status = g3ctrl_process_request(ch, R_G3_FUNCID_G3_DUMP_REQ, preq);

    return status;
}
/******************************************************************************
   End of function  R_G3_DumpReq
******************************************************************************/

/******************************************************************************
 * Function Name:R_G3_DumpAbortReq
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_G3_DumpAbortReq(uint8_t ch)
{
    r_result_t status;

    status = g3ctrl_process_request_without_payload(ch, R_G3_FUNCID_G3_DUMP_ABORT_REQ);

    return status;
}
/******************************************************************************
   End of function  R_G3_DumpAbortReq
******************************************************************************/
/******************************************************************************
 * Function Name:R_G3CTRL_Init
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_G3CTRL_Init(uint8_t *preq_bin, uint16_t req_buff_size, void *pcb_str, uint16_t cb_buff_size)
{
    R_memset((void *)&g3_info, 0, sizeof(r_g3_info_t));

    if ((NULL != preq_bin) && (NULL != pcb_str))
    {
        g3ctrl_os_res_init();

        g3_info.g3mode[0] = R_G3_UNITID_MAX;
        g3_info.g3mode[1] = R_G3_UNITID_MAX;

        g3_info.preq_bin = preq_bin;
        g3_info.pcb_str = (r_g3_cb_str_t *)(pcb_str);
        g3_info.req_buff_size = req_buff_size;
        g3_info.cb_buff_size = cb_buff_size;

        R_EAP_SapInit(g3_info.preq_bin, g3_info.req_buff_size, (void *)g3_info.pcb_str, g3_info.cb_buff_size);
        R_ADP_SapInit(g3_info.preq_bin, g3_info.req_buff_size, (void *)g3_info.pcb_str, g3_info.cb_buff_size);
        R_G3MAC_SapInit(g3_info.preq_bin, g3_info.req_buff_size, (void *)g3_info.pcb_str, g3_info.cb_buff_size);
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
        R_G3RFMAC_SapInit(g3_info.preq_bin, g3_info.req_buff_size, (void *)g3_info.pcb_str, g3_info.cb_buff_size);
#endif
    }
} /* R_G3CTRL_Init */
/******************************************************************************
   End of function  R_G3CTRL_Init
******************************************************************************/

/******************************************************************************
 * Function Name:R_G3CTRL_Deinit
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_G3CTRL_Deinit(void)
{
    g3ctrl_os_res_deinit();
} /* R_G3CTRL_Deinit */
/******************************************************************************
   End of function  R_G3CTRL_Deinit
******************************************************************************/

/******************************************************************************
 * Function Name:R_G3CTRL_RegistCb
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_G3CTRL_RegistCb(uint8_t ch, uint8_t g3mode, r_g3_callback_t *pcallbacks)
{
    r_result_t status;

    if ((R_G3_CH_MAX <= ch) || (NULL == pcallbacks))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    g3_info.g3mode[ch] = g3mode;
    g3_info.cb[ch].R_G3_SetConfigCnf = pcallbacks->R_G3_SetConfigCnf;
    g3_info.cb[ch].R_G3_GetConfigCnf = pcallbacks->R_G3_GetConfigCnf;
    g3_info.cb[ch].R_G3_EventInd = pcallbacks->R_G3_EventInd;
    g3_info.cb[ch].R_G3_GetInfoCnf = pcallbacks->R_G3_GetInfoCnf;
    g3_info.cb[ch].R_G3_ClearInfoCnf = pcallbacks->R_G3_ClearInfoCnf;
    g3_info.cb[ch].R_G3_DumpCnf = pcallbacks->R_G3_DumpCnf;
    g3_info.cb[ch].R_G3_DumpAbortCnf = pcallbacks->R_G3_DumpAbortCnf;
    g3_info.cb[ch].R_G3_DumpInd = pcallbacks->R_G3_DumpInd;

    switch (g3_info.g3mode[ch] & 0x7F)
    {
    case R_G3_MODE_EAP:
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    case R_G3_MODE_HYBRID_EAP:
    case R_G3_MODE_RF_EAP:
#endif
    {
        status = R_EAP_RegistCb(ch, (void *)&pcallbacks->mode.eap);
    }
    break;
    case R_G3_MODE_ADP:
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    case R_G3_MODE_HYBRID_ADP:
    case R_G3_MODE_RF_ADP:
#endif
    {
        status = R_ADP_RegistCb(ch, (void *)&pcallbacks->mode.adp);
    }
    break;
#ifndef R_DEFINE_DISABLE_UMAC
    case R_G3_MODE_MAC:
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    case R_G3_MODE_HYBRID_MAC:
#endif
    {
        status = R_G3MAC_RegistCb(ch, (void *)&pcallbacks->mode.mac);
    }
    break;
#endif /* #ifndef R_DEFINE_DISABLE_UMAC */
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
#ifndef R_DEFINE_DISABLE_UMAC
    case R_G3_MODE_RF_MAC:
    {
        status = R_G3RFMAC_RegistCb(ch, (void *)&pcallbacks->mode.rfmac);
    }
    break;
#endif
#endif
    default:
    {
        status = R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    break;
    } /* switch */

    return status;
} /* R_G3CTRL_RegistCb */
/******************************************************************************
   End of function  R_G3CTRL_RegistCb
******************************************************************************/

/******************************************************************************
 * Function Name:R_G3CTRL_GetG3Mode
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
uint8_t R_G3CTRL_GetG3Mode(uint8_t ch)
{
    if (ch < R_G3_CH_MAX)
    {
        return g3_info.g3mode[ch];
    }
    else
    {
        return 0xFF;
    }
}
/******************************************************************************
   End of function  R_G3CTRL_GetG3Mode
******************************************************************************/

/******************************************************************************
 * Function Name:R_G3CTRL_SendCmd
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_G3CTRL_SendCmd(uint8_t *pbuffer, uint32_t size)
{
    r_result_t status;
    st_sys_send_cmd_t bin_cmd;

    if (NULL == pbuffer)
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    bin_cmd.reserved = R_FALSE;
    bin_cmd.dst_block_id = R_C3SAP_BLOCKID_UARTIF_0;
    bin_cmd.src_block_id = R_C3SAP_BLOCKID_G3;
    bin_cmd.pbuffer = pbuffer;
    bin_cmd.size = size;

#if defined(R_DEFINE_OPEN_RX)
    status = R_SYS3_SendCmd(&bin_cmd);
#else
    status = R_SYS_SendCmd(&bin_cmd);
#endif

    return status;
} /* R_G3CTRL_SendCmd */
/******************************************************************************
   End of function  R_G3CTRL_SendCmd
******************************************************************************/

/******************************************************************************
 * Function Name:R_G3CTRL_ReceiveCmd
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_G3CTRL_ReceiveCmd(uint8_t *pbuffer)
{
    uint8_t idc;
    uint8_t idp;

    if (NULL != pbuffer)
    {
        idc = R_G3_GET_PKT_IDC(pbuffer[0]);
        idp = R_G3_GET_PKT_IDP(pbuffer[0]);

        if (R_G3_UNITID_G3CTRL == idp)
        {
            g3ctrl_base_cb((uint8_t *)pbuffer);
        }
        else
        {
            switch (g3_info.g3mode[idc] & 0x7F)
            {
            case R_G3_MODE_EAP:
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
            case R_G3_MODE_HYBRID_EAP:
            case R_G3_MODE_RF_EAP:
#endif
                R_EAP_BaseCb((uint8_t *)pbuffer);
                break;

            case R_G3_MODE_ADP:
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
            case R_G3_MODE_HYBRID_ADP:
            case R_G3_MODE_RF_ADP:
#endif
                R_ADP_BaseCb((uint8_t *)pbuffer);
                break;
#ifndef R_DEFINE_DISABLE_UMAC
            case R_G3_MODE_MAC:
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
            case R_G3_MODE_HYBRID_MAC:
#endif
                R_G3MAC_BaseCb((uint8_t *)pbuffer);
                break;
#endif /* #ifndef R_DEFINE_DISABLE_UMAC */

#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
#ifndef R_DEFINE_DISABLE_UMAC
            case R_G3_MODE_RF_MAC:
                R_G3RFMAC_BaseCb((uint8_t *)pbuffer);
                break;
#endif
#endif
            default:
                break;
            }
        }
        return R_RESULT_SUCCESS;
    }
    else
    {
        return R_RESULT_ILLEGAL_NULL_POINTER;
    }
} /* R_G3CTRL_ReceiveCmd */
/******************************************************************************
   End of function  R_G3CTRL_ReceiveCmd
******************************************************************************/

/******************************************************************************
 * Function Name:g3ctrl_base_cb
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
static void g3ctrl_base_cb(uint8_t *pbuff)
{
    uint8_t idc;
    uint16_t funcId;
    uint16_t length = 0;
    uint16_t max_len = g3_info.cb_buff_size;
    r_result_t status;
    r_g3ctrl_callback_t *pCb;

    if ((NULL == pbuff) || (NULL == g3_info.pcb_str))
    {
        return;
    }
    idc = R_G3_GET_PKT_IDC(pbuff[0]);
    funcId = ((((uint16_t)pbuff[0] << 8u) | pbuff[1]) & 0x3FFFu);
    pCb = &g3_info.cb[idc];

    status = R_G3CTRL_CbBin2Str(funcId, &pbuff[2], max_len, g3_info.pcb_str, &length);

    if (R_RESULT_SUCCESS == status)
    {
        switch (funcId)
        {
        case R_G3_FUNCID_G3_INIT_CNF:
            g3ctrl_set_signal(R_G3CTRL_SIGNAL_CONFIRM);
            break;
        case R_G3_FUNCID_G3_DEINIT_CNF:
            g3ctrl_set_signal(R_G3CTRL_SIGNAL_CONFIRM);
            break;
        case R_G3_FUNCID_G3_SETCONFIG_CNF:
            if (NULL != pCb->R_G3_SetConfigCnf)
            {
                pCb->R_G3_SetConfigCnf(&g3_info.pcb_str->set_config);
            }
            break;
        case R_G3_FUNCID_G3_GETCONFIG_CNF:
            if (NULL != pCb->R_G3_GetConfigCnf)
            {
                pCb->R_G3_GetConfigCnf(&g3_info.pcb_str->get_config);
            }
            break;
        case R_G3_FUNCID_G3_GETINFO_CNF:
            if (NULL != pCb->R_G3_GetInfoCnf)
            {
                pCb->R_G3_GetInfoCnf(&g3_info.pcb_str->get_info);
            }
            break;
        case R_G3_FUNCID_G3_CLEARINFO_CNF:
            if (NULL != pCb->R_G3_ClearInfoCnf)
            {
                pCb->R_G3_ClearInfoCnf(&g3_info.pcb_str->clear_info);
            }
            break;

        case R_G3_FUNCID_G3_DUMP_CNF:
            if (NULL != pCb->R_G3_DumpCnf)
            {
                pCb->R_G3_DumpCnf(&g3_info.pcb_str->dump_cnf);
            }
            break;
        case R_G3_FUNCID_G3_DUMP_ABORT_CNF:
            if (NULL != pCb->R_G3_DumpAbortCnf)
            {
                pCb->R_G3_DumpAbortCnf(&g3_info.pcb_str->abort);
            }
            break;
        case R_G3_FUNCID_G3_DUMP_IND:
            if (NULL != pCb->R_G3_DumpInd)
            {
                pCb->R_G3_DumpInd(&g3_info.pcb_str->dump_ind);
            }
            break;
        case R_G3_FUNCID_G3_EVENT_IND:
            if (NULL != pCb->R_G3_EventInd)
            {
                pCb->R_G3_EventInd(&g3_info.pcb_str->event);
            }
            break;
        default:
            break;
        } /* switch */
    }
} /* g3ctrl_base_cb */
/******************************************************************************
   End of function  g3ctrl_base_cb
******************************************************************************/

/******************************************************************************
 * Function Name:g3ctrl_check_param
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
static r_result_t g3ctrl_check_param(uint8_t ch, uint8_t *preq_buff)
{
    if (R_G3_CH_MAX <= ch)
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    if (NULL == preq_buff)
    {
        return R_RESULT_INVALID_REQUEST;
    }

    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  g3ctrl_check_param
******************************************************************************/

/******************************************************************************
 * Function Name:g3ctrl_process_request
 * Description : Main function for the processing of G3CTRL requests. All commands
 *               and functions are processed in the same way.
 * Arguments : uint8_t ch: The channel used for communication
 *             uint16_t func_id: The function ID indicating the request (e.g. R_G3_FUNCID_G3_SETCONFIG_REQ)
 *             void *preq: A generic pointer containing information about the specific request
 * Return Value : - R_RESULT_BAD_INPUT_ARGUMENTS if the arguments are wrong or NULL
 *                - R_RESULT_SUCCESS if the conversion performed by R_G3CTRL_ReqStr2Bin and
 *                  sending of the command succeeds.
 *                - R_RESULT_FAILED otherwise.
 ******************************************************************************/
static r_result_t g3ctrl_process_request(uint8_t ch, uint16_t func_id, void *preq)
{
    uint8_t *preq_buff = g3_info.preq_bin;
    uint16_t length = 0u;
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
/* -Wconversion false positive:
 * https://bugzilla.mozilla.org/show_bug.cgi?id=450196
 * and many others
 */
#endif
    uint16_t max_len = g3_info.req_buff_size - 2u;
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
    uint8_t cmd = (uint8_t)(func_id & 0xFFu);
    r_result_t status;

    status = g3ctrl_check_param(ch, preq_buff);
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
        status = R_G3CTRL_ReqStr2Bin(func_id, preq, max_len, &preq_buff[2], &length);

        if (R_RESULT_SUCCESS == status)
        {
            preq_buff[0] = R_G3_SET_PKT_HEAD(ch, R_G3_CMDTYPE_REQ, R_G3_UNITID_G3CTRL);
            preq_buff[1] = cmd;

            status = R_G3CTRL_SendCmd(preq_buff, length + 2u);
        }

        R_G3CTRL_SemaphoreRelease();
    }

    return status;
} /* g3ctrl_process_request */
/******************************************************************************
   End of function  g3ctrl_process_request
******************************************************************************/

/******************************************************************************
 * Function Name:g3ctrl_process_request_without_payload
 * Description : Main function for the processing of G3CTRL requests. All commands
 *               and functions are processed in the same way.
 * Arguments : uint8_t ch: The channel used for communication
 *             uint16_t func_id: The function ID indicating the request (e.g. R_G3_FUNCID_G3_SETCONFIG_REQ)
 * Return Value : - R_RESULT_SUCCESS if the sending of the command succeeds.
 *                - R_RESULT_FAILED otherwise.
 ******************************************************************************/
static r_result_t g3ctrl_process_request_without_payload(uint8_t ch, uint16_t func_id)
{
    uint8_t *preq_buff = g3_info.preq_bin;
    uint8_t cmd = (uint8_t)(func_id & 0xFFu);
    r_result_t status;

    status = g3ctrl_check_param(ch, preq_buff);
    if (R_RESULT_SUCCESS != status)
    {
        return status;
    }

    status = R_G3CTRL_SemaphoreWait();
    if (R_RESULT_SUCCESS == status)
    {
        preq_buff[0] = R_G3_SET_PKT_HEAD(ch, R_G3_CMDTYPE_REQ, R_G3_UNITID_G3CTRL);
        preq_buff[1] = cmd;

        status = R_G3CTRL_SendCmd(preq_buff, 2u);

        R_G3CTRL_SemaphoreRelease();
    }

    return status;
} /* g3ctrl_process_request_without_payload */
/******************************************************************************
   End of function  g3ctrl_process_request_without_payload
******************************************************************************/
