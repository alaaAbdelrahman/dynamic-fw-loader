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
 * File Name : r_adp_binstruct.h
 * Description : G3 ADP layer bin struct
 ******************************************************************************/

#ifndef R_ADP_BINSTRUCT_H
#define R_ADP_BINSTRUCT_H

#include "r_g3mac_binstruct.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_ADP_AVOID_ERROR  (4u)

/******************************************************************************
Typedef definitions
******************************************************************************/

/*===========================================================================*/
/* For ADP SAP ID                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* For ADP SAP IF ENUM                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* For ADP SAP IF STRUCT                                                     */
/*===========================================================================*/
typedef struct
{
    uint8_t binNsduLength[2];
    uint8_t binNsdu[R_ADP_AVOID_ERROR];
    uint8_t nsduHandle;
    uint8_t discoverRoute;
    uint8_t qualityOfService;
} r_adp_adpd_data_req_bin_t;

typedef struct
{
    uint8_t nsduHandle;
    uint8_t discoverRoute;
    uint8_t qualityOfService;
} r_adp_adpd_data_req_back_bin_t;


typedef r_adp_adpd_data_cnf_t r_adp_adpd_data_cnf_bin_t;

typedef struct
{
    uint8_t binNsduLength[2];
    uint8_t binNsdu[R_ADP_AVOID_ERROR];
    uint8_t linkQualityIndicator;
} r_adp_adpd_data_ind_bin_t;

typedef r_adp_adpm_discovery_req_t r_adp_adpm_discovery_req_bin_t;


typedef struct
{
    uint8_t binPanId[2];
    uint8_t address[2];
    uint8_t linkQuality;
    uint8_t mediaType;
    uint8_t binRcCoord[2];
} adp_pan_descriptor_bin_t;

typedef struct
{
    uint8_t status;
    uint8_t PANCount;
    adp_pan_descriptor_bin_t binPANDescriptor[R_ADP_AVOID_ERROR];
} r_adp_adpm_discovery_cnf_bin_t;

typedef struct
{
    uint8_t status;
    uint8_t PANCount;
} r_adp_adpm_discovery_cnf_bbin_t;

typedef struct
{
    uint8_t binPanId[2];
} r_adp_adpm_nwk_start_req_bin_t;

typedef struct
{
    uint8_t status;
} r_adp_adpm_nwk_start_cnf_bin_t;

typedef struct
{
    uint8_t binPanId[2];
    uint8_t lbaAddress[2];
    uint8_t mediaType;
} r_adp_adpm_nwk_join_req_bin_t;

typedef struct
{
    uint8_t status;
    uint8_t networkAddress[2];
    uint8_t binPanId[2];
    uint8_t mediaType;
} r_adp_adpm_nwk_join_cnf_bin_t;

typedef struct
{
    uint8_t status;
} r_adp_adpm_nwk_leave_cnf_bin_t;

typedef r_adp_adpm_reset_cnf_t r_adp_adpm_reset_cnf_bin_t;

typedef struct
{
    uint8_t aibAttributeId;
    uint8_t binAibAttributeIndex[2];
} r_adp_adpm_get_req_bin_t;

typedef struct
{
    uint8_t status;
    uint8_t aibAttributeId;
    uint8_t binAibAttributeIndex[2];
    uint8_t binAibAttributeValue[R_ADP_MAX_IB_SIZE];
} r_adp_adpm_get_cnf_bin_t;

typedef struct
{
    uint8_t aibAttributeId;
    uint8_t binAibAttributeIndex[2];
    uint8_t binAibAttributeValue[R_ADP_MAX_IB_SIZE];
} r_adp_adpm_set_req_bin_t;

typedef struct
{
    uint8_t status;
    uint8_t aibAttributeId;
    uint8_t binAibAttributeIndex[2];
} r_adp_adpm_set_cnf_bin_t;

