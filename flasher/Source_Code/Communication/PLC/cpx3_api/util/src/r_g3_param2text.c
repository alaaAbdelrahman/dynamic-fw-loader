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
* http://www.renesas.com/disclaimer *
* Copyright (C) 2018-2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_g3_param2text.c
* Version      : 1.0
* Description  : Convert structure to text for log output
******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 13.07.2018 1.00     First Release
******************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"
#include "r_g3_api.h"
#include "r_g3_cmd.h"
#include "r_g3mac_statistics.h"
#include "r_adp_statistics.h"
#include "r_hyal_statistics.h"
#include "r_eap_statistics.h"
#include "r_cmn_param2text.h"
#include "r_g3_param2text.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define IB_LENGTH_8      (sizeof (uint8_t))
#define IB_LENGTH_16     (sizeof (uint16_t))
#define IB_LENGTH_32     (sizeof (uint32_t))
#define IB_LENGTH_TABLE  (0u)

#define STATUS_CASE(id, msg)  case R_##id##_STATUS_##msg: \
        p_ret = #msg; break;
#define IBID_GEN(id, size)    case R_##id: \
        * txt = #id; * len = size; break;

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum
{
    R_G3IFCOMM_LOG_ID_REQ,
    R_G3IFCOMM_LOG_ID_REQ_ERROR,
    R_G3IFCOMM_LOG_ID_CB,
    R_G3IFCOMM_LOG_ID_CB_MSG_FULL,
    R_G3IFCOMM_LOG_ID_CB_BUFF_FULL,
    R_G3IFCOMM_LOG_ID_TIMEOUT,
    R_G3IFCOMM_LOG_ID_LOW_LAYER_CMD,
}r_g3ifcomm_log_id_t;

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static uint32_t  g3_loginfo2text_if( uint8_t *log, char* pout_text);
static const char *g3_stats2text_umac (uint32_t index);
static const char *g3_stats2text_adp (uint32_t index);
static const char *g3_stats2text_eap (uint32_t index);
static const char *g3_unitid2text(uint8_t id);
static const char *g3_cmdid2text(uint8_t unitId, uint8_t cmdId);
static const char *g3_cmdid2text_g3(uint8_t cmdId);
static const char *g3_cmdid2text_mac(uint8_t cmdId);
static const char *g3_cmdid2text_adp(uint8_t cmdId);
static const char *g3_cmdid2text_eap(uint8_t cmdId);
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
static const char *g3_cmdid2text_rfmac(uint8_t cmdId);
static const char *g3_cmdid2text_rflmac(uint8_t cmdId);
#endif
static const char *g3_cmd_type2text(uint8_t type);
static const char *g3_status2text_ctrl (uint8_t status);
static const char *g3_status2text_mac (uint8_t status);
static const char *g3_status2text_adp (uint32_t status);
static const char *g3_status2text_eap (uint32_t status);
static uint32_t g3_ibid2text_mac (uint16_t id, const char ** txt, uint8_t * len);
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
static uint32_t g3_ibid2text_rfmac (uint16_t id, const char ** txt, uint8_t * len);
#endif
static uint32_t g3_ibid2text_adp (uint16_t id, const char ** txt, uint8_t * len);
static uint32_t g3_ibid2text_eap (uint16_t id, const char ** txt, uint8_t * len);
static const char *g3_layer2text(uint8_t layer);
static const char *g3_type_info2text(uint8_t layer);
static const char *g3_layer_info2text(uint8_t layer);
static const char *g3_media_type2text(r_media_type_t type);
static const char *g3_bandplan2text(uint8_t mode);
static const char *g3_addr_mode2text(uint8_t mode);
static const char *g3_key_state2text(uint8_t state);
static const char *g3_metric_type2text(uint8_t type);
static const char *g3_key_type2text(uint8_t type);
static const char *g3_mod_type2text(uint8_t modType);
static const char *g3_mps2text(uint8_t pms);


/******************************************************************************
* Function Name : R_G3_LogInfo2Text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
uint32_t R_G3_LogInfo2Text( uint8_t layer, uint8_t *log, char* pout_text)
{
    uint32_t length;
    
    if( R_G3_INFO_LAYER_LMAC_DSP == layer )
    {
		length = 0;
    }
    else
    {
        length = g3_loginfo2text_if( log, pout_text );
    }
    
    return length;
}
/******************************************************************************
   End of function  R_G3_LogInfo2Text
******************************************************************************/


/******************************************************************************
* Function Name : g3_loginfo2text_if
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static uint32_t  g3_loginfo2text_if( uint8_t *log, char* pout_text)
{
    uint8_t ch      = (log[3]>>6);
    uint8_t core    = ((log[3]>>4)&0x03);
    uint8_t unitId  = (log[3]&0x0F);
    uint8_t logId   = log[2];
    uint16_t param  = ( (log[1]<<8) | log[0] );
    uint32_t length;

    length  = sprintf(pout_text, "ch%d %s ", ch, g3_unitid2text(unitId) );

    {
        if( core != 0 ) /* IF LOG */
        {
            switch( logId )
            {
                case R_G3IFCOMM_LOG_ID_REQ:
                case R_G3IFCOMM_LOG_ID_CB:
                case R_G3IFCOMM_LOG_ID_LOW_LAYER_CMD:
                    {
                        length  += R_G3_FuncId2Text( param, &pout_text[length] );
                    }
                    break;
                case R_G3IFCOMM_LOG_ID_REQ_ERROR:
                    {
                        uint8_t cmdId = (param>>8)&0xFF;
                        uint8_t status = (param&0xFF);

                        length  += sprintf(&pout_text[length], "REQ ERROR:%s status=%s", g3_cmdid2text(unitId,cmdId),R_G3_Status2Text(unitId,status) );
                    }
                    break;
                case R_G3IFCOMM_LOG_ID_CB_MSG_FULL:
                    {
                        uint8_t cmdId = param&0xFF;

                        length  += sprintf(&pout_text[length], "IND MSG FULL %s", g3_cmdid2text(unitId,cmdId) );
                    }
                    break;
                case R_G3IFCOMM_LOG_ID_CB_BUFF_FULL:
                    {
                        uint8_t cmdId = param&0xFF;

                        length  += sprintf(&pout_text[length], "IND BUFF FULL %s", g3_cmdid2text(unitId,cmdId) );
                    }
                    break;
                case R_G3IFCOMM_LOG_ID_TIMEOUT:
                    {
                        uint8_t cmdId = param&0xFF;

                        length  += sprintf(&pout_text[length], "TIMEOUT %s", g3_cmdid2text(unitId,cmdId) );
                    }
                    break;
                default:
                    {
                        length  += sprintf(&pout_text[length], "0x%04X", param );
                    }
                    break;
            }
        }
        else
        {
            length  += sprintf(&pout_text[length], "timestamp 0x%02X%04X", logId, param );
        }
    }
    
    return length;
}
/******************************************************************************
   End of function  g3_loginfo2text_if
******************************************************************************/

/******************************************************************************
* Function Name : R_G3_Stats2Text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
const char *R_G3_Stats2Text(uint8_t layer, uint32_t index)
{
    const char *p_char;
    
    switch( layer )
    {
#ifndef R_DEFINE_DISABLE_UMAC
        case R_G3_INFO_LAYER_UMAC:
        {
            p_char = g3_stats2text_umac(index);
        }
        break;
#endif
        case R_G3_INFO_LAYER_ADP:
        {
            p_char = g3_stats2text_adp(index);
        }
        break;
        case R_G3_INFO_LAYER_EAP:
        {
            p_char = g3_stats2text_eap(index);
        }
        break;
        default:
        {
            p_char = "UNDEFINED ID or below layer status";
        }
    }
    
    return p_char;
}
/******************************************************************************
   End of function  R_G3_Stats2Text
******************************************************************************/


