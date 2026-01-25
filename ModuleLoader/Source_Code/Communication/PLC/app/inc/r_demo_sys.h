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
* File Name   : r_demo_sys.h
*    @version
*        $Rev: 4495 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2019-04-19 16:12:48 +0900#$
* Description :
******************************************************************************/

#ifndef R_DEMO_SYS_H
#define R_DEMO_SYS_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_DEMO_SYS_STATUS_NOT_SET       (0xFFu)     /*!< Default value for confirm structures */
#define R_DEMO_SYS_STATUS_FAILED        (0xFEu)     /*!< Error Status when api call return failure */

#define R_DEMO_SYS_CB_FLAG_PING         (1 << 0)    /*!< Event flag for SYSTEM-PING.confirmation */
#define R_DEMO_SYS_CB_FLAG_VERSION      (1 << 1)    /*!< Event flag for SYSTEM-VERSION.confirmation */
#define R_DEMO_SYS_CB_FLAG_GET_INFO     (1 << 2)    /*!< Event flag for SYSTEM-GETINFO.confirmation */
#define R_DEMO_SYS_CB_FLAG_CLEAR_INFO   (1 << 3)    /*!< Event flag for SYSTEM-CLEARINFO.confirmation */
#define R_DEMO_SYS_CB_FLAG_SROM_READ    (1 << 4)    /*!< Event flag for SYSTEM-SROM-READ.confirmation */
#define R_DEMO_SYS_CB_FLAG_SROM_WRITE   (1 << 5)    /*!< Event flag for SYSTEM-SROM-WRITE.confirmation */
#define R_DEMO_SYS_CB_FLAG_SROM_ERASE   (1 << 6)    /*!< Event flag for SYSTEM-SROM-ERASE.confirmation */
#define R_DEMO_SYS_CB_FLAG_EVENT        (1 << 7)    /*!< Event flag for SYSTEM-EVENT.indication */
#define R_DEMO_SYS_CB_FLAG_REBOOT       (1 << 8)    /*!< Event flag for SYSTEM-REBOOT-REQUEST.indication */
#define R_DEMO_SYS_CB_FLAG_BOOTUP       (1 << 9)    /*!< Event flag for SYSTEM-BOOTUP.indication */

/******************************************************************************
Typedef definitions
******************************************************************************/

typedef struct
{
/*===========================================================================*/
/*    SYS callbacks                                                          */
/*===========================================================================*/

    r_sys_ping_cnf_t              pingCnf;
    r_sys_version_cnf_t           versionCnf;
    r_sys_clear_info_cnf_t        clearInfoCnf;
    r_sys_get_info_cnf_t          getInfoCnf;
    r_sys_memread_cnf_t           sromReadCnf;
    r_sys_memwrite_cnf_t          sromWriteCnf;
    r_sys_memerase_cnf_t          sromEraseCnf;
    r_sys_event_ind_t             eventInd;
    r_sys_rebootreq_ind_t         rebootReqInd;

    uint32_t cbBuffU32[R_DEMO_APP_STATS_BUFF_MAXLEN / 4];

} r_demo_sys_cb_str_t;

/******************************************************************************
Exported global variables
******************************************************************************/
extern uint32_t             g_demo_sys_cb;

/******************************************************************************
Functions prototype
******************************************************************************/

/*===========================================================================*/
/*    SYS APIs                                                            */
/*===========================================================================*/
r_result_t R_DEMO_InitSysCallBack (r_sys_callback_t * pCallBack);
r_result_t R_DEMO_AppSysMenu (void);
r_result_t R_DEMO_SysPing (r_sys_ping_cnf_t ** cnf);
r_result_t R_DEMO_SysVersion (r_sys_version_cnf_t ** cnf);
r_result_t R_DEMO_SysGetInfo (r_sys_get_info_req_t * req, r_sys_get_info_cnf_t ** cnf);
r_result_t R_DEMO_SysClearInfo (r_sys_clear_info_req_t * req, r_sys_clear_info_cnf_t ** cnf);
r_result_t R_DEMO_AppSysGetStatistics (r_sys_info_block_t block);
r_result_t R_DEMO_AppSysGetLog (r_sys_info_block_t block);
r_result_t R_DEMO_AppSysClearInfo (uint8_t type);
r_result_t R_DEMO_SysSromRead (r_sys_memread_req_t* req);
r_result_t R_DEMO_SysSromWrite (r_sys_memwrite_req_t* req);
r_result_t R_DEMO_SysSromErase (r_sys_memerase_req_t* req);


#endif /* R_DEMO_SYS_H */

