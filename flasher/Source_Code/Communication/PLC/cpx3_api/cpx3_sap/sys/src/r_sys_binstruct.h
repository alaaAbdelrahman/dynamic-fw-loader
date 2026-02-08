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
 * File Name : r_sys_binstruct.h
 * Description : System Command Struct for serial command
 ******************************************************************************/

#ifndef R_SYS_BINSTRUCT_H
#define R_SYS_BINSTRUCT_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_SYS_GETINFO_LEN_MAX   (1u)     /* byte */
#define R_SYS_MEMREAD_LEN_MAX   (1u)     /* byte */
#define R_SYS_MEMWRITE_LEN_MAX  (1u)     /* byte */

/******************************************************************************
Typedef definitions
******************************************************************************/

/* Request */
typedef struct
{
    uint8_t mode;
    uint8_t interval[2];
} r_sys_reset_req_bin_t;

typedef struct
{
    uint8_t infoTypeBit;
    uint8_t infoBlockBit;
} r_sys_clear_info_req_bin_t;

typedef struct
{
    uint8_t infoType;
    uint8_t infoBlock;
} r_sys_get_info_req_bin_t;


typedef struct
{
    uint8_t addr[4];
    uint8_t length[2];
    uint8_t accessType;
} r_sys_memread_req_bin_t;

typedef struct
{
    uint8_t addr[4];
    uint8_t length[2];
    uint8_t accessType;
    uint8_t data[R_SYS_MEMREAD_LEN_MAX];
} r_sys_memwrite_req_bin_t;

typedef struct
{
    uint8_t addr[4];
    uint8_t length[4];
} r_sys_memerase_req_bin_t;

/* Confirm */
typedef struct
{
    uint8_t status;
} r_sys_ping_cnf_bin_t;

typedef struct
{
    uint8_t pfVer;
    uint8_t standard;
    uint8_t bandPlan[2];
    uint8_t sysVer[2];
    uint8_t hwVer[4];
} r_sys_version_cnf_bin_t;

typedef struct
{
    uint8_t status;
} r_sys_clear_info_cnf_bin_t;

typedef struct
{
    uint8_t status;
    uint8_t infoType;
    uint8_t infoBlock;
    uint8_t length[2];
    uint8_t info[R_SYS_GETINFO_LEN_MAX];
} r_sys_get_info_cnf_bin_t;

typedef struct
{
    uint8_t status;
    uint8_t addr[4];
    uint8_t length[2];
    uint8_t accessType;
    uint8_t data[R_SYS_MEMREAD_LEN_MAX];
} r_sys_memread_cnf_bin_t;

typedef struct
{
    uint8_t status;
    uint8_t addr[4];
    uint8_t length[2];
    uint8_t accessType;
} r_sys_memwrite_cnf_bin_t;

typedef struct
{
    uint8_t status;
    uint8_t addr[4];
    uint8_t length[4];
} r_sys_memerase_cnf_bin_t;


/* Indication */
typedef struct
{
    uint8_t cause;
} r_sys_event_ind_bin_t;

typedef struct
{
    uint8_t status;
} r_sys_rebootreq_ind_bin_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

#endif /* R_SYS_BINSTRUCT_H */

