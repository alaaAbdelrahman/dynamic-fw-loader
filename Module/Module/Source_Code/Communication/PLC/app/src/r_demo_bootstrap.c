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
 * File Name   : r_demo_bootstrap.c
 *    @version
 *        $Rev: 2394 $
 *    @last editor
 *        $Author: khues $
 *    @date
 *        $Date:: 2017-05-05 08:48:11 +0200#$
 * Description :
 ******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "r_typedefs.h"
#include "r_stdio_api.h"
#include "r_byte_swap.h"
#include "r_config.h"

#include "r_io_vec.h"

/* g3 part */
#include "r_c3sap_api.h"
#include "r_demo_config.h"
#include "r_demo_app.h"
#include "r_demo_app_eap.h"
#include "r_demo_nvm_process.h"
#include "r_demo_print.h"
#include "r_demo_tools.h"
#include "r_demo_api.h"
#include "r_demo_metric_computation.h"
#include "r_ipv6_headers.h"
#include "r_udp_headers.h"
#include "r_icmp_v6.h"
#include "r_check_sum.h"
#include "r_demo_common.h"
#include "r_demo_parameters.h"
#include "r_demo_os_wrapper.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t current_AppJoinNetwork = CPX3_GET_R_ADP_IB_METRIC_TYPE;
extern uint8_t get_handle_state;
extern uint8_t config_monitor;
extern uint8_t cpx3_current_task; /* variable hold current task */

uint8_t joinCnt;
uint8_t beaconIndex;
uint8_t duration;

uint8_t thresh_lqi = R_MIN_JOIN_LQI_START;
r_boolean_t joinFinished = R_FALSE;

r_adp_adpm_network_join_req_t nwjReq;
r_adp_adpm_network_join_cnf_t *nwjCfm;
r_adp_adpm_discovery_cnf_t *disCfm;

r_demo_paninfo_t *pSortPan;

#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
r_loadng_ib_info_t loadIb[R_MEDIA_TYPE2_BOTH];
#else
r_loadng_ib_info_t loadIb[1];
#endif
static r_loadng_ib_info_t *ploadIb=loadIb; // static
r_loadng_link_info_t loadLink;

uint8_t tempArray[R_ADP_MAX_IB_SIZE];

// ploadIb = loadIb;

/////////////////////////////////////////////////////////////////////////////////////////////////

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static void demo_pkup_pan(const r_loadng_ib_info_t *ibInfo, r_loadng_link_info_t *linkInfo, uint8_t thresh_lqi[], r_adp_adpm_discovery_cnf_t *in_cnf, r_demo_paninfo_t *ont_pan);
static void demo_pan_shuffle(r_demo_paninfo_t *pPan);

/******************************************************************************
Exported global variables
******************************************************************************/
extern r_demo_config_t g_demo_config;
extern r_demo_entity_t g_demo_entity;
extern r_demo_buff_t g_demo_buff;

extern r_demo_g3_cb_str_t g_g3cb[R_G3_CH_MAX];

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Functions
******************************************************************************/

