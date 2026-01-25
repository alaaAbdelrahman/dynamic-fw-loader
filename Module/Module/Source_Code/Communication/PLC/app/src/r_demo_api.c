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
 * File Name   : r_demo_api.c
 *    @version
 *        $Rev: 11089 $
 *    @last editor
 *        $Author: a5089763 $
 *    @date
 *        $Date:: 2022-07-12 11:09:31 +0900#$
 * Description :
 ******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include "r_typedefs.h"
#include "r_stdio_api.h"
#include "r_byte_swap.h"
#if !defined(R_DEFINE_OPEN_RX)
#include "r_app_timer.h"
#endif
/* g3 part */
#include "r_c3sap_api.h"
#include "r_g3_param2text.h"

/* app part */
#include "r_demo_tools.h"
#include "r_demo_api.h"
#include "r_demo_app.h"
#include "r_demo_app_thread.h"
#include "r_demo_parameters.h"
#include "r_demo_os_wrapper.h"

volatile void *current_config_flag = NULL;
uint32_t current_config_id = 0;

extern uint8_t config_monitor;
extern uint8_t current_AppResetTask;
extern uint8_t current_AppSetIpContent;

uint8_t send_discovery_req_flag = 1;

/******************************************************************************
Macro definitions
******************************************************************************/
/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Exported global variables
******************************************************************************/
extern r_demo_config_t g_demo_config;
extern volatile r_demo_g3_cb_str_t g_g3cb[R_G3_CH_MAX];

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/
/******************************************************************************
Private global variables and functions
******************************************************************************/

/******************************************************************************
Functions
******************************************************************************/

/*===========================================================================*/
/*    G3CTRL APIs                                                            */
/*===========================================================================*/

/******************************************************************************
 * Function Name: R_DEMO_G3SetConfig
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_G3SetConfig(uint8_t chId, r_g3_set_config_req_t *config,
                              r_g3_set_config_cnf_t **cnf)
{
    r_result_t status;
    R_VOLATILE r_g3_set_config_cnf_t *cfgCfm = (R_VOLATILE r_g3_set_config_cnf_t *)&g_g3cb[chId].setConfig;
    *cnf = (r_g3_set_config_cnf_t *)cfgCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_SET_CONFIG);

    status = R_G3_SetConfigReq(chId, config);
    if (R_RESULT_SUCCESS != status)
    {
        return R_RESULT_FAILED;
    }

    else
    {
        current_config_flag = &g_g3cb[chId].statusFlags;
        current_config_id = R_FLAG_SET_CONFIG;
        current_AppResetTask = CPX3_ADPM_RESET;
        config_monitor = 1;
    }
    return R_RESULT_SUCCESS;
} /* R_DEMO_G3SetConfig */
/******************************************************************************
   End of function  R_DEMO_G3SetConfig
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_G3GetInfo
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_G3GetInfo(uint8_t chId, r_g3_get_info_req_t *req,
                            r_g3_get_info_cnf_t **cnf)
{
    r_result_t status;
    R_VOLATILE r_g3_get_info_cnf_t *getInfoCfm = (R_VOLATILE r_g3_get_info_cnf_t *)&g_g3cb[chId].getInfo;
    *cnf = (r_g3_get_info_cnf_t *)getInfoCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_GET_INFO);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> G3-GetInfo requesting... ");
#endif
    }

    status = R_G3_GetInfoReq(chId, req);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_G3_GetInfoReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_GET_INFO);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_G3_STATUS_SUCCESS == getInfoCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("success. \n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(0, getInfoCfm->status), getInfoCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_G3GetInfo */
/******************************************************************************
   End of function  R_DEMO_G3GetInfo
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_G3ClearInfo
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_G3ClearInfo(uint8_t chId, r_g3_clear_info_req_t *req,
                              r_g3_clear_info_cnf_t **cnf)
{
    r_result_t status;
    R_VOLATILE r_g3_clear_info_cnf_t *clrInfoCfm = (R_VOLATILE r_g3_clear_info_cnf_t *)&g_g3cb[chId].clrInfo;
    *cnf = (r_g3_clear_info_cnf_t *)clrInfoCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_CLEAR_INFO);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> G3-ClearInfo requesting... ");
#endif
    }

    status = R_G3_ClearInfoReq(chId, req);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_G3_ClearInfoReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_CLEAR_INFO);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_G3_STATUS_SUCCESS == clrInfoCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("success. \n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(0, clrInfoCfm->status), clrInfoCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_G3ClearInfo */
/******************************************************************************
   End of function  R_DEMO_G3ClearInfo
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_G3Dump
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_G3Dump(uint32_t dump_timeout)
{
    uint16_t last_num;
    uint32_t dump_size = 0;
    uint16_t seg_cnt = 0;
    r_g3_dump_req_t req;
    R_VOLATILE r_g3_dump_cnf_t *dumpCfm = (R_VOLATILE r_g3_dump_cnf_t *)&g_g3cb[0].dumpCnf;
    R_VOLATILE r_g3_dump_ind_t *dumpInd = (R_VOLATILE r_g3_dump_ind_t *)&g_g3cb[0].dumpInd;
    uint16_t i;
    r_result_t status;
#if defined(R_DEFINE_OPEN_RX)
    uint64_t expire_tick_count;
    uint64_t now_tick_count;
#else
    uint32_t expire_tick_count;
#endif
    R_memset((uint8_t *)&req, 0, sizeof(r_g3_dump_req_t));

    R_DEMO_ClearEventFlag(&g_g3cb[0].statusFlags, R_FLAG_DUMP);

    /* Set dump request parameter */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
        req.segmentInterval = R_APP_DUMP_INTERVAL_VERBOSE_ON;
    }
    else
    {
        req.segmentInterval = R_APP_DUMP_INTERVAL_VERBOSE_OFF;
    }
    req.maxDmpLen = (500U * 1024U); /* 500KB */
    req.timeOut = dump_timeout;
    req.dmpMode = 0U;       /* AGC enable mode */
    req.segmentLen = 1024U; /* 1KB */

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> G3-Dump requesting... ");
#endif
    }

    status = R_G3_DumpReq(0, &req);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf(" waiting dump trigger ");
