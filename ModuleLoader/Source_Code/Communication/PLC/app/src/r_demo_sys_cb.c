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
* File Name   : r_demo_sys_cb.c
*    @version
*        $Rev: 4402 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2019-02-25 16:15:15 +0900#$
* Description :
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/

/* common */
#include "r_typedefs.h"

/* synergy */

/* sys part */
#include "r_c3sap_api.h"

/* app part */
#include "r_demo_api.h"
#include "r_demo_app.h"
#include "r_demo_sys.h"
#include "r_demo_app_thread.h"
#include "r_demo_os_wrapper.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/
/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
Functions
******************************************************************************/
static void sys_ping_confirm (const r_sys_ping_cnf_t * confirm);
static void sys_version_confirm (const r_sys_version_cnf_t * confirm);
static void sys_clear_info_confirm (const r_sys_clear_info_cnf_t * confirm);
static void sys_get_info_confirm (const r_sys_get_info_cnf_t * confirm);
static void sys_event_indication (const r_sys_event_ind_t * indication);
static void sys_rebootreq_indication (const r_sys_rebootreq_ind_t * indication);

/******************************************************************************
Exported global variables
******************************************************************************/
extern volatile r_demo_sys_cb_str_t g_syscb;

/******************************************************************************
* Function Name: sys_ping_confirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void sys_ping_confirm (const r_sys_ping_cnf_t * confirm)
{
    g_syscb.pingCnf = *confirm;
    R_DEMO_SetEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_PING );
}
/******************************************************************************
   End of function  sys_ping_confirm
******************************************************************************/

/******************************************************************************
* Function Name: sys_version_confirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void sys_version_confirm (const r_sys_version_cnf_t * confirm)
{
    g_syscb.versionCnf = *confirm;
    R_DEMO_SetEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_VERSION );
}
/******************************************************************************
   End of function  sys_version_confirm
******************************************************************************/

/******************************************************************************
* Function Name: sys_clear_info_confirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void sys_clear_info_confirm (const r_sys_clear_info_cnf_t * confirm)
{
    g_syscb.clearInfoCnf = *confirm;
    R_DEMO_SetEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_CLEAR_INFO );
}
/******************************************************************************
   End of function  sys_clear_info_confirm
******************************************************************************/

/******************************************************************************
* Function Name: sys_get_info_confirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void sys_get_info_confirm (const r_sys_get_info_cnf_t * confirm)
{
    g_syscb.getInfoCnf = *confirm;
    g_syscb.getInfoCnf.pinfo = (uint32_t *)g_syscb.cbBuffU32;

    if (
        (confirm->length <= R_DEMO_APP_STATS_BUFF_MAXLEN) &&
        (NULL != confirm->pinfo)
        )
    {
        R_memcpy ((uint8_t *)g_syscb.getInfoCnf.pinfo, (const uint8_t *)confirm->pinfo, confirm->length);
    }
    else
    {
        g_syscb.getInfoCnf.status = R_DEMO_SYS_STATUS_FAILED;
    }

    R_DEMO_SetEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_GET_INFO );
}
/******************************************************************************
   End of function  sys_get_info_confirm
******************************************************************************/

/******************************************************************************
* Function Name: sys_srom_read_confirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void sys_srom_read_confirm (const r_sys_memread_cnf_t * confirm)
{
    g_syscb.sromReadCnf = *confirm;
    g_syscb.sromReadCnf.pdata = (uint8_t *)g_syscb.cbBuffU32;

    if (
        (confirm->length <= R_DEMO_APP_STATS_BUFF_MAXLEN) &&
        (NULL != confirm->pdata)
        )
    {
        R_memcpy ((uint8_t *)g_syscb.sromReadCnf.pdata, (const uint8_t *)confirm->pdata, confirm->length);
    }
    else
    {
        g_syscb.sromReadCnf.status = R_DEMO_SYS_STATUS_FAILED;
    }

    R_DEMO_SetEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_SROM_READ );
}
/******************************************************************************
   End of function  sys_srom_read_confirm
******************************************************************************/

/******************************************************************************
* Function Name: sys_srom_write_confirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void sys_srom_write_confirm (const r_sys_memwrite_cnf_t * confirm)
{
    g_syscb.sromWriteCnf = *confirm;
    R_DEMO_SetEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_SROM_WRITE );
}
/******************************************************************************
   End of function  sys_srom_write_confirm
******************************************************************************/

/******************************************************************************
* Function Name: sys_srom_erase_confirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void sys_srom_erase_confirm (const r_sys_memerase_cnf_t * confirm)
{
    g_syscb.sromEraseCnf = *confirm;
    R_DEMO_SetEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_SROM_ERASE );
}
/******************************************************************************
   End of function  sys_srom_erase_confirm
******************************************************************************/

/******************************************************************************
* Function Name: sys_event_indication
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void sys_event_indication (const r_sys_event_ind_t * indication)
{
    g_syscb.eventInd = *indication;
    R_DEMO_SetEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_EVENT );
}
/******************************************************************************
   End of function  sys_event_indication
******************************************************************************/


/******************************************************************************
* Function Name: sys_rebootreq_indication
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void sys_rebootreq_indication (const r_sys_rebootreq_ind_t * indication)
{
    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd ((const uint8_t *)indication,
                              R_DEMO_APP_HANDLE_REBOOT_REQUEST_IND,
                              sizeof (r_sys_rebootreq_ind_t));
    R_DEMO_SetEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_REBOOT );
}
/******************************************************************************
   End of function  sys_rebootreq_indication
******************************************************************************/



/******************************************************************************
* Function Name: R_DEMO_InitSysCallBack
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_InitSysCallBack (r_sys_callback_t * pCallBack)
{

    if (NULL == pCallBack)
    {
        return R_RESULT_ILLEGAL_NULL_POINTER;
    }

    R_memset ((uint8_t *)pCallBack, 0, sizeof (r_sys_callback_t));

    pCallBack->R_SYS_PingCnf                                       =   sys_ping_confirm;
    pCallBack->R_SYS_VersionCnf                                    =   sys_version_confirm;
    pCallBack->R_SYS_GetInfoCnf                                    =   sys_get_info_confirm;
    pCallBack->R_SYS_ClearInfoCnf                                  =   sys_clear_info_confirm;
    pCallBack->R_SYS_SromReadCnf                                   =   sys_srom_read_confirm;
    pCallBack->R_SYS_SromWriteCnf                                  =   sys_srom_write_confirm;
    pCallBack->R_SYS_SromEraseCnf                                  =   sys_srom_erase_confirm;
    pCallBack->R_SYS_EventInd                                      =   sys_event_indication;
    pCallBack->R_SYS_ReBootReqInd                                  =   sys_rebootreq_indication;

    return R_RESULT_SUCCESS;
} /* R_DEMO_InitSysCallBack */
/******************************************************************************
   End of function  R_DEMO_InitSysCallBack
******************************************************************************/
