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
 * File Name : r_c3sap_g3ctrl_convert.c
 * Description : G3 layer Bin convert API
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
#include "r_g3_sap.h"
#include "r_g3ctrl_binstruct.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static uint16_t g3ctrl_str2bin_init_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t g3ctrl_str2bin_setconfig_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t g3ctrl_str2bin_getinfo_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t g3ctrl_str2bin_clearinfo_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t g3ctrl_str2bin_dump_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t g3ctrl_bin2str_init_cnf (void * pin_bin, uint16_t max_len, uint8_t * pout_str);
static uint16_t g3ctrl_bin2str_setconfig_cnf (void * pin_bin, uint16_t max_len, uint8_t * pout_str);
static uint16_t g3ctrl_bin2str_getconfig_cnf (void * pin_bin, uint16_t max_len, uint8_t * pout_str);
static uint16_t g3ctrl_bin2str_getinfo_cnf (void * pin_bin, uint16_t max_len, uint8_t * pout_str);
static uint16_t g3ctrl_bin2str_dump_cnf (void * pin_bin, uint16_t max_len, uint8_t * pout_str);
static uint16_t g3ctrl_bin2str_dump_ind (void * pin_bin, uint16_t max_len, uint8_t * pout_str);
static uint16_t g3ctrl_bin2str_event_ind (void * pin_bin, uint16_t max_len, uint8_t * pout_str);

/*===========================================================================*/
/* Functions                                                                 */
/*===========================================================================*/

/******************************************************************************
* Function Name:R_G3CTRL_ReqStr2Bin
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_G3CTRL_ReqStr2Bin (uint16_t funcId, void * pin_str, uint16_t max_len, uint8_t * pout_bin, uint16_t * pout_len)
{
    r_result_t ret    = R_RESULT_FAILED;
    uint16_t   length = 0u;

    if (((NULL == pin_str) || (NULL == pout_bin)) || (NULL == pout_len))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    switch (funcId)
    {
        case R_G3_FUNCID_G3_INIT_REQ:
            length = g3ctrl_str2bin_init_req (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_G3_SETCONFIG_REQ:
            length = g3ctrl_str2bin_setconfig_req (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_G3_GETINFO_REQ:
            length = g3ctrl_str2bin_getinfo_req (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_G3_CLEARINFO_REQ:
            length = g3ctrl_str2bin_clearinfo_req (pin_str, max_len, pout_bin);
            break;

        case R_G3_FUNCID_G3_DUMP_REQ:
            length = g3ctrl_str2bin_dump_req (pin_str, max_len, pout_bin);
            break;
        case R_G3_FUNCID_G3_DEINIT_REQ:
        case R_G3_FUNCID_G3_GETCONFIG_REQ:
        case R_G3_FUNCID_G3_DUMP_ABORT_REQ:
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
} /* R_G3CTRL_ReqStr2Bin */
/******************************************************************************
   End of function  R_G3CTRL_ReqStr2Bin
******************************************************************************/

/******************************************************************************
* Function Name:R_G3CTRL_CbBin2Str
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_G3CTRL_CbBin2Str (uint16_t funcId, uint8_t * pin_bin, uint16_t max_len, void * pout_str, uint16_t * pout_len)
{
    r_result_t ret    = R_RESULT_FAILED;
    uint16_t   length = 0u;

    if (((NULL == pin_bin) || (NULL == pout_str)) || (NULL == pout_len))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    switch (funcId)
    {
        case R_G3_FUNCID_G3_INIT_CNF:
            length = g3ctrl_bin2str_init_cnf (pin_bin, max_len, pout_str);
            break;
        case R_G3_FUNCID_G3_DEINIT_CNF:
        case R_G3_FUNCID_G3_SETCONFIG_CNF:
        case R_G3_FUNCID_G3_CLEARINFO_CNF:
        case R_G3_FUNCID_G3_DUMP_ABORT_CNF:
            length = g3ctrl_bin2str_setconfig_cnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_G3_GETCONFIG_CNF:
            length = g3ctrl_bin2str_getconfig_cnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_G3_GETINFO_CNF:
            length = g3ctrl_bin2str_getinfo_cnf (pin_bin, max_len, pout_str);
            break;

        case R_G3_FUNCID_G3_DUMP_CNF:
            length = g3ctrl_bin2str_dump_cnf (pin_bin, max_len, pout_str);
            break;
        case R_G3_FUNCID_G3_DUMP_IND:
            length = g3ctrl_bin2str_dump_ind (pin_bin, max_len, pout_str);
            break;
        case R_G3_FUNCID_G3_EVENT_IND:
            length = g3ctrl_bin2str_event_ind (pin_bin, max_len, pout_str);
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
} /* R_G3CTRL_CbBin2Str */
/******************************************************************************
   End of function  R_G3CTRL_CbBin2Str
******************************************************************************/