#endif

        /* Wait for completion of R_G3_ClearInfoReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[0].statusFlags, R_FLAG_DUMP);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        if (R_G3_STATUS_SUCCESS == dumpCfm->status)
        {
            if (R_TRUE == g_demo_config.verboseEnabled)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("segmentNum:%d\n", dumpCfm->segmentNum);
#endif
            }

            /* Initialize dump variables */
            dumpInd->endFlg = 0U;
            dumpInd->seqNum = 0xFFFFU;
            last_num = 0xFFFFU;
#if R_DEFINE_OPEN_RX
            R_OS_SystemTimeGet((st_os_systemtime_t *)&now_tick_count);
            expire_tick_count = now_tick_count + req.timeOut + ((req.segmentInterval + 20) * (req.maxDmpLen / req.segmentLen));

            while (now_tick_count < expire_tick_count)

#else
            R_TIMER_GetExpireTickCount(&expire_tick_count, req.timeOut + ((req.segmentInterval + 20) * (req.maxDmpLen / req.segmentLen)));
            while (R_TIMER_TickCountExpired(expire_tick_count) == R_FALSE)
#endif
            {
                if (last_num != dumpInd->seqNum)
                {
                    seg_cnt++;
                    if (R_TRUE == g_demo_config.verboseEnabled)
                    {
                        for (i = 0; i < dumpInd->segmentLen; i++)
                        {
#ifdef __PLC__DEBUG__
                            R_STDIO_Printf("%02X ", dumpInd->pDmpData[i]);
#endif
                            if ((i % 16) == 15)
                            {
#ifdef __PLC__DEBUG__
                                R_STDIO_Printf("\n");
#endif
                            }
                        }
                    }
                    else
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf(".");
#endif
                    }
                    last_num = dumpInd->seqNum;
                    dump_size += dumpInd->segmentLen;
                    if (R_TRUE == dumpInd->endFlg)
                    {
                        if (dumpCfm->segmentNum != seg_cnt)
                        {
#ifdef __PLC__DEBUG__
                            R_STDIO_Printf("Segment numbers do not match (%d) (%d)\n", dumpCfm->segmentNum, seg_cnt);
#endif
                        }
                        break;
                    }
                }
#if defined(R_DEFINE_OPEN_RX)
                R_OS_SystemTimeGet((st_os_systemtime_t *)&now_tick_count);
#endif
            }
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("complete!!(%ldbyte)\n", dump_size);
#endif
        }
        else
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(0, dumpCfm->status), dumpCfm->status);
#endif
        }
    }

    return status;
} /* R_DEMO_G3Dump */
/******************************************************************************
   End of function  R_DEMO_G3Dump
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_DeInit
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_DeInit(uint8_t chId)
{
    r_result_t status;

#ifdef __PLC__DEBUG__
    R_STDIO_Printf("\n -> CPX3 DeInit (ch%d)...", chId);
#endif

    status = R_G3_Deinit(chId, 2000);
    if (R_RESULT_SUCCESS != status)
    {
        return R_RESULT_FAILED;
    }
    else
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("done.");
#endif
        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_DeInit */
/******************************************************************************
   End of function  R_DEMO_DeInit
******************************************************************************/

/*===========================================================================*/
/*    MAC APIs                                                               */
/*===========================================================================*/
#ifndef R_DEFINE_DISABLE_UMAC
/******************************************************************************
 * Function Name: R_DEMO_MacInit
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_MacInit(uint8_t chId)
{
    r_g3_init_req_t req = {0};
    r_g3_callback_t callBack;
    r_result_t status;

#ifdef __PLC__DEBUG__
    R_STDIO_Printf("\n -> CPX3 Init as MAC mode(ch%d)...", chId);
#endif

    req.g3mode = R_G3_MODE_MAC;
    req.init.mac.neighbourTableSize = R_DEMO_G3MAC_NEIGBOUR_TABLE_SIZE;
    req.init.mac.deviceTableSize = R_DEMO_G3MAC_DEVICE_TABLE_SIZE;

#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    req.g3mode |= (g_demo_config.mediaType << 4);
    if (R_DEMO_InitMacCallBack(chId, &callBack, g_demo_config.mediaType) != R_RESULT_SUCCESS)
#else
    if (R_DEMO_InitMacCallBack(chId, &callBack) != R_RESULT_SUCCESS)
#endif
    {
        return R_RESULT_FAILED;
    }

    status = R_G3_Init(chId, &callBack, &req, 2000);
    if (R_RESULT_SUCCESS != status)
    {
        return R_RESULT_FAILED;
    }
    else
    {
        R_DEMO_LED(chId, R_DEMO_LED_BOOT);
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("done.");
#endif
        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_MacInit */
/******************************************************************************
   End of function  R_DEMO_MacInit
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_McpsData
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_McpsData(uint8_t chId, const r_g3mac_mcps_data_req_t *mcpsDataReq,
                           r_g3mac_mcps_data_cnf_t **cnf)
{
    r_result_t status;
    R_VOLATILE r_g3mac_mcps_data_cnf_t *mcpsDataCfm = (R_VOLATILE r_g3mac_mcps_data_cnf_t *)&g_g3cb[chId].mcpsDataCnf;
    *cnf = (r_g3mac_mcps_data_cnf_t *)mcpsDataCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_MCPS_DATA);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Sending MAC Data frame...");
#endif
    }

    status = R_G3MAC_McpsDataReq(chId, (r_g3mac_mcps_data_req_t *)mcpsDataReq);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed!\n");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_G3MAC_McpsDataReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_MCPS_DATA);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_G3MAC_STATUS_SUCCESS == mcpsDataCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("success.\n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(R_G3_MODE_MAC, mcpsDataCfm->status), mcpsDataCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_McpsData */
/******************************************************************************
   End of function  R_DEMO_McpsData
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_MlmeReset
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_MlmeReset(uint8_t chId, const r_g3mac_mlme_reset_req_t *mlmeResetReq,
                            r_g3mac_mlme_reset_cnf_t **cnf)
{
    r_result_t status;
    R_VOLATILE r_g3mac_mlme_reset_cnf_t *mlmeResetCfm = (R_VOLATILE r_g3mac_mlme_reset_cnf_t *)&g_g3cb[chId].mlmeResetCnf;
    *cnf = (r_g3mac_mlme_reset_cnf_t *)mlmeResetCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_MLME_RESET);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Resetting MAC device...");
#endif
    }

    status = R_G3MAC_MlmeResetReq(chId, (r_g3mac_mlme_reset_req_t *)mlmeResetReq);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed!\n");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_G3MAC_MlmeResetReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_MLME_RESET);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_G3MAC_STATUS_SUCCESS == mlmeResetCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("success.\n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(R_G3_MODE_MAC, mlmeResetCfm->status), mlmeResetCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_MlmeReset */