#ifndef R_DEFINE_DISABLE_UMAC
/******************************************************************************
* Function Name : g3_stats2text_umac
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_stats2text_umac (uint32_t index)
{
    const char * p_ret;
    
    switch (index)
    {
        STATS_TXT (G3MAC, TXDATAPACKETCOUNT)
        STATS_TXT (G3MAC, RXDATAPACKETCOUNT)
        STATS_TXT (G3MAC, TXCMDPACKETCOUNT)
        STATS_TXT (G3MAC, RXCMDPACKETCOUNT)
        STATS_TXT (G3MAC, CSMAFAILCOUNT)
        STATS_TXT (G3MAC, CSMANOACKCOUNT)
        STATS_TXT (G3MAC, TXDATABROADCASTCOUNT)
        STATS_TXT (G3MAC, RXDATABROADCASTCOUNT)
        STATS_TXT (G3MAC, BADCRCCOUNT)
        STATS_TXT (G3MAC, ALL_IN_RECEIVES)
        STATS_TXT (G3MAC, ALL_IN_DISCARDS)
        STATS_TXT (G3MAC, ALL_OUT_TRANSMITS)
        STATS_TXT (G3MAC, ALL_OUT_SUCCESS)
        STATS_TXT (G3MAC, ALL_OUT_FAILURE)
        STATS_TXT (G3MAC, ALL_OUT_FAILURE_LR)
        STATS_TXT (G3MAC, DATA_IN_RECEIVES)
        STATS_TXT (G3MAC, DATA_IN_RECEIVES_BCAST)
        STATS_TXT (G3MAC, DATA_IN_REASM_REQDS)
        STATS_TXT (G3MAC, DATA_IN_REASM_DISCARDS)
        STATS_TXT (G3MAC, DATA_IN_REASM_OKS)
        STATS_TXT (G3MAC, DATA_IN_DISCARDS_DEVICETABLE)
        STATS_TXT (G3MAC, DATA_IN_DISCARDS_SECURITY)
        STATS_TXT (G3MAC, DATA_IN_DELIVERS_UNICAST)
        STATS_TXT (G3MAC, DATA_IN_DELIVERS_BCAST)
        STATS_TXT (G3MAC, DATA_OUT_TRANSMITS)
        STATS_TXT (G3MAC, DATA_OUT_TRANSMITS_BCAST)
        STATS_TXT (G3MAC, DATA_OUT_RETRANSMISSION)
        STATS_TXT (G3MAC, DATA_OUT_SUCCESS)
        STATS_TXT (G3MAC, DATA_OUT_FAILURE)
        STATS_TXT (G3MAC, DATA_OUT_BCAST_SUCCESS)
        STATS_TXT (G3MAC, DATA_OUT_BCAST_FAILURE)
        STATS_TXT (G3MAC, CMD_IN_BCNREQ)
        STATS_TXT (G3MAC, CMD_IN_BCN)
        STATS_TXT (G3MAC, CMD_IN_TMREQ)
        STATS_TXT (G3MAC, CMD_IN_TMRES)
        STATS_TXT (G3MAC, CMD_OUT_BCNREQ_TRANSMITS)
        STATS_TXT (G3MAC, CMD_OUT_BCNREQ_SUCCESS)
        STATS_TXT (G3MAC, CMD_OUT_BCNREQ_FAILLURE)
        STATS_TXT (G3MAC, CMD_OUT_BCN_TRANSMITS)
        STATS_TXT (G3MAC, CMD_OUT_BCN_SUCCESS)
        STATS_TXT (G3MAC, CMD_OUT_BCN_FAILLURE)
        STATS_TXT (G3MAC, CMD_OUT_TMRES_TRANSMITS)
        STATS_TXT (G3MAC, CMD_OUT_TMRES_SUCCESS)
        STATS_TXT (G3MAC, CMD_OUT_TMRES_FAILLURE)
        STATS_TXT (G3MAC, MCPS_DATA_REQD)
        STATS_TXT (G3MAC, MCPS_DATA_SUCCESS)
        STATS_TXT (G3MAC, MCPS_DATA_FAILURE)
        STATS_TXT (G3MAC, MCPS_DATA_FAILURE_NOACK)
        STATS_TXT (G3MAC, MCPS_DATA_FAILURE_CSMAFAILURE)
        STATS_TXT (G3MAC, MCPS_DATA_FAILURE_LMLABORT)
        STATS_TXT (G3MAC, MCPS_DATA_IND)
        STATS_TXT (G3MAC, MCPS_TMR_RECEIVE_IND)
        STATS_TXT (G3MAC, MCPS_TMR_TRANSMIT_IND)
        STATS_TXT (G3MAC, MCPS_SUCCESS_UC_ROBUST)
        STATS_TXT (G3MAC, MCPS_SUCCESS_UC_BPSK)
        STATS_TXT (G3MAC, MCPS_SUCCESS_UC_QPSK)
        STATS_TXT (G3MAC, MCPS_SUCCESS_UC_8PSK)
        STATS_TXT (G3MAC, MCPS_FAILURE_UC_ROBUST)
        STATS_TXT (G3MAC, MCPS_FAILURE_UC_BPSK)
        STATS_TXT (G3MAC, MCPS_FAILURE_UC_QPSK)
        STATS_TXT (G3MAC, MCPS_FAILURE_UC_8PSK)
        STATS_TXT (G3MAC, MCPS_SUCCESS_CSMA_NCW3)
        STATS_TXT (G3MAC, MCPS_SUCCESS_CSMA_NCW4)
        STATS_TXT (G3MAC, MCPS_SUCCESS_CSMA_NCW5)
        STATS_TXT (G3MAC, MCPS_SUCCESS_CSMA_NCW6)
        STATS_TXT (G3MAC, MCPS_SUCCESS_CSMA_NCW7)
        STATS_TXT (G3MAC, MCPS_SUCCESS_CSMA_NCW8)
        STATS_TXT (G3MAC, MCPS_SUCCESS_CSMA_NCW9)
        STATS_TXT (G3MAC, MCPS_SUCCESS_CSMA_NCW10)
        STATS_TXT (G3MAC, MCPS_FAILURE_CSMA_NCW3)
        STATS_TXT (G3MAC, MCPS_FAILURE_CSMA_NCW4)
        STATS_TXT (G3MAC, MCPS_FAILURE_CSMA_NCW5)
        STATS_TXT (G3MAC, MCPS_FAILURE_CSMA_NCW6)
        STATS_TXT (G3MAC, MCPS_FAILURE_CSMA_NCW7)
        STATS_TXT (G3MAC, MCPS_FAILURE_CSMA_NCW8)
        STATS_TXT (G3MAC, MCPS_FAILURE_CSMA_NCW9)
        STATS_TXT (G3MAC, MCPS_FAILURE_CSMA_NCW10)
        default:
            p_ret = NULL;
            break;
    }
    
    return p_ret;
}
/******************************************************************************
   End of function  g3_stats2text_umac
******************************************************************************/
#endif

/******************************************************************************
* Function Name : g3_stats2text_adp
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_stats2text_adp (uint32_t index)
{
    const char * p_ret;
    
    switch (index)
    {
        STATS_TXT (ADP, LOWPAN_IN_RECEIVES)
        STATS_TXT (ADP, LOWPAN_IN_HDR_ERRORS)
        STATS_TXT (ADP, LOWPAN_IN_MESH_RECEIVES)
        STATS_TXT (ADP, LOWPAN_IN_MESH_FORWDS)
        STATS_TXT (ADP, LOWPAN_IN_MESH_DELIVERS)
        STATS_TXT (ADP, LOWPAN_IN_REASM_REQDS)
        STATS_TXT (ADP, LOWPAN_IN_REASM_FAILS)
        STATS_TXT (ADP, LOWPAN_IN_REASM_OKS)
        STATS_TXT (ADP, LOWPAN_IN_COMP_REQDS)
        STATS_TXT (ADP, LOWPAN_IN_COMP_FAILS)
        STATS_TXT (ADP, LOWPAN_IN_COMP_OKS)
        STATS_TXT (ADP, LOWPAN_IN_DISCARDS)
        STATS_TXT (ADP, LOWPAN_IN_DISCARDS_SECURITY)
        STATS_TXT (ADP, LOWPAN_IN_DISCARDS_BCAST_TBL)
        STATS_TXT (ADP, LOWPAN_IN_DISCARDS_GROUP_TBL)
        STATS_TXT (ADP, LOWPAN_IN_DISCARDS_IPV6_BFULL)
        STATS_TXT (ADP, LOWPAN_IN_DISCARDS_LBP_BFULL)
        STATS_TXT (ADP, LOWPAN_IN_DISCARDS_RELAY_BFULL)
        STATS_TXT (ADP, LOWPAN_IN_DELIVERS)
        STATS_TXT (ADP, LOWPAN_IN_DELIVERS_UNICAST)
        STATS_TXT (ADP, LOWPAN_IN_DELIVERS_MULTICAST)
        STATS_TXT (ADP, LOWPAN_OUT_REQUESTS)
        STATS_TXT (ADP, LOWPAN_OUT_COMP_REQDS)
        STATS_TXT (ADP, LOWPAN_OUT_COMP_FAILS)
        STATS_TXT (ADP, LOWPAN_OUT_COMP_OKS)
        STATS_TXT (ADP, LOWPAN_OUT_FRAG_REQDS)
        STATS_TXT (ADP, LOWPAN_OUT_FRAG_FAILS)
        STATS_TXT (ADP, LOWPAN_OUT_FRAG_OKS)
        STATS_TXT (ADP, LOWPAN_OUT_FRAG_CREATES)
        STATS_TXT (ADP, LOWPAN_OUT_MESH_HOP_LIMIT_EXCEEDS)
        STATS_TXT (ADP, LOWPAN_OUT_MESH_NO_ROUTES)
        STATS_TXT (ADP, LOWPAN_OUT_MESH_REQUESTS)
        STATS_TXT (ADP, LOWPAN_OUT_MESH_FORWDS)
        STATS_TXT (ADP, LOWPAN_OUT_MESH_TRANSMITS)
        STATS_TXT (ADP, LOWPAN_OUT_DISCARDS)
        STATS_TXT (ADP, LOWPAN_OUT_TRANSMITS)
        STATS_TXT (ADP, LOWPAN_OUT_SUCCESS)
        STATS_TXT (ADP, LOWPAN_OUT_FAILURE)
        STATS_TXT (ADP, LOWPAN_OUT_FAILURE_NO_ACK)
        STATS_TXT (ADP, LOWPAN_OUT_IPV6_TRANSMITS)
        STATS_TXT (ADP, LOWPAN_OUT_IPV6_SUCCESS)
        STATS_TXT (ADP, LOWPAN_OUT_IPV6_FAILURE)
        STATS_TXT (ADP, LOWPAN_OUT_RELAY_TRANSMITS)
        STATS_TXT (ADP, LOWPAN_OUT_RELAY_SUCCESS)
        STATS_TXT (ADP, LOWPAN_OUT_RELAY_FAILURE)
        STATS_TXT (ADP, LOWPAN_IN_LBP_RECEIVES)
        STATS_TXT (ADP, LOWPAN_IN_LBP_DISCARDS)
        STATS_TXT (ADP, LOWPAN_OUT_LBP_DISCARDS)
        STATS_TXT (ADP, LOWPAN_OUT_LBP_REQUESTS)
        STATS_TXT (ADP, LOWPAN_OUT_LBP_TRANSMITS)
        STATS_TXT (ADP, LOWPAN_OUT_LBP_SUCCESS)
        STATS_TXT (ADP, LOWPAN_OUT_LBP_FAILURE)
        STATS_TXT (ADP, EAPPSK_SUCCESS)
        STATS_TXT (ADP, EAPPSK_FAILURE_MSG3_MACS)
        STATS_TXT (ADP, EAPPSK_FAILURE_MSG3_EAX)
        STATS_TXT (ADP, EAPPSK_FAILURE_MSG3_PCH)
        STATS_TXT (ADP, LOWPAN_IN_LOADNG_RECEIVES)
        STATS_TXT (ADP, LOWPAN_IN_LOADNG_RECEIVES_RREQ)
        STATS_TXT (ADP, LOWPAN_IN_LOADNG_RECEIVES_RREP)
        STATS_TXT (ADP, LOWPAN_IN_LOADNG_RECEIVES_RERR)
        STATS_TXT (ADP, LOWPAN_IN_LOADNG_RECEIVES_PREQ)
        STATS_TXT (ADP, LOWPAN_IN_LOADNG_RECEIVES_PREP)
        STATS_TXT (ADP, LOWPAN_IN_LOADNG_DISCARDS)
        STATS_TXT (ADP, LOWPAN_IN_LOADNG_DISCARDS_BY_BLACKLIST)
        STATS_TXT (ADP, LOWPAN_OUT_LOADNG_DISCARDS)
        STATS_TXT (ADP, LOWPAN_OUT_LOADNG_REQUESTS_RREQ)
        STATS_TXT (ADP, LOWPAN_OUT_LOADNG_REQUESTS_RREQ_REPAIR)
        STATS_TXT (ADP, LOWPAN_OUT_LOADNG_REQUESTS_PREQ)
        STATS_TXT (ADP, LOWPAN_OUT_LOADNG_TRANSMITS)
        STATS_TXT (ADP, LOWPAN_OUT_LOADNG_SUCCESS)
        STATS_TXT (ADP, LOWPAN_OUT_LOADNG_FAILURE)
        STATS_TXT (ADP, LOWPAN_OUT_LOADNG_FAILURE_NOACK)
        STATS_TXT (ADP, LOWPAN_OUT_LOADNG_FAILURE_NOACK_RREQ)
        STATS_TXT (ADP, LOWPAN_OUT_LOADNG_REQUESTS_RREQ_FWD)
        STATS_TXT (ADP, CLUSTER_DISCARD_RREQ_FWD)
        STATS_TXT (ADP, TRICKE_DISCARD)
        STATS_TXT (ADP, TXDISCARD_BLACK_PLC)
        STATS_TXT (ADP, TXDISCARD_BLACK_RF)
        STATS_TXT_WL (HYAL, DATAREQ_MEDIA_0)
        STATS_TXT_WL (HYAL, DATAREQ_MEDIA_1)
        STATS_TXT_WL (HYAL, DATAREQ_MEDIA_2)
        STATS_TXT_WL (HYAL, DATAREQ_MEDIA_3)
        STATS_TXT_WL (HYAL, DATAREQ_MEDIA_4)
        STATS_TXT_WL (HYAL, DATACNF_SUCCESS_MEDIA_0)
        STATS_TXT_WL (HYAL, DATACNF_SUCCESS_MEDIA_1)
        STATS_TXT_WL (HYAL, DATACNF_SUCCESS_MEDIA_2)
        STATS_TXT_WL (HYAL, DATACNF_SUCCESS_MEDIA_3)
        STATS_TXT_WL (HYAL, DATACNF_SUCCESS_MEDIA_4)
        STATS_TXT_WL (HYAL, DATACNF_ERR_MEDIA_0)
        STATS_TXT_WL (HYAL, DATACNF_ERR_MEDIA_1)
        STATS_TXT_WL (HYAL, DATACNF_ERR_MEDIA_2)
        STATS_TXT_WL (HYAL, DATACNF_ERR_MEDIA_3)
        STATS_TXT_WL (HYAL, DATACNF_ERR_MEDIA_4)
        STATS_TXT_WL (HYAL, TX_DATA_PLC)
        STATS_TXT_WL (HYAL, TX_DATA_RF)
        STATS_TXT_WL (HYAL, TX_BACKUPED_PLC)
        STATS_TXT_WL (HYAL, TX_BACKUPED_RF)
        STATS_TXT_WL (HYAL, TXDISCARD_NONPOS_PLC)
        STATS_TXT_WL (HYAL, TXDISCARD_NONPOS_RF)
        STATS_TXT_WL (HYAL, TXERR_NOACK_PLC)
        STATS_TXT_WL (HYAL, TXERR_NOACK_RF)
        STATS_TXT_WL (HYAL, TXERR_CA_PLC)
        STATS_TXT_WL (HYAL, TXERR_CA_RF)
        STATS_TXT_WL (HYAL, TXERR_DUTYLIMIT_RF)
        STATS_TXT_WL (HYAL, RX_DATA_PLC)
        STATS_TXT_WL (HYAL, RX_DATA_RF)
        STATS_TXT_WL (HYAL, RXDISCARD_DUPLICATE_PLC)
        STATS_TXT_WL (HYAL, RXDISCARD_DUPLICATE_RF)
        default:
            p_ret = NULL;
            break;
    }
    
    return p_ret;
}
/******************************************************************************
   End of function  g3_stats2text_adp
******************************************************************************/

