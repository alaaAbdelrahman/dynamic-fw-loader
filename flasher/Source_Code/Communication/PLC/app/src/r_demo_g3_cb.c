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
* File Name   : r_demo_g3_cb.c
*    @version
*        $Rev: 11188 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2022-07-25 16:50:19 +0900#$
* Description :
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/

/* common */
#include "r_typedefs.h"

/* g3 part */
#include "r_c3sap_api.h"


/* app part */
#include "r_demo_api.h"
#include "r_demo_app.h"
#include "r_demo_app_thread.h"
#include "r_demo_os_wrapper.h"
/******************************************************************************
Macro definitions
******************************************************************************/
/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Exported global variables
******************************************************************************/
extern r_demo_g3_cb_str_t g_g3cb[R_G3_CH_MAX];
//extern uint8_t            g_demo_dump_segment[];


/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/
/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
Functions
******************************************************************************/



/******************************************************************************
* Function Name: R_G3_SetConfigConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_SetConfigConfirm (uint8_t chId, const r_g3_set_config_cnf_t * confirm)
{
    g_g3cb[chId].setConfig = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_SET_CONFIG );
}
/******************************************************************************
   End of function  R_G3_SetConfigConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_SetConfigCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_SetConfigCnf0 (const r_g3_set_config_cnf_t * confirm)
{
    R_G3_SetConfigConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3_SetConfigCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_SetConfigCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_SetConfigCnf1 (const r_g3_set_config_cnf_t * confirm)
{
    R_G3_SetConfigConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3_SetConfigCnf1
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_GetConfigConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_GetConfigConfirm (uint8_t chId, const r_g3_get_config_cnf_t * confirm)
{
    g_g3cb[chId].getConfig = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_GET_CONFIG );
}
/******************************************************************************
   End of function  R_G3_GetConfigConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_GetConfigCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_GetConfigCnf0 (const r_g3_get_config_cnf_t * confirm)
{
    R_G3_GetConfigConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3_GetConfigCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_GetConfigCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_GetConfigCnf1 (const r_g3_get_config_cnf_t * confirm)
{
    R_G3_GetConfigConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3_GetConfigCnf1
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_GetInfoConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_GetInfoConfirm (uint8_t chId, const r_g3_get_info_cnf_t * confirm)
{
    g_g3cb[chId].getInfo = *confirm;
    g_g3cb[chId].getInfo.pInfo = g_g3cb[chId].cbBuffU32;

    if (
        (confirm->length <= R_DEMO_APP_STATS_BUFF_MAXLEN) &&
        (NULL != confirm->pInfo)
        )
    {
        R_memcpy ((uint8_t *)g_g3cb[chId].getInfo.pInfo, (const uint8_t *)confirm->pInfo, confirm->length);
    }
    else
    {
        g_g3cb[chId].getInfo.status = R_DEMO_G3_STATUS_FAILED;
    }

    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_GET_INFO );

} /* R_G3_GetInfoConfirm */
/******************************************************************************
   End of function  R_G3_GetInfoConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_GetInfoCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_GetInfoCnf0 (const r_g3_get_info_cnf_t * confirm)
{
    R_G3_GetInfoConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3_GetInfoCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_GetInfoCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_GetInfoCnf1 (const r_g3_get_info_cnf_t * confirm)
{
    R_G3_GetInfoConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3_GetInfoCnf1
******************************************************************************/



/******************************************************************************
* Function Name: R_G3_ClearInfoConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_ClearInfoConfirm (uint8_t chId, const r_g3_clear_info_cnf_t * confirm)
{
    g_g3cb[chId].clrInfo = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_CLEAR_INFO );
}
/******************************************************************************
   End of function  R_G3_ClearInfoConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_ClearInfoCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_ClearInfoCnf0 (const r_g3_clear_info_cnf_t * confirm)
{
    R_G3_ClearInfoConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3_ClearInfoCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_ClearInfoCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_ClearInfoCnf1 (const r_g3_clear_info_cnf_t * confirm)
{
    R_G3_ClearInfoConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3_ClearInfoCnf1
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_DumpConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_DumpConfirm (uint8_t chId, const r_g3_dump_cnf_t * confirm)
{
    g_g3cb[chId].dumpCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_DUMP );
}
/******************************************************************************
   End of function  R_G3_DumpConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_DumpCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_DumpCnf0 (const r_g3_dump_cnf_t * confirm)
{
    R_G3_DumpConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3_DumpCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_DumpCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_DumpCnf1 (const r_g3_dump_cnf_t * confirm)
{
    R_G3_DumpConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3_DumpCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_G3_DumpAbortConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_DumpAbortConfirm (uint8_t chId, const r_g3_dump_abort_cnf_t * confirm)
{
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_DUMP_ABORT );
}
/******************************************************************************
   End of function  R_G3_DumpAbortConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_DumpAbortCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_DumpAbortCnf0 (const r_g3_dump_abort_cnf_t * confirm)
{
    R_G3_DumpAbortConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3_DumpAbortCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_DumpAbortCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_DumpAbortCnf1 (const r_g3_dump_abort_cnf_t * confirm)
{
    R_G3_DumpAbortConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3_DumpAbortCnf1
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_EventIndication
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_EventIndication (uint8_t chId, const r_g3_event_ind_t * indication)
{
    g_g3cb[chId].eventInd = *indication;
}
/******************************************************************************
   End of function  R_G3_EventIndication
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_EventInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_EventInd0 (const r_g3_event_ind_t * indication)
{
    R_G3_EventIndication (0, indication);
}
/******************************************************************************
   End of function  R_G3_EventInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_EventInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_EventInd1 (const r_g3_event_ind_t * indication)
{
    R_G3_EventIndication (1, indication);
}
/******************************************************************************
   End of function  R_G3_EventInd1
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_DumpIndication
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_DumpIndication (uint8_t chId, const r_g3_dump_ind_t * ind)
{
    g_g3cb[chId].dumpInd = *ind;
    //R_memcpy(g_demo_dump_segment, ind->pDmpData, ind->segmentLen);
    //g_g3cb[chId].dumpInd.pDmpData = g_demo_dump_segment;
    
}
/******************************************************************************
   End of function  R_G3_DumpIndication
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_DumpInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_DumpInd0 (const r_g3_dump_ind_t * indication)
{
    R_G3_DumpIndication (0, indication);
}
/******************************************************************************
   End of function  R_G3_DumpInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3_DumpInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3_DumpInd1 (const r_g3_dump_ind_t * indication)
{
    R_G3_DumpIndication (1, indication);
}
/******************************************************************************
   End of function  R_G3_DumpInd1
******************************************************************************/




/*===========================================================================*/
/*    G3MAC callbacks                                                        */
/*===========================================================================*/
#ifndef R_DEFINE_DISABLE_UMAC
/******************************************************************************
* Function Name: R_G3MAC_McpsDataConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_McpsDataConfirm (uint8_t chId, const r_g3mac_mcps_data_cnf_t * confirm)
{
    g_g3cb[chId].mcpsDataCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_MCPS_DATA );
}
/******************************************************************************
   End of function  R_G3MAC_McpsDataConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_McpsDataCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_McpsDataCnf0 (const r_g3mac_mcps_data_cnf_t * confirm)
{
    R_G3MAC_McpsDataConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3MAC_McpsDataCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_McpsDataCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_McpsDataCnf1 (const r_g3mac_mcps_data_cnf_t * confirm)
{
    R_G3MAC_McpsDataConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3MAC_McpsDataCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_G3MAC_MlmeResetConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeResetConfirm (uint8_t chId, const r_g3mac_mlme_reset_cnf_t * confirm)
{
    g_g3cb[chId].mlmeResetCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_MLME_RESET );
}
/******************************************************************************
   End of function  R_G3MAC_MlmeResetConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeResetCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeResetCnf0 (const r_g3mac_mlme_reset_cnf_t * confirm)
{
    R_G3MAC_MlmeResetConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeResetCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeResetCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeResetCnf1 (const r_g3mac_mlme_reset_cnf_t * confirm)
{
    R_G3MAC_MlmeResetConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeResetCnf1
******************************************************************************/
#endif

/******************************************************************************
* Function Name: R_G3MAC_MlmeSetConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeSetConfirm (uint8_t chId, const r_g3mac_mlme_set_cnf_t * confirm)
{
    g_g3cb[chId].mlmeSetCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_MLME_SET );
}
/******************************************************************************
   End of function  R_G3MAC_MlmeSetConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeSetCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeSetCnf0 (const r_g3mac_mlme_set_cnf_t * confirm)
{
    R_G3MAC_MlmeSetConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeSetCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeSetCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeSetCnf1 (const r_g3mac_mlme_set_cnf_t * confirm)
{
    R_G3MAC_MlmeSetConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeSetCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_G3MAC_MlmeGetConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeGetConfirm (uint8_t chId, const r_g3mac_mlme_get_cnf_t * confirm)
{
    g_g3cb[chId].mlmeGetCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_MLME_GET );
}
/******************************************************************************
   End of function  R_G3MAC_MlmeGetConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeGetCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeGetCnf0 (const r_g3mac_mlme_get_cnf_t * confirm)
{
    R_G3MAC_MlmeGetConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeGetCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeGetCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeGetCnf1 (const r_g3mac_mlme_get_cnf_t * confirm)
{
    R_G3MAC_MlmeGetConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeGetCnf1
******************************************************************************/


#ifndef R_DEFINE_DISABLE_UMAC
/******************************************************************************
* Function Name: R_G3MAC_MlmeScanConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeScanConfirm (uint8_t chId, const r_g3mac_mlme_scan_cnf_t * confirm)
{
    g_g3cb[chId].mlmeScanCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_MLME_SCAN );
}
/******************************************************************************
   End of function  R_G3MAC_MlmeScanConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeScanCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeScanCnf0 (const r_g3mac_mlme_scan_cnf_t * confirm)
{
    R_G3MAC_MlmeScanConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeScanCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeScanCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeScanCnf1 (const r_g3mac_mlme_scan_cnf_t * confirm)
{
    R_G3MAC_MlmeScanConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeScanCnf1
******************************************************************************/



/******************************************************************************
* Function Name: R_G3MAC_MlmeStartConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeStartConfirm (uint8_t chId, const r_g3mac_mlme_start_cnf_t * confirm)
{
    g_g3cb[chId].mlmeStartCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_MLME_START );
}
/******************************************************************************
   End of function  R_G3MAC_MlmeStartConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeStartCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeStartCnf0 (const r_g3mac_mlme_start_cnf_t * confirm)
{
    R_G3MAC_MlmeStartConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeStartCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeStartCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeStartCnf1 (const r_g3mac_mlme_start_cnf_t * confirm)
{
    R_G3MAC_MlmeStartConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeStartCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_G3MAC_McpsDataIndication
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_McpsDataIndication (uint8_t chId, const r_g3mac_mcps_data_ind_t * ind)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueMacDataInd (ind); // use single only
}
/******************************************************************************
   End of function  R_G3MAC_McpsDataIndication
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_McpsDataInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_McpsDataInd0 (const r_g3mac_mcps_data_ind_t * indication)
{
    R_G3MAC_McpsDataIndication (0, indication);
}
/******************************************************************************
   End of function  R_G3MAC_McpsDataInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_McpsDataInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_McpsDataInd1 (const r_g3mac_mcps_data_ind_t * indication)
{
    R_G3MAC_McpsDataIndication (1, indication);
}
/******************************************************************************
   End of function  R_G3MAC_McpsDataInd1
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeBeaconNotifyInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeBeaconNotifyInd (uint8_t chId, const r_g3mac_mlme_bcn_notify_ind_t * indication)
{
    g_g3cb[chId].mlmeBeaconNotifyInd = *indication;
}
/******************************************************************************
   End of function  R_G3MAC_MlmeBeaconNotifyInd
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeBeaconNotifyInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeBeaconNotifyInd0 (const r_g3mac_mlme_bcn_notify_ind_t * indication)
{
    R_G3MAC_MlmeBeaconNotifyInd (0, indication);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeBeaconNotifyInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeBeaconNotifyInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeBeaconNotifyInd1 (const r_g3mac_mlme_bcn_notify_ind_t * indication)
{
    R_G3MAC_MlmeBeaconNotifyInd (1, indication);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeBeaconNotifyInd1
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeCommStatusInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeCommStatusInd (uint8_t chId, const r_g3mac_mlme_comm_status_ind_t * indication)
{
    g_g3cb[chId].mlmeCommStatusInd = *indication;
}
/******************************************************************************
   End of function  R_G3MAC_MlmeCommStatusInd
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeCommStatusInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeCommStatusInd0 (const r_g3mac_mlme_comm_status_ind_t * indication)
{
    R_G3MAC_MlmeCommStatusInd (0, indication);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeCommStatusInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeCommStatusInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeCommStatusInd1 (const r_g3mac_mlme_comm_status_ind_t * indication)
{
    R_G3MAC_MlmeCommStatusInd (1, indication);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeCommStatusInd1
******************************************************************************/


/******************************************************************************
* Function Name: R_G3MAC_MlmeFrameCountInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeFrameCountInd (uint8_t chId, const r_g3mac_mlme_framecount_ind_t * indication)
{
    g_g3cb[chId].mlmeFrameCountInd = *indication;
}
/******************************************************************************
   End of function  R_G3MAC_MlmeFrameCountInd
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeFrameCountInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeFrameCountInd0 (const r_g3mac_mlme_framecount_ind_t * indication)
{
    R_G3MAC_MlmeFrameCountInd (0, indication);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeFrameCountInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeFrameCountInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeFrameCountInd1 (const r_g3mac_mlme_framecount_ind_t * indication)
{
    R_G3MAC_MlmeFrameCountInd (1, indication);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeFrameCountInd1
******************************************************************************/
#endif

/******************************************************************************
* Function Name: R_G3MAC_MlmeTmrReceiveInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeTmrReceiveInd (uint8_t chId, const r_g3mac_mlme_tmr_receive_ind_t * indication)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd ((const uint8_t *)indication,
                              R_DEMO_APP_HANDLE_MAC_TMR_RCV_IND,
                              sizeof (r_g3mac_mlme_tmr_receive_ind_t));
}
/******************************************************************************
   End of function  R_G3MAC_MlmeTmrReceiveInd
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeTmrReceiveInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeTmrReceiveInd0 (const r_g3mac_mlme_tmr_receive_ind_t * indication)
{
    R_G3MAC_MlmeTmrReceiveInd (0, indication);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeTmrReceiveInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeTmrReceiveInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeTmrReceiveInd1 (const r_g3mac_mlme_tmr_receive_ind_t * indication)
{
    R_G3MAC_MlmeTmrReceiveInd (1, indication);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeTmrReceiveInd1
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeTmrTransmitInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeTmrTransmitInd (uint8_t chId, const r_g3mac_mlme_tmr_transmit_ind_t * indication)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd ((const uint8_t *)indication,
                              R_DEMO_APP_HANDLE_MAC_TMR_TRANSMIT_IND,
                              sizeof (r_g3mac_mlme_tmr_transmit_ind_t));
}
/******************************************************************************
   End of function  R_G3MAC_MlmeTmrTransmitInd
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeTmrTransmitInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeTmrTransmitInd0 (const r_g3mac_mlme_tmr_transmit_ind_t * indication)
{
    R_G3MAC_MlmeTmrTransmitInd (0, indication);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeTmrTransmitInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3MAC_MlmeTmrTransmitInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3MAC_MlmeTmrTransmitInd1 (const r_g3mac_mlme_tmr_transmit_ind_t * indication)
{
    R_G3MAC_MlmeTmrTransmitInd (1, indication);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeTmrTransmitInd1
******************************************************************************/

