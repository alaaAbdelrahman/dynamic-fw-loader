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
* File Name   : r_demo_eap.c
*    @version
*        $Rev: 9602 $
*    @last editor
*        $Author: a0202438 $
*    @date
*        $Date:: 2022-04-08 13:53:43 +0900#$
* Description :
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include "r_typedefs.h"
#include "r_stdio_api.h"
#include "r_byte_swap.h"

/* g3 part */
#include "r_c3sap_api.h"
#include "r_g3_param2text.h"


/* app part */
#include "r_demo_api.h"
#include "r_demo_tools.h"
#include "r_demo_app.h"
#include "r_demo_app_eap.h"
#include "r_demo_parameters.h"
#include "r_demo_os_wrapper.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_DEMPO_APP_EAP_ENTRIES_PER_CYCLE  (16)

/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Exported global variables
******************************************************************************/
extern r_demo_config_t             g_demo_config;
extern r_demo_entity_t             g_demo_entity;
extern r_demo_buff_t               g_demo_buff;

extern volatile r_demo_g3_cb_str_t g_g3cb[R_G3_CH_MAX];

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

r_demo_client_info_t g_demo_client;

/******************************************************************************
Private global variables and functions
******************************************************************************/

/******************************************************************************
Functions
******************************************************************************/



/******************************************************************************
* Function Name: R_LBS_ShowDeviceList
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_LBS_ShowDeviceList (void)
{
    uint16_t i;

#ifdef __PLC__DEBUG__
    R_STDIO_Printf ("\n -> List of connected devices:");
#endif

    for (i = 0; i < R_DEMO_CLIENT_NUM_MAX; i++)
    {
        if (R_DEMO_CSTATE_JOINED == g_demo_client.tbl.cTable[i].valid_state)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> No 0x%04X :Short Address: 0x%.4X", i, R_BYTE_ArrToUInt16 (g_demo_client.tbl.cTable[i].network_address));
#endif
        }
    }
} /* R_LBS_ShowDeviceList */
/******************************************************************************
   End of function  R_LBS_ShowDeviceList
******************************************************************************/


/******************************************************************************
* Function Name: R_LBS_KickDeviceNo
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_LBS_KickDeviceNo (uint8_t no)
{
    r_eap_eapm_network_req_t   req;
    r_eap_eapm_network_cnf_t * pCnf;
    uint8_t *                  extAddr = NULL;
    r_result_t                 result;

    if (
        (no < R_DEMO_CLIENT_NUM_MAX) &&
        (R_DEMO_CSTATE_INVALID != g_demo_client.tbl.cTable[no].valid_state)
        )
    {
        extAddr = g_demo_client.tbl.cTable[no].extended_address;
    }

    if (NULL == extAddr)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> No is invalid:");
#endif
        return;
    }

#ifdef __PLC__DEBUG__
    R_STDIO_Printf ("\n -> Kicking device 0x%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X.",
                    extAddr[0], extAddr[1], extAddr[2], extAddr[3],
                    extAddr[4], extAddr[5], extAddr[6], extAddr[7]
                    );
#endif
    R_memcpy (req.extendedAddress, extAddr, 8);
    req.reqHandle = g_demo_entity.nsduHandle++;
    req.gmkId = 0;
    req.cmdId = R_EAP_NETWORK_KICK;

    result = R_DEMO_EapmNetwork (R_DEMO_G3_USE_PRIMARY_CH, &req, &pCnf);

    /* Check if KICK was successful. */
    if ((R_RESULT_SUCCESS == result) && (R_EAP_STATUS_SUCCESS == pCnf->status))
    {
        g_demo_client.tbl.cTable[no].valid_state = R_DEMO_CSTATE_LEAVED;
    }
} /* R_LBS_KickDeviceNo */
/******************************************************************************
   End of function  R_LBS_KickDeviceNo
******************************************************************************/