/******************************************************************************
* Function Name : g3_stats2text_eap
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_stats2text_eap (uint32_t index)
{
    const char * p_ret;
    
    switch (index)
    {
        STATS_TXT (EAP, LBP_IN_RECEIVES)
        STATS_TXT (EAP, LBP_IN_RECEIVES_JOINRERQ)
        STATS_TXT (EAP, LBP_IN_RECEIVES_LEAVE)
        STATS_TXT (EAP, LBP_IN_DISCARD)
        STATS_TXT (EAP, LBP_OUT_TRANSMITS)
        STATS_TXT (EAP, LBP_OUT_SUCCESS)
        STATS_TXT (EAP, LBP_OUT_FAILURE)
        STATS_TXT (EAP, EAPPSK_SUCCESS)
        STATS_TXT (EAP, EAPPSK_FAILURE_MSG2_MACP)
        STATS_TXT (EAP, EAPPSK_FAILURE_MSG4_EAX)
        STATS_TXT (EAP, EAPPSK_FAILURE_MSG4_PCH)
        STATS_TXT (EAP, NEWDEVICE)
        STATS_TXT (EAP, JOIN_SUCCESS)
        STATS_TXT (EAP, JOIN_FAILURE)
        STATS_TXT (EAP, KICK_SUCCESS)
        STATS_TXT (EAP, KICK_FAILURE)
        STATS_TXT (EAP, GMKTRANS_SUCCESS)
        STATS_TXT (EAP, GMKTRANS_FAILURE)
        STATS_TXT (EAP, GMKACTIVATE_SUCCESS)
        STATS_TXT (EAP, GMKACTIVATE_FAILURE)
        default:
            p_ret = NULL;
            break;
    }
    
    return p_ret;
}
/******************************************************************************
   End of function  g3_stats2text_eap
******************************************************************************/

/******************************************************************************
* Function Name : R_G3_FuncId2Text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
uint32_t R_G3_FuncId2Text( uint16_t funcId, char* pout_text)
{
    uint32_t length;
    uint8_t type = R_G3_GET_CMDTYPE(funcId);
    uint8_t unit = R_G3_GET_UNITID(funcId);
    uint8_t cmdId = R_G3_GET_CMD_ID(funcId);

    length   = sprintf(pout_text, "%s ", g3_cmd_type2text(type) );
    length  += sprintf(&pout_text[length], "%s", g3_cmdid2text(unit,cmdId) );

    return length;
}
/******************************************************************************
   End of function  R_G3_FuncId2Text
******************************************************************************/

/******************************************************************************
* Function Name : g3_unitid2text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_unitid2text(uint8_t id)
{
    const char *p_ret;

    switch (id) {
    ENUM_CASE(R_G3_UNITID, G3CTRL )
    ENUM_CASE(R_G3_UNITID, RESERVE)
    ENUM_CASE(R_G3_UNITID, G3LMACIF)
    ENUM_CASE(R_G3_UNITID, G3MAC)
    ENUM_CASE(R_G3_UNITID, G3ADP)
    ENUM_CASE(R_G3_UNITID, G3EAP)
    ENUM_CASE(R_G3_UNITID, G3RFMAC)
    default:
        p_ret = "Unknown Type";
        break;
    }
    return p_ret;
}
/******************************************************************************
   End of function  g3_unitid2text
******************************************************************************/


/******************************************************************************
* Function Name : g3_cmdid2text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_cmdid2text(uint8_t unitId, uint8_t cmdId)
{
    const char *ret;

    switch( unitId )
    {
        case R_G3_UNITID_G3CTRL:
            ret = g3_cmdid2text_g3( cmdId );
            break;
        case R_G3_UNITID_G3MAC:
            ret = g3_cmdid2text_mac( cmdId );
            break;
        case R_G3_UNITID_G3ADP:
            ret = g3_cmdid2text_adp( cmdId );
            break;
        case R_G3_UNITID_G3EAP:
            ret = g3_cmdid2text_eap( cmdId );
            break;
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
        case R_G3_UNITID_G3RFMAC:
            ret = g3_cmdid2text_rfmac( cmdId );
            break;
        case R_G3_UNITID_RFLMAC:
            ret = g3_cmdid2text_rflmac( cmdId );
            break;
#endif
        default:
            ret = "Unknown Type";
            break;
    }
    return ret;
}
/******************************************************************************
   End of function  g3_cmdid2text
******************************************************************************/

/******************************************************************************
* Function Name : g3_cmdid2text_g3
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_cmdid2text_g3(uint8_t cmdId)
{
    const char *p_ret;

    switch( cmdId )
    {
    ENUM_CASE(R_G3_CMDID, G3_INIT )
    ENUM_CASE(R_G3_CMDID, G3_DEINIT )
    ENUM_CASE(R_G3_CMDID, G3_GETCONFIG )
    ENUM_CASE(R_G3_CMDID, G3_SETCONFIG )
    ENUM_CASE(R_G3_CMDID, G3_CLEARINFO )
    ENUM_CASE(R_G3_CMDID, G3_GETINFO )
    ENUM_CASE(R_G3_CMDID, G3_DUMP )
    ENUM_CASE(R_G3_CMDID, G3_DUMP_ABORT )
    ENUM_CASE(R_G3_CMDID, G3_EVENT )
    default:
        p_ret = "Unknown Type";
        break;
    }

    return p_ret;
}
/******************************************************************************
   End of function  g3_cmdid2text_g3
******************************************************************************/

/******************************************************************************
* Function Name : g3_cmdid2text_mac
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_cmdid2text_mac(uint8_t cmdId)
{
    const char *p_ret;

    switch( cmdId )
    {
#ifndef R_DEFINE_DISABLE_UMAC
    ENUM_CASE(R_G3_CMDID, MCPS_DATA )
    ENUM_CASE(R_G3_CMDID, MLME_RESET )
#endif
    ENUM_CASE(R_G3_CMDID, MLME_GET )
    ENUM_CASE(R_G3_CMDID, MLME_SET )
#ifndef R_DEFINE_DISABLE_UMAC
    ENUM_CASE(R_G3_CMDID, MLME_SCAN )
    ENUM_CASE(R_G3_CMDID, MLME_START )
    ENUM_CASE(R_G3_CMDID, MLME_BEACON_NOTIFY )
    ENUM_CASE(R_G3_CMDID, MLME_COMM_STATUS )
    ENUM_CASE(R_G3_CMDID, MLME_FRAMECOUNT )
#endif
    ENUM_CASE(R_G3_CMDID, MLME_TMR_RECEIVE )
    ENUM_CASE(R_G3_CMDID, MLME_TMR_TRANSMIT )

    default:
        p_ret = "Unknown Type";
        break;
    }

    return p_ret;
}
/******************************************************************************
   End of function  g3_cmdid2text_mac
******************************************************************************/

