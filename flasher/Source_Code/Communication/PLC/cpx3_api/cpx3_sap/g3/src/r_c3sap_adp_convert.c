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
 * File Name : r_c3sap_adp_convert.c
 * Description : G3 ADP layer convert
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
#include "r_g3rfmac_sap.h"
#include "r_adp_sap.h"
#include "r_g3mac_binstruct.h"
#include "r_adp_binstruct.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static uint16_t adp_str2bin_adpd_data_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t adp_str2bin_adpm_discovery_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t adp_str2bin_adpm_nwk_start_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t adp_str2bin_adpm_nwk_join_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t adp_str2bin_adpm_get_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t adp_str2bin_adpm_set_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t adp_str2bin_adpm_route_dis_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t adp_str2bin_adpm_lbp_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);

static uint16_t adp_bin2str_adpd_data_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t adp_bin2str_adpm_reset_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t adp_bin2str_adpm_discovery_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t adp_bin2str_adpm_nwk_join_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t adp_bin2str_adpm_get_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t adp_bin2str_adpm_set_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t adp_bin2str_adpm_route_disc_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t adp_bin2str_adpm_path_disc_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t adp_bin2str_adpd_data_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t adp_bin2str_adpm_lbp_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t adp_bin2str_adpm_path_disc_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t adp_bin2str_adpm_nwk_status_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t adp_bin2str_adpm_eap_key_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t adp_bin2str_adpm_fcounter_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t adp_bin2str_adpm_r_update_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t adp_bin2str_adpm_rrep_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t adp_bin2str_adpm_beaconind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);

/*===========================================================================*/
/*===========================================================================*/
/* CALL G3MAC CONVERT FROM Binary to Struct for Request                      */
/*===========================================================================*/
/*===========================================================================*/

/******************************************************************************
* Function Name:R_ADP_ReqStr2Bin
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_ADP_ReqStr2Bin (uint16_t func_id, void * pin_str, uint16_t max_len, uint8_t * pout_bin, uint16_t * pout_len)
{
    r_result_t ret    = R_RESULT_FAILED;
    uint16_t   length = 0u;

    if (((NULL == pin_str) || (NULL == pout_bin)) || (NULL == pout_len))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    switch (func_id)
    {
        case R_G3_FUNCID_ADPD_DATA_REQ:
            length = adp_str2bin_adpd_data_req (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_ADPM_DISCOVERY_REQ:
            length = adp_str2bin_adpm_discovery_req (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_ADPM_NETWORK_START_REQ:
            length = adp_str2bin_adpm_nwk_start_req (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_ADPM_NETWORK_JOIN_REQ:
            length = adp_str2bin_adpm_nwk_join_req (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_ADPM_GET_REQ:
            length = adp_str2bin_adpm_get_req (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_ADPM_SET_REQ:
            length = adp_str2bin_adpm_set_req (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_ADPM_ROUTE_DISCOVERY_REQ:
        case R_G3_FUNCID_ADPM_PATH_DISCOVERY_REQ:
            length = adp_str2bin_adpm_route_dis_req (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_ADPM_LBP_REQ:
            length = adp_str2bin_adpm_lbp_req (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_ADPM_RESET_REQ:
        case R_G3_FUNCID_ADPM_NETWORK_LEAVE_REQ:
            ret    = R_RESULT_SUCCESS;
            break;

        default:
            ret    = R_RESULT_BAD_INPUT_ARGUMENTS;
            break;
    } /* switch */

    if (
        (R_RESULT_SUCCESS == ret) ||
        (length)
        )
    {
        ret       = R_RESULT_SUCCESS;
        *pout_len = length;
    }
    return ret;
} /* R_ADP_ReqStr2Bin */
/******************************************************************************
   End of function  R_ADP_ReqStr2Bin
******************************************************************************/

/*===========================================================================*/
/*===========================================================================*/
/* CALL G3LMIF CONVERT FROM STRUCT to Bin for CallBack                       */
/*===========================================================================*/
/*===========================================================================*/

