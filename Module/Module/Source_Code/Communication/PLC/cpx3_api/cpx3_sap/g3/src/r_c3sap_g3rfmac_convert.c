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
 * File Name : r_c3sap_g3rfmac_convert.c
 * Description : G3 MAC layer convert
 ******************************************************************************/
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "r_byte_swap.h"
#include "r_c3sap_sys_if.h"
#include "r_c3sap_g3_if.h"
#include "r_c3sap_g3_convert.h"
#include "r_g3_cmd.h"
#include "r_g3rfmac_sap.h"
#include "r_g3rfmac_binstruct.h"
#include "r_rflmac_sap.h"
#include "r_rflmac_binstruct.h"

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
static uint16_t g3rfmac_str2bin_mcps_datareq (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t g3rfmac_str2bin_mlme_resetreq (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
#endif
static uint16_t g3rfmac_str2bin_mlme_getreq (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t g3rfmac_str2bin_mlme_setreq (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
#ifndef R_DEFINE_DISABLE_UMAC
static uint16_t g3rfmac_str2bin_mlme_startreq (void * pin_str, uint16_t max_len, uint8_t * pout_bin);

static uint16_t g3rfmac_bin2str_mcps_datacnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t g3rfmac_bin2str_mcps_dataind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t g3rfmac_bin2str_mlme_resetcnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
#endif
static uint16_t g3rfmac_bin2str_mlme_getcnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t g3rfmac_bin2str_mlme_setcnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
#ifndef R_DEFINE_DISABLE_UMAC
static uint16_t g3rfmac_bin2str_mlme_scancnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t g3rfmac_bin2str_mlme_beaconind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t g3rfmac_bin2str_mlme_commind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t g3rfmac_bin2str_mlme_fcntind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t g3rfmac_bin2str_lml_dataind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
#endif

/*===========================================================================*/
/*===========================================================================*/
/* CALL G3RFMAC CONVERT FROM Binary to Struct for Request                      */
/*===========================================================================*/
/*===========================================================================*/
/******************************************************************************
* Function Name : R_G3RFMAC_ReqStr2Bin
* Description   :
* Arguments     :
* Return Value  :
******************************************************************************/
r_result_t R_G3RFMAC_ReqStr2Bin (uint16_t funcId, void * pin_str, uint16_t max_len, uint8_t * pout_bin, uint16_t * pout_len)
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
        case R_G3_FUNCID_RF_MCPS_DATA_REQ:
            length = g3rfmac_str2bin_mcps_datareq (pin_str, max_len, pout_bin);
            break;
            
        case R_G3_FUNCID_RF_MLME_RESET_REQ:
        case R_G3_FUNCID_RF_MLME_SCAN_REQ:
            length = g3rfmac_str2bin_mlme_resetreq (pin_str, max_len, pout_bin);
            break;
#endif
        case R_G3_FUNCID_RF_MLME_GET_REQ:
            length = g3rfmac_str2bin_mlme_getreq (pin_str, max_len, pout_bin);
            break;
            
        case R_G3_FUNCID_RF_MLME_SET_REQ:
            length = g3rfmac_str2bin_mlme_setreq (pin_str, max_len, pout_bin);
            break;
#ifndef R_DEFINE_DISABLE_UMAC
        case R_G3_FUNCID_RF_MLME_START_REQ:
            length = g3rfmac_str2bin_mlme_startreq (pin_str, max_len, pout_bin);
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
} /* R_G3RFMAC_ReqStr2Bin */
/******************************************************************************
   End of function  R_GRF3MAC_ReqStr2Bin
******************************************************************************/

/*===========================================================================*/
/*===========================================================================*/
/* CALL G3LMIF CONVERT FROM STRUCT to Bin for CallBack                       */
/*===========================================================================*/
/*===========================================================================*/
/******************************************************************************
* Function Name : R_G3RFMAC_CbBin2Str
* Description   :
* Arguments     :
* Return Value  :
******************************************************************************/
r_result_t R_G3RFMAC_CbBin2Str (uint16_t funcId, uint8_t * pin_bin, uint16_t max_len, void * pout_str, uint16_t * pout_len)
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
        case R_G3_FUNCID_RF_MCPS_DATA_CNF:
            length = g3rfmac_bin2str_mcps_datacnf (pin_bin, max_len, pout_str);
            break;
            
        case R_G3_FUNCID_RF_MCPS_DATA_IND:
            length = g3rfmac_bin2str_mcps_dataind (pin_bin, max_len, pout_str);
            break;
            
        case R_G3_FUNCID_RF_MLME_RESET_CNF:
        case R_G3_FUNCID_RF_MLME_START_CNF:
        case R_G3_FUNCID_RF_MLME_WARNING_IND:
            length = g3rfmac_bin2str_mlme_resetcnf (pin_bin, max_len, pout_str);
            break;
#endif
        case R_G3_FUNCID_RF_MLME_GET_CNF:
            length = g3rfmac_bin2str_mlme_getcnf (pin_bin, max_len, pout_str);
            break;
            
        case R_G3_FUNCID_RF_MLME_SET_CNF:
            length = g3rfmac_bin2str_mlme_setcnf (pin_bin, max_len, pout_str);
            break;
#ifndef R_DEFINE_DISABLE_UMAC
        case R_G3_FUNCID_RF_MLME_SCAN_CNF:
            length = g3rfmac_bin2str_mlme_scancnf (pin_bin, max_len, pout_str);
            break;
            
        case R_G3_FUNCID_RF_MLME_BEACON_NOTIFY_IND:
            length = g3rfmac_bin2str_mlme_beaconind (pin_bin, max_len, pout_str);
            break;
            
        case R_G3_FUNCID_RF_MLME_COMM_STATUS_IND:
            length = g3rfmac_bin2str_mlme_commind (pin_bin, max_len, pout_str);
            break;
            
        case R_G3_FUNCID_RF_MLME_FRAMECOUNT_IND:
            length = g3rfmac_bin2str_mlme_fcntind (pin_bin, max_len, pout_str);
            break;
        case R_G3_FUNCID_RF_LML_DATA_IND:
        case R_G3_FUNCID_RF_MCPS_LML_DATA_IND:
            length = g3rfmac_bin2str_lml_dataind (pin_bin, max_len, pout_str);
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
} /* R_G3RFMAC_CbBin2Str */
/******************************************************************************
   End of function  R_G3RFMAC_CbBin2Str
******************************************************************************/

/*===========================================================================*/
/*===========================================================================*/
/* CALL G3RFMAC CONVERT FROM Binary to Struct for Request                    */
/*===========================================================================*/
/*===========================================================================*/
#ifndef R_DEFINE_DISABLE_UMAC
/******************************************************************************
* Function Name : g3rfmac_str2bin_mcps_datareq
* Description   :
* Arguments     :
* Return Value  :
******************************************************************************/
static uint16_t g3rfmac_str2bin_mcps_datareq (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    r_g3rfmac_mcps_data_req_t *     pstr = (r_g3rfmac_mcps_data_req_t *)pin_str;
    r_g3rfmac_mcps_data_req_bin_t * pbin = (r_g3rfmac_mcps_data_req_bin_t *)pout_bin;
    uint16_t                      length;
    
    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    
    length = (uint16_t)((sizeof (r_g3rfmac_mcps_data_req_bin_t) - R_G3CORE_AVOID_ERROR) + pstr->headerIELength + pstr->payloadIELength + pstr->msduLength);
    
    if (
        (max_len < length) ||
        (NULL == pstr->pMsdu)
        )
    {
        return 0u;
    }
    
    pbin->srcAddrMode           = pstr->srcAddrMode;
    pbin->dstAddrMode           = pstr->dstAddrMode;
    pbin->binDstPanId[0]        = GET_S2C0 (pstr->dstPanId);
    pbin->binDstPanId[1]        = GET_S2C1 (pstr->dstPanId);
    R_memcpy (pbin->dstAddr, pstr->dstAddr, 8);
    pbin->binMsduLength[0]      = GET_S2C0 (pstr->msduLength);
    pbin->binMsduLength[1]      = GET_S2C1 (pstr->msduLength);
    
    R_memcpy (&pbin->msduHandle, &pstr->msduHandle, sizeof (r_g3rfmac_mcps_data_req_bbin_t));
    
    pbin->binHeaderIELength[0]  = GET_S2C0 (pstr->headerIELength);
    pbin->binHeaderIELength[1]  = GET_S2C1 (pstr->headerIELength);
    pbin->binPayloadIELength[0] = GET_S2C0 (pstr->payloadIELength);
    pbin->binPayloadIELength[1] = GET_S2C1 (pstr->payloadIELength);
    
    if( pstr->headerIELength != 0 )
    {
        if( pstr->pHeaderIE != NULL )
        {
            R_memcpy (&pbin->bin[0], pstr->pHeaderIE,  pstr->headerIELength);
        }
        else
        {
            return 0u;
        }
    }
    
    if( pstr->payloadIELength != 0 )
    {
        if( pstr->pPayloadIE != NULL )
        {
            R_memcpy (&pbin->bin[pstr->headerIELength], pstr->pPayloadIE, pstr->payloadIELength);
        }
        else
        {
            return 0u;
        }
    }
    
    if( pstr->msduLength != 0 )
    {
        if( pstr->pMsdu != NULL )
        {
            R_memcpy (&pbin->bin[pstr->headerIELength+pstr->payloadIELength], pstr->pMsdu, pstr->msduLength);
        }
        else
        {
            return 0u;
        }
    }
    
    return length;
} /* g3rfmac_str2bin_mcps_datareq */
/******************************************************************************
   End of function  g3rfmac_str2bin_mcps_datareq
******************************************************************************/

/******************************************************************************
* Function Name : g3rfmac_str2bin_mlme_resetreq
* Description   :
* Arguments     :
* Return Value  :
******************************************************************************/
static uint16_t g3rfmac_str2bin_mlme_resetreq (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                       length = (uint16_t)sizeof (r_g3rfmac_mlme_reset_req_bin_t);
    r_g3rfmac_mlme_reset_req_t *     pstr = (r_g3rfmac_mlme_reset_req_t *)pin_str;
    r_g3rfmac_mlme_reset_req_bin_t * pbin = (r_g3rfmac_mlme_reset_req_bin_t *)pout_bin;
    
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
   End of function  g3rfmac_str2bin_mlme_resetreq
******************************************************************************/
#endif /* #ifndef R_DEFINE_DISABLE_UMAC */
/******************************************************************************
* Function Name : g3rfmac_str2bin_mlme_getreq
* Description   :
* Arguments     :
* Return Value  :
******************************************************************************/
static uint16_t g3rfmac_str2bin_mlme_getreq (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                     length = (uint16_t)sizeof (r_g3rfmac_mlme_get_req_bin_t);
    r_g3rfmac_mlme_get_req_t *     pstr = (r_g3rfmac_mlme_get_req_t *)pin_str;
    r_g3rfmac_mlme_get_req_bin_t * pbin = (r_g3rfmac_mlme_get_req_bin_t *)pout_bin;
    
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
} /* g3rfmac_str2bin_mlme_getreq */
/******************************************************************************
   End of function  g3rfmac_str2bin_mlme_getreq
******************************************************************************/

/******************************************************************************
* Function Name : g3rfmac_str2bin_mlme_setreq
* Description   :
* Arguments     :
* Return Value  :
******************************************************************************/
static uint16_t g3rfmac_str2bin_mlme_setreq (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                     length = (uint16_t)sizeof (r_g3rfmac_mlme_set_req_bin_t);
    r_g3rfmac_mlme_set_req_t *     pstr = (r_g3rfmac_mlme_set_req_t *)pin_str;
    r_g3rfmac_mlme_set_req_bin_t * pbin = (r_g3rfmac_mlme_set_req_bin_t *)pout_bin;
    
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
    R_memcpy (pbin->binPibAttributeValue, pstr->pibAttributeValue, R_G3RFMAC_MAX_IB_SIZE);
    
    return length;
} /* g3rfmac_str2bin_mlme_setreq */
/******************************************************************************
   End of function  g3rfmac_str2bin_mlme_setreq
******************************************************************************/
#ifndef R_DEFINE_DISABLE_UMAC
/******************************************************************************
* Function Name :g3rfmac_str2bin_mlme_startreq
* Description   :
* Arguments     :
* Return Value  :
******************************************************************************/
static uint16_t g3rfmac_str2bin_mlme_startreq (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                       length = (uint16_t)sizeof (r_g3rfmac_mlme_start_req_bin_t);
    r_g3rfmac_mlme_start_req_t *     pstr = (r_g3rfmac_mlme_start_req_t *)pin_str;
    r_g3rfmac_mlme_start_req_bin_t * pbin = (r_g3rfmac_mlme_start_req_bin_t *)pout_bin;
    
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
} /* g3rfmac_str2bin_mlme_startreq */
/******************************************************************************
   End of function  g3rfmac_str2bin_mlme_startreq
******************************************************************************/

/*===========================================================================*/
/*===========================================================================*/
/* CALL G3RFMAC CONVERT FROM STRUCT to Bin for CallBack                       */
/*===========================================================================*/
/*===========================================================================*/

/******************************************************************************
* Function Name : g3rfmac_bin2str_mcps_datacnf
* Description   :
* Arguments     :
* Return Value  :
******************************************************************************/
static uint16_t g3rfmac_bin2str_mcps_datacnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                      length = (uint16_t)sizeof (r_g3rfmac_mcps_data_cnf_t);
    r_g3rfmac_mcps_data_cnf_bin_t * pbin = (r_g3rfmac_mcps_data_cnf_bin_t *)pin_bin;
    r_g3rfmac_mcps_data_cnf_t *     pstr = (r_g3rfmac_mcps_data_cnf_t *)pout_str;
    
    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status     = (r_g3rfmac_cb_status_t)pbin->status;
    pstr->msduHandle = pbin->msduHandle;
    
    return length;
} /* g3rfmac_bin2str_mcps_datacnf */
/******************************************************************************
   End of function  g3rfmac_bin2str_mcps_datacnf
******************************************************************************/

/******************************************************************************
* Function Name : g3rfmac_bin2str_mcps_dataind
* Description   :
* Arguments     :
* Return Value  :
******************************************************************************/
static uint16_t g3rfmac_bin2str_mcps_dataind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    r_g3rfmac_mcps_data_ind_bin_t *  pbin = (r_g3rfmac_mcps_data_ind_bin_t *)pin_bin;
    r_g3rfmac_mcps_data_ind_t *      pstr = (r_g3rfmac_mcps_data_ind_t *)pout_str;
    uint16_t                       length;
    
    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    length = (uint16_t)(sizeof (r_g3rfmac_mcps_data_ind_t) + R_BYTE_ArrToUInt16 (pbin->binHeaderIELength) + R_BYTE_ArrToUInt16 (pbin->binPayloadIELength) + R_BYTE_ArrToUInt16 (pbin->binMsduLength));
    if (max_len < length)
    {
        return 0u;
    }
    
    pstr->srcAddrMode           = pbin->srcAddrMode;
    pstr->srcPanId              = R_BYTE_ArrToUInt16 (pbin->binSrcPanId);
    R_memcpy (pstr->srcAddr, pbin->srcAddr, 8u);
    pstr->dstAddrMode           = pbin->dstAddrMode;
    pstr->dstPanId              = R_BYTE_ArrToUInt16 (pbin->binDstPanId);
    R_memcpy (pstr->dstAddr, pbin->dstAddr, 8u);
    pstr->msduLength            = R_BYTE_ArrToUInt16 (pbin->binMsduLength);
    pstr->msduLinkQuality       = pbin->msduLinkQuality;
    pstr->dsn                   = pbin->dsn;
    pstr->secured               = pbin->secured;
    pstr->keyIndex              = pbin->keyIndex;
    pstr->headerIELength        = R_BYTE_ArrToUInt16 (pbin->binHeaderIELength);
    pstr->payloadIELength       = R_BYTE_ArrToUInt16 (pbin->binPayloadIELength);
    
    pstr->pHeaderIE             = (uint8_t *)&pstr[1];
    R_memcpy (pstr->pHeaderIE, &pbin->bin[0], pstr->headerIELength);
    
    pstr->pPayloadIE            = &pstr->pHeaderIE[pstr->headerIELength];
    R_memcpy (pstr->pPayloadIE, &pbin->bin[pstr->headerIELength], pstr->payloadIELength);
    
    pstr->pMsdu                 = &pstr->pPayloadIE[pstr->payloadIELength];
    R_memcpy (pstr->pMsdu, &pbin->bin[pstr->headerIELength+pstr->payloadIELength], pstr->msduLength);
    
    return length;
} /* g3rfmac_bin2str_mcps_dataind */
/******************************************************************************
   End of function  g3rfmac_bin2str_mcps_dataind
******************************************************************************/

/******************************************************************************
* Function Name : g3rfmac_bin2str_mlme_resetcnf
* Description   :
* Arguments     :
* Return Value  :
******************************************************************************/
static uint16_t g3rfmac_bin2str_mlme_resetcnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                       length = (uint16_t)sizeof (r_g3rfmac_mlme_reset_cnf_t);
    r_g3rfmac_mlme_reset_cnf_bin_t * pbin = (r_g3rfmac_mlme_reset_cnf_bin_t *)pin_bin;
    r_g3rfmac_mlme_reset_cnf_t *     pstr = (r_g3rfmac_mlme_reset_cnf_t *)pout_str;
    
    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status = (r_g3rfmac_cb_status_t)pbin->status;
    return length;
}
/******************************************************************************
   End of function  g3rfmac_bin2str_mlme_resetcnf
******************************************************************************/
#endif
/******************************************************************************
* Function Name : g3rfmac_bin2str_mlme_getcnf
* Description   :
* Arguments     :
* Return Value  :
******************************************************************************/
static uint16_t g3rfmac_bin2str_mlme_getcnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                     length = (uint16_t)sizeof (r_g3rfmac_mlme_get_cnf_t);
    r_g3rfmac_mlme_get_cnf_bin_t * pbin = (r_g3rfmac_mlme_get_cnf_bin_t *)pin_bin;
    r_g3rfmac_mlme_get_cnf_t *     pstr = (r_g3rfmac_mlme_get_cnf_t *)pout_str;
    
    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status            = (r_g3rfmac_cb_status_t)pbin->status;
    pstr->pibAttributeId    = R_BYTE_ArrToUInt16 (pbin->binPibAttributeId);
    pstr->pibAttributeIndex = R_BYTE_ArrToUInt16 (pbin->binPibAttributeIndex);
    
    R_memcpy (pstr->pibAttributeValue, pbin->pibAttributeValue, R_G3RFMAC_MAX_IB_SIZE);
    return length;
} /* g3rfmac_bin2str_mlme_getcnf */
/******************************************************************************
   End of function  g3rfmac_bin2str_mlme_getcnf
******************************************************************************/