/******************************************************************************
* Function Name : g3_cmdid2text_adp
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_cmdid2text_adp(uint8_t cmdId)
{
    const char *p_ret;
    
    switch( cmdId )
    {
    case R_G3_CMDID_ADPD_DATA:            p_ret = "ADPD_DATA";            break;
    case R_G3_CMDID_ADPM_RESET:           p_ret = "ADPM_RESET";           break;
    case R_G3_CMDID_ADPM_DISCOVERY:       p_ret = "ADPM_DISCOVERY";       break;
    case R_G3_CMDID_ADPM_NETWORK_START:   p_ret = "ADPM_NETWORK_START";   break;
    case R_G3_CMDID_ADPM_NETWORK_JOIN:    p_ret = "ADPM_NETWORK_JOIN";    break;
    case R_G3_CMDID_ADPM_NETWORK_LEAVE:   p_ret = "ADPM_NETWORK_LEAVE";   break;
    case R_G3_CMDID_ADPM_GET:             p_ret = "ADPM_GET";             break;
    case R_G3_CMDID_ADPM_SET:             p_ret = "ADPM_SET";             break;
    case R_G3_CMDID_ADPM_ROUTE_DISCOVERY: p_ret = "ADPM_ROUTE_DISC";      break;
    case R_G3_CMDID_ADPM_PATH_DISCOVERY:  p_ret = "ADPM_PATH_DISC";       break;
    case R_G3_CMDID_ADPM_LBP:             p_ret = "ADPM_LBP";             break;
    case R_G3_CMDID_ADPM_NETWORK_STATUS:  p_ret = "ADPM_NETWK_STATUS";    break;
    case R_G3_CMDID_ADPM_BUFFER:          p_ret = "ADPM_BUFFER";          break;
    case R_G3_CMDID_ADPM_KEY_STATE:       p_ret = "ADPM_KEY_STATE";       break;
    case R_G3_CMDID_ADPM_ROUTE_ERROR:     p_ret = "ADPM_ROUTE_ERROR";     break;
    case R_G3_CMDID_ADPM_EAP_KEY:         p_ret = "ADPM_EAP_KEY";         break;
    case R_G3_CMDID_ADPM_FRAME_COUNTER:   p_ret = "ADPM_FRAME_COUNTER";   break;
    case R_G3_CMDID_ADPM_ROUTE_UPDATE:    p_ret = "ADPM_ROUTE_UPDATE";    break;
    case R_G3_CMDID_ADPM_LOAD_SEQ_NUM:    p_ret = "ADPM_LOAD_SEQ_NUM";    break;
    case R_G3_CMDID_ADPM_RREP:            p_ret = "ADPM_RREP";            break;
    case R_G3_CMDID_ADPM_BEACON_NOTIFY:   p_ret = "ADPM_BEACON_NOTIFY";   break;
    default:
        p_ret = "Unknown Type";
        break;
    }

    return p_ret;
}
/******************************************************************************
   End of function  g3_cmdid2text_adp
******************************************************************************/

/******************************************************************************
* Function Name : g3_cmdid2text_eap
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_cmdid2text_eap(uint8_t cmdId)
{
    const char *p_ret;

    switch( cmdId )
    {
    ENUM_CASE(R_G3_CMDID, EAPM_RESET )
    ENUM_CASE(R_G3_CMDID, EAPM_START )
    ENUM_CASE(R_G3_CMDID, EAPM_GET )
    ENUM_CASE(R_G3_CMDID, EAPM_SET )
    ENUM_CASE(R_G3_CMDID, EAPM_NETWORK )
    ENUM_CASE(R_G3_CMDID, EAPM_SETCLIENTINFO )
    ENUM_CASE(R_G3_CMDID, EAPM_NETWORK_JOIN )
    ENUM_CASE(R_G3_CMDID, EAPM_NETWORK_LEAVE )
    ENUM_CASE(R_G3_CMDID, EAPM_NEWDEVICE )
    ENUM_CASE(R_G3_CMDID, EAPM_EAP_KEY )
    default:
        p_ret = "Unknown Type";
        break;
    }

    return p_ret;
}
/******************************************************************************
   End of function  g3_cmdid2text_eap
******************************************************************************/
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
/******************************************************************************
* Function Name:g3_cmdid2text_rfmac
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static const char *g3_cmdid2text_rfmac(uint8_t cmdId)
{
    const char *p_ret;

    switch( cmdId )
    {
    ENUM_CASE(R_G3_CMDID, RF_MCPS_DATA )
    ENUM_CASE(R_G3_CMDID, RF_MLME_RESET )
    ENUM_CASE(R_G3_CMDID, RF_MLME_GET )
    ENUM_CASE(R_G3_CMDID, RF_MLME_SET )
    ENUM_CASE(R_G3_CMDID, RF_MLME_SCAN )
    ENUM_CASE(R_G3_CMDID, RF_MLME_START )
    ENUM_CASE(R_G3_CMDID, RF_MLME_BEACON_NOTIFY )
    ENUM_CASE(R_G3_CMDID, RF_MLME_COMM_STATUS )
    ENUM_CASE(R_G3_CMDID, RF_MLME_FRAMECOUNT )
    ENUM_CASE(R_G3_CMDID, RF_MLME_WARNING )
    default:
        p_ret = "Unknown Type";
        break;
    }

    return p_ret;
}
/******************************************************************************
   End of function  g3_cmdid2text_rfmac
******************************************************************************/

/******************************************************************************
* Function Name:g3_cmdid2text_rflmac
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static const char *g3_cmdid2text_rflmac(uint8_t cmdId)
{
    const char *p_ret;

    switch( cmdId )
    {
    ENUM_CASE(R_G3_CMDID, RF_LML_DATA )
    default:
        p_ret = "Unknown Type";
        break;
    }

    return p_ret;
}
/******************************************************************************
   End of function  g3_cmdid2text_rfmac
******************************************************************************/
#endif

/******************************************************************************
* Function Name : g3_cmd_type2text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_cmd_type2text(uint8_t type)
{
    const char *p_ret;

    switch( type )
    {
    ENUM_CASE(R_G3_CMDTYPE, REQ )
    ENUM_CASE(R_G3_CMDTYPE, CNF )
    ENUM_CASE(R_G3_CMDTYPE, IND )
    default:
            p_ret = "Unknown Type";
        break;
    }
    return p_ret;
}
/******************************************************************************
   End of function  g3_cmd_type2text
******************************************************************************/

/******************************************************************************
* Function Name : R_G3_Status2Text
* Description   :
* Arguments     :
* Return Value  :
******************************************************************************/
const char * R_G3_Status2Text(uint8_t unit_id, uint8_t status)
{
    const char *p_char;
    
    switch(unit_id)
    {
        case R_G3_UNITID_G3CTRL:
        {
            p_char = g3_status2text_ctrl (status);
        }
        break;
        case R_G3_UNITID_G3MAC:
        case R_G3_UNITID_G3RFMAC:
        {
            p_char = g3_status2text_mac (status);
        }
        break;
        case R_G3_UNITID_G3ADP:
        {
            p_char = g3_status2text_adp (status);
        }
        break;
        case R_G3_UNITID_G3EAP:
        {
            p_char = g3_status2text_eap (status);
        }
        break;
        default:
        {
            p_char = "UNDEFINED ID or below layer status";
        }
        break;
    }
    
    return p_char;
}

/******************************************************************************
* Function Name: g3_status2text_ctrl
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static const char *g3_status2text_ctrl (uint8_t status)
{
    const char * p_ret;
    switch (status)
    {
        STATUS_CASE (G3, SUCCESS)
        STATUS_CASE (G3, INVALID_REQUEST)
        STATUS_CASE (G3, INSUFFICIENT_MEMSIZE)
        STATUS_CASE (G3, NO_RESPONSE)
        STATUS_CASE (G3, IF_FATAL_ERROR)
        STATUS_CASE (G3, IF_TIMEOUT)
        STATUS_CASE (G3, IF_QUEUE_FULL)
        STATUS_CASE (G3, IF_INVALID_STATE)
        STATUS_CASE (G3, IF_INVALID_PARAMETER)
        STATUS_CASE (G3, IF_INVALID_RESPONSE)
        STATUS_CASE (G3, IF_NO_RESPONSE)
        STATUS_CASE (G3, INVALID_PARAMETER)
        STATUS_CASE (G3, INVALID_STATE)
        default:
        {
            p_ret = "MAC UNDEFINED STATUS";
        }
        break;
    }
    
    return p_ret;
}
/******************************************************************************
   End of function  g3_status2text_ctrl
******************************************************************************/

/******************************************************************************
* Function Name : g3_status2text_mac
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_status2text_mac (uint8_t status)
{
    const char * p_ret;
    switch (status)
    {
        STATUS_CASE (G3MAC, SUCCESS)
        STATUS_CASE (G3MAC, TX_TIMEOUT)
        STATUS_CASE (G3MAC, COUNTER_ERROR)
        STATUS_CASE (G3MAC, IMPROPER_KEY_TYPE)
        STATUS_CASE (G3MAC, IMPROPER_SECURITY_LEVEL)
        STATUS_CASE (G3MAC, UNSUPPORTED_LEGACY)
        STATUS_CASE (G3MAC, UNSUPPORTED_SECURITY)
        STATUS_CASE (G3MAC, CHANNEL_ACCESS_FAILURE)
        STATUS_CASE (G3MAC, SECURITY_ERROR)
        STATUS_CASE (G3MAC, FRAME_TOO_LONG)
        STATUS_CASE (G3MAC, INVALID_HANDLE)
        STATUS_CASE (G3MAC, INVALID_PARAMETER)
        STATUS_CASE (G3MAC, NO_ACK)
        STATUS_CASE (G3MAC, NO_BEACON)
        STATUS_CASE (G3MAC, NO_DATA)
        STATUS_CASE (G3MAC, NO_SHORT_ADDRESS)
        STATUS_CASE (G3MAC, OUT_OF_CAP)
        STATUS_CASE (G3MAC, ALTERNATE_PANID_DETECTION)
        STATUS_CASE (G3MAC, UNAVAILABLE_KEY)
        STATUS_CASE (G3MAC, UNSUPPORTED_ATTRIBUTE)
        STATUS_CASE (G3MAC, INVALID_ADDRESS)
        STATUS_CASE (G3MAC, INVALID_INDEX)
        STATUS_CASE (G3MAC, LIMIT_REACHED)
        STATUS_CASE (G3MAC, READ_ONLY)
        STATUS_CASE (G3MAC, SCAN_IN_PROGRESS)
        STATUS_CASE (G3MAC, INVALID_STATE)
        STATUS_CASE (G3MAC, TXRX_DISABLED)
        STATUS_CASE (G3MAC, NO_RESPONSE)
        STATUS_CASE (G3RFMAC, DUTY_CYCLE_REACHED)
        STATUS_CASE (G3RFMAC, TX_ACTIVE)
        default:
        {
            p_ret = "MAC UNDEFINED STATUS";
        }
        break;
    }
    
    return p_ret;
} /* g3_status2text_mac */
/******************************************************************************
   End of function  g3_status2text_mac
******************************************************************************/