/******************************************************************************
* Function Name:g3ctrl_str2bin_init_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3ctrl_str2bin_init_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t          length = 0u;
    r_g3_init_req_t * pstr   = (r_g3_init_req_t *)pin_str;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }

    switch (pstr->g3mode)
    {
#ifndef R_DEFINE_DISABLE_UMAC
        case R_G3_MODE_MAC:
        {
            r_g3_mac_init_req_bin_t * pbin = (r_g3_mac_init_req_bin_t *)pout_bin;
            length = sizeof (r_g3_mac_init_req_bin_t);
            if (max_len < length)
            {
                return 0u;
            }
            pbin->binG3mode = (uint8_t)pstr->g3mode;
            pbin->binNeighbourTableSize[0] = GET_S2C0 (pstr->init.mac.neighbourTableSize);
            pbin->binNeighbourTableSize[1] = GET_S2C1 (pstr->init.mac.neighbourTableSize);
            pbin->binDeviceTableSize[0]    = GET_S2C0 (pstr->init.mac.deviceTableSize);
            pbin->binDeviceTableSize[1]    = GET_S2C1 (pstr->init.mac.deviceTableSize);
            break;
        }
#endif
        case R_G3_MODE_ADP:
        {
            r_g3_adp_init_req_bin_t * pbin = (r_g3_adp_init_req_bin_t *)pout_bin;
            length = sizeof (r_g3_adp_init_req_bin_t);
            if (max_len < length)
            {
                return 0u;
            }
            pbin->binG3mode                = (uint8_t)pstr->g3mode;
            pbin->binNeighbourTableSize[0] = GET_S2C0 (pstr->init.adp.neighbourTableSize);
            pbin->binNeighbourTableSize[1] = GET_S2C1 (pstr->init.adp.neighbourTableSize);
            pbin->binDeviceTableSize[0]    = GET_S2C0 (pstr->init.adp.deviceTableSize);
            pbin->binDeviceTableSize[1]    = GET_S2C1 (pstr->init.adp.deviceTableSize);
            pbin->binPanDescriptorNum[0]   = GET_S2C0 (pstr->init.adp.panDescriptorNum);
            pbin->binPanDescriptorNum[1]   = GET_S2C1 (pstr->init.adp.panDescriptorNum);
            pbin->binAdpRouteTableSize[0]  = GET_S2C0 (pstr->init.adp.routeTableSize);
            pbin->binAdpRouteTableSize[1]  = GET_S2C1 (pstr->init.adp.routeTableSize);
            pbin->binAdpdBuffNum[0]        = GET_S2C0 (pstr->init.adp.adpdBuffNum);
            pbin->binAdpdBuffNum[1]        = GET_S2C1 (pstr->init.adp.adpdBuffNum);
            pbin->binRouteType             = pstr->init.adp.routeType;
            break;
        }

        case R_G3_MODE_EAP:
        {
            r_g3_eap_init_req_bin_t * pbin = (r_g3_eap_init_req_bin_t *)pout_bin;
            length = sizeof (r_g3_eap_init_req_bin_t);
            if (max_len < length)
            {
                return 0u;
            }
            pbin->binG3mode                = (uint8_t)pstr->g3mode;
            pbin->binNeighbourTableSize[0] = GET_S2C0 (pstr->init.eap.neighbourTableSize);
            pbin->binNeighbourTableSize[1] = GET_S2C1 (pstr->init.eap.neighbourTableSize);
            pbin->binDeviceTableSize[0]    = GET_S2C0 (pstr->init.eap.deviceTableSize);
            pbin->binDeviceTableSize[1]    = GET_S2C1 (pstr->init.eap.deviceTableSize);
            pbin->binPanDescriptorNum[0]   = GET_S2C0 (pstr->init.eap.panDescriptorNum);
            pbin->binPanDescriptorNum[1]   = GET_S2C1 (pstr->init.eap.panDescriptorNum);
            pbin->binAdpRouteTableSize[0]  = GET_S2C0 (pstr->init.eap.routeTableSize);
            pbin->binAdpRouteTableSize[1]  = GET_S2C1 (pstr->init.eap.routeTableSize);
            pbin->binAdpdBuffNum[0]        = GET_S2C0 (pstr->init.eap.adpdBuffNum);
            pbin->binAdpdBuffNum[1]        = GET_S2C1 (pstr->init.eap.adpdBuffNum);
            pbin->binRouteType             = pstr->init.eap.routeType;
            pbin->binEapLbpBuffNum[0]      = GET_S2C0 (pstr->init.eap.eapLbpBuffNum);
            pbin->binEapLbpBuffNum[1]      = GET_S2C1 (pstr->init.eap.eapLbpBuffNum);
            pbin->binEapCinfoTableNum[0]   = GET_S2C0 (pstr->init.eap.eapCinfoTableNum);
            pbin->binEapCinfoTableNum[1]   = GET_S2C1 (pstr->init.eap.eapCinfoTableNum);
            break;
        }

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
#ifndef R_DEFINE_DISABLE_UMAC
		case R_G3_MODE_HYBRID_MAC:
        case R_G3_MODE_RF_MAC:
        {
            r_g3_hymac_init_req_bin_t * pbin = (r_g3_hymac_init_req_bin_t *)pout_bin;
            length = sizeof (r_g3_hymac_init_req_bin_t);
            if (max_len < length)
            {
                return 0u;
            }
            pbin->binG3mode = (uint8_t)pstr->g3mode;
            pbin->binNeighbourTableSize[0] = GET_S2C0 (pstr->init.hymac.neighbourTableSize);
            pbin->binNeighbourTableSize[1] = GET_S2C1 (pstr->init.hymac.neighbourTableSize);
            pbin->binDeviceTableSize[0]    = GET_S2C0 (pstr->init.hymac.deviceTableSize);
            pbin->binDeviceTableSize[1]    = GET_S2C1 (pstr->init.hymac.deviceTableSize);
            pbin->binPosTableSize_RF[0]    = GET_S2C0 (pstr->init.hymac.posTableSize_RF);
            pbin->binPosTableSize_RF[1]    = GET_S2C1 (pstr->init.hymac.posTableSize_RF);
            break;
        }
#endif
		case R_G3_MODE_HYBRID_ADP:
        case R_G3_MODE_RF_ADP:
        {
            r_g3_hyadp_init_req_bin_t * pbin = (r_g3_hyadp_init_req_bin_t *)pout_bin;
            length = sizeof (r_g3_hyadp_init_req_bin_t);
            if (max_len < length)
            {
                return 0u;
            }
            pbin->binG3mode                = (uint8_t)pstr->g3mode;
            pbin->binNeighbourTableSize[0] = GET_S2C0 (pstr->init.hyadp.neighbourTableSize);
            pbin->binNeighbourTableSize[1] = GET_S2C1 (pstr->init.hyadp.neighbourTableSize);
            pbin->binDeviceTableSize[0]    = GET_S2C0 (pstr->init.hyadp.deviceTableSize);
            pbin->binDeviceTableSize[1]    = GET_S2C1 (pstr->init.hyadp.deviceTableSize);
            pbin->binPanDescriptorNum[0]   = GET_S2C0 (pstr->init.hyadp.panDescriptorNum);
            pbin->binPanDescriptorNum[1]   = GET_S2C1 (pstr->init.hyadp.panDescriptorNum);
            pbin->binAdpRouteTableSize[0]  = GET_S2C0 (pstr->init.hyadp.routeTableSize);
            pbin->binAdpRouteTableSize[1]  = GET_S2C1 (pstr->init.hyadp.routeTableSize);
            pbin->binAdpdBuffNum[0]        = GET_S2C0 (pstr->init.hyadp.adpdBuffNum);
            pbin->binAdpdBuffNum[1]        = GET_S2C1 (pstr->init.hyadp.adpdBuffNum);
            pbin->binRouteType             = pstr->init.hyadp.routeType;
            pbin->binPosTableSize_RF[0]    = GET_S2C0 (pstr->init.hyadp.posTableSize_RF);
            pbin->binPosTableSize_RF[1]    = GET_S2C1 (pstr->init.hyadp.posTableSize_RF);
            break;
        }
        case R_G3_MODE_HYBRID_EAP:
        case R_G3_MODE_RF_EAP:
        {
            r_g3_hyeap_init_req_bin_t * pbin = (r_g3_hyeap_init_req_bin_t *)pout_bin;
            length = sizeof (r_g3_hyeap_init_req_bin_t);
            if (max_len < length)
            {
                return 0u;
            }
            pbin->binG3mode                = (uint8_t)pstr->g3mode;
            pbin->binNeighbourTableSize[0] = GET_S2C0 (pstr->init.hyeap.neighbourTableSize);
            pbin->binNeighbourTableSize[1] = GET_S2C1 (pstr->init.hyeap.neighbourTableSize);
            pbin->binDeviceTableSize[0]    = GET_S2C0 (pstr->init.hyeap.deviceTableSize);
            pbin->binDeviceTableSize[1]    = GET_S2C1 (pstr->init.hyeap.deviceTableSize);
            pbin->binPanDescriptorNum[0]   = GET_S2C0 (pstr->init.hyeap.panDescriptorNum);
            pbin->binPanDescriptorNum[1]   = GET_S2C1 (pstr->init.hyeap.panDescriptorNum);
            pbin->binAdpRouteTableSize[0]  = GET_S2C0 (pstr->init.hyeap.routeTableSize);
            pbin->binAdpRouteTableSize[1]  = GET_S2C1 (pstr->init.hyeap.routeTableSize);
            pbin->binAdpdBuffNum[0]        = GET_S2C0 (pstr->init.hyeap.adpdBuffNum);
            pbin->binAdpdBuffNum[1]        = GET_S2C1 (pstr->init.hyeap.adpdBuffNum);
            pbin->binRouteType             = pstr->init.hyeap.routeType;
            pbin->binEapLbpBuffNum[0]      = GET_S2C0 (pstr->init.hyeap.eapLbpBuffNum);
            pbin->binEapLbpBuffNum[1]      = GET_S2C1 (pstr->init.hyeap.eapLbpBuffNum);
            pbin->binEapCinfoTableNum[0]   = GET_S2C0 (pstr->init.hyeap.eapCinfoTableNum);
            pbin->binEapCinfoTableNum[1]   = GET_S2C1 (pstr->init.hyeap.eapCinfoTableNum);
            pbin->binPosTableSize_RF[0]    = GET_S2C0 (pstr->init.hyeap.posTableSize_RF);
            pbin->binPosTableSize_RF[1]    = GET_S2C1 (pstr->init.hyeap.posTableSize_RF);
            break;
        }
#endif /* R_DEFINE_DISABLE_HYBRID R_DEFINE_DISABLE_APP_HYBRID */
        default:
        {
            /* do nothing */
            break;
        }
    } /* switch */

    return length;
} /* g3ctrl_str2bin_init_req */
/******************************************************************************
   End of function  g3ctrl_str2bin_init_req
******************************************************************************/