/******************************************************************************
* Function Name : g3rfmac_bin2str_mlme_setcnf
* Description   :
* Arguments     :
* Return Value  :
******************************************************************************/
static uint16_t g3rfmac_bin2str_mlme_setcnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                     length = (uint16_t)sizeof (r_g3rfmac_mlme_set_cnf_t);
    r_g3rfmac_mlme_set_cnf_bin_t * pbin = (r_g3rfmac_mlme_set_cnf_bin_t *)pin_bin;
    r_g3rfmac_mlme_set_cnf_t *     pstr = (r_g3rfmac_mlme_set_cnf_t *)pout_str;
    
    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status            = (r_g3rfmac_cb_status_t)pbin->status;
    pstr->pibAttributeId    = R_BYTE_ArrToUInt16 (pbin->binPibAttributeId);
    pstr->pibAttributeIndex = R_BYTE_ArrToUInt16 (pbin->binPibAttributeIndex);
    
    return length;
} /* g3rfmac_bin2str_mlme_setcnf */
/******************************************************************************
   End of function  g3rfmac_bin2str_mlme_setcnf
******************************************************************************/
#ifndef R_DEFINE_DISABLE_UMAC
/******************************************************************************
* Function Name : g3rfmac_bin2str_mlme_scancnf
* Description   :
* Arguments     :
* Return Value  :
******************************************************************************/
static uint16_t g3rfmac_bin2str_mlme_scancnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    r_g3rfmac_mlme_scan_cnf_bin_t * pbin = (r_g3rfmac_mlme_scan_cnf_bin_t *)pin_bin;
    r_g3rfmac_mlme_scan_cnf_t *     pstr = (r_g3rfmac_mlme_scan_cnf_t *)pout_str;
    uint16_t                      length;
    
    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    length = (uint16_t)sizeof(r_g3rfmac_mlme_scan_cnf_t);
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status   = (r_g3rfmac_cb_status_t)pbin->status;
    
    return length;
} /* g3rfmac_bin2str_mlme_scancnf */
/******************************************************************************
   End of function  g3rfmac_bin2str_mlme_scancnf
******************************************************************************/

