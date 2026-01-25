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
 * File Name : r_g3mac_sap.h
 * Description : G3 MAC layer API
 ******************************************************************************/

#ifndef R_G3MAC_SAP_H
#define R_G3MAC_SAP_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_G3MAC_MAX_IB_SIZE  (0x17u)
#define R_G3MAC_KEY_NUM      (2u)

/******************************************************************************
Macro definitions
******************************************************************************/

/*===========================================================================*/
/* For MAC SAP ID                                                            */
/*===========================================================================*/
typedef enum
{
    R_G3MAC_IB_ACKWAIT_DURATION              =   0x0040,
    R_G3MAC_IB_MAX_BE                        =   0x0047,
    R_G3MAC_IB_BSN                           =   0x0049,
    R_G3MAC_IB_DSN                           =   0x004C,
    R_G3MAC_IB_MAX_CSMABACKOFFS              =   0x004E,
    R_G3MAC_IB_MIN_BE                        =   0x004F,
    R_G3MAC_IB_PANID                         =   0x0050,
    R_G3MAC_IB_PROMISCUOUS_MODE              =   0x0051,
    R_G3MAC_IB_SHORTADDRESS                  =   0x0053,
    R_G3MAC_IB_MAX_FRAME_RETRIES             =   0x0059,
    R_G3MAC_IB_TIMESTAMP_SUPPORTED           =   0x005C,
    R_G3MAC_IB_SECURITY_ENABLED              =   0x005D,
    R_G3MAC_IB_KEY_TABLE                     =   0x0071,
    R_G3MAC_IB_DEVICE_TABLE                  =   0x0073,
    R_G3MAC_IB_FRAME_COUNTER                 =   0x0077,
    R_G3MAC_IB_DUPLICATE_DETECTION_TTL       =   0x0078,
    R_G3MAC_IB_HIGHPRIORITY_WINDOWSIZE       =   0x0100,
    R_G3MAC_IB_TXDATAPACKET_COUNT            =   0x0101,
    R_G3MAC_IB_RXDATAPACKET_COUNT            =   0x0102,
    R_G3MAC_IB_TXCMDPACKET_COUNT             =   0x0103,
    R_G3MAC_IB_RXCMDPACKET_COUNT             =   0x0104,
    R_G3MAC_IB_CSMAFAIL_COUNT                =   0x0105,
    R_G3MAC_IB_CSMANOACK_COUNT               =   0x0106,
    R_G3MAC_IB_RXDATABROADCAST_COUNT         =   0x0107,
    R_G3MAC_IB_TXDATABROADCAST_COUNT         =   0x0108,
    R_G3MAC_IB_BADCRC_COUNT                  =   0x0109,
    R_G3MAC_IB_NEIGHBOUR_TABLE               =   0x010A,
    R_G3MAC_IB_CSMA_FAIRNESS_LIMIT           =   0x010C,
    R_G3MAC_IB_TMR_TTL                       =   0x010D,
    R_G3MAC_IB_POS_TABLE_ENTRY_TTL           =   0x010E,
    R_G3MAC_IB_RCCOORD                       =   0x010F,
    R_G3MAC_IB_TONEMASK                      =   0x0110,
    R_G3MAC_IB_BEACON_RAND_WIN_LENGTH        =   0x0111,
    R_G3MAC_IB_A                             =   0x0112,
    R_G3MAC_IB_K                             =   0x0113,
    R_G3MAC_IB_MINCWATTEMPTS                 =   0x0114,
    R_G3MAC_IB_CENELEC_LEGACY_MODE           =   0x0115,
    R_G3MAC_IB_FCC_LEGACY_MODE               =   0x0116,
    R_G3MAC_IB_BROADCAST_MAX_CW_EANBLE       =   0x011E,
    R_G3MAC_IB_TRANSMIT_ATTEN                =   0x011F,
    R_G3MAC_IB_POS_TABLE                     =   0x0120,
    R_G3MAC_IB_PLC_DISABLE                   =   0x0121,

    R_G3MAC_IB_COHERENT_TRANSMISSION         =   0x0800,
    R_G3MAC_IB_NEIGHBOUR_TABLE_SIZE          =   0x0801,
    R_G3MAC_IB_DEVICE_TABLE_SIZE             =   0x0802,
    R_G3MAC_IB_VALID_NEITABLE_ENTRIES        =   0x0804,
    R_G3MAC_IB_VALID_DEVTABLE_ENTRIES        =   0x0805,
    R_G3MAC_IB_NEIGHBOUR_TABLE_BY_SHORT_ADDR =   0x0806,
    R_G3MAC_IB_KEY_VALIDATE                  =   0x0807,
    R_G3MAC_IB_EXTADDRESS                    =   0x0808,
    R_G3MAC_IB_SOFT_VERSION                  =   0x0809,
    R_G3MAC_IB_UNICAST_DATA_TX_TIMEOUT       =   0x080A,
    R_G3MAC_IB_BROADCAST_DATA_TX_TIMEOUT     =   0x080B,
    R_G3MAC_IB_BEACON_REQUEST_TX_TIMEOUT     =   0x080C,
    R_G3MAC_IB_TX_GAIN                       =   0x080D,
    R_G3MAC_IB_FRAME_CNT_IND_INTERVAL        =   0x080E,
    R_G3MAC_IB_DEVICE_TABLE_BY_SHORT_ADDR    =   0x080F,
    R_G3MAC_IB_NEIGHBOUR_INDEX_BY_SHORT_ADDR =   0x0810,
    R_G3MAC_IB_DEVICE_INDEX_BY_SHORT_ADDR    =   0x0811,
    R_G3MAC_IB_VALID_POSTABLE_ENTRIES        =   0x0812,
    R_G3MAC_IB_POS_TABLE_BY_SHORT_ADDR       =   0x0813,
    R_G3MAC_IB_TMR_IND_ENABLE                =   0x0814,
    R_G3MAC_IB_KEEP_MOD_RETRY_NUM            =   0x0815,
    R_G3MAC_IB_NEI_UPDATE_AFTER_RETRANSMIT   =   0x0816,
    R_G3MAC_IB_COMM_STATUS_IND_MASK          =   0x0817,
    R_G3MAC_IB_TMR_REQ_LEAD_TIME_BEF_TTL     =   0x0818,
    R_G3MAC_IB_MIN_NEITABLE_ENTRIES          =   0x0819,

    R_G3MAC_IB_OFFSET_SNR                    =   0x081A,
    R_G3MAC_IB_THRESH_CARRIER_NUM            =   0x081B,
    R_G3MAC_IB_THRESH_TONENUM_PER_MAP        =   0x081C,
    R_G3MAC_IB_THRESH_SNR_DIFFERENTIAL       =   0x081D,
    R_G3MAC_IB_THRESH_SNR_COHERENT           =   0x081E,

    R_G3MAC_IB_DISABLE_BEACON                =   0x0821,
    R_G3MAC_IB_DISABLE_TMRES                 =   0x0822,
    R_G3MAC_IB_REF_RSSI_DB                   =   0x0823,
    R_G3MAC_IB_DISABLE_TXGAIN_TMR            =   0x0824,
    R_G3MAC_IB_TOTAL_NEITABLE_ENTRIES        =   0x0825,
    R_G3MAC_IB_TOTAL_POSTABLE_ENTRIES        =   0x0826,

    R_G3MAC_IB_ENABLE_DEBUG                  =   0x0828,   /* add for CCTT221 */

    R_G3MAC_IB_POS_RECENT_ENTRY_TH           =   0x0840,
    R_G3MAC_IB_POS_RECENT_ENTRY              =   0x0841,
    R_G3MAC_IB_TRICKLE_MIN_LQI_VALUE         =   0x0842,

    R_G3MAC_IB_STATISTICS                    =   0x0850,

    /* LML Sap Pib */
    R_G3MAC_IB_PHY_TX_POWER                  =   0x8340,
    R_G3MAC_IB_PHY_TX_FILTER_SCALE           =   0x8341,
    R_G3MAC_IB_PHY_TX_DIGITAL_PREAMBLE_GAIN  =   0x8342,
    R_G3MAC_IB_PHY_TX_DIGITAL_GAIN           =   0x8343,
    R_G3MAC_IB_PHY_TXENB_POLARITY            =   0x8344,
    R_G3MAC_IB_PHY_TX_WAIT_TIME              =   0x8345,
    R_G3MAC_IB_PHY_TX_BREAK                  =   0x8346,
    R_G3MAC_IB_PHY_TX_ACK_GAIN               =   0x8347,
    R_G3MAC_IB_PHY_SATT_CTRL_DISABLE         =   0x8348,
    R_G3MAC_IB_PHY_AC_PHASE_OFFSET           =   0x8349,
    R_G3MAC_IB_PHY_STATISTICS                =   0x834A,
    R_G3MAC_IB_PHY_TXRX_DISABLE              =   0x834B,
    R_G3MAC_IB_PHY_ZCD_IND_POLARITY          =   0x834D,
    R_G3MAC_IB_PHY_TPGA_CAJ                  =   0x834F,
    R_G3MAC_IB_PHY_SATT_POLARITY             =   0x8350,
    
	R_G3MAC_IB_PHY_PREAMBLE_NUM              =   0x8445,
    R_G3MAC_IB_PHY_RX_GAIN_CUR               =   0x8452,
    R_G3MAC_IB_LML_LQI_ADJ_BND_HI            =   0x8453,
    R_G3MAC_IB_LML_LQI_ADJ_BND_LO            =   0x8454,
    R_G3MAC_IB_LML_LQI_ADJ_SLOPE_HI          =   0x8455,
    R_G3MAC_IB_LML_LQI_ADJ_SLOPE_LO          =   0x8456,

    R_G3MAC_IB_LML_EIFS                      =   0x840A,
#ifndef R_DEFINE_DISABLE_CCTT244
    R_G3MAC_IB_LML_PREAMBLE_NUM              =   0x8445,
#endif

} r_g3mac_ib_id_t;