/******************************************************************************
* Function Name:g3ctrl_str2bin_setconfig_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3ctrl_str2bin_setconfig_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                length;
    r_g3_set_config_req_t * pstr   = (r_g3_set_config_req_t *)pin_str;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }

    switch (pstr->g3mode & 0x0F)
    {
        case R_G3_MODE_ADP:
        case R_G3_MODE_EAP:
        {
            r_g3_adpex_set_config_req_bin_t * pbin = (r_g3_adpex_set_config_req_bin_t *)pout_bin;
            if (0u == pstr->config.adp.extIDFlg)
            {
                length = sizeof (r_g3_adp_set_config_req_bin_t);
            }
            else
            {
                length = sizeof (r_g3_adpex_set_config_req_bin_t);
            }

            if (max_len < length)
            {
                return 0u;
            }
            pbin->binG3mode      = (uint8_t)pstr->g3mode;
            pbin->binBandPlan    = (uint8_t)pstr->config.adp.bandPlan;
            pbin->binReserved[0] = GET_I2C0 (pstr->config.adp.reserved);
            pbin->binReserved[1] = GET_I2C1 (pstr->config.adp.reserved);
            pbin->binReserved[2] = GET_I2C2 (pstr->config.adp.reserved);
            pbin->binReserved[3] = GET_I2C3 (pstr->config.adp.reserved);
            R_memcpy (pbin->binExtendedAddress, pstr->config.adp.extendedAddress, 8);
            R_memcpy (pbin->binPsk, pstr->config.adp.psk, 16);
            pbin->binExtIDFlg    = pstr->config.adp.extIDFlg;
            if (1u == pstr->config.adp.extIDFlg)
            {
                pbin->binExtIDLength  = pstr->config.adp.pExtId->extIDLength;
                R_memcpy (pbin->binExtID, pstr->config.adp.pExtId->extID, 36);
            }
            break;
        }

        case R_G3_MODE_MAC:
        default:
        {
            r_g3_mac_set_config_req_bin_t * pbin = (r_g3_mac_set_config_req_bin_t *)pout_bin;
            length = sizeof (r_g3_mac_set_config_req_bin_t);
            if (max_len < length)
            {
                return 0u;
            }
            pbin->binG3mode = (uint8_t)pstr->g3mode;
            pbin->binBandPlan = (uint8_t)pstr->config.mac.bandPlan;
            pbin->binReserved[0] = GET_I2C0 (pstr->config.mac.reserved);
            pbin->binReserved[1] = GET_I2C1 (pstr->config.mac.reserved);
            pbin->binReserved[2] = GET_I2C2 (pstr->config.mac.reserved);
            pbin->binReserved[3] = GET_I2C3 (pstr->config.mac.reserved);
            R_memcpy (pbin->binExtendedAddress, pstr->config.mac.extendedAddress, 8);
            break;
        }
    } /* switch */

    return length;

} /* g3ctrl_str2bin_setconfig_req */
/******************************************************************************
   End of function  g3ctrl_str2bin_setconfig_req
******************************************************************************/

