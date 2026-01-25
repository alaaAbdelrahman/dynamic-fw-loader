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
 * File Name : r_sys_sap.h
 * Description : System Command Struct
 ******************************************************************************/

#ifndef R_SYS_SAP_H
#define R_SYS_SAP_H

/******************************************************************************
Macro definitions
******************************************************************************/

/* INFO Type Bit */
#define R_SYS_INFO_TYPE_BIT_STATS  (0x01u)
#define R_SYS_INFO_TYPE_BIT_LOGS   (0x02u)

/* Version Request Standard */
#define R_SYS_VERSION_STANDARD_G3           (0x01u)
#define R_SYS_VERSION_STANDARD_PRIME        (0x02u)
#define R_SYS_VERSION_STANDARD_RF           (0x04u)
#define R_SYS_VERSION_STANDARD_MM           (0x08u)
#define R_SYS_VERSION_STANDARD_TURBO        (0x80u)

/* Version Request Band Plan */
#define R_SYS_VERSION_BAND_PLAN_CENELEC_A   (0x0001u)
#define R_SYS_VERSION_BAND_PLAN_CENELEC_B   (0x0002u)
#define R_SYS_VERSION_BAND_PLAN_ARIB        (0x0004u)
#define R_SYS_VERSION_BAND_PLAN_FCC         (0x0008u)
#define R_SYS_VERSION_BAND_PLAN_CENELEC_C   (0x0010u)
#define R_SYS_VERSION_BAND_PLAN_PRIME_V136  (0x0100u)
#define R_SYS_VERSION_BAND_PLAN_PRIME_V14   (0x0200u)

/******************************************************************************
Typedef definitions
******************************************************************************/

/* System Block Status Code */
typedef enum
{
    R_SYS_STATUS_SUCCESS,
    R_SYS_STATUS_FAILURE,
    R_SYS_STATUS_INVALID_STATUS,
    R_SYS_STATUS_INVALID_PARAMETER,
    R_SYS_STATUS_TIMEOUT,
    R_SYS_STATUS_INSUFFICIENT_MEMSIZE,
    R_SYS_STATUS_NOT_SUPPORTED,
    R_SYS_STATUS_UART_SYNC_BREAK,
    R_SYS_STATUS_DRV_ERROR,
    R_SYS_STATUS_OUT_OF_CAPS,
    R_SYS_STATUS_CRC_ERROR,
    R_SYS_STATUS_INVALID_RESPONSE,
    R_SYS_STATUS_SEC_ERROR,
    R_SYS_STATUS_FATAL_ERROR,
    R_SYS_STATUS_INVALID_HYB_FIRMWARE
} r_sys_status_t;

/* INFO Type */
typedef enum
{
    R_SYS_INFO_TYPE_STATS,
    R_SYS_INFO_TYPE_LOGS,
    R_SYS_INFO_TYPE_MAX
} r_sys_info_type_t;

/* INFO Block */
typedef enum
{
    R_SYS_INFO_BLOCK_SYSTEM,
    R_SYS_INFO_BLOCK_UARTIF_0,
    R_SYS_INFO_BLOCK_UARTIF_1,
    R_SYS_INFO_BLOCK_MAX
} r_sys_info_block_t;


/* System reset request */
typedef struct
{
    uint8_t  mode;
    uint16_t interval;
} r_sys_reset_req_t;

/* Clear system info request */
typedef struct
{
    uint8_t infoTypeBit;
    uint8_t infoBlockBit;
} r_sys_clear_info_req_t;

/* Get info request */
typedef struct
{
    uint8_t infoType;
    uint8_t infoBlock;
} r_sys_get_info_req_t;



/* SROM read request */
typedef struct
{
    uint32_t addr;
    uint16_t length;
    uint8_t  accessType;
} r_sys_memread_req_t;

/* SROM write request */
typedef struct
{
    uint32_t  addr;
    uint16_t  length;
    uint8_t   accessType;
    uint8_t * pdata;
} r_sys_memwrite_req_t;

/* SROM erase request */
typedef struct
{
    uint32_t addr;
    uint32_t length;
} r_sys_memerase_req_t;

/* Ping confirm */
typedef struct
{
    uint8_t status;
} r_sys_ping_cnf_t;

/* Version info */
typedef struct
{
    uint8_t  pfVer;
    uint8_t  standard;
    uint16_t bandPlan;
    uint16_t sysVer;
    uint32_t hwVer;
} r_sys_version_cnf_t;

/* Clear system info confirm */
typedef struct
{
    uint8_t status;
} r_sys_clear_info_cnf_t;

/* Get system info confirm */
typedef struct
{
    uint8_t    status;
    uint8_t    infoType;
    uint8_t    infoBlock;
    uint16_t   length;
    uint32_t * pinfo;
} r_sys_get_info_cnf_t;



/* SROM read confirm */
typedef struct
{
    uint8_t   status;
    uint32_t  addr;
    uint16_t  length;
    uint8_t   accessType;
    uint8_t * pdata;
} r_sys_memread_cnf_t;

/* SROM write confirm */
typedef struct
{
    uint8_t  status;
    uint32_t addr;
    uint16_t length;
    uint8_t  accessType;
} r_sys_memwrite_cnf_t;

/* SROM erase confirm */
typedef struct
{
    uint8_t  status;
    uint32_t addr;
    uint32_t length;
} r_sys_memerase_cnf_t;

/* indication from system */
typedef struct
{
    uint8_t cause;
} r_sys_event_ind_t;

/* Indication of reboot request */
typedef struct
{
    uint8_t status;
} r_sys_rebootreq_ind_t;

typedef r_sys_status_t                  e_sys_status_t;
typedef r_sys_info_block_t              e_sys_info_block_t;
typedef r_sys_ping_cnf_t                st_sys_ping_cnf_t;
typedef r_sys_version_cnf_t             st_sys_version_cnf_t;
typedef r_sys_reset_req_t               st_sys_reset_req_t;
typedef r_sys_clear_info_req_t          st_sys_clear_info_req_t;
typedef r_sys_clear_info_cnf_t          st_sys_clear_info_cnf_t;
typedef r_sys_get_info_req_t            st_sys_get_info_req_t;
typedef r_sys_get_info_cnf_t            st_sys_get_info_cnf_t;
typedef r_sys_memread_req_t             st_sys_mem_read_req_t;
typedef r_sys_memread_cnf_t             st_sys_mem_read_cnf_t;
typedef r_sys_memwrite_req_t            st_sys_mem_write_req_t;
typedef r_sys_memwrite_cnf_t            st_sys_mem_write_cnf_t;
typedef r_sys_memerase_req_t            st_sys_mem_erase_req_t;
typedef r_sys_memerase_cnf_t            st_sys_mem_erase_cnf_t;
typedef r_sys_event_ind_t               st_sys_event_ind_t;
typedef r_sys_rebootreq_ind_t           st_sys_reboot_request_ind_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

#endif /* R_SYS_SAP_H */

