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
 * File Name : r_eap_sap.h
 * Description : EAP layer API
 ******************************************************************************/

#ifndef R_EAP_SAP_H
#define R_EAP_SAP_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_EAP_MAX_IB_SIZE  (66u)

/******************************************************************************
Typedef definitions
******************************************************************************/

/*===========================================================================*/
/* For EAP SAP IF PARAM                                                      */
/*===========================================================================*/
typedef enum
{
    R_EAP_IB_GMK                =   0x00,
    R_EAP_IB_ACTIVEKEYINDEX     =   0x01,
    R_EAP_IB_COORDSHORTADDRESS  =   0x02,
    R_EAP_IB_CINFOTABLEENTRIES  =   0x03,
    R_EAP_IB_CLIENTINFOTABLE    =   0x04,
    R_EAP_IB_JOIN_WAITTIMESEC   =   0x05,
    R_EAP_IB_JOINGMKID          =   0x06,
    R_EAP_IB_DISABLE_JOIN       =   0x07,
    R_EAP_IB_EAPPSK_RETRYNUM    =   0x08,
    R_EAP_IB_NWK_WAITTIMESEC    =   0x09,
    R_EAP_IB_ONETIME_CLIENTINFO =   0x0A,
    R_EAP_IB_KEY_IND_ENABLE     =   0x0B,
    R_EAP_IB_IDP_IND_ENABLE     =   0x0C,
    R_EAP_IB_KEY_VALIDATE       =   0x0D,
    R_EAP_IB_TXALLGMK_JOIN      =   0x0E,
    R_EAP_IB_SOFTVERSION        =   0x10,
    R_EAP_IB_STATISTICS         =   0x50,
} r_eap_ib_id_t;


/*===========================================================================*/
/* For EAP SAP IF ENUM                                                       */
/*===========================================================================*/
typedef enum
{
    R_EAP_STATUS_SUCCESS                    =   0x00,

    R_EAP_STATUS_INVALID_PARAMETER          =   R_ADP_STATUS_INVALID_PARAMETER,
    R_EAP_STATUS_UNSUPPORTED_ATTRIBUTE      =   R_ADP_STATUS_UNSUPPORTED_ATTRIBUTE,
    R_EAP_STATUS_INVALID_INDEX              =   R_ADP_STATUS_INVALID_INDEX,
    R_EAP_STATUS_READ_ONLY                  =   R_ADP_STATUS_READ_ONLY,

    R_EAP_STATUS_MAC_COUNTER_ERROR          =   R_ADP_STATUS_COUNTER_ERROR,
    R_EAP_STATUS_MAC_CHANNEL_ACCESS_FAILURE =   R_ADP_STATUS_CHANNEL_ACCESS_FAILURE,
    R_EAP_STATUS_MAC_SECURITY_ERROR         =   R_ADP_STATUS_SECURITY_ERROR,
    R_EAP_STATUS_MAC_NO_ACK                 =   R_ADP_STATUS_NO_ACK,
    R_EAP_STATUS_MAC_OUT_OF_CAP             =   R_ADP_STATUS_OUT_OF_CAP,
    R_EAP_STATUS_MAC_UNAVAILABLE_KEY        =   R_ADP_STATUS_UNAVAILABLE_KEY,
    R_EAP_STATUS_MAC_LIMIT_REACHED          =   R_ADP_STATUS_LIMIT_REACHED,

    R_EAP_STATUS_LML_NO_ACK                 =   R_ADP_STATUS_LML_NO_ACK,
    R_EAP_STATUS_LML_CSMA_FAILURE           =   R_ADP_STATUS_LML_CSMA_FAILURE,
    R_EAP_STATUS_LML_BUFFER_FULL            =   R_ADP_STATUS_LML_BUFFER_FULL,
    R_EAP_STATUS_LML_INVALID_REQ            =   R_ADP_STATUS_LML_INVALID_REQ,
    R_EAP_STATUS_LML_TOO_FEW_SC             =   R_ADP_STATUS_LML_TOO_FEW_SC,
    R_EAP_STATUS_LML_ABORT_ERROR            =   R_ADP_STATUS_LML_ABORT_ERROR,
    R_EAP_STATUS_LML_NO_RESPONSE            =   R_ADP_STATUS_LML_NO_RESPONSE,
    R_EAP_STATUS_LML_FAILURE                =   R_ADP_STATUS_LML_FAILURE,

    R_EAP_STATUS_INVALID_REQUEST            =   R_ADP_STATUS_INVALID_REQUEST,
    R_EAP_STATUS_FAILED                     =   R_ADP_STATUS_FAILED,
    R_EAP_STATUS_CONFIG_ERROR               =   R_ADP_STATUS_CONFIG_ERROR,

    R_EAP_STATUS_ADP_ROUTE_ERROR            =   R_ADP_STATUS_ROUTE_ERROR,
    R_EAP_STATUS_ADP_TIMEOUT                =   R_ADP_STATUS_TIMEOUT,
    R_EAP_STATUS_ADP_QUEUE_FULL             =   R_ADP_STATUS_REQ_QUEUE_FULL,

    R_EAP_STATUS_EAP_PSK_IN_PROGRESS        =   0x98,
    R_EAP_STATUS_BLACKLISTED_DEVICE         =   0x99,
    R_EAP_STATUS_EAP_PSK_FAILURE            =   0x9A,
    R_EAP_STATUS_REQ_QUEUE_FULL             =   0x9B,
    R_EAP_STATUS_TIMEOUT                    =   0x9C,
    R_EAP_STATUS_JOIN_DISCARD               =   0x9D,

    /* IF status */
    R_EAP_STATUS_IF_FATAL_ERROR             =   R_ADP_STATUS_IF_FATAL_ERROR,
    R_EAP_STATUS_IF_TIMEOUT                 =   R_ADP_STATUS_IF_TIMEOUT,
    R_EAP_STATUS_IF_QUEUE_FULL              =   R_ADP_STATUS_IF_QUEUE_FULL,
    R_EAP_STATUS_IF_INVALID_STATE           =   R_ADP_STATUS_IF_INVALID_STATE,

    /* closed status */
    R_EAP_STATUS_INSUFFICIENT_MEMSIZE       =   R_ADP_STATUS_INSUFFICIENT_MEMSIZE,
    R_EAP_STATUS_IF_NO_RESPONSE             =   0xA3,

} r_eap_status_t;