/******************************************************************************
* Function Name:g3ctrl_str2bin_getinfo_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3ctrl_str2bin_getinfo_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                  length = sizeof (r_g3_get_info_req_bin_t);
    r_g3_get_info_req_t *     pstr   = (r_g3_get_info_req_t *)pin_str;
    r_g3_get_info_req_bin_t * pbin   = (r_g3_get_info_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }

    R_memcpy (pbin, pstr, sizeof (r_g3_get_info_req_bin_t));
    return length;
} /* g3ctrl_str2bin_getinfo_req */
/******************************************************************************
   End of function  g3ctrl_str2bin_getinfo_req
******************************************************************************/

/******************************************************************************
* Function Name:g3ctrl_str2bin_clearinfo_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3ctrl_str2bin_clearinfo_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                    length = sizeof (r_g3_clear_info_req_bin_t);
    r_g3_clear_info_req_t *     pstr   = (r_g3_clear_info_req_t *)pin_str;
    r_g3_clear_info_req_bin_t * pbin   = (r_g3_clear_info_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }

    pbin->binInfoTypeBit     = (uint8_t)pstr->infoTypeBit;
    pbin->binInfoLayerBit[0] = GET_S2C0 (pstr->infoLayerBit);
    pbin->binInfoLayerBit[1] = GET_S2C1 (pstr->infoLayerBit);

    return length;
} /* g3ctrl_str2bin_clearinfo_req */
/******************************************************************************
   End of function  g3ctrl_str2bin_clearinfo_req
******************************************************************************/