/******************************************************************************
* Function Name : g3rfmac_bin2str_mlme_beaconind
* Description   :
* Arguments     :
* Return Value  :
******************************************************************************/
static uint16_t g3rfmac_bin2str_mlme_beaconind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    r_g3rfmac_mlme_bcn_nfy_ind_bin_t * pbin   = (r_g3rfmac_mlme_bcn_nfy_ind_bin_t *)pin_bin;
    r_g3rfmac_mlme_bcn_notify_ind_t  * pstr   = (r_g3rfmac_mlme_bcn_notify_ind_t *)pout_str;
    uint16_t                           length;
    
    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    
    length = (uint16_t)sizeof(r_g3rfmac_mlme_bcn_notify_ind_t) + pbin->beaconPayloadLength;
    if (max_len < length)
    {
        return 0u;
    }
    
    pstr->panDescriptor.panId       = R_BYTE_ArrToUInt16( pbin->binPanDescriptor.binPanId );
    pstr->panDescriptor.address     = R_BYTE_ArrToUInt16(pbin->binPanDescriptor.binAddress);
    pstr->panDescriptor.linkQuality = pbin->binPanDescriptor.linkQuality;
    
    pstr->ebsn                      = pbin->ebsn;
    pstr->beaconPayloadLength       = pbin->beaconPayloadLength;
    pstr->pBeaconPayload            = (uint8_t*)&pstr[1];
    R_memcpy (pstr->pBeaconPayload, pbin->binBeaconPayload, pstr->beaconPayloadLength);
    
    return length;
} /* g3rfmac_bin2str_mlme_beaconind */
/******************************************************************************
   End of function  g3rfmac_bin2str_mlme_beaconind
******************************************************************************/

