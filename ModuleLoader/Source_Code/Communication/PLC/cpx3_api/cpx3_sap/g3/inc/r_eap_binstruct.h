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
 * File Name : r_eap_binstruct.h
 * Description : G3 EAP layer bin struct
 ******************************************************************************/

#ifndef R_EAP_BINSTRUCT_H
#define R_EAP_BINSTRUCT_H

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/*===========================================================================*/
/* For EAP SAP ID                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* For EAP SAP IF ENUM                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* For EAP SAP IF STRUCT                                                     */
/*===========================================================================*/
typedef r_eap_eapm_reset_cnf_t r_eap_eapm_reset_cnf_bin_t;


typedef struct
{
    uint8_t eibAttributeId;
    uint8_t binEibAttributeIndex[2];
} r_eap_eapm_get_req_bin_t;

typedef struct
{
    uint8_t status;
    uint8_t eibAttributeId;
    uint8_t binEibAttributeIndex[2];
    uint8_t binEibAttributeValue[R_EAP_MAX_IB_SIZE];
} r_eap_eapm_get_cnf_bin_t;

typedef struct
{
    uint8_t eibAttributeId;
    uint8_t binEibAttributeIndex[2];
    uint8_t binEibAttributeValue[R_EAP_MAX_IB_SIZE];
} r_eap_eapm_set_req_bin_t;

typedef struct
{
    uint8_t status;
    uint8_t eibAttributeId;
    uint8_t binEibAttributeIndex[2];
} r_eap_eapm_set_cnf_bin_t;

typedef r_eap_eapm_network_join_ind_t r_eap_eapm_nwk_join_ind_bin_t;

typedef r_eap_eapm_network_leave_ind_t r_eap_eapm_nwk_leave_ind_bin_t;

typedef r_eap_eapm_network_req_t r_eap_eapm_network_req_bin_t;

typedef r_eap_eapm_network_cnf_t r_eap_eapm_network_cnf_bin_t;

typedef struct
{
    uint8_t extendedAddress[8];
} r_eap_eapm_newdevice_ind_bin_t;

typedef r_eap_eapm_set_clientinfo_req_t r_eap_eapm_set_cinfo_req_bin_t;

typedef struct
{
    uint8_t status;
    uint8_t extendedAddress[8];
    uint8_t binAttributeIndex[2];
} r_eap_eapm_set_cinfo_cnf_bin_t;


/* EAPM-EAP-KEY */
typedef r_eap_eapm_eap_key_ind_t r_eap_eapm_eap_key_ind_bin_t;



/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#endif /* #ifndef R_EAP_BINSTRUCT_H */