/*===========================================================================*/
/*    ADP callbacks                                                        */
/*===========================================================================*/


/******************************************************************************
* Function Name: R_ADP_AdpdDataConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpdDataConfirm (uint8_t chId, const r_adp_adpd_data_cnf_t * confirm)
{
    g_g3cb[chId].adpdDataCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_ADPD_DATA );

}
/******************************************************************************
   End of function  R_ADP_AdpdDataConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpdDataCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpdDataCnf0 (const r_adp_adpd_data_cnf_t * confirm)
{
    R_ADP_AdpdDataConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpdDataCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpdDataCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpdDataCnf1 (const r_adp_adpd_data_cnf_t * confirm)
{
    R_ADP_AdpdDataConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpdDataCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_ADP_AdpmResetConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmResetConfirm (uint8_t chId, const r_adp_adpm_reset_cnf_t * confirm)
{
    g_g3cb[chId].adpmResetCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_ADPM_RESET );
}
/******************************************************************************
   End of function  R_ADP_AdpmResetConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmResetCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmResetCnf0 (const r_adp_adpm_reset_cnf_t * confirm)
{
    R_ADP_AdpmResetConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmResetCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmResetCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmResetCnf1 (const r_adp_adpm_reset_cnf_t * confirm)
{
    R_ADP_AdpmResetConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmResetCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_ADP_AdpmDiscoveryConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmDiscoveryConfirm (uint8_t chId, const r_adp_adpm_discovery_cnf_t * confirm)
{
    g_g3cb[chId].adpmDiscoveryCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_ADPM_DISCOVERY );
}
/******************************************************************************
   End of function  R_ADP_AdpmDiscoveryConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmDiscoveryCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmDiscoveryCnf0 (const r_adp_adpm_discovery_cnf_t * confirm)
{
    R_ADP_AdpmDiscoveryConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmDiscoveryCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmDiscoveryCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmDiscoveryCnf1 (const r_adp_adpm_discovery_cnf_t * confirm)
{
    R_ADP_AdpmDiscoveryConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmDiscoveryCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_ADP_AdpmNetworkStartConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmNetworkStartConfirm (uint8_t chId, const r_adp_adpm_network_start_cnf_t * confirm)
{
    g_g3cb[chId].adpmNetworkStartCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_ADPM_START );
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkStartConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmNetworkStartCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmNetworkStartCnf0 (const r_adp_adpm_network_start_cnf_t * confirm)
{
    R_ADP_AdpmNetworkStartConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkStartCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmNetworkStartCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmNetworkStartCnf1 (const r_adp_adpm_network_start_cnf_t * confirm)
{
    R_ADP_AdpmNetworkStartConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkStartCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_ADP_AdpmNetworkJoinConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmNetworkJoinConfirm (uint8_t chId, const r_adp_adpm_network_join_cnf_t * confirm)
{
    g_g3cb[chId].adpmNetworkJoinCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_ADPM_JOIN );
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkJoinConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmNetworkJoinCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmNetworkJoinCnf0 (const r_adp_adpm_network_join_cnf_t * confirm)
{
    R_ADP_AdpmNetworkJoinConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkJoinCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmNetworkJoinCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmNetworkJoinCnf1 (const r_adp_adpm_network_join_cnf_t * confirm)
{
    R_ADP_AdpmNetworkJoinConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkJoinCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_ADP_AdpmNetworkLeaveConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmNetworkLeaveConfirm (uint8_t chId, const r_adp_adpm_network_leave_cnf_t * confirm)
{
    g_g3cb[chId].adpmNetworkLeaveCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_ADPM_LEAVE );
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkLeaveConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmNetworkLeaveCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmNetworkLeaveCnf0 (const r_adp_adpm_network_leave_cnf_t * confirm)
{
    R_ADP_AdpmNetworkLeaveConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkLeaveCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmNetworkLeaveCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmNetworkLeaveCnf1 (const r_adp_adpm_network_leave_cnf_t * confirm)
{
    R_ADP_AdpmNetworkLeaveConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkLeaveCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_ADP_AdpmGetConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmGetConfirm (uint8_t chId, const r_adp_adpm_get_cnf_t * confirm)
{
    g_g3cb[chId].adpmGetCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_ADPM_GET );
    
}
/******************************************************************************
   End of function  R_ADP_AdpmGetConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmGetCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmGetCnf0 (const r_adp_adpm_get_cnf_t * confirm)
{
    R_ADP_AdpmGetConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmGetCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmGetCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmGetCnf1 (const r_adp_adpm_get_cnf_t * confirm)
{
    R_ADP_AdpmGetConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmGetCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_ADP_AdpmSetConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmSetConfirm (uint8_t chId, const r_adp_adpm_set_cnf_t * confirm)
{
    g_g3cb[chId].adpmSetCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_ADPM_SET );

}
/******************************************************************************
   End of function  R_ADP_AdpmSetConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmSetCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmSetCnf0 (const r_adp_adpm_set_cnf_t * confirm)
{
    R_ADP_AdpmSetConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmSetCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmSetCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmSetCnf1 (const r_adp_adpm_set_cnf_t * confirm)
{
    R_ADP_AdpmSetConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmSetCnf1
******************************************************************************/



/******************************************************************************
* Function Name: R_ADP_AdpmRouteDiscoveryConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmRouteDiscoveryConfirm (uint8_t chId, const r_adp_adpm_route_disc_cnf_t * confirm)
{
    g_g3cb[chId].adpmRouteDiscoveryCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_ADPM_ROUTE );
}
/******************************************************************************
   End of function  R_ADP_AdpmRouteDiscoveryConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmRouteDiscoveryCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmRouteDiscoveryCnf0 (const r_adp_adpm_route_disc_cnf_t * confirm)
{
    R_ADP_AdpmRouteDiscoveryConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmRouteDiscoveryCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmRouteDiscoveryCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmRouteDiscoveryCnf1 (const r_adp_adpm_route_disc_cnf_t * confirm)
{
    R_ADP_AdpmRouteDiscoveryConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmRouteDiscoveryCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_ADP_AdpmPathDiscoveryConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmPathDiscoveryConfirm (uint8_t chId, const r_adp_adpm_path_discovery_cnf_t * confirm)
{
    g_g3cb[chId].adpmPathDiscoveryCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_ADPM_PATH );
}
/******************************************************************************
   End of function  R_ADP_AdpmPathDiscoveryConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmPathDiscoveryCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmPathDiscoveryCnf0 (const r_adp_adpm_path_discovery_cnf_t * confirm)
{
    R_ADP_AdpmPathDiscoveryConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmPathDiscoveryCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmPathDiscoveryCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmPathDiscoveryCnf1 (const r_adp_adpm_path_discovery_cnf_t * confirm)
{
    R_ADP_AdpmPathDiscoveryConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmPathDiscoveryCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_ADP_AdpmLbpConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmLbpConfirm (uint8_t chId, const r_adp_adpm_lbp_cnf_t * confirm)
{
    g_g3cb[chId].adpmLbpCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_ADPM_LBP );
}
/******************************************************************************
   End of function  R_ADP_AdpmLbpConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmLbpCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmLbpCnf0 (const r_adp_adpm_lbp_cnf_t * confirm)
{
    R_ADP_AdpmLbpConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmLbpCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmLbpCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmLbpCnf1 (const r_adp_adpm_lbp_cnf_t * confirm)
{
    R_ADP_AdpmLbpConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_ADP_AdpmLbpCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_ADP_AdpdDataIndication
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpdDataIndication (uint8_t chId, const r_adp_adpd_data_ind_t * indication)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueDataInd (indication); // use single only
}
/******************************************************************************
   End of function  R_ADP_AdpdDataIndication
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpdDataInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpdDataInd0 (const r_adp_adpd_data_ind_t * indication)
{
    R_ADP_AdpdDataIndication (0, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpdDataInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpdDataInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpdDataInd1 (const r_adp_adpd_data_ind_t * indication)
{
    R_ADP_AdpdDataIndication (1, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpdDataInd1
******************************************************************************/


/******************************************************************************
* Function Name: R_ADP_AdpmNetworkLeaveInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmNetworkLeaveInd (uint8_t chId)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd (NULL,
                              R_DEMO_APP_HANDLE_LEAVE_IND,
                              0);
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkLeaveInd
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmNetworkLeaveInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmNetworkLeaveInd0 (void)
{
    R_ADP_AdpmNetworkLeaveInd (0);
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkLeaveInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmNetworkLeaveInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmNetworkLeaveInd1 (void)
{
    R_ADP_AdpmNetworkLeaveInd (1);
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkLeaveInd1
******************************************************************************/



/******************************************************************************
* Function Name: R_ADP_AdpmNetworkStatusInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmNetworkStatusInd (uint8_t chId, const r_adp_adpm_network_status_ind_t * indication)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd ((const uint8_t *)indication,
                              R_DEMO_APP_HANDLE_STATUS_IND,
                              sizeof (r_adp_adpm_network_status_ind_t));
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkStatusInd
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmNetworkStatusInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmNetworkStatusInd0 (const r_adp_adpm_network_status_ind_t * indication)
{
    R_ADP_AdpmNetworkStatusInd (0, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkStatusInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmNetworkStatusInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmNetworkStatusInd1 (const r_adp_adpm_network_status_ind_t * indication)
{
    R_ADP_AdpmNetworkStatusInd (1, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmNetworkStatusInd1
******************************************************************************/


/******************************************************************************
* Function Name: R_ADP_AdpmPathDiscoveryInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmPathDiscoveryInd (uint8_t chId, const r_adp_adpm_path_discovery_ind_t * indication)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd ((const uint8_t *)indication,
                              R_DEMO_APP_HANDLE_PATH_DIS_IND,
                              sizeof (r_adp_adpm_path_discovery_ind_t));
}
/******************************************************************************
   End of function  R_ADP_AdpmPathDiscoveryInd
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmPathDiscoveryInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmPathDiscoveryInd0 (const r_adp_adpm_path_discovery_ind_t * indication)
{
    R_ADP_AdpmPathDiscoveryInd (0, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmPathDiscoveryInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmPathDiscoveryInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmPathDiscoveryInd1 (const r_adp_adpm_path_discovery_ind_t * indication)
{
    R_ADP_AdpmPathDiscoveryInd (1, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmPathDiscoveryInd1
******************************************************************************/



/******************************************************************************
* Function Name: R_ADP_AdpmLbpIndication
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmLbpIndication (uint8_t chId, const r_adp_adpm_lbp_ind_t * indication)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd ((const uint8_t *)indication,
                              R_DEMO_APP_HANDLE_LBP_IND,
                              sizeof (r_adp_adpm_lbp_ind_t));
}
/******************************************************************************
   End of function  R_ADP_AdpmLbpIndication
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmLbpInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmLbpInd0 (const r_adp_adpm_lbp_ind_t * indication)
{
    R_ADP_AdpmLbpIndication (0, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmLbpInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmLbpInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmLbpInd1 (const r_adp_adpm_lbp_ind_t * indication)
{
    R_ADP_AdpmLbpIndication (1, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmLbpInd1
******************************************************************************/



/******************************************************************************
* Function Name: R_ADP_AdpmBufferIndication
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmBufferIndication (uint8_t chId, const r_adp_adpm_buffer_ind_t * indication)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd ((const uint8_t *)indication,
                              R_DEMO_APP_HANDLE_BUFFER_IND,
                              sizeof (r_adp_adpm_buffer_ind_t));
}
/******************************************************************************
   End of function  R_ADP_AdpmBufferIndication
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmBufferInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmBufferInd0 (const r_adp_adpm_buffer_ind_t * indication)
{
    R_ADP_AdpmBufferIndication (0, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmBufferInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmBufferInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmBufferInd1 (const r_adp_adpm_buffer_ind_t * indication)
{
    R_ADP_AdpmBufferIndication (1, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmBufferInd1
******************************************************************************/


/******************************************************************************
* Function Name: R_ADP_AdpmKeyStateIndication
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmKeyStateIndication (uint8_t chId, const r_adp_adpm_key_state_ind_t * indication)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd ((const uint8_t *)indication,
                              R_DEMO_APP_HANDLE_KEY_STATE_IND,
                              sizeof (r_adp_adpm_key_state_ind_t));

}
/******************************************************************************
   End of function  R_ADP_AdpmKeyStateIndication
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmKeyStateInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmKeyStateInd0 (const r_adp_adpm_key_state_ind_t * indication)
{
    R_ADP_AdpmKeyStateIndication (0, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmKeyStateInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmKeyStateInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmKeyStateInd1 (const r_adp_adpm_key_state_ind_t * indication)
{
    R_ADP_AdpmKeyStateIndication (1, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmKeyStateInd1
******************************************************************************/


/******************************************************************************
* Function Name: R_ADP_AdpmRouteErrorIndication
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmRouteErrorIndication (uint8_t chId, const r_adp_adpm_route_error_ind_t * indication)
{
    g_g3cb[chId].adpmRouteErrorInd = *indication;
}
/******************************************************************************
   End of function  R_ADP_AdpmRouteErrorIndication
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmRouteErrorInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmRouteErrorInd0 (const r_adp_adpm_route_error_ind_t * indication)
{
    R_ADP_AdpmRouteErrorIndication (0, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmRouteErrorInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmRouteErrorInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmRouteErrorInd1 (const r_adp_adpm_route_error_ind_t * indication)
{
    R_ADP_AdpmRouteErrorIndication (1, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmRouteErrorInd1
******************************************************************************/


