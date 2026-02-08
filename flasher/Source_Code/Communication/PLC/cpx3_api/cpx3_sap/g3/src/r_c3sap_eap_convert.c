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
 * File Name : r_c3sap_eap_convert.c
 * Description : G3 AEAP layer convert
 ******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "r_byte_swap.h"
#include "r_g3_cmd.h"
#include "r_c3sap_g3_if.h"
#include "r_c3sap_sys_if.h"
#include "r_c3sap_g3_convert.h"
#include "r_g3mac_sap.h"
#include "r_g3rfmac_sap.h"
#include "r_adp_sap.h"
#include "r_eap_sap.h"
#include "r_eap_binstruct.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static uint16_t eap_str2bin_eapm_get_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t eap_str2bin_eapm_set_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t eap_str2bin_eapm_network_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t eap_str2bin_eapm_set_cinfo_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);

static uint16_t eap_bin2str_eapm_reset_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t eap_bin2str_eapm_get_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t eap_bin2str_eapm_set_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t eap_bin2str_eapm_network_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t eap_bin2str_eapm_set_cinfo_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t eap_bin2str_eapm_nwk_join_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t eap_bin2str_eapm_nwk_leave_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t eap_bin2str_eapm_eap_key_ind(uint8_t * pin_bin, uint16_t max_len, void * pout_str);

/*===========================================================================*/
/*===========================================================================*/
/* CALL EAP CONVERT FROM Binary to Struct for Request                        */
/*===========================================================================*/
/*===========================================================================*/
/******************************************************************************
* Function Name:R_EAP_ReqStr2Bin
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_EAP_ReqStr2Bin (uint16_t funcId, void * pin_str, uint16_t max_len, uint8_t * pout_bin, uint16_t * pout_len)
{
    r_result_t ret    = R_RESULT_FAILED;
    uint16_t   length = 0u;

    if (((NULL == pin_str) || (NULL == pout_bin)) || (NULL == pout_len))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    switch (funcId)
    {
        case R_G3_FUNCID_EAPM_GET_REQ:
            length = eap_str2bin_eapm_get_req (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_EAPM_SET_REQ:
            length = eap_str2bin_eapm_set_req (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_EAPM_NETWORK_REQ:
            length = eap_str2bin_eapm_network_req (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_EAPM_SETCLIENTINFO_REQ:
            length = eap_str2bin_eapm_set_cinfo_req (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_EAPM_RESET_REQ:
        case R_G3_FUNCID_EAPM_START_REQ:
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
} /* R_EAP_ReqStr2Bin */
/******************************************************************************
   End of function  R_EAP_ReqStr2Bin
******************************************************************************/

/*===========================================================================*/
/*===========================================================================*/
/* CALL G3LMIF CONVERT FROM STRUCT to Bin for CallBack                       */
/*===========================================================================*/
/*===========================================================================*/

/******************************************************************************
* Function Name:R_EAP_CbBin2Str
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_EAP_CbBin2Str (uint16_t funcId, uint8_t * pin_bin, uint16_t max_len, void * pout_str, uint16_t * pout_len)
{
    r_result_t ret    = R_RESULT_FAILED;
    uint16_t   length = 0u;

    if (((NULL == pin_bin) || (NULL == pout_str)) || (NULL == pout_len))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    switch (funcId)
    {
        case R_G3_FUNCID_EAPM_RESET_CNF:
        case R_G3_FUNCID_EAPM_START_CNF:
            length = eap_bin2str_eapm_reset_cnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_EAPM_GET_CNF:
            length = eap_bin2str_eapm_get_cnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_EAPM_SET_CNF:
            length = eap_bin2str_eapm_set_cnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_EAPM_NETWORK_CNF:
            length = eap_bin2str_eapm_network_cnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_EAPM_SETCLIENTINFO_CNF:
            length = eap_bin2str_eapm_set_cinfo_cnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_EAPM_NETWORK_JOIN_IND:
            length = eap_bin2str_eapm_nwk_join_ind (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_EAPM_NETWORK_LEAVE_IND:
        case R_G3_FUNCID_EAPM_NEWDEVICE_IND:
            length = eap_bin2str_eapm_nwk_leave_ind (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_EAPM_EAP_KEY_IND:
            length = eap_bin2str_eapm_eap_key_ind (pin_bin, max_len, pout_str);
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
} /* R_EAP_CbBin2Str */
/******************************************************************************
   End of function  R_EAP_CbBin2Str
******************************************************************************/

/*===========================================================================*/
/*===========================================================================*/
/* CALL G3MAC CONVERT FROM Binary to Struct for Request                      */
/*===========================================================================*/
/*===========================================================================*/

