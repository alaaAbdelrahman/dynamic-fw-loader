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
 * File Name : r_g3_sap.h
 * Description : G3 layer API
 ******************************************************************************/

#ifndef R_G3_SAP_H
#define R_G3_SAP_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_G3_INFO_BYTESIZE_MAX                  (1024u)
#define R_G3_CH_MAX                             (2u)

#define R_G3_INFO_TYPE_BIT_STATS                (0x01u)
#define R_G3_INFO_TYPE_BIT_LOG                  (0x02u)
#define R_G3_INFO_TYPE_BIT_ALL                  (0x00u)

#define R_G3_MODE_BIT_HYBRID                    (0x20u)
#define R_G3_MODE_BIT_RF                        (0x10u)


#define R_G3_CBMAP_RF_MCPS_DATA_CNF              (0x00000001)
#define R_G3_CBMAP_RF_MCPS_DATA_IND              (0x00000002)
#define R_G3_CBMAP_RF_MLME_RESET_CNF             (0x00000004)
#define R_G3_CBMAP_RF_MLME_GET_CNF               (0x00000008)
#define R_G3_CBMAP_RF_MLME_SET_CNF               (0x00000010)
#define R_G3_CBMAP_RF_MLME_SCAN_CNF              (0x00000020)
#define R_G3_CBMAP_RF_MLME_START_CNF             (0x00000040)
#define R_G3_CBMAP_RF_MLME_BEACON_NOTIFY_IND     (0x00000080)
#define R_G3_CBMAP_RF_MLME_COMM_STATUS_IND       (0x00000100)
#define R_G3_CBMAP_RF_MLME_FRAMECOUNT_IND        (0x00000200)
#define R_G3_CBMAP_RF_MLME_WARNING_IND           (0x00000400)

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum
{
    R_G3_STATUS_SUCCESS              = 0,

    /* shared with ADP status */
    R_G3_STATUS_INVALID_REQUEST      = 0x80,

    /* IF status */
    R_G3_STATUS_INSUFFICIENT_MEMSIZE = 0xA0,
    R_G3_STATUS_NO_RESPONSE          = 0xA4,
    R_G3_STATUS_UNINITIALIZED_STATE  = 0xA5,
    R_G3_STATUS_REQUEST_FAILED       = 0xA6,

    R_G3_STATUS_IF_FATAL_ERROR       = 0xB0,
    R_G3_STATUS_IF_TIMEOUT           = 0xB1,
    R_G3_STATUS_IF_QUEUE_FULL        = 0xB2,
    R_G3_STATUS_IF_INVALID_STATE     = 0xB3,
    R_G3_STATUS_IF_INVALID_PARAMETER = 0xB4,
    R_G3_STATUS_IF_INVALID_RESPONSE  = 0xB5,
    R_G3_STATUS_IF_NO_RESPONSE       = 0xB6,

    /* shared with MAC status */
    R_G3_STATUS_INVALID_PARAMETER    = 0xE8,
    R_G3_STATUS_INVALID_STATE        = 0xFD,

} r_g3_status_t;


typedef enum
{
    R_G3_MODE_LMAC = 0x02,
    R_G3_MODE_MAC,
    R_G3_MODE_ADP,
    R_G3_MODE_EAP,
    R_G3_MODE_HYBRID_MAC = (R_G3_MODE_MAC|R_G3_MODE_BIT_HYBRID),
    R_G3_MODE_HYBRID_ADP = (R_G3_MODE_ADP|R_G3_MODE_BIT_HYBRID),
    R_G3_MODE_HYBRID_EAP = (R_G3_MODE_EAP|R_G3_MODE_BIT_HYBRID),
    R_G3_MODE_RF_MAC     = (R_G3_MODE_MAC|R_G3_MODE_BIT_RF),
    R_G3_MODE_RF_ADP     = (R_G3_MODE_ADP|R_G3_MODE_BIT_RF),
    R_G3_MODE_RF_EAP     = (R_G3_MODE_EAP|R_G3_MODE_BIT_RF)
} r_g3_mode_t;

typedef enum
{
    R_G3_INFO_LAYER_LMAC_DSP = 0x01,
    R_G3_INFO_LAYER_UMAC     = 0x03,
    R_G3_INFO_LAYER_ADP,
    R_G3_INFO_LAYER_EAP,
    R_G3_INFO_LAYER_RF_MAC   = 0x0B,
    R_G3_INFO_LAYER_END,
} r_g3_info_layer_t;