typedef uint8_t r_eap_cb_status_t;

typedef enum
{
    R_EAP_NETWORK_KICK        = 0x01, /*!< Kick */
    R_EAP_NETWORK_GMKSEND     = 0x02, /*!< GMK Sending on Re-Keying */
    R_EAP_NETWORK_GMKACTIVATE = 0x03, /*!< GMK Activate on Re-Keying */
    R_EAP_NETWORK_GMKREMOVAL  = 0x04  /*!< GMK Removal after Re-Keying */
} r_eap_network_cmdid_t;

/*add   for ROUTE   B */
typedef struct
{
    uint8_t length;
    uint8_t id[36];
} r_eapm_extid_t;

typedef struct
{
    uint8_t valid_state;              /* EAP_CINFO_VALID_STATE */
    uint8_t extended_address[8];
    uint8_t access_ctrl;              /* EAP_CINFO_ACCESS_CONTROL_INFO */
    uint8_t network_address[2];
    uint8_t PSK[16];
    uint8_t wait_msg3;
} r_eap_clientinfo_table_t;


/*===========================================================================*/
/* For EAP SAP IF STRUCT                                                     */
/*===========================================================================*/
/* EAPM-RESET */
typedef struct
{
    r_eap_cb_status_t status;
} r_eap_eapm_reset_cnf_t;

/* EAPM-START */
typedef struct
{
    r_eap_cb_status_t status;
} r_eap_eapm_start_cnf_t;

/* EAPM-GET */
typedef struct
{
    uint8_t  eibAttributeId;
    uint16_t eibAttributeIndex;
} r_eap_eapm_get_req_t;

typedef struct
{
    r_eap_cb_status_t status;
    uint8_t           eibAttributeId;
    uint16_t          eibAttributeIndex;
    uint8_t           eibAttributeValue[R_EAP_MAX_IB_SIZE];
} r_eap_eapm_get_cnf_t;

/* EAPM-SET */
typedef struct
{
    uint8_t   eibAttributeId;
    uint16_t  eibAttributeIndex;
    uint8_t * eibAttributeValue;
} r_eap_eapm_set_req_t;

typedef struct
{
    r_eap_cb_status_t status;
    uint8_t           eibAttributeId;
    uint16_t          eibAttributeIndex;
} r_eap_eapm_set_cnf_t;

/* EAPM-NETWORK-JOIN */
typedef struct
{
    r_eap_cb_status_t status;
    uint8_t           extendedAddress[8];
    uint8_t           lbaAddress[2];
    uint8_t           mediaType;
} r_eap_eapm_network_join_ind_t;

/* EAPM-NETWORK-LEAVE */
typedef struct
{
    uint8_t extendedAddress[8];
} r_eap_eapm_network_leave_ind_t;

/* EAPM-NETWORK */
typedef struct
{
    uint8_t gmkId;
    uint8_t cmdId;             // 1:    Kick    2:  GMK transmission    3:GMK   activation
    uint8_t reqHandle;
    uint8_t extendedAddress[8];
} r_eap_eapm_network_req_t;

typedef struct
{
    r_eap_cb_status_t status;
    uint8_t           reqHandle;
    uint8_t           extendedAddress[8];
} r_eap_eapm_network_cnf_t;

/* EAPM-NEWDEVICE */
typedef struct
{
    uint8_t extendedAddress[8];
} r_eap_eapm_newdevice_ind_t;


/* EAPM-SET-CLIENTINFO */
typedef struct
{
    uint8_t          validFlg;
    uint8_t          extendedAddress[8];
    uint8_t          accessControl;
    uint8_t          networkAddress[2];
    uint8_t          PSK[16];
    uint8_t          wait_msg3;
} r_eap_eapm_set_clientinfo_req_t;

typedef struct
{
    r_eap_cb_status_t status;
    uint8_t           extendedAddress[8];
    uint16_t          attributeIndex;
} r_eap_eapm_set_clientinfo_cnf_t;


/* EAPM-EAP-KEY */
typedef struct
{
    uint8_t extendedAddress[8];
    uint8_t type;       /* 4:MSK */
    uint8_t length;
    uint8_t key[64];
} r_eap_eapm_eap_key_ind_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#endif /* R_EAP_SAP_H */