/*******************************************************************************
* Function Name:R_ADP_CbBin2Str
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_ADP_CbBin2Str (uint16_t func_id, uint8_t * pin_bin, uint16_t max_len, void * pout_str, uint16_t * pout_len)
{
    r_result_t ret    = R_RESULT_FAILED;
    uint16_t   length = 0u;

    if (((NULL == pin_bin) || (NULL == pout_str)) || (NULL == pout_len))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    switch (func_id)
    {
        case R_G3_FUNCID_ADPD_DATA_CNF:
        case R_G3_FUNCID_ADPM_KEY_STATE_IND:
            length = adp_bin2str_adpd_data_cnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_ADPM_RESET_CNF:
        case R_G3_FUNCID_ADPM_NETWORK_START_CNF:
        case R_G3_FUNCID_ADPM_NETWORK_LEAVE_CNF:
        case R_G3_FUNCID_ADPM_BUFFER_IND:
        case R_G3_FUNCID_MLME_RESET_CNF:
            length = adp_bin2str_adpm_reset_cnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_ADPM_DISCOVERY_CNF:
            length = adp_bin2str_adpm_discovery_cnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_ADPM_NETWORK_JOIN_CNF:
            length = adp_bin2str_adpm_nwk_join_cnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_ADPM_GET_CNF:
            length = adp_bin2str_adpm_get_cnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_ADPM_SET_CNF:
            length = adp_bin2str_adpm_set_cnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_ADPM_ROUTE_DISCOVERY_CNF:
        case R_G3_FUNCID_ADPM_LBP_CNF:
            length = adp_bin2str_adpm_route_disc_cnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_ADPM_PATH_DISCOVERY_CNF:
            length = adp_bin2str_adpm_path_disc_cnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_ADPD_DATA_IND:
            length = adp_bin2str_adpd_data_ind (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_ADPM_LBP_IND:
            length = adp_bin2str_adpm_lbp_ind (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_ADPM_PATH_DISCOVERY_IND:
        case R_G3_FUNCID_ADPM_ROUTE_ERROR_IND:
            length = adp_bin2str_adpm_path_disc_ind (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_ADPM_NETWORK_STATUS_IND:
            length = adp_bin2str_adpm_nwk_status_ind (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_ADPM_EAP_KEY_IND:
            length = adp_bin2str_adpm_eap_key_ind (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_ADPM_FRAME_COUNTER_IND:
            length = adp_bin2str_adpm_fcounter_ind (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_ADPM_NETWORK_LEAVE_IND:
            ret    = R_RESULT_SUCCESS;
            break;

        case R_G3_FUNCID_ADPM_ROUTE_UPDATE_IND:
        case R_G3_FUNCID_ADPM_LOAD_SEQ_NUM_IND:
            length = adp_bin2str_adpm_r_update_ind (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_ADPM_RREP_IND:
            length = adp_bin2str_adpm_rrep_ind (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_ADPM_BEACON_NOTIFY_IND:
            length = adp_bin2str_adpm_beaconind (pin_bin, max_len, pout_str);
            break;
        default:
            ret    = R_RESULT_BAD_INPUT_ARGUMENTS;
            break;
    } /* switch */

    if (
        (R_RESULT_SUCCESS == ret) ||
        (length)
        )
    {
        ret       = R_RESULT_SUCCESS;
        *pout_len = length;
    }
    return ret;
} /* R_ADP_CbBin2Str */
/******************************************************************************
   End of function  R_ADP_CbBin2Str
******************************************************************************/

/*===========================================================================*/
/*===========================================================================*/
/* CALL G3MAC CONVERT FROM Binary to Struct for Request                      */
/*===========================================================================*/
/*===========================================================================*/

/******************************************************************************
* Function Name:adp_str2bin_adpd_data_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_str2bin_adpd_data_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    r_adp_adpd_data_req_t *          pstr = (r_adp_adpd_data_req_t *)pin_str;
    r_adp_adpd_data_req_bin_t *      pbin = (r_adp_adpd_data_req_bin_t *)pout_bin;
    r_adp_adpd_data_req_back_bin_t * pback;
    uint16_t                         length;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }

    length                  = (uint16_t)((sizeof (r_adp_adpd_data_req_bin_t) - R_G3CORE_AVOID_ERROR) + pstr->nsduLength);

    if (
        (max_len < length) ||
        (NULL == pstr->pNsdu)
        )
    {
        return 0u;
    }

    pbin->binNsduLength[0]  = GET_S2C0 (pstr->nsduLength);
    pbin->binNsduLength[1]  = GET_S2C1 (pstr->nsduLength);
    R_memcpy (pbin->binNsdu, pstr->pNsdu, pstr->nsduLength);

    pback = (r_adp_adpd_data_req_back_bin_t *)&pbin->binNsdu[pstr->nsduLength];
    pback->nsduHandle       = pstr->nsduHandle;
    pback->discoverRoute    = pstr->discoverRoute;
    pback->qualityOfService = pstr->qualityOfService;

    return length;
} /* adp_str2bin_adpd_data_req */
/******************************************************************************
   End of function  adp_str2bin_adpd_data_req
******************************************************************************/

