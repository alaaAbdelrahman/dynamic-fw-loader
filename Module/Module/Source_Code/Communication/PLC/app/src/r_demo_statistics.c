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
* File Name   : r_demo_statistics.c
*    @version
*        $Rev: 2394 $
*    @last editor
*        $Author: khues $
*    @date
*        $Date:: 2017-05-05 08:48:11 +0200#$
* Description :
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "r_typedefs.h"
#include "r_stdio_api.h"
#include "r_byte_swap.h"

/* g3 part */
#include "r_c3sap_api.h"
#include "r_g3_param2text.h"

#include "r_demo_app.h"
#include "r_demo_sys.h"
#include "r_demo_app_eap.h"
#include "r_demo_nvm_process.h"
#include "r_demo_print.h"

#include "r_ipv6_headers.h"
#include "r_udp_headers.h"
#include "r_demo_tools.h"
#include "r_demo_api.h"
#include "r_demo_common.h"
#include "r_demo_statistics.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/
#define R_APP_PRV_PRINT_CHAR_BUFF_NUM 10000
/******************************************************************************
Private global variables and functions
******************************************************************************/
#if !(defined __VANGO__)
static char gs_app_print_cnf_char[R_APP_PRV_PRINT_CHAR_BUFF_NUM];
#endif
/******************************************************************************
Exported global variables
******************************************************************************/
extern r_demo_buff_t               g_demo_buff;
extern r_demo_config_t             g_demo_config;

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Functions
******************************************************************************/
/******************************************************************************
* Function Name: R_DEMO_AppGetStatistics
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_AppGetStatistics (uint8_t chId, r_g3_info_layer_t layer)
{
    uint32_t              i;
    const char *          pTxt;

    r_g3_get_info_req_t   req;
    r_g3_get_info_cnf_t * pCnf;

    req.infoType = R_G3_INFO_TYPE_STATS;
    req.infoLayer = layer;

    if (
        (R_DEMO_G3GetInfo (chId, &req, &pCnf) != R_RESULT_SUCCESS) ||
        (R_G3_STATUS_SUCCESS != pCnf->status)
        )
    {
        return R_RESULT_FAILED;
    }

    switch (layer)
    {
#ifndef R_DEFINE_DISABLE_UMAC
        case R_G3_INFO_LAYER_UMAC:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n   MAC Statistics\n");
#endif
            break;
#endif
        case R_G3_INFO_LAYER_ADP:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n   ADP Statistics\n");
#endif
            break;

        case R_G3_INFO_LAYER_EAP:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n   EAP Statistics\n");
#endif
            break;

        default:
            layer = R_G3_INFO_LAYER_END;
            break;
    } /* switch */

    if (R_G3_INFO_LAYER_END != layer)
    {
        for (i = 0; i < (uint32_t)(pCnf->length >> 2); i++)
        {
            pTxt = R_G3_Stats2Text((uint8_t)layer, i);
            if (NULL == pTxt)
            {
                continue;
            }
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("%41s(%3ld):%10lu\n", pTxt, i, pCnf->pInfo[i]);
#endif
        }
    }

    return R_RESULT_SUCCESS;
} /* R_DEMO_AppGetStatistics */
/******************************************************************************
   End of function  R_DEMO_AppGetStatistics
******************************************************************************/



