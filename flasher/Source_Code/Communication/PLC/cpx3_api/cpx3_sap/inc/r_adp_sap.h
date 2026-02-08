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
 * File Name : r_adp_sap.h
 * Description : ADP layer API
 ******************************************************************************/

#ifndef R_ADP_SAP_H
#define R_ADP_SAP_H

/******************************************************************************
Macro definitions
******************************************************************************/

/*===========================================================================*/
/* For ADP SAP IF PARAM                                                      */
/*===========================================================================*/
#define R_ADP_MAX_IB_SIZE                   (0x25u)
#define R_ADP_PREFIX_TABLE_SIZE             (4u)                                         /*!< Size of the prefix table */
#define R_ADP_MAX_PATHTABLE_SIZE            (32u)
#define R_ADP_CONTEXT_TABLE_SIZE            (16u)                                        /*!< Size of context table (max. 16 because of 4 bit IDs) (ErDF requirements : The adpContextInformationTable is a data set of 16 entries.) */
#define R_ADP_BLACKLIST_TABLE_SIZE          (64u)                                        /*!< Blacklisted neighbor table size */
#define R_ADP_MAX_CONTEXT_LENGTH            (128u)                                       /*!< Maximum prefix length in context table in bit */
#define R_ADP_MAX_PREFIX_LENGTH             (128u)
#define R_ADP_BROADCAST_LOG_TABLE_SIZE      (128u)                                       /*!< Size of broadcast log table */
#define R_ADP_GROUP_TABLE_SIZE              (16u)                                        /*!< Size of table which contains the group addresses */
#define R_ADP_DESTINATION_ADDRESS_SET_SIZE  (10u)                                        /*!< Size of table which contains the destination address set */

/*===========================================================================*/
/* For ADP SAP ID                                                            */
/*===========================================================================*/

/******************************************************************************
Typedef definitions
******************************************************************************/