/*===========================================================================*/
/* For MAC SAP IF ENUM                                                       */
/*===========================================================================*/

typedef enum
{
    /*  These are MAC statuses  */
    R_G3MAC_STATUS_SUCCESS                   =   0x00,

    R_G3MAC_STATUS_TX_TIMEOUT                =   0xC0,
    R_G3MAC_STATUS_COUNTER_ERROR             =   0xDB,
    R_G3MAC_STATUS_IMPROPER_KEY_TYPE         =   0xDC,
    R_G3MAC_STATUS_IMPROPER_SECURITY_LEVEL   =   0xDD,
    R_G3MAC_STATUS_UNSUPPORTED_LEGACY        =   0xDE,
    R_G3MAC_STATUS_UNSUPPORTED_SECURITY      =   0xDF,
    R_G3MAC_STATUS_CHANNEL_ACCESS_FAILURE    =   0xE1,
    R_G3MAC_STATUS_SECURITY_ERROR            =   0xE4,
    R_G3MAC_STATUS_FRAME_TOO_LONG            =   0xE5,
    R_G3MAC_STATUS_INVALID_HANDLE            =   0xE7,
    R_G3MAC_STATUS_INVALID_PARAMETER         =   0xE8,
    R_G3MAC_STATUS_NO_ACK                    =   0xE9,
    R_G3MAC_STATUS_NO_BEACON                 =   0xEA,
    R_G3MAC_STATUS_NO_DATA                   =   0xEB,
    R_G3MAC_STATUS_NO_SHORT_ADDRESS          =   0xEC,
    R_G3MAC_STATUS_OUT_OF_CAP                =   0xED,
    R_G3MAC_STATUS_ALTERNATE_PANID_DETECTION =   0xEE,
    R_G3MAC_STATUS_UNAVAILABLE_KEY           =   0xF3,
    R_G3MAC_STATUS_UNSUPPORTED_ATTRIBUTE     =   0xF4,
    R_G3MAC_STATUS_INVALID_ADDRESS           =   0xF5,
    R_G3MAC_STATUS_INVALID_INDEX             =   0xF9,
    R_G3MAC_STATUS_LIMIT_REACHED             =   0xFA,
    R_G3MAC_STATUS_READ_ONLY                 =   0xFB,
    R_G3MAC_STATUS_SCAN_IN_PROGRESS          =   0xFC,

    /*  These are MAC implementation specific statuses  */
    R_G3MAC_STATUS_INVALID_STATE             =   0xFD,
	R_G3MAC_STATUS_TXRX_DISABLED             =   0xFE,
    R_G3MAC_STATUS_NO_RESPONSE               =   0xFF,

    /*  LML */
    R_G3MAC_STATUS_LML_NO_ACK                =   0xC1,
    R_G3MAC_STATUS_LML_CSMA_FAILURE          =   0xC2,
    R_G3MAC_STATUS_LML_BUFFER_FULL           =   0xC4,
    R_G3MAC_STATUS_LML_INVALID_REQ           =   0xC5,
    R_G3MAC_STATUS_LML_TOO_FEW_SC            =   0xC6,
    R_G3MAC_STATUS_LML_ABORT_ERROR           =   0xCD,
    R_G3MAC_STATUS_LML_NO_RESPONSE           =   0xCE,
    R_G3MAC_STATUS_LML_FAILURE               =   0xCF,

    /*  IF  */
    R_G3MAC_STATUS_IF_FATAL_ERROR            =   0xB0,
    R_G3MAC_STATUS_IF_TIMEOUT,
    R_G3MAC_STATUS_IF_QUEUE_FULL,
    R_G3MAC_STATUS_IF_INVALID_STATE,

    R_G3MAC_STATUS_INSUFFICIENT_MEMSIZE =   0xA0,
    R_G3MAC_STATUS_IF_NO_RESPONSE       =   0xA1,

} r_g3mac_status_t;