typedef enum
{
    R_G3_INFO_LAYER_BIT_ALL         = 0u,
    R_G3_INFO_LAYER_BIT_LMAC_DSP    = (1u<<R_G3_INFO_LAYER_LMAC_DSP),
    R_G3_INFO_LAYER_BIT_UMAC        = (1u<<R_G3_INFO_LAYER_UMAC),
    R_G3_INFO_LAYER_BIT_ADP         = (1u<<R_G3_INFO_LAYER_ADP),
    R_G3_INFO_LAYER_BIT_EAP         = (1u<<R_G3_INFO_LAYER_EAP),
    R_G3_INFO_LAYER_BIT_RF_MAC      = (1u<<R_G3_INFO_LAYER_RF_MAC),
} r_g3_info_layer_bit_t;

typedef enum
{
    R_G3_INFO_TYPE_STATS = 0x0,
    R_G3_INFO_TYPE_LOG,
    R_G3_INFO_TYPE_END,
} r_g3_info_type_t;

typedef enum
{
    R_G3_BANDPLAN_CENELEC_A = 0x0,
    R_G3_BANDPLAN_CENELEC_B,
    R_G3_BANDPLAN_ARIB,
    R_G3_BANDPLAN_FCC,
    R_G3_BANDPLAN_END,
} r_g3_bandplan_t;

typedef enum
{
    R_G3_LAYER_G3CTRL = 0x00,
    R_G3_LAYER_LMAC = 0x02,
    R_G3_LAYER_UMAC,
    R_G3_LAYER_ADP,
    R_G3_LAYER_EAP,
    R_G3_LAYER_RFMAC = 0x0B,
    R_G3_LAYER_END,
} r_g3_layer_t;

/*===========================================================================*/
/* Request command struct definitions                                        */
/*===========================================================================*/

/* G3-INIT */

typedef  struct
{
    uint16_t neighbourTableSize;
    uint16_t deviceTableSize;
} r_g3_mac_init_req_t;

typedef  struct
{
    uint16_t neighbourTableSize;
    uint16_t deviceTableSize;
    uint16_t posTableSize_RF;
} r_g3_hymac_init_req_t;

typedef struct
{
    uint16_t reserved[2];
    uint16_t posTableSize_RF;
} r_g3_rfmac_init_req_t;

typedef struct
{
    uint16_t neighbourTableSize;
    uint16_t deviceTableSize;
    uint16_t panDescriptorNum;
    uint16_t routeTableSize;
    uint16_t adpdBuffNum;
    uint8_t  routeType;             /* reserved */
} r_g3_adp_init_req_t;

typedef struct
{
    uint16_t neighbourTableSize;
    uint16_t deviceTableSize;
    uint16_t panDescriptorNum;
    uint16_t routeTableSize;
    uint16_t adpdBuffNum;
    uint8_t  routeType;             /* reserved */
    uint16_t posTableSize_RF;
} r_g3_hyadp_init_req_t;

typedef struct
{
    uint16_t reserved[2];
    uint16_t panDescriptorNum;
    uint16_t routeTableSize;
    uint16_t adpdBuffNum;
    uint8_t  routeType;             /* reserved */
    uint16_t posTableSize_RF;
} r_g3_rfadp_init_req_t;

typedef struct
{
    uint16_t neighbourTableSize;
    uint16_t deviceTableSize;
    uint16_t panDescriptorNum;
    uint16_t routeTableSize;
    uint16_t adpdBuffNum;
    uint8_t  routeType;             /* reserved */
    uint16_t eapLbpBuffNum;
    uint16_t eapCinfoTableNum;
} r_g3_eap_init_req_t;

typedef struct
{
    uint16_t neighbourTableSize;
    uint16_t deviceTableSize;
    uint16_t panDescriptorNum;
    uint16_t routeTableSize;
    uint16_t adpdBuffNum;
    uint8_t  routeType;             /* reserved */
    uint16_t eapLbpBuffNum;
    uint16_t eapCinfoTableNum;
    uint16_t posTableSize_RF;
} r_g3_hyeap_init_req_t;

typedef struct
{
    uint16_t reserved[2];
    uint16_t panDescriptorNum;
    uint16_t routeTableSize;
    uint16_t adpdBuffNum;
    uint8_t  routeType;             /* reserved */
    uint16_t eapLbpBuffNum;
    uint16_t eapCinfoTableNum;
    uint16_t posTableSize_RF;
} r_g3_rfeap_init_req_t;