/******************************************************************************
* Function Name: R_LBS_RemoveKeyDev
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_LBS_RemoveKeyDev(uint8_t no, uint8_t index)
{
    r_eap_eapm_network_req_t   req;
    r_eap_eapm_network_cnf_t * pCnf;
    uint8_t *                  extAddr = NULL;
    r_result_t                 result;

    if (
        (no < R_DEMO_CLIENT_NUM_MAX) &&
        (R_DEMO_CSTATE_INVALID != g_demo_client.tbl.cTable[no].valid_state)
        )
    {
        extAddr = g_demo_client.tbl.cTable[no].extended_address;
    }

    if (NULL == extAddr)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> No is invalid:");
#endif
        return;
    }

#ifdef __PLC__DEBUG__
    R_STDIO_Printf ("\n -> Key Removeing device 0x%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X.",

                    extAddr[0], extAddr[1], extAddr[2], extAddr[3],
                    extAddr[4], extAddr[5], extAddr[6], extAddr[7]
                    );
#endif
    R_memcpy (req.extendedAddress, extAddr, 8);
    req.reqHandle = g_demo_entity.nsduHandle++;
    req.gmkId = index;
    req.cmdId = R_EAP_NETWORK_GMKREMOVAL;

    result = R_DEMO_EapmNetwork (R_DEMO_G3_USE_PRIMARY_CH, &req, &pCnf);

    /* Check if KICK was successful. */
    if ((R_RESULT_SUCCESS == result) && (R_EAP_STATUS_SUCCESS == pCnf->status))
    {
        g_demo_client.tbl.cTable[no].valid_state = R_DEMO_CSTATE_LEAVED;
    }
} /* R_LBS_RemoveKeyDev */
/******************************************************************************
   End of function  R_LBS_RemoveKeyDev
******************************************************************************/

/******************************************************************************
* Function Name: R_DEMO_GetClientInfo
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static r_eap_clientinfo_table_t *R_DEMO_GetClientInfo (const uint8_t * extendedAddress)
{
    uint16_t                   i;
    uint16_t                   blankIndex = 0xFFFF;
    r_eap_clientinfo_table_t * ptr = NULL;
    for (i = 0; i < R_DEMO_CLIENT_NUM_MAX; i++)
    {
        if (R_DEMO_CSTATE_INVALID == g_demo_client.tbl.cTable[i].valid_state)
        {
            if (0xFFFF == blankIndex)
            {
                blankIndex = i;
            }
        }
        else if (R_memcmp (g_demo_client.tbl.cTable[i].extended_address, extendedAddress, 8) == 0)
        {
            ptr = &g_demo_client.tbl.cTable[i];
            break;
        }
        else
        {
            /**/
        }
    }
    if (
        (R_DEMO_CLIENT_NUM_MAX == i) &&
        (R_DEMO_CSTATE_INVALID == g_demo_client.tbl.cTable[blankIndex].valid_state)
        )
    {
        ptr = &g_demo_client.tbl.cTable[blankIndex];
    }

    return ptr;
} /* R_DEMO_GetClientInfo */
/******************************************************************************
   End of function  R_DEMO_GetClientInfo
******************************************************************************/


/******************************************************************************
* Function Name: R_DEMO_AppHandleEapNewDeviceInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_DEMO_AppHandleEapNewDeviceInd (const r_eap_eapm_newdevice_ind_t * ind)
{
    r_eap_clientinfo_table_t * pTbl;

    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Received EAP Newdevice indication with EUI64 address: 0x%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X",
                        ind->extendedAddress[0], ind->extendedAddress[1], ind->extendedAddress[2], ind->extendedAddress[3],
                        ind->extendedAddress[4], ind->extendedAddress[5], ind->extendedAddress[6], ind->extendedAddress[7]
                        );
#endif
    }

    pTbl = (r_eap_clientinfo_table_t *)R_DEMO_GetClientInfo (ind->extendedAddress);

    if (NULL == pTbl)
    {
        return;
    }
    if (R_DEMO_CSTATE_INVALID == pTbl->valid_state)
    {
        pTbl->access_ctrl = R_FALSE;
        R_memcpy (pTbl->PSK, g_demo_config.pskKey, 16);
        R_memcpy (pTbl->extended_address, ind->extendedAddress, 8);
        R_BYTE_UInt16ToArr (++g_demo_client.networkAddrCnt, pTbl->network_address);
        pTbl->wait_msg3 = (g_demo_config.extIdFlg)? R_TRUE : R_FALSE;
    }
    else
    {
        if (g_demo_config.secyrityF2)
        {
            /* for Security Solution F2 (but don't care for kick and rekeying) */
            R_BYTE_UInt16ToArr (++g_demo_client.networkAddrCnt, pTbl->network_address);
        }
    }
    pTbl->valid_state = R_DEMO_CSTATE_VALID;

    R_DEMO_EapmSetClientinfoWrap (R_DEMO_G3_USE_PRIMARY_CH, pTbl);
} /* R_DEMO_AppHandleEapNewDeviceInd */
/******************************************************************************
   End of function  R_DEMO_AppHandleEapNewDeviceInd
******************************************************************************/


