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
 * File Name   : r_demo_app.h
 *    @version
 *        $Rev: 11089 $
 *    @last editor
 *        $Author: a5089763 $
 *    @date
 *        $Date:: 2022-07-12 11:09:31 +0900#$
 * Description :
 ******************************************************************************/

#ifndef R_DEMO_APP_H
#define R_DEMO_APP_H

#if defined(R_DEFINE_OPEN_RX)
#include "r_os_wrapper.h"
#else
#include "r_config.h"
#endif

//---------------------------------
#include "../../cpx3_fw_new_v1/cpx3_switch.h"
#include "r_g3_sap.h"

r_result_t CPX3_ADP_Init(void);
void CPX3_AppResetDevice(void);
void CPX3_AppWaitInsert(void);
r_result_t CPX3_AppJointNewtork(void);
void CPX3_AppSetIpContent(void);

typedef enum
{
    CPX3_ADP_SET_CONFIG = 0,
    CPX3_ADPM_RESET,
    CPX3_ADPM_GET_WRAP,
    /***************set ipcontent******************** */
    CPX3_TX_GAIN,                  /* Set tx gain. */
    CPX3_TX_FILTER_SCALE,          /* Set tx filter scale. */
    CPX3_TX_DIGITAL_PREAMBLE_GAIN, /* Set tx digital preamble gain. */

    CPX3_TX_DIGITAL_GAIN, /* Set tx digital gain. */

    CPX3_TX_ENABLE_POLARITY, /* Set tx enable polarity. */

    CPX3_TX_WAIT_TIME,      /* Set tx wait time. */
    CPX3_TX_WAIT_TIME_2,    /* Set tx wait time_ 2. */
    CPX3_SATT_CTRL_DISABLE, /* Set SATT Ctrl disable. */

    /************** MAC PIB Settings **************/
    CPX3_SET_FRAME_COUNTER_IND,                /* Set frame counter indication interval. */
    CPX3_SET_TONE_MAP_IND,                     /* Set tone map indication. */
    CPX3_SET_OFFSET_SNR_FOR_ADAPTIVE_TONE_MAP, /* Set offset snr for adaptive tone map. */

    CPX3_GET_TONE_MASK, /* Get tone mask. */

    CPX3_HANDLE_GET_TONE_MASK,

    CPX3_SET_TONE_MASK, /* Set tone mask. */
    CPX3_SET_MAX_BE,    /* Set Max Be */
    CPX3_SET_MIN_BE,    /* Set Min Be */
    CPX3_SET_SET_MAX_A, /* Set max A */

    CPX3_SET_MAC_BEACONE_RAND_WL,            /* Set macBeaconRandomizationWindowLength */
    CPX3_SET_MAC_BROADCASET_DATA_TX_TIMEOUT, /* Set macBroadcastDataTxTimeout */
    CPX3_SET_MAC_RETRY_MAX_CW,               /* Set macRetryMaxCw */
                                             /************** ADP IB Settings **************/

    CPX3_SET_DEVICE_TYPE,                    /* Set the device type. */
    CPX3_ACTIVATE_ADPM_ROUTE_UPD_IND,        /* Activate ADPM-ROUTE-UPDATE.indication. */
    CPX3_ACTIVATE_ADPM_RREP_IND,             /* Activate ADPM-RREP.indication. */
    CPX3_SET_GROUP_TABLE_ENTERIES,           /* Set group table entries. */
    CPX3_SET_ADP_DEFAULT_COORD_ROUTE_ENABLE, /* Set adpDefaultCoordRouteEnabled for CCTT187. */
    CPX3_VAL_APD_MAX_HOPS,                   // R_VAL_ADP_MAX_HOPS
    CPX3_VAL_APD_LOW_LQI,                    // R_VAL_ADP_LOW_LQI
    CPX3_VAL_APD_HIGH_LQI,                   // R_VAL_ADP_HIGH_LQI
    CPX3_VAL_APD_WEAK_LQI,                   // R_VAL_ADP_WEAK_LQI
    CPX3_VAL_APD_KQ,                         // R_VAL_ADP_KQ
    CPX3_VAL_APD_KRT,                        // R_VAL_ADP_KRT
    CPX3_VAL_APD_MAX_JOIN_WAIT_TIME,         // R_VAL_ADP_MAX_JOIN_WAIT_TIME
    CPX3_VAL_APD_NET_TRANSVERAL_TIME,        // R_VAL_ADP_NET_TRAVERSAL_TIME
    CPX3_VAL_APD_PREP_WAIT,                  // R_VAL_ADP_RREP_WAIT

    //////////////////////////////////////////
    CPX3_SET_IB_FRAME_COUNTER,
    CPX3_SET_IB_LOAD_SEQ_NUMBER,

    //////////////////////////////////////////
    // join network

    CPX3_GET_R_ADP_IB_METRIC_TYPE,
    CPX3_HANDLE_GET_R_ADP_IB_METRIC_TYPE,

    CPX3_GET_R_ADP_IB_LOW_LQI_VALUE,
    CPX3_HANDLE_GET_R_ADP_IB_LOW_LQI_VALUE,

    CPX3_GET_R_ADP_IB_HIGH_LQI_VALUE,
    CPX3_HANDLE_GET_R_ADP_IB_HIGH_LQI_VALUE,

    CPX3_GET_R_ADP_IB_KQ,
    CPX3_HANDLE_GET_R_ADP_IB_KQ,

    CPX3_GET_R_ADP_IB_KH,
    CPX3_HANDLE_GET_R_ADP_IB_KH

} app_reset_state;

