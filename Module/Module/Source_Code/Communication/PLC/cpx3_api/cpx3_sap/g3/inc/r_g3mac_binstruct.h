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
 * File Name : r_g3mac_binstruct.h
 * Description : G3 MAC layer bin struct
 ******************************************************************************/

#ifndef R_G3MAC_BINSTRUCT_H
#define R_G3MAC_BINSTRUCT_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_G3MAC_AVOID_ERROR  (4u)

/******************************************************************************
Typedef definitions
******************************************************************************/

/*########################################################################## */
/* MCPS-DATA                                                                 */
/*########################################################################## */
/*  MCPS-DATA.request   */
typedef struct
{
    uint8_t srcAddrMode;
    uint8_t dstAddrMode;
    uint8_t binDstPanId[2];
    uint8_t dstAddr[8];
    uint8_t binMsduLength[2];
    uint8_t msduHandle;
    uint8_t txOptions;
    uint8_t securityLevel;
    uint8_t KeyIndex;
    uint8_t qualityOfService;
    uint8_t binMsdu[R_G3MAC_AVOID_ERROR];
} r_g3mac_mcps_data_req_bin_t;

typedef struct
{
    uint8_t msduHandle;
    uint8_t txOptions;
    uint8_t securityLevel;
    uint8_t KeyIndex;
    uint8_t qualityOfService;
} r_g3mac_mcps_data_req_bbin_t;


/*  MCPS-DATA.confirm   */
typedef struct
{
    uint8_t msduHandle;
    uint8_t status;
    uint8_t rsv;
} r_g3mac_mcps_data_cnf_bin_t;


/*  MCPS-DATA.indication    */
typedef struct
{
    uint8_t srcAddrMode;
    uint8_t binSrcPanId[2];
    uint8_t srcAddr[8];
    uint8_t dstAddrMode;
    uint8_t binDstPanId[2];
    uint8_t dstAddr[8];
    uint8_t binMsduLength[2];
    uint8_t binMsdu[R_G3MAC_AVOID_ERROR];
} r_g3mac_mcps_data_ind_bin_t;


typedef struct
{
    uint8_t                securityLevel;
    uint8_t                keyIndex;
    uint8_t                qualityOfService;
    r_g3mac_neighbor_mod_t estimatedMod;
    uint8_t                estimatedTonemap[3];
    r_g3mac_neighbor_mod_t receivedMod;
    uint8_t                receivedTonemap[3];
    uint8_t                RSSIdb;
    uint8_t                rsv;
} r_g3mac_mcps_data_ind_bbin2_t;

typedef struct
{
    uint8_t                msduLinkQuality;
    uint8_t                dsn;
    r_g3mac_mcps_data_ind_bbin2_t bin;
} r_g3mac_mcps_data_ind_bbin_t;


/*##########################################################################*/
/* MLME-RESET                                                               */
/*##########################################################################*/
/*  MLME-RESET.request  */
typedef r_g3mac_mlme_reset_req_t r_g3mac_mlme_reset_req_bin_t;

/*  MLME-RESET.confirm  */
typedef r_g3mac_mlme_reset_cnf_t r_g3mac_mlme_reset_cnf_bin_t;


/*##########################################################################*/
/* MLME-GET,MLME-SET                                                        */
/*##########################################################################*/
/*  MLME-GET.requset    */
typedef struct
{
    uint8_t binPibAttributeId[2];
    uint8_t binPibAttributeIndex[2];
} r_g3mac_mlme_get_req_bin_t;

/*  MLME-SET.request    */
typedef struct
{
    uint8_t binPibAttributeId[2];
    uint8_t binPibAttributeIndex[2];
    uint8_t binPibAttributeValue[R_G3MAC_MAX_IB_SIZE];
} r_g3mac_mlme_set_req_bin_t;

typedef struct
{
    uint8_t status;
    uint8_t binPibAttributeId[2];
    uint8_t binPibAttributeIndex[2];
    uint8_t pibAttributeValue[R_G3MAC_MAX_IB_SIZE];
} r_g3mac_mlme_get_cnf_bin_t;

/*  MLME-SET.confirm    */
typedef struct
{
    uint8_t status;
    uint8_t binPibAttributeId[2];
    uint8_t binPibAttributeIndex[2];
} r_g3mac_mlme_set_cnf_bin_t;


/*##########################################################################*/
/* MLME-SCAN                                                                */
/*##########################################################################*/
/*  MLME-SCAN.request   */
typedef struct
{
    uint8_t scanDuration;
} r_g3mac_mlme_scan_req_bin_t;

typedef struct
{
    uint8_t binPanId[2];
    uint8_t address[2];
    uint8_t linkQuality;
    uint8_t mediaType;
    uint8_t binRcCoord[2];
} r_g3mac_pan_descriptor_bin_t;

/*  MLME-SCAN.confirm   */
typedef struct
{
    uint8_t status;
} r_g3mac_mlme_scan_cnf_bin_t;


/*  MLME-BEACON-NOTIFY.indication   */
typedef struct
{
    uint8_t phaseDiff;
    r_g3mac_pan_descriptor_bin_t panDescriptor;
} r_g3mac_mlme_bcn_nfy_ind_bin_t;


/*##########################################################################*/
/* MLME-START                                                               */
/*##########################################################################*/
/*  MLME-START.request  */
typedef struct
{
    uint8_t binPanId[2];
} r_g3mac_mlme_start_req_bin_t;


/*  MLME-START.confirm  */
typedef struct
{
    uint8_t status;
} r_g3mac_mlme_start_cnf_bin_t;


/*##########################################################################*/
/* OTHER INDICATION                                                         */
/*##########################################################################*/
/*  MLME-COMM-STATUS.indication */
typedef struct
{
    uint8_t binPanId[2];
    uint8_t srcAddrMode;
    uint8_t srcAddr[8];
    uint8_t dstAddrMode;
    uint8_t dstAddr[8];
    uint8_t status;
    uint8_t securityLevel;
    uint8_t keyIndex;
} r_g3mac_mlme_com_sts_ind_bin_t;

/* MLME-FRAMECOUNT.indication */
typedef struct
{
    uint8_t binFrameCounter[4];
} r_g3mac_mlme_fcount_ind_bin_t;

/* MLME-TMR-RECEIVE */
typedef r_g3mac_mlme_tmr_receive_ind_t  r_g3mac_mlme_tmr_rcv_ind_bin_t;

/* MLME-TMR-TRANSMIT */
typedef r_g3mac_mlme_tmr_transmit_ind_t r_g3mac_mlme_tmr_trs_ind_bin_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

#endif /* R_G3MAC_BINSTRUCT_H */