/******************************************************************************
* Function Name : g3rfmac_bin2str_mlme_commind
* Description   :
* Arguments     :
* Return Value  :
******************************************************************************/
static uint16_t g3rfmac_bin2str_mlme_commind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                         length = (uint16_t)sizeof (r_g3rfmac_mlme_comm_status_ind_t);
    r_g3rfmac_mlme_com_sts_ind_bin_t * pbin = (r_g3rfmac_mlme_com_sts_ind_bin_t *)pin_bin;
    r_g3rfmac_mlme_comm_status_ind_t * pstr = (r_g3rfmac_mlme_comm_status_ind_t *)pout_str;
    
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
    
    pstr->status        = (r_g3rfmac_cb_status_t)pbin->status;
    pstr->secured       = pbin->secured;
    pstr->keyIndex      = pbin->keyIndex;
    
    return length;
} /* g3rfmac_bin2str_mlme_commind */
/******************************************************************************
   End of function  g3rfmac_bin2str_mlme_commind
******************************************************************************/

/******************************************************************************
* Function Name:g3rfmac_bin2str_mlme_fcntind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3rfmac_bin2str_mlme_fcntind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                        length = (uint16_t)sizeof (r_g3rfmac_mlme_framecount_ind_t);
    r_g3rfmac_mlme_fcount_ind_bin_t * pbin = (r_g3rfmac_mlme_fcount_ind_bin_t *)pin_bin;
    r_g3rfmac_mlme_framecount_ind_t * pstr = (r_g3rfmac_mlme_framecount_ind_t *)pout_str;
    
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
   End of function  g3rfmac_bin2str_mlme_fcntind
******************************************************************************/

