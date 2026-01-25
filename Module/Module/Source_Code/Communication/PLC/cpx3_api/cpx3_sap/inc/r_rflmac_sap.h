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
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
 * File Name : r_rflmac_sap.h
 * Description : RF LMAC layer API
 ******************************************************************************/
#include "r_g3mac_sap.h"

#ifndef R_RFLMAC_SAP_H
#define R_RFLMAC_SAP_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_RFLMAC_MAX_IB_SIZE      (8u)

/******************************************************************************
Macro definitions
******************************************************************************/

typedef enum
{
    R_RFLMACIB_PHY_CURRENT_CHANNEL               = 0,
    R_RFLMACIB_PHY_CHANNELS_SUPPORTED            = 1,
    R_RFLMACIB_PHY_TRANSMIT_POWER                = 2,
    R_RFLMACIB_PHY_CURRENT_PAGE                  = 3,
    R_RFLMACIB_PHY_FSK_FEC_TX_ENA                = 4,
    R_RFLMACIB_PHY_FSK_FEC_RX_ENA                = 5,
    R_RFLMACIB_PHY_FSK_FEC_SCHEME                = 6,
    R_RFLMACIB_PHY_CCA_VTH                       = 7,
    R_RFLMACIB_PHY_LVLFLTR_VTH                   = 8,
    R_RFLMACIB_PHY_BACKOFF_SEED                  = 9,
    R_RFLMACIB_PHY_CRCERROR_UPMSG                = 10,
    R_RFLMACIB_MAC_ADDRESS_FILTER1_ENA           = 11,
    R_RFLMACIB_MAC_SHORT_ADDRESS1                = 12,
    R_RFLMACIB_MAC_PANID1                        = 13,
    R_RFLMACIB_MAC_EXTENDED_ADDRESS1             = 14,
    R_RFLMACIB_MAC_PAN_COORD1                    = 15,
    R_RFLMACIB_MAC_FRAME_PEND1                   = 16,
    R_RFLMACIB_MAC_ADDRESS_FILTER2_ENA           = 17,
    R_RFLMACIB_MAC_SHORT_ADDRESS2                = 18,
    R_RFLMACIB_MAC_PANID2                        = 19,
    R_RFLMACIB_MAC_EXTENDED_ADDRESS2             = 20,
    R_RFLMACIB_MAC_PAN_COORD2                    = 21,
    R_RFLMACIB_MAC_FRAME_PEND2                   = 22,
    R_RFLMACIB_MAC_MAXCSMABACKOFF                = 23,
    R_RFLMACIB_MAC_MINBE                         = 24,
    R_RFLMACIB_MAC_MAXBE                         = 25,
    R_RFLMACIB_MAC_MAX_FRAME_RETRIES             = 26,
    R_RFLMACIB_PHY_CCA_DURATION                  = 27,
    R_RFLMACIB_PHY_FSK_PREAMBLE_LENGTH           = 28,
    R_RFLMACIB_PHY_MRFSK_SFD                     = 29,
    R_RFLMACIB_PHY_FSK_SCRAMBLE_PSDU             = 30,
    R_RFLMACIB_PHY_FSK_OPE_MODE                  = 31,
    R_RFLMACIB_PHY_FCS_LENGTH                    = 32,
    R_RFLMACIB_PHY_ACK_REPLY_TIME                = 33,
    R_RFLMACIB_PHY_ACK_WAIT_DURATION             = 34,
    R_RFLMACIB_PHY_PROFILE_SPECIFIC_MODE         = 35,
    R_RFLMACIB_PHY_ANTENNA_SWITCH_ENA            = 36,
    R_RFLMACIB_PHY_ANTENNA_DIVERSITY_RX_ENA      = 37,
    R_RFLMACIB_PHY_ANTENNA_SELECT_TX             = 38,
    R_RFLMACIB_PHY_ANTENNA_SELECT_ACKTX          = 39,
    R_RFLMACIB_PHY_ANTENNA_SELECT_ACKRX          = 40,
    R_RFLMACIB_PHY_RX_TIMEOUT_MODE               = 41,
    R_RFLMACIB_PHY_FREQ_BAND_ID                  = 42,
    R_RFLMACIB_PHY_DATA_RATE                     = 43,
    R_RFLMACIB_PHY_ARIB_STD_T108_MODE            = 44,
    R_RFLMACIB_PHY_CHANNELS_SUPPORTED_PAGE       = 45,
    R_RFLMACIB_PHY_CSMA_BACKOFF_PERIOD           = 46,
    R_RFLMACIB_PHY_PREAMBLE_4BYTE_RX_MODE        = 47,
    R_RFLMACIB_PHY_AGC_START_VTH                 = 48,
    R_RFLMACIB_PHY_CCA_BANDWIDTH                 = 49,
    R_RFLMACIB_PHY_ED_BANDWIDTH                  = 50,
    R_RFLMACIB_PHY_ANTENNA_DIVERSITY_START_VTH   = 51,
    R_RFLMACIB_PHY_ANTENNA_SWITCHING_TIME        = 52,
    R_RFLMACIB_PHY_SFD_DETECTION_EXTEND          = 53,
    R_RFLMACIB_PHY_AGC_WAIT_GAIN_OFFSET          = 54,
    R_RFLMACIB_PHY_CCA_VTH_OFFSET                = 55,
    R_RFLMACIB_PHY_ANTENNA_SWITCH_ENA_TIMING     = 56,
    R_RFLMACIB_PHY_GPIO0_SETTING                 = 57,
    R_RFLMACIB_PHY_GPIO3_SETTING                 = 58,

} r_rflmac_ib_id_t;

/*===========================================================================*/
/* For RF-LMAC SAP IF STRUCT                                                 */
/*===========================================================================*/
/* RF-LML-RESET */
typedef struct
{
    r_g3mac_status_t        status;
} r_rflmac_lml_reset_cnf_t;

/* RF-LML-START */
typedef struct
{
    r_g3mac_status_t        status;
} r_rflmac_lml_start_cnf_t;

/* RF-LML-GET */
typedef struct
{
    uint16_t                paramId;
} r_rflmac_lml_get_req_t;

typedef struct
{
    r_g3mac_status_t        status;
    uint16_t                paramId;
    uint8_t                 content[R_RFLMAC_MAX_IB_SIZE];
} r_rflmac_lml_get_cnf_t;

/* RF-LML-SET */
typedef struct
{
    uint16_t                paramId;
    uint8_t *               pContent;
} r_rflmac_lml_set_req_t;

typedef struct
{
    r_g3mac_status_t        status;
    uint16_t                paramId;
} r_rflmac_lml_set_cnf_t;

/* RF-LML-DATA */
typedef struct
{
    uint8_t                 options;
    uint32_t                time;
    uint16_t                psduLen;
    uint8_t *               psdu;
} r_rflmac_lml_data_req_t;

typedef struct
{
    r_g3mac_status_t        status;
} r_rflmac_lml_data_cnf_t;

typedef struct
{
    uint32_t                rxTime;
    uint8_t                 lqi;
    uint8_t                 crc;
    uint8_t                 phr;
    uint8_t                 status;
    uint16_t                rssi;
    uint16_t                psduLen;
    uint8_t                 currentChannel;
    uint8_t *               psdu;
} r_rflmac_lml_data_ind_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#endif  /*  R_RFLMACSAP_H   */