/******************************************************************************
* Function Name : g3_status2text_adp
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_status2text_adp (uint32_t status)
{
    const char * p_ret;
    
    switch (status)
    {
        STATUS_CASE (ADP, SUCCESS)
        STATUS_CASE (ADP, COUNTER_ERROR)
        STATUS_CASE (ADP, IMPROPER_KEY_TYPE)
        STATUS_CASE (ADP, IMPROPER_SECURITY_LEVEL)
        STATUS_CASE (ADP, UNSUPPORTED_LEGACY)
        STATUS_CASE (ADP, UNSUPPORTED_SECURITY)
        STATUS_CASE (ADP, CHANNEL_ACCESS_FAILURE)
        STATUS_CASE (ADP, SECURITY_ERROR)
        STATUS_CASE (ADP, FRAME_TOO_LONG)
        STATUS_CASE (ADP, INVALID_HANDLE)
        STATUS_CASE (ADP, INVALID_PARAMETER)
        STATUS_CASE (ADP, NO_ACK)
        STATUS_CASE (ADP, NO_BEACON)
        STATUS_CASE (ADP, NO_DATA)
        STATUS_CASE (ADP, NO_SHORT_ADDRESS)
        STATUS_CASE (ADP, OUT_OF_CAP)
        STATUS_CASE (ADP, ALTERNATE_PANID_DETECTION)
        STATUS_CASE (ADP, UNAVAILABLE_KEY)
        STATUS_CASE (ADP, UNSUPPORTED_ATTRIBUTE)
        STATUS_CASE (ADP, INVALID_ADDRESS)
        STATUS_CASE (ADP, INVALID_INDEX)
        STATUS_CASE (ADP, LIMIT_REACHED)
        STATUS_CASE (ADP, READ_ONLY)
        STATUS_CASE (ADP, SCAN_IN_PROGRESS)
        STATUS_CASE (ADP, MAC_INVALID_STATE)
        STATUS_CASE (ADP, MAC_NO_RESPONSE)
        STATUS_CASE (ADP, INVALID_REQUEST)
        STATUS_CASE (ADP, NOT_PERMITTED)
        STATUS_CASE (ADP, INVALID_IPV6_FRAME)
        STATUS_CASE (ADP, ROUTE_ERROR)
        STATUS_CASE (ADP, INCOMPLETE_PATH)
        STATUS_CASE (ADP, ALREADY_IN_PROGRESS)
        STATUS_CASE (ADP, FAILED)
        STATUS_CASE (ADP, TIMEOUT)
        STATUS_CASE (ADP, REQ_QUEUE_FULL)
        STATUS_CASE (ADP, INSUFFICIENT_MEMSIZE)
        STATUS_CASE (ADP, DUTY_CYCLE_REACHED)
        STATUS_CASE (ADP, TX_ACTIVE)
        default:
            p_ret = "FATAL UNDEFINED ERROR";
            break;
    }
    
    return p_ret;
}
/******************************************************************************
   End of function  g3_status2text_adp
******************************************************************************/

/******************************************************************************
* Function Name : g3_status2text_eap
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_status2text_eap (uint32_t status)
{
    const char * p_ret;
    
    switch (status)
    {
        STATUS_CASE (EAP, SUCCESS)
        STATUS_CASE (EAP, INVALID_REQUEST)
        STATUS_CASE (EAP, CONFIG_ERROR)
        STATUS_CASE (EAP, TIMEOUT)
        STATUS_CASE (EAP, REQ_QUEUE_FULL)
        STATUS_CASE (EAP, UNSUPPORTED_ATTRIBUTE)
        STATUS_CASE (EAP, INVALID_INDEX)
        STATUS_CASE (EAP, READ_ONLY)
        STATUS_CASE (EAP, EAP_PSK_IN_PROGRESS)
        STATUS_CASE (EAP, BLACKLISTED_DEVICE)
        STATUS_CASE (EAP, EAP_PSK_FAILURE)
        STATUS_CASE (EAP, JOIN_DISCARD)
        STATUS_CASE (EAP, INSUFFICIENT_MEMSIZE)
        default:
            p_ret = "FATAL UNDEFINED ERROR";
            break;
    }
    
    return p_ret;
}
/******************************************************************************
   End of function  g3_status2text_eap
******************************************************************************/

/******************************************************************************
* Function Name : R_G3_Ibid2Text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
const char * R_G3_Ibid2Text(uint8_t layer, uint16_t id, uint8_t * len)
{
    const char * p_txt = NULL;
    uint32_t     ret  = 1;
    
    switch(layer)
    {
        case R_G3_LAYER_UMAC:
        {
            ret = g3_ibid2text_mac (id, &p_txt, len);
        }
        break;
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
        case R_G3_LAYER_RFMAC:
        {
            ret = g3_ibid2text_rfmac (id, &p_txt, len);
        }
        break;
#endif
        case R_G3_LAYER_ADP:
        {
            ret = g3_ibid2text_adp (id, &p_txt, len);
        }
        break;
        case R_G3_LAYER_EAP:
        {
            ret = g3_ibid2text_eap (id, &p_txt, len);
        }
        break;
        default:
        {
            /* do nothing */
        }
        break;
    }
    
    if (ret)
    {
        *len = 0xFF;
        p_txt = "Unknown Symbol";
    }
    return p_txt;
}
/******************************************************************************
   End of function  R_G3_Ibid2Text
******************************************************************************/