//---------------------------------

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_CAP_ICMP_IDENTIFIER (0x0102u)
#define R_CAP_SYNCWORD (0xF8726FBAul)
#define R_CAP_CAPSET_MAX_LEN (32u)

#define R_DEMO_APP_MODE_NORMAL (0u)
#define R_DEMO_APP_MODE_DEMO (1u)
#define R_DEMO_APP_MODE_CERT (2u)

#define R_DEMO_APP_UART_BOOT (0u)
#define R_DEMO_APP_SROM_BOOT (1u)

#define R_DEMO_APP_STRING_BUFFER_SIZE (1u)
#define R_DEMO_APP_NSDU_BUFFER_SIZE (1280u)
#define R_DEMO_APP_MSDU_PRINT_MAXLEN (128u)
#define R_DEMO_APP_STATS_BUFF_MAXLEN (1024u)
#define R_DEMO_APP_PAN_MAXNUM (8u)

#define R_DEMO_G3_USE_PRIMARY_CH (0u)   /*!< CPX3 G3 FW can use dual adp, but wrapper fixed single ch */
#define R_DEMO_G3_USE_SECONDARY_CH (1u) /*!< CPX3 G3 FW can use dual adp, but wrapper fixed single ch */

#define R_DEMO_G3_STATUS_NOT_SET (0x01u) /*!< Default value for confirm structures */
#define R_DEMO_G3_STATUS_FAILED (0x02u)  /*!< Error Status when api call return failure */

#define R_APP_WAIT_CNF_TIMEOUT (300u)                         /*!< Timeout for wait confirm in seconds */
#define R_APP_WAIT_SROM_CMN_CNF_TIMEOUT (3000u)               /*!< Timeout for wait confirm in milliseconds (SROM access) */
#define R_APP_WAIT_SROM_WRITE_CNF_TIMEOUT (12u)               /*!< Timeout for wait confirm in milliseconds (SROM write timeout:12[ms]) */
#define R_APP_WAIT_SROM_ERASE_CNF_TIMEOUT (24000u)            /*!< Timeout for wait confirm in milliseconds (SROM 64KByte erase timeout:6[s], Max Erase Size 256KByte) */
#define R_APP_DUMP_INTERVAL_VERBOSE_ON (500u)                 /*!< Dump indication interval time(ms) */
#define R_APP_DUMP_INTERVAL_VERBOSE_OFF (100u)                /*!< Dump indication interval time(ms) */
#define R_APP_DUMP_SEGMENT_BUFF_SIZE (1024u)                  /*!< Dump indication interval time(ms) */
#define R_HANDLE_APP_DONT_CARE (0x00u)                        /*!< R_HANDLE_APP_DONT_CARE */
#define R_HANDLE_APP_FIRST (0x01u)                            /*!< R_HANDLE_APP_FIRST */
#define R_HANDLE_APP_CMN_REQUEST (0x01u)                      /*!< R_HANDLE_APP_CMN_REQUEST */
#define R_HANDLE_APP_LAST (0x02u)                             /*!< R_HANDLE_APP_LAST */
#define R_MAX_USED_APP_TIMER_HANDLES (R_HANDLE_APP_LAST + 1u) /*!< + 1 due to starting from 0 */