/******************************************************************************
* Function Name:eap_str2bin_eapm_get_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t eap_str2bin_eapm_get_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                   length = sizeof (r_eap_eapm_get_req_bin_t);
    r_eap_eapm_get_req_t *     pstr   = (r_eap_eapm_get_req_t *)pin_str;
    r_eap_eapm_get_req_bin_t * pbin   = (r_eap_eapm_get_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pbin->eibAttributeId          = pstr->eibAttributeId;
    pbin->binEibAttributeIndex[0] = GET_S2C0 (pstr->eibAttributeIndex);
    pbin->binEibAttributeIndex[1] = GET_S2C1 (pstr->eibAttributeIndex);
    return length;
} /* eap_str2bin_eapm_get_req */
/******************************************************************************
   End of function  eap_str2bin_eapm_get_req
******************************************************************************/

/******************************************************************************
* Function Name:eap_str2bin_eapm_set_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t eap_str2bin_eapm_set_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                   length = sizeof (r_eap_eapm_set_req_bin_t);
    r_eap_eapm_set_req_t *     pstr   = (r_eap_eapm_set_req_t *)pin_str;
    r_eap_eapm_set_req_bin_t * pbin   = (r_eap_eapm_set_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (
        (max_len < length) ||
        (NULL == pstr->eibAttributeValue)
        )
    {
        return 0u;
    }

    pbin->eibAttributeId          = pstr->eibAttributeId;
    pbin->binEibAttributeIndex[0] = GET_S2C0 (pstr->eibAttributeIndex);
    pbin->binEibAttributeIndex[1] = GET_S2C1 (pstr->eibAttributeIndex);

    R_memcpy (pbin->binEibAttributeValue, pstr->eibAttributeValue, R_EAP_MAX_IB_SIZE);
    return length;
} /* eap_str2bin_eapm_set_req */
/******************************************************************************
   End of function  eap_str2bin_eapm_set_req
******************************************************************************/

/******************************************************************************
* Function Name:eap_str2bin_eapm_network_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t eap_str2bin_eapm_network_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                       length = sizeof (r_eap_eapm_network_req_bin_t);
    r_eap_eapm_network_req_t *     pstr   = (r_eap_eapm_network_req_t *)pin_str;
    r_eap_eapm_network_req_bin_t * pbin   = (r_eap_eapm_network_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pbin, pstr, sizeof (r_eap_eapm_network_req_bin_t));
    return length;
}
/******************************************************************************
   End of function  eap_str2bin_eapm_network_req
******************************************************************************/

/******************************************************************************
* Function Name:eap_str2bin_eapm_set_cinfo_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t eap_str2bin_eapm_set_cinfo_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                          length = sizeof (r_eap_eapm_set_cinfo_req_bin_t);
    r_eap_eapm_set_clientinfo_req_t * pstr   = (r_eap_eapm_set_clientinfo_req_t *)pin_str;
    r_eap_eapm_set_cinfo_req_bin_t *  pbin   = (r_eap_eapm_set_cinfo_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pbin, pstr, length);
    return length;
} /* eap_str2bin_eapm_set_cinfo_req */
/******************************************************************************
   End of function  eap_str2bin_eapm_set_cinfo_req
******************************************************************************/


/*===========================================================================*/
/*===========================================================================*/
/* CALL G3MAC CONVERT FROM STRUCT to Bin for CallBack                        */
/*===========================================================================*/
/*===========================================================================*/

/******************************************************************************
* Function Name:eap_bin2str_eapm_reset_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t eap_bin2str_eapm_reset_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                     length = sizeof (r_eap_eapm_reset_cnf_t);
    r_eap_eapm_reset_cnf_bin_t * pbin   = (r_eap_eapm_reset_cnf_bin_t *)pin_bin;
    r_eap_eapm_reset_cnf_t *     pstr   = (r_eap_eapm_reset_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pstr, pbin, sizeof (r_eap_eapm_reset_cnf_t));
    return length;

} /* eap_bin2str_eapm_reset_cnf */
/******************************************************************************
   End of function  eap_bin2str_eapm_reset_cnf
******************************************************************************/

/******************************************************************************
* Function Name:eap_bin2str_eapm_get_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t eap_bin2str_eapm_get_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                   length = sizeof (r_eap_eapm_get_cnf_t);
    r_eap_eapm_get_cnf_bin_t * pbin   = (r_eap_eapm_get_cnf_bin_t *)pin_bin;
    r_eap_eapm_get_cnf_t *     pstr   = (r_eap_eapm_get_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status            = pbin->status;
    pstr->eibAttributeId    = pbin->eibAttributeId;
    pstr->eibAttributeIndex = R_BYTE_ArrToUInt16 (pbin->binEibAttributeIndex);
    R_memcpy (pstr->eibAttributeValue, pbin->binEibAttributeValue, R_EAP_MAX_IB_SIZE);

    return length;
} /* eap_bin2str_eapm_get_cnf */
/******************************************************************************
   End of function  eap_bin2str_eapm_get_cnf
******************************************************************************/