/******************************************************************************
* Function Name:adp_str2bin_adpm_discovery_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_str2bin_adpm_discovery_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                         length = sizeof (r_adp_adpm_discovery_req_bin_t);
    r_adp_adpm_discovery_req_t *     pstr   = (r_adp_adpm_discovery_req_t *)pin_str;
    r_adp_adpm_discovery_req_bin_t * pbin   = (r_adp_adpm_discovery_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pbin->duration = pstr->duration;
    return length;

} /* adp_str2bin_adpm_discovery_req */
/******************************************************************************
   End of function  adp_str2bin_adpm_discovery_req
******************************************************************************/

/******************************************************************************
* Function Name:adp_str2bin_adpm_nwk_start_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_str2bin_adpm_nwk_start_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                         length = sizeof (r_adp_adpm_nwk_start_req_bin_t);
    r_adp_adpm_network_start_req_t * pstr   = (r_adp_adpm_network_start_req_t *)pin_str;
    r_adp_adpm_nwk_start_req_bin_t * pbin   = (r_adp_adpm_nwk_start_req_bin_t *)pout_bin;

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
} /* adp_str2bin_adpm_nwk_start_req */
/******************************************************************************
   End of function  adp_str2bin_adpm_nwk_start_req
******************************************************************************/

/******************************************************************************
* Function Name:adp_str2bin_adpm_nwk_join_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_str2bin_adpm_nwk_join_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                        length = sizeof (r_adp_adpm_nwk_join_req_bin_t);
    r_adp_adpm_network_join_req_t * pstr   = (r_adp_adpm_network_join_req_t *)pin_str;
    r_adp_adpm_nwk_join_req_bin_t * pbin   = (r_adp_adpm_nwk_join_req_bin_t *)pout_bin;

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
    R_memcpy (pbin->lbaAddress, pstr->lbaAddress, 2u);
    pbin->mediaType = pstr->mediaType;
    return length;

} /* adp_str2bin_adpm_nwk_join_req */
/******************************************************************************
   End of function  adp_str2bin_adpm_nwk_join_req
******************************************************************************/

/******************************************************************************
* Function Name:adp_str2bin_adpm_get_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_str2bin_adpm_get_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                   length = sizeof (r_adp_adpm_get_req_bin_t);
    r_adp_adpm_get_req_t *     pstr   = (r_adp_adpm_get_req_t *)pin_str;
    r_adp_adpm_get_req_bin_t * pbin   = (r_adp_adpm_get_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pbin->aibAttributeId          = pstr->aibAttributeId;
    pbin->binAibAttributeIndex[0] = GET_S2C0 (pstr->aibAttributeIndex);
    pbin->binAibAttributeIndex[1] = GET_S2C1 (pstr->aibAttributeIndex);
    return length;
} /* adp_str2bin_adpm_get_req */
/******************************************************************************
   End of function  adp_str2bin_adpm_get_req
******************************************************************************/

/******************************************************************************
* Function Name:adp_str2bin_adpm_set_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_str2bin_adpm_set_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                   length = sizeof (r_adp_adpm_set_req_bin_t);
    r_adp_adpm_set_req_t *     pstr   = (r_adp_adpm_set_req_t *)pin_str;
    r_adp_adpm_set_req_bin_t * pbin   = (r_adp_adpm_set_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (
        (max_len < length) ||
        (NULL == pstr->aibAttributeValue)
        )
    {
        return 0u;
    }

    pbin->aibAttributeId          = pstr->aibAttributeId;
    pbin->binAibAttributeIndex[0] = GET_S2C0 (pstr->aibAttributeIndex);
    pbin->binAibAttributeIndex[1] = GET_S2C1 (pstr->aibAttributeIndex);

    R_memcpy (pbin->binAibAttributeValue, pstr->aibAttributeValue, R_ADP_MAX_IB_SIZE);
    return length;
} /* adp_str2bin_adpm_set_req */
/******************************************************************************
   End of function  adp_str2bin_adpm_set_req
******************************************************************************/