/******************************************************************************
* Function Name : g3_ibid2text_mac
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static uint32_t g3_ibid2text_mac (uint16_t id, const char ** txt, uint8_t * len)
{
    uint32_t ret = 0;
    switch (id)
    {
        IBID_GEN (G3MAC_IB_ACKWAIT_DURATION                  , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_MAX_BE                            , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_BSN                               , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_DSN                               , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_MAX_CSMABACKOFFS                  , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_MIN_BE                            , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_PANID                             , IB_LENGTH_16)
        IBID_GEN (G3MAC_IB_PROMISCUOUS_MODE                  , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_SHORTADDRESS                      , IB_LENGTH_16)
        IBID_GEN (G3MAC_IB_MAX_FRAME_RETRIES                 , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_TIMESTAMP_SUPPORTED               , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_SECURITY_ENABLED                  , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_KEY_TABLE                         , IB_LENGTH_TABLE)
        IBID_GEN (G3MAC_IB_DEVICE_TABLE                      , IB_LENGTH_TABLE)
        IBID_GEN (G3MAC_IB_FRAME_COUNTER                     , IB_LENGTH_32)
        IBID_GEN (G3MAC_IB_DUPLICATE_DETECTION_TTL           , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_HIGHPRIORITY_WINDOWSIZE           , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_TXDATAPACKET_COUNT                , IB_LENGTH_32)
        IBID_GEN (G3MAC_IB_RXDATAPACKET_COUNT                , IB_LENGTH_32)
        IBID_GEN (G3MAC_IB_TXCMDPACKET_COUNT                 , IB_LENGTH_32)
        IBID_GEN (G3MAC_IB_RXCMDPACKET_COUNT                 , IB_LENGTH_32)
        IBID_GEN (G3MAC_IB_CSMAFAIL_COUNT                    , IB_LENGTH_32)
        IBID_GEN (G3MAC_IB_CSMANOACK_COUNT                   , IB_LENGTH_32)
        IBID_GEN (G3MAC_IB_RXDATABROADCAST_COUNT             , IB_LENGTH_32)
        IBID_GEN (G3MAC_IB_TXDATABROADCAST_COUNT             , IB_LENGTH_32)
        IBID_GEN (G3MAC_IB_BADCRC_COUNT                      , IB_LENGTH_32)
        IBID_GEN (G3MAC_IB_NEIGHBOUR_TABLE                   , IB_LENGTH_TABLE)
        IBID_GEN (G3MAC_IB_CSMA_FAIRNESS_LIMIT               , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_TMR_TTL                           , IB_LENGTH_32)
        IBID_GEN (G3MAC_IB_POS_TABLE_ENTRY_TTL               , IB_LENGTH_32)
        IBID_GEN (G3MAC_IB_RCCOORD                           , IB_LENGTH_16)
        IBID_GEN (G3MAC_IB_TONEMASK                          , IB_LENGTH_TABLE)
        IBID_GEN (G3MAC_IB_BEACON_RAND_WIN_LENGTH            , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_A                                 , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_K                                 , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_MINCWATTEMPTS                     , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_CENELEC_LEGACY_MODE               , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_FCC_LEGACY_MODE                   , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_BROADCAST_MAX_CW_EANBLE           , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_TRANSMIT_ATTEN                    , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_POS_TABLE                         , IB_LENGTH_TABLE)
        IBID_GEN (G3MAC_IB_PLC_DISABLE                       , IB_LENGTH_8)

        IBID_GEN (G3MAC_IB_COHERENT_TRANSMISSION             , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_NEIGHBOUR_TABLE_SIZE              , IB_LENGTH_16)
        IBID_GEN (G3MAC_IB_DEVICE_TABLE_SIZE                 , IB_LENGTH_16)
        IBID_GEN (G3MAC_IB_VALID_NEITABLE_ENTRIES            , IB_LENGTH_16)
        IBID_GEN (G3MAC_IB_VALID_DEVTABLE_ENTRIES            , IB_LENGTH_16)
        IBID_GEN (G3MAC_IB_NEIGHBOUR_TABLE_BY_SHORT_ADDR     , IB_LENGTH_TABLE)
        IBID_GEN (G3MAC_IB_KEY_VALIDATE                      , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_EXTADDRESS                        , IB_LENGTH_TABLE)
        IBID_GEN (G3MAC_IB_SOFT_VERSION                      , IB_LENGTH_TABLE)
        IBID_GEN (G3MAC_IB_UNICAST_DATA_TX_TIMEOUT           , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_BROADCAST_DATA_TX_TIMEOUT         , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_BEACON_REQUEST_TX_TIMEOUT         , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_TX_GAIN                           , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_FRAME_CNT_IND_INTERVAL            , IB_LENGTH_32)
        IBID_GEN (G3MAC_IB_DEVICE_TABLE_BY_SHORT_ADDR        , IB_LENGTH_TABLE)
        IBID_GEN (G3MAC_IB_NEIGHBOUR_INDEX_BY_SHORT_ADDR     , IB_LENGTH_16)
        IBID_GEN (G3MAC_IB_DEVICE_INDEX_BY_SHORT_ADDR        , IB_LENGTH_16)
        IBID_GEN (G3MAC_IB_VALID_POSTABLE_ENTRIES            , IB_LENGTH_16)
        IBID_GEN (G3MAC_IB_POS_TABLE_BY_SHORT_ADDR           , IB_LENGTH_16)
        IBID_GEN (G3MAC_IB_TMR_IND_ENABLE                    , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_KEEP_MOD_RETRY_NUM                , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_NEI_UPDATE_AFTER_RETRANSMIT       , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_COMM_STATUS_IND_MASK              , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_TMR_REQ_LEAD_TIME_BEF_TTL         , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_MIN_NEITABLE_ENTRIES              , IB_LENGTH_16)

        IBID_GEN (G3MAC_IB_OFFSET_SNR                        , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_THRESH_CARRIER_NUM                , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_THRESH_TONENUM_PER_MAP            , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_THRESH_SNR_DIFFERENTIAL           , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_THRESH_SNR_COHERENT               , IB_LENGTH_8)

        IBID_GEN (G3MAC_IB_DISABLE_BEACON                    , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_DISABLE_TMRES                     , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_REF_RSSI_DB                       , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_DISABLE_TXGAIN_TMR                , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_TOTAL_NEITABLE_ENTRIES            , IB_LENGTH_16)
        IBID_GEN (G3MAC_IB_TOTAL_POSTABLE_ENTRIES            , IB_LENGTH_16)

        IBID_GEN (G3MAC_IB_ENABLE_DEBUG                      , IB_LENGTH_8)

        IBID_GEN (G3MAC_IB_POS_RECENT_ENTRY_TH               , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_POS_RECENT_ENTRY                  , IB_LENGTH_16)
        IBID_GEN (G3MAC_IB_TRICKLE_MIN_LQI_VALUE             , IB_LENGTH_8)

        IBID_GEN (G3MAC_IB_STATISTICS                        , IB_LENGTH_TABLE)

        IBID_GEN (G3MAC_IB_PHY_TX_POWER                      , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_PHY_TX_FILTER_SCALE               , IB_LENGTH_16)
        IBID_GEN (G3MAC_IB_PHY_TX_DIGITAL_PREAMBLE_GAIN      , IB_LENGTH_16)
        IBID_GEN (G3MAC_IB_PHY_TX_DIGITAL_GAIN               , IB_LENGTH_16)
        IBID_GEN (G3MAC_IB_PHY_TXENB_POLARITY                , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_PHY_TX_WAIT_TIME                  , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_PHY_TX_BREAK                      , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_PHY_TX_ACK_GAIN                   , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_PHY_SATT_CTRL_DISABLE             , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_PHY_AC_PHASE_OFFSET               , IB_LENGTH_16)
        IBID_GEN (G3MAC_IB_PHY_STATISTICS                    , IB_LENGTH_TABLE)
        IBID_GEN (G3MAC_IB_PHY_TXRX_DISABLE                  , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_PHY_ZCD_IND_POLARITY              , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_PHY_TPGA_CAJ                      , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_PHY_SATT_POLARITY                 , IB_LENGTH_8)

        IBID_GEN (G3MAC_IB_PHY_PREAMBLE_NUM                  , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_PHY_RX_GAIN_CUR                   , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_LML_LQI_ADJ_BND_HI                , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_LML_LQI_ADJ_BND_LO                , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_LML_LQI_ADJ_SLOPE_HI              , IB_LENGTH_8)
        IBID_GEN (G3MAC_IB_LML_LQI_ADJ_SLOPE_LO              , IB_LENGTH_8)

        default:
        {
            ret = 1;
        }
        break;
    }
    
    return ret;
}
/******************************************************************************
   End of function  g3_ibid2text_mac
******************************************************************************/

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
/******************************************************************************
* Function Name : g3_ibid2text_rfmac
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static uint32_t g3_ibid2text_rfmac (uint16_t id, const char ** txt, uint8_t * len)
{
    uint32_t ret = 0;
    switch (id)
    {
        IBID_GEN (G3RFMAC_IB_PHY_TRANSMIT_POWER              , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_PHY_RSSI_OUTPUT_OFFSET          , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_PHY_CCA_BANDWIDTH               , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_PHY_CCA_DURATION                , IB_LENGTH_16)
        IBID_GEN (G3RFMAC_IB_PHY_FSK_PREAMBLE_LENGTH         , IB_LENGTH_16)
        IBID_GEN (G3RFMAC_IB_PHY_FSK_FEC_RX_ENA              , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_PHY_FSK_FEC_TX_ENA              , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_PHY_ANTENNA_SWITCH_ENA          , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_PHY_ANTENNA_SELECT_TX           , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_PHY_REGULATORY_MODE             , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_PHY_AGC_WAIT_GAIN_OFFSET        , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_PHY_CCA_VTH                     , IB_LENGTH_16)
        IBID_GEN (G3RFMAC_IB_PHY_CCA_VTH_OFFSET              , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_PHY_ANTENNA_SWITCH_ENA_TIMING   , IB_LENGTH_16)
        IBID_GEN (G3RFMAC_IB_PHY_GPIO0_SETTING               , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_PHY_DATA_RATE                   , IB_LENGTH_16)
        IBID_GEN (G3RFMAC_IB_PHY_RMODET_ON_MAX               , IB_LENGTH_16)
        IBID_GEN (G3RFMAC_IB_PHY_RMODET_OFF_MIN              , IB_LENGTH_16)

        IBID_GEN (G3RFMAC_IB_PANID                           , IB_LENGTH_16)
        IBID_GEN (G3RFMAC_IB_PROMISCUOUS_MODE                , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_RX_ON_WHEN_IDLE                 , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_SHORTADDRESS                    , IB_LENGTH_16)
        IBID_GEN (G3RFMAC_IB_SECURITY_ENABLED                , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_FRAMECNT_IND_INTERVAL           , IB_LENGTH_32)
        IBID_GEN (G3RFMAC_IB_KEY_TABLE                       , IB_LENGTH_TABLE)
        IBID_GEN (G3RFMAC_IB_METRICSENABLED                  , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_RXFRAMECOUNTER_FLUSH            , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_RF_FRONTEND_GAIN                , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_POS_TABLE_ENTRY_TTL             , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_RCCOORD                         , IB_LENGTH_16)
        IBID_GEN (G3RFMAC_IB_BEACON_RAND_WIN_LENGTH          , IB_LENGTH_8)

        IBID_GEN (G3RFMAC_IB_DSN_RF                          , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_MAXBE_RF                        , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_MAXCSMABACKOFFS_RF              , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_MAXFRAMERETRIES_RF              , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_MINBE_RF                        , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_TIMESTAMPSUPPORTED_RF           , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_DEVICETABLE_RF                  , IB_LENGTH_TABLE)
        IBID_GEN (G3RFMAC_IB_FRAMECOUNTER_RF                 , IB_LENGTH_32)
        IBID_GEN (G3RFMAC_IB_DUPLICATEDETECTIONTTL_RF        , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_COUNTEROCTETS_RF                , IB_LENGTH_32)
        IBID_GEN (G3RFMAC_IB_RETRYCOUNT_RF                   , IB_LENGTH_32)
        IBID_GEN (G3RFMAC_IB_MULTIPLERETRYCOUNT_RF           , IB_LENGTH_32)
        IBID_GEN (G3RFMAC_IB_TXFAILCOUNT_RF                  , IB_LENGTH_32)
        IBID_GEN (G3RFMAC_IB_TXSUCCESSCOUNT_RF               , IB_LENGTH_32)
        IBID_GEN (G3RFMAC_IB_FCSERRORCOUNT_RF                , IB_LENGTH_32)
        IBID_GEN (G3RFMAC_IB_SECURITYFAILURE_RF              , IB_LENGTH_32)
        IBID_GEN (G3RFMAC_IB_DUPLICATEFRAMECOUNT_RF          , IB_LENGTH_32)
        IBID_GEN (G3RFMAC_IB_RXSUCCESSCOUNT_RF               , IB_LENGTH_32)
        IBID_GEN (G3RFMAC_IB_EBRFILTER_RF                    , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_BEACONAUTORESPOND_RF            , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_USEENHANCEDBEACON_RF            , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_EBSN_RF                         , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_POSTABLE_RF                     , IB_LENGTH_TABLE)

        IBID_GEN (G3RFMAC_IB_OPEMODE_RF                      , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_CHANNEL_NUMBER_RF               , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_DUTY_CYCLE_USAGE_RF             , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_DUTY_CYCLE_PERIOD_RF            , IB_LENGTH_16)
        IBID_GEN (G3RFMAC_IB_DUTY_CYCLE_LIMIT_RF             , IB_LENGTH_16)
        IBID_GEN (G3RFMAC_IB_DUTY_CYCLE_THRESHOLD_RF         , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_DISABLE_PHY_RF                  , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_FREQBAND_RF                     , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_TRANSMITATTEN_RF                , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_ADAPTIVEPOWERSTEP               , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_ADAPTIVEPOWERHIGHBOUND          , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_ADAPTIVEPOWERLOWBOUND           , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_BEACON_RAND_WIN_LENGTH_RF       , IB_LENGTH_16)
        IBID_GEN (G3RFMAC_IB_MAC_MAX_CCA_ATTEMPTS_RETRIES    , IB_LENGTH_8)
        IBID_GEN (G3RFMAC_IB_MAC_INITIAL_RETRY_TIME          , IB_LENGTH_16)
        IBID_GEN (G3RFMAC_IB_MAC_MAXIMUM_RETRY_TIME          , IB_LENGTH_16)

        IBID_GEN (G3RFMAC_IB_KEY_STATE                       , IB_LENGTH_TABLE)
        IBID_GEN (G3RFMAC_IB_EXTADDRESS                      , IB_LENGTH_TABLE)
        IBID_GEN (G3RFMAC_IB_SOFT_VERSION                    , IB_LENGTH_TABLE)
        IBID_GEN (G3RFMAC_IB_DEVICE_TABLE_BY_SHORT_ADDR      , IB_LENGTH_TABLE)
        IBID_GEN (G3RFMAC_IB_POS_TABLE_BY_SHORT_ADDR         , IB_LENGTH_TABLE)
        IBID_GEN (G3RFMAC_IB_CONTINUOUS_TX_ENABLED_RF        , IB_LENGTH_8)
		IBID_GEN (G3RFMAC_IB_MAC_G3CERTCOMPATIBILITY_VERSION , IB_LENGTH_8)

        default:
        {
            ret = 1;
        }
        break;
    }

    return ret;
} /* g3_ibid2text_rfmac */
/******************************************************************************
   End of function  g3_ibid2text_rfmac
******************************************************************************/
#endif