/******************************************************************************
Typedef definitions
******************************************************************************/
/*!
    \brief Type of Status Flag
 */
#define R_FLAG_GET_INFO (0x00000001ul)
#define R_FLAG_CLEAR_INFO (0x00000002ul)
#define R_FLAG_GET_CONFIG (0x00000004ul)
#define R_FLAG_SET_CONFIG (0x00000008ul)
#define R_FLAG_MCPS_DATA (0x00000010ul)
#define R_FLAG_MLME_RESET (0x00000020ul)
#define R_FLAG_MLME_SET (0x00000040ul)
#define R_FLAG_MLME_GET (0x00000080ul)
#define R_FLAG_MLME_SCAN (0x00000100ul)
#define R_FLAG_MLME_START (0x00000200ul)
#define R_FLAG_ADPM_LBP (0x00000400ul)
#define R_FLAG_ADPM_LEAVE (0x00000800ul)
#define R_FLAG_ADPM_JOIN (0x00001000ul)
#define R_FLAG_ADPM_DISCOVERY (0x00002000ul)
#define R_FLAG_ADPD_DATA (0x00004000ul)
#define R_FLAG_ADPM_START (0x00008000ul)
#define R_FLAG_ADPM_SET (0x00010000ul)
#define R_FLAG_ADPM_GET (0x00020000ul)
#define R_FLAG_ADPM_ROUTE (0x00040000ul)
#define R_FLAG_ADPM_PATH (0x00080000ul)
#define R_FLAG_ADPM_RESET (0x00100000ul)
#define R_FLAG_EAPM_START (0x00200000ul)
#define R_FLAG_EAPM_GET (0x00400000ul)
#define R_FLAG_EAPM_SET (0x00800000ul)
#define R_FLAG_EAPM_NETWORK (0x01000000ul)
#define R_FLAG_EAPM_SETCLIENTINFO (0x02000000ul)
#define R_FLAG_EAPM_RESET (0x04000000ul)
#define R_FLAG_DUMP (0x20000000ul)
#define R_FLAG_DUMP_ABORT (0x40000000ul)
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
// TODO interim value
#define R_FLAG_RF_MLME_SET (0x00000004ul)
#define R_FLAG_RF_MLME_GET (0x00000008ul)
#define R_FLAG_RF_MLME_RESET (0x00400000ul)
#define R_FLAG_RF_MCPS_DATA (0x00800000ul)
#endif

/*!
    \enum r_modem_platform_type_t
    \brief Type of CPX modem platform
 */
typedef enum
{
    R_PLATFORM_TYPE_CPX1 = 0x01, /*!< R_PLATFORM_TYPE_CPX1 */
    R_PLATFORM_TYPE_CPX2 = 0x02, /*!< R_PLATFORM_TYPE_CPX2 */
    R_PLATFORM_TYPE_CPX3 = 0x03  /*!< R_PLATFORM_TYPE_CPX3 */

} r_modem_platform_type_t;

/*!
    \enum r_modem_board_type_t
    \brief Type of CPX modem board
 */
typedef enum
{
    R_BOARD_TYPE_EU_OFDM = 0x00, /*!< R_BOARD_TYPE_EU_OFDM */
    R_BOARD_TYPE_G_CPX = 0x01,   /*!< R_BOARD_TYPE_G_CPX */
    R_BOARD_TYPE_G_CPX3 = 0x02   /*!< R_BOARD_TYPE_G_CPX */

} r_modem_board_type_t;

/*!
    \struct r_cap_icmp_header_t
    \brief ICMP CAP header
 */
typedef struct
{
    uint8_t type;
    uint8_t code;
    uint8_t checksum[2];
    uint8_t identifier[2];
    uint8_t sequenceNumber[2];

} r_cap_icmp_header_t;

/*!
    \struct r_cap_command_header_t
    \brief CAP command header
 */
typedef struct
{
    uint8_t Syncword[4];
    uint8_t CheckSum[2]; /* include command */
    uint8_t AckReq;
    uint8_t Type;
    uint8_t CmdID[2];
    uint8_t SequenceNumber[2];
    uint8_t Reserve[2];
    uint8_t length[2]; /* byte length after here (payload length only) */

} r_cap_command_header_t;