typedef enum
{
    R_G3MAC_TX_NO_ACK = 0x0,
    R_G3MAC_TX_ACK,
    R_G3MAC_TX_END,
} r_g3mac_tx_options_t;

typedef enum
{
    R_G3MAC_QOS_NORMAL =   0x00,               /*!< Normal quality of service */
    R_G3MAC_QOS_HIGH   =   0x01,               /*!< High quality of service */
    R_G3MAC_QOS_END
} r_g3mac_qos_t;

typedef enum
{
    R_G3MAC_ADDR_MODE_NO_ADDRESS =   0x00,     /*!< No address */
    R_G3MAC_ADDR_MODE_SHORT      =   0x02,     /*!< 16 bit short address */
    R_G3MAC_ADDR_MODE_EXTENDED   =   0x03      /*!< 64 bit extended address */
} r_g3mac_address_mode_t;

typedef enum
{
    R_G3MAC_MOD_TYPE_ROBO  =   0x00u,          /*!<< Robust mode */
    R_G3MAC_MOD_TYPE_BPSK  =   0x01u,          /*!<< (D)BPSK */
    R_G3MAC_MOD_TYPE_QPSK  =   0x02u,          /*!<< (D)QPSK */
    R_G3MAC_MOD_TYPE_8PSK  =   0x03u,          /*!<< (D)8PSK */
    R_G3MAC_MOD_TYPE_16QAM =   0x04u,          /*!<< 16-QAM */
    R_G3MAC_MOD_TYPE_END
} r_g3mac_mod_type_t;