/******************************************************************************
* Function Name: R_ADP_AdpmEapKeyIndication
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmEapKeyIndication (uint8_t chId, const r_adp_adpm_eap_key_ind_t * indication)
{
    g_g3cb[chId].adpmEapKeyInd = *indication;
}
/******************************************************************************
   End of function  R_ADP_AdpmEapKeyIndication
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmEapKeyInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmEapKeyInd0 (const r_adp_adpm_eap_key_ind_t * indication)
{
    R_ADP_AdpmEapKeyIndication (0, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmEapKeyInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmEapKeyInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmEapKeyInd1 (const r_adp_adpm_eap_key_ind_t * indication)
{
    R_ADP_AdpmEapKeyIndication (1, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmEapKeyInd1
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmFrameCounterInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmFrameCounterInd (uint8_t chId, const r_adp_adpm_framecounter_ind_t * indication)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd ((const uint8_t *)indication,
                              R_DEMO_APP_HANDLE_FRAMECOUNT_IND,
                              sizeof (r_adp_adpm_framecounter_ind_t));
}
/******************************************************************************
   End of function  R_ADP_AdpmFrameCounterInd
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmFrameCounterInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmFrameCounterInd0 (const r_adp_adpm_framecounter_ind_t * indication)
{
    R_ADP_AdpmFrameCounterInd (0, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmFrameCounterInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmFrameCounterInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmFrameCounterInd1 (const r_adp_adpm_framecounter_ind_t * indication)
{
    R_ADP_AdpmFrameCounterInd (1, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmFrameCounterInd1
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmRouteUpdateInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmRouteUpdateInd (uint8_t chId, const r_adp_adpm_route_update_ind_t * indication)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd ((const uint8_t *)indication,
                              R_DEMO_APP_HANDLE_ADP_ROUTE_UPDATE_IND,
                              sizeof (r_adp_adpm_route_update_ind_t));
}
/******************************************************************************
   End of function  R_ADP_AdpmRouteUpdateInd
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmRouteUpdateInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmRouteUpdateInd0 (const r_adp_adpm_route_update_ind_t * indication)
{
    R_ADP_AdpmRouteUpdateInd (0, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmRouteUpdateInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmRouteUpdateInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmRouteUpdateInd1 (const r_adp_adpm_route_update_ind_t * indication)
{
    R_ADP_AdpmRouteUpdateInd (1, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmRouteUpdateInd1
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmLoadSeqNumInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmLoadSeqNumInd (uint8_t chId, const r_adp_adpm_load_seq_num_ind_t * indication)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd ((const uint8_t *)indication,
                              R_DEMO_APP_HANDLE_ADP_LOAD_SEQ_NUM_IND,
                              sizeof (r_adp_adpm_load_seq_num_ind_t));
}
/******************************************************************************
   End of function  R_ADP_AdpmLoadSeqNumInd
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmLoadSeqNumInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmLoadSeqNumInd0 (const r_adp_adpm_load_seq_num_ind_t * indication)
{
    R_ADP_AdpmLoadSeqNumInd (0, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmLoadSeqNumInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmLoadSeqNumInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmLoadSeqNumInd1 (const r_adp_adpm_load_seq_num_ind_t * indication)
{
    R_ADP_AdpmLoadSeqNumInd (1, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmLoadSeqNumInd1
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmRrepInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmRrepInd (uint8_t chId, const r_adp_adpm_rrep_ind_t * indication)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd ((const uint8_t *)indication,
                              R_DEMO_APP_HANDLE_ADP_RREP_IND,
                              sizeof (r_adp_adpm_rrep_ind_t));
}
/******************************************************************************
   End of function  R_ADP_AdpmRrepInd
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmRrepInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmRrepInd0 (const r_adp_adpm_rrep_ind_t * indication)
{
    R_ADP_AdpmRrepInd (0, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmRrepInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmRrepInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmRrepInd1 (const r_adp_adpm_rrep_ind_t * indication)
{
    R_ADP_AdpmRrepInd (1, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmRrepInd1
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmBeaconNotifyInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmBeaconNotifyInd (uint8_t chId, const r_adp_adpm_bcn_notify_ind_t * indication)
{
    g_g3cb[chId].mlmeBeaconNotifyInd = *indication;
}
/******************************************************************************
   End of function  R_ADP_AdpmRrepInd
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmBeaconNotifyInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmBeaconNotifyInd0 (const r_adp_adpm_bcn_notify_ind_t * indication)
{
    R_ADP_AdpmBeaconNotifyInd (0, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmBeaconNotifyInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_ADP_AdpmBeaconNotifyInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_ADP_AdpmBeaconNotifyInd1 (const r_adp_adpm_bcn_notify_ind_t * indication)
{
    R_ADP_AdpmBeaconNotifyInd (1, indication);
}
/******************************************************************************
   End of function  R_ADP_AdpmBeaconNotifyInd1
******************************************************************************/

/*===========================================================================*/
/*    EAP callbacks                                                        */
/*===========================================================================*/

/******************************************************************************
* Function Name: R_EAP_EapmResetConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmResetConfirm (uint8_t chId, const r_eap_eapm_reset_cnf_t * confirm)
{
    g_g3cb[chId].eapmResetCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_EAPM_RESET );
}
/******************************************************************************
   End of function  R_EAP_EapmResetConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmResetCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmResetCnf0 (const r_eap_eapm_reset_cnf_t * confirm)
{
    R_EAP_EapmResetConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_EAP_EapmResetCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmResetCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmResetCnf1 (const r_eap_eapm_reset_cnf_t * confirm)
{
    R_EAP_EapmResetConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_EAP_EapmResetCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_EAP_EapmStartConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmStartConfirm (uint8_t chId, const r_eap_eapm_start_cnf_t * confirm)
{
    g_g3cb[chId].eapmStartCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_EAPM_START );
}
/******************************************************************************
   End of function  R_EAP_EapmStartConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmStartCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmStartCnf0 (const r_eap_eapm_start_cnf_t * confirm)
{
    R_EAP_EapmStartConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_EAP_EapmStartCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmStartCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmStartCnf1 (const r_eap_eapm_start_cnf_t * confirm)
{
    R_EAP_EapmStartConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_EAP_EapmStartCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_EAP_EapmGetConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmGetConfirm (uint8_t chId, const r_eap_eapm_get_cnf_t * confirm)
{
    g_g3cb[chId].eapmGetCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_EAPM_GET );
}
/******************************************************************************
   End of function  R_EAP_EapmGetConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmGetCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmGetCnf0 (const r_eap_eapm_get_cnf_t * confirm)
{
    R_EAP_EapmGetConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_EAP_EapmGetCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmGetCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmGetCnf1 (const r_eap_eapm_get_cnf_t * confirm)
{
    R_EAP_EapmGetConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_EAP_EapmGetCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_EAP_EapmSetConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmSetConfirm (uint8_t chId, const r_eap_eapm_set_cnf_t * confirm)
{
    g_g3cb[chId].eapmSetCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_EAPM_SET );
}
/******************************************************************************
   End of function  R_EAP_EapmSetConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmSetCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmSetCnf0 (const r_eap_eapm_set_cnf_t * confirm)
{
    R_EAP_EapmSetConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_EAP_EapmSetCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmSetCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmSetCnf1 (const r_eap_eapm_set_cnf_t * confirm)
{
    R_EAP_EapmSetConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_EAP_EapmSetCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_EAP_EapmNetworkConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmNetworkConfirm (uint8_t chId, const r_eap_eapm_network_cnf_t * confirm)
{
    g_g3cb[chId].eapmNetworkCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_EAPM_NETWORK );
}
/******************************************************************************
   End of function  R_EAP_EapmNetworkConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmNetworkCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmNetworkCnf0 (const r_eap_eapm_network_cnf_t * confirm)
{
    R_EAP_EapmNetworkConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_EAP_EapmNetworkCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmNetworkCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmNetworkCnf1 (const r_eap_eapm_network_cnf_t * confirm)
{
    R_EAP_EapmNetworkConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_EAP_EapmNetworkCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_EAP_EapmSetClientInfoConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmSetClientInfoConfirm (uint8_t chId, const r_eap_eapm_set_clientinfo_cnf_t * confirm)
{
    g_g3cb[chId].eapmSetClientInfoCnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_EAPM_SETCLIENTINFO );
}
/******************************************************************************
   End of function  R_EAP_EapmSetClientInfoConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmSetClientInfoCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmSetClientInfoCnf0 (const r_eap_eapm_set_clientinfo_cnf_t * confirm)
{
    R_EAP_EapmSetClientInfoConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_EAP_EapmSetClientInfoCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmSetClientInfoCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmSetClientInfoCnf1 (const r_eap_eapm_set_clientinfo_cnf_t * confirm)
{
    R_EAP_EapmSetClientInfoConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_EAP_EapmSetClientInfoCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_EAP_EapmNetworkJoinIndication
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmNetworkJoinIndication (uint8_t chId, const r_eap_eapm_network_join_ind_t * indication)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd ((const uint8_t *)indication,
                              R_DEMO_APP_HANDLE_EAP_NETWORKJOIN_IND,
                              sizeof (r_eap_eapm_network_join_ind_t));
}
/******************************************************************************
   End of function  R_EAP_EapmNetworkJoinIndication
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmNetworkJoinInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmNetworkJoinInd0 (const r_eap_eapm_network_join_ind_t * indication)
{
    R_EAP_EapmNetworkJoinIndication (0, indication);
}
/******************************************************************************
   End of function  R_EAP_EapmNetworkJoinInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmNetworkJoinInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmNetworkJoinInd1 (const r_eap_eapm_network_join_ind_t * indication)
{
    R_EAP_EapmNetworkJoinIndication (1, indication);
}
/******************************************************************************
   End of function  R_EAP_EapmNetworkJoinInd1
******************************************************************************/


/******************************************************************************
* Function Name: R_EAP_EapmNetworkLeaveInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmNetworkLeaveInd (uint8_t chId, const r_eap_eapm_network_leave_ind_t * indication)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd ((const uint8_t *)indication,
                              R_DEMO_APP_HANDLE_EAP_NETWORKLEAVE_IND,
                              sizeof (r_eap_eapm_network_leave_ind_t));
}
/******************************************************************************
   End of function  R_EAP_EapmNetworkLeaveInd
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmNetworkLeaveInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmNetworkLeaveInd0 (const r_eap_eapm_network_leave_ind_t * indication)
{
    R_EAP_EapmNetworkLeaveInd (0, indication);
}
/******************************************************************************
   End of function  R_EAP_EapmNetworkLeaveInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmNetworkLeaveInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmNetworkLeaveInd1 (const r_eap_eapm_network_leave_ind_t * indication)
{
    R_EAP_EapmNetworkLeaveInd (1, indication);
}
/******************************************************************************
   End of function  R_EAP_EapmNetworkLeaveInd1
******************************************************************************/


/******************************************************************************
* Function Name: R_EAP_EapmNewDeviceIndication
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmNewDeviceIndication (uint8_t chId, const r_eap_eapm_newdevice_ind_t * indication)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd ((const uint8_t *)indication,
                              R_DEMO_APP_HANDLE_EAP_NEWDEVICE_IND,
                              sizeof (r_eap_eapm_newdevice_ind_t));
}
/******************************************************************************
   End of function  R_EAP_EapmNewDeviceIndication
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmNewDeviceInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmNewDeviceInd0 (const r_eap_eapm_newdevice_ind_t * indication)
{
    R_EAP_EapmNewDeviceIndication (0, indication);
}
/******************************************************************************
   End of function  R_EAP_EapmNewDeviceInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmNewDeviceInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmNewDeviceInd1 (const r_eap_eapm_newdevice_ind_t * indication)
{
    R_EAP_EapmNewDeviceIndication (1, indication);
}
/******************************************************************************
   End of function  R_EAP_EapmNewDeviceInd1
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmEapKeyIndication
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmEapKeyIndication (uint8_t chId, const r_eap_eapm_eap_key_ind_t * indication)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd ((const uint8_t *)indication,
                              R_DEMO_APP_HANDLE_EAP_EAP_KEY_IND,
                              sizeof (r_eap_eapm_eap_key_ind_t));
}
/******************************************************************************
   End of function  R_EAP_EapmEapKeyIndication
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmEapKeyInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmEapKeyInd0 (const r_eap_eapm_eap_key_ind_t * indication)
{
    R_EAP_EapmEapKeyIndication (0, indication);
}
/******************************************************************************
   End of function  R_EAP_EapmEapKeyInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_EAP_EapmEapKeyInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_EAP_EapmEapKeyInd1 (const r_eap_eapm_eap_key_ind_t * indication)
{
    R_EAP_EapmEapKeyIndication (1, indication);
}
/******************************************************************************
   End of function  R_EAP_EapmEapKeyInd1
******************************************************************************/

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
/*RF callback*/
/******************************************************************************
* Function Name: R_G3RFMAC_McpsDataCnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_McpsDataConfirm (uint8_t chId, const r_g3rfmac_mcps_data_cnf_t * confirm)
{
    g_g3cb[chId].rf_mcps_data_cnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_MCPS_DATA );
}
/******************************************************************************
   End of function  G3RFMAC_McpsDataConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_McpsDataCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_McpsDataCnf0 (const r_g3rfmac_mcps_data_cnf_t * confirm)
{
    R_G3RFMAC_McpsDataConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3RFMAC_McpsDataCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_McpsDataCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_McpsDataCnf1 (const r_g3rfmac_mcps_data_cnf_t * confirm)
{
    R_G3RFMAC_McpsDataConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3RFMAC_McpsDataCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_G3RFMAC_MlmeResetConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeResetConfirm (uint8_t chId, const r_g3rfmac_mlme_reset_cnf_t * confirm)
{
    g_g3cb[chId].rf_mlme_reset_cnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_MLME_RESET );
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeResetConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeResetCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeResetCnf0 (const r_g3rfmac_mlme_reset_cnf_t * confirm)
{
    R_G3RFMAC_MlmeResetConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeResetCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeResetCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeResetCnf1 (const r_g3rfmac_mlme_reset_cnf_t * confirm)
{
    R_G3RFMAC_MlmeResetConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeResetCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_G3RFMAC_MlmeSetConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeSetConfirm (uint8_t chId, const r_g3rfmac_mlme_set_cnf_t * confirm)
{
    g_g3cb[chId].rf_mlme_set_cnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_RF_MLME_SET );
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeSetConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeSetCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeSetCnf0 (const r_g3rfmac_mlme_set_cnf_t * confirm)
{
    R_G3RFMAC_MlmeSetConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeSetCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeSetCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeSetCnf1 (const r_g3rfmac_mlme_set_cnf_t * confirm)
{
    R_G3RFMAC_MlmeSetConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeSetCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_G3RFMAC_MlmeGetConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeGetConfirm (uint8_t chId, const r_g3rfmac_mlme_get_cnf_t * confirm)
{
    g_g3cb[chId].rf_mlme_get_cnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_RF_MLME_GET );
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeGetConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeGetCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeGetCnf0 (const r_g3rfmac_mlme_get_cnf_t * confirm)
{
    R_G3RFMAC_MlmeGetConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeGetCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeGetCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeGetCnf1 (const r_g3rfmac_mlme_get_cnf_t * confirm)
{
    R_G3RFMAC_MlmeGetConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeGetCnf1
******************************************************************************/



/******************************************************************************
* Function Name: R_G3RFMAC_MlmeScanConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeScanConfirm (uint8_t chId, const r_g3rfmac_mlme_scan_cnf_t * confirm)
{
    g_g3cb[chId].rf_mlme_scan_cnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_MLME_SCAN );
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeScanConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeScanCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeScanCnf0 (const r_g3rfmac_mlme_scan_cnf_t * confirm)
{
    R_G3RFMAC_MlmeScanConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeScanCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeScanCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeScanCnf1 (const r_g3rfmac_mlme_scan_cnf_t * confirm)
{
    R_G3RFMAC_MlmeScanConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeScanCnf1
******************************************************************************/



/******************************************************************************
* Function Name: R_G3RFMAC_MlmeStartConfirm
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeStartConfirm (uint8_t chId, const r_g3rfmac_mlme_start_cnf_t * confirm)
{
    g_g3cb[chId].rf_mlme_start_cnf = *confirm;
    R_DEMO_SetEventFlag( &g_g3cb[chId].statusFlags, R_FLAG_MLME_START );
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeStartConfirm
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeStartCnf0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeStartCnf0 (const r_g3rfmac_mlme_start_cnf_t * confirm)
{
    R_G3RFMAC_MlmeStartConfirm (0, confirm);
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeStartCnf0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeStartCnf1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeStartCnf1 (const r_g3rfmac_mlme_start_cnf_t * confirm)
{
    R_G3RFMAC_MlmeStartConfirm (1, confirm);
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeStartCnf1
******************************************************************************/