/******************************************************************************
* Function Name:adp_str2bin_adpm_route_dis_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_str2bin_adpm_route_dis_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                          length = sizeof (r_adp_adpm_route_disc_req_bin_t);
    r_adp_adpm_route_disc_req_t *     pstr   = (r_adp_adpm_route_disc_req_t *)pin_str;
    r_adp_adpm_route_disc_req_bin_t * pbin   = (r_adp_adpm_route_disc_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pbin, pstr, sizeof (r_adp_adpm_route_disc_req_bin_t));
    return length;
}
/******************************************************************************
   End of function  adp_str2bin_adpm_route_dis_req
******************************************************************************/

/******************************************************************************
* Function Name:adp_str2bin_adpm_lbp_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_str2bin_adpm_lbp_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    r_adp_adpm_lbp_req_t *          pstr = (r_adp_adpm_lbp_req_t *)pin_str;
    r_adp_adpm_lbp_req_bin_t *      pbin = (r_adp_adpm_lbp_req_bin_t *)pout_bin;
    r_adp_adpm_lbp_req_back_bin_t * pback;
    uint16_t                        length;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }

    length                  = (uint16_t)((sizeof (r_adp_adpm_lbp_req_bin_t) - R_G3CORE_AVOID_ERROR) + pstr->nsduLength);
    if (
        (max_len < length) ||
        (NULL == pstr->pNsdu)
        )
    {
        return 0u;
    }

    pbin->dstAddrType       = pstr->dstAddrType;
    R_memcpy (pbin->dstAddr, pstr->dstAddr, 8u);

    pbin->binNsduLength[0]  = GET_S2C0 (pstr->nsduLength);
    pbin->binNsduLength[1]  = GET_S2C1 (pstr->nsduLength);
    R_memcpy (pbin->binNsdu, pstr->pNsdu, pstr->nsduLength);

    pback = (r_adp_adpm_lbp_req_back_bin_t *)&pbin->binNsdu[pstr->nsduLength];
    pback->nsduHandle       = pstr->nsduHandle;
    pback->maxHops          = pstr->maxHops;
    pback->discoverRoute    = pstr->discoverRoute;
    pback->qualityOfService = pstr->qualityOfService;
    pback->securityEnabled  = pstr->securityEnabled;
    pback->mediaType        = pstr->mediaType;
    return length;

} /* adp_str2bin_adpm_lbp_req */
/******************************************************************************
   End of function  adp_str2bin_adpm_lbp_req
******************************************************************************/


/*===========================================================================*/
/*===========================================================================*/
/* CALL G3MAC CONVERT FROM STRUCT to Bin for CallBack                       */
/*===========================================================================*/
/*===========================================================================*/

/******************************************************************************
* Function Name:adp_bin2str_adpd_data_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_bin2str_adpd_data_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                    length = sizeof (r_adp_adpd_data_cnf_t);
    r_adp_adpd_data_cnf_bin_t * pbin   = (r_adp_adpd_data_cnf_bin_t *)pin_bin;
    r_adp_adpd_data_cnf_t *     pstr   = (r_adp_adpd_data_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pstr, pbin, sizeof (r_adp_adpd_data_cnf_t));
    return length;
}
/******************************************************************************
   End of function  adp_bin2str_adpd_data_cnf
******************************************************************************/

/******************************************************************************
* Function Name:adp_bin2str_adpm_reset_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_bin2str_adpm_reset_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                     length = sizeof (r_adp_adpm_reset_cnf_t);
    r_adp_adpm_reset_cnf_bin_t * pbin   = (r_adp_adpm_reset_cnf_bin_t *)pin_bin;
    r_adp_adpm_reset_cnf_t *     pstr   = (r_adp_adpm_reset_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pstr, pbin, sizeof (r_adp_adpm_reset_cnf_t));
    return length;
}
/******************************************************************************
   End of function  adp_bin2str_adpm_reset_cnf
******************************************************************************/

