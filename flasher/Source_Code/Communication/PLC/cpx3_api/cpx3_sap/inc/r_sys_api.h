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
 * File Name : r_sys_api.h
 * Description : SYSTEM API prototype
 ******************************************************************************/

#ifndef R_SYS_API_H
#define R_SYS_API_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_sys_sap.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/


typedef struct
{
    const uint8_t * p_fw_plc;                   /* Pointer to download Firmware */
    const uint8_t * p_fw_rf;                    /* Pointer to download Firmware for RF*/
    uint8_t         dl_baud;                    /* Baud rate ID for Firmware download  */
    uint8_t         cmd_baud;                   /* Baud rate ID for communicating with CPX */
    uint32_t        dl_timeout;                 /* Timeout for Firmware download (ms) */
} r_sys_boot_info_t;

/*******************************************************************************************************************//**
 * @addtogroup group_cpx_api_callbacks
 * @{
 **********************************************************************************************************************/

typedef struct
{
    void (* R_SYS_PingCnf)              (const r_sys_ping_cnf_t * pcnf);                /**< Callback function for confirmation SYSTEM-PING: Request of confirming operation condition of CPX3 */
    void (* R_SYS_VersionCnf)           (const r_sys_version_cnf_t * pcnf);             /**< Callback function for confirmation SYSTEM-VERSION: Request of getting version information of CPX3 and System block */
    void (* R_SYS_ClearInfoCnf)         (const r_sys_clear_info_cnf_t * pcnf);          /**< Callback function for confirmation SYSTEM-CLEARINFO: Request of clearing the statistics and the logs for System block (including UART-IF block) */
    void (* R_SYS_GetInfoCnf)           (const r_sys_get_info_cnf_t * pcnf);            /**< Callback function for confirmation SYSTEM-GETINFO: Request of getting the statistics and the logs for System block (including UART-IF block) */
    void (* R_SYS_SromReadCnf)          (const r_sys_memread_cnf_t * pcnf);
    void (* R_SYS_SromWriteCnf)         (const r_sys_memwrite_cnf_t * pcnf);
    void (* R_SYS_SromEraseCnf)         (const r_sys_memerase_cnf_t * pcnf);
#ifdef R_DEFINE_ENABLE_MEMRW
    void (* R_SYS_MemReadCnf)           (const r_sys_memread_cnf_t * pcnf);
#endif
    void (* R_SYS_EventInd)             (const r_sys_event_ind_t * pind);               /**< Callback function for indication SYSTEM-EVENT: Notification of CPX3 event */
    void (* R_SYS_ReBootReqInd)         (const r_sys_rebootreq_ind_t * pind);           /**< Callback function for indication SYSTEM-REBOOT-REQUEST: Notification of fatal state in CPX3 */
    void (* R_SYS_BootUpInd)            (void);                                         /**< Callback function for Indication SYSTEM-BOOTUP:  */
} r_sys_callback_t;


/*******************************************************************************************************************//**
 * @} (end addtogroup group_cpx_api_callbacks)
 **********************************************************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/*===========================================================================*/
/* SYS SAP Function prototype                                                */
/*===========================================================================*/
r_result_t R_SYS_Init (r_sys_boot_info_t * pbootInfo, r_sys_callback_t * pcallbacks);
r_result_t R_SYS_Deinit (void);
r_result_t R_SYS_PingReq (void);
r_result_t R_SYS_VersionReq (void);
r_result_t R_SYS_ResetReq (r_sys_reset_req_t * preq);
r_result_t R_SYS_ClearInfoReq (r_sys_clear_info_req_t * preq);
r_result_t R_SYS_GetInfoReq (r_sys_get_info_req_t * preq);

r_result_t R_SYS_SromReadReq (r_sys_memread_req_t * preq);
r_result_t R_SYS_SromWriteReq (r_sys_memwrite_req_t * preq);
r_result_t R_SYS_SromEraseReq (r_sys_memerase_req_t * preq);
#ifdef R_DEFINE_ENABLE_MEMRW
r_result_t R_SYS_MemReadReq (r_sys_memread_req_t * preq);
#endif
#endif /* R_G3_API_H */