/******************************************************************************
* Function Name: R_G3RFMAC_McpsDataIndication
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_McpsDataIndication (uint8_t chId, const r_g3rfmac_mcps_data_ind_t * ind)
{
    UNUSED (chId);

    /* Enqueue indication. */
    R_DEMO_AppThreadEnqueInd ((const uint8_t *)ind,
                              R_DEMO_APP_HANDLE_RFMAC_DATA_IND,
                              sizeof (r_g3rfmac_mcps_data_ind_t));
}
/******************************************************************************
   End of function  R_G3RFMAC_McpsDataIndication
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_McpsDataInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_McpsDataInd0 (const r_g3rfmac_mcps_data_ind_t * indication)
{
    R_G3RFMAC_McpsDataIndication (0, indication);
}
/******************************************************************************
   End of function  R_G3RFMAC_McpsDataInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_McpsDataInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_McpsDataInd1 (const r_g3rfmac_mcps_data_ind_t * indication)
{
    R_G3RFMAC_McpsDataIndication (1, indication);
}
/******************************************************************************
   End of function  R_G3MAC_McpsDataInd1
******************************************************************************/


/******************************************************************************
* Function Name: R_G3RFMAC_MlmeBeaconNotifyInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeBeaconNotifyInd (uint8_t chId, const r_g3rfmac_mlme_bcn_notify_ind_t * indication)
{
    g_g3cb[chId].rf_mlme_beacon_notify_ind = *indication;
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeBeaconNotifyInd
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeBeaconNotifyInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeBeaconNotifyInd0 (const r_g3rfmac_mlme_bcn_notify_ind_t * indication)
{
    R_G3RFMAC_MlmeBeaconNotifyInd (0, indication);
}
/******************************************************************************
   End of function  R_G3MAC_MlmeBeaconNotifyInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeBeaconNotifyInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeBeaconNotifyInd1 (const r_g3rfmac_mlme_bcn_notify_ind_t * indication)
{
    R_G3RFMAC_MlmeBeaconNotifyInd (1, indication);
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeBeaconNotifyInd1
******************************************************************************/


/******************************************************************************
* Function Name: R_G3RFMAC_MlmeCommStatusInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeCommStatusInd (uint8_t chId, const r_g3rfmac_mlme_comm_status_ind_t * indication)
{
    g_g3cb[chId].rf_mlme_comm_status_ind = *indication;
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeCommStatusInd
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeCommStatusInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeCommStatusInd0 (const r_g3rfmac_mlme_comm_status_ind_t * indication)
{
    R_G3RFMAC_MlmeCommStatusInd (0, indication);
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeCommStatusInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeCommStatusInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeCommStatusInd1 (const r_g3rfmac_mlme_comm_status_ind_t * indication)
{
    R_G3RFMAC_MlmeCommStatusInd (1, indication);
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeCommStatusInd1
******************************************************************************/


/******************************************************************************
* Function Name: R_G3RFMAC_MlmeFrameCountInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeFrameCountInd (uint8_t chId, const r_g3rfmac_mlme_framecount_ind_t * indication)
{
    g_g3cb[chId].rf_mlme_frame_count_ind = *indication;
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeFrameCountInd
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeFrameCountInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeFrameCountInd0 (const r_g3rfmac_mlme_framecount_ind_t * indication)
{
    R_G3RFMAC_MlmeFrameCountInd (0, indication);
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeFrameCountInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeFrameCountInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeFrameCountInd1 (const r_g3rfmac_mlme_framecount_ind_t * indication)
{
    R_G3RFMAC_MlmeFrameCountInd (1, indication);
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeFrameCountInd1
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeWarningInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeWarningInd (uint8_t chId, const r_g3rfmac_mlme_warning_ind_t * indication)
{
    g_g3cb[chId].rf_mlme_warning_ind = *indication;
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeWarningInd
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeWarningInd0
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeWarningInd0 (const r_g3rfmac_mlme_warning_ind_t * indication)
{
    R_G3RFMAC_MlmeWarningInd (0, indication);
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeWarningInd0
******************************************************************************/

/******************************************************************************
* Function Name: R_G3RFMAC_MlmeWarningInd1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void R_G3RFMAC_MlmeWarningInd1 (const r_g3rfmac_mlme_warning_ind_t * indication)
{
    R_G3RFMAC_MlmeWarningInd (1, indication);
}
/******************************************************************************
   End of function  R_G3RFMAC_MlmeWarningInd1
******************************************************************************/
#endif /* R_DEFINE_DISABLE_HYBRID R_DEFINE_DISABLE_APP_HYBRID */

/******************************************************************************
* Function Name: R_DEMO_InitG3CallBack
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_DEMO_InitG3CallBack (uint8_t chId, r_g3_callback_t * pCallBack)
{
    if (0 == chId)
    {
        pCallBack->R_G3_SetConfigCnf                                  =   R_G3_SetConfigCnf0;
        pCallBack->R_G3_GetConfigCnf                                  =   R_G3_GetConfigCnf0;
        pCallBack->R_G3_GetInfoCnf                                    =   R_G3_GetInfoCnf0;
        pCallBack->R_G3_ClearInfoCnf                                  =   R_G3_ClearInfoCnf0;
        pCallBack->R_G3_EventInd                                      =   R_G3_EventInd0;
        pCallBack->R_G3_DumpCnf                                       =   R_G3_DumpCnf0;
        pCallBack->R_G3_DumpAbortCnf                                  =   R_G3_DumpAbortCnf0;
        pCallBack->R_G3_DumpInd                                       =   R_G3_DumpInd0;
    }
    else
    {
        pCallBack->R_G3_SetConfigCnf                                  =   R_G3_SetConfigCnf1;
        pCallBack->R_G3_GetConfigCnf                                  =   R_G3_GetConfigCnf1;
        pCallBack->R_G3_GetInfoCnf                                    =   R_G3_GetInfoCnf1;
        pCallBack->R_G3_ClearInfoCnf                                  =   R_G3_ClearInfoCnf1;
        pCallBack->R_G3_EventInd                                      =   R_G3_EventInd1;
        pCallBack->R_G3_DumpCnf                                       =   R_G3_DumpCnf1;
        pCallBack->R_G3_DumpAbortCnf                                  =   R_G3_DumpAbortCnf1;
        pCallBack->R_G3_DumpInd                                       =   R_G3_DumpInd1;
    }

}
/******************************************************************************
   End of function  R_DEMO_InitG3CallBack
******************************************************************************/


/******************************************************************************
* Function Name: R_DEMO_InitMacCallBack
* Description :
* Arguments :
* Return Value :
******************************************************************************/
#ifndef R_DEFINE_DISABLE_UMAC
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
r_result_t R_DEMO_InitMacCallBack (uint8_t chId, r_g3_callback_t * pCallBack, uint8_t mediaType)
#else
r_result_t R_DEMO_InitMacCallBack (uint8_t chId, r_g3_callback_t * pCallBack)
#endif
{

    if (NULL == pCallBack)
    {
        return R_RESULT_ILLEGAL_NULL_POINTER;
    }

    R_memset ((uint8_t *)pCallBack, 0, sizeof (r_g3_callback_t));

    R_DEMO_InitG3CallBack ( chId, pCallBack );

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    switch(mediaType)
    {
        case R_MEDIA_TYPE0_PLC:
        {
#endif
            if (0 == chId)
            {
                pCallBack->mode.mac.R_G3MAC_McpsDataCnf                       =   R_G3MAC_McpsDataCnf0;
                pCallBack->mode.mac.R_G3MAC_MlmeResetCnf                      =   R_G3MAC_MlmeResetCnf0;
                pCallBack->mode.mac.R_G3MAC_MlmeGetCnf                        =   R_G3MAC_MlmeGetCnf0;
                pCallBack->mode.mac.R_G3MAC_MlmeSetCnf                        =   R_G3MAC_MlmeSetCnf0;
                pCallBack->mode.mac.R_G3MAC_MlmeScanCnf                       =   R_G3MAC_MlmeScanCnf0;
                pCallBack->mode.mac.R_G3MAC_MlmeStartCnf                      =   R_G3MAC_MlmeStartCnf0;
                pCallBack->mode.mac.R_G3MAC_McpsDataInd                       =   R_G3MAC_McpsDataInd0;
                pCallBack->mode.mac.R_G3MAC_MlmeBeaconNotifyInd               =   R_G3MAC_MlmeBeaconNotifyInd0;
                pCallBack->mode.mac.R_G3MAC_MlmeCommStatusInd                 =   R_G3MAC_MlmeCommStatusInd0;
                pCallBack->mode.mac.R_G3MAC_MlmeFrameCountInd                 =   R_G3MAC_MlmeFrameCountInd0;
                pCallBack->mode.mac.R_G3MAC_MlmeTmrReceiveInd                 =   R_G3MAC_MlmeTmrReceiveInd0;
                pCallBack->mode.mac.R_G3MAC_MlmeTmrTransmitInd                =   R_G3MAC_MlmeTmrTransmitInd0;
            }
            else
            {
                pCallBack->mode.mac.R_G3MAC_McpsDataCnf                       =   R_G3MAC_McpsDataCnf1;
                pCallBack->mode.mac.R_G3MAC_MlmeResetCnf                      =   R_G3MAC_MlmeResetCnf1;
                pCallBack->mode.mac.R_G3MAC_MlmeGetCnf                        =   R_G3MAC_MlmeGetCnf1;
                pCallBack->mode.mac.R_G3MAC_MlmeSetCnf                        =   R_G3MAC_MlmeSetCnf1;
                pCallBack->mode.mac.R_G3MAC_MlmeScanCnf                       =   R_G3MAC_MlmeScanCnf1;
                pCallBack->mode.mac.R_G3MAC_MlmeStartCnf                      =   R_G3MAC_MlmeStartCnf1;
                pCallBack->mode.mac.R_G3MAC_McpsDataInd                       =   R_G3MAC_McpsDataInd1;
                pCallBack->mode.mac.R_G3MAC_MlmeBeaconNotifyInd               =   R_G3MAC_MlmeBeaconNotifyInd1;
                pCallBack->mode.mac.R_G3MAC_MlmeCommStatusInd                 =   R_G3MAC_MlmeCommStatusInd1;
                pCallBack->mode.mac.R_G3MAC_MlmeFrameCountInd                 =   R_G3MAC_MlmeFrameCountInd1;
                pCallBack->mode.mac.R_G3MAC_MlmeTmrReceiveInd                 =   R_G3MAC_MlmeTmrReceiveInd1;
                pCallBack->mode.mac.R_G3MAC_MlmeTmrTransmitInd                =   R_G3MAC_MlmeTmrTransmitInd1;
            }
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
        }
        break;
        case R_MEDIA_TYPE2_BOTH:
        {
            if (0 == chId)
            {
                pCallBack->mode.hymac.R_G3MAC_McpsDataCnf                     =   R_G3MAC_McpsDataCnf0;
                pCallBack->mode.hymac.R_G3MAC_MlmeResetCnf                    =   R_G3MAC_MlmeResetCnf0;
                pCallBack->mode.hymac.R_G3MAC_MlmeGetCnf                      =   R_G3MAC_MlmeGetCnf0;
                pCallBack->mode.hymac.R_G3MAC_MlmeSetCnf                      =   R_G3MAC_MlmeSetCnf0;
                pCallBack->mode.hymac.R_G3MAC_MlmeScanCnf                     =   R_G3MAC_MlmeScanCnf0;
                pCallBack->mode.hymac.R_G3MAC_MlmeStartCnf                    =   R_G3MAC_MlmeStartCnf0;
                pCallBack->mode.hymac.R_G3MAC_McpsDataInd                     =   R_G3MAC_McpsDataInd0;
                pCallBack->mode.hymac.R_G3MAC_MlmeBeaconNotifyInd             =   R_G3MAC_MlmeBeaconNotifyInd0;
                pCallBack->mode.hymac.R_G3MAC_MlmeCommStatusInd               =   R_G3MAC_MlmeCommStatusInd0;
                pCallBack->mode.hymac.R_G3MAC_MlmeFrameCountInd               =   R_G3MAC_MlmeFrameCountInd0;
                pCallBack->mode.hymac.R_G3MAC_MlmeTmrReceiveInd               =   R_G3MAC_MlmeTmrReceiveInd0;
                pCallBack->mode.hymac.R_G3MAC_MlmeTmrTransmitInd              =   R_G3MAC_MlmeTmrTransmitInd0;
                pCallBack->mode.hymac.R_G3RFMAC_McpsDataCnf                   =   R_G3RFMAC_McpsDataCnf0;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeResetCnf                  =   R_G3RFMAC_MlmeResetCnf0;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeGetCnf                    =   R_G3RFMAC_MlmeGetCnf0;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeSetCnf                    =   R_G3RFMAC_MlmeSetCnf0;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeScanCnf                   =   R_G3RFMAC_MlmeScanCnf0;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeStartCnf                  =   R_G3RFMAC_MlmeStartCnf0;
                pCallBack->mode.hymac.R_G3RFMAC_McpsDataInd                   =   R_G3RFMAC_McpsDataInd0;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeBeaconNotifyInd           =   R_G3RFMAC_MlmeBeaconNotifyInd0;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeCommStatusInd             =   R_G3RFMAC_MlmeCommStatusInd0;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeFrameCountInd             =   R_G3RFMAC_MlmeFrameCountInd0;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeWarningInd                =   R_G3RFMAC_MlmeWarningInd0;
            }
            else
            {
                pCallBack->mode.hymac.R_G3MAC_McpsDataCnf                     =   R_G3MAC_McpsDataCnf1;
                pCallBack->mode.hymac.R_G3MAC_MlmeResetCnf                    =   R_G3MAC_MlmeResetCnf1;
                pCallBack->mode.hymac.R_G3MAC_MlmeGetCnf                      =   R_G3MAC_MlmeGetCnf1;
                pCallBack->mode.hymac.R_G3MAC_MlmeSetCnf                      =   R_G3MAC_MlmeSetCnf1;
                pCallBack->mode.hymac.R_G3MAC_MlmeScanCnf                     =   R_G3MAC_MlmeScanCnf1;
                pCallBack->mode.hymac.R_G3MAC_MlmeStartCnf                    =   R_G3MAC_MlmeStartCnf1;
                pCallBack->mode.hymac.R_G3MAC_McpsDataInd                     =   R_G3MAC_McpsDataInd1;
                pCallBack->mode.hymac.R_G3MAC_MlmeBeaconNotifyInd             =   R_G3MAC_MlmeBeaconNotifyInd1;
                pCallBack->mode.hymac.R_G3MAC_MlmeCommStatusInd               =   R_G3MAC_MlmeCommStatusInd1;
                pCallBack->mode.hymac.R_G3MAC_MlmeFrameCountInd               =   R_G3MAC_MlmeFrameCountInd1;
                pCallBack->mode.hymac.R_G3MAC_MlmeTmrReceiveInd               =   R_G3MAC_MlmeTmrReceiveInd1;
                pCallBack->mode.hymac.R_G3MAC_MlmeTmrTransmitInd              =   R_G3MAC_MlmeTmrTransmitInd1;
                pCallBack->mode.hymac.R_G3RFMAC_McpsDataCnf                   =   R_G3RFMAC_McpsDataCnf1;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeResetCnf                  =   R_G3RFMAC_MlmeResetCnf1;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeGetCnf                    =   R_G3RFMAC_MlmeGetCnf1;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeSetCnf                    =   R_G3RFMAC_MlmeSetCnf1;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeScanCnf                   =   R_G3RFMAC_MlmeScanCnf1;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeStartCnf                  =   R_G3RFMAC_MlmeStartCnf1;
                pCallBack->mode.hymac.R_G3RFMAC_McpsDataInd                   =   R_G3RFMAC_McpsDataInd1;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeBeaconNotifyInd           =   R_G3RFMAC_MlmeBeaconNotifyInd1;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeCommStatusInd             =   R_G3RFMAC_MlmeCommStatusInd1;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeFrameCountInd             =   R_G3RFMAC_MlmeFrameCountInd1;
                pCallBack->mode.hymac.R_G3RFMAC_MlmeWarningInd                =   R_G3RFMAC_MlmeWarningInd1;
            }
        }
        break;
        case R_MEDIA_TYPE1_RF:
        {
            if (0 == chId)
            {
                pCallBack->mode.rfmac.R_G3RFMAC_McpsDataCnf                   =   R_G3RFMAC_McpsDataCnf0;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeResetCnf                  =   R_G3RFMAC_MlmeResetCnf0;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeGetCnf                    =   R_G3RFMAC_MlmeGetCnf0;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeSetCnf                    =   R_G3RFMAC_MlmeSetCnf0;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeScanCnf                   =   R_G3RFMAC_MlmeScanCnf0;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeStartCnf                  =   R_G3RFMAC_MlmeStartCnf0;
                pCallBack->mode.rfmac.R_G3RFMAC_McpsDataInd                   =   R_G3RFMAC_McpsDataInd0;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeBeaconNotifyInd           =   R_G3RFMAC_MlmeBeaconNotifyInd0;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeCommStatusInd             =   R_G3RFMAC_MlmeCommStatusInd0;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeFrameCountInd             =   R_G3RFMAC_MlmeFrameCountInd0;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeWarningInd                =   R_G3RFMAC_MlmeWarningInd0;
            }
            else
            {
                pCallBack->mode.rfmac.R_G3RFMAC_McpsDataCnf                   =   R_G3RFMAC_McpsDataCnf1;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeResetCnf                  =   R_G3RFMAC_MlmeResetCnf1;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeGetCnf                    =   R_G3RFMAC_MlmeGetCnf1;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeSetCnf                    =   R_G3RFMAC_MlmeSetCnf1;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeScanCnf                   =   R_G3RFMAC_MlmeScanCnf1;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeStartCnf                  =   R_G3RFMAC_MlmeStartCnf1;
                pCallBack->mode.rfmac.R_G3RFMAC_McpsDataInd                   =   R_G3RFMAC_McpsDataInd1;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeBeaconNotifyInd           =   R_G3RFMAC_MlmeBeaconNotifyInd1;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeCommStatusInd             =   R_G3RFMAC_MlmeCommStatusInd1;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeFrameCountInd             =   R_G3RFMAC_MlmeFrameCountInd1;
                pCallBack->mode.rfmac.R_G3RFMAC_MlmeWarningInd                =   R_G3RFMAC_MlmeWarningInd1;
            }
        }
        break;
        default:
        {
            /*do nothing*/
        }
        break;
    }
#endif /* R_DEFINE_DISABLE_HYBRID R_DEFINE_DISABLE_APP_HYBRID */

    return R_RESULT_SUCCESS;
} /* R_DEMO_InitMacCallBack */
/******************************************************************************
   End of function  R_DEMO_InitMacCallBack
******************************************************************************/
#endif