/******************************************************************************
* Function Name: R_DEMO_AppHandleEapNwkLeaveInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_DEMO_AppHandleEapNwkLeaveInd (const r_eap_eapm_network_leave_ind_t * ind)
{
    r_eap_clientinfo_table_t * pTbl;

    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Received EAP NeteworkLeave indication with EUI64 address: 0x%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X",
                        ind->extendedAddress[0], ind->extendedAddress[1], ind->extendedAddress[2], ind->extendedAddress[3],
                        ind->extendedAddress[4], ind->extendedAddress[5], ind->extendedAddress[6], ind->extendedAddress[7]
                        );
#endif
    }
    pTbl = R_DEMO_GetClientInfo (ind->extendedAddress);
    if (NULL == pTbl)
    {
        return;
    }
    pTbl->valid_state = R_DEMO_CSTATE_LEAVED;
} /* R_DEMO_AppHandleEapNwkLeaveInd */
/******************************************************************************
   End of function  R_DEMO_AppHandleEapNwkLeaveInd
******************************************************************************/


/******************************************************************************
* Function Name: R_DEMO_AppHandleEapNwkJoinInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_DEMO_AppHandleEapNwkJoinInd (const r_eap_eapm_network_join_ind_t * ind)
{
    r_eap_clientinfo_table_t * pTbl;


    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Received EAP NeteworkJoin indication");
#endif
    }

    pTbl = R_DEMO_GetClientInfo (ind->extendedAddress);
    if (NULL == pTbl)
    {
        return;
    }
    pTbl->valid_state = R_DEMO_CSTATE_JOINED;

    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> NetworkAddress 0x%.2X%.2X  EUI64:0x%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X",
                        pTbl->network_address[0], pTbl->network_address[1],
                        ind->extendedAddress[0], ind->extendedAddress[1], ind->extendedAddress[2], ind->extendedAddress[3],
                        ind->extendedAddress[4], ind->extendedAddress[5], ind->extendedAddress[6], ind->extendedAddress[7]
                        );
#endif
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> mediaType:%d Status: %s(0x%.2X)\n", ind->mediaType, R_G3_Status2Text(R_G3_INFO_LAYER_EAP, ind->status), ind->status);
#endif
#else
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Status: %s(0x%.2X)\n", R_G3_Status2Text (R_G3_MODE_EAP, ind->status), ind->status);
#endif
#endif

    }
} /* R_DEMO_AppHandleEapNwkJoinInd */
/******************************************************************************
   End of function  R_DEMO_AppHandleEapNwkJoinInd
******************************************************************************/



/******************************************************************************
* Function Name: R_DEMO_AppHandleEapEapKeyInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_DEMO_AppHandleEapEapKeyInd (const r_eap_eapm_eap_key_ind_t * ind)
{
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Received EAP EapKey indication (type = %s) with EUI64 address: 0x%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X",
                        R_G3_Def2text(R_G3_DEF_TYPE_KEY_TYPE, ind->type),
                        ind->extendedAddress[0], ind->extendedAddress[1], ind->extendedAddress[2], ind->extendedAddress[3],
                        ind->extendedAddress[4], ind->extendedAddress[5], ind->extendedAddress[6], ind->extendedAddress[7]
                        );
#endif
    }

    if ((g_demo_config.extIdFlg)&&(R_EAPPSK_KEY_TYPE_ID_P == ind->type))
    {
        r_eap_clientinfo_table_t * pTbl = (r_eap_clientinfo_table_t *)R_DEMO_GetClientInfo(ind->extendedAddress);

        if ((NULL != pTbl) && (pTbl->valid_state) && (R_TRUE == pTbl->wait_msg3))
        {
#if 0
            while (1)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("\n d - discard the joining");
#endif
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("\n all other - accept the joinnig");
#endif

                R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

                if (strlen((char *)g_demo_buff.getStringBuffer) == 1)
                {
                    if ('d' == g_demo_buff.getStringBuffer[0])
                    {
                        return;
                    }
                }
                else
                {
                    break;
                }
            }
#endif
            pTbl->wait_msg3 = R_FALSE;
            R_DEMO_EapmSetClientinfoWrap(R_DEMO_G3_USE_PRIMARY_CH, pTbl);
        }
    }
} /* R_DEMO_AppHandleEapEapKeyInd */
/******************************************************************************
   End of function  R_DEMO_AppHandleEapEapKeyInd
******************************************************************************/