/******************************************************************************
   End of function  R_DEMO_MlmeReset
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_MlmeStart
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_MlmeStart(uint8_t chId, const r_g3mac_mlme_start_req_t *mlmeStartReq,
                            r_g3mac_mlme_start_cnf_t **cnf)
{
    r_result_t status;
    R_VOLATILE r_g3mac_mlme_start_cnf_t *mlmeStartCfm = (R_VOLATILE r_g3mac_mlme_start_cnf_t *)&g_g3cb[chId].mlmeStartCnf;
    *cnf = (r_g3mac_mlme_start_cnf_t *)mlmeStartCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_MLME_START);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Startting MAC device...");
#endif
    }

    status = R_G3MAC_MlmeStartReq(chId, (r_g3mac_mlme_start_req_t *)mlmeStartReq);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed!\n");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_G3MAC_MlmeStartReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_MLME_START);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_G3MAC_STATUS_SUCCESS == mlmeStartCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("success.\n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(R_G3_MODE_MAC, mlmeStartCfm->status), mlmeStartCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_MlmeStart */
/******************************************************************************
   End of function  R_DEMO_MlmeStart
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_MlmeScan
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_MlmeScan(uint8_t chId, const r_g3mac_mlme_scan_req_t *mlmeScanReq,
                           r_g3mac_mlme_scan_cnf_t **cnf)
{
    r_result_t status;
    R_VOLATILE r_g3mac_mlme_scan_cnf_t *mlmeScanCfm = (R_VOLATILE r_g3mac_mlme_scan_cnf_t *)&g_g3cb[chId].mlmeScanCnf;
    *cnf = (r_g3mac_mlme_scan_cnf_t *)mlmeScanCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_MLME_SCAN);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Scanting MAC device...");
#endif
    }

    status = R_G3MAC_MlmeScanReq(chId, (r_g3mac_mlme_scan_req_t *)mlmeScanReq);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed!\n");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_G3MAC_MlmeScanReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_MLME_SCAN);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_MlmeScan */
/******************************************************************************
   End of function  R_DEMO_MlmeScan
******************************************************************************/
#endif /* #ifndef R_DEFINE_DISABLE_UMAC */

/******************************************************************************
 * Function Name: R_DEMO_MlmeSet
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_MlmeSet(uint8_t chId, const r_g3mac_mlme_set_req_t *mlmeSetReq,
                          r_g3mac_mlme_set_cnf_t **cnf)
{
    uint8_t len;
    r_result_t status;
    R_VOLATILE r_g3mac_mlme_set_cnf_t *mlmeSetCfm = (R_VOLATILE r_g3mac_mlme_set_cnf_t *)&g_g3cb[chId].mlmeSetCnf;
    *cnf = (r_g3mac_mlme_set_cnf_t *)mlmeSetCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_MLME_SET);

    status = R_G3MAC_MlmeSetReq(chId, (r_g3mac_mlme_set_req_t *)mlmeSetReq);
    if (R_RESULT_SUCCESS != status)
    {
        return R_RESULT_FAILED;
    }
    else
    {
        current_config_flag = &g_g3cb[chId].statusFlags;
        current_config_id = R_FLAG_MLME_SET;
        config_monitor = 1;
    }
    return R_RESULT_SUCCESS;

} /* R_DEMO_MlmeSet */
/******************************************************************************
   End of function  R_DEMO_MlmeSet
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_MlmeGet
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_MlmeGet(uint8_t chId, const r_g3mac_mlme_get_req_t *mlmeGetReq,
                          r_g3mac_mlme_get_cnf_t **cnf)
{
    uint8_t len;
    r_result_t status;
    R_VOLATILE r_g3mac_mlme_get_cnf_t *mlmeGetCfm = (R_VOLATILE r_g3mac_mlme_get_cnf_t *)&g_g3cb[chId].mlmeGetCnf;
    *cnf = (r_g3mac_mlme_get_cnf_t *)mlmeGetCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_MLME_GET);

    // here
    status = R_G3MAC_MlmeGetReq(chId, (r_g3mac_mlme_get_req_t *)mlmeGetReq);
    if (R_RESULT_SUCCESS != status)
    {
        return R_RESULT_FAILED;
    }
    else
    {

        current_config_flag = &g_g3cb[chId].statusFlags;
        current_config_id = R_FLAG_MLME_GET;
        config_monitor = 1;
    }
    return R_RESULT_SUCCESS;

} /* R_DEMO_MlmeGet */
/******************************************************************************
   End of function  R_DEMO_MlmeGet
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_MlmeGetWrap
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_g3mac_status_t R_DEMO_MlmeGetWrap(uint8_t chId, uint16_t id, uint16_t index, uint8_t *val)
{
    r_g3mac_mlme_get_req_t req;
    r_g3mac_mlme_get_cnf_t *mlmeGetCfm;

    req.pibAttributeId = id;
    req.pibAttributeIndex = index;

    // Here

    if (R_DEMO_MlmeGet(chId, &req, &mlmeGetCfm) == R_RESULT_SUCCESS)
    {
        // gat data from mlmeGetCfm
        R_memcpy(val, mlmeGetCfm->pibAttributeValue, sizeof(mlmeGetCfm->pibAttributeValue));
        return (r_g3mac_status_t)mlmeGetCfm->status;
    }
    else
    {
        return (r_g3mac_status_t)R_DEMO_G3_STATUS_FAILED;
    }
} /* R_DEMO_MlmeGetWrap */
/******************************************************************************
   End of function  R_DEMO_MlmeGetWrap
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_MlmeSetWrap
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_g3mac_status_t R_DEMO_MlmeSetWrap(uint8_t chId, uint16_t id, uint16_t index, uint8_t *val)
{
    r_g3mac_mlme_set_req_t req;
    r_g3mac_mlme_set_cnf_t *pCnf;
    r_g3mac_status_t state;
    req.pibAttributeId = id;
    req.pibAttributeIndex = index;
    req.pibAttributeValue = val;
    if (R_DEMO_MlmeSet(chId, &req, &pCnf) == R_RESULT_SUCCESS)
    {
        state = (r_g3mac_status_t)pCnf->status;
    }
    else
    {
        state = (r_g3mac_status_t)R_DEMO_G3_STATUS_FAILED;
    }
    return state;
}
/******************************************************************************
   End of function  R_DEMO_MlmeSetWrap
******************************************************************************/