/******************************************************************************
* Function Name: R_DEMO_InitAdpCallBack
* Description :
* Arguments :
* Return Value :
******************************************************************************/
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
r_result_t R_DEMO_InitAdpCallBack (uint8_t chId, r_g3_callback_t * pCallBack, uint8_t mediaType)
#else
r_result_t R_DEMO_InitAdpCallBack (uint8_t chId, r_g3_callback_t * pCallBack)
#endif
{

    if (NULL == pCallBack)
    {
        return R_RESULT_ILLEGAL_NULL_POINTER;
    }

    R_memset ((uint8_t *)pCallBack, 0, sizeof (r_g3_callback_t));

    R_DEMO_InitG3CallBack ( chId, pCallBack );

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    switch(mediaType)
    {
        case R_MEDIA_TYPE0_PLC:
        {
#endif
            if (0 == chId)
            {
                pCallBack->mode.adp.R_ADP_AdpdDataCnf                         =   R_ADP_AdpdDataCnf0;
                pCallBack->mode.adp.R_ADP_AdpmResetCnf                        =   R_ADP_AdpmResetCnf0;
                pCallBack->mode.adp.R_ADP_AdpmDiscoveryCnf                    =   R_ADP_AdpmDiscoveryCnf0;
                pCallBack->mode.adp.R_ADP_AdpmNetworkStartCnf                 =   R_ADP_AdpmNetworkStartCnf0;
                pCallBack->mode.adp.R_ADP_AdpmNetworkJoinCnf                  =   R_ADP_AdpmNetworkJoinCnf0;
                pCallBack->mode.adp.R_ADP_AdpmNetworkLeaveCnf                 =   R_ADP_AdpmNetworkLeaveCnf0;
                pCallBack->mode.adp.R_ADP_AdpmGetCnf                          =   R_ADP_AdpmGetCnf0;
                pCallBack->mode.adp.R_ADP_AdpmSetCnf                          =   R_ADP_AdpmSetCnf0;
                pCallBack->mode.adp.R_ADP_AdpmRouteDiscoveryCnf               =   R_ADP_AdpmRouteDiscoveryCnf0;
                pCallBack->mode.adp.R_ADP_AdpmPathDiscoveryCnf                =   R_ADP_AdpmPathDiscoveryCnf0;
                pCallBack->mode.adp.R_ADP_AdpmLbpCnf                          =   R_ADP_AdpmLbpCnf0;
                pCallBack->mode.adp.R_ADP_AdpdDataInd                         =   R_ADP_AdpdDataInd0;
                pCallBack->mode.adp.R_ADP_AdpmNetworkLeaveInd                 =   R_ADP_AdpmNetworkLeaveInd0;
                pCallBack->mode.adp.R_ADP_AdpmPathDiscoveryInd                =   R_ADP_AdpmPathDiscoveryInd0;
                pCallBack->mode.adp.R_ADP_AdpmLbpInd                          =   R_ADP_AdpmLbpInd0;
                pCallBack->mode.adp.R_ADP_AdpmNetworkStatusInd                =   R_ADP_AdpmNetworkStatusInd0;
                pCallBack->mode.adp.R_ADP_AdpmBufferInd                       =   R_ADP_AdpmBufferInd0;
                pCallBack->mode.adp.R_ADP_AdpmKeyStateInd                     =   R_ADP_AdpmKeyStateInd0;
                pCallBack->mode.adp.R_ADP_AdpmRouteErrorInd                   =   R_ADP_AdpmRouteErrorInd0;
                pCallBack->mode.adp.R_ADP_AdpmEapKeyInd                       =   R_ADP_AdpmEapKeyInd0;
                pCallBack->mode.adp.R_ADP_AdpmFrameCounterInd                 =   R_ADP_AdpmFrameCounterInd0;
                pCallBack->mode.adp.R_ADP_AdpmRouteUpdateInd                  =   R_ADP_AdpmRouteUpdateInd0;
                pCallBack->mode.adp.R_ADP_AdpmLoadSeqNumInd                   =   R_ADP_AdpmLoadSeqNumInd0;
                pCallBack->mode.adp.R_ADP_AdpmRrepInd                         =   R_ADP_AdpmRrepInd0;
                pCallBack->mode.adp.R_ADP_AdpmBeaconNotifyInd                 =   R_ADP_AdpmBeaconNotifyInd0;
                pCallBack->mode.adp.R_G3MAC_MlmeGetCnf                        =   R_G3MAC_MlmeGetCnf0;
                pCallBack->mode.adp.R_G3MAC_MlmeSetCnf                        =   R_G3MAC_MlmeSetCnf0;
                pCallBack->mode.adp.R_G3MAC_MlmeTmrReceiveInd                 =   R_G3MAC_MlmeTmrReceiveInd0;
                pCallBack->mode.adp.R_G3MAC_MlmeTmrTransmitInd                =   R_G3MAC_MlmeTmrTransmitInd0;
            }
            else
            {
                pCallBack->mode.adp.R_ADP_AdpdDataCnf                         =   R_ADP_AdpdDataCnf1;
                pCallBack->mode.adp.R_ADP_AdpmResetCnf                        =   R_ADP_AdpmResetCnf1;
                pCallBack->mode.adp.R_ADP_AdpmDiscoveryCnf                    =   R_ADP_AdpmDiscoveryCnf1;
                pCallBack->mode.adp.R_ADP_AdpmNetworkStartCnf                 =   R_ADP_AdpmNetworkStartCnf1;
                pCallBack->mode.adp.R_ADP_AdpmNetworkJoinCnf                  =   R_ADP_AdpmNetworkJoinCnf1;
                pCallBack->mode.adp.R_ADP_AdpmNetworkLeaveCnf                 =   R_ADP_AdpmNetworkLeaveCnf1;
                pCallBack->mode.adp.R_ADP_AdpmGetCnf                          =   R_ADP_AdpmGetCnf1;
                pCallBack->mode.adp.R_ADP_AdpmSetCnf                          =   R_ADP_AdpmSetCnf1;
                pCallBack->mode.adp.R_ADP_AdpmRouteDiscoveryCnf               =   R_ADP_AdpmRouteDiscoveryCnf1;
                pCallBack->mode.adp.R_ADP_AdpmPathDiscoveryCnf                =   R_ADP_AdpmPathDiscoveryCnf1;
                pCallBack->mode.adp.R_ADP_AdpmLbpCnf                          =   R_ADP_AdpmLbpCnf1;
                pCallBack->mode.adp.R_ADP_AdpdDataInd                         =   R_ADP_AdpdDataInd1;
                pCallBack->mode.adp.R_ADP_AdpmNetworkLeaveInd                 =   R_ADP_AdpmNetworkLeaveInd1;
                pCallBack->mode.adp.R_ADP_AdpmPathDiscoveryInd                =   R_ADP_AdpmPathDiscoveryInd1;
                pCallBack->mode.adp.R_ADP_AdpmLbpInd                          =   R_ADP_AdpmLbpInd1;
                pCallBack->mode.adp.R_ADP_AdpmNetworkStatusInd                =   R_ADP_AdpmNetworkStatusInd1;
                pCallBack->mode.adp.R_ADP_AdpmBufferInd                       =   R_ADP_AdpmBufferInd1;
                pCallBack->mode.adp.R_ADP_AdpmKeyStateInd                     =   R_ADP_AdpmKeyStateInd1;
                pCallBack->mode.adp.R_ADP_AdpmRouteErrorInd                   =   R_ADP_AdpmRouteErrorInd1;
                pCallBack->mode.adp.R_ADP_AdpmEapKeyInd                       =   R_ADP_AdpmEapKeyInd1;
                pCallBack->mode.adp.R_ADP_AdpmFrameCounterInd                 =   R_ADP_AdpmFrameCounterInd1;
                pCallBack->mode.adp.R_ADP_AdpmRouteUpdateInd                  =   R_ADP_AdpmRouteUpdateInd1;
                pCallBack->mode.adp.R_ADP_AdpmLoadSeqNumInd                   =   R_ADP_AdpmLoadSeqNumInd1;
                pCallBack->mode.adp.R_ADP_AdpmRrepInd                         =   R_ADP_AdpmRrepInd1;
                pCallBack->mode.adp.R_ADP_AdpmBeaconNotifyInd                 =   R_ADP_AdpmBeaconNotifyInd1;
                pCallBack->mode.adp.R_G3MAC_MlmeGetCnf                        =   R_G3MAC_MlmeGetCnf1;
                pCallBack->mode.adp.R_G3MAC_MlmeSetCnf                        =   R_G3MAC_MlmeSetCnf1;
                pCallBack->mode.adp.R_G3MAC_MlmeTmrReceiveInd                 =   R_G3MAC_MlmeTmrReceiveInd1;
                pCallBack->mode.adp.R_G3MAC_MlmeTmrTransmitInd                =   R_G3MAC_MlmeTmrTransmitInd1;
            }
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
        }
        break;
        case R_MEDIA_TYPE2_BOTH:
        {
            if (0 == chId)
            {
                pCallBack->mode.hyadp.R_ADP_AdpdDataCnf                       =   R_ADP_AdpdDataCnf0;
                pCallBack->mode.hyadp.R_ADP_AdpmResetCnf                      =   R_ADP_AdpmResetCnf0;
                pCallBack->mode.hyadp.R_ADP_AdpmDiscoveryCnf                  =   R_ADP_AdpmDiscoveryCnf0;
                pCallBack->mode.hyadp.R_ADP_AdpmNetworkStartCnf               =   R_ADP_AdpmNetworkStartCnf0;
                pCallBack->mode.hyadp.R_ADP_AdpmNetworkJoinCnf                =   R_ADP_AdpmNetworkJoinCnf0;
                pCallBack->mode.hyadp.R_ADP_AdpmNetworkLeaveCnf               =   R_ADP_AdpmNetworkLeaveCnf0;
                pCallBack->mode.hyadp.R_ADP_AdpmGetCnf                        =   R_ADP_AdpmGetCnf0;
                pCallBack->mode.hyadp.R_ADP_AdpmSetCnf                        =   R_ADP_AdpmSetCnf0;
                pCallBack->mode.hyadp.R_ADP_AdpmRouteDiscoveryCnf             =   R_ADP_AdpmRouteDiscoveryCnf0;
                pCallBack->mode.hyadp.R_ADP_AdpmPathDiscoveryCnf              =   R_ADP_AdpmPathDiscoveryCnf0;
                pCallBack->mode.hyadp.R_ADP_AdpmLbpCnf                        =   R_ADP_AdpmLbpCnf0;
                pCallBack->mode.hyadp.R_ADP_AdpdDataInd                       =   R_ADP_AdpdDataInd0;
                pCallBack->mode.hyadp.R_ADP_AdpmNetworkLeaveInd               =   R_ADP_AdpmNetworkLeaveInd0;
                pCallBack->mode.hyadp.R_ADP_AdpmPathDiscoveryInd              =   R_ADP_AdpmPathDiscoveryInd0;
                pCallBack->mode.hyadp.R_ADP_AdpmLbpInd                        =   R_ADP_AdpmLbpInd0;
                pCallBack->mode.hyadp.R_ADP_AdpmNetworkStatusInd              =   R_ADP_AdpmNetworkStatusInd0;
                pCallBack->mode.hyadp.R_ADP_AdpmBufferInd                     =   R_ADP_AdpmBufferInd0;
                pCallBack->mode.hyadp.R_ADP_AdpmKeyStateInd                   =   R_ADP_AdpmKeyStateInd0;
                pCallBack->mode.hyadp.R_ADP_AdpmRouteErrorInd                 =   R_ADP_AdpmRouteErrorInd0;
                pCallBack->mode.hyadp.R_ADP_AdpmEapKeyInd                     =   R_ADP_AdpmEapKeyInd0;
                pCallBack->mode.hyadp.R_ADP_AdpmFrameCounterInd               =   R_ADP_AdpmFrameCounterInd0;
                pCallBack->mode.hyadp.R_ADP_AdpmRouteUpdateInd                =   R_ADP_AdpmRouteUpdateInd0;
                pCallBack->mode.hyadp.R_ADP_AdpmLoadSeqNumInd                 =   R_ADP_AdpmLoadSeqNumInd0;
                pCallBack->mode.hyadp.R_ADP_AdpmRrepInd                       =   R_ADP_AdpmRrepInd0;
                pCallBack->mode.hyadp.R_ADP_AdpmBeaconNotifyInd               =   R_ADP_AdpmBeaconNotifyInd0;
                pCallBack->mode.hyadp.R_G3MAC_MlmeGetCnf                      =   R_G3MAC_MlmeGetCnf0;
                pCallBack->mode.hyadp.R_G3MAC_MlmeSetCnf                      =   R_G3MAC_MlmeSetCnf0;
                pCallBack->mode.hyadp.R_G3MAC_MlmeTmrReceiveInd               =   R_G3MAC_MlmeTmrReceiveInd0;
                pCallBack->mode.hyadp.R_G3MAC_MlmeTmrTransmitInd              =   R_G3MAC_MlmeTmrTransmitInd0;
                pCallBack->mode.hyadp.R_G3RFMAC_MlmeGetCnf                    =   R_G3RFMAC_MlmeGetCnf0;
                pCallBack->mode.hyadp.R_G3RFMAC_MlmeSetCnf                    =   R_G3RFMAC_MlmeSetCnf0;
            }
            else
            {
                pCallBack->mode.hyadp.R_ADP_AdpdDataCnf                       =   R_ADP_AdpdDataCnf1;
                pCallBack->mode.hyadp.R_ADP_AdpmResetCnf                      =   R_ADP_AdpmResetCnf1;
                pCallBack->mode.hyadp.R_ADP_AdpmDiscoveryCnf                  =   R_ADP_AdpmDiscoveryCnf1;
                pCallBack->mode.hyadp.R_ADP_AdpmNetworkStartCnf               =   R_ADP_AdpmNetworkStartCnf1;
                pCallBack->mode.hyadp.R_ADP_AdpmNetworkJoinCnf                =   R_ADP_AdpmNetworkJoinCnf1;
                pCallBack->mode.hyadp.R_ADP_AdpmNetworkLeaveCnf               =   R_ADP_AdpmNetworkLeaveCnf1;
                pCallBack->mode.hyadp.R_ADP_AdpmGetCnf                        =   R_ADP_AdpmGetCnf1;
                pCallBack->mode.hyadp.R_ADP_AdpmSetCnf                        =   R_ADP_AdpmSetCnf1;
                pCallBack->mode.hyadp.R_ADP_AdpmRouteDiscoveryCnf             =   R_ADP_AdpmRouteDiscoveryCnf1;
                pCallBack->mode.hyadp.R_ADP_AdpmPathDiscoveryCnf              =   R_ADP_AdpmPathDiscoveryCnf1;
                pCallBack->mode.hyadp.R_ADP_AdpmLbpCnf                        =   R_ADP_AdpmLbpCnf1;
                pCallBack->mode.hyadp.R_ADP_AdpdDataInd                       =   R_ADP_AdpdDataInd1;
                pCallBack->mode.hyadp.R_ADP_AdpmNetworkLeaveInd               =   R_ADP_AdpmNetworkLeaveInd1;
                pCallBack->mode.hyadp.R_ADP_AdpmPathDiscoveryInd              =   R_ADP_AdpmPathDiscoveryInd1;
                pCallBack->mode.hyadp.R_ADP_AdpmLbpInd                        =   R_ADP_AdpmLbpInd1;
                pCallBack->mode.hyadp.R_ADP_AdpmNetworkStatusInd              =   R_ADP_AdpmNetworkStatusInd1;
                pCallBack->mode.hyadp.R_ADP_AdpmBufferInd                     =   R_ADP_AdpmBufferInd1;
                pCallBack->mode.hyadp.R_ADP_AdpmKeyStateInd                   =   R_ADP_AdpmKeyStateInd1;
                pCallBack->mode.hyadp.R_ADP_AdpmRouteErrorInd                 =   R_ADP_AdpmRouteErrorInd1;
                pCallBack->mode.hyadp.R_ADP_AdpmEapKeyInd                     =   R_ADP_AdpmEapKeyInd1;
                pCallBack->mode.hyadp.R_ADP_AdpmFrameCounterInd               =   R_ADP_AdpmFrameCounterInd1;
                pCallBack->mode.hyadp.R_ADP_AdpmRouteUpdateInd                =   R_ADP_AdpmRouteUpdateInd1;
                pCallBack->mode.hyadp.R_ADP_AdpmLoadSeqNumInd                 =   R_ADP_AdpmLoadSeqNumInd1;
                pCallBack->mode.hyadp.R_ADP_AdpmRrepInd                       =   R_ADP_AdpmRrepInd1;
                pCallBack->mode.hyadp.R_ADP_AdpmBeaconNotifyInd               =   R_ADP_AdpmBeaconNotifyInd1;
                pCallBack->mode.hyadp.R_G3MAC_MlmeGetCnf                      =   R_G3MAC_MlmeGetCnf1;
                pCallBack->mode.hyadp.R_G3MAC_MlmeSetCnf                      =   R_G3MAC_MlmeSetCnf1;
                pCallBack->mode.hyadp.R_G3MAC_MlmeTmrReceiveInd               =   R_G3MAC_MlmeTmrReceiveInd1;
                pCallBack->mode.hyadp.R_G3MAC_MlmeTmrTransmitInd              =   R_G3MAC_MlmeTmrTransmitInd1;
                pCallBack->mode.hyadp.R_G3RFMAC_MlmeGetCnf                    =   R_G3RFMAC_MlmeGetCnf1;
                pCallBack->mode.hyadp.R_G3RFMAC_MlmeSetCnf                    =   R_G3RFMAC_MlmeSetCnf1;
            }
        }
        break;
        case R_MEDIA_TYPE1_RF:
        {
            if (0 == chId)
            {
                pCallBack->mode.rfadp.R_ADP_AdpdDataCnf                       =   R_ADP_AdpdDataCnf0;
                pCallBack->mode.rfadp.R_ADP_AdpmResetCnf                      =   R_ADP_AdpmResetCnf0;
                pCallBack->mode.rfadp.R_ADP_AdpmDiscoveryCnf                  =   R_ADP_AdpmDiscoveryCnf0;
                pCallBack->mode.rfadp.R_ADP_AdpmNetworkStartCnf               =   R_ADP_AdpmNetworkStartCnf0;
                pCallBack->mode.rfadp.R_ADP_AdpmNetworkJoinCnf                =   R_ADP_AdpmNetworkJoinCnf0;
                pCallBack->mode.rfadp.R_ADP_AdpmNetworkLeaveCnf               =   R_ADP_AdpmNetworkLeaveCnf0;
                pCallBack->mode.rfadp.R_ADP_AdpmGetCnf                        =   R_ADP_AdpmGetCnf0;
                pCallBack->mode.rfadp.R_ADP_AdpmSetCnf                        =   R_ADP_AdpmSetCnf0;
                pCallBack->mode.rfadp.R_ADP_AdpmRouteDiscoveryCnf             =   R_ADP_AdpmRouteDiscoveryCnf0;
                pCallBack->mode.rfadp.R_ADP_AdpmPathDiscoveryCnf              =   R_ADP_AdpmPathDiscoveryCnf0;
                pCallBack->mode.rfadp.R_ADP_AdpmLbpCnf                        =   R_ADP_AdpmLbpCnf0;
                pCallBack->mode.rfadp.R_ADP_AdpdDataInd                       =   R_ADP_AdpdDataInd0;
                pCallBack->mode.rfadp.R_ADP_AdpmNetworkLeaveInd               =   R_ADP_AdpmNetworkLeaveInd0;
                pCallBack->mode.rfadp.R_ADP_AdpmPathDiscoveryInd              =   R_ADP_AdpmPathDiscoveryInd0;
                pCallBack->mode.rfadp.R_ADP_AdpmLbpInd                        =   R_ADP_AdpmLbpInd0;
                pCallBack->mode.rfadp.R_ADP_AdpmNetworkStatusInd              =   R_ADP_AdpmNetworkStatusInd0;
                pCallBack->mode.rfadp.R_ADP_AdpmBufferInd                     =   R_ADP_AdpmBufferInd0;
                pCallBack->mode.rfadp.R_ADP_AdpmKeyStateInd                   =   R_ADP_AdpmKeyStateInd0;
                pCallBack->mode.rfadp.R_ADP_AdpmRouteErrorInd                 =   R_ADP_AdpmRouteErrorInd0;
                pCallBack->mode.rfadp.R_ADP_AdpmEapKeyInd                     =   R_ADP_AdpmEapKeyInd0;
                pCallBack->mode.rfadp.R_ADP_AdpmFrameCounterInd               =   R_ADP_AdpmFrameCounterInd0;
                pCallBack->mode.rfadp.R_ADP_AdpmRouteUpdateInd                =   R_ADP_AdpmRouteUpdateInd0;
                pCallBack->mode.rfadp.R_ADP_AdpmLoadSeqNumInd                 =   R_ADP_AdpmLoadSeqNumInd0;
                pCallBack->mode.rfadp.R_ADP_AdpmRrepInd                       =   R_ADP_AdpmRrepInd0;
                pCallBack->mode.rfadp.R_ADP_AdpmBeaconNotifyInd               =   R_ADP_AdpmBeaconNotifyInd0;
                pCallBack->mode.rfadp.R_G3RFMAC_MlmeGetCnf                    =   R_G3RFMAC_MlmeGetCnf0;
                pCallBack->mode.rfadp.R_G3RFMAC_MlmeSetCnf                    =   R_G3RFMAC_MlmeSetCnf0;
            }
            else
            {
                pCallBack->mode.rfadp.R_ADP_AdpdDataCnf                       =   R_ADP_AdpdDataCnf1;
                pCallBack->mode.rfadp.R_ADP_AdpmResetCnf                      =   R_ADP_AdpmResetCnf1;
                pCallBack->mode.rfadp.R_ADP_AdpmDiscoveryCnf                  =   R_ADP_AdpmDiscoveryCnf1;
                pCallBack->mode.rfadp.R_ADP_AdpmNetworkStartCnf               =   R_ADP_AdpmNetworkStartCnf1;
                pCallBack->mode.rfadp.R_ADP_AdpmNetworkJoinCnf                =   R_ADP_AdpmNetworkJoinCnf1;
                pCallBack->mode.rfadp.R_ADP_AdpmNetworkLeaveCnf               =   R_ADP_AdpmNetworkLeaveCnf1;
                pCallBack->mode.rfadp.R_ADP_AdpmGetCnf                        =   R_ADP_AdpmGetCnf1;
                pCallBack->mode.rfadp.R_ADP_AdpmSetCnf                        =   R_ADP_AdpmSetCnf1;
                pCallBack->mode.rfadp.R_ADP_AdpmRouteDiscoveryCnf             =   R_ADP_AdpmRouteDiscoveryCnf1;
                pCallBack->mode.rfadp.R_ADP_AdpmPathDiscoveryCnf              =   R_ADP_AdpmPathDiscoveryCnf1;
                pCallBack->mode.rfadp.R_ADP_AdpmLbpCnf                        =   R_ADP_AdpmLbpCnf1;
                pCallBack->mode.rfadp.R_ADP_AdpdDataInd                       =   R_ADP_AdpdDataInd1;
                pCallBack->mode.rfadp.R_ADP_AdpmNetworkLeaveInd               =   R_ADP_AdpmNetworkLeaveInd1;
                pCallBack->mode.rfadp.R_ADP_AdpmPathDiscoveryInd              =   R_ADP_AdpmPathDiscoveryInd1;
                pCallBack->mode.rfadp.R_ADP_AdpmLbpInd                        =   R_ADP_AdpmLbpInd1;
                pCallBack->mode.rfadp.R_ADP_AdpmNetworkStatusInd              =   R_ADP_AdpmNetworkStatusInd1;
                pCallBack->mode.rfadp.R_ADP_AdpmBufferInd                     =   R_ADP_AdpmBufferInd1;
                pCallBack->mode.rfadp.R_ADP_AdpmKeyStateInd                   =   R_ADP_AdpmKeyStateInd1;
                pCallBack->mode.rfadp.R_ADP_AdpmRouteErrorInd                 =   R_ADP_AdpmRouteErrorInd1;
                pCallBack->mode.rfadp.R_ADP_AdpmEapKeyInd                     =   R_ADP_AdpmEapKeyInd1;
                pCallBack->mode.rfadp.R_ADP_AdpmFrameCounterInd               =   R_ADP_AdpmFrameCounterInd1;
                pCallBack->mode.rfadp.R_ADP_AdpmRouteUpdateInd                =   R_ADP_AdpmRouteUpdateInd1;
                pCallBack->mode.rfadp.R_ADP_AdpmLoadSeqNumInd                 =   R_ADP_AdpmLoadSeqNumInd1;
                pCallBack->mode.rfadp.R_ADP_AdpmRrepInd                       =   R_ADP_AdpmRrepInd1;
                pCallBack->mode.rfadp.R_ADP_AdpmBeaconNotifyInd               =   R_ADP_AdpmBeaconNotifyInd1;
                pCallBack->mode.rfadp.R_G3RFMAC_MlmeGetCnf                    =   R_G3RFMAC_MlmeGetCnf1;
                pCallBack->mode.rfadp.R_G3RFMAC_MlmeSetCnf                    =   R_G3RFMAC_MlmeSetCnf1;
            }
        }
        break;
        default:
        {
            /*do nothing*/
        }
        break;
    }
#endif /* R_DEFINE_DISABLE_HYBRID R_DEFINE_DISABLE_APP_HYBRID */

    return R_RESULT_SUCCESS;
} /* R_DEMO_InitAdpCallBack */
/******************************************************************************
   End of function  R_DEMO_InitAdpCallBack
******************************************************************************/