/******************************************************************************
* Function Name: R_DEMO_EapInit
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_EapInit (uint8_t chId)
{
    r_g3_init_req_t req = { 0 };
    r_g3_callback_t callBack;
#ifdef __PLC__DEBUG__
    R_STDIO_Printf ("\n -> CPX3 Init as EAP mode(ch%d)...", chId);
#endif

    req.g3mode = R_G3_MODE_EAP;
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    req.g3mode |= (g_demo_config.mediaType << 4);
#endif
    req.init.eap.adpdBuffNum = R_DEMO_ADP_ADPD_DATA_QUEUE_SIZE;
    req.init.eap.routeTableSize = R_DEMO_ADP_ROUTING_TABLE_SIZE;
    req.init.eap.neighbourTableSize = R_DEMO_G3MAC_NEIGBOUR_TABLE_SIZE;
    req.init.eap.deviceTableSize = R_DEMO_G3MAC_DEVICE_TABLE_SIZE;
    req.init.eap.panDescriptorNum = R_DEMO_ADP_MAX_PAN_DESCRIPTORS;
    req.init.eap.routeType = 0; /* reserved */

    req.init.eap.eapLbpBuffNum = R_DEMO_EAP_LBP_BUFF_SIZE;
    req.init.eap.eapCinfoTableNum = R_DEMO_EAP_CINFO_TABLE_SIZE;
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    req.init.hyeap.posTableSize_RF = 100;
#endif


#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    if (R_DEMO_InitEapCallBack (chId, &callBack, g_demo_config.mediaType) != R_RESULT_SUCCESS)
#else
    if (R_DEMO_InitEapCallBack (chId, &callBack) != R_RESULT_SUCCESS)
#endif
    {
        return R_RESULT_FAILED;
    }

    if (R_G3_Init (chId, &callBack, &req, 2000) != R_RESULT_SUCCESS)
    {
        return R_RESULT_FAILED;
    }
    else
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("done.");
#endif
        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_EapInit */
/******************************************************************************
   End of function  R_DEMO_EapInit
******************************************************************************/


/******************************************************************************
* Function Name: R_DEMO_EapmStart
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_EapmStart (uint8_t chId, r_eap_eapm_start_cnf_t ** cnf)
{
    r_result_t                          status;
    R_VOLATILE r_eap_eapm_start_cnf_t * eStaCfm = (R_VOLATILE r_eap_eapm_start_cnf_t *)&g_g3cb[chId].eapmStartCnf;
    *cnf = (r_eap_eapm_start_cnf_t *)eStaCfm;

    /* As server, start a network */
    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_EAPM_START);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Starting EAP (EAPM-START)...");