#ifndef R_DEFINE_DISABLE_UMAC
/******************************************************************************
 * Function Name: R_DEMO_MacSetConfig
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_g3mac_status_t R_DEMO_MacSetConfig(uint8_t chId)
{
    r_g3_set_config_req_t req = {0};
    r_g3_set_config_cnf_t *setcfgCfm;
    r_g3_config_extid_t tmpExtId = {0};

    R_BYTE_UInt64ToArr(g_demo_config.deviceEUI64, (uint8_t *)&tmpExtId);

    req.g3mode = R_G3_MODE_MAC;
    req.config.mac.bandPlan = g_demo_config.bandPlan;
    R_memcpy(req.config.mac.extendedAddress, (uint8_t *)&tmpExtId, 8);

    if (R_DEMO_G3SetConfig(chId, &req, &setcfgCfm) == R_RESULT_SUCCESS)
    {
        return (r_g3mac_status_t)setcfgCfm->status;
    }
    else
    {
        return (r_g3mac_status_t)R_DEMO_G3_STATUS_FAILED;
    }
} /* R_DEMO_MacSetConfig */
/******************************************************************************
   End of function  R_DEMO_MacSetConfig
******************************************************************************/
#endif
/*===========================================================================*/
/*    ADP APIs                                                               */
/*===========================================================================*/

/******************************************************************************
 * Function Name: R_DEMO_AdpInit
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_AdpInit(uint8_t chId)
{
    r_g3_init_req_t req = {0};
    r_g3_callback_t callBack;
    r_result_t status;
    // back
    req.g3mode = R_G3_MODE_ADP;
    req.init.adp.adpdBuffNum = R_DEMO_ADP_ADPD_DATA_QUEUE_SIZE;
    req.init.adp.routeTableSize = R_DEMO_ADP_ROUTING_TABLE_SIZE;
    req.init.adp.neighbourTableSize = R_DEMO_G3MAC_NEIGBOUR_TABLE_SIZE;
    req.init.adp.deviceTableSize = R_DEMO_G3MAC_DEVICE_TABLE_SIZE;
    req.init.adp.panDescriptorNum = R_DEMO_ADP_MAX_PAN_DESCRIPTORS;
    req.init.adp.routeType = 0;

#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    req.init.hyadp.posTableSize_RF = 100;
    req.g3mode |= (g_demo_config.mediaType << 4);
    if (R_DEMO_InitAdpCallBack(chId, &callBack, g_demo_config.mediaType) != R_RESULT_SUCCESS)
#else
    if (R_DEMO_InitAdpCallBack(chId, &callBack) != R_RESULT_SUCCESS)
#endif
    {
        return R_RESULT_FAILED;
    }

    status = R_G3_Init(chId, &callBack, &req, 2000);

    return status;

} /* R_DEMO_AdpInit */
/******************************************************************************
   End of function  R_DEMO_AdpInit
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AdpmLbp
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_AdpmLbp(uint8_t chId, const r_adp_adpm_lbp_req_t *lbpReq,
                          r_adp_adpm_lbp_cnf_t **cnf)
{
    r_result_t status;
    R_VOLATILE r_adp_adpm_lbp_cnf_t *lbpCfm = (R_VOLATILE r_adp_adpm_lbp_cnf_t *)&g_g3cb[chId].adpmLbpCnf;
    *cnf = (r_adp_adpm_lbp_cnf_t *)lbpCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_LBP);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Sending LBP frame...");
#endif
    }

    /* Invoke ADPM-LBP.request */
    status = R_ADP_AdpmLbpReq(chId, (r_adp_adpm_lbp_req_t *)lbpReq);
    if (R_RESULT_SUCCESS != status)
    {
        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_ADP_AdpmLbpReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_LBP);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_ADP_STATUS_SUCCESS == lbpCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("success. NSDU handle: 0x%.2X \n", lbpCfm->nsduHandle);
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(R_G3_MODE_ADP, lbpCfm->status), lbpCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_AdpmLbp */
/******************************************************************************
   End of function  R_DEMO_AdpmLbp
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AdpmNetworkLeave
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_AdpmNetworkLeave(uint8_t chId, r_adp_adpm_network_leave_cnf_t **cnf)
{
    r_result_t status;
    R_VOLATILE r_adp_adpm_network_leave_cnf_t *nwlCfm = (R_VOLATILE r_adp_adpm_network_leave_cnf_t *)&g_g3cb[chId].adpmNetworkLeaveCnf;
    *cnf = (r_adp_adpm_network_leave_cnf_t *)nwlCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_LEAVE);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Leaving network...");
#endif
    }

    status = R_ADP_AdpmNetworkLeaveReq(chId);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_ADP_AdpmNetworkLeaveReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_LEAVE);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_ADP_STATUS_SUCCESS == nwlCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("success.\n");
#endif
                R_DEMO_LED(chId, R_DEMO_LED_BOOT);
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(R_G3_MODE_ADP, nwlCfm->status), nwlCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_AdpmNetworkLeave */
/******************************************************************************
   End of function  R_DEMO_AdpmNetworkLeave
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AdpmNetworkJoin
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_AdpmNetworkJoin(uint8_t chId, const r_adp_adpm_network_join_req_t *nwjReq,
                                  r_adp_adpm_network_join_cnf_t **cnf)
{
    r_result_t status;
    R_VOLATILE r_adp_adpm_network_join_cnf_t *nwjCfm = (R_VOLATILE r_adp_adpm_network_join_cnf_t *)&g_g3cb[chId].adpmNetworkJoinCnf;
    *cnf = (r_adp_adpm_network_join_cnf_t *)nwjCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_JOIN);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Joining PanID:0x%.4X LBA:0x%02X%02X mediaType:%d...", nwjReq->panId, nwjReq->lbaAddress[0], nwjReq->lbaAddress[1], nwjReq->mediaType);
#endif
#else
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Joining PAN with PanID:0x%.4X LBA:0x%02X%02X...", nwjReq->panId, nwjReq->lbaAddress[0], nwjReq->lbaAddress[1]);
#endif
#endif
    }

    status = R_ADP_AdpmNetworkJoinReq(chId, (r_adp_adpm_network_join_req_t *)nwjReq);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_ADP_AdpmNetworkJoinReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_JOIN);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_ADP_STATUS_SUCCESS == nwjCfm->status)
        {
            if (R_TRUE == g_demo_config.verboseEnabled)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("success.");
#endif
#ifdef __PLC__DEBUG__
                R_STDIO_Printf(" Nwk Address: 0x%.4X\n", R_BYTE_ArrToUInt16((const uint8_t *)nwjCfm->networkAddress));
#endif
            }
            R_DEMO_LED(chId, R_DEMO_LED_IDLE);
        }
        else
        {
            if (R_TRUE == g_demo_config.verboseEnabled)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(R_G3_MODE_ADP, nwjCfm->status), nwjCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_AdpmNetworkJoin */