typedef enum
{
    R_G3MAC_MOD_SCHEME_DIFFERENTIAL =   0x00u, /*!<< Differential modulation scheme */
    R_G3MAC_MOD_SCHEME_COHERENT     =   0x01u, /*!<< Coherent modulation scheme */
    R_G3MAC_MOD_SCHEME_END,
} r_g3mac_mod_scheme_t;


typedef enum
{
    R_G3MAC_COMMIND_MASK_DB =   0x00u,         /*!<< R_G3MAC_STATUS_COUNTER_ERROR */
    R_G3MAC_COMMIND_MASK_E4,                   /*!<< R_G3MAC_STATUS_SECURITY_ERROR */
    R_G3MAC_COMMIND_MASK_EE,                   /*!<< R_G3MAC_STATUS_ALTERNATE_PANID_DETECTION */
    R_G3MAC_COMMIND_MASK_F3,                   /*!<< R_G3MAC_STATUS_UNAVAILABLE_KEY */
} r_g3mac_comm_sts_ind_mask_bit_t;



typedef uint8_t r_g3mac_cb_status_t;

/*  Neighbour    Table   */
typedef struct
{
    uint8_t rsv                     :   1;
    uint8_t posTrace                :   1;
    uint8_t ntTrace                 :   1;
    uint8_t txRes                   :   1;
    uint8_t txGain                  :   4;     /*[MSB] */
} r_g3mac_neighbor_sta_bit_t;