/******************************************************************************
* Function Name:g3ctrl_str2bin_dump_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3ctrl_str2bin_dump_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                    length = sizeof (r_g3_dump_req_bin_t);
    r_g3_dump_req_t *           pstr   = (r_g3_dump_req_t *)pin_str;
    r_g3_dump_req_bin_t *       pbin   = (r_g3_dump_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }

    pbin->binMaxDmpLen[0]       = GET_I2C0 (pstr->maxDmpLen);
    pbin->binMaxDmpLen[1]       = GET_I2C1 (pstr->maxDmpLen);
    pbin->binMaxDmpLen[2]       = GET_I2C2 (pstr->maxDmpLen);
    pbin->binMaxDmpLen[3]       = GET_I2C3 (pstr->maxDmpLen);
    pbin->binTimeOut[0]         = GET_I2C0 (pstr->timeOut);
    pbin->binTimeOut[1]         = GET_I2C1 (pstr->timeOut);
    pbin->binTimeOut[2]         = GET_I2C2 (pstr->timeOut);
    pbin->binTimeOut[3]         = GET_I2C3 (pstr->timeOut);
    pbin->binDmpMode            = pstr->dmpMode;
    R_memcpy (pbin->binReserved, &pstr->reserved, 3);
    pbin->binSegmentLen[0]      = GET_S2C0 (pstr->segmentLen);
    pbin->binSegmentLen[1]      = GET_S2C1 (pstr->segmentLen);
    pbin->binSegmentInterval[0] = GET_I2C0 (pstr->segmentInterval);
    pbin->binSegmentInterval[1] = GET_I2C1 (pstr->segmentInterval);
    pbin->binSegmentInterval[2] = GET_I2C2 (pstr->segmentInterval);
    pbin->binSegmentInterval[3] = GET_I2C3 (pstr->segmentInterval);

    return length;
} /* g3ctrl_str2bin_dump_req */
/******************************************************************************
   End of function  g3ctrl_str2bin_dump_req
******************************************************************************/