/* Information Base Attributes default  value   */
/* defined in specification */
typedef enum
{
    R_ADP_IB_SECURITY_LEVEL                =   0x00,                                     /*!< The minimum security level to be used for incoming and outgoing Adaptation frames */
    R_ADP_IB_PREFIX_TABLE                  =   0x01,                                     /*!< Contains the list of prefixes defined on this PAN. Note that it is assumed that the link local IPv6 address exists independently and is not affected by the prefixes defined in the prefix table */
    R_ADP_IB_BROADCAST_LOG_TABLE_ENTRY_TTL =   0x02,                                     /*!< Maximum time to live of a adpBroadcastLogTable entry (in seconds) */
    R_ADP_IB_METRIC_TYPE                   =   0x03,                                     /*!< Metric Type to be used for routing purposes */
    R_ADP_IB_LOW_LQI_VALUE                 =   0x04,                                     /*!< The low LQI value defines the LQI value, used in metric computation, below which a link to a neighbour is considered as an unreliable link. This value shall be lower than adpHighLQIValue */
    R_ADP_IB_HIGH_LQI_VALUE                =   0x05,                                     /*!< The high LQI value defines the LQI value, used in metric computation, above which a link to a neighbour is considered as a reliable link. This value is greater than adpLowLQIValue */
    R_ADP_IB_RREP_WAIT                     =   0x06,                                     /*!< Waiting time for RREP message generation (in seconds) */
    R_ADP_IB_CONTEXT_INFORMATION_TABLE     =   0x07,                                     /*!< Contains the context information associated to each CID extension field */
    R_ADP_IB_COORD_SHORT_ADDRESS           =   0x08,                                     /*!< Defines the short address of the coordinator */
    R_ADP_IB_RLC_ENABLED                   =   0x09,                                     /*!< Enable the sending of RLCREQ frame by the device */
    R_ADP_IB_ADD_REV_LINK_COST             =   0x0A,                                     /*!< Represents an additional adjustment related to the possible asymmetry of the link (in the absence of reverse information) that is taken into account for link cost computation */
    R_ADP_IB_BROADCAST_LOG_TABLE           =   0x0B,                                     /*!< Contains the broadcast log table */
    R_ADP_IB_ROUTING_TABLE                 =   0x0C,                                     /*!< Contains the routing table */
    R_ADP_IB_UNICAST_RREQ_GEN_ENABLE       =   0x0D,                                     /*!< If TRUE, the RREQ shall be generated with */
    R_ADP_IB_GROUP_TABLE                   =   0x0E,                                     /*!< Contains the group addresses to which the device belongs */
    R_ADP_IB_MAX_HOPS                      =   0x0F,                                     /*!< Defines the maximum number of hops to be used by the routing algorithm */
    R_ADP_IB_DEVICE_TYPE                   =   0x10,                                     /*!< Defines the type of the device connected to the modem */
    R_ADP_IB_NET_TRAVERSAL_TIME            =   0x11,                                     /*!< The Max duration between RREQ and the correspondent RREP (in seconds) */
    R_ADP_IB_ROUTING_TABLE_ENTRY_TTL       =   0x12,                                     /*!< Maximum time to live of a routing table entry (in seconds) */
    R_ADP_IB_KR                            =   0x13,                                     /*!< A weight factor for ROBO to calculate link cost */
    R_ADP_IB_KM                            =   0x14,                                     /*!< A weight factor for modulation to calculate link cost */
    R_ADP_IB_KC                            =   0x15,                                     /*!< A weight factor for number of active tones to calculate link cost */
    R_ADP_IB_KQ                            =   0x16,                                     /*!< A weight factor for LQI to calculate route cost */
    R_ADP_IB_KH                            =   0x17,                                     /*!< A weight factor for hop to calculate link cost */
    R_ADP_IB_RREQ_RETRIES                  =   0x18,                                     /*!< The number of RREQ retransmission in case of RREP reception time out */
    R_ADP_IB_RREQ_WAIT                     =   0x19,                                     /*!< ofTime in seconds to wait between two consecutive RREQ generations. */
    R_ADP_IB_WEAK_LQI_VALUE                =   0x1A,                                     /*!< The weak link value defines the threshold below which a link to a direct neighbour is considered as weak link */
    R_ADP_IB_KRT                           =   0x1B,                                     /*!< A weight factor for the number of active routes in the routing table to calculate link cost */
    R_ADP_IB_SOFT_VERSION                  =   0x1C,                                     /*!< The software version */
    R_ADP_IB_BLACKLIST_TABLE               =   0x1E,                                     /*!< Contains the list of the blacklisted neighbours */
    R_ADP_IB_BLACKLIST_TABLE_ENTRY_TTL     =   0x1F,                                     /*!< Maximum time to live of a blacklisted neighbour set entry in minutes */
    R_ADP_IB_MAX_JOIN_WAIT_TIME            =   0x20,                                     /*!< Network joint timeout in seconds for LBD */
    R_ADP_IB_PATH_DISCOVERY_TIME           =   0x21,                                     /*!< Timeout for path discovery in seconds */
    R_ADP_IB_ACTIVE_KEY_INDEX              =   0x22,                                     /*!< Index of active GMK to be used for data transmission */
    R_ADP_IB_DESTINATION_ADDRESS_SET       =   0x23,                                     /*!< Contains the destination address set table */
    R_ADP_IB_DEFAULT_COORD_ROUTE_ENABLED   =   0x24,                                     /*!< If TRUE, the adaptation layer adds a default route to the coordinator after successful completion of the bootstrapping procedure. If FALSE no default route will be created */

    R_ADP_IB_DELAY_LOW_LQI                 =   0x25,                                     /*!< Delay in ms before retransmitting an RREQ in an intermediate node when the LQI of the received RREQ is below adpRREQJitterLowLQI */
    R_ADP_IB_DELAY_HIGH_LQI                =   0x26,                                     /*!< Delay in ms before retransmitting an RREQ in an intermediate node when the LQI of the received RREQ is above adpRREQJitterHighLQI */
    R_ADP_IB_RREQ_JITTER_LOW_LQI           =   0x27,                                     /*!< LQI value below which RREQ retransmission is delayed by adpDelayLowLQI */
    R_ADP_IB_RREQ_JITTER_HIGH_LQI          =   0x28,                                     /*!< LQI value above which RREQ retransmission is delayed by adpDelayHighLQI */

    R_ADP_IB_TRICKLE_DATA_ENABLED          =   0x29,
    R_ADP_IB_TRICKLE_STEP                  =   0x2B,
    R_ADP_IB_TRICKLE_FACTOR_IMAX           =   0x2C,
    R_ADP_IB_TRICKLE_IMIN                  =   0x2D,
    R_ADP_IB_TRICKLE_MAX_KI                =   0x2E,
    R_ADP_IB_TRICKLE_ADAPTIVE_IMIN         =   0x2F,

    R_ADP_IB_CLUSTER_TRICKLE_ENABLED       =   0x30,
    R_ADP_IB_CLUSTER_MIN_LQI               =   0x31,
    R_ADP_IB_CLUSTER_TRICKLE_K             =   0x32,
    R_ADP_IB_CLUSTER_RREQ_RC_DEVIATION     =   0x33,
    R_ADP_IB_CLUSTER_TRICKLE_I             =   0x34,

    R_ADP_IB_STATISTICS                    =   0x50,

    /* Renesas specific ADP attributes. */
    R_ADP_IB_LOAD_SEQ_NUMBER               =   0x51,                                     /* similar to R_ADP_IB_LOADNG_SEQNUM */
    R_ADP_IB_ROUTE_TABLE_BY_ADDR           =   0x52,                                     /* similar to R_ADP_IB_RT_BY_DST_ADDR */
    R_ADP_IB_ROUTE_TABLE_SIZE              =   0x53,
    R_ADP_IB_VALID_RTABLE_ENTRIES          =   0x54,
    R_ADP_IB_ROUTE_INDEX_BY_ADDR           =   0x55,
    R_ADP_IB_ROUTE_IND_ENABLE              =   0x56,
    R_ADP_IB_LOAD_SEQ_NUM_IND_INTERVAL     =   0x57,
    R_ADP_IB_BEACON_IND_ENABLE             =   0x58,
    R_ADP_IB_BUFF_IND_DISABLE              =   0x59,
    R_ADP_IB_CLR_ALL_ROUTE                 =   0x5A,
    R_ADP_IB_RREP_IND_ENABLE               =   0x5B,
    R_ADP_IB_DATATYPE                      =   0x5C,
    R_ADP_IB_ENABLE_DATATRANS              =   0x5D,
    R_ADP_IB_DISABLE_RELAY                 =   0x5E,

    R_ADP_IB_ENABLE_DEBUG                  =   0x5F,

    R_ADP_IB_TRICKLE_K                     =   0x60,
    R_ADP_IB_TRICKLE_IMAX                  =   0x61,

    R_ADP_IB_KEY_IND_ENABLE                =   0x62,
    R_ADP_IB_IDS_IND_ENABLE                =   0x63,
    R_ADP_IB_DECLINE_ACTIVEKEY_RMV         =   0x64,


    R_ADP_IB_LOW_LQI_VALUE_RF              =   0xD0,
    R_ADP_IB_HIGH_LQI_VALUE_RF             =   0xD1,
    R_ADP_IB_KQ_RF                         =   0xD2,
    R_ADP_IB_KH_RF                         =   0xD3,
    R_ADP_IB_KRT_RF                        =   0xD4,
    R_ADP_IB_KDC_RF                        =   0xD5,
    R_ADP_IB_USE_BACKUPMEDIA               =   0xD6,
    R_ADP_IB_WEAK_LQI_VALUE_RF             =   0xD7,

    R_ADP_IB_DISABLE_DEFAULT_ROUTING       =   0xF0,                                     /*!< If TRUE, the default routing (LOADng) is disabled. If FALSE, the default routing (LOADng) is enabled */

} r_adp_ib_id_t;