typedef union
{
    uint8_t byte;
    r_g3mac_neighbor_sta_bit_t bit;
} r_g3mac_neighbor_sta_t;


typedef struct
{
    uint8_t modType         :   3;             /*[LSB] */
    uint8_t rsv             :   1;
    uint8_t phaseDiff       :   3;
    uint8_t pms             :   1;             /*[MSB] */
} r_g3mac_neighbor_mod_bit_t;

typedef union
{
    uint8_t byte;
    r_g3mac_neighbor_mod_bit_t bit;
} r_g3mac_neighbor_mod_t;

typedef struct
{
    r_g3mac_neighbor_sta_t sta;
    uint8_t                shortAddress[2];
    uint8_t                tonemap[3];
    r_g3mac_neighbor_mod_t mod;
    uint8_t                tmrValidTime[4];
    uint8_t                revLqi;
    uint8_t                txCoeff[6];
    uint8_t                fwdLqi;
} r_g3mac_mac_neighbor_table_t;

typedef struct
{
    uint8_t shortAddress[2];
    uint8_t frameCounter[4];
    uint8_t updatedTime[4];
} r_g3mac_device_table_t;

typedef struct
{
    uint16_t panId;
    uint8_t  address[2];
    uint8_t  linkQuality;
    uint8_t  mediaType;
    uint16_t rcCoord;
} r_g3mac_pan_descriptor_t;

typedef struct
{
    uint8_t macVersion[2];
    uint8_t dspVersion[2];
} r_g3mac_soft_version_t;

typedef struct
{
    uint8_t shortAddress[2];
    uint8_t fwdLqi;
    uint8_t posValidTime[4];
    r_g3mac_neighbor_sta_t sta;
} r_g3mac_mac_pos_table_t;


/*===========================================================================*/
/* For MAC SAP IF STRUCT                                                     */
/*===========================================================================*/

/* MCPS-DATA */
typedef struct
{
    uint8_t   srcAddrMode;
    uint8_t   dstAddrMode;
    uint16_t  dstPanId;
    uint8_t   dstAddr[8];
    uint16_t  msduLength;
    uint8_t * pMsdu;
    uint8_t   msduHandle;
    uint8_t   txOptions;
    uint8_t   securityLevel;
    uint8_t   keyIndex;
    uint8_t   qualityOfService;
} r_g3mac_mcps_data_req_t;