r_result_t CPX3_AppGetNewtorkParmHandle(void)
{

    /*********************************************************************************************************
                                          13/10/2024  Sunday
    **********************************************************************************************************/
    switch (current_AppJoinNetwork)
    {
    case CPX3_GET_R_ADP_IB_METRIC_TYPE:
        if (R_DEMO_AdpmGetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_METRIC_TYPE, 0, tempArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        // ploadIb->adpMetricType = tempArray[0];
        get_handle_state = 0;
        current_AppJoinNetwork = CPX3_HANDLE_GET_R_ADP_IB_METRIC_TYPE;
        return R_RESULT_SUCCESS;
        break;
    case CPX3_HANDLE_GET_R_ADP_IB_METRIC_TYPE:
        if (R_DEMO_AdpmGetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_METRIC_TYPE, 0, tempArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        ploadIb->adpMetricType = tempArray[0];
        get_handle_state = 1;
        config_monitor = 0;
        current_AppJoinNetwork = CPX3_GET_R_ADP_IB_LOW_LQI_VALUE;
        return R_RESULT_SUCCESS;

        break;
    case CPX3_GET_R_ADP_IB_LOW_LQI_VALUE:
        if (R_DEMO_AdpmGetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_LOW_LQI_VALUE, 0, tempArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        // ploadIb->adpLowLQIValue = tempArray[0];
        get_handle_state = 0;
        current_AppJoinNetwork = CPX3_HANDLE_GET_R_ADP_IB_LOW_LQI_VALUE;
        return R_RESULT_SUCCESS;

        break;
    case CPX3_HANDLE_GET_R_ADP_IB_LOW_LQI_VALUE:
        if (R_DEMO_AdpmGetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_LOW_LQI_VALUE, 0, tempArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        ploadIb->adpLowLQIValue = tempArray[0];
        get_handle_state = 1;
        config_monitor = 0;
        current_AppJoinNetwork = CPX3_GET_R_ADP_IB_HIGH_LQI_VALUE;
        return R_RESULT_SUCCESS;

        break;
    case CPX3_GET_R_ADP_IB_HIGH_LQI_VALUE:
        if (R_DEMO_AdpmGetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_HIGH_LQI_VALUE, 0, tempArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        // ploadIb->adpHighLQIValue = tempArray[0];
        get_handle_state = 0;
        current_AppJoinNetwork = CPX3_HANDLE_GET_R_ADP_IB_HIGH_LQI_VALUE;
        return R_RESULT_SUCCESS;

        break;
    case CPX3_HANDLE_GET_R_ADP_IB_HIGH_LQI_VALUE:
        if (R_DEMO_AdpmGetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_HIGH_LQI_VALUE, 0, tempArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        ploadIb->adpHighLQIValue = tempArray[0];
        get_handle_state = 1;
        config_monitor = 0;
        current_AppJoinNetwork = CPX3_GET_R_ADP_IB_KQ;
        return R_RESULT_SUCCESS;

        break;
    case CPX3_GET_R_ADP_IB_KQ:
        if (R_DEMO_AdpmGetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_KQ, 0, tempArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        // ploadIb->adpKq = tempArray[0];
        get_handle_state = 0;
        current_AppJoinNetwork = CPX3_HANDLE_GET_R_ADP_IB_KQ;
        return R_RESULT_SUCCESS;

        break;
    case CPX3_HANDLE_GET_R_ADP_IB_KQ:
        if (R_DEMO_AdpmGetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_KQ, 0, tempArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        ploadIb->adpKq = tempArray[0];
        get_handle_state = 1;
        config_monitor = 0;
        current_AppJoinNetwork = CPX3_GET_R_ADP_IB_KH;
        return R_RESULT_SUCCESS;

        break;
    case CPX3_GET_R_ADP_IB_KH:
        if (R_DEMO_AdpmGetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_KH, 0, tempArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        // ploadIb->adpKh = tempArray[0];
        get_handle_state = 0;
        current_AppJoinNetwork = CPX3_HANDLE_GET_R_ADP_IB_KH;
        return R_RESULT_SUCCESS;

        break;
    case CPX3_HANDLE_GET_R_ADP_IB_KH:
        if (R_DEMO_AdpmGetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_KH, 0, tempArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        ploadIb->adpKh = tempArray[0];
        get_handle_state = 1;
        config_monitor = 0;
        cpx3_current_task = CPX3_APP_SCAN_NETWORK;
        return R_RESULT_SUCCESS;

        break;

    default:
        return R_RESULT_FAILED;

        break;
    }
}

/******************************************************************************
 * Function Name: R_DEMO_AppJoinNetwork
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t CPX3_AppScantNewtorkHandle(void)
{
    r_result_t state = R_RESULT_SUCCESS;
    duration = (uint8_t)((R_DEMO_MODE_CERT == g_demo_config.appMode) ? R_SCAN_DURATION_CERT : R_SCAN_DURATION);
    state = R_DEMO_AppNetworkDiscovery(duration);
    return state;

    // if (state == 0)
    // {
    //     // not found
    //     cpx3_current_task = CPX3_APP_SCAN_NETWORK;
        
    //     return R_RESULT_SUCCESS;
        
    // }
    // else

    // {
    //     // FOUND NETWORK
    //     cpx3_current_task = CPX3_APP_JOIN_NETWORK;
    //     return R_RESULT_SUCCESS;
    // }
}

r_result_t CPX3_AppJointNewtork(void)
{

    /* Loop until join process finished successfully. */
    while (R_FALSE == joinFinished)
    {
        /* Start discovery and check if a network has been discovered. */
        // while (R_DEMO_AppNetworkDiscovery(duration) == 0)
        // {
        //     R_DEMO_BusyWait(1); /* wait */
        // }

        disCfm = &g_g3cb[R_DEMO_G3_USE_PRIMARY_CH].adpmDiscoveryCnf;
        pSortPan = &g_demo_buff.panInfo;

#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
        demo_pkup_pan(loadIb, &loadLink, thresh_lqi, disCfm, pSortPan);
#else
        demo_pkup_pan(loadIb, &loadLink, &thresh_lqi, disCfm, pSortPan);
#endif

        if (
            (pSortPan->panCount) &&
            (R_BYTE_ArrToUInt16(pSortPan->pan[0].address) != g_demo_config.coordShortAddress))
        {
            /* shuffle LBA to avoid heavy biased relay node to coordinator */
            demo_pan_shuffle(pSortPan);
        }

        joinCnt = 0x0u;
        beaconIndex = 0x0u;

        /* Check if a suitable beacon has been found. Check that the beacon has a minimum LQI
             to avoid nodes in lower stages to join the coordinator immediately. */
        if (pSortPan->panCount)
        {
            while (beaconIndex < pSortPan->panCount)
            {
                /* Set LBA address and PAN ID for join request. */
                nwjReq.panId = pSortPan->pan[beaconIndex].panId;
                R_memcpy(nwjReq.lbaAddress, pSortPan->pan[beaconIndex].address, 2);
                nwjReq.mediaType = pSortPan->pan[beaconIndex].mediaType;
                if ((R_DEMO_AdpmNetworkJoin(R_DEMO_G3_USE_PRIMARY_CH, &nwjReq, &nwjCfm) == R_RESULT_SUCCESS) &&
                    (R_ADP_STATUS_SUCCESS == nwjCfm->status))
                {
                    g_demo_entity.panId = nwjCfm->panId;
                    g_demo_entity.shortAddress = R_BYTE_ArrToUInt16((uint8_t *)nwjCfm->networkAddress);
                    joinFinished = R_TRUE;
                    break;
                }
                else
                {
                    if ((joinCnt++) >= R_JOIN_RETRYNUM_TO_SAME_LBA)
                    {
                        joinCnt = 0;
                        beaconIndex++;
                    }

                    /* Wait some time to allow other devices to join the network before flooding
                     the network with beacon requests and beacons. */
                    R_DEMO_BusyWait(R_WAIT_BETWEEN_JOINREQ);

                    if (beaconIndex > R_JOIN_RETRYNUM_PER_DISCOVERY)
                    {
                        break;
                    }
                }
            }
        }
        else
        {

            //            /* Decrement thresh_lqi and start new discovery until it matches weakLqiValue. */
            // #if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
            //            if (thresh_lqi[R_MEDIA_TYPE0_PLC] > R_WEAK_LQI_TH)
            //            {
            //                thresh_lqi[R_MEDIA_TYPE0_PLC] = (uint8_t)(thresh_lqi[R_MEDIA_TYPE0_PLC] - R_MIN_JOIN_LQI_STEP);
            //            }
            //            if (thresh_lqi[R_MEDIA_TYPE1_RF] > R_WEAK_LQI_TH_RF)
            //            {
            //                thresh_lqi[R_MEDIA_TYPE1_RF] = (uint8_t)(thresh_lqi[R_MEDIA_TYPE1_RF] - R_MIN_JOIN_LQI_STEP);
            //            }
            // #else
            if (thresh_lqi > R_WEAK_LQI_TH)
            {
                thresh_lqi = (uint8_t)(thresh_lqi - R_MIN_JOIN_LQI_STEP);
            }
            // #endif

            /* Wait some time to allow other devices to join the network before flooding
             the network with beacon requests and beacons. */
            R_DEMO_BusyWait(R_WAIT_BETWEEN_DISCOVERIES);
        }

    } /* while (joinFinished == R_FALSE) */

    if (R_DEMO_MODE_CERT == g_demo_config.appMode)
    {
        if (R_DEMO_SetContext0forCert(g_demo_entity.panId) != (r_result_t)R_ADP_STATUS_SUCCESS)
        {
            return 1;
        }
    }

#if R_ROUTE_DISCOVERY_AFTER_JOIN == R_TRUE
    /*    else */
    if ((R_G3_BANDPLAN_ARIB != g_demo_config.bandPlan) || (R_DEMO_MODE_CERT != g_demo_config.appMode))
    {
        r_adp_adpm_route_disc_req_t rdisReq;
        r_adp_adpm_route_disc_cnf_t *rdisCfm;

        R_BYTE_UInt16ToArr(g_demo_config.coordShortAddress, rdisReq.dstAddress);
        rdisReq.maxHops = 8u;

        /* Call route discovery function. */
        R_DEMO_AdpmRouteDiscovery(R_DEMO_G3_USE_PRIMARY_CH, &rdisReq, &rdisCfm);
    }
#endif
} /* R_DEMO_AppJoinNetwork */
/******************************************************************************
   End of function  R_DEMO_AppJoinNetwork
******************************************************************************/

/******************************************************************************
 * Function Name: demo_pkup_pan
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
static void demo_pkup_pan(const r_loadng_ib_info_t *ibInfo, r_loadng_link_info_t *linkInfo, uint8_t thresh_lqi[], r_adp_adpm_discovery_cnf_t *in_cnf, r_demo_paninfo_t *ont_pan)
{
    uint32_t write_flg = 0;

    uint8_t pan_cnt = 0;
    uint16_t i;
    uint16_t j;
    uint16_t k;
    uint8_t linkCost;

    r_adp_pan_descriptor_t *pin_pan;
    r_adp_pan_descriptor_t *pcur_pan;

    R_memset((uint8_t *)ont_pan, 0, sizeof(r_demo_paninfo_t));

    for (i = 0; i < in_cnf->PANCount; i++)
    {
        pin_pan = &in_cnf->PANDescriptor[i];

        linkInfo->lqi = pin_pan->linkQuality;
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
        linkInfo->mediaType = pin_pan->mediaType;
        linkCost = R_DEMO_ComputeDirectionalLinkCost(&ibInfo[pin_pan->mediaType], linkInfo);
#else
        linkCost = R_DEMO_ComputeDirectionalLinkCost(ibInfo, linkInfo);
#endif
        pin_pan->rcCoord = (((uint32_t)pin_pan->rcCoord + linkCost) >= 0xFFFFu) ? 0xFFFFu : (uint16_t)(pin_pan->rcCoord + linkCost);

#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
        if (pin_pan->linkQuality > thresh_lqi[pin_pan->mediaType])
#else
        if (pin_pan->linkQuality > thresh_lqi[0])
#endif
        {
            write_flg = 0;
            if (0 == pan_cnt)
            {
                ont_pan->pan[0] = *pin_pan;
                pan_cnt++;
                continue;
            }
            for (j = 0; j < pan_cnt; j++)
            {
                pcur_pan = &ont_pan->pan[j];
                if (pin_pan->rcCoord == pcur_pan->rcCoord)
                {
                    if (pin_pan->linkQuality > pcur_pan->linkQuality)
                    {
                        k = (R_DEMO_APP_PAN_MAXNUM == pan_cnt) ? (uint16_t)(pan_cnt - 1u) : (uint16_t)pan_cnt;
                        for (; k > (j); k--)
                        {
                            ont_pan->pan[k] = ont_pan->pan[k - 1];
                        }
                        *pcur_pan = *pin_pan;
                        pan_cnt = (pan_cnt < R_DEMO_APP_PAN_MAXNUM) ? (uint8_t)(pan_cnt + 1u) : (uint8_t)pan_cnt;
                        write_flg = 1;
                        break;
                    }
                }
                else if (pin_pan->rcCoord < pcur_pan->rcCoord)
                {
                    k = (R_DEMO_APP_PAN_MAXNUM == pan_cnt) ? (uint16_t)(pan_cnt - 1u) : (uint16_t)pan_cnt;
                    for (; k > (j); k--)
                    {
                        ont_pan->pan[k] = ont_pan->pan[k - 1];
                    }
                    *pcur_pan = *pin_pan;
                    pan_cnt = (pan_cnt < R_DEMO_APP_PAN_MAXNUM) ? (uint8_t)(pan_cnt + 1u) : (uint8_t)pan_cnt;
                    write_flg = 1;
                    break;
                }
                else
                {
                    /**/
                }
            }
            if ((0 == write_flg) &&
                (pan_cnt < R_DEMO_APP_PAN_MAXNUM))
            {
                ont_pan->pan[pan_cnt] = *pin_pan;
                pan_cnt++;
                continue;
            }
        }
    }
    ont_pan->panCount = pan_cnt;

    return;
} /* demo_pkup_pan */
/******************************************************************************
   End of function  demo_pkup_pan
******************************************************************************/

/******************************************************************************
 * Function Name: demo_pan_shuffle
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
static void demo_pan_shuffle(r_demo_paninfo_t *pPan)
{
    r_adp_pan_descriptor_t *pSrc = pPan->pan;
    r_adp_pan_descriptor_t dst;

    uint32_t tmp;
    uint16_t i;
    uint16_t j;
    uint16_t size;

    if (pPan->panCount > 1)
    {
        size = pPan->panCount;
        for (i = 0; i < size; i++)
        {
            if (pSrc[i].rcCoord >= 0x7FFF)
            {
                size = (uint16_t)((i > 0) ? (i - 1) : 0);
                break;
            }
        }
        if (size <= 2)
        {
            /* targets of shuffle are only LBA which have route to coordinator */
            return;
        }

        tmp = (uint32_t)(g_demo_config.deviceEUI64 >> 32);
        tmp = tmp ^ (uint32_t)g_demo_config.deviceEUI64;
        srand(tmp);
        for (i = 0; i < size; i++)
        {
            j = (uint16_t)rand() % size;
            dst = pSrc[i];
            pSrc[i] = pSrc[j];
            pSrc[j] = dst;
        }
    }

    return;
} /* demo_pan_shuffle */
/******************************************************************************
   End of function  demo_pan_shuffle
******************************************************************************/