/******************************************************************************
* Function Name:adp_bin2str_adpm_discovery_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_bin2str_adpm_discovery_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    r_adp_adpm_discovery_cnf_bin_t * pbin = (r_adp_adpm_discovery_cnf_bin_t *)pin_bin;
    r_adp_adpm_discovery_cnf_t *     pstr = (r_adp_adpm_discovery_cnf_t *)pout_str;
    uint16_t                         length;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }

    length              = (uint16_t)(sizeof (r_adp_adpm_discovery_cnf_t) + (pbin->PANCount * sizeof (r_adp_pan_descriptor_t)));
    if (max_len < length)
    {
        return 0u;
    }

    R_memcpy (pstr, pbin, sizeof (r_adp_adpm_discovery_cnf_bbin_t));
    pstr->PANDescriptor = (r_adp_pan_descriptor_t *)&pstr[1];

    if (pbin->PANCount)
    {
        uint8_t i;
        for (i = 0u; i < pbin->PANCount; i++)
        {
            pstr->PANDescriptor[i].panId       = R_BYTE_ArrToUInt16 (pbin->binPANDescriptor[i].binPanId);
            R_memcpy (pstr->PANDescriptor[i].address, pbin->binPANDescriptor[i].address, 2u);
            pstr->PANDescriptor[i].linkQuality = pbin->binPANDescriptor[i].linkQuality;
            pstr->PANDescriptor[i].mediaType = pbin->binPANDescriptor[i].mediaType;
            pstr->PANDescriptor[i].rcCoord     = R_BYTE_ArrToUInt16 (pbin->binPANDescriptor[i].binRcCoord);
        }
    }
    return length;
} /* adp_bin2str_adpm_discovery_cnf */
/******************************************************************************
   End of function  adp_bin2str_adpm_discovery_cnf
******************************************************************************/

/******************************************************************************
* Function Name:adp_bin2str_adpm_nwk_join_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_bin2str_adpm_nwk_join_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                        length = sizeof (r_adp_adpm_network_join_cnf_t);
    r_adp_adpm_nwk_join_cnf_bin_t * pbin   = (r_adp_adpm_nwk_join_cnf_bin_t *)pin_bin;
    r_adp_adpm_network_join_cnf_t * pstr   = (r_adp_adpm_network_join_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }

    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pstr, pbin, 3u);
    pstr->panId = R_BYTE_ArrToUInt16 (pbin->binPanId);
    pstr->mediaType = pbin->mediaType;
    return length;
} /* adp_bin2str_adpm_nwk_join_cnf */
/******************************************************************************
   End of function  adp_bin2str_adpm_nwk_join_cnf
******************************************************************************/

/******************************************************************************
* Function Name:adp_bin2str_adpm_get_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_bin2str_adpm_get_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                   length = sizeof (r_adp_adpm_get_cnf_t);
    r_adp_adpm_get_cnf_bin_t * pbin   = (r_adp_adpm_get_cnf_bin_t *)pin_bin;
    r_adp_adpm_get_cnf_t *     pstr   = (r_adp_adpm_get_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }

    if (max_len < length)
    {
        return 0u;
    }
    pstr->status            = pbin->status;
    pstr->aibAttributeId    = pbin->aibAttributeId;
    pstr->aibAttributeIndex = R_BYTE_ArrToUInt16 (pbin->binAibAttributeIndex);
    R_memcpy (pstr->aibAttributeValue, pbin->binAibAttributeValue, R_ADP_MAX_IB_SIZE);

    return length;
} /* adp_bin2str_adpm_get_cnf */
/******************************************************************************
   End of function  adp_bin2str_adpm_get_cnf
******************************************************************************/

/******************************************************************************
* Function Name:adp_bin2str_adpm_set_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_bin2str_adpm_set_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                   length = sizeof (r_adp_adpm_set_cnf_t);
    r_adp_adpm_set_cnf_bin_t * pbin   = (r_adp_adpm_set_cnf_bin_t *)pin_bin;
    r_adp_adpm_set_cnf_t *     pstr   = (r_adp_adpm_set_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status            = pbin->status;
    pstr->aibAttributeId    = pbin->aibAttributeId;
    pstr->aibAttributeIndex = R_BYTE_ArrToUInt16 (pbin->binAibAttributeIndex);
    return length;
} /* adp_bin2str_adpm_set_cnf */
/******************************************************************************
   End of function  adp_bin2str_adpm_set_cnf
******************************************************************************/

/******************************************************************************
* Function Name:adp_bin2str_adpm_route_disc_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_bin2str_adpm_route_disc_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                          length = sizeof (r_adp_adpm_route_disc_cnf_t);
    r_adp_adpm_route_disc_cnf_bin_t * pbin   = (r_adp_adpm_route_disc_cnf_bin_t *)pin_bin;
    r_adp_adpm_route_disc_cnf_t *     pstr   = (r_adp_adpm_route_disc_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pstr, pbin, sizeof (r_adp_adpm_route_disc_cnf_t));
    return length;
}
/******************************************************************************
   End of function  adp_bin2str_adpm_route_disc_cnf
******************************************************************************/