#endif
    }

    status = R_EAP_EapmStartReq (chId);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_EAP_EapmStartReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_EAPM_START);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_EAP_STATUS_SUCCESS == eStaCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("success.\n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text (R_G3_MODE_EAP, eStaCfm->status), eStaCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_EapmStart */
/******************************************************************************
   End of function  R_DEMO_EapmStart
******************************************************************************/



/******************************************************************************
* Function Name: R_DEMO_EapmNetwork
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_EapmNetwork (uint8_t chId, const r_eap_eapm_network_req_t * eNwkReq,
                               r_eap_eapm_network_cnf_t ** cnf)
{
    r_result_t                            status;
    R_VOLATILE r_eap_eapm_network_cnf_t * eNwkCfm = (R_VOLATILE r_eap_eapm_network_cnf_t *)&g_g3cb[chId].eapmNetworkCnf;
    *cnf = (r_eap_eapm_network_cnf_t *)eNwkCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_EAPM_NETWORK);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Setting EAP NETWORK REQUEST mode (1:Kick, 4:GMK Remove)  0x%.2X...", eNwkReq->cmdId);
#endif
    }

    status = R_EAP_EapmNetworkReq (chId, (r_eap_eapm_network_req_t *)eNwkReq);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_EAP_EapmNetworkReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_EAPM_NETWORK);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_EAP_STATUS_SUCCESS == eNwkCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("success. EUI64: %.8X\n", R_BYTE_ArrToUInt64 ((uint8_t *)eNwkCfm->extendedAddress));
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("failed. Status: %s (0x%.2X) EUI64: %.8X\n", R_G3_Status2Text (R_G3_MODE_EAP, eNwkCfm->status), eNwkCfm->status, R_BYTE_ArrToUInt64 ((uint8_t *)eNwkCfm->extendedAddress));
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_EapmNetwork */
/******************************************************************************
   End of function  R_DEMO_EapmNetwork
******************************************************************************/



/******************************************************************************
* Function Name: R_DEMO_EapmSet
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_EapmSet (uint8_t chId, const r_eap_eapm_set_req_t * setReq,
                           r_eap_eapm_set_cnf_t ** cnf)
{
    uint8_t                           len;
    r_result_t                        status;
    R_VOLATILE r_eap_eapm_set_cnf_t * setCfm = (R_VOLATILE r_eap_eapm_set_cnf_t *)&g_g3cb[chId].eapmSetCnf;
    *cnf = (r_eap_eapm_set_cnf_t *)setCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_EAPM_SET);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Setting EAP IB %s(0x%.2X) Index: %d...", R_G3_Ibid2Text(R_G3_INFO_LAYER_EAP, setReq->eibAttributeId, &len), setReq->eibAttributeId, setReq->eibAttributeIndex);
#endif
    }

    status = R_EAP_EapmSetReq (chId, (r_eap_eapm_set_req_t *)setReq);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_EAP_EapmSetReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_EAPM_SET);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_EAP_STATUS_SUCCESS == setCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("success.\n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text (R_G3_MODE_EAP, setCfm->status), setCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_EapmSet */
/******************************************************************************
   End of function  R_DEMO_EapmSet
******************************************************************************/



/******************************************************************************
* Function Name: R_DEMO_EapmSetClientinfo
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_EapmSetClientinfo (uint8_t chId, const r_eap_eapm_set_clientinfo_req_t * setReq,
                                     r_eap_eapm_set_clientinfo_cnf_t ** cnf)
{
    r_result_t                                   status;
    R_VOLATILE r_eap_eapm_set_clientinfo_cnf_t * setCfm = (R_VOLATILE r_eap_eapm_set_clientinfo_cnf_t *)&g_g3cb[chId].eapmSetClientInfoCnf;
    *cnf = (r_eap_eapm_set_clientinfo_cnf_t *)setCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_EAPM_SETCLIENTINFO);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Setting EAP Set Client EUI64 0x%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X \n\t NetworkAddress 0x%.2X%.2X...",
                        setReq->extendedAddress[0], setReq->extendedAddress[1], setReq->extendedAddress[2], setReq->extendedAddress[3],
                        setReq->extendedAddress[4], setReq->extendedAddress[5], setReq->extendedAddress[6], setReq->extendedAddress[7],
                        setReq->networkAddress[0], setReq->networkAddress[1]
                        );
#endif
    }

    status = R_EAP_EapmSetClientinfoReq (chId, (r_eap_eapm_set_clientinfo_req_t *)setReq);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_EAP_EapmSetClientinfoReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_EAPM_SETCLIENTINFO);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_EAP_STATUS_SUCCESS == setCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("success.\n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text (R_G3_MODE_EAP, setCfm->status), setCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_EapmSetClientinfo */
/******************************************************************************
   End of function  R_DEMO_EapmSetClientinfo
******************************************************************************/


/******************************************************************************
* Function Name: R_DEMO_EapmGet
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_EapmGet (uint8_t chId, const r_eap_eapm_get_req_t * getReq,
                           r_eap_eapm_get_cnf_t ** cnf)
{
    uint8_t                           len;
    r_result_t                        status;
    R_VOLATILE r_eap_eapm_get_cnf_t * getCfm = (R_VOLATILE r_eap_eapm_get_cnf_t *)&g_g3cb[chId].eapmGetCnf;
    *cnf = (r_eap_eapm_get_cnf_t *)getCfm;

    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_EAPM_GET);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Getting EAP IB %s(0x%.2X) Index: %d...", R_G3_Ibid2Text(R_G3_INFO_LAYER_EAP, getReq->eibAttributeId, &len), getReq->eibAttributeId, getReq->eibAttributeIndex);
#endif
    }

    status = R_EAP_EapmGetReq (chId, (r_eap_eapm_get_req_t *)getReq);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("failed");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_EAP_EapmGetReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_EAPM_GET);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_EAP_STATUS_SUCCESS == getCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("success.\n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text (R_G3_MODE_EAP, getCfm->status), getCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_EapmGet */
/******************************************************************************
   End of function  R_DEMO_EapmGet
******************************************************************************/


/******************************************************************************
* Function Name: R_DEMO_EapmReset
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_EapmReset (uint8_t chId, r_eap_eapm_reset_cnf_t ** cnf)
{
    r_result_t                          status;
    R_VOLATILE r_eap_eapm_reset_cnf_t * rstCfm = (R_VOLATILE r_eap_eapm_reset_cnf_t *)&g_g3cb[chId].eapmResetCnf;
    *cnf = (r_eap_eapm_reset_cnf_t *)rstCfm;

    /* As server, start a network */
    R_DEMO_ClearEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_EAPM_RESET);

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Resetting device...");
#endif
    }

    status = R_EAP_EapmResetReq (chId);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_EAP_EapmResetReq */
        status = R_DEMO_WaitEventFlag(&g_g3cb[chId].statusFlags, R_FLAG_EAPM_RESET);
        if (R_RESULT_SUCCESS != status)
        {
            return status;
        }

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_EAP_STATUS_SUCCESS == rstCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("success.\n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("failed. Status: %s(0x%.2X)\n", R_G3_Status2Text (R_G3_MODE_EAP, rstCfm->status), rstCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_EapmReset */
/******************************************************************************
   End of function  R_DEMO_EapmReset
******************************************************************************/


/******************************************************************************
* Function Name: R_DEMO_EapmGetWrap
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_eap_status_t R_DEMO_EapmGetWrap (uint8_t chId, uint8_t id, uint16_t index, uint8_t * val)
{
    r_eap_eapm_get_req_t   req;
    r_eap_eapm_get_cnf_t * eGetCfm;

    req.eibAttributeId = id;
    req.eibAttributeIndex = index;

    if (R_DEMO_EapmGet (chId, &req, &eGetCfm) == R_RESULT_SUCCESS)
    {
        R_memcpy (val, eGetCfm->eibAttributeValue, sizeof (eGetCfm->eibAttributeValue));
        return (r_eap_status_t)eGetCfm->status;
    }
    else
    {
        return R_EAP_STATUS_FAILED;
    }
} /* R_DEMO_EapmGetWrap */
/******************************************************************************
   End of function  R_DEMO_EapmGetWrap
******************************************************************************/



/******************************************************************************
* Function Name: R_DEMO_EapmSetWrap
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_eap_status_t R_DEMO_EapmSetWrap (uint8_t chId, uint8_t id, uint16_t index, uint8_t * val)
{
    r_eap_eapm_set_req_t   req;
    r_eap_eapm_set_cnf_t * eSetCfm;

    req.eibAttributeId = id;
    req.eibAttributeIndex = index;
    req.eibAttributeValue = val;
    if (R_DEMO_EapmSet (chId, &req, &eSetCfm) == R_RESULT_SUCCESS)
    {
        return (r_eap_status_t)eSetCfm->status;
    }
    else
    {
        return R_EAP_STATUS_FAILED;
    }
}
/******************************************************************************
   End of function  R_DEMO_EapmSetWrap
******************************************************************************/




/******************************************************************************
* Function Name: R_DEMO_EapmSetClientinfoWrap
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_eap_status_t R_DEMO_EapmSetClientinfoWrap (uint8_t chId, r_eap_clientinfo_table_t * pTbl)
{
    r_eap_eapm_set_clientinfo_cnf_t * pCnf;
    r_eap_eapm_set_clientinfo_req_t   req;

    R_memcpy (&req, pTbl, sizeof (r_eap_clientinfo_table_t));
    if (R_DEMO_EapmSetClientinfo (chId, &req, &pCnf) == R_RESULT_SUCCESS)
    {
        return (r_eap_status_t)pCnf->status;
    }
    else
    {
        return R_EAP_STATUS_FAILED;
    }
}
/******************************************************************************
   End of function  R_DEMO_EapmSetClientinfoWrap
******************************************************************************/





/******************************************************************************
* Function Name: set_eap_ib_entry
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t set_eap_ib_entry (uint8_t chId, const r_eap_ib_id_t attributeId)
{
    r_eap_eapm_set_req_t   eapmSetReq;
    r_eap_eapm_set_cnf_t * eapmSetCfm;

    uint8_t                tempArray[R_EAP_MAX_IB_SIZE];

    /* Set request structure. */
    eapmSetReq.eibAttributeId = attributeId;
    eapmSetReq.eibAttributeIndex = 0;
    eapmSetReq.eibAttributeValue = tempArray;

    switch (attributeId)
    {
        /* One byte elements. */
        case R_EAP_IB_ACTIVEKEYINDEX:
        case R_EAP_IB_JOINGMKID:
        case R_EAP_IB_KEY_IND_ENABLE:
        case R_EAP_IB_IDP_IND_ENABLE:

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 8-bit input (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 4)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.2X", hex_string_to_uint8 (g_demo_buff.getStringBuffer));
#endif

                eapmSetReq.eibAttributeValue[0] = hex_string_to_uint8 (g_demo_buff.getStringBuffer);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            R_DEMO_EapmSet (chId, &eapmSetReq, &eapmSetCfm);
            break;

        /* Two byte elements. */
        case R_EAP_IB_COORDSHORTADDRESS:
        case R_EAP_IB_CINFOTABLEENTRIES:
        case R_EAP_IB_JOIN_WAITTIMESEC:
        case R_EAP_IB_SOFTVERSION:

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 16-bit input (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.4X", hex_string_to_uint16 (g_demo_buff.getStringBuffer));
#endif

                R_BYTE_UInt16ToArr (hex_string_to_uint16 (g_demo_buff.getStringBuffer), eapmSetReq.eibAttributeValue);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            R_DEMO_EapmSet (chId, &eapmSetReq, &eapmSetCfm);
            break;

        case R_EAP_IB_GMK:
        case R_EAP_IB_CLIENTINFOTABLE:
        case R_EAP_IB_STATISTICS:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("-> Writing of this attribute is currently not supported.");
#endif
            break;

        default:

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Unknown ID");
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 32-bit input (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 10)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.8X", hex_string_to_uint32 (g_demo_buff.getStringBuffer));
#endif

                R_BYTE_UInt32ToArr (hex_string_to_uint32 (g_demo_buff.getStringBuffer), eapmSetReq.eibAttributeValue);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            R_DEMO_EapmSet (chId, &eapmSetReq, &eapmSetCfm);
            break;

    } /* switch */

    return R_RESULT_SUCCESS;
} /* set_eap_ib_entry */
/******************************************************************************
   End of function  set_eap_ib_entry
******************************************************************************/




/******************************************************************************
* Function Name: get_eap_ib_entry
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t get_eap_ib_entry (uint8_t chId, const r_eap_ib_id_t attributeId)
{
    uint16_t                   i;
    uint16_t                   tableNum;
    r_eap_eapm_get_req_t       eapmGetReq;
    r_eap_eapm_get_cnf_t *     eapmGetCfm;

    r_eap_clientinfo_table_t * pCinfoTable;

    /* Set request structure. */
    eapmGetReq.eibAttributeId = attributeId;
    eapmGetReq.eibAttributeIndex = 0;

    switch (attributeId)
    {
        /* One byte elements. */
        case R_EAP_IB_ACTIVEKEYINDEX:
        case R_EAP_IB_JOINGMKID:
        case R_EAP_IB_KEY_IND_ENABLE:
        case R_EAP_IB_IDP_IND_ENABLE:
            R_DEMO_EapmGet (chId, &eapmGetReq, &eapmGetCfm);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.2X", *(uint8_t *)(eapmGetCfm->eibAttributeValue));
#endif
            break;

        /* Two byte elements. */
        case R_EAP_IB_COORDSHORTADDRESS:
        case R_EAP_IB_CINFOTABLEENTRIES:
        case R_EAP_IB_JOIN_WAITTIMESEC:
        case R_EAP_IB_SOFTVERSION:
            R_DEMO_EapmGet (chId, &eapmGetReq, &eapmGetCfm);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.4X", R_BYTE_ArrToUInt16 ((eapmGetCfm->eibAttributeValue)));
#endif
            break;

        case R_EAP_IB_STATISTICS:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 16-bit index (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.4X", hex_string_to_uint16 (g_demo_buff.getStringBuffer));
#endif

                eapmGetReq.eibAttributeIndex = hex_string_to_uint16 (g_demo_buff.getStringBuffer);
            }
            else
            {
                return R_RESULT_FAILED;
            }
            R_DEMO_EapmGet (chId, &eapmGetReq, &eapmGetCfm);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.8X", R_BYTE_ArrToUInt32 ((eapmGetCfm->eibAttributeValue)));
#endif
            break;

        case R_EAP_IB_CLIENTINFOTABLE:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n --- Routing Table ---");
#endif

            R_DEMO_EapmGetWrap (chId, R_EAP_IB_CINFOTABLEENTRIES, 0, g_demo_buff.getStringBuffer);

            if (R_BYTE_ArrToUInt16 (g_demo_buff.getStringBuffer) == 0)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\n    [EMPTY] Client Information Table \n");
#endif
                break;
            }
            tableNum = R_BYTE_ArrToUInt16 (g_demo_buff.getStringBuffer);

            for (i = 0; i < tableNum; i++)
            {
                /* For larger tables, show in parts. */
                if ((i > 0) &&
                    ((i % R_DEMPO_APP_EAP_ENTRIES_PER_CYCLE) == 0))
                {
                    R_DEMO_HndWaitEnter();
                }

                /* Get next element */
                R_DEMO_EapmGetWrap (chId, attributeId, i, g_demo_buff.getStringBuffer);

                pCinfoTable = (r_eap_clientinfo_table_t *)g_demo_buff.getStringBuffer;

                if (pCinfoTable->valid_state)
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\n%4d:  Access Control(Black List): %d\n", i, pCinfoTable->access_ctrl);
#endif
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("    ExtAddress: 0x%02X%02X%02X%02X%02X%02X%02X%02X\n    NetworkAddress: 0x%02X%02X\n    PSK: 0x%02X%02X%02X%02X%02X%02X%02X%02X_%02X%02X%02X%02X%02X%02X%02X%02X\n",
                                    pCinfoTable->extended_address[0], pCinfoTable->extended_address[1], pCinfoTable->extended_address[2], pCinfoTable->extended_address[3],
                                    pCinfoTable->extended_address[4], pCinfoTable->extended_address[5], pCinfoTable->extended_address[6], pCinfoTable->extended_address[7],
                                    pCinfoTable->network_address[0], pCinfoTable->network_address[1],
                                    pCinfoTable->PSK[0], pCinfoTable->PSK[1], pCinfoTable->PSK[2], pCinfoTable->PSK[3],
                                    pCinfoTable->PSK[4], pCinfoTable->PSK[5], pCinfoTable->PSK[6], pCinfoTable->PSK[7],
                                    pCinfoTable->PSK[8], pCinfoTable->PSK[9], pCinfoTable->PSK[10], pCinfoTable->PSK[11],
                                    pCinfoTable->PSK[12], pCinfoTable->PSK[13], pCinfoTable->PSK[14], pCinfoTable->PSK[15]);
#endif
                }
                else
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\n Entry not set.");
#endif
                }
            }

            break;

        default:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Unknown ID");
#endif
            R_DEMO_EapmGet (chId, &eapmGetReq, &eapmGetCfm);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.8X", R_BYTE_ArrToUInt32 ((eapmGetCfm->eibAttributeValue)));
#endif
            break;

    } /* switch */

    return R_RESULT_SUCCESS;
} /* get_eap_ib_entry */
/******************************************************************************
   End of function  get_eap_ib_entry
******************************************************************************/