typedef struct
{
    uint8_t             msduHandle;
    r_g3mac_cb_status_t status;
    uint8_t             rsv;
} r_g3mac_mcps_data_cnf_t;

typedef struct
{
    uint8_t                srcAddrMode;
    uint16_t               srcPanId;
    uint8_t                srcAddr[8];
    uint8_t                dstAddrMode;
    uint16_t               dstPanId;
    uint8_t                dstAddr[8];
    uint16_t               msduLength;
    uint8_t *              pMsdu;
    uint8_t                msduLinkQuality;
    uint8_t                dsn;
    uint8_t                securityLevel;
    uint8_t                keyIndex;
    uint8_t                qualityOfService;
    r_g3mac_neighbor_mod_t estimatedMod;
    uint8_t                estimatedTonemap[3];
    r_g3mac_neighbor_mod_t receivedMod;
    uint8_t                receivedTonemap[3];
    uint8_t                RSSIdB;
    uint8_t                rsv;
} r_g3mac_mcps_data_ind_t;

/* MLME-RESET  */
typedef struct
{
    uint8_t setDefaultPIB;
} r_g3mac_mlme_reset_req_t;

typedef struct
{
    r_g3mac_cb_status_t status;
} r_g3mac_mlme_reset_cnf_t;

/* MLME-GET */
typedef struct
{
    uint16_t pibAttributeId;
    uint16_t pibAttributeIndex;
} r_g3mac_mlme_get_req_t;

typedef struct
{
    r_g3mac_cb_status_t status;
    uint16_t            pibAttributeId;
    uint16_t            pibAttributeIndex;
    uint8_t             pibAttributeValue[R_G3MAC_MAX_IB_SIZE];
} r_g3mac_mlme_get_cnf_t;

/* MLME-SET */
typedef struct
{
    uint16_t  pibAttributeId;
    uint16_t  pibAttributeIndex;
    uint8_t * pibAttributeValue;
} r_g3mac_mlme_set_req_t;

typedef struct
{
    r_g3mac_cb_status_t status;
    uint16_t            pibAttributeId;
    uint16_t            pibAttributeIndex;
} r_g3mac_mlme_set_cnf_t;

/* MLME-SCAN */
typedef struct
{
    uint8_t scanDuration;
} r_g3mac_mlme_scan_req_t;

typedef struct
{
    r_g3mac_cb_status_t        status;
} r_g3mac_mlme_scan_cnf_t;


/* MLME-BEACON-NOTIFY */
typedef struct
{
    uint8_t phaseDiff;
    r_g3mac_pan_descriptor_t panDescriptor;
} r_g3mac_mlme_bcn_notify_ind_t;


/* MLME-START */
typedef struct
{
    uint16_t panId;
} r_g3mac_mlme_start_req_t;

typedef struct
{
    r_g3mac_cb_status_t status;
} r_g3mac_mlme_start_cnf_t;

/*  MLME-COMM-STATUS */
typedef struct
{
    uint16_t            panId;
    uint8_t             srcAddrMode;
    uint8_t             srcAddr[8];
    uint8_t             dstAddrMode;
    uint8_t             dstAddr[8];
    r_g3mac_cb_status_t status;
    uint8_t             securityLevel;
    uint8_t             keyIndex;
} r_g3mac_mlme_comm_status_ind_t;

/* MLME-FRAMECOUNT */
typedef struct
{
    uint32_t frameCounter;
} r_g3mac_mlme_framecount_ind_t;

/* MLME-TMR-RECEIVE */
typedef struct
{
    uint8_t srcAddr[2];
} r_g3mac_mlme_tmr_receive_ind_t;

/* MLME-TMR-TRANSMIT */
typedef struct
{
    uint8_t dstAddrMode;
    uint8_t dstAddr[8];
    uint8_t pms;
    uint8_t mod;
} r_g3mac_mlme_tmr_transmit_ind_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#endif  /*  R_G3MAC_SAP_H   */

