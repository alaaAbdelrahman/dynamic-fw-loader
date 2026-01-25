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
 * File Name : r_rflmac_binstruct.h
 * Description : RF LMAC layer bin struct
 ******************************************************************************/

#ifndef R_RFLMAC_BINSTRUCT_H
#define R_RFLMAC_BINSTRUCT_H

#include "r_rflmac_sap.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_RFLMAC_AVOID_ERROR  (4u)

/******************************************************************************
Typedef definitions
******************************************************************************/
/* RF-LML-RESET */
typedef r_rflmac_lml_reset_cnf_t r_rflmac_lml_reset_cnf_bin_t;

/* RF-LML-GET */
typedef struct
{
    uint8_t binParamId[2];
} r_rflmac_lml_get_req_bin_t;

typedef struct
{
    uint8_t binStatus;
    uint8_t binParamId[2];
    uint8_t binContent[R_RFLMAC_MAX_IB_SIZE];
} r_rflmac_lml_get_cnf_bin_t;

/* RF-LML-SET */
typedef struct
{
    uint8_t binParamId[2];
    uint8_t binContent[R_RFLMAC_MAX_IB_SIZE];
} r_rflmac_lml_set_req_bin_t;

typedef struct
{
    uint8_t binStatus;
    uint8_t binParamId[2];
} r_rflmac_lml_set_cnf_bin_t;

/* RF-LML-DATA */
typedef struct
{
    uint8_t binOptions;
    uint8_t binTime[4];
    uint8_t binPsduLen[2];
    uint8_t binPsdu[R_RFLMAC_AVOID_ERROR];
} r_rflmac_lml_data_req_bin_t;

typedef r_rflmac_lml_data_cnf_t r_rflmac_lml_data_cnf_bin_t;

typedef struct
{
    uint8_t binRxTime[4];
    uint8_t binLqi;
    uint8_t binCrc;
    uint8_t binPhr;
    uint8_t binStatus;
    uint8_t binRssi[2];
    uint8_t binPsduLen[2];
    uint8_t binCurrentChannel;
    uint8_t binPsdu[R_RFLMAC_AVOID_ERROR];
} r_rflmac_lml_data_ind_bin_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

#endif /* R_RFLMAC_BINSTRUCT_H */