/*===========================================================================*/
/* For ADP SAP IF ENUM                                                       */
/*===========================================================================*/
typedef enum
{
    /* based on G.9903 */
    R_ADP_STATUS_SUCCESS                   =   R_G3MAC_STATUS_SUCCESS,                   //  0x00

    /* shared with MAC */
    R_ADP_STATUS_COUNTER_ERROR             =   R_G3MAC_STATUS_COUNTER_ERROR,             //  0xDB
    R_ADP_STATUS_IMPROPER_KEY_TYPE         =   R_G3MAC_STATUS_IMPROPER_KEY_TYPE,         //  0xDC
    R_ADP_STATUS_IMPROPER_SECURITY_LEVEL   =   R_G3MAC_STATUS_IMPROPER_SECURITY_LEVEL,   //  0xDD
    R_ADP_STATUS_UNSUPPORTED_LEGACY        =   R_G3MAC_STATUS_UNSUPPORTED_LEGACY,        //  0xDE
    R_ADP_STATUS_UNSUPPORTED_SECURITY      =   R_G3MAC_STATUS_UNSUPPORTED_SECURITY,      //  0xDF
    R_ADP_STATUS_CHANNEL_ACCESS_FAILURE    =   R_G3MAC_STATUS_CHANNEL_ACCESS_FAILURE,    //  0xE1
    R_ADP_STATUS_SECURITY_ERROR            =   R_G3MAC_STATUS_SECURITY_ERROR,            //  0xE4
    R_ADP_STATUS_FRAME_TOO_LONG            =   R_G3MAC_STATUS_FRAME_TOO_LONG,            //  0xE5
    R_ADP_STATUS_INVALID_HANDLE            =   R_G3MAC_STATUS_INVALID_HANDLE,            //  0xE7
    R_ADP_STATUS_INVALID_PARAMETER         =   R_G3MAC_STATUS_INVALID_PARAMETER,         //  0xE8
    R_ADP_STATUS_NO_ACK                    =   R_G3MAC_STATUS_NO_ACK,                    //  0xE9
    R_ADP_STATUS_NO_BEACON                 =   R_G3MAC_STATUS_NO_BEACON,                 //  0xEA
    R_ADP_STATUS_NO_DATA                   =   R_G3MAC_STATUS_NO_DATA,                   //  0xEB
    R_ADP_STATUS_NO_SHORT_ADDRESS          =   R_G3MAC_STATUS_NO_SHORT_ADDRESS,          //  0xEC
    R_ADP_STATUS_OUT_OF_CAP                =   R_G3MAC_STATUS_OUT_OF_CAP,                //  0xED
    R_ADP_STATUS_ALTERNATE_PANID_DETECTION =   R_G3MAC_STATUS_ALTERNATE_PANID_DETECTION, //  0xEE
    R_ADP_STATUS_UNAVAILABLE_KEY           =   R_G3MAC_STATUS_UNAVAILABLE_KEY,           //  0xF3
    R_ADP_STATUS_UNSUPPORTED_ATTRIBUTE     =   R_G3MAC_STATUS_UNSUPPORTED_ATTRIBUTE,     //  0xF4
    R_ADP_STATUS_INVALID_ADDRESS           =   R_G3MAC_STATUS_INVALID_ADDRESS,           //  0xF5
    R_ADP_STATUS_INVALID_INDEX             =   R_G3MAC_STATUS_INVALID_INDEX,             //  0xF9
    R_ADP_STATUS_LIMIT_REACHED             =   R_G3MAC_STATUS_LIMIT_REACHED,             //  0xFA
    R_ADP_STATUS_READ_ONLY                 =   R_G3MAC_STATUS_READ_ONLY,                 //  0xFB
    R_ADP_STATUS_SCAN_IN_PROGRESS          =   R_G3MAC_STATUS_SCAN_IN_PROGRESS,          //  0xFC

    R_ADP_STATUS_MAC_INVALID_STATE         =   R_G3MAC_STATUS_INVALID_STATE,
    R_ADP_STATUS_MAC_NO_RESPONSE           =   R_G3MAC_STATUS_NO_RESPONSE,
    R_ADP_STATUS_DUTY_CYCLE_REACHED        =   R_G3RFMAC_STATUS_DUTY_CYCLE_REACHED,      //0xD0
    R_ADP_STATUS_TX_ACTIVE                 =   R_G3RFMAC_STATUS_TX_ACTIVE,               //0xF2
    R_ADP_STATUS_LML_NO_ACK                =   R_G3MAC_STATUS_LML_NO_ACK,
    R_ADP_STATUS_LML_CSMA_FAILURE          =   R_G3MAC_STATUS_LML_CSMA_FAILURE,
    R_ADP_STATUS_LML_BUFFER_FULL           =   R_G3MAC_STATUS_LML_BUFFER_FULL,
    R_ADP_STATUS_LML_INVALID_REQ           =   R_G3MAC_STATUS_LML_INVALID_REQ,
    R_ADP_STATUS_LML_TOO_FEW_SC            =   R_G3MAC_STATUS_LML_TOO_FEW_SC,
    R_ADP_STATUS_LML_ABORT_ERROR           =   R_G3MAC_STATUS_LML_ABORT_ERROR,
    R_ADP_STATUS_LML_NO_RESPONSE           =   R_G3MAC_STATUS_LML_NO_RESPONSE,
    R_ADP_STATUS_LML_FAILURE               =   R_G3MAC_STATUS_LML_FAILURE,

    /* ADP status */
    R_ADP_STATUS_INVALID_REQUEST           =   0x80,
    R_ADP_STATUS_INVALID_IPV6_FRAME        =   0x82,
    R_ADP_STATUS_ROUTE_ERROR               =   0x83,
    R_ADP_STATUS_NOT_PERMITTED             =   0x84,
    R_ADP_STATUS_TIMEOUT                   =   0x86,
    R_ADP_STATUS_ALREADY_IN_PROGRESS       =   0x87,
    R_ADP_STATUS_INCOMPLETE_PATH           =   0x88,

    /* original status */
    R_ADP_STATUS_REQ_QUEUE_FULL            =   0x92,
    R_ADP_STATUS_FAILED                    =   0x93,
    R_ADP_STATUS_CONFIG_ERROR              =   0x95,

    /* IF status */
    R_ADP_STATUS_IF_FATAL_ERROR            =   R_G3MAC_STATUS_IF_FATAL_ERROR,
    R_ADP_STATUS_IF_TIMEOUT                =   R_G3MAC_STATUS_IF_TIMEOUT,
    R_ADP_STATUS_IF_QUEUE_FULL             =   R_G3MAC_STATUS_IF_QUEUE_FULL,
    R_ADP_STATUS_IF_INVALID_STATE          =   R_G3MAC_STATUS_IF_INVALID_STATE,

    /* closed status */
    R_ADP_STATUS_INSUFFICIENT_MEMSIZE      =   R_G3MAC_STATUS_INSUFFICIENT_MEMSIZE,
    R_ADP_STATUS_IF_NO_RESPONSE            =   0xA2,

} r_adp_status_t;


