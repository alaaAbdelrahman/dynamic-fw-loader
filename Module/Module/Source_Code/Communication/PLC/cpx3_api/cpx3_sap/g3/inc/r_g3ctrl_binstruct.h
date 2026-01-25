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
 * File Name : r_g3ctrl_binstruct.h
 * Description : G3 layer bin struct
 ******************************************************************************/

#ifndef R_G3CTRL_BINSTRUCT_H
#define R_G3CTRL_BINSTRUCT_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_G3CTRL_AVOID_ERROR  (4u)

/******************************************************************************
Typedef definitions
******************************************************************************/

/*===========================================================================*/
/* For G3 SAP ID                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* For G3 SAP IF ENUM                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* For G3 SAP IF STRUCT                                                     */
/*===========================================================================*/

typedef struct
{
    uint8_t binG3mode;
    uint8_t binNeighbourTableSize[2];
    uint8_t binDeviceTableSize[2];
} r_g3_mac_init_req_bin_t;


typedef struct
{
    uint8_t binG3mode;
    uint8_t binNeighbourTableSize[2];
    uint8_t binDeviceTableSize[2];
    uint8_t binPanDescriptorNum[2];
    uint8_t binAdpRouteTableSize[2];
    uint8_t binAdpdBuffNum[2];
    uint8_t binRouteType;
} r_g3_adp_init_req_bin_t;


typedef struct
{
    uint8_t binG3mode;
    uint8_t binNeighbourTableSize[2];
    uint8_t binDeviceTableSize[2];
    uint8_t binPanDescriptorNum[2];
    uint8_t binAdpRouteTableSize[2];
    uint8_t binAdpdBuffNum[2];
    uint8_t binRouteType;
    uint8_t binEapLbpBuffNum[2];
    uint8_t binEapCinfoTableNum[2];
} r_g3_eap_init_req_bin_t;


typedef struct
{
    uint8_t binG3mode;
    uint8_t binNeighbourTableSize[2];
    uint8_t binDeviceTableSize[2];
    uint8_t binPosTableSize_RF[2];
} r_g3_hymac_init_req_bin_t;

typedef struct
{
    uint8_t binG3mode;
    uint8_t binNeighbourTableSize[2];
    uint8_t binDeviceTableSize[2];
    uint8_t binPanDescriptorNum[2];
    uint8_t binAdpRouteTableSize[2];
    uint8_t binAdpdBuffNum[2];
    uint8_t binRouteType;
    uint8_t binPosTableSize_RF[2];
} r_g3_hyadp_init_req_bin_t;


typedef struct
{
    uint8_t binG3mode;
    uint8_t binNeighbourTableSize[2];
    uint8_t binDeviceTableSize[2];
    uint8_t binPanDescriptorNum[2];
    uint8_t binAdpRouteTableSize[2];
    uint8_t binAdpdBuffNum[2];
    uint8_t binRouteType;
    uint8_t binEapLbpBuffNum[2];
    uint8_t binEapCinfoTableNum[2];
    uint8_t binPosTableSize_RF[2];
} r_g3_hyeap_init_req_bin_t;



typedef struct
{
    uint8_t     binStatus;
    uint8_t     binFailedLayer;
} r_g3_init_cnf_bin_t;

typedef struct
{
    uint8_t binG3mode;
    uint8_t binBandPlan;
    uint8_t binReserved[4];
    uint8_t binExtendedAddress[8];
} r_g3_mac_set_config_req_bin_t;


typedef struct
{
    uint8_t binG3mode;
    uint8_t binBandPlan;
    uint8_t binReserved[4];
    uint8_t binExtendedAddress[8];
    uint8_t binPsk[16];
    uint8_t binExtIDFlg;
} r_g3_adp_set_config_req_bin_t;


typedef struct
{
    uint8_t binG3mode;
    uint8_t binBandPlan;
    uint8_t binReserved[4];
    uint8_t binExtendedAddress[8];
    uint8_t binPsk[16];
    uint8_t binExtIDFlg;
    uint8_t binExtIDLength;
    uint8_t binExtID[36];
} r_g3_adpex_set_config_req_bin_t;


typedef struct
{
    uint8_t binStatus;
} r_g3_set_config_cnf_bin_t;


typedef struct
{
    uint8_t binStatus;
    uint8_t binG3mode;
    uint8_t binBandPlan;
    uint8_t binReserved[4];
    uint8_t binExtendedAddress[8];
    uint8_t binPsk[16];
    uint8_t binExtIDFlg;
    uint8_t binExtIDLength;
    uint8_t binExtID[36];
} r_g3_get_config_cnf_bin_t;


typedef struct
{

    uint8_t binEventCode;
    uint8_t binLength[2];
    uint8_t binParam[R_G3CTRL_AVOID_ERROR];
} r_g3_event_ind_bin_t;


typedef struct
{
    uint8_t binInfoTypeBit;
    uint8_t binInfoLayerBit[2];
} r_g3_clear_info_req_bin_t;


typedef struct
{
    uint8_t binStatus;
} r_g3_clear_info_cnf_bin_t;


typedef struct
{
    uint8_t binInfoType;
    uint8_t binInfoLayer;
} r_g3_get_info_req_bin_t;


typedef struct
{
    uint8_t binStatus;
    uint8_t binInfoType;
    uint8_t binInfoLayer;
    uint8_t binLength[2];
    uint8_t binInfo[R_G3CTRL_AVOID_ERROR];
} r_g3_get_info_cnf_bin_t;


typedef struct
{
    uint8_t binMaxDmpLen[4];
    uint8_t binTimeOut[4];
    uint8_t binDmpMode;
    uint8_t binReserved[3];
    uint8_t binSegmentLen[2];
    uint8_t binSegmentInterval[4];
} r_g3_dump_req_bin_t;

typedef struct
{
    uint8_t binStatus;
    uint8_t binResult;
    uint8_t binSegmentNum[2];
    uint8_t binSegmentLen[2];
    uint8_t binSegmentInterval[4];
    uint8_t binDmpLen[4];
} r_g3_dump_cnf_bin_t;

typedef struct
{
    uint8_t binSeqNum[2];
    uint8_t binEndFlg;
    uint8_t binSegmentLen[2];
    uint8_t binDmpData[R_G3CTRL_AVOID_ERROR];
} r_g3_dump_ind_bin_t;

typedef struct
{
    uint8_t binStatus;
} r_g3_dump_abort_cnf_bin_t;


/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

#endif  /* R_G3CTRL_BINSTRUCT_H */