/******************************************************************************
   End of function  R_DEMO_AdpmNetworkJoin
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AdpmDiscovery
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_AdpmDiscovery(uint8_t chId, const r_adp_adpm_discovery_req_t *disReq,
                                r_adp_adpm_discovery_cnf_t **cnf)
{
    uint16_t i;
    r_result_t status;
    R_VOLATILE r_adp_adpm_discovery_cnf_t *disCfm = (R_VOLATILE r_adp_adpm_discovery_cnf_t *)&g_g3cb[chId].adpmDiscoveryCnf;
    *cnf = (r_adp_adpm_discovery_cnf_t *)disCfm;
    // if (send_discovery_req_flag)
    // {
    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_DISCOVERY);
    /* Start scanning */
    status = R_ADP_AdpmDiscoveryReq(chId, (r_adp_adpm_discovery_req_t *)disReq);
    //}

    if (R_RESULT_SUCCESS != status)
    {
        return R_RESULT_FAILED;
    }
    else
    {
        send_discovery_req_flag = 0 ;
        current_config_flag = &g_g3cb[chId].statusFlags;
        current_config_id = R_FLAG_ADPM_DISCOVERY;
        config_monitor = 1;
        return R_RESULT_SUCCESS;
    }
    // /* Wait for completion of R_ADP_AdpmDiscoveryReq */
    // status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_DISCOVERY);
    // if (R_RESULT_SUCCESS != status)
    // {
    //     return status;
    // }

    //     /* Check if verbose is enabled */
    //     if (R_TRUE == g_demo_config.verboseEnabled)
    //     {

    //         if (R_ADP_STATUS_SUCCESS == disCfm->status)
    //         {
    //             /* Show scan results */
    //             if (0 != disCfm->PANCount)
    //             {
    //                 R_STDIO_Printf("\n\n---------------Active network(s) found---------------------");

    //                 for (i = 0; i < disCfm->PANCount; i++)
    //                 {
    // #ifdef __PLC__DEBUG__
    // #if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    //                     R_STDIO_Printf("\n %d - PAN ID: 0x%.4X address: 0x%.4X MediaType:%d RC Coord: 0x%.4X LQI: 0x%.2X", i, disCfm->PANDescriptor[i].panId,
    // #else
    //                     R_STDIO_Printf("\n %d - PAN ID: 0x%.4X Short address: 0x%.4X RC Coordinator: 0x%.4X LQI: 0x%.2X", i, disCfm->PANDescriptor[i].panId,
    // #endif
    //                                    R_BYTE_ArrToUInt16(disCfm->PANDescriptor[i].address),
    // #if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    //                                    disCfm->PANDescriptor[i].mediaType,
    // #endif
    //                                    disCfm->PANDescriptor[i].rcCoord,
    //                                    disCfm->PANDescriptor[i].linkQuality);
    // #endif
    //                 }
    // #ifdef __PLC__DEBUG__
    //                 R_STDIO_Printf("\n");
    // #endif
    //             }
    //             else
    //             {
    // #ifdef __PLC__DEBUG__
    //                 R_STDIO_Printf("\n -> No active network(s) found.\n");
    // #endif
    //             }
    //         }
    //         else
    //         {
    // #ifdef __PLC__DEBUG__
    //             R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(R_G3_MODE_ADP, disCfm->status), disCfm->status);
    // #endif
    //         }
    //     }

    
}
/* R_DEMO_AdpmDiscovery */
/******************************************************************************
   End of function  R_DEMO_AdpmDiscovery
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AdpdData
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_AdpdData(uint8_t chId, const r_adp_adpd_data_req_t *dataReq,
                           r_adp_adpd_data_cnf_t **cnf)
{
    r_result_t status;
    R_VOLATILE r_adp_adpd_data_cnf_t *dataCfm = (R_VOLATILE r_adp_adpd_data_cnf_t *)&g_g3cb[chId].adpdDataCnf;
    *cnf = (r_adp_adpd_data_cnf_t *)dataCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPD_DATA);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Sending data frame...");
#endif
    }

#if !defined(R_DEFINE_OPEN_RX)
    R_DEMO_WaitcnfTimerOn(R_APP_WAIT_CNF_TIMEOUT * 1000);
#endif

    /* Invoke ADPD-DATA.request */
    status = R_ADP_AdpdDataReq(chId, (r_adp_adpd_data_req_t *)dataReq);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed!");
#endif
        }
#if !defined(R_DEFINE_OPEN_RX)
        R_DEMO_WaitcnfTimerOff();
#endif
        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_ADP_AdpdDataReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPD_DATA);
        if (R_RESULT_SUCCESS != status)
        {
            if (R_RESULT_TIMEOUT == status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("failed. by timeout NSDU handle: 0x%.2X \n", dataReq->nsduHandle);
#endif
            }
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_ADP_STATUS_SUCCESS == dataCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("success. NSDU handle: 0x%.2X \n", dataCfm->nsduHandle);
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(R_G3_MODE_ADP, dataCfm->status), dataCfm->status);
#endif
            }
        }
#if !defined(R_DEFINE_OPEN_RX)
        R_DEMO_WaitcnfTimerOff();
#endif
        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_AdpdData */