typedef union
{
    uint32_t                reserved;
    r_g3_mac_init_req_t     mac;
    r_g3_adp_init_req_t     adp;
    r_g3_eap_init_req_t     eap;
    r_g3_hymac_init_req_t   hymac;
    r_g3_hyadp_init_req_t   hyadp;
    r_g3_hyeap_init_req_t   hyeap;
    r_g3_rfmac_init_req_t   rfmac;
    r_g3_rfadp_init_req_t   rfadp;
    r_g3_rfeap_init_req_t   rfeap;

} r_g3_unit_init_req_t;

typedef struct
{
    uint8_t                 g3mode;         /* r_g3_mode_t */
    r_g3_unit_init_req_t    init;
} r_g3_init_req_t;

typedef struct
{
    uint8_t                 status;
    uint8_t                 failedLayer;    /* r_g3_layer_t */
} r_g3_init_cnf_t;

/* G3-DEINIT */
typedef struct
{
    uint8_t                 status;
} r_g3_deinit_cnf_t;

/* G3-GETCONFIG */
typedef struct
{
    uint8_t extIDLength;
    uint8_t extID[36];
} r_g3_config_extid_t;

typedef struct
{
    uint8_t               status;
    uint8_t               g3mode;   /* r_g3_mode_t */
    uint8_t               bandPlan; /* r_g3_bandplan_t */
    uint32_t              reserved;
    uint8_t               extendedAddress[8];
    uint8_t               psk[16];
    uint8_t               extIDFlg;
    r_g3_config_extid_t * pExtId;
} r_g3_get_config_cnf_t;

/* G3-SETCONFIG */
typedef struct
{
    uint8_t  bandPlan;              /* r_g3_bandplan_t */
    uint32_t reserved;
    uint8_t  extendedAddress[8];
} r_g3_mac_set_config_req_t;

typedef struct
{
    uint8_t               bandPlan; /* r_g3_bandplan_t */
    uint32_t              reserved;
    uint8_t               extendedAddress[8];
    uint8_t               psk[16];
    uint8_t               extIDFlg;
    r_g3_config_extid_t * pExtId;
} r_g3_adp_set_config_req_t;

typedef union
{
    r_g3_mac_set_config_req_t mac;
    r_g3_adp_set_config_req_t adp;
} r_g3_unit_set_config_req_t;

typedef struct
{
    uint8_t g3mode;                 /* r_g3_mode_t */
    r_g3_unit_set_config_req_t config;
} r_g3_set_config_req_t;

typedef struct
{
    uint8_t status;
} r_g3_set_config_cnf_t;

/* G3-CLEARINFO */
typedef struct
{
    uint8_t  infoTypeBit;
    uint16_t infoLayerBit;
} r_g3_clear_info_req_t;

/* G3-CLEARINFO */
typedef struct
{
    uint8_t status;
} r_g3_clear_info_cnf_t;

/* G3-GETINFO */
typedef struct
{
    uint8_t infoType;               /* r_g3_info_type_t */
    uint8_t infoLayer;              /* r_g3_info_layer_t */
} r_g3_get_info_req_t;

typedef struct
{
    uint8_t    status;
    uint8_t    infoType;            /* r_g3_info_layer_t */
    uint8_t    infoLayer;           /* r_g3_info_type_t */
    uint16_t   length;
    uint32_t * pInfo;
} r_g3_get_info_cnf_t;




/* G3-DUMP */
typedef struct
{
    uint32_t  maxDmpLen;
    uint32_t  timeOut;
    uint8_t   dmpMode;
    uint8_t   reserved[3];
    uint16_t  segmentLen;
    uint32_t  segmentInterval;
} r_g3_dump_req_t;

typedef struct
{
    uint8_t   status;
    uint8_t   result;
    uint16_t  segmentNum;
    uint16_t  segmentLen;
    uint32_t  segmentInterval;
    uint32_t  dmpLen;
} r_g3_dump_cnf_t;

typedef struct
{
    uint16_t  seqNum;
    uint8_t   endFlg;
    uint16_t  segmentLen;
    uint8_t * pDmpData;
} r_g3_dump_ind_t;

/* G3-DUMP-ABORT */
typedef struct
{
    uint8_t   status;
} r_g3_dump_abort_cnf_t;


/* G3-EVENT */
typedef struct
{
    uint8_t   eventCode;
    uint16_t  length;
    uint8_t * pParam;
} r_g3_event_ind_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#endif /* R_G3_SAP_H */