/******************************************************************************
* Function Name:g3rfmac_bin2str_mlme_fcntind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3rfmac_bin2str_lml_dataind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                        length;
    r_rflmac_lml_data_ind_bin_t     * pbin = (r_rflmac_lml_data_ind_bin_t *)pin_bin;
    r_rflmac_lml_data_ind_t         * pstr = (r_rflmac_lml_data_ind_t *)pout_str;
    
    length = (uint16_t)sizeof (r_rflmac_lml_data_ind_t) + R_BYTE_ArrToUInt16 (pbin->binPsduLen);
     
    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->rxTime         = R_BYTE_ArrToUInt32 (pbin->binRxTime);
    pstr->lqi            = pbin->binLqi;
    pstr->crc            = pbin->binCrc;
    pstr->phr            = pbin->binPhr;
    pstr->status         = pbin->binStatus;
    pstr->rssi           = R_BYTE_ArrToUInt16 (pbin->binRssi);
    pstr->psduLen        = R_BYTE_ArrToUInt16 (pbin->binPsduLen);
    pstr->currentChannel = pbin->binCurrentChannel;
    pstr->psdu           = (uint8_t*)&pstr[1];
    R_memcpy (pstr->psdu, pbin->binPsdu, pstr->psduLen);
    
    return length;
}
/******************************************************************************
   End of function  g3rfmac_bin2str_mlme_fcntind
******************************************************************************/
#endif /* #ifndef R_DEFINE_DISABLE_UMAC */
#endif /* R_DEFINE_DISABLE_HYBRID */