/******************************************************************************
* Function Name:adp_bin2str_adpm_path_disc_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_bin2str_adpm_path_disc_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    r_adp_adpm_path_disc_cnf_bin_t *  pbin = (r_adp_adpm_path_disc_cnf_bin_t *)pin_bin;
    r_adp_adpm_path_discovery_cnf_t * pstr = (r_adp_adpm_path_discovery_cnf_t *)pout_str;
    uint16_t                          length;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }

    length          = (uint16_t)(sizeof (r_adp_adpm_path_discovery_cnf_t) + (sizeof (r_adp_path_table_t) * pbin->pathTableEntries));
    if (max_len < length)
    {
        return 0u;
    }

    R_memcpy (pstr, pbin, sizeof (r_adp_adpm_path_disc_cnf_bbin_t));
    pstr->pathTable = (r_adp_path_table_t *)&pstr[1];

    if (pbin->pathTableEntries)
    {
        R_memcpy (pstr->pathTable, pbin->binPathTable, sizeof (r_adp_path_table_t) * pstr->pathTableEntries);
    }

    return length;
} /* adp_bin2str_adpm_path_disc_cnf */
/******************************************************************************
   End of function  adp_bin2str_adpm_path_disc_cnf
******************************************************************************/

/******************************************************************************
* Function Name:adp_bin2str_adpd_data_ind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_bin2str_adpd_data_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    r_adp_adpd_data_ind_bin_t * pbin = (r_adp_adpd_data_ind_bin_t *)pin_bin;
    r_adp_adpd_data_ind_t *     pstr = (r_adp_adpd_data_ind_t *)pout_str;
    uint16_t                    length;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }

    length                     = (uint16_t)(sizeof (r_adp_adpd_data_ind_t) + R_BYTE_ArrToUInt16 (pbin->binNsduLength));
    if (max_len < length)
    {
        return 0u;
    }

    pstr->nsduLength           = R_BYTE_ArrToUInt16 (pbin->binNsduLength);
    pstr->pNsdu                = (uint8_t *)&pstr[1];
    pstr->linkQualityIndicator = pbin->binNsdu[pstr->nsduLength];
    R_memcpy (pstr->pNsdu, pbin->binNsdu, pstr->nsduLength);

    return length;
} /* adp_bin2str_adpd_data_ind */
/******************************************************************************
   End of function  adp_bin2str_adpd_data_ind
******************************************************************************/

/******************************************************************************
* Function Name:adp_bin2str_adpm_lbp_ind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_bin2str_adpm_lbp_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    r_adp_adpm_lbp_ind_bin_t * pbin = (r_adp_adpm_lbp_ind_bin_t *)pin_bin;
    r_adp_adpm_lbp_ind_t *     pstr = (r_adp_adpm_lbp_ind_t *)pout_str;
    uint16_t                   length;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }

    length                     = (uint16_t)(sizeof (r_adp_adpm_lbp_ind_t) + R_BYTE_ArrToUInt16 (pbin->binNsduLength));
    if (max_len < length)
    {
        return 0u;
    }

    R_memcpy (pstr, pbin, 2u);
    pstr->nsduLength           = R_BYTE_ArrToUInt16 (pbin->binNsduLength);
    pstr->pNsdu                = (uint8_t *)&pstr[1];
    R_memcpy (pstr->pNsdu, pbin->binNsdu, pstr->nsduLength);

    pstr->linkQualityIndicator = pbin->binNsdu[pstr->nsduLength];
    pstr->securityEnabled      = pbin->binNsdu[pstr->nsduLength + 1];
    pstr->mediaType            = pbin->binNsdu[pstr->nsduLength + 2];
    return length;
} /* adp_bin2str_adpm_lbp_ind */
/******************************************************************************
   End of function  adp_bin2str_adpm_lbp_ind
******************************************************************************/