/******************************************************************************
   End of function  R_DEMO_AdpdData
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AdpmNetworkStart
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_AdpmNetworkStart(uint8_t chId, const r_adp_adpm_network_start_req_t *nwsReq,
                                   r_adp_adpm_network_start_cnf_t **cnf)
{
    r_result_t status;
    R_VOLATILE r_adp_adpm_network_start_cnf_t *nwsCfm = (R_VOLATILE r_adp_adpm_network_start_cnf_t *)&g_g3cb[chId].adpmNetworkStartCnf;
    *cnf = (r_adp_adpm_network_start_cnf_t *)nwsCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_START);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Starting PAN with ID: 0x%.4X...", nwsReq->panId & 0xFCFF);
#endif
    }

    status = R_ADP_AdpmNetworkStartReq(chId, (r_adp_adpm_network_start_req_t *)nwsReq);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_ADP_AdpmNetworkStartReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_START);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_ADP_STATUS_SUCCESS == nwsCfm->status)
        {
            if (R_TRUE == g_demo_config.verboseEnabled)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("success.\n");
#endif
            }
            R_DEMO_LED(chId, R_DEMO_LED_IDLE);
        }
        else
        {
            if (R_TRUE == g_demo_config.verboseEnabled)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(R_G3_MODE_ADP, nwsCfm->status), nwsCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_AdpmNetworkStart */
/******************************************************************************
   End of function  R_DEMO_AdpmNetworkStart
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AdpmSet
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_AdpmSet(uint8_t chId, const r_adp_adpm_set_req_t *setReq,
                          r_adp_adpm_set_cnf_t **cnf)
{
    r_result_t status;
    uint8_t len;
    R_VOLATILE r_adp_adpm_set_cnf_t *setCfm = (R_VOLATILE r_adp_adpm_set_cnf_t *)&g_g3cb[chId].adpmSetCnf;
    *cnf = (r_adp_adpm_set_cnf_t *)setCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_SET);

    status = R_ADP_AdpmSetReq(chId, (r_adp_adpm_set_req_t *)setReq);
    if (R_RESULT_SUCCESS != status)
    {

        return R_RESULT_FAILED;
    }
    else
    {
        current_config_flag = &g_g3cb[chId].statusFlags;
        current_config_id = R_FLAG_ADPM_SET;
        config_monitor = 1;
    }
    return R_RESULT_SUCCESS;
} /* R_DEMO_AdpmSet */
/******************************************************************************
   End of function  R_DEMO_AdpmSet
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AdpmGet
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_AdpmGet(uint8_t chId, const r_adp_adpm_get_req_t *getReq,
                          r_adp_adpm_get_cnf_t **cnf)
{
    uint8_t len;
    r_result_t status;
    R_VOLATILE r_adp_adpm_get_cnf_t *getCfm = (R_VOLATILE r_adp_adpm_get_cnf_t *)&g_g3cb[chId].adpmGetCnf;
    *cnf = (r_adp_adpm_get_cnf_t *)getCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_GET);

    status = R_ADP_AdpmGetReq(chId, (r_adp_adpm_get_req_t *)getReq);
    if (R_RESULT_SUCCESS != status)
    {
        return R_RESULT_FAILED;
    }
    else
    {

        current_config_flag = &g_g3cb[chId].statusFlags;
        current_config_id = R_FLAG_ADPM_GET;
        current_AppResetTask = 12;
        config_monitor = 1;
    }

    return R_RESULT_SUCCESS;

} /* R_DEMO_AdpmGet */
/******************************************************************************
   End of function  R_DEMO_AdpmGet
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AdpmRouteDiscovery
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_AdpmRouteDiscovery(uint8_t chId, const r_adp_adpm_route_disc_req_t *rdisReq,
                                     r_adp_adpm_route_disc_cnf_t **cnf)
{
    r_result_t status;
    R_VOLATILE r_adp_adpm_route_disc_cnf_t *rdisCfm = (R_VOLATILE r_adp_adpm_route_disc_cnf_t *)&g_g3cb[chId].adpmRouteDiscoveryCnf;
    *cnf = (r_adp_adpm_route_disc_cnf_t *)rdisCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_ROUTE);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Starting route discovery...");
#endif
    }

    status = R_ADP_AdpmRouteDiscoveryReq(chId, (r_adp_adpm_route_disc_req_t *)rdisReq);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_ADP_AdpmRouteDiscoveryReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_ROUTE);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_ADP_STATUS_SUCCESS == rdisCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("success.\n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(R_G3_MODE_ADP, rdisCfm->status), rdisCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_AdpmRouteDiscovery */