/******************************************************************************
* Function Name: R_DEMO_AppGetLog
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_AppGetLog (uint8_t chId, r_g3_info_layer_t layer)
{
    uint32_t              i;
    r_g3_get_info_req_t   req;
    r_g3_get_info_cnf_t * pCnf;
    uint32_t              length = 0;

    req.infoType  = R_G3_INFO_TYPE_LOG;
    req.infoLayer = layer;

    if (
        (uint32_t)(R_DEMO_G3GetInfo (chId, &req, &pCnf) != R_RESULT_SUCCESS) ||
        (uint32_t)(R_G3_STATUS_SUCCESS != pCnf->status)
        )
    {
        return R_RESULT_FAILED;
    }

    switch (layer)
    {
#ifndef R_DEFINE_DISABLE_UMAC
        case R_G3_INFO_LAYER_UMAC:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n   MAC Log\n");
#endif
            break;
#endif
        case R_G3_INFO_LAYER_ADP:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n   ADP Log\n");
#endif
            break;

        case R_G3_INFO_LAYER_EAP:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n   EAP Log\n");
#endif
            break;
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
        case R_G3_INFO_LAYER_RF_MAC:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n   RF MAC Log\n");
#endif
            break;
#endif
        default:
            break;
    } /* switch */

    if (R_G3_INFO_LAYER_LMAC_DSP == layer)
    {
        for (i = 0; i < (uint32_t)(pCnf->length >> 2); i += 2)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("      %4ld: 0x%08lX(%10lu[4MHz])    0x%08lX\n", i, pCnf->pInfo[i], pCnf->pInfo[i], pCnf->pInfo[i + 1]);
#endif
        }
    }
    else
    {
#if !(defined __VANGO__)
        for (i = 0; i < (uint32_t)(pCnf->length >> 2); i += 2)
        {
            length += sprintf(&gs_app_print_cnf_char[length], "    %4d: %10d[ms] ",i,pCnf->pInfo[i]);
            length +=  R_G3_LogInfo2Text( layer, (uint8_t *)&pCnf->pInfo[i+1], &gs_app_print_cnf_char[length] );
            length += sprintf(&gs_app_print_cnf_char[length], "\r\n");
        }

        for(i=0; (i<length)&&(i<R_APP_PRV_PRINT_CHAR_BUFF_NUM);i++)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("%c", gs_app_print_cnf_char[i]);
#endif
        }
#else
        for (i = 0; i < (pCnf->length >> 2); i += 2)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("      %4ld: 0x%08lX(%10lu[ms])    0x%08lX\n", i, pCnf->pInfo[i], pCnf->pInfo[i], pCnf->pInfo[i + 1]);
#endif
        }
#endif
    }

    return R_RESULT_SUCCESS;
} /* R_DEMO_AppGetLog */
/******************************************************************************
   End of function  R_DEMO_AppGetLog
******************************************************************************/



/******************************************************************************
* Function Name: R_DEMO_AppClearInfo
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_AppClearInfo (uint8_t chId, uint8_t type)
{
    r_g3_clear_info_req_t   req = {0}; // all zero(or all 1) mean all clear
    r_g3_clear_info_cnf_t * pCnf;

    req.infoTypeBit = type;

    if (
        (uint32_t)(R_DEMO_G3ClearInfo (chId, &req, &pCnf) != R_RESULT_SUCCESS) ||
        (uint32_t)(R_G3_STATUS_SUCCESS != pCnf->status)
        )
    {
        return R_RESULT_FAILED;
    }
    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_DEMO_AppClearInfo
******************************************************************************/
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
/******************************************************************************
* Function Name: get_rfmac_statistics
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static r_result_t get_rfmac_statistics(uint8_t chId)
{
    int i;
    r_g3rfmac_mlme_get_req_t       mlmeGetReq;
    r_g3rfmac_mlme_get_cnf_t *     mlmeGetCfm;

    /* Set request structure. */
    mlmeGetReq.pibAttributeIndex = 0;


    for (i = R_G3RFMAC_IB_COUNTEROCTETS_RF; i <= R_G3RFMAC_IB_RXSUCCESSCOUNT_RF; i++)
    {
        mlmeGetReq.pibAttributeId = i;
        R_DEMO_RfMlmeGet(chId, &mlmeGetReq, &mlmeGetCfm);
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("0x%.8lX", R_BYTE_ArrToUInt32((mlmeGetCfm->pibAttributeValue)));
#endif
    }

    return R_RESULT_SUCCESS;
} /* get_rfmac_statistics */
/******************************************************************************
   End of function  get_rfmac_statistics
******************************************************************************/
#endif
/******************************************************************************
* Function Name: R_DEMO_MenuStatistics
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_MenuStatistics (void)
{
    /* Configuration Menu */
    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\f-------------------Configuration Menu-----------------------");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 0 - Clear Statistics");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 1 - Clear Log");