/******************************************************************************
* Function Name:adp_bin2str_adpm_path_disc_ind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_bin2str_adpm_path_disc_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                          length = sizeof (r_adp_adpm_path_discovery_ind_t);
    r_adp_adpm_path_disc_ind_bin_t *  pbin   = (r_adp_adpm_path_disc_ind_bin_t *)pin_bin;
    r_adp_adpm_path_discovery_ind_t * pstr   = (r_adp_adpm_path_discovery_ind_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }

    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pstr, pbin, sizeof (r_adp_adpm_path_discovery_ind_t));
    return length;
} /* adp_bin2str_adpm_path_disc_ind */
/******************************************************************************
   End of function  adp_bin2str_adpm_path_disc_ind
******************************************************************************/

/******************************************************************************
* Function Name:adp_bin2str_adpm_nwk_status_ind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_bin2str_adpm_nwk_status_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                          length = sizeof (r_adp_adpm_network_status_ind_t);
    r_adp_adpm_nwk_status_ind_bin_t * pbin   = (r_adp_adpm_nwk_status_ind_bin_t *)pin_bin;
    r_adp_adpm_network_status_ind_t * pstr   = (r_adp_adpm_network_status_ind_t *)pout_str;

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

    pstr->status        = pbin->status;
    pstr->securityLevel = pbin->securityLevel;
    pstr->keyIndex      = pbin->keyIndex;
    pstr->mediaType     = pbin->mediaType;
    return length;
} /* adp_bin2str_adpm_nwk_status_ind */
/******************************************************************************
   End of function  adp_bin2str_adpm_nwk_status_ind
******************************************************************************/

/******************************************************************************
* Function Name:adp_bin2str_adpm_eap_key_ind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_bin2str_adpm_eap_key_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                       length = sizeof (r_adp_adpm_eap_key_ind_t);
    r_adp_adpm_eap_key_ind_bin_t * pbin   = (r_adp_adpm_eap_key_ind_bin_t *)pin_bin;
    r_adp_adpm_eap_key_ind_t *     pstr   = (r_adp_adpm_eap_key_ind_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }

    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pstr, pbin, sizeof (r_adp_adpm_eap_key_ind_t));
    return length;
} /* adp_bin2str_adpm_eap_key_ind */
/******************************************************************************
   End of function  adp_bin2str_adpm_eap_key_ind
******************************************************************************/

/******************************************************************************
* Function Name:adp_bin2str_adpm_fcounter_ind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_bin2str_adpm_fcounter_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                        length = sizeof (r_adp_adpm_framecounter_ind_t);
    r_adp_adpm_fcounter_ind_bin_t * pbin   = (r_adp_adpm_fcounter_ind_bin_t *)pin_bin;
    r_adp_adpm_framecounter_ind_t * pstr   = (r_adp_adpm_framecounter_ind_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }

    if (max_len < length)
    {
        return 0u;
    }
    pstr->frameCounter = R_BYTE_ArrToUInt32 (pbin->binFrameCounter);
    pstr->mediaType = pbin->mediaType;
    return length;
} /* adp_bin2str_adpm_fcounter_ind */
/******************************************************************************
   End of function  adp_bin2str_adpm_fcounter_ind
******************************************************************************/

/******************************************************************************
* Function Name:adp_bin2str_adpm_r_update_ind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_bin2str_adpm_r_update_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t length = sizeof (r_adp_adpm_route_update_ind_t);

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
   End of function  adp_bin2str_adpm_r_update_ind
******************************************************************************/

/******************************************************************************
* Function Name:adp_bin2str_adpm_rrep_ind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_bin2str_adpm_rrep_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t length = sizeof (r_adp_adpm_rrep_ind_t);

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
   End of function  adp_bin2str_adpm_rrep_ind
******************************************************************************/

/******************************************************************************
* Function Name:adp_bin2str_adpm_beaconind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t adp_bin2str_adpm_beaconind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                         length = sizeof (r_adp_adpm_bcn_notify_ind_t);
    r_adp_adpm_bcn_nfy_ind_bin_t * pbin   = (r_adp_adpm_bcn_nfy_ind_bin_t *)pin_bin;
    r_adp_adpm_bcn_notify_ind_t *  pstr   = (r_adp_adpm_bcn_notify_ind_t *)pout_str;

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
    pstr->panDescriptor.mediaType   = pbin->panDescriptor.mediaType;
    pstr->panDescriptor.rcCoord     = R_BYTE_ArrToUInt16 (pbin->panDescriptor.binRcCoord);
    return length;
} /* adp_bin2str_adpm_beaconind */
/******************************************************************************
   End of function  adp_bin2str_adpm_beaconind
******************************************************************************/