typedef enum
{
    R_ADP_DEVICE_TYPE_PEER        = 0,                                                   /*!< PEER */
    R_ADP_DEVICE_TYPE_COORDINATOR = 1,                                                   /*!< COORDINATOR */
    R_ADP_DEVICE_TYPE_NOT_DEFINED = 2                                                    /*!< NONE */
} r_adp_device_type_t;

typedef enum
{
    R_LOADNG_HOPCOUNT_METRIC = 0x00,                                                     /*!< R_LOADNG_HOPCOUNT_METRIC */
    R_LOADNG_CAPACITY_METRIC = 0x0E,                                                     /*!< R_LOADNG_CAPACITY_METRIC */
    R_LOADNG_DEFAULT_METRIC  = 0x0F,                                                     /*!< R_LOADNG_DEFAULT_METRIC */
    R_LOADNG_UNKNOWN_METRIC  = 0xFF                                                      /*!< R_LOADNG_UNKNOWN_METRIC */
} r_adp_loadng_metric_type_t;



typedef enum
{
    R_EAPPSK_KEY_TYPE_PSK = 0,
    R_EAPPSK_KEY_TYPE_AK,
    R_EAPPSK_KEY_TYPE_KDK,
    R_EAPPSK_KEY_TYPE_TEK,
    R_EAPPSK_KEY_TYPE_MSK,
    R_EAPPSK_KEY_TYPE_EMSK,
    R_EAPPSK_KEY_TYPE_ID_P = 0x40,
    R_EAPPSK_KEY_TYPE_ID_S,
} r_eappsk_key_type_t;