/******************************************************************************
* Function Name: R_DEMO_InitEapCallBack
* Description :
* Arguments :
* Return Value :
******************************************************************************/
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
r_result_t R_DEMO_InitEapCallBack (uint8_t chId, r_g3_callback_t * pCallBack, uint8_t mediaType)
#else
r_result_t R_DEMO_InitEapCallBack (uint8_t chId, r_g3_callback_t * pCallBack)
#endif
{

    if (NULL == pCallBack)
    {
        return R_RESULT_ILLEGAL_NULL_POINTER;
    }

    R_memset ((uint8_t *)pCallBack, 0, sizeof (r_g3_callback_t));

    R_DEMO_InitG3CallBack ( chId, pCallBack );

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    switch(mediaType)
    {
        case R_MEDIA_TYPE0_PLC:
        {
#endif
            if (0 == chId)
            {
                pCallBack->mode.eap.R_EAP_EapmGetCnf                          =   R_EAP_EapmGetCnf0;
                pCallBack->mode.eap.R_EAP_EapmNetworkCnf                      =   R_EAP_EapmNetworkCnf0;
                pCallBack->mode.eap.R_EAP_EapmNetworkJoinInd                  =   R_EAP_EapmNetworkJoinInd0;
                pCallBack->mode.eap.R_EAP_EapmNetworkLeaveInd                 =   R_EAP_EapmNetworkLeaveInd0;
                pCallBack->mode.eap.R_EAP_EapmNewDeviceInd                    =   R_EAP_EapmNewDeviceInd0;
                pCallBack->mode.eap.R_EAP_EapmResetCnf                        =   R_EAP_EapmResetCnf0;
                pCallBack->mode.eap.R_EAP_EapmSetClientinfoCnf                =   R_EAP_EapmSetClientInfoCnf0;
                pCallBack->mode.eap.R_EAP_EapmSetCnf                          =   R_EAP_EapmSetCnf0;
                pCallBack->mode.eap.R_EAP_EapmStartCnf                        =   R_EAP_EapmStartCnf0;
                pCallBack->mode.eap.R_EAP_EapmEapKeyInd                       =   R_EAP_EapmEapKeyInd0;

                pCallBack->mode.eap.R_ADP_AdpdDataCnf                         =   R_ADP_AdpdDataCnf0;
                pCallBack->mode.eap.R_ADP_AdpmResetCnf                        =   R_ADP_AdpmResetCnf0;
                pCallBack->mode.eap.R_ADP_AdpmDiscoveryCnf                    =   R_ADP_AdpmDiscoveryCnf0;
                pCallBack->mode.eap.R_ADP_AdpmNetworkStartCnf                 =   R_ADP_AdpmNetworkStartCnf0;
                pCallBack->mode.eap.R_ADP_AdpmNetworkJoinCnf                  =   R_ADP_AdpmNetworkJoinCnf0;
                pCallBack->mode.eap.R_ADP_AdpmNetworkLeaveCnf                 =   R_ADP_AdpmNetworkLeaveCnf0;
                pCallBack->mode.eap.R_ADP_AdpmGetCnf                          =   R_ADP_AdpmGetCnf0;
                pCallBack->mode.eap.R_ADP_AdpmSetCnf                          =   R_ADP_AdpmSetCnf0;
                pCallBack->mode.eap.R_ADP_AdpmRouteDiscoveryCnf               =   R_ADP_AdpmRouteDiscoveryCnf0;
                pCallBack->mode.eap.R_ADP_AdpmPathDiscoveryCnf                =   R_ADP_AdpmPathDiscoveryCnf0;
                pCallBack->mode.eap.R_ADP_AdpdDataInd                         =   R_ADP_AdpdDataInd0;
                pCallBack->mode.eap.R_ADP_AdpmNetworkLeaveInd                 =   R_ADP_AdpmNetworkLeaveInd0;
                pCallBack->mode.eap.R_ADP_AdpmPathDiscoveryInd                =   R_ADP_AdpmPathDiscoveryInd0;
                pCallBack->mode.eap.R_ADP_AdpmNetworkStatusInd                =   R_ADP_AdpmNetworkStatusInd0;
                pCallBack->mode.eap.R_ADP_AdpmBufferInd                       =   R_ADP_AdpmBufferInd0;
                pCallBack->mode.eap.R_ADP_AdpmKeyStateInd                     =   R_ADP_AdpmKeyStateInd0;
                pCallBack->mode.eap.R_ADP_AdpmRouteErrorInd                   =   R_ADP_AdpmRouteErrorInd0;
                pCallBack->mode.eap.R_ADP_AdpmEapKeyInd                       =   R_ADP_AdpmEapKeyInd0;
                pCallBack->mode.eap.R_ADP_AdpmFrameCounterInd                 =   R_ADP_AdpmFrameCounterInd0;
                pCallBack->mode.eap.R_ADP_AdpmRouteUpdateInd                  =   R_ADP_AdpmRouteUpdateInd0;
                pCallBack->mode.eap.R_ADP_AdpmLoadSeqNumInd                   =   R_ADP_AdpmLoadSeqNumInd0;
                pCallBack->mode.eap.R_ADP_AdpmRrepInd                         =   R_ADP_AdpmRrepInd0;
                pCallBack->mode.eap.R_ADP_AdpmBeaconNotifyInd                 =   R_ADP_AdpmBeaconNotifyInd0;
                pCallBack->mode.eap.R_G3MAC_MlmeGetCnf                        =   R_G3MAC_MlmeGetCnf0;
                pCallBack->mode.eap.R_G3MAC_MlmeSetCnf                        =   R_G3MAC_MlmeSetCnf0;
                pCallBack->mode.eap.R_G3MAC_MlmeTmrReceiveInd                 =   R_G3MAC_MlmeTmrReceiveInd0;
                pCallBack->mode.eap.R_G3MAC_MlmeTmrTransmitInd                =   R_G3MAC_MlmeTmrTransmitInd0;
            }
            else
            {
                pCallBack->mode.eap.R_EAP_EapmGetCnf                          =   R_EAP_EapmGetCnf1;
                pCallBack->mode.eap.R_EAP_EapmNetworkCnf                      =   R_EAP_EapmNetworkCnf1;
                pCallBack->mode.eap.R_EAP_EapmNetworkJoinInd                  =   R_EAP_EapmNetworkJoinInd1;
                pCallBack->mode.eap.R_EAP_EapmNetworkLeaveInd                 =   R_EAP_EapmNetworkLeaveInd1;
                pCallBack->mode.eap.R_EAP_EapmNewDeviceInd                    =   R_EAP_EapmNewDeviceInd1;
                pCallBack->mode.eap.R_EAP_EapmResetCnf                        =   R_EAP_EapmResetCnf1;
                pCallBack->mode.eap.R_EAP_EapmSetClientinfoCnf                =   R_EAP_EapmSetClientInfoCnf1;
                pCallBack->mode.eap.R_EAP_EapmSetCnf                          =   R_EAP_EapmSetCnf1;
                pCallBack->mode.eap.R_EAP_EapmStartCnf                        =   R_EAP_EapmStartCnf1;
                pCallBack->mode.eap.R_EAP_EapmEapKeyInd                       =   R_EAP_EapmEapKeyInd1;

                pCallBack->mode.eap.R_ADP_AdpdDataCnf                         =   R_ADP_AdpdDataCnf1;
                pCallBack->mode.eap.R_ADP_AdpmResetCnf                        =   R_ADP_AdpmResetCnf1;
                pCallBack->mode.eap.R_ADP_AdpmDiscoveryCnf                    =   R_ADP_AdpmDiscoveryCnf1;
                pCallBack->mode.eap.R_ADP_AdpmNetworkStartCnf                 =   R_ADP_AdpmNetworkStartCnf1;
                pCallBack->mode.eap.R_ADP_AdpmNetworkJoinCnf                  =   R_ADP_AdpmNetworkJoinCnf1;
                pCallBack->mode.eap.R_ADP_AdpmNetworkLeaveCnf                 =   R_ADP_AdpmNetworkLeaveCnf1;
                pCallBack->mode.eap.R_ADP_AdpmGetCnf                          =   R_ADP_AdpmGetCnf1;
                pCallBack->mode.eap.R_ADP_AdpmSetCnf                          =   R_ADP_AdpmSetCnf1;
                pCallBack->mode.eap.R_ADP_AdpmRouteDiscoveryCnf               =   R_ADP_AdpmRouteDiscoveryCnf1;
                pCallBack->mode.eap.R_ADP_AdpmPathDiscoveryCnf                =   R_ADP_AdpmPathDiscoveryCnf1;
                pCallBack->mode.eap.R_ADP_AdpdDataInd                         =   R_ADP_AdpdDataInd1;
                pCallBack->mode.eap.R_ADP_AdpmNetworkLeaveInd                 =   R_ADP_AdpmNetworkLeaveInd1;
                pCallBack->mode.eap.R_ADP_AdpmPathDiscoveryInd                =   R_ADP_AdpmPathDiscoveryInd1;
                pCallBack->mode.eap.R_ADP_AdpmNetworkStatusInd                =   R_ADP_AdpmNetworkStatusInd1;
                pCallBack->mode.eap.R_ADP_AdpmBufferInd                       =   R_ADP_AdpmBufferInd1;
                pCallBack->mode.eap.R_ADP_AdpmKeyStateInd                     =   R_ADP_AdpmKeyStateInd1;
                pCallBack->mode.eap.R_ADP_AdpmRouteErrorInd                   =   R_ADP_AdpmRouteErrorInd1;
                pCallBack->mode.eap.R_ADP_AdpmEapKeyInd                       =   R_ADP_AdpmEapKeyInd1;
                pCallBack->mode.eap.R_ADP_AdpmFrameCounterInd                 =   R_ADP_AdpmFrameCounterInd1;
                pCallBack->mode.eap.R_ADP_AdpmRouteUpdateInd                  =   R_ADP_AdpmRouteUpdateInd1;
                pCallBack->mode.eap.R_ADP_AdpmLoadSeqNumInd                   =   R_ADP_AdpmLoadSeqNumInd1;
                pCallBack->mode.eap.R_ADP_AdpmRrepInd                         =   R_ADP_AdpmRrepInd1;
                pCallBack->mode.eap.R_ADP_AdpmBeaconNotifyInd                 =   R_ADP_AdpmBeaconNotifyInd1;

                pCallBack->mode.eap.R_G3MAC_MlmeGetCnf                        =   R_G3MAC_MlmeGetCnf1;
                pCallBack->mode.eap.R_G3MAC_MlmeSetCnf                        =   R_G3MAC_MlmeSetCnf1;
                pCallBack->mode.eap.R_G3MAC_MlmeTmrReceiveInd                 =   R_G3MAC_MlmeTmrReceiveInd1;
                pCallBack->mode.eap.R_G3MAC_MlmeTmrTransmitInd                =   R_G3MAC_MlmeTmrTransmitInd1;

            }
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
        }
        break;
        case R_MEDIA_TYPE2_BOTH:
        {
            if (0 == chId)
            {
                pCallBack->mode.hyeap.R_EAP_EapmGetCnf                        =   R_EAP_EapmGetCnf0;
                pCallBack->mode.hyeap.R_EAP_EapmNetworkCnf                    =   R_EAP_EapmNetworkCnf0;
                pCallBack->mode.hyeap.R_EAP_EapmNetworkJoinInd                =   R_EAP_EapmNetworkJoinInd0;
                pCallBack->mode.hyeap.R_EAP_EapmNetworkLeaveInd               =   R_EAP_EapmNetworkLeaveInd0;
                pCallBack->mode.hyeap.R_EAP_EapmNewDeviceInd                  =   R_EAP_EapmNewDeviceInd0;
                pCallBack->mode.hyeap.R_EAP_EapmResetCnf                      =   R_EAP_EapmResetCnf0;
                pCallBack->mode.hyeap.R_EAP_EapmSetClientinfoCnf              =   R_EAP_EapmSetClientInfoCnf0;
                pCallBack->mode.hyeap.R_EAP_EapmSetCnf                        =   R_EAP_EapmSetCnf0;
                pCallBack->mode.hyeap.R_EAP_EapmStartCnf                      =   R_EAP_EapmStartCnf0;
                pCallBack->mode.hyeap.R_EAP_EapmEapKeyInd                     =   R_EAP_EapmEapKeyInd0;

                pCallBack->mode.hyeap.R_ADP_AdpdDataCnf                       =   R_ADP_AdpdDataCnf0;
                pCallBack->mode.hyeap.R_ADP_AdpmResetCnf                      =   R_ADP_AdpmResetCnf0;
                pCallBack->mode.hyeap.R_ADP_AdpmDiscoveryCnf                  =   R_ADP_AdpmDiscoveryCnf0;
                pCallBack->mode.hyeap.R_ADP_AdpmNetworkStartCnf               =   R_ADP_AdpmNetworkStartCnf0;
                pCallBack->mode.hyeap.R_ADP_AdpmNetworkJoinCnf                =   R_ADP_AdpmNetworkJoinCnf0;
                pCallBack->mode.hyeap.R_ADP_AdpmNetworkLeaveCnf               =   R_ADP_AdpmNetworkLeaveCnf0;
                pCallBack->mode.hyeap.R_ADP_AdpmGetCnf                        =   R_ADP_AdpmGetCnf0;
                pCallBack->mode.hyeap.R_ADP_AdpmSetCnf                        =   R_ADP_AdpmSetCnf0;
                pCallBack->mode.hyeap.R_ADP_AdpmRouteDiscoveryCnf             =   R_ADP_AdpmRouteDiscoveryCnf0;
                pCallBack->mode.hyeap.R_ADP_AdpmPathDiscoveryCnf              =   R_ADP_AdpmPathDiscoveryCnf0;
                pCallBack->mode.hyeap.R_ADP_AdpdDataInd                       =   R_ADP_AdpdDataInd0;
                pCallBack->mode.hyeap.R_ADP_AdpmNetworkLeaveInd               =   R_ADP_AdpmNetworkLeaveInd0;
                pCallBack->mode.hyeap.R_ADP_AdpmPathDiscoveryInd              =   R_ADP_AdpmPathDiscoveryInd0;
                pCallBack->mode.hyeap.R_ADP_AdpmNetworkStatusInd              =   R_ADP_AdpmNetworkStatusInd0;
                pCallBack->mode.hyeap.R_ADP_AdpmBufferInd                     =   R_ADP_AdpmBufferInd0;
                pCallBack->mode.hyeap.R_ADP_AdpmKeyStateInd                   =   R_ADP_AdpmKeyStateInd0;
                pCallBack->mode.hyeap.R_ADP_AdpmRouteErrorInd                 =   R_ADP_AdpmRouteErrorInd0;
                pCallBack->mode.hyeap.R_ADP_AdpmEapKeyInd                     =   R_ADP_AdpmEapKeyInd0;
                pCallBack->mode.hyeap.R_ADP_AdpmFrameCounterInd               =   R_ADP_AdpmFrameCounterInd0;
                pCallBack->mode.hyeap.R_ADP_AdpmRouteUpdateInd                =   R_ADP_AdpmRouteUpdateInd0;
                pCallBack->mode.hyeap.R_ADP_AdpmLoadSeqNumInd                 =   R_ADP_AdpmLoadSeqNumInd0;
                pCallBack->mode.hyeap.R_ADP_AdpmRrepInd                       =   R_ADP_AdpmRrepInd0;
                pCallBack->mode.hyeap.R_ADP_AdpmBeaconNotifyInd               =   R_ADP_AdpmBeaconNotifyInd0;

                pCallBack->mode.hyeap.R_G3MAC_MlmeGetCnf                      =   R_G3MAC_MlmeGetCnf0;
                pCallBack->mode.hyeap.R_G3MAC_MlmeSetCnf                      =   R_G3MAC_MlmeSetCnf0;
                pCallBack->mode.hyeap.R_G3MAC_MlmeTmrReceiveInd               =   R_G3MAC_MlmeTmrReceiveInd0;
                pCallBack->mode.hyeap.R_G3MAC_MlmeTmrTransmitInd              =   R_G3MAC_MlmeTmrTransmitInd0;

                pCallBack->mode.hyeap.R_G3RFMAC_MlmeGetCnf                    =   R_G3RFMAC_MlmeGetCnf0;
                pCallBack->mode.hyeap.R_G3RFMAC_MlmeSetCnf                    =   R_G3RFMAC_MlmeSetCnf0;
            }
            else
            {
                pCallBack->mode.hyeap.R_EAP_EapmGetCnf                        =   R_EAP_EapmGetCnf1;
                pCallBack->mode.hyeap.R_EAP_EapmNetworkCnf                    =   R_EAP_EapmNetworkCnf1;
                pCallBack->mode.hyeap.R_EAP_EapmNetworkJoinInd                =   R_EAP_EapmNetworkJoinInd1;
                pCallBack->mode.hyeap.R_EAP_EapmNetworkLeaveInd               =   R_EAP_EapmNetworkLeaveInd1;
                pCallBack->mode.hyeap.R_EAP_EapmNewDeviceInd                  =   R_EAP_EapmNewDeviceInd1;
                pCallBack->mode.hyeap.R_EAP_EapmResetCnf                      =   R_EAP_EapmResetCnf1;
                pCallBack->mode.hyeap.R_EAP_EapmSetClientinfoCnf              =   R_EAP_EapmSetClientInfoCnf1;
                pCallBack->mode.hyeap.R_EAP_EapmSetCnf                        =   R_EAP_EapmSetCnf1;
                pCallBack->mode.hyeap.R_EAP_EapmStartCnf                      =   R_EAP_EapmStartCnf1;
                pCallBack->mode.hyeap.R_EAP_EapmEapKeyInd                     =   R_EAP_EapmEapKeyInd1;

                pCallBack->mode.hyeap.R_ADP_AdpdDataCnf                       =   R_ADP_AdpdDataCnf1;
                pCallBack->mode.hyeap.R_ADP_AdpmResetCnf                      =   R_ADP_AdpmResetCnf1;
                pCallBack->mode.hyeap.R_ADP_AdpmDiscoveryCnf                  =   R_ADP_AdpmDiscoveryCnf1;
                pCallBack->mode.hyeap.R_ADP_AdpmNetworkStartCnf               =   R_ADP_AdpmNetworkStartCnf1;
                pCallBack->mode.hyeap.R_ADP_AdpmNetworkJoinCnf                =   R_ADP_AdpmNetworkJoinCnf1;
                pCallBack->mode.hyeap.R_ADP_AdpmNetworkLeaveCnf               =   R_ADP_AdpmNetworkLeaveCnf1;
                pCallBack->mode.hyeap.R_ADP_AdpmGetCnf                        =   R_ADP_AdpmGetCnf1;
                pCallBack->mode.hyeap.R_ADP_AdpmSetCnf                        =   R_ADP_AdpmSetCnf1;
                pCallBack->mode.hyeap.R_ADP_AdpmRouteDiscoveryCnf             =   R_ADP_AdpmRouteDiscoveryCnf1;
                pCallBack->mode.hyeap.R_ADP_AdpmPathDiscoveryCnf              =   R_ADP_AdpmPathDiscoveryCnf1;
                pCallBack->mode.hyeap.R_ADP_AdpdDataInd                       =   R_ADP_AdpdDataInd1;
                pCallBack->mode.hyeap.R_ADP_AdpmNetworkLeaveInd               =   R_ADP_AdpmNetworkLeaveInd1;
                pCallBack->mode.hyeap.R_ADP_AdpmPathDiscoveryInd              =   R_ADP_AdpmPathDiscoveryInd1;
                pCallBack->mode.hyeap.R_ADP_AdpmNetworkStatusInd              =   R_ADP_AdpmNetworkStatusInd1;
                pCallBack->mode.hyeap.R_ADP_AdpmBufferInd                     =   R_ADP_AdpmBufferInd1;
                pCallBack->mode.hyeap.R_ADP_AdpmKeyStateInd                   =   R_ADP_AdpmKeyStateInd1;
                pCallBack->mode.hyeap.R_ADP_AdpmRouteErrorInd                 =   R_ADP_AdpmRouteErrorInd1;
                pCallBack->mode.hyeap.R_ADP_AdpmEapKeyInd                     =   R_ADP_AdpmEapKeyInd1;
                pCallBack->mode.hyeap.R_ADP_AdpmFrameCounterInd               =   R_ADP_AdpmFrameCounterInd1;
                pCallBack->mode.hyeap.R_ADP_AdpmRouteUpdateInd                =   R_ADP_AdpmRouteUpdateInd1;
                pCallBack->mode.hyeap.R_ADP_AdpmLoadSeqNumInd                 =   R_ADP_AdpmLoadSeqNumInd1;
                pCallBack->mode.hyeap.R_ADP_AdpmRrepInd                       =   R_ADP_AdpmRrepInd1;
                pCallBack->mode.hyeap.R_ADP_AdpmBeaconNotifyInd               =   R_ADP_AdpmBeaconNotifyInd1;

                pCallBack->mode.hyeap.R_G3MAC_MlmeGetCnf                      =   R_G3MAC_MlmeGetCnf1;
                pCallBack->mode.hyeap.R_G3MAC_MlmeSetCnf                      =   R_G3MAC_MlmeSetCnf1;
                pCallBack->mode.hyeap.R_G3MAC_MlmeTmrReceiveInd               =   R_G3MAC_MlmeTmrReceiveInd1;
                pCallBack->mode.hyeap.R_G3MAC_MlmeTmrTransmitInd              =   R_G3MAC_MlmeTmrTransmitInd1;

                pCallBack->mode.hyeap.R_G3RFMAC_MlmeGetCnf                    =   R_G3RFMAC_MlmeGetCnf1;
                pCallBack->mode.hyeap.R_G3RFMAC_MlmeSetCnf                    =   R_G3RFMAC_MlmeSetCnf1;
            }
        }
        break;
        case R_MEDIA_TYPE1_RF:
        {
            if (0 == chId)
            {
                pCallBack->mode.rfeap.R_EAP_EapmGetCnf                        =   R_EAP_EapmGetCnf0;
                pCallBack->mode.rfeap.R_EAP_EapmNetworkCnf                    =   R_EAP_EapmNetworkCnf0;
                pCallBack->mode.rfeap.R_EAP_EapmNetworkJoinInd                =   R_EAP_EapmNetworkJoinInd0;
                pCallBack->mode.rfeap.R_EAP_EapmNetworkLeaveInd               =   R_EAP_EapmNetworkLeaveInd0;
                pCallBack->mode.rfeap.R_EAP_EapmNewDeviceInd                  =   R_EAP_EapmNewDeviceInd0;
                pCallBack->mode.rfeap.R_EAP_EapmResetCnf                      =   R_EAP_EapmResetCnf0;
                pCallBack->mode.rfeap.R_EAP_EapmSetClientinfoCnf              =   R_EAP_EapmSetClientInfoCnf0;
                pCallBack->mode.rfeap.R_EAP_EapmSetCnf                        =   R_EAP_EapmSetCnf0;
                pCallBack->mode.rfeap.R_EAP_EapmStartCnf                      =   R_EAP_EapmStartCnf0;
                pCallBack->mode.rfeap.R_EAP_EapmEapKeyInd                     =   R_EAP_EapmEapKeyInd0;

                pCallBack->mode.rfeap.R_ADP_AdpdDataCnf                       =   R_ADP_AdpdDataCnf0;
                pCallBack->mode.rfeap.R_ADP_AdpmResetCnf                      =   R_ADP_AdpmResetCnf0;
                pCallBack->mode.rfeap.R_ADP_AdpmDiscoveryCnf                  =   R_ADP_AdpmDiscoveryCnf0;
                pCallBack->mode.rfeap.R_ADP_AdpmNetworkStartCnf               =   R_ADP_AdpmNetworkStartCnf0;
                pCallBack->mode.rfeap.R_ADP_AdpmNetworkJoinCnf                =   R_ADP_AdpmNetworkJoinCnf0;
                pCallBack->mode.rfeap.R_ADP_AdpmNetworkLeaveCnf               =   R_ADP_AdpmNetworkLeaveCnf0;
                pCallBack->mode.rfeap.R_ADP_AdpmGetCnf                        =   R_ADP_AdpmGetCnf0;
                pCallBack->mode.rfeap.R_ADP_AdpmSetCnf                        =   R_ADP_AdpmSetCnf0;
                pCallBack->mode.rfeap.R_ADP_AdpmRouteDiscoveryCnf             =   R_ADP_AdpmRouteDiscoveryCnf0;
                pCallBack->mode.rfeap.R_ADP_AdpmPathDiscoveryCnf              =   R_ADP_AdpmPathDiscoveryCnf0;
                pCallBack->mode.rfeap.R_ADP_AdpdDataInd                       =   R_ADP_AdpdDataInd0;
                pCallBack->mode.rfeap.R_ADP_AdpmNetworkLeaveInd               =   R_ADP_AdpmNetworkLeaveInd0;
                pCallBack->mode.rfeap.R_ADP_AdpmPathDiscoveryInd              =   R_ADP_AdpmPathDiscoveryInd0;
                pCallBack->mode.rfeap.R_ADP_AdpmNetworkStatusInd              =   R_ADP_AdpmNetworkStatusInd0;
                pCallBack->mode.rfeap.R_ADP_AdpmBufferInd                     =   R_ADP_AdpmBufferInd0;
                pCallBack->mode.rfeap.R_ADP_AdpmKeyStateInd                   =   R_ADP_AdpmKeyStateInd0;
                pCallBack->mode.rfeap.R_ADP_AdpmRouteErrorInd                 =   R_ADP_AdpmRouteErrorInd0;
                pCallBack->mode.rfeap.R_ADP_AdpmEapKeyInd                     =   R_ADP_AdpmEapKeyInd0;
                pCallBack->mode.rfeap.R_ADP_AdpmFrameCounterInd               =   R_ADP_AdpmFrameCounterInd0;
                pCallBack->mode.rfeap.R_ADP_AdpmRouteUpdateInd                =   R_ADP_AdpmRouteUpdateInd0;
                pCallBack->mode.rfeap.R_ADP_AdpmLoadSeqNumInd                 =   R_ADP_AdpmLoadSeqNumInd0;
                pCallBack->mode.rfeap.R_ADP_AdpmRrepInd                       =   R_ADP_AdpmRrepInd0;
                pCallBack->mode.rfeap.R_ADP_AdpmBeaconNotifyInd               =   R_ADP_AdpmBeaconNotifyInd0;

                pCallBack->mode.rfeap.R_G3RFMAC_MlmeGetCnf                    =   R_G3RFMAC_MlmeGetCnf0;
                pCallBack->mode.rfeap.R_G3RFMAC_MlmeSetCnf                    =   R_G3RFMAC_MlmeSetCnf0;
            }
            else
            {
                pCallBack->mode.rfeap.R_EAP_EapmGetCnf                        =   R_EAP_EapmGetCnf1;
                pCallBack->mode.rfeap.R_EAP_EapmNetworkCnf                    =   R_EAP_EapmNetworkCnf1;
                pCallBack->mode.rfeap.R_EAP_EapmNetworkJoinInd                =   R_EAP_EapmNetworkJoinInd1;
                pCallBack->mode.rfeap.R_EAP_EapmNetworkLeaveInd               =   R_EAP_EapmNetworkLeaveInd1;
                pCallBack->mode.rfeap.R_EAP_EapmNewDeviceInd                  =   R_EAP_EapmNewDeviceInd1;
                pCallBack->mode.rfeap.R_EAP_EapmResetCnf                      =   R_EAP_EapmResetCnf1;
                pCallBack->mode.rfeap.R_EAP_EapmSetClientinfoCnf              =   R_EAP_EapmSetClientInfoCnf1;
                pCallBack->mode.rfeap.R_EAP_EapmSetCnf                        =   R_EAP_EapmSetCnf1;
                pCallBack->mode.rfeap.R_EAP_EapmStartCnf                      =   R_EAP_EapmStartCnf1;
                pCallBack->mode.rfeap.R_EAP_EapmEapKeyInd                     =   R_EAP_EapmEapKeyInd1;

                pCallBack->mode.rfeap.R_ADP_AdpdDataCnf                       =   R_ADP_AdpdDataCnf1;
                pCallBack->mode.rfeap.R_ADP_AdpmResetCnf                      =   R_ADP_AdpmResetCnf1;
                pCallBack->mode.rfeap.R_ADP_AdpmDiscoveryCnf                  =   R_ADP_AdpmDiscoveryCnf1;
                pCallBack->mode.rfeap.R_ADP_AdpmNetworkStartCnf               =   R_ADP_AdpmNetworkStartCnf1;
                pCallBack->mode.rfeap.R_ADP_AdpmNetworkJoinCnf                =   R_ADP_AdpmNetworkJoinCnf1;
                pCallBack->mode.rfeap.R_ADP_AdpmNetworkLeaveCnf               =   R_ADP_AdpmNetworkLeaveCnf1;
                pCallBack->mode.rfeap.R_ADP_AdpmGetCnf                        =   R_ADP_AdpmGetCnf1;
                pCallBack->mode.rfeap.R_ADP_AdpmSetCnf                        =   R_ADP_AdpmSetCnf1;
                pCallBack->mode.rfeap.R_ADP_AdpmRouteDiscoveryCnf             =   R_ADP_AdpmRouteDiscoveryCnf1;
                pCallBack->mode.rfeap.R_ADP_AdpmPathDiscoveryCnf              =   R_ADP_AdpmPathDiscoveryCnf1;
                pCallBack->mode.rfeap.R_ADP_AdpdDataInd                       =   R_ADP_AdpdDataInd1;
                pCallBack->mode.rfeap.R_ADP_AdpmNetworkLeaveInd               =   R_ADP_AdpmNetworkLeaveInd1;
                pCallBack->mode.rfeap.R_ADP_AdpmPathDiscoveryInd              =   R_ADP_AdpmPathDiscoveryInd1;
                pCallBack->mode.rfeap.R_ADP_AdpmNetworkStatusInd              =   R_ADP_AdpmNetworkStatusInd1;
                pCallBack->mode.rfeap.R_ADP_AdpmBufferInd                     =   R_ADP_AdpmBufferInd1;
                pCallBack->mode.rfeap.R_ADP_AdpmKeyStateInd                   =   R_ADP_AdpmKeyStateInd1;
                pCallBack->mode.rfeap.R_ADP_AdpmRouteErrorInd                 =   R_ADP_AdpmRouteErrorInd1;
                pCallBack->mode.rfeap.R_ADP_AdpmEapKeyInd                     =   R_ADP_AdpmEapKeyInd1;
                pCallBack->mode.rfeap.R_ADP_AdpmFrameCounterInd               =   R_ADP_AdpmFrameCounterInd1;
                pCallBack->mode.rfeap.R_ADP_AdpmRouteUpdateInd                =   R_ADP_AdpmRouteUpdateInd1;
                pCallBack->mode.rfeap.R_ADP_AdpmLoadSeqNumInd                 =   R_ADP_AdpmLoadSeqNumInd1;
                pCallBack->mode.rfeap.R_ADP_AdpmRrepInd                       =   R_ADP_AdpmRrepInd1;
                pCallBack->mode.rfeap.R_ADP_AdpmBeaconNotifyInd               =   R_ADP_AdpmBeaconNotifyInd1;

                pCallBack->mode.rfeap.R_G3RFMAC_MlmeGetCnf                    =   R_G3RFMAC_MlmeGetCnf1;
                pCallBack->mode.rfeap.R_G3RFMAC_MlmeSetCnf                    =   R_G3RFMAC_MlmeSetCnf1;
            }
        }
        break;
        default:
        {
            /*do nothing*/
        }
        break;
    }
#endif /* R_DEFINE_DISABLE_HYBRID R_DEFINE_DISABLE_APP_HYBRID */

    return R_RESULT_SUCCESS;
} /* R_DEMO_InitEapCallBack */
/******************************************************************************
   End of function  R_DEMO_InitEapCallBack
******************************************************************************/

