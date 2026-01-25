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
* File Name   : r_demo_parameters.h
*    @version
*        $Rev: 2585 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2017-01-11 15:58:25 +0900#$
* Description :
******************************************************************************/

#ifndef R_DEMO_PARAMETERS_H
#define R_DEMO_PARAMETERS_H

/******************************************************************************
Macro definitions
******************************************************************************/
    #define R_PROFILE_METERING

/******************************************************************************
CPX3 Table & Buffer Configuration
******************************************************************************/
#define R_DEMO_G3MAC_NEIGBOUR_TABLE_SIZE  (500u)        /*!< Neighbour table size */
#define R_DEMO_G3MAC_DEVICE_TABLE_SIZE    (500u)        /*!< Device table size */
#define R_DEMO_ADP_ADPD_DATA_QUEUE_SIZE   (2u)          /*!< Adpd-data queue size range 1~4 */
#define R_DEMO_ADP_ROUTING_TABLE_SIZE     (500u)        /*!< Routing table size range 1~2000 */
#define R_DEMO_ADP_MAX_PAN_DESCRIPTORS    (64u)         /*!< Maximum number of PAN descriptors stored during discovery  range 1~128 */
#define R_DEMO_EAP_LBP_BUFF_SIZE          (8u)          /*!< LBP buffer size range 1-128 */
#define R_DEMO_EAP_CINFO_TABLE_SIZE       (64u)         /*!< Client information table size range 1~1200 */

/******************************************************************************
Common Profile Settings
******************************************************************************/
#if  defined R_RL78_PLC
#define R_FRAMECOUNT_INTERVAL           (0x200)     /*!< Interval for frame counter indications */
#define R_VAL_MAC_TMR_IND_ENABLE        (R_FALSE)   /*!< Controls if TMR indications are issued */
#define R_VAL_ADP_ROUTE_IND_ENABLE      (R_FALSE)   /*!< Controls if route indications are issued */
#define R_VAL_ADP_RREP_IND_ENABLE       (R_FALSE)   /*!< Controls if rrep indications are issued */
#else
#define R_FRAMECOUNT_INTERVAL           (0x100)     /*!< Interval for frame counter indications */
#define R_VAL_MAC_TMR_IND_ENABLE        (R_TRUE)    /*!< Controls if TMR indications are issued */
#define R_VAL_ADP_ROUTE_IND_ENABLE      (R_TRUE)    /*!< Controls if route indications are issued */
#define R_VAL_ADP_RREP_IND_ENABLE       (R_TRUE)    /*!< Controls if rrep indications are issued */
#endif
#define R_VAL_MAC_OFFSET_SNR_CA         (0xFD)      /*!< The offset value for SNR of subcarriers in dB for CENELECA */
#define R_VAL_MAC_OFFSET_SNR_ARIB       (0xFA)      /*!< The offset value for SNR of subcarriers in dB for ARIB */
#define R_VAL_MAC_OFFSET_SNR_FCC        (0xFA)      /*!< The offset value for SNR of subcarriers in dB for FCC */
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
#define R_FRAMECOUNT_RF_INTERVAL        (0x64)      /*!< Interval for frame counter indications for RF */
#endif

/* PHY PIB Settings */
#define R_VAL_PHY_SATT_CTRL_DISABLE     (R_FALSE)   /*!< it should be TRUE when SATT is not mouted */


#ifdef R_PROFILE_DEFAULT
/******************************************************************************
Default Profile Settings (MAC & ADP IB settings as specified in G.9903)
******************************************************************************/
/* Bootstrapping module configuration */
#define R_MIN_JOIN_LQI_START            (80u)       /*!< Minimum LQI value for the first discovery */
#define R_MIN_JOIN_LQI_STEP             (5u)        /*!< Minimum LQI step for following discoveries */
#define R_WAIT_BETWEEN_DISCOVERIES      (3000u)     /*!< Period between two route discoveries (milliseconds) */
#define R_SCAN_DURATION                 (15u)       /*!< Network discovery period */
#define R_JOIN_RETRYNUM_TO_SAME_LBA     (2u)        /*!< Retry number for ADPM-NETOWORK-JOIN */
#define R_JOIN_RETRYNUM_PER_DISCOVERY   (3u)        /*!< Change LBA from PAN descriptor */
#define R_WAIT_BETWEEN_JOINREQ          (5000u)     /*!< Period between two join requests (milliseconds) */
#define R_WEAK_LQI_TH                   (60u)       /*!< Minimum LQI value for the first discovery */
#define R_ROUTE_DISCOVERY_AFTER_JOIN    (R_FALSE)   /*!< Activate/Deactivate automatic route discovery after join completion */

/* MAC PIB Settings */
#define R_VAL_MAC_MAX_BE                (8)         /*!< macMaxBe to be used */
#define R_VAL_MAC_MIN_BE                (3)         /*!< macMinBe to be used */
#define R_VAL_MAC_A                     (8)         /*!< macA to be used */
#define R_VAL_MAC_BEACON_RAND_WIDTH     (12)        /*!< macBeaconRandomizationWindowLength to be used */
#define R_VAL_MAC_BCAST_TX_TIMEOUT      (30)        /*!< macBroadcastDataTxTimeout to be used */
#define R_VAL_MAC_RETRY_MAC_CW          (R_FALSE)   /*!< macRetryMaxCw to be used */