typedef uint8_t r_adp_cb_status_t;


/*===========================================================================*/
/* For ADP TABLE STRUCT                                                      */
/*===========================================================================*/
__packed typedef struct
{
    uint8_t compressionFlag;
    uint8_t contextLength;
    uint8_t context[R_ADP_MAX_CONTEXT_LENGTH / 8u];
    uint8_t validLifetime[2];
} r_adp_contextinfo_table_t;

__packed typedef struct
{
    uint8_t B_neighbour_address[2];
    uint8_t validTime[2];
    uint8_t mediaType;
} r_adp_blacklisted_nei_table_t;


__packed typedef struct
{
    uint8_t prefixLength;
    uint8_t prefix[R_ADP_MAX_PREFIX_LENGTH / 8u];
    uint8_t L;
    uint8_t A;
    uint8_t validLifetime[4];
    uint8_t preferredLifetime[4];
} r_adp_prefix_table_t;

__packed typedef struct
{
    uint8_t validState;
    uint8_t memberAddr[2];
} r_adp_group_table_t;

__packed typedef struct
{
    uint8_t srcAddr[2];
    uint8_t seqNumber;
    uint8_t validTime[2];
} r_adp_broadcast_log_table_t;

__packed typedef struct
{
    uint8_t R_dest_Addr[2];
    uint8_t R_next_Addr[2];
    uint8_t R_metric[2];
    uint8_t R_hop_count;
    uint8_t R_weak_link_count : 4;
    uint8_t rsv               : 3;
    uint8_t R_media_type      : 1;
    uint8_t validTime[2];
} r_adp_routing_table_t;