/******************************************************************************
   End of function  R_DEMO_AdpmRouteDiscovery
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AdpmPathDiscovery
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_AdpmPathDiscovery(uint8_t chId, const r_adp_adpm_path_discovery_req_t *pdisReq,
                                    r_adp_adpm_path_discovery_cnf_t **cnf)
{
    uint8_t i;
    r_result_t status;
    R_VOLATILE r_adp_adpm_path_discovery_cnf_t *pdisCfm = (R_VOLATILE r_adp_adpm_path_discovery_cnf_t *)&g_g3cb[chId].adpmPathDiscoveryCnf;
    *cnf = (r_adp_adpm_path_discovery_cnf_t *)pdisCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_PATH);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Starting path discovery...");
#endif
    }

    status = R_ADP_AdpmPathDiscoveryReq(chId, (r_adp_adpm_path_discovery_req_t *)pdisReq);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_ADP_AdpmPathDiscoveryReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_PATH);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        if (R_ADP_STATUS_SUCCESS == pdisCfm->status)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("success.");
#endif
        }
        else
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed.");
#endif
        }

#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n");
#endif

#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\nDestination address: 0x%.4X", R_BYTE_ArrToUInt16((const uint8_t *)pdisCfm->dstAddr));
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\nMetric Type: 0x%.2X", pdisCfm->pathMetricType);
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\nPath Table Entries: 0x%.2X", pdisCfm->pathTableEntries);
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\nStatus: %s(0x%.2X)", R_G3_Status2Text(R_G3_MODE_ADP, pdisCfm->status), pdisCfm->status);
#endif

#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n\n---------------Route hops---------------------");
#endif

        for (i = 0; i < pdisCfm->pathTableEntries; i++)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n %d - Path address: 0x%.4X", i, R_BYTE_ArrToUInt16(pdisCfm->pathTable[i].pathAddress));
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n %d - Link cost: 0x%.2X", i, pdisCfm->pathTable[i].linkCost);
#endif
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n %d - MNS: %d PhaseDiff: %d MRx: %d MTx: %d", i, pdisCfm->pathTable[i].mns, pdisCfm->pathTable[i].phaseDiff, pdisCfm->pathTable[i].MRx, pdisCfm->pathTable[i].MTx);
#endif
#else
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n %d - Metric not supported: 0x%.2X", i, pdisCfm->pathTable[i].mns);
#endif
#endif
        }

#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n");
#endif

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_AdpmPathDiscovery */
/******************************************************************************
   End of function  R_DEMO_AdpmPathDiscovery
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AdpmReset
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_AdpmReset(uint8_t chId, r_adp_adpm_reset_cnf_t **cnf)
{
    r_result_t status;
    R_VOLATILE r_adp_adpm_reset_cnf_t *rstCfm = (R_VOLATILE r_adp_adpm_reset_cnf_t *)&g_g3cb[chId].adpmResetCnf;
    *cnf = (r_adp_adpm_reset_cnf_t *)rstCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_RESET);

    status = R_ADP_AdpmResetReq(chId);
    if (R_RESULT_SUCCESS != status)
    {
        return R_RESULT_FAILED;
    }
    else
    {
        //         /* Wait for completion of R_ADP_AdpmResetReq */
        //         status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_ADPM_RESET);
        //         if (R_RESULT_SUCCESS != status)
        //         {
        //             return status;
        //         }

        //         /* Check if verbose is enabled */
        //         if (R_TRUE == g_demo_config.verboseEnabled)
        //         {
        //             if (R_ADP_STATUS_SUCCESS == rstCfm->status)
        //             {
        // #ifdef __PLC__DEBUG__
        //                 R_STDIO_Printf("success.\n");
        // #endif
        //             }
        //             else
        //             {
        // #ifdef __PLC__DEBUG__
        //                 R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(R_G3_MODE_ADP, rstCfm->status), rstCfm->status);
        // #endif
        //             }
        //         }

        current_config_flag = &g_g3cb[chId].statusFlags;
        current_config_id = R_FLAG_ADPM_RESET;
        current_AppResetTask = CPX3_ADPM_GET_WRAP;
        config_monitor = 1;
    }
    return R_RESULT_SUCCESS;

} /* R_DEMO_AdpmReset */
/******************************************************************************
   End of function  R_DEMO_AdpmReset
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AdpmGetWrap
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_adp_status_t R_DEMO_AdpmGetWrap(uint8_t chId, r_adp_ib_id_t id, uint16_t index, uint8_t *val)
{
    r_adp_adpm_get_req_t req;
    r_adp_adpm_get_cnf_t *getCfm;

    req.aibAttributeId = id;
    req.aibAttributeIndex = index;

    if (R_DEMO_AdpmGet(chId, &req, &getCfm) == R_RESULT_SUCCESS)
    { // Enter Here
        R_memcpy(val, getCfm->aibAttributeValue, sizeof(getCfm->aibAttributeValue));
        return (r_adp_status_t)getCfm->status;
    }
    else
    {
        return (r_adp_status_t)R_DEMO_G3_STATUS_FAILED;
    }
} /* R_DEMO_AdpmGetWrap */
/******************************************************************************
   End of function  R_DEMO_AdpmGetWrap
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AdpmSetWrap
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_adp_status_t R_DEMO_AdpmSetWrap(uint8_t chId, r_adp_ib_id_t id, uint16_t index, uint8_t *val)
{
    r_adp_adpm_set_req_t req;
    r_adp_adpm_set_cnf_t *pCnf;

    req.aibAttributeId = id;
    req.aibAttributeIndex = index;
    req.aibAttributeValue = val;
    if (R_DEMO_AdpmSet(chId, &req, &pCnf) == R_RESULT_SUCCESS)
    {
        return (r_adp_status_t)pCnf->status;
    }
    else
    {
        return (r_adp_status_t)R_DEMO_G3_STATUS_FAILED;
    }
}
/******************************************************************************
   End of function  R_DEMO_AdpmSetWrap
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AdpSetConfig
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_adp_status_t R_DEMO_AdpSetConfig(uint8_t chId)
{
    r_g3_set_config_req_t req = {0};
    r_g3_set_config_cnf_t *setcfgCfm;
    r_g3_config_extid_t tmpExtId = {0};

    R_BYTE_UInt64ToArr(g_demo_config.deviceEUI64, (uint8_t *)&tmpExtId);

    req.g3mode = R_G3_MODE_ADP;
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    req.g3mode |= (g_demo_config.mediaType << 4);
#endif
    req.config.adp.bandPlan = g_demo_config.bandPlan;
    R_memcpy(req.config.adp.extendedAddress, (uint8_t *)&tmpExtId, 8);
    R_memcpy(req.config.adp.psk, g_demo_config.pskKey, 16);

    if (g_demo_config.extIdFlg)
    {
        req.config.adp.extIDFlg = R_TRUE;
        req.config.adp.pExtId = &tmpExtId;
        R_memcpy(req.config.adp.pExtId, &g_demo_config.extId, sizeof(r_g3_config_extid_t));
    }
    else
    {
        /**/
    }

    if (R_DEMO_G3SetConfig(chId, &req, &setcfgCfm) == R_RESULT_SUCCESS)
    {
        return (r_adp_status_t)setcfgCfm->status;
    }
    else
    {
        return (r_adp_status_t)R_DEMO_G3_STATUS_FAILED;
    }
} /* R_DEMO_AdpSetConfig */
/******************************************************************************
   End of function  R_DEMO_AdpSetConfig
******************************************************************************/

#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
/******************************************************************************
 * Function Name: R_DEMO_RfMlmeSet
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_RfMlmeSet(uint8_t chId, const r_g3rfmac_mlme_set_req_t *mlmeSetReq,
                            r_g3rfmac_mlme_set_cnf_t **cnf)
{
    uint8_t len;
    r_result_t status;
    R_VOLATILE r_g3rfmac_mlme_set_cnf_t *mlmeSetCfm = (R_VOLATILE r_g3rfmac_mlme_set_cnf_t *)&g_g3cb[chId].rf_mlme_set_cnf;
    *cnf = (r_g3rfmac_mlme_set_cnf_t *)mlmeSetCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_RF_MLME_SET);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Setting RF MAC PIB %s(0x%.4X) Index: %d...", R_G3_Ibid2Text(R_G3_INFO_LAYER_RF_MAC, mlmeSetReq->pibAttributeId, &len), mlmeSetReq->pibAttributeId, mlmeSetReq->pibAttributeIndex);
#endif
    }

    status = R_G3RFMAC_MlmeSetReq(chId, (r_g3rfmac_mlme_set_req_t *)mlmeSetReq);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed!\n");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_G3MAC_RfMlmeSetReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_RF_MLME_SET);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_G3MAC_STATUS_SUCCESS == mlmeSetCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("success.\n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(R_G3_MODE_MAC, mlmeSetCfm->status), mlmeSetCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_RfMlmeSet */