typedef struct
{
    uint8_t status;
    uint8_t binPanId[2];
    uint8_t srcAddrMode;
    uint8_t srcAddr[8];
    uint8_t dstAddrMode;
    uint8_t dstAddr[8];
    uint8_t securityLevel;
    uint8_t keyIndex;
    uint8_t mediaType;
} r_adp_adpm_nwk_status_ind_bin_t;

typedef r_adp_adpm_route_disc_req_t r_adp_adpm_route_disc_req_bin_t;

typedef r_adp_adpm_route_disc_cnf_t r_adp_adpm_route_disc_cnf_bin_t;

typedef struct
{
    uint8_t dstAddress[2];
    uint8_t pathMetricType;
} r_adp_adpm_path_disc_req_bin_t;


typedef struct
{
    uint8_t            dstAddr[2];
    uint8_t            status;
    uint8_t            pathMetricType;
    uint8_t            origAddr[2];
    uint8_t            pathTableEntries;
    r_adp_path_table_t binPathTable[R_ADP_AVOID_ERROR];
} r_adp_adpm_path_disc_cnf_bin_t;

typedef struct
{
    uint8_t dstAddr[2];
    uint8_t status;
    uint8_t pathMetricType;
    uint8_t origAddr[2];
    uint8_t pathTableEntries;
} r_adp_adpm_path_disc_cnf_bbin_t;

typedef r_adp_adpm_path_discovery_ind_t r_adp_adpm_path_disc_ind_bin_t;

typedef struct
{
    uint8_t dstAddrType;
    uint8_t dstAddr[8];
    uint8_t binNsduLength[2];
    uint8_t binNsdu[R_ADP_AVOID_ERROR];
    uint8_t nsduHandle;
    uint8_t maxHops;
    uint8_t discoverRoute;
    uint8_t qualityOfService;
    uint8_t securityEnabled;
    uint8_t mediaType;
} r_adp_adpm_lbp_req_bin_t;

typedef struct
{
    uint8_t nsduHandle;
    uint8_t maxHops;
    uint8_t discoverRoute;
    uint8_t qualityOfService;
    uint8_t securityEnabled;
    uint8_t mediaType;
} r_adp_adpm_lbp_req_back_bin_t;


typedef r_adp_adpm_lbp_cnf_t r_adp_adpm_lbp_cnf_bin_t;


typedef struct
{
    uint8_t srcAddr[2];
    uint8_t binNsduLength[2];
    uint8_t binNsdu[R_ADP_AVOID_ERROR];
    uint8_t linkQualityIndicator;
    uint8_t securityEnabled;
    uint8_t mediaType;
} r_adp_adpm_lbp_ind_bin_t;

typedef struct
{
    uint8_t bufferReady;
} r_adp_adpm_buffer_ind_bin_t;

typedef struct
{
    uint8_t keyIndex;
    uint8_t keyState;   // 0:receive 1:activate  2:remove
} r_adp_adpm_key_state_ind_bin_t;

typedef struct
{
    uint8_t unreachableAddress[2];
} r_adp_adpm_route_err_ind_bin_t;

typedef r_adp_adpm_eap_key_ind_t r_adp_adpm_eap_key_ind_bin_t;

typedef struct
{
    uint8_t binFrameCounter[4];
    uint8_t  mediaType;
} r_adp_adpm_fcounter_ind_bin_t;


/* ADPSP-MACRESET */
typedef struct
{
    uint8_t MAC_reboot; /* 0:reset recovery 1:fw download recovery */
} r_adp_adpsp_macreset_req_bin_t;

typedef r_g3mac_mlme_reset_cnf_t r_adp_adpsp_macreset_cnf_bin_t;

/* ADPM-BEACON NOTIFY */
typedef r_g3mac_mlme_bcn_nfy_ind_bin_t r_adp_adpm_bcn_nfy_ind_bin_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#endif /* #ifndef R_ADP_BINSTRUCT_H */