/*!
    \struct r_cap_cmd_capm_set_req_t
    \brief CAP set request
 */
typedef struct
{
    uint8_t type;
    uint8_t id[2];
    uint8_t index[2];
    uint8_t payload[R_CAP_CAPSET_MAX_LEN];

} r_cap_cmd_capm_set_req_t;

/*!
    \struct r_cap_cmd_capm_get_req_t
    \brief CAP get request
 */
typedef struct
{
    uint8_t type;
    uint8_t id[2];
    uint8_t index[2];

} r_cap_cmd_capm_get_req_t;

/*!
    \struct r_cap_cmd_capm_set_cnf_t
    \brief CAP set confirm
 */
typedef struct
{
    uint8_t Status[4];
    uint8_t type;
    uint8_t id[2];
    uint8_t index[2];

} r_cap_cmd_capm_set_cnf_t;

/*!
    \struct r_cap_cmd_capm_get_cnf_t
    \brief CAP get confirm
 */
typedef struct
{
    uint8_t Status[4];
    uint8_t type;
    uint8_t id[2];
    uint8_t index[2];
    uint8_t payload[R_CAP_CAPSET_MAX_LEN];

} r_cap_cmd_capm_get_cnf_t;

/*!
    \enum r_cap_type_id_t
    \brief CAP type ID
 */
typedef enum
{
    R_CAP_TYPE_ID_GET_SET_ADP = 0x02, /*!< R_CAP_TYPE_ID_GET_SET_ADP */
    R_CAP_TYPE_ID_GET_SET_MAC = 0x03, /*!< R_CAP_TYPE_ID_GET_SET_MAC */
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    R_CAP_TYPE_ID_GET_SET_RFMAC = 0x0B, /*!< R_CAP_TYPE_ID_GET_SET_RFMAC */
#endif
    R_CAP_TYPE_ID_GET_SET_STATS = 0x80, /*!< R_CAP_TYPE_ID_GET_SET_STATS */
    R_CAP_TYPE_ID_GET_SET_LOGS = 0x81,  /*!< R_CAP_TYPE_ID_GET_SET_LOGS */
    R_CAP_TYPE_ID_APP_CMD = 0x82        /*!< R_CAP_TYPE_ID_APP_CMD */

} r_cap_type_id_t;

/*!
    \enum r_cap_type_id_t
    \brief CAP type ID
 */
typedef enum
{
    R_CAP_APP_CMD_TYPE_ID_SET_TIME,   /*!< Set system time */
    R_CAP_APP_CMD_TYPE_ID_START_DUMP, /*!< Isseu Dump request */

} r_cap_app_cmd_type_id_t;

/*!
\enum r_cap_type_stats_index_t
\brief CAP statistices type
*/
typedef enum
{
    R_CAP_STATS_INDEX_MAC_SND = 0x00,
    R_CAP_STATS_INDEX_MAC_MOD,
    R_CAP_STATS_INDEX_MAC_CSMA1,
    R_CAP_STATS_INDEX_MAC_CSMA2,
} r_cap_type_stats_index_t;

/*!
    \enum r_cap_type_id_t
    \brief CAP type ID
 */
typedef enum
{
    CAP_SUCCESS = 0x00,
    CAP_INVALID_REQUEST = 0x01,
    CAP_UNKNOWN_DEVICE = 0x02,
    CAP_FAILURE = 0x80,
    CAP_NON_SUPPORTED_ATTRIBUTE = 0x90,
    CAP_INVALID_INDEX = 0x91,
    CAP_OUT_OF_RANGE = 0x92,
    CAP_READ_ONLY = 0x93

} r_cap_sap_status_t;

typedef enum
{
    R_DEMO_MODE_CUI = 0,
    R_DEMO_MODE_AUTO,
    R_DEMO_MODE_CERT,
} r_demo_operation_mode_t;

#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
typedef union
{
    uint8_t byte;
    struct
    {
        uint8_t macOperatingMode_RF : 4;
        uint8_t macFrequencyBand_RF : 4;
    };
} r_demo_rfband_t;
#endif