/******************************************************************************
* Function Name: g3_ibid2text_adp
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint32_t g3_ibid2text_adp (uint16_t id, const char ** txt, uint8_t * len)
{
    uint32_t ret = 0;
    switch (id)
    {
        IBID_GEN (ADP_IB_SECURITY_LEVEL, IB_LENGTH_8)
        IBID_GEN (ADP_IB_PREFIX_TABLE, IB_LENGTH_TABLE)
        IBID_GEN (ADP_IB_BROADCAST_LOG_TABLE_ENTRY_TTL, IB_LENGTH_16)
        IBID_GEN (ADP_IB_METRIC_TYPE, IB_LENGTH_8)
        IBID_GEN (ADP_IB_LOW_LQI_VALUE, IB_LENGTH_8)
        IBID_GEN (ADP_IB_HIGH_LQI_VALUE, IB_LENGTH_8)
        IBID_GEN (ADP_IB_RREP_WAIT, IB_LENGTH_8)
        IBID_GEN (ADP_IB_CONTEXT_INFORMATION_TABLE, IB_LENGTH_TABLE)
        IBID_GEN (ADP_IB_COORD_SHORT_ADDRESS, IB_LENGTH_16)
        IBID_GEN (ADP_IB_RLC_ENABLED, IB_LENGTH_8)
        IBID_GEN (ADP_IB_ADD_REV_LINK_COST, IB_LENGTH_8)
        IBID_GEN (ADP_IB_BROADCAST_LOG_TABLE, IB_LENGTH_TABLE)
        IBID_GEN (ADP_IB_ROUTING_TABLE, IB_LENGTH_TABLE)
        IBID_GEN (ADP_IB_UNICAST_RREQ_GEN_ENABLE, IB_LENGTH_8)
        IBID_GEN (ADP_IB_GROUP_TABLE, IB_LENGTH_TABLE)
        IBID_GEN (ADP_IB_MAX_HOPS, IB_LENGTH_8)
        IBID_GEN (ADP_IB_DEVICE_TYPE, IB_LENGTH_8)
        IBID_GEN (ADP_IB_NET_TRAVERSAL_TIME, IB_LENGTH_16)
        IBID_GEN (ADP_IB_ROUTING_TABLE_ENTRY_TTL, IB_LENGTH_16)
        IBID_GEN (ADP_IB_KR, IB_LENGTH_8)
        IBID_GEN (ADP_IB_KM, IB_LENGTH_8)
        IBID_GEN (ADP_IB_KC, IB_LENGTH_8)
        IBID_GEN (ADP_IB_KQ, IB_LENGTH_8)
        IBID_GEN (ADP_IB_KH, IB_LENGTH_8)
        IBID_GEN (ADP_IB_RREQ_RETRIES, IB_LENGTH_8)
        IBID_GEN (ADP_IB_RREQ_WAIT, IB_LENGTH_8)
        IBID_GEN (ADP_IB_WEAK_LQI_VALUE, IB_LENGTH_8)
        IBID_GEN (ADP_IB_KRT, IB_LENGTH_8)
        IBID_GEN (ADP_IB_SOFT_VERSION, IB_LENGTH_TABLE)
        IBID_GEN (ADP_IB_BLACKLIST_TABLE, IB_LENGTH_TABLE)
        IBID_GEN (ADP_IB_BLACKLIST_TABLE_ENTRY_TTL, IB_LENGTH_16)
        IBID_GEN (ADP_IB_MAX_JOIN_WAIT_TIME, IB_LENGTH_16)
        IBID_GEN (ADP_IB_PATH_DISCOVERY_TIME, IB_LENGTH_16)
        IBID_GEN (ADP_IB_ACTIVE_KEY_INDEX, IB_LENGTH_8)
        IBID_GEN (ADP_IB_DESTINATION_ADDRESS_SET, IB_LENGTH_TABLE)
        IBID_GEN (ADP_IB_DEFAULT_COORD_ROUTE_ENABLED, IB_LENGTH_8)

        IBID_GEN (ADP_IB_DELAY_LOW_LQI, IB_LENGTH_16)
        IBID_GEN (ADP_IB_DELAY_HIGH_LQI, IB_LENGTH_16)
        IBID_GEN (ADP_IB_RREQ_JITTER_LOW_LQI, IB_LENGTH_8)
        IBID_GEN (ADP_IB_RREQ_JITTER_HIGH_LQI, IB_LENGTH_8)

        IBID_GEN (ADP_IB_TRICKLE_DATA_ENABLED, IB_LENGTH_8)
        IBID_GEN (ADP_IB_TRICKLE_STEP, IB_LENGTH_16)
        IBID_GEN (ADP_IB_TRICKLE_FACTOR_IMAX, IB_LENGTH_8)
        IBID_GEN (ADP_IB_TRICKLE_IMIN, IB_LENGTH_16)
        IBID_GEN (ADP_IB_TRICKLE_MAX_KI, IB_LENGTH_8)
        IBID_GEN (ADP_IB_TRICKLE_ADAPTIVE_IMIN, IB_LENGTH_8)

        IBID_GEN (ADP_IB_CLUSTER_TRICKLE_ENABLED, IB_LENGTH_8)
        IBID_GEN (ADP_IB_CLUSTER_MIN_LQI, IB_LENGTH_8)
        IBID_GEN (ADP_IB_CLUSTER_TRICKLE_K, IB_LENGTH_8)
        IBID_GEN (ADP_IB_CLUSTER_RREQ_RC_DEVIATION, IB_LENGTH_8)
        IBID_GEN (ADP_IB_CLUSTER_TRICKLE_I, IB_LENGTH_8)

        /* RENESAS ORIGINAL */
        IBID_GEN (ADP_IB_STATISTICS, IB_LENGTH_TABLE)
        IBID_GEN (ADP_IB_LOAD_SEQ_NUMBER, IB_LENGTH_16)
        IBID_GEN (ADP_IB_ROUTE_TABLE_BY_ADDR, IB_LENGTH_TABLE)
        IBID_GEN (ADP_IB_ROUTE_TABLE_SIZE, IB_LENGTH_16)
        IBID_GEN (ADP_IB_VALID_RTABLE_ENTRIES, IB_LENGTH_16)
        IBID_GEN (ADP_IB_ROUTE_INDEX_BY_ADDR, IB_LENGTH_16)
        IBID_GEN (ADP_IB_ROUTE_IND_ENABLE, IB_LENGTH_8)
        IBID_GEN (ADP_IB_LOAD_SEQ_NUM_IND_INTERVAL, IB_LENGTH_16)
        IBID_GEN (ADP_IB_BEACON_IND_ENABLE, IB_LENGTH_8)
        IBID_GEN (ADP_IB_BUFF_IND_DISABLE, IB_LENGTH_8)
        IBID_GEN (ADP_IB_CLR_ALL_ROUTE, IB_LENGTH_8)
        IBID_GEN (ADP_IB_RREP_IND_ENABLE, IB_LENGTH_8)
        IBID_GEN (ADP_IB_DATATYPE, IB_LENGTH_8)
        IBID_GEN (ADP_IB_ENABLE_DATATRANS, IB_LENGTH_8)
        IBID_GEN (ADP_IB_DISABLE_RELAY, IB_LENGTH_8)

        IBID_GEN (ADP_IB_ENABLE_DEBUG, IB_LENGTH_8)

        IBID_GEN (ADP_IB_TRICKLE_K, IB_LENGTH_8)
        IBID_GEN (ADP_IB_TRICKLE_IMAX, IB_LENGTH_16)

        IBID_GEN (ADP_IB_KEY_IND_ENABLE, IB_LENGTH_8)
        IBID_GEN (ADP_IB_IDS_IND_ENABLE, IB_LENGTH_8)

        IBID_GEN (ADP_IB_LOW_LQI_VALUE_RF , IB_LENGTH_8)
        IBID_GEN (ADP_IB_HIGH_LQI_VALUE_RF, IB_LENGTH_8)
        IBID_GEN (ADP_IB_KQ_RF            , IB_LENGTH_8)
        IBID_GEN (ADP_IB_KH_RF            , IB_LENGTH_8)
        IBID_GEN (ADP_IB_KRT_RF           , IB_LENGTH_8)
        IBID_GEN (ADP_IB_KDC_RF           , IB_LENGTH_8)
        IBID_GEN (ADP_IB_USE_BACKUPMEDIA  , IB_LENGTH_8)
        IBID_GEN (ADP_IB_WEAK_LQI_VALUE_RF, IB_LENGTH_8)
        IBID_GEN (ADP_IB_DISABLE_DEFAULT_ROUTING, IB_LENGTH_8)

        default:
        {
            ret = 1;
        }
        break;
    }
    
    return ret;
}
/******************************************************************************
   End of function  g3_ibid2text_adp
******************************************************************************/