#endif
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
        if ((R_MEDIA_TYPE0_PLC == g_demo_config.mediaType) || (R_MEDIA_TYPE2_BOTH == g_demo_config.mediaType))
#endif
        {
#ifndef R_DEFINE_DISABLE_UMAC
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n 3 - Get MAC Statistics");
#endif
#endif
        }
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
        if ((R_MEDIA_TYPE1_RF == g_demo_config.mediaType) || (R_MEDIA_TYPE2_BOTH == g_demo_config.mediaType))
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n a - Get RF MAC Statistics");
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n b - Get RF MAC Log");
#endif
        }
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 4 - Get ADP Statistics");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 5 - Get EAP Statistics");
#endif
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
        if ((R_MEDIA_TYPE0_PLC == g_demo_config.mediaType) || (R_MEDIA_TYPE2_BOTH == g_demo_config.mediaType))
#endif
        {
#ifndef R_DEFINE_DISABLE_UMAC
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n 7 - Get MAC Log");
#endif
#endif
        }
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 8 - Get ADP Log");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 9 - Get EAP Log");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n z - Return");
#endif

        R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

        if (strlen ((char *)g_demo_buff.getStringBuffer) == 1)
        {
            switch (g_demo_buff.getStringBuffer[0])
            {
                case '0':
                    R_DEMO_AppClearInfo (R_DEMO_G3_USE_PRIMARY_CH, R_G3_INFO_TYPE_BIT_STATS);
                    break;

                case '1':
                    R_DEMO_AppClearInfo (R_DEMO_G3_USE_PRIMARY_CH, R_G3_INFO_TYPE_BIT_LOG);
                    break;

#ifndef R_DEFINE_DISABLE_UMAC
                case '3':
                    R_DEMO_AppGetStatistics (R_DEMO_G3_USE_PRIMARY_CH, R_G3_INFO_LAYER_UMAC);
                    break;
#endif
                case '4':
                    R_DEMO_AppGetStatistics (R_DEMO_G3_USE_PRIMARY_CH, R_G3_INFO_LAYER_ADP);
                    break;

                case '5':
                    R_DEMO_AppGetStatistics (R_DEMO_G3_USE_PRIMARY_CH, R_G3_INFO_LAYER_EAP);
                    break;

#ifndef R_DEFINE_DISABLE_UMAC
                case '7':
                    R_DEMO_AppGetLog (R_DEMO_G3_USE_PRIMARY_CH, R_G3_INFO_LAYER_UMAC);
                    break;
#endif
                case '8':
                    R_DEMO_AppGetLog (R_DEMO_G3_USE_PRIMARY_CH, R_G3_INFO_LAYER_ADP);
                    break;

                case '9':
                    R_DEMO_AppGetLog (R_DEMO_G3_USE_PRIMARY_CH, R_G3_INFO_LAYER_EAP);
                    break;

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
                case 'a':
                    get_rfmac_statistics(R_DEMO_G3_USE_PRIMARY_CH);
                    break;
                case 'b':
                    R_DEMO_AppGetLog (R_DEMO_G3_USE_PRIMARY_CH, R_G3_INFO_LAYER_RF_MAC);
                    break;
#endif

                case 'z':
                    return R_RESULT_SUCCESS;

                default:
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\n\n Invalid option! \n");
#endif
                    break;
            } /* switch */

        }
    }
} /* R_DEMO_MenuStatistics */
/******************************************************************************
   End of function  R_DEMO_MenuStatistics
******************************************************************************/