typedef struct
{
    r_modem_platform_type_t modemPlatformType;
    r_modem_board_type_t modemBoardType;
    r_g3_bandplan_t bandPlan;
    r_adp_device_type_t devType;
    uint8_t extIdFlg;
    uint8_t secyrityF2;
    r_boolean_t verboseEnabled;
    r_boolean_t macPromiscuousEnabled;
    r_demo_operation_mode_t appMode;
    r_g3mac_qos_t qualityOfService;
    r_boolean_t discoverRoute;
    uint64_t deviceEUI64; /*!< EUI64 of the device */
    uint16_t panId;
    uint16_t coordShortAddress;
    uint8_t pskKey[16];     /*!< Pre-shared key */
    uint8_t gmk0[16];       /*!< GMK0 for coordinator */
    uint8_t gmk1[16];       /*!< GMK1 for coordinator */
    uint8_t activeKeyIndex; /*!< active key index for coordinator */
    uint8_t wait[4];
    uint8_t tonemask[9];
    r_g3_config_extid_t extId;
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    uint8_t mediaType; /* 0:PLC only 1:RF only 2:Hybrid */
    r_demo_rfband_t bandPlanRf;
    r_boolean_t plcDisabled;
    r_boolean_t rfDisabled;
#endif
} r_demo_config_t;

typedef struct
{
    uint16_t panId;        /*!< Assigned PAN ID */
    uint16_t shortAddress; /*!< Assigned short address */
    uint8_t nsduHandle;    /*!< NSDU handle for data transmissions */
} r_demo_entity_t;

typedef struct
{
    uint8_t panCount;
    r_adp_pan_descriptor_t pan[R_DEMO_APP_PAN_MAXNUM];
} r_demo_paninfo_t;

typedef struct
{
    uint8_t getStringBuffer[R_DEMO_APP_STRING_BUFFER_SIZE]; /*!< String buffer used for console interaction */
    uint8_t Nsdu[R_DEMO_APP_NSDU_BUFFER_SIZE];              /*!< Buffer for IPv6 data frame */
    r_demo_paninfo_t panInfo;
} r_demo_buff_t;

typedef struct
{
    uint8_t g3mode;
    uint8_t startMode;
    uint8_t extIdFlg;
} r_demo_mode_ch_t;

typedef struct
{
    uint8_t bandPlan;
    uint8_t tonemask[9];
    r_demo_mode_ch_t ch[2];
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    r_demo_rfband_t bandPlanRf;
#else
    uint8_t rsv;
#endif
} r_demo_mode_t;