__packed typedef struct
{
    uint8_t validState;
    uint8_t dstAddr[2];
} r_adp_destination_address_set_t;


__packed typedef struct
{
    uint8_t adpVersion[2];
    uint8_t macVersion[2];
    uint8_t dspVersion[2];
    uint8_t rfMacVersion[3];
    uint8_t rfPhyVersion[10];
} r_adp_soft_version_t;


/*===========================================================================*/
/* For ADP SAP IF STRUCT                                                     */
/*===========================================================================*/
/* ADPD-DATA */
__packed typedef struct
{
    uint16_t  nsduLength;
    uint8_t * pNsdu;
    uint8_t   nsduHandle;
    uint8_t   discoverRoute;
    uint8_t   qualityOfService;
} r_adp_adpd_data_req_t;

__packed typedef struct
{
    r_adp_cb_status_t status;     /*!< The status code of a previous ADPD-DATA.request identified by its nsduHandle */
    uint8_t           nsduHandle; /*!< The handle of the NSDU to transmit */
} r_adp_adpd_data_cnf_t;

__packed typedef struct
{
    uint16_t  nsduLength;
    uint8_t * pNsdu;
    uint8_t   linkQualityIndicator;
} r_adp_adpd_data_ind_t;

/* ADPM-DISCOVERY */
__packed typedef struct
{
    uint8_t duration;             /*!< Scan duration in seconds */
} r_adp_adpm_discovery_req_t;


typedef r_g3mac_pan_descriptor_t r_adp_pan_descriptor_t;

__packed typedef struct
{
    r_adp_cb_status_t        status;
    uint8_t                  PANCount;
    r_adp_pan_descriptor_t * PANDescriptor;
} r_adp_adpm_discovery_cnf_t;

/* ADPM-NETWORK-START */
__packed typedef struct
{
    uint16_t panId;               /*!< The PAN ID to use for the network */
} r_adp_adpm_network_start_req_t;

__packed typedef struct
{
    r_adp_cb_status_t status;     /*!< The result of the request for starting a network */
} r_adp_adpm_network_start_cnf_t;

/* ADPM-NETWORK-JOIN */
__packed typedef struct
{
    uint16_t panId;
    uint8_t  lbaAddress[2];
    uint8_t  mediaType;
} r_adp_adpm_network_join_req_t;

__packed typedef struct
{
    r_adp_cb_status_t status;
    uint8_t           networkAddress[2];
    uint16_t          panId;
    uint8_t           mediaType;
} r_adp_adpm_network_join_cnf_t;

/* ADPM-NETWORK-LEAVE */
__packed typedef struct
{
    r_adp_cb_status_t status;     /*!< The status of the request */
} r_adp_adpm_network_leave_cnf_t;

/* ADPM-RESET */
__packed typedef struct
{
    r_adp_cb_status_t status;     /*!< The result of the request for a reset */
} r_adp_adpm_reset_cnf_t;

/* ADPM-GET */
__packed typedef struct
{
    uint8_t  aibAttributeId;
    uint16_t aibAttributeIndex;
} r_adp_adpm_get_req_t;

__packed typedef struct
{
    r_adp_cb_status_t status;
    uint8_t           aibAttributeId;
    uint16_t          aibAttributeIndex;
    uint8_t           aibAttributeValue[R_ADP_MAX_IB_SIZE];
} r_adp_adpm_get_cnf_t;

/* ADPM-SET */
__packed typedef struct
{
    uint8_t   aibAttributeId;
    uint16_t  aibAttributeIndex;
    uint8_t * aibAttributeValue;
} r_adp_adpm_set_req_t;

__packed typedef struct
{
    r_adp_cb_status_t status;
    uint8_t           aibAttributeId;
    uint16_t          aibAttributeIndex;
} r_adp_adpm_set_cnf_t;