/******************************************************************************
* Function Name : g3_ibid2text_eap
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static uint32_t g3_ibid2text_eap (uint16_t id, const char ** txt, uint8_t * len)
{
    uint32_t ret = 0;
    switch (id)
    {
        IBID_GEN (EAP_IB_GMK,                   16)
        IBID_GEN (EAP_IB_ACTIVEKEYINDEX,        IB_LENGTH_8)
        IBID_GEN (EAP_IB_COORDSHORTADDRESS,     IB_LENGTH_16)
        IBID_GEN (EAP_IB_CINFOTABLEENTRIES,     IB_LENGTH_16)
        IBID_GEN (EAP_IB_CLIENTINFOTABLE,       IB_LENGTH_TABLE)
        IBID_GEN (EAP_IB_JOIN_WAITTIMESEC,      IB_LENGTH_16)
        IBID_GEN (EAP_IB_JOINGMKID,             IB_LENGTH_8)
        IBID_GEN (EAP_IB_DISABLE_JOIN,          IB_LENGTH_8)
        IBID_GEN (EAP_IB_EAPPSK_RETRYNUM,       IB_LENGTH_8)
        IBID_GEN (EAP_IB_NWK_WAITTIMESEC,       IB_LENGTH_16)
        IBID_GEN (EAP_IB_ONETIME_CLIENTINFO,    IB_LENGTH_8)
        IBID_GEN (EAP_IB_KEY_IND_ENABLE,        IB_LENGTH_8)
        IBID_GEN (EAP_IB_IDP_IND_ENABLE,        IB_LENGTH_8)
        IBID_GEN (EAP_IB_SOFTVERSION,           IB_LENGTH_16)
        IBID_GEN (EAP_IB_STATISTICS,            IB_LENGTH_TABLE)
        default:
        {
            ret = 1;
        }
        break;
    }
    
    return ret;
}
/******************************************************************************
   End of function  g3_ibid2text_eap
******************************************************************************/

 /******************************************************************************
* Function Name : R_G3_Def2text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
const char *R_G3_Def2text(e_g3_def_type_t type, uint16_t id)
{
    const char *p_ret;
    
    switch(type)
    {
        case R_G3_DEF_TYPE_UNIT:
        {
            p_ret = g3_unitid2text((uint8_t)id);
        }
        break;
        case R_G3_DEF_TYPE_CMD_ID:
        {
            p_ret = g3_cmdid2text((uint8_t)(id<<8), (uint8_t)(id&0xFF));
        }
        break;
        case R_G3_DEF_TYPE_CMD_TYPE:
        {
            p_ret = g3_cmd_type2text((uint8_t)id);
        }
        break;
        case R_G3_DEF_TYPE_LAYER:
        {
            p_ret = g3_layer2text((uint8_t)id);
        }
        break;
        case R_G3_DEF_TYPE_INFO_TYPE:
        {
            p_ret = g3_type_info2text((uint8_t)id);
        }
        break;
        case R_G3_DEF_TYPE_INFO_LAYER:
        {
            p_ret = g3_layer_info2text((uint8_t)id);
        }
        break;
        case R_G3_DEF_TYPE_MEDIA:
        {
            p_ret = g3_media_type2text((r_media_type_t)id);
        }
        break;
        case R_G3_DEF_TYPE_BAND:
        {
            p_ret = g3_bandplan2text((uint8_t)id);
        }
        break;
        case R_G3_DEF_TYPE_ADR_MODE:
        {
            p_ret = g3_addr_mode2text((uint8_t)id);
        }
        break;
        case R_G3_DEF_TYPE_KEY_STATE:
        {
            p_ret = g3_key_state2text((uint8_t)id);
        }
        break;
        case R_G3_DEF_TYPE_KEY_TYPE:
        {
            p_ret = g3_key_type2text((uint8_t)id);
        }
        break;
        case R_G3_DEF_TYPE_METRIC:
        {
            p_ret = g3_metric_type2text((uint8_t)id);
        }
        break;
        case R_G3_DEF_TYPE_MOD_TYPE:
        {
            p_ret = g3_mod_type2text((uint8_t)id);
        }
        break;
        case R_G3_DEF_TYPE_PMS:
        {
            p_ret = g3_mps2text((uint8_t)id);
        }
        break;
        default:
        {
            p_ret = NULL;
        }
        break;
    }
    
    return p_ret;
}
/******************************************************************************
   End of function  R_G3_Def2text
******************************************************************************/

/******************************************************************************
* Function Name : g3_layer2text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_layer2text(uint8_t layer)
{
    const char *ret;
    
    switch (layer) {
    case R_G3_LAYER_G3CTRL: ret = "G3CTRL"; break;
    case R_G3_LAYER_LMAC : ret = "LMAC"; break;
    case R_G3_LAYER_UMAC: ret = "UMAC"; break;
    case R_G3_LAYER_ADP: ret = "ADP"; break;
    case R_G3_LAYER_EAP: ret = "EAP"; break;
    case R_G3_LAYER_RFMAC: ret = "RFMAC"; break;
    case R_G3_LAYER_END: ret = "END"; break;
    default:
        ret = "Unknown";
        break;
    }
    return ret;
}
/******************************************************************************
   End of function  g3_layer2text
******************************************************************************/

/******************************************************************************
* Function Name : g3_type_info2text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_type_info2text(uint8_t layer)
{
    const char *p_ret;
    
    switch (layer) {
    ENUM_CASE(R_G3_INFO_TYPE, STATS)
    ENUM_CASE(R_G3_INFO_TYPE, LOG)
    default:
        p_ret = "Unknown Type";
        break;
    }
    return p_ret;
}
/******************************************************************************
   End of function  g3_layer_info2text
******************************************************************************/

/******************************************************************************
* Function Name : g3_layer_info2text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_layer_info2text(uint8_t layer)
{
    const char *p_ret;

    switch (layer) {
    ENUM_CASE(R_G3_INFO_LAYER, LMAC_DSP)
    ENUM_CASE(R_G3_INFO_LAYER, UMAC)
    ENUM_CASE(R_G3_INFO_LAYER, ADP)
    ENUM_CASE(R_G3_INFO_LAYER, EAP)
    ENUM_CASE(R_G3_INFO_LAYER, RF_MAC)
    default:
        p_ret = "Unknown Type";
        break;
    }
    return p_ret;
}
/******************************************************************************
   End of function  g3_layer_info2text
******************************************************************************/

/******************************************************************************
* Function Name : g3_media_type2text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_media_type2text(r_media_type_t type)
{
    const char *ret;

    switch (type) {
    case R_MEDIA_TYPE0_PLC:
        ret = "PLC";
        break;
    case R_MEDIA_TYPE1_RF:
        ret = "RF";
        break;
    case R_MEDIA_TYPE2_BOTH:
        ret = "BOTH";
        break;
    case R_MEDIA_TYPE3_PLC_ONLY:
        ret = "PLC_ONLY";
        break;
    case R_MEDIA_TYPE4_RF_ONLY:
        ret = "RF_ONLY";
        break;
    default:
        ret = "Unknown Type";
        break;
    }
    return ret;
}
/******************************************************************************
   End of function  R_LOG_SetGetType2Text
******************************************************************************/

/******************************************************************************
* Function Name : g3_bandplan2text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_bandplan2text(uint8_t mode)
{
    const char *ret;

    switch (mode)
    {
    case R_G3_BANDPLAN_CENELEC_A:
        ret = "CENELEC-A";
        break;
    case R_G3_BANDPLAN_CENELEC_B:
        ret = "CENELEC-B";
        break;
    case R_G3_BANDPLAN_ARIB:
        ret = "ARIB";
        break;
    case R_G3_BANDPLAN_FCC:
        ret = "FCC";
        break;
    default:
        ret = "Unknown Symbol";
        break;
    }
    return ret;
}
/******************************************************************************
   End of function  g3_bandplan2text
******************************************************************************/

/******************************************************************************
* Function Name : g3_addr_mode2text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_addr_mode2text(uint8_t mode)
{
    const char *ret;

    switch (mode)
    {
    case R_G3MAC_ADDR_MODE_NO_ADDRESS:
        ret = "NO_ADDRESS";
        break;
    case R_G3MAC_ADDR_MODE_SHORT:
        ret = "Short";
        break;
    case R_G3MAC_ADDR_MODE_EXTENDED:
        ret = "Extended";
        break;
    default:
        ret = "Unknown Symbol";
        break;
    }
    return ret;
}
/******************************************************************************
   End of function  g3_addr_mode2text
******************************************************************************/

/******************************************************************************
* Function Name : g3_key_state2text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_key_state2text(uint8_t state)
{
    const char *ret;

    switch (state)
    {
    case 0:
        ret = "RECEIVE";
        break;
    case 1:
        ret = "ACTIVE";
        break;
    case 2:
        ret = "REMIVE";
        break;
    default:
        ret = "Unknown Symbol";
        break;
    }
    return ret;
}
/******************************************************************************
   End of function  g3_key_state2text
******************************************************************************/

/******************************************************************************
* Function Name:g3_key_type2text
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static const char *g3_key_type2text(uint8_t type)
{
    const char *ret;

    switch (type)
    {
    case 0:
        ret = "PSK";
        break;
    case 1:
        ret = "AK";
        break;
    case 2:
        ret = "KDK";
        break;
    case 3:
        ret = "TEK";
        break;
    case 4:
        ret = "MSK";
        break;
    case 5:
        ret = "EMSK";
        break;
    case 0x40:
        ret = "ID_P";
        break;
    case 0x41:
        ret = "ID_S";
        break;
    case 0x80:
        ret = "SP1";
        break;
    default:
        ret = "Unknown Symbol";
        break;
    }
    return ret;
}
/******************************************************************************
   End of function  g3_key_type2text
******************************************************************************/

/******************************************************************************
* Function Name:g3_metric_type2text
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static const char *g3_metric_type2text(uint8_t type)
{
    const char *ret;

    switch (type)
    {
    case 0:
        ret = "HOP_COUNT";
        break;
    case 0x0E:
        ret = "LINKY";
        break;
    case 0x0F:
        ret = "ANNEX_B";
        break;
    default:
        ret = "Unknown Symbol";
        break;
    }
    return ret;
}
/******************************************************************************
   End of function  g3_metric_type2text
******************************************************************************/

/******************************************************************************
* Function Name : g3_mod_type2text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_mod_type2text(uint8_t modType)
{
    const char *ret;

    switch (modType)
    {
    case R_G3MAC_MOD_TYPE_ROBO:
        ret = "ROBO";
        break;
    case R_G3MAC_MOD_TYPE_BPSK:
        ret = "BPSK";
        break;
    case R_G3MAC_MOD_TYPE_QPSK:
        ret = "QPSK";
        break;
    case R_G3MAC_MOD_TYPE_8PSK:
        ret = "8PSK";
        break;
    case R_G3MAC_MOD_TYPE_16QAM:
        ret = "16QAM";
        break;
    default:
        ret = "Unknown Symbol";
        break;
    }
    return ret;
}
/******************************************************************************
   End of function  g3_mod_type2text
******************************************************************************/

/******************************************************************************
* Function Name : g3_mps2text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *g3_mps2text(uint8_t pms)
{
    const char *ret;
    
    switch (pms)
    {
    case R_G3MAC_MOD_SCHEME_DIFFERENTIAL:
        ret = "(D)";
        break;
    case R_G3MAC_MOD_SCHEME_COHERENT:
        ret = "(C)";
        break;
    default:
        ret = "Unknown Symbol";
        break;
    }
    return ret;
}
/******************************************************************************
   End of function  g3_mps2text
******************************************************************************/