/******************************************************************************
* Function Name:eap_bin2str_eapm_set_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t eap_bin2str_eapm_set_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                   length = sizeof (r_eap_eapm_set_cnf_t);
    r_eap_eapm_set_cnf_bin_t * pbin   = (r_eap_eapm_set_cnf_bin_t *)pin_bin;
    r_eap_eapm_set_cnf_t *     pstr   = (r_eap_eapm_set_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status            = pbin->status;
    pstr->eibAttributeId    = pbin->eibAttributeId;
    pstr->eibAttributeIndex = R_BYTE_ArrToUInt16 (pbin->binEibAttributeIndex);
    return length;
} /* eap_bin2str_eapm_set_cnf */
/******************************************************************************
   End of function  eap_bin2str_eapm_set_cnf
******************************************************************************/

/******************************************************************************
* Function Name:eap_bin2str_eapm_network_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t eap_bin2str_eapm_network_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                       length = sizeof (r_eap_eapm_network_cnf_t);
    r_eap_eapm_network_cnf_bin_t * pbin   = (r_eap_eapm_network_cnf_bin_t *)pin_bin;
    r_eap_eapm_network_cnf_t *     pstr   = (r_eap_eapm_network_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pstr, pbin, sizeof (r_eap_eapm_network_cnf_t));
    return length;
}
/******************************************************************************
   End of function  eap_bin2str_eapm_network_cnf
******************************************************************************/

/******************************************************************************
* Function Name:eap_bin2str_eapm_set_cinfo_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t eap_bin2str_eapm_set_cinfo_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                          length = sizeof (r_eap_eapm_set_clientinfo_cnf_t);
    r_eap_eapm_set_cinfo_cnf_bin_t *  pbin   = (r_eap_eapm_set_cinfo_cnf_bin_t *)pin_bin;
    r_eap_eapm_set_clientinfo_cnf_t * pstr   = (r_eap_eapm_set_clientinfo_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status         = pbin->status;
    R_memcpy (pstr->extendedAddress, pbin->extendedAddress, 8u);
    pstr->attributeIndex = R_BYTE_ArrToUInt16 (pbin->binAttributeIndex);
    return length;
} /* eap_bin2str_eapm_set_cinfo_cnf */
/******************************************************************************
   End of function  eap_bin2str_eapm_set_cinfo_cnf
******************************************************************************/

/******************************************************************************
* Function Name:eap_bin2str_eapm_nwk_join_ind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t eap_bin2str_eapm_nwk_join_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                        length = sizeof (r_eap_eapm_network_join_ind_t);
    r_eap_eapm_nwk_join_ind_bin_t * pbin   = (r_eap_eapm_nwk_join_ind_bin_t *)pin_bin;
    r_eap_eapm_network_join_ind_t * pstr   = (r_eap_eapm_network_join_ind_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pstr, pbin, sizeof (r_eap_eapm_network_join_ind_t));
    return length;
}
/******************************************************************************
   End of function  eap_bin2str_eapm_nwk_join_ind
******************************************************************************/

/******************************************************************************
* Function Name:eap_bin2str_eapm_nwk_leave_ind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t eap_bin2str_eapm_nwk_leave_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                         length = sizeof (r_eap_eapm_network_leave_ind_t);
    r_eap_eapm_nwk_leave_ind_bin_t * pbin   = (r_eap_eapm_nwk_leave_ind_bin_t *)pin_bin;
    r_eap_eapm_network_leave_ind_t * pstr   = (r_eap_eapm_network_leave_ind_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pstr, pbin, sizeof (r_eap_eapm_network_leave_ind_t));
    return length;
}
/******************************************************************************
   End of function  eap_bin2str_eapm_nwk_leave_ind
******************************************************************************/


/******************************************************************************
* Function Name:eap_bin2str_eapm_eap_key_ind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t eap_bin2str_eapm_eap_key_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                         length = sizeof (r_eap_eapm_eap_key_ind_t);
    r_eap_eapm_eap_key_ind_bin_t * pbin   = (r_eap_eapm_eap_key_ind_bin_t *)pin_bin;
    r_eap_eapm_eap_key_ind_t * pstr   = (r_eap_eapm_eap_key_ind_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pstr, pbin, sizeof (r_eap_eapm_eap_key_ind_t));
    return length;
}
/******************************************************************************
   End of function  eap_bin2str_eapm_eap_key_ind
******************************************************************************/