typedef struct
{
    /*===========================================================================*/
    /*    G3CTRL callbacks                                                       */
    /*===========================================================================*/

    r_g3_set_config_cnf_t setConfig;
    r_g3_get_config_cnf_t getConfig;
    r_g3_get_info_cnf_t getInfo;
    r_g3_clear_info_cnf_t clrInfo;
    r_g3_dump_cnf_t dumpCnf;
    r_g3_dump_ind_t dumpInd;
    r_g3_event_ind_t eventInd;
    uint32_t cbBuffU32[R_DEMO_APP_STATS_BUFF_MAXLEN / 4];

    /*===========================================================================*/
    /*    G3MAC callbacks                                                        */
    /*===========================================================================*/
    r_g3mac_mcps_data_cnf_t mcpsDataCnf;
    r_g3mac_mlme_reset_cnf_t mlmeResetCnf;
    r_g3mac_mlme_get_cnf_t mlmeGetCnf;
    r_g3mac_mlme_set_cnf_t mlmeSetCnf;
    r_g3mac_mlme_scan_cnf_t mlmeScanCnf;
    r_g3mac_mlme_start_cnf_t mlmeStartCnf;

    /*    r_g3mac_mcps_data_ind_t             mcpsDataInd; */
    r_g3mac_mlme_bcn_notify_ind_t mlmeBeaconNotifyInd;
    r_g3mac_mlme_comm_status_ind_t mlmeCommStatusInd;
    r_g3mac_mlme_framecount_ind_t mlmeFrameCountInd;
    r_g3mac_mlme_tmr_receive_ind_t mlmeTmrReceivInd;
    r_g3mac_mlme_tmr_transmit_ind_t mlmeTmrTransmitInd;

    /*===========================================================================*/
    /*    ADP callbacks                                                        */
    /*===========================================================================*/
    r_adp_adpd_data_cnf_t adpdDataCnf;
    r_adp_adpm_reset_cnf_t adpmResetCnf;
    r_adp_adpm_discovery_cnf_t adpmDiscoveryCnf;
    r_adp_adpm_network_start_cnf_t adpmNetworkStartCnf;
    r_adp_adpm_network_join_cnf_t adpmNetworkJoinCnf;
    r_adp_adpm_network_leave_cnf_t adpmNetworkLeaveCnf;
    r_adp_adpm_get_cnf_t adpmGetCnf;
    r_adp_adpm_set_cnf_t adpmSetCnf;
    r_adp_adpm_route_disc_cnf_t adpmRouteDiscoveryCnf;
    r_adp_adpm_path_discovery_cnf_t adpmPathDiscoveryCnf;
    r_adp_adpm_lbp_cnf_t adpmLbpCnf;

    /*    r_adp_adpd_data_ind_t               adpdDataInd; */
    r_adp_adpm_network_status_ind_t adpmNetworkStatusInd;
    /*    r_adp_adpm_path_discovery_ind_t adpmPathDiscoveryInd; */
    r_adp_adpm_lbp_ind_t adpmLbpInd;

    /*    r_adp_adpm_buffer_ind_t             adpmBufferInd; */
    /*    r_adp_adpm_key_state_ind_t      adpmKeyStateInd; */
    r_adp_adpm_route_error_ind_t adpmRouteErrorInd;
    r_adp_adpm_eap_key_ind_t adpmEapKeyInd;
    r_adp_adpm_framecounter_ind_t adpmFrameCounterInd;
    r_adp_adpm_route_update_ind_t adpmRouteUpdateInd;
    r_adp_adpm_rrep_ind_t adpmRrepInd;

    /*===========================================================================*/
    /*    EAP callbacks                                                        */
    /*===========================================================================*/
    r_eap_eapm_reset_cnf_t eapmResetCnf;
    r_eap_eapm_start_cnf_t eapmStartCnf;
    r_eap_eapm_get_cnf_t eapmGetCnf;
    r_eap_eapm_set_cnf_t eapmSetCnf;
    r_eap_eapm_network_cnf_t eapmNetworkCnf;
    r_eap_eapm_set_clientinfo_cnf_t eapmSetClientInfoCnf;

    /*    r_eap_eapm_network_join_ind_t       eapmNetworkJoinInd; */
    r_eap_eapm_network_leave_ind_t eapmNetworkLeaveInd;

    /*    r_eap_eapm_newdevice_ind_t          eapmNewDeviceInd; */

#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    /*===========================================================================*/
    /*    RF callbacks                                                        */
    /*===========================================================================*/
    r_g3rfmac_mcps_data_cnf_t rf_mcps_data_cnf;
    r_g3rfmac_mlme_reset_cnf_t rf_mlme_reset_cnf;
    r_g3rfmac_mlme_get_cnf_t rf_mlme_get_cnf;
    r_g3rfmac_mlme_set_cnf_t rf_mlme_set_cnf;
    r_g3rfmac_mlme_scan_cnf_t rf_mlme_scan_cnf;
    r_g3rfmac_mlme_start_cnf_t rf_mlme_start_cnf;
    r_g3rfmac_mcps_data_ind_t rf_mcps_data_ind;
    r_g3rfmac_mlme_bcn_notify_ind_t rf_mlme_beacon_notify_ind;
    r_g3rfmac_mlme_comm_status_ind_t rf_mlme_comm_status_ind;
    r_g3rfmac_mlme_framecount_ind_t rf_mlme_frame_count_ind;
    r_g3rfmac_mlme_warning_ind_t rf_mlme_warning_ind;
#endif

    uint32_t statusFlags;

} r_demo_g3_cb_str_t;

/******************************************************************************
Functions prototype
******************************************************************************/

/***********************************************************************
 * Function Name     : R_DEMO_AppHandleMacTmrRcvInd
 * Description       : Handling function for MAC TMR Receive indications
 * Argument          : None
 * Return Value      : None
 ***********************************************************************/
/*!
   \fn void R_DEMO_AppHandleMacTmrRcvInd(const r_g3mac_mlme_tmr_receive_ind_t* ind);
   \brief Handling function for MAC TMR Receive indications
 */
void R_DEMO_AppHandleMacTmrRcvInd(const r_g3mac_mlme_tmr_receive_ind_t *ind);