/* ADPM-NETWORK-STATUS */
__packed typedef struct
{
    r_adp_cb_status_t status;
    uint16_t          panId;
    uint8_t           srcAddrMode;
    uint8_t           srcAddr[8];
    uint8_t           dstAddrMode;
    uint8_t           dstAddr[8];
    uint8_t           securityLevel;
    uint8_t           keyIndex;
    uint8_t           mediaType;
} r_adp_adpm_network_status_ind_t;

/* ADPM-ROUTE-DISCOVERY */
__packed typedef struct
{
    uint8_t dstAddress[2];
    uint8_t maxHops;
} r_adp_adpm_route_disc_req_t;

__packed typedef struct
{
    r_adp_cb_status_t status;
    uint8_t           dstAddr[2];
} r_adp_adpm_route_disc_cnf_t;

/* ADPM-PATH-DISCOVERY */
__packed typedef struct
{
    uint8_t dstAddress[2];
    uint8_t pathMetricType;
} r_adp_adpm_path_discovery_req_t;

__packed typedef struct
{
    uint8_t pathAddress[2];
    uint8_t reserved   :2;
    uint8_t MTx        :1;
    uint8_t MRx        :1;
    uint8_t phaseDiff  :3;
    uint8_t mns        :1;
    uint8_t linkCost;
} r_adp_path_table_t;

__packed typedef struct
{
    uint8_t              dstAddr[2];
    r_adp_cb_status_t    status;
    uint8_t              pathMetricType;
    uint8_t              origAddr[2];
    uint8_t              pathTableEntries;
    r_adp_path_table_t * pathTable;
} r_adp_adpm_path_discovery_cnf_t;

__packed typedef struct
{
    uint8_t origAddr[2];
} r_adp_adpm_path_discovery_ind_t;

/* ADPM-LBP */
__packed typedef struct
{
    uint8_t   dstAddrType;
    uint8_t   dstAddr[8];
    uint16_t  nsduLength;
    uint8_t * pNsdu;
    uint8_t   nsduHandle;
    uint8_t   maxHops;
    uint8_t   discoverRoute;
    uint8_t   qualityOfService;
    uint8_t   securityEnabled;
    uint8_t   mediaType;
} r_adp_adpm_lbp_req_t;

__packed typedef struct
{
    r_adp_cb_status_t status;
    uint8_t           nsduHandle;
    uint8_t           mediaType;
} r_adp_adpm_lbp_cnf_t;

__packed typedef struct
{
    uint8_t   srcAddr[2];
    uint16_t  nsduLength;
    uint8_t * pNsdu;
    uint8_t   linkQualityIndicator;
    uint8_t   securityEnabled;
    uint8_t   mediaType;
} r_adp_adpm_lbp_ind_t;

/* ADPM-BUFFER */
__packed typedef struct
{
    uint8_t bufferReady;
} r_adp_adpm_buffer_ind_t;

/* ADPM-KEY-STATUS */
__packed typedef struct
{
    uint8_t keyIndex;
    uint8_t keyState;   /* 0:receive 1:activate  2:remove */
} r_adp_adpm_key_state_ind_t;

/* ADPM-ROUTE-ERROR */
__packed typedef struct
{
    uint8_t unreachableAddress[2];
} r_adp_adpm_route_error_ind_t;

/* ADPM-EAP-KEY */
__packed typedef struct
{
    uint8_t type;       /* 4:MSK */
    uint8_t length;
    uint8_t key[64];
} r_adp_adpm_eap_key_ind_t;

/* ADPM-FRAMECOUNTER */
__packed typedef struct
{
    uint32_t frameCounter;
    uint8_t  mediaType;
} r_adp_adpm_framecounter_ind_t;

/* ADPM-ROUTE-UPDATE */
__packed typedef struct
{
    uint8_t dstAddr[2];
} r_adp_adpm_route_update_ind_t;

/* ADPM-LOAD-SEQ-NUM */
__packed typedef struct
{
    uint8_t adpLoadSeqNumber[2];
} r_adp_adpm_load_seq_num_ind_t;

/* ADPM-RREP */
__packed typedef struct
{
    uint8_t rrepOriginator[2];
    uint8_t rrepDestination[2];
} r_adp_adpm_rrep_ind_t;


/* ADPM-BEACON NOTIFY */
typedef r_g3mac_mlme_bcn_notify_ind_t r_adp_adpm_bcn_notify_ind_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#endif /* #ifndef R_ADP_SAP_H */