/******************************************************************************
* Function Name:g3ctrl_bin2str_init_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3ctrl_bin2str_init_cnf (void * pin_bin, uint16_t max_len, uint8_t * pout_str)
{
    uint16_t                  length = sizeof (r_g3_init_cnf_t);
    r_g3_init_cnf_bin_t     * pbin   = (r_g3_init_cnf_bin_t *)pin_bin;
    r_g3_init_cnf_t         * pstr   = (r_g3_init_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pstr, pbin, sizeof (r_g3_init_cnf_t));
    return length;
}
/******************************************************************************
   End of function  g3ctrl_bin2str_init_cnf
******************************************************************************/

/******************************************************************************
* Function Name:g3ctrl_bin2str_setconfig_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3ctrl_bin2str_setconfig_cnf (void * pin_bin, uint16_t max_len, uint8_t * pout_str)
{
    uint16_t                    length = sizeof (r_g3_set_config_cnf_t);
    r_g3_set_config_cnf_bin_t * pbin   = (r_g3_set_config_cnf_bin_t *)pin_bin;
    r_g3_set_config_cnf_t *     pstr   = (r_g3_set_config_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    R_memcpy (pstr, pbin, sizeof (r_g3_set_config_cnf_t));
    return length;
}
/******************************************************************************
   End of function  g3ctrl_bin2str_setconfig_cnf
******************************************************************************/

/******************************************************************************
* Function Name:g3ctrl_bin2str_getconfig_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3ctrl_bin2str_getconfig_cnf (void * pin_bin, uint16_t max_len, uint8_t * pout_str)
{
    uint16_t                    length = sizeof (r_g3_get_config_cnf_t);
    r_g3_get_config_cnf_bin_t * pbin   = (r_g3_get_config_cnf_bin_t *)pin_bin;
    r_g3_get_config_cnf_t *     pstr   = (r_g3_get_config_cnf_t *)pout_str;     /* PRQA S 3305 */

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (1u == pbin->binExtIDFlg)
    {
        length = (uint16_t)(sizeof (r_g3_get_config_cnf_t) + sizeof (r_g3_config_extid_t));
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status   = pbin->binStatus;
    pstr->g3mode   = pbin->binG3mode;
    pstr->bandPlan = pbin->binBandPlan;
    pstr->reserved = R_BYTE_ArrToUInt32 (pbin->binReserved);
    R_memcpy (pstr->extendedAddress, pbin->binExtendedAddress, 8);
    R_memcpy (pstr->psk, pbin->binPsk, 16);
    pstr->extIDFlg = pbin->binExtIDFlg;
    pstr->pExtId   = (r_g3_config_extid_t *)&pstr[1];
    if (1u == pbin->binExtIDFlg)
    {
        R_memcpy (pstr->pExtId, &pbin->binExtIDLength, sizeof (r_g3_config_extid_t));
    }

    return length;
} /* g3ctrl_bin2str_getconfig_cnf */
/******************************************************************************
   End of function  g3ctrl_bin2str_getconfig_cnf
******************************************************************************/