/***********************************************************************
 * Function Name     : R_DEMO_AppHandleMacTmrTransmitInd
 * Description       : Handling function for MAC TMR Transmit indications
 * Argument          : None
 * Return Value      : None
 ***********************************************************************/
/*!
   \fn void R_DEMO_AppHandleMacTmrTransmitInd(const r_g3mac_mlme_tmr_transmit_ind_t* ind);
   \brief Handling function for MAC TMR Transmit indications
 */
void R_DEMO_AppHandleMacTmrTransmitInd(const r_g3mac_mlme_tmr_transmit_ind_t *ind);

/***********************************************************************
 * Function Name     : R_DEMO_AppMainMenu
 * Description       : Demo application main menu
 * Argument          : None
 * Return Value      : None
 ***********************************************************************/
/*!
   \fn void R_DEMO_AppMainMenu(void);
   \brief Demo application main menu
 */
void R_DEMO_AppMainMenu(void);

/***********************************************************************
 * Function Name     : R_DEMO_AppHandleDataIndication
 * Description       : Handling function for data indications
 * Argument          : ind : Pointer to indication structure
 * Return Value      : None
 ***********************************************************************/
/*!
   \fn void R_DEMO_AppHandleDataIndication(const r_adp_adpd_data_ind_t* ind);
   \brief Handling function for data indications
 */
// void R_DEMO_AppHandleDataIndication (const r_adp_adpd_data_ind_t * ind);

/***********************************************************************
 * Function Name     : R_DEMO_AppHandleLeaveIndication
 * Description       : Handling function for network leave indications
 * Argument          : None
 * Return Value      : None
 ***********************************************************************/
/*!
   \fn void R_DEMO_AppHandleLeaveIndication(void);
   \brief Handling function for network leave indications
 */
void R_DEMO_AppHandleLeaveIndication(void);

/***********************************************************************
 * Function Name     : R_DEMO_AppHandleBufferInd
 * Description       : Handling function for buffer indications
 * Argument          : ind : Pointer to indication structure
 * Return Value      : None
 ***********************************************************************/
/*!
   \fn void R_DEMO_AppHandleBufferInd(const r_adp_adpm_buffer_ind_t* ind);
   \brief Handling function for buffer indications
 */
void R_DEMO_AppHandleBufferInd(const r_adp_adpm_buffer_ind_t *ind);

/***********************************************************************
 * Function Name     : R_DEMO_AppHandleKeyStateInd
 * Description       : Handling function for key state indications
 * Argument          : ind : Pointer to indication structure
 * Return Value      : None
 ***********************************************************************/
/*!
   \fn void R_DEMO_AppHandleKeyStateInd(const r_adp_adpm_key_state_ind_t* ind);
   \brief Handling function for key state indications
 */
void R_DEMO_AppHandleKeyStateInd(const r_adp_adpm_key_state_ind_t *ind);

/***********************************************************************
 * Function Name     : R_DEMO_AppHandleStatusInd
 * Description       : Handling function for network status indications
 * Argument          : ind : Pointer to indication structure
 * Return Value      : None
 ***********************************************************************/
/*!
   \fn void R_DEMO_AppHandleStatusInd(const r_adp_adpm_network_status_ind_t* ind);
   \brief Handling function for network status indications
 */
void R_DEMO_AppHandleStatusInd(const r_adp_adpm_network_status_ind_t *ind);

/***********************************************************************
 * Function Name     : R_DEMO_AppHandlePathDiscInd
 * Description       : Handling function for path discovery indications
 * Argument          : ind : Pointer to indication structure
 * Return Value      : None
 ***********************************************************************/
/*!
   \fn void R_DEMO_AppHandlePathDiscInd(const r_adp_adpm_path_discovery_ind_t* ind);
   \brief Handling function for path discovery indications
 */
void R_DEMO_AppHandlePathDiscInd(const r_adp_adpm_path_discovery_ind_t *ind);

/***********************************************************************
 * Function Name     : R_DEMO_AppHandleFrameCntInd
 * Description       : Handling function for reboot request indications
 * Argument          : None
 * Return Value      : None
 ***********************************************************************/
/*!
   \fn void R_DEMO_AppHandleFrameCntInd(const r_adp_adpm_framecounter_ind_t* ind);
   \brief Handling function for reboot request indications
 */
