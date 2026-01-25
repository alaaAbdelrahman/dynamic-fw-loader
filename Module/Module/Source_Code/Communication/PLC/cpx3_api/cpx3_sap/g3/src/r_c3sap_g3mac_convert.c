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
 * File Name : r_c3sap_g3mac_convert.c
 * Description : G3 MAC layer convert
 ******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "r_byte_swap.h"
#include "r_c3sap_sys_if.h"
#include "r_c3sap_g3_if.h"
#include "r_c3sap_g3_convert.h"
#include "r_g3_cmd.h"
#include "r_g3mac_sap.h"
#include "r_g3mac_binstruct.h"

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
#ifndef R_DEFINE_DISABLE_UMAC
static uint16_t g3mac_str2bin_mcps_datareq (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t g3mac_str2bin_mlme_resetreq (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
#endif
static uint16_t g3mac_str2bin_mlme_getreq (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t g3mac_str2bin_mlme_setreq (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
#ifndef R_DEFINE_DISABLE_UMAC
static uint16_t g3mac_str2bin_mlme_startreq (void * pin_str, uint16_t max_len, uint8_t * pout_bin);


static uint16_t g3mac_bin2str_mcps_datacnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t g3mac_bin2str_mcps_dataind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t g3mac_bin2str_mlme_resetcnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
#endif
static uint16_t g3mac_bin2str_mlme_getcnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t g3mac_bin2str_mlme_setcnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
#ifndef R_DEFINE_DISABLE_UMAC
static uint16_t g3mac_bin2str_mlme_scancnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
#endif
static uint16_t g3mac_bin2str_mlme_beaconind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
#ifndef R_DEFINE_DISABLE_UMAC
static uint16_t g3mac_bin2str_mlme_commind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t g3mac_bin2str_mlme_fcntind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
#endif
static uint16_t g3mac_bin2str_mlme_tmrrcvind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t g3mac_bin2str_mlme_tmrtransind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);

/*===========================================================================*/
/*===========================================================================*/
/* CALL G3MAC CONVERT FROM Binary to Struct for Request                      */
/*===========================================================================*/
/*===========================================================================*/
/******************************************************************************
* Function Name:R_G3MAC_ReqStr2Bin
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_G3MAC_ReqStr2Bin (uint16_t funcId, void * pin_str, uint16_t max_len, uint8_t * pout_bin, uint16_t * pout_len)
{
    r_result_t ret    = R_RESULT_FAILED;
    uint16_t   length = 0u;

    if (((NULL == pin_str) || (NULL == pout_bin)) || (NULL == pout_len))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    switch (funcId)
    {
#ifndef R_DEFINE_DISABLE_UMAC
        case R_G3_FUNCID_MCPS_DATA_REQ:
            length = g3mac_str2bin_mcps_datareq (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_MLME_RESET_REQ:
        case R_G3_FUNCID_MLME_SCAN_REQ:
            length = g3mac_str2bin_mlme_resetreq (pin_str, max_len, pout_bin);
            break;
#endif
        case R_G3_FUNCID_MLME_GET_REQ:
            length = g3mac_str2bin_mlme_getreq (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_MLME_SET_REQ:
            length = g3mac_str2bin_mlme_setreq (pin_str, max_len, pout_bin);
            break;
#ifndef R_DEFINE_DISABLE_UMAC
        case R_G3_FUNCID_MLME_START_REQ:
            length = g3mac_str2bin_mlme_startreq (pin_str, max_len, pout_bin);
            break;
#endif
        default:
            ret    = R_RESULT_BAD_INPUT_ARGUMENTS;
            break;
    } /* switch */

    if (length)
    {
        ret       = R_RESULT_SUCCESS;
        *pout_len = length;
    }
    return ret;
} /* R_G3MAC_ReqStr2Bin */
/******************************************************************************
   End of function  R_G3MAC_ReqStr2Bin
******************************************************************************/

/*===========================================================================*/
/*===========================================================================*/
/* CALL G3LMIF CONVERT FROM STRUCT to Bin for CallBack                       */
/*===========================================================================*/
/*===========================================================================*/
/******************************************************************************
* Function Name:R_G3MAC_CbBin2Str
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_G3MAC_CbBin2Str (uint16_t funcId, uint8_t * pin_bin, uint16_t max_len, void * pout_str, uint16_t * pout_len)
{
    r_result_t ret    = R_RESULT_FAILED;
    uint16_t   length = 0u;

    if (((NULL == pin_bin) || (NULL == pout_str)) || (NULL == pout_len))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    switch (funcId)
    {
#ifndef R_DEFINE_DISABLE_UMAC
        case R_G3_FUNCID_MCPS_DATA_CNF:
            length = g3mac_bin2str_mcps_datacnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_MCPS_DATA_IND:
            length = g3mac_bin2str_mcps_dataind (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_MLME_RESET_CNF:
            length = g3mac_bin2str_mlme_resetcnf (pin_bin, max_len, pout_str);
            break;
#endif
        case R_G3_FUNCID_MLME_GET_CNF:
            length = g3mac_bin2str_mlme_getcnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_MLME_SET_CNF:
            length = g3mac_bin2str_mlme_setcnf (pin_bin, max_len, pout_str);
            break;
#ifndef R_DEFINE_DISABLE_UMAC
        case R_G3_FUNCID_MLME_SCAN_CNF:
            length = g3mac_bin2str_mlme_scancnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_MLME_START_CNF:
            length = g3mac_bin2str_mlme_resetcnf (pin_bin, max_len, pout_str);
            break;
#endif 
        case R_G3_FUNCID_MLME_BEACON_NOTIFY_IND:
            length = g3mac_bin2str_mlme_beaconind (pin_bin, max_len, pout_str);
            break;
#ifndef R_DEFINE_DISABLE_UMAC
        case R_G3_FUNCID_MLME_COMM_STATUS_IND:
            length = g3mac_bin2str_mlme_commind (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_MLME_FRAMECOUNT_IND:
            length = g3mac_bin2str_mlme_fcntind (pin_bin, max_len, pout_str);
            break;
#endif
        case R_G3_FUNCID_MLME_TMR_RECEIVE_IND:
            length = g3mac_bin2str_mlme_tmrrcvind (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_MLME_TMR_TRANSMIT_IND:
            length = g3mac_bin2str_mlme_tmrtransind (pin_bin, max_len, pout_str);
            break;

        default:
            ret    = R_RESULT_BAD_INPUT_ARGUMENTS;
            break;
    } /* switch */

    if (length)
    {
        ret       = R_RESULT_SUCCESS;
        *pout_len = length;
    }
    return ret;
} /* R_G3MAC_CbBin2Str */
/******************************************************************************
   End of function  R_G3MAC_CbBin2Str
******************************************************************************/

/*===========================================================================*/
/*===========================================================================*/
/* CALL G3MAC CONVERT FROM Binary to Struct for Request                      */
/*===========================================================================*/
/*===========================================================================*/
#ifndef R_DEFINE_DISABLE_UMAC
/******************************************************************************
* Function Name:g3mac_str2bin_mcps_datareq
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3mac_str2bin_mcps_datareq (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    r_g3mac_mcps_data_req_t *     pstr = (r_g3mac_mcps_data_req_t *)pin_str;
    r_g3mac_mcps_data_req_bin_t * pbin = (r_g3mac_mcps_data_req_bin_t *)pout_bin;
    uint16_t                      length;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }

    length                 = (uint16_t)((sizeof (r_g3mac_mcps_data_req_bin_t) - R_G3CORE_AVOID_ERROR) + pstr->msduLength);

    if (
        (max_len < length) ||
        (NULL == pstr->pMsdu)
        )
    {
        return 0u;
    }

    pbin->srcAddrMode      = pstr->srcAddrMode;
    pbin->dstAddrMode      = pstr->dstAddrMode;
    pbin->binDstPanId[0]   = GET_S2C0 (pstr->dstPanId);
    pbin->binDstPanId[1]   = GET_S2C1 (pstr->dstPanId);
    R_memcpy (pbin->dstAddr, pstr->dstAddr, 8);
    pbin->binMsduLength[0] = GET_S2C0 (pstr->msduLength);
    pbin->binMsduLength[1] = GET_S2C1 (pstr->msduLength);

    R_memcpy (&pbin->msduHandle, &pstr->msduHandle, sizeof (r_g3mac_mcps_data_req_bbin_t));
    R_memcpy (pbin->binMsdu, pstr->pMsdu, pstr->msduLength);

    return length;
} /* g3mac_str2bin_mcps_datareq */
/******************************************************************************
   End of function  g3mac_str2bin_mcps_datareq
******************************************************************************/

/******************************************************************************
* Function Name:g3mac_str2bin_mlme_resetreq
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3mac_str2bin_mlme_resetreq (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                       length = sizeof (r_g3mac_mlme_reset_req_bin_t);
    r_g3mac_mlme_reset_req_t *     pstr   = (r_g3mac_mlme_reset_req_t *)pin_str;
    r_g3mac_mlme_reset_req_bin_t * pbin   = (r_g3mac_mlme_reset_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pbin->setDefaultPIB = pstr->setDefaultPIB;
    return length;
}
/******************************************************************************
   End of function  g3mac_str2bin_mlme_resetreq
******************************************************************************/
#endif /* #ifndef R_DEFINE_DISABLE_UMAC */
/******************************************************************************
* Function Name:g3mac_str2bin_mlme_getreq
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3mac_str2bin_mlme_getreq (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                     length = sizeof (r_g3mac_mlme_get_req_bin_t);
    r_g3mac_mlme_get_req_t *     pstr   = (r_g3mac_mlme_get_req_t *)pin_str;
    r_g3mac_mlme_get_req_bin_t * pbin   = (r_g3mac_mlme_get_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pbin->binPibAttributeId[0]    = GET_S2C0 (pstr->pibAttributeId);
    pbin->binPibAttributeId[1]    = GET_S2C1 (pstr->pibAttributeId);
    pbin->binPibAttributeIndex[0] = GET_S2C0 (pstr->pibAttributeIndex);
    pbin->binPibAttributeIndex[1] = GET_S2C1 (pstr->pibAttributeIndex);
    return length;
} /* g3mac_str2bin_mlme_getreq */
/******************************************************************************
   End of function  g3mac_str2bin_mlme_getreq
******************************************************************************/

/******************************************************************************
* Function Name:g3mac_str2bin_mlme_setreq
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3mac_str2bin_mlme_setreq (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                     length = sizeof (r_g3mac_mlme_set_req_bin_t);
    r_g3mac_mlme_set_req_t *     pstr   = (r_g3mac_mlme_set_req_t *)pin_str;
    r_g3mac_mlme_set_req_bin_t * pbin   = (r_g3mac_mlme_set_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (
        (max_len < length) ||
        (NULL == pstr->pibAttributeValue)
        )
    {
        return 0u;
    }
    pbin->binPibAttributeId[0]    = GET_S2C0 (pstr->pibAttributeId);
    pbin->binPibAttributeId[1]    = GET_S2C1 (pstr->pibAttributeId);
    pbin->binPibAttributeIndex[0] = GET_S2C0 (pstr->pibAttributeIndex);
    pbin->binPibAttributeIndex[1] = GET_S2C1 (pstr->pibAttributeIndex);
    R_memcpy (pbin->binPibAttributeValue, pstr->pibAttributeValue, R_G3MAC_MAX_IB_SIZE);

    return length;
} /* g3mac_str2bin_mlme_setreq */
/******************************************************************************
   End of function  g3mac_str2bin_mlme_setreq
******************************************************************************/
#ifndef R_DEFINE_DISABLE_UMAC
/******************************************************************************
* Function Name:g3mac_str2bin_mlme_startreq
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3mac_str2bin_mlme_startreq (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                       length = sizeof (r_g3mac_mlme_start_req_bin_t);
    r_g3mac_mlme_start_req_t *     pstr   = (r_g3mac_mlme_start_req_t *)pin_str;
    r_g3mac_mlme_start_req_bin_t * pbin   = (r_g3mac_mlme_start_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pbin->binPanId[0] = GET_S2C0 (pstr->panId);
    pbin->binPanId[1] = GET_S2C1 (pstr->panId);
    return length;
} /* g3mac_str2bin_mlme_startreq */
/******************************************************************************
   End of function  g3mac_str2bin_mlme_startreq
******************************************************************************/

/*===========================================================================*/
/*===========================================================================*/
/* CALL G3MAC CONVERT FROM STRUCT to Bin for CallBack                       */
/*===========================================================================*/
/*===========================================================================*/

/******************************************************************************
* Function Name:g3mac_bin2str_mcps_datacnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3mac_bin2str_mcps_datacnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                      length = sizeof (r_g3mac_mcps_data_cnf_t);
    r_g3mac_mcps_data_cnf_bin_t * pbin   = (r_g3mac_mcps_data_cnf_bin_t *)pin_bin;
    r_g3mac_mcps_data_cnf_t *     pstr   = (r_g3mac_mcps_data_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status     = (r_g3mac_cb_status_t)pbin->status;
    pstr->msduHandle = pbin->msduHandle;
    pstr->rsv = pbin->rsv;
    return length;
} /* g3mac_bin2str_mcps_datacnf */
/******************************************************************************
   End of function  g3mac_bin2str_mcps_datacnf
******************************************************************************/

/******************************************************************************
* Function Name:g3mac_bin2str_mcps_dataind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3mac_bin2str_mcps_dataind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    r_g3mac_mcps_data_ind_bin_t *  pbin = (r_g3mac_mcps_data_ind_bin_t *)pin_bin;
    r_g3mac_mcps_data_ind_t *      pstr = (r_g3mac_mcps_data_ind_t *)pout_str;
    r_g3mac_mcps_data_ind_bbin_t * pbin2;
    uint16_t                       length;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    length                 = (uint16_t)(sizeof (r_g3mac_mcps_data_ind_t) + R_BYTE_ArrToUInt16 (pbin->binMsduLength));
    if (max_len < length)
    {
        return 0u;
    }

    pstr->srcAddrMode      = pbin->srcAddrMode;
    pstr->srcPanId         = R_BYTE_ArrToUInt16 (pbin->binSrcPanId);
    R_memcpy (pstr->srcAddr, pbin->srcAddr, 8u);
    pstr->dstAddrMode      = pbin->dstAddrMode;
    pstr->dstPanId         = R_BYTE_ArrToUInt16 (pbin->binDstPanId);
    R_memcpy (pstr->dstAddr, pbin->dstAddr, 8u);
    pstr->msduLength       = R_BYTE_ArrToUInt16 (pbin->binMsduLength);

    pstr->pMsdu            = (uint8_t *)&pstr[1];
    R_memcpy (pstr->pMsdu, pbin->binMsdu, pstr->msduLength);

    pbin2                  = (r_g3mac_mcps_data_ind_bbin_t *)&pbin->binMsdu[pstr->msduLength];

    pstr->msduLinkQuality  = pbin2->msduLinkQuality;
    pstr->dsn              = pbin2->dsn;
    
    R_memcpy(&pstr->securityLevel, &pbin2->bin, sizeof (r_g3mac_mcps_data_ind_bbin2_t));
    
    return length;
} /* g3mac_bin2str_mcps_dataind */
/******************************************************************************
   End of function  g3mac_bin2str_mcps_dataind
******************************************************************************/

/******************************************************************************
* Function Name:g3mac_bin2str_mlme_resetcnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3mac_bin2str_mlme_resetcnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                       length = sizeof (r_g3mac_mlme_reset_cnf_t);
    r_g3mac_mlme_reset_cnf_bin_t * pbin   = (r_g3mac_mlme_reset_cnf_bin_t *)pin_bin;
    r_g3mac_mlme_reset_cnf_t *     pstr   = (r_g3mac_mlme_reset_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status = (r_g3mac_cb_status_t)pbin->status;
    return length;
}
/******************************************************************************
   End of function  g3mac_bin2str_mlme_resetcnf
******************************************************************************/
#endif /* #ifndef R_DEFINE_DISABLE_UMAC */
/******************************************************************************
* Function Name:g3mac_bin2str_mlme_getcnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3mac_bin2str_mlme_getcnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                     length = sizeof (r_g3mac_mlme_get_cnf_t);
    r_g3mac_mlme_get_cnf_bin_t * pbin   = (r_g3mac_mlme_get_cnf_bin_t *)pin_bin;
    r_g3mac_mlme_get_cnf_t *     pstr   = (r_g3mac_mlme_get_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status            = (r_g3mac_cb_status_t)pbin->status;
    pstr->pibAttributeId    = R_BYTE_ArrToUInt16 (pbin->binPibAttributeId);
    pstr->pibAttributeIndex = R_BYTE_ArrToUInt16 (pbin->binPibAttributeIndex);

    R_memcpy (pstr->pibAttributeValue, pbin->pibAttributeValue, R_G3MAC_MAX_IB_SIZE);
    return length;
} /* g3mac_bin2str_mlme_getcnf */
/******************************************************************************
   End of function  g3mac_bin2str_mlme_getcnf
******************************************************************************/

/******************************************************************************
* Function Name:g3mac_bin2str_mlme_setcnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3mac_bin2str_mlme_setcnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                     length = sizeof (r_g3mac_mlme_set_cnf_t);
    r_g3mac_mlme_set_cnf_bin_t * pbin   = (r_g3mac_mlme_set_cnf_bin_t *)pin_bin;
    r_g3mac_mlme_set_cnf_t *     pstr   = (r_g3mac_mlme_set_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status            = (r_g3mac_cb_status_t)pbin->status;
    pstr->pibAttributeId    = R_BYTE_ArrToUInt16 (pbin->binPibAttributeId);
    pstr->pibAttributeIndex = R_BYTE_ArrToUInt16 (pbin->binPibAttributeIndex);

    return length;
} /* g3mac_bin2str_mlme_setcnf */
/******************************************************************************
   End of function  g3mac_bin2str_mlme_setcnf
******************************************************************************/
#ifndef R_DEFINE_DISABLE_UMAC
/******************************************************************************
* Function Name:g3mac_bin2str_mlme_scancnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3mac_bin2str_mlme_scancnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    r_g3mac_mlme_scan_cnf_bin_t * pbin = (r_g3mac_mlme_scan_cnf_bin_t *)pin_bin;
    r_g3mac_mlme_scan_cnf_t *     pstr = (r_g3mac_mlme_scan_cnf_t *)pout_str;
    uint16_t                      length;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    length = sizeof(r_g3mac_mlme_scan_cnf_t);
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status   = (r_g3mac_cb_status_t)pbin->status;

    return length;
} /* g3mac_bin2str_mlme_scancnf */
/******************************************************************************
   End of function  g3mac_bin2str_mlme_scancnf
******************************************************************************/
#endif
/******************************************************************************
* Function Name:g3mac_bin2str_mlme_beaconind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3mac_bin2str_mlme_beaconind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                         length = sizeof (r_g3mac_mlme_bcn_notify_ind_t);
    r_g3mac_mlme_bcn_nfy_ind_bin_t * pbin   = (r_g3mac_mlme_bcn_nfy_ind_bin_t *)pin_bin;
    r_g3mac_mlme_bcn_notify_ind_t *  pstr   = (r_g3mac_mlme_bcn_notify_ind_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->phaseDiff                 = pbin->phaseDiff;
    pstr->panDescriptor.panId       = R_BYTE_ArrToUInt16 (pbin->panDescriptor.binPanId);
    R_memcpy (pstr->panDescriptor.address, pbin->panDescriptor.address, 2);
    pstr->panDescriptor.linkQuality = pbin->panDescriptor.linkQuality;
    pstr->panDescriptor.rcCoord     = R_BYTE_ArrToUInt16 (pbin->panDescriptor.binRcCoord);
    pstr->panDescriptor.mediaType   = pbin->panDescriptor.mediaType;
    return length;
} /* g3mac_bin2str_mlme_beaconind */
/******************************************************************************
   End of function  g3mac_bin2str_mlme_beaconind
******************************************************************************/
#ifndef R_DEFINE_DISABLE_UMAC
/******************************************************************************
* Function Name:g3mac_bin2str_mlme_commind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3mac_bin2str_mlme_commind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                         length = sizeof (r_g3mac_mlme_comm_status_ind_t);
    r_g3mac_mlme_com_sts_ind_bin_t * pbin   = (r_g3mac_mlme_com_sts_ind_bin_t *)pin_bin;
    r_g3mac_mlme_comm_status_ind_t * pstr   = (r_g3mac_mlme_comm_status_ind_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->panId         = R_BYTE_ArrToUInt16 (pbin->binPanId);
    pstr->srcAddrMode   = pbin->srcAddrMode;
    R_memcpy (pstr->srcAddr, pbin->srcAddr, 8u);
    pstr->dstAddrMode   = pbin->dstAddrMode;
    R_memcpy (pstr->dstAddr, pbin->dstAddr, 8u);

    pstr->status        = (r_g3mac_cb_status_t)pbin->status;
    pstr->securityLevel = pbin->securityLevel;
    pstr->keyIndex      = pbin->keyIndex;

    return length;
} /* g3mac_bin2str_mlme_commind */
/******************************************************************************
   End of function  g3mac_bin2str_mlme_commind
******************************************************************************/

/******************************************************************************
* Function Name:g3mac_bin2str_mlme_fcntind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3mac_bin2str_mlme_fcntind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                        length = sizeof (r_g3mac_mlme_framecount_ind_t);
    r_g3mac_mlme_fcount_ind_bin_t * pbin   = (r_g3mac_mlme_fcount_ind_bin_t *)pin_bin;
    r_g3mac_mlme_framecount_ind_t * pstr   = (r_g3mac_mlme_framecount_ind_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->frameCounter = R_BYTE_ArrToUInt32 (pbin->binFrameCounter);
    return length;
}
/******************************************************************************
   End of function  g3mac_bin2str_mlme_fcntind
******************************************************************************/
#endif /* #ifndef R_DEFINE_DISABLE_UMAC */
/******************************************************************************
* Function Name:g3mac_bin2str_mlme_tmrrcvind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3mac_bin2str_mlme_tmrrcvind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t length = sizeof (r_g3mac_mlme_tmr_receive_ind_t);

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pout_str, pin_bin, length);
    return length;
}
/******************************************************************************
   End of function  g3mac_bin2str_mlme_tmrrcvind
******************************************************************************/

/******************************************************************************
* Function Name:g3mac_bin2str_mlme_tmrtransind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3mac_bin2str_mlme_tmrtransind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t length = sizeof (r_g3mac_mlme_tmr_transmit_ind_t);

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pout_str, pin_bin, length);
    return length;
}
/******************************************************************************
   End of function  g3mac_bin2str_mlme_tmrtransind
******************************************************************************/