/******************************************************************************
* Function Name:g3ctrl_bin2str_getinfo_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3ctrl_bin2str_getinfo_cnf (void * pin_bin, uint16_t max_len, uint8_t * pout_str)
{
    uint16_t                  length;
    r_g3_get_info_cnf_bin_t * pbin = (r_g3_get_info_cnf_bin_t *)pin_bin;
    r_g3_get_info_cnf_t *     pstr = (r_g3_get_info_cnf_t *)pout_str;           /* PRQA S 3305 */

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }

    length          = (uint16_t)((sizeof (r_g3_get_info_cnf_t) + R_BYTE_ArrToUInt16 (pbin->binLength)));
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status    = pbin->binStatus;
    pstr->infoType  = pbin->binInfoType;
    pstr->infoLayer = pbin->binInfoLayer;
    pstr->length    = R_BYTE_ArrToUInt16 (pbin->binLength);
    pstr->pInfo     = (uint32_t *)&pstr[1];

    R_memcpy (pstr->pInfo, pbin->binInfo, pstr->length);

    return length;
} /* g3ctrl_bin2str_getinfo_cnf */
/******************************************************************************
   End of function  g3ctrl_bin2str_getinfo_cnf
******************************************************************************/


/******************************************************************************
* Function Name:g3ctrl_bin2str_dump_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3ctrl_bin2str_dump_cnf (void * pin_bin, uint16_t max_len, uint8_t * pout_str)
{
    uint16_t               length = sizeof (r_g3_dump_cnf_t);
    r_g3_dump_cnf_bin_t *  pbin = (r_g3_dump_cnf_bin_t *)pin_bin;
    r_g3_dump_cnf_t *      pstr = (r_g3_dump_cnf_t *)pout_str; /* PRQA S 3305 */
    
    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status          = pbin->binStatus;
    pstr->result          = pbin->binResult;
    pstr->segmentNum      = R_BYTE_ArrToUInt16 (pbin->binSegmentNum);
    pstr->segmentLen      = R_BYTE_ArrToUInt16 (pbin->binSegmentLen);
    pstr->segmentInterval = R_BYTE_ArrToUInt32 (pbin->binSegmentInterval);
    pstr->dmpLen          = R_BYTE_ArrToUInt32 (pbin->binDmpLen);

    return length;
} /* g3ctrl_bin2str_dump_cnf */
/******************************************************************************
   End of function  g3ctrl_bin2str_dump_cnf
******************************************************************************/

/******************************************************************************
* Function Name:g3ctrl_bin2str_dump_ind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3ctrl_bin2str_dump_ind (void * pin_bin, uint16_t max_len, uint8_t * pout_str)
{
    uint16_t               length;
    r_g3_dump_ind_bin_t *  pbin = (r_g3_dump_ind_bin_t *)pin_bin;
    r_g3_dump_ind_t *      pstr = (r_g3_dump_ind_t *)pout_str; /* PRQA S 3305 */

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }

    length          = (uint16_t)(sizeof (r_g3_dump_ind_t) + R_BYTE_ArrToUInt16 (pbin->binSegmentLen));
    if (max_len < length)
    {
        return 0u;
    }
    pstr->seqNum        = R_BYTE_ArrToUInt16 (pbin->binSeqNum);
    pstr->endFlg        = pbin->binEndFlg;
    pstr->segmentLen    = R_BYTE_ArrToUInt16 (pbin->binSegmentLen);
    pstr->pDmpData      = (uint8_t *)&pstr[1];
    R_memcpy (pstr->pDmpData, pbin->binDmpData, pstr->segmentLen);

    return length;
} /* g3ctrl_bin2str_dump_ind */
/******************************************************************************
   End of function  g3ctrl_bin2str_dump_ind
******************************************************************************/

/******************************************************************************
* Function Name:g3ctrl_bin2str_event_ind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t g3ctrl_bin2str_event_ind (void * pin_bin, uint16_t max_len, uint8_t * pout_str)
{
    uint16_t               length;
    r_g3_event_ind_bin_t * pbin = (r_g3_event_ind_bin_t *)pin_bin;
    r_g3_event_ind_t *     pstr = (r_g3_event_ind_t *)pout_str; /* PRQA S 3305 */

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }

    length          = (uint16_t)(sizeof (r_g3_event_ind_t) + R_BYTE_ArrToUInt16 (pbin->binLength));
    if (max_len < length)
    {
        return 0u;
    }
    pstr->eventCode = pbin->binEventCode;
    pstr->length    = R_BYTE_ArrToUInt16 (pbin->binLength);
    pstr->pParam    = (uint8_t *)&pstr[1];
    R_memcpy (pstr->pParam, pbin->binParam, pstr->length);

    return length;
} /* g3ctrl_bin2str_event_ind */
/******************************************************************************
   End of function  g3ctrl_bin2str_event_ind
******************************************************************************/