/******************************************************************************
   End of function  R_DEMO_RfMlmeSet
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_RfMlmeGet
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_RfMlmeGet(uint8_t chId, const r_g3rfmac_mlme_get_req_t *mlmeGetReq,
                            r_g3rfmac_mlme_get_cnf_t **cnf)
{
    uint8_t len;
    r_result_t status;
    R_VOLATILE r_g3rfmac_mlme_get_cnf_t *mlmeGetCfm = (R_VOLATILE r_g3rfmac_mlme_get_cnf_t *)&g_g3cb[chId].rf_mlme_get_cnf;
    *cnf = (r_g3rfmac_mlme_get_cnf_t *)mlmeGetCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_RF_MLME_GET);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Getting RF MAC PIB %s(0x%.4X) Index: %d...", R_G3_Ibid2Text(R_G3_INFO_LAYER_RF_MAC, mlmeGetReq->pibAttributeId, &len), mlmeGetReq->pibAttributeId, mlmeGetReq->pibAttributeIndex);
#endif
    }

    status = R_G3RFMAC_MlmeGetReq(chId, (r_g3rfmac_mlme_get_req_t *)mlmeGetReq);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed.\n");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_G3MAC_RfMlmeGetReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_RF_MLME_GET);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_ADP_STATUS_SUCCESS == mlmeGetCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("success.\n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(R_G3_MODE_MAC, mlmeGetCfm->status), mlmeGetCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_RfMlmeGet */
/******************************************************************************
   End of function  R_DEMO_RfMlmeGet
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_RfMlmeGetWrap
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_g3rfmac_status_t R_DEMO_RfMlmeGetWrap(uint8_t chId, uint16_t id, uint16_t index, uint8_t *val)
{
    r_g3rfmac_mlme_get_req_t req;
    r_g3rfmac_mlme_get_cnf_t *mlmeGetCfm;

    req.pibAttributeId = id;
    req.pibAttributeIndex = index;

    if (R_DEMO_RfMlmeGet(chId, &req, &mlmeGetCfm) == R_RESULT_SUCCESS)
    {
        R_memcpy(val, mlmeGetCfm->pibAttributeValue, sizeof(mlmeGetCfm->pibAttributeValue));
        return (r_g3rfmac_status_t)mlmeGetCfm->status;
    }
    else
    {
        return (r_g3rfmac_status_t)R_DEMO_G3_STATUS_FAILED;
    }
} /* R_DEMO_RfMlmeGetWrap */
/******************************************************************************
   End of function  R_DEMO_RfMlmeGetWrap
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_RfMlmeSetWrap
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_g3rfmac_status_t R_DEMO_RfMlmeSetWrap(uint8_t chId, uint16_t id, uint16_t index, uint8_t *val)
{
    r_g3rfmac_mlme_set_req_t req;
    r_g3rfmac_mlme_set_cnf_t *pCnf;

    req.pibAttributeId = id;
    req.pibAttributeIndex = index;
    req.pibAttributeValue = val;
    if (R_DEMO_RfMlmeSet(chId, &req, &pCnf) == R_RESULT_SUCCESS)
    {
        return (r_g3rfmac_status_t)pCnf->status;
    }
    else
    {
        return (r_g3rfmac_status_t)R_DEMO_G3_STATUS_FAILED;
    }
}
/******************************************************************************
   End of function  R_DEMO_RfMlmeSetWrap
******************************************************************************/

#ifndef R_DEFINE_DISABLE_UMAC
/******************************************************************************
 * Function Name: R_DEMO_RfMcpsData
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_RfMcpsData(uint8_t chId, const r_g3rfmac_mcps_data_req_t *mcpsDataReq,
                             r_g3rfmac_mcps_data_cnf_t **cnf)
{
    r_result_t status;
    R_VOLATILE r_g3rfmac_mcps_data_cnf_t *mcpsDataCfm = (R_VOLATILE r_g3rfmac_mcps_data_cnf_t *)&g_g3cb[chId].rf_mcps_data_cnf;
    *cnf = (r_g3rfmac_mcps_data_cnf_t *)mcpsDataCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_RF_MCPS_DATA);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Sending MAC Data frame...");
#endif
    }

    status = R_G3RFMAC_McpsDataReq(chId, (r_g3rfmac_mcps_data_req_t *)mcpsDataReq);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed!\n");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_G3MAC_RfMcpsDataReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_RF_MCPS_DATA);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_G3MAC_STATUS_SUCCESS == mcpsDataCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("success.\n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(R_G3_MODE_MAC, mcpsDataCfm->status), mcpsDataCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_RfMcpsData */
/******************************************************************************
   End of function  R_DEMO_RfMcpsData
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_RfMlmeReset
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_RfMlmeReset(uint8_t chId, const r_g3rfmac_mlme_reset_req_t *mlmeResetReq,
                              r_g3rfmac_mlme_reset_cnf_t **cnf)
{
    r_result_t status;
    R_VOLATILE r_g3rfmac_mlme_reset_cnf_t *mlmeResetCfm = (R_VOLATILE r_g3rfmac_mlme_reset_cnf_t *)&g_g3cb[chId].rf_mlme_reset_cnf;
    *cnf = (r_g3rfmac_mlme_reset_cnf_t *)mlmeResetCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_RF_MLME_RESET);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Resetting MAC device...");
#endif
    }

    status = R_G3RFMAC_MlmeResetReq(chId, (r_g3rfmac_mlme_reset_req_t *)mlmeResetReq);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("failed!\n");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_G3MAC_RfMlmeResetReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_RF_MLME_RESET);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_G3MAC_STATUS_SUCCESS == mlmeResetCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("success.\n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text(R_G3_MODE_MAC, mlmeResetCfm->status), mlmeResetCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_RfMlmeReset */
/******************************************************************************
   End of function  R_DEMO_RfMlmeReset
******************************************************************************/
#endif /* #ifndef R_DEFINE_DISABLE_UMAC */

#endif