void R_DEMO_AppHandleFrameCntInd(r_adp_adpm_framecounter_ind_t *ind);

/***********************************************************************
 * Function Name     : R_DEMO_AppHandleRouteInd
 * Description       : Handling function for route update indications
 * Argument          : None
 * Return Value      : None
 ***********************************************************************/
/*!
   \fn void R_DEMO_AppHandleRouteInd(const r_adp_adpm_route_update_ind_t* ind);
   \brief Handling function for route update indications
 */
void R_DEMO_AppHandleRouteInd(const r_adp_adpm_route_update_ind_t *ind);

/***********************************************************************
 * Function Name     : R_DEMO_AppHandleLoadInd
 * Description       : Handling function for load sequence number indications
 * Argument          : None
 * Return Value      : None
 ***********************************************************************/
/*!
   \fn void R_DEMO_AppHandleLoadInd(const r_adp_adpm_load_seq_num_ind_t* ind);
   \brief Handling function for load sequence number indications
 */
void R_DEMO_AppHandleLoadInd(const r_adp_adpm_load_seq_num_ind_t *ind);

/***********************************************************************
 * Function Name     : R_DEMO_AppHandleRrepInd
 * Description       : Handling function for rrep indications
 * Argument          : None
 * Return Value      : None
 ***********************************************************************/
/*!
   \fn void R_DEMO_AppHandleRrepInd(const r_adp_adpm_rrep_ind_t* ind);
   \brief Handling function for rrep indications
 */
void R_DEMO_AppHandleRrepInd(const r_adp_adpm_rrep_ind_t *ind);

/***********************************************************************
 * Function Name     : R_DEMO_AppHandleSysRebootReqInd
 * Description       : Handling function for reboot request indications
 * Argument          : None
 * Return Value      : None
 ***********************************************************************/
/*!
   \fn void R_DEMO_AppHandleSysRebootReqInd(void);
   \brief Handling function for reboot request indications
 */
void R_DEMO_AppHandleSysRebootReqInd(const r_sys_rebootreq_ind_t *ind);

/***********************************************************************
 * Function Name     : R_DEMO_AppResetDevice
 * Description       : Handling function for device resetting
 * Argument          : None
 * Return Value      : None
 ***********************************************************************/
/*!
   \fn void R_DEMO_AppResetDevice(void);
   \brief Handling function for device resetting
 */
r_result_t R_DEMO_AppResetDevice(void);

/***********************************************************************
 * Function Name     : R_DEMO_AppJoinNetwork
 * Description       : Starts network join procedure
 * Argument          : None
 * Return Value      : None
 ***********************************************************************/
/*!
   \fn void R_DEMO_AppJoinNetwork(void);
   \brief Starts network join procedure
 */
r_result_t R_DEMO_AppJoinNetwork(void);

/*!
   \fn void R_DEMO_DeviceStartup(void)
   \brief Function used for device reboot(defined externally)
 */
r_result_t R_DEMO_ModemBoot(void);
void R_DEMO_ModemReboot(void);
void R_DEMO_App(void);
void R_DEMO_AppMainMenuProc(void);
void R_DEMO_AppMainMenuProcLbs(void);
r_result_t R_DEMO_AppClearInfo(uint8_t chId, uint8_t type);
r_result_t R_DEMO_AppGetLog(uint8_t chId, r_g3_info_layer_t layer);
r_result_t R_DEMO_AppGetStatistics(uint8_t chId, r_g3_info_layer_t layer);
r_result_t R_DEMO_SetDeviceType(void);
r_result_t R_DEMO_AppNetworkStart(uint16_t panId);
//uint16_t R_DEMO_AppNetworkDiscovery(uint8_t duration);
r_result_t R_DEMO_AppNetworkDiscovery(uint8_t duration);

void R_DEMO_AppHandleMcpsDataInd(const r_g3mac_mcps_data_ind_t *ind);

r_result_t R_DEMO_AppToggleMacPromiscuous(uint8_t onoff);
r_result_t R_DEMO_CheckModeBandPlan(uint8_t bandPlan, r_demo_mode_ch_t *pModeCh);

#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
void R_DEMO_AppHandleRfMcpsDataInd(const r_g3rfmac_mcps_data_ind_t *ind);
#endif

/*!
   \fn flags_manipulation
   \brief Status flag manipulation
 */

#endif /* R_DEMO_APP_H */