/* ADP IB Settings. */
#define R_VAL_ADP_MAX_HOPS               (8u)        /*!< Maximum number of hops to be used. */
#define R_VAL_ADP_DEFAULT_COORD_ROUTE_ENABLED  (R_TRUE)   /*!< Controls if the default coordinator route is added to the routing table */
#define R_VAL_ADP_MAX_JOIN_WAIT_TIME     (20u)       /*!< Maximum join time of a bootstraping node until timeout in seconds */
#define R_VAL_ADP_LOW_LQI                (0u)        /*!< If the LQI is lower than the defined threshold the link is considered broken */
#define R_VAL_ADP_HIGH_LQI               (255u)      /*!< If the LQI is higher than the defined threshold the link is considered perfect */
#define R_VAL_ADP_WEAK_LQI               (52u)       /*!< If the LQI is lower than the defined threshold the link is considered weak */
#define R_VAL_ADP_KQ                     (10u)       /*!< Multiplier to control the impact of LQI values on the computed link cost */
#define R_VAL_ADP_KRT                    (0u)        /*!< Multiplier to control the impact of routing table entries on the computed link cost */
#define R_VAL_ADP_DEFAULT_ROUTING_DISABLE      (R_FALSE)   /*!< Controls the default disable routing when routeType=ROUTE_TYPE_NORMAL */

#define R_VAL_ADP_NET_TRAVERSAL_TIME     (20u)       /*!< adpNetTraversalTime to be used */
#define R_VAL_ADP_RREP_WAIT              (4u)        /*!< adpRREPWait to be used */


#else
/******************************************************************************
Metering Profile Settings
******************************************************************************/
/* Bootstrapping module configuration */
#define R_MIN_JOIN_LQI_START            (80u)       /*!< Maximum LQI value for the first discovery */
#define R_MIN_JOIN_LQI_STEP             (4u)        /*!< Down LQI step for following discoveries */
#define R_WAIT_BETWEEN_DISCOVERIES      (3000u)     /*!< Period between two route discoveries (milliseconds) */
#define R_SCAN_DURATION                 (5u)       /*!< Network discovery period */
#define R_JOIN_RETRYNUM_TO_SAME_LBA     (2u)        /*!< Retry number for ADPM-NETOWORK-JOIN */
#define R_JOIN_RETRYNUM_PER_DISCOVERY   (3u)        /*!< Change LBA from PAN descriptor */
#define R_WAIT_BETWEEN_JOINREQ          (5000u)     /*!< Period between two join requests (milliseconds) */
#define R_WEAK_LQI_TH                   (60u)       /*!< Minimum LQI value for the first discovery */
#define R_ROUTE_DISCOVERY_AFTER_JOIN    (R_TRUE)    /*!< Activate/Deactivate automatic route discovery after join completion */
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
#define R_MIN_JOIN_LQI_START_RF         (100u)       /*!< Maximum LQI value for the first discovery for RF media */
#define R_WEAK_LQI_TH_RF                (70u)       /*!< Minimum LQI value for the first discovery for RF media */
#endif

/* MAC PIB Settings */
#define R_VAL_MAC_MAX_BE                (9)         /*!< macMaxBe to be used */
#define R_VAL_MAC_MIN_BE                (3)         /*!< macMinBe to be used */
#define R_VAL_MAC_A                     (3)         /*!< macA to be used */
#define R_VAL_MAC_BEACON_RAND_WIDTH     (45)        /*!< macBeaconRandomizationWindowLength to be used */
#define R_VAL_MAC_BCAST_TX_TIMEOUT      (5)         /*!< macBroadcastDataTxTimeout to be used */
#define R_VAL_MAC_RETRY_MAC_CW          (R_FALSE)   /*!< macRetryMaxCw to be used */

/* ADP IB Settings. */
#define R_VAL_ADP_MAX_HOPS               (14u)       /*!< Maximum number of hops to be used. */
#define R_VAL_ADP_DEFAULT_COORD_ROUTE_ENABLED  (R_TRUE)    /*!< Controls if the default coordinator route is added to the routing table */
#define R_VAL_ADP_MAX_JOIN_WAIT_TIME     (180u)      /*!< Maximum join time of a bootstraping node until timeout in seconds */
#define R_VAL_ADP_LOW_LQI                (52u)       /*!< If the LQI is lower than the defined threshold the link is considered broken */
#define R_VAL_ADP_HIGH_LQI               (100u)      /*!< If the LQI is higher than the defined threshold the link is considered perfect */
#define R_VAL_ADP_WEAK_LQI               (58u)       /*!< If the LQI is lower than the defined threshold the link is considered weak */
#define R_VAL_ADP_KQ                     (30u)       /*!< Multiplier to control the impact of LQI values on the computed link cost */
#define R_VAL_ADP_KRT                    (8u)        /*!< Multiplier to control the impact of routing table entries on the computed link cost */
#define R_VAL_ADP_DEFAULT_ROUTING_DISABLE      (R_FALSE)    /*!< Controls the default disable routing when routeType=ROUTE_TYPE_NORMAL */

#define R_VAL_ADP_NET_TRAVERSAL_TIME     (40u)       /*!< adpNetTraversalTime to be used */
#define R_VAL_ADP_RREP_WAIT              (8u)        /*!< adpRREPWait to be used */


#endif
/******************************************************************************
CerTification Profile Settings
******************************************************************************/
#define R_SCAN_DURATION_CERT            (5u)         /*!< Network discovery period */


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Functions prototype
******************************************************************************/

#endif /* R_DEMO_PARAMETERS_H */

