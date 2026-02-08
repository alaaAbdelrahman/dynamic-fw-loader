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
* File Name   : r_demo_ib_access.c
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
#include <stdlib.h>
#include <string.h>
#include "r_typedefs.h"
#include "r_stdio_api.h"
#include "r_byte_swap.h"

/* g3 part */
#include "r_c3sap_api.h"

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
#include "r_demo_parameters.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_DEMO_APP_ENTRIES_PER_CYCLE  (50)
/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
/*!
   \fn r_result_t get_ib_entry(const r_adp_ib_id_t attributeId);
   \brief  Gets element from ADP IB
 */
static r_result_t get_ib_entry (uint8_t chId, const r_adp_ib_id_t attributeId, uint8_t allEntries);

/*!
   \fn r_result_t set_ib_entry(const r_adp_ib_id_t attributeId);
   \brief  Sets element in ADP IB
 */
static r_result_t set_ib_entry (uint8_t chId, const r_adp_ib_id_t attributeId);

/*!
   \fn static r_result_t get_mac_pib_entry(const r_g3mac_ib_id_t attributeId);
   \brief  Gets element from MAC PIB
 */
static r_result_t get_mac_pib_entry (uint8_t chId, const r_g3mac_ib_id_t attributeId, uint8_t allEntries);

/*!
   \fn static r_result_t set_mac_pib_entry(const r_g3mac_ib_id_t attributeId);
   \brief  Sets element in MAC PIB
 */
static r_result_t set_mac_pib_entry (uint8_t chId, const r_g3mac_ib_id_t attributeId);

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
static r_result_t get_rfmac_pib_entry(uint8_t chId, const r_g3rfmac_ib_id_t attributeId);
static r_result_t set_rfmac_pib_entry(uint8_t chId, const r_g3rfmac_ib_id_t attributeId);
#endif

/******************************************************************************
Exported global variables
******************************************************************************/
extern r_demo_config_t             g_demo_config;
extern r_demo_buff_t               g_demo_buff;

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Functions
******************************************************************************/
/******************************************************************************
* Function Name: R_DEMO_MenuConfig
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_MenuConfig (void)
{
    /* Configuration Menu */
    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\f-------------------Configuration Menu-----------------------");
#endif
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
        if ((R_MEDIA_TYPE0_PLC == g_demo_config.mediaType) || (R_MEDIA_TYPE2_BOTH == g_demo_config.mediaType))
#endif
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n 1 - Get MAC PIB entry");
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n 2 - Set MAC PIB entry");
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n 3 - Show all MAC PIB entries");
#endif
        }
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
        if ((R_MEDIA_TYPE1_RF == g_demo_config.mediaType) || (R_MEDIA_TYPE2_BOTH == g_demo_config.mediaType))
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n a - Get RF MAC PIB entry");
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n b - Set RF MAC PIB entry");
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n c - Show all RF MAC PIB entries");
#endif
        }
#endif

#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 4 - Get ADP IB entry");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 5 - Set ADP IB entry");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 6 - Show all ADP IB entries");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 7 - Get EAP IB entry");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 8 - Set EAP IB entry");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 9 - Show all EAP IB entries");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n z - Return");
#endif

        R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

        if (strlen ((char *)g_demo_buff.getStringBuffer) == 1)
        {
            switch (g_demo_buff.getStringBuffer[0])
            {
                case '1':
                    while (1)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf ("\n -> Enter MAC PIB identifier (HEX)...");
#endif

                        R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

                        if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
                        {
                            get_mac_pib_entry (R_DEMO_G3_USE_PRIMARY_CH, (r_g3mac_ib_id_t)hex_string_to_uint16 (g_demo_buff.getStringBuffer), R_TRUE);
                        }

                        R_DEMO_HndWaitEnter();
                        break;
                    }
                    break;

                case '2':
                    while (1)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf ("\n -> Enter MAC PIB identifier (HEX)...");
#endif

                        R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

                        if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
                        {
                            set_mac_pib_entry (R_DEMO_G3_USE_PRIMARY_CH, (r_g3mac_ib_id_t)hex_string_to_uint16 (g_demo_buff.getStringBuffer));
                        }

                        R_DEMO_HndWaitEnter();
                        break;
                    }
                    break;

                case '3':
                    r_demo_disp_ib_info (R_G3_LAYER_UMAC);
                    R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);
                    break;

                case '4':
                    while (1)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf ("\n -> Enter ADP IB identifier (HEX)...");
#endif

                        R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

                        if (strlen ((char *)g_demo_buff.getStringBuffer) == 4)
                        {
                            get_ib_entry (R_DEMO_G3_USE_PRIMARY_CH, (r_adp_ib_id_t)hex_string_to_uint8 (g_demo_buff.getStringBuffer), R_TRUE);
                        }

                        R_DEMO_HndWaitEnter();
                        break;
                    }
                    break;

                case '5':
                    while (1)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf ("\n -> Enter ADP IB identifier (HEX)...");
#endif

                        R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

                        if (strlen ((char *)g_demo_buff.getStringBuffer) == 4)
                        {
                            set_ib_entry (R_DEMO_G3_USE_PRIMARY_CH, (r_adp_ib_id_t)hex_string_to_uint8 (g_demo_buff.getStringBuffer));
                        }

                        R_DEMO_HndWaitEnter();
                        break;
                    }
                    break;

                case '6':
                    r_demo_disp_ib_info (R_G3_LAYER_ADP);
                    R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);
                    break;

                case '7':
                    if (R_ADP_DEVICE_TYPE_COORDINATOR != g_demo_config.devType)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf ("\n -> Can not accept ! (Eap sap have not be activated now)");
#endif
                        break;
                    }
                    while (1)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf ("\n -> Enter EAP IB identifier (HEX)...");
#endif

                        R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

                        if (strlen ((char *)g_demo_buff.getStringBuffer) == 4)
                        {
                            get_eap_ib_entry (R_DEMO_G3_USE_PRIMARY_CH, (r_eap_ib_id_t)hex_string_to_uint8 (g_demo_buff.getStringBuffer));
                        }

                        R_DEMO_HndWaitEnter();
                        break;
                    }
                    break;

                case '8':
                    if (R_ADP_DEVICE_TYPE_COORDINATOR != g_demo_config.devType)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf ("\n -> Can not accept ! (Eap sap have not be activated now)");
#endif
                        break;
                    }
                    while (1)
                    {

#ifdef __PLC__DEBUG__
                        R_STDIO_Printf ("\n -> Enter EAP IB identifier (HEX)...");
#endif

                        R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

                        if (strlen ((char *)g_demo_buff.getStringBuffer) == 4)
                        {
                            set_eap_ib_entry (R_DEMO_G3_USE_PRIMARY_CH, (r_eap_ib_id_t)hex_string_to_uint8 (g_demo_buff.getStringBuffer));
                        }

                        R_DEMO_HndWaitEnter();
                        break;
                    }
                    break;

                case '9':
                    r_demo_disp_ib_info (R_G3_LAYER_EAP);
                    R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);
                    break;

                case 'z':
                    return R_RESULT_SUCCESS;

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
                case 'a':
                    while (1)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf ("\n -> Enter RF MAC PIB identifier (HEX)...");
#endif

                        R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

                        if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
                        {
                            get_rfmac_pib_entry (R_DEMO_G3_USE_PRIMARY_CH, (r_g3mac_ib_id_t)hex_string_to_uint16 (g_demo_buff.getStringBuffer));
                        }

                        R_DEMO_HndWaitEnter();
                        break;
                    }
                    break;

                case 'b':
                    while (1)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf ("\n -> Enter RF MAC PIB identifier (HEX)...");
#endif

                        R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

                        if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
                        {
                            set_rfmac_pib_entry (R_DEMO_G3_USE_PRIMARY_CH, (r_g3mac_ib_id_t)hex_string_to_uint16 (g_demo_buff.getStringBuffer));
                        }

                        R_DEMO_HndWaitEnter();
                        break;
                    }
                    break;

                case 'c':
                    r_demo_disp_ib_info (R_G3_LAYER_RFMAC);
                    R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);
                    break;
#endif

                default:
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\n\n Invalid option! \n");
#endif
                    break;
            } /* switch */

        }
    }
} /* menu_config */
/******************************************************************************
   End of function  R_DEMO_MenuConfig
******************************************************************************/

/******************************************************************************
* Function Name: get_ib_entry
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static r_result_t get_ib_entry (uint8_t chId, const r_adp_ib_id_t attributeId, uint8_t allEntries)
{

    r_adp_group_table_t *             groupTablePointer;
    r_adp_broadcast_log_table_t *     broadcastLogTablePointer;
    r_adp_routing_table_t *           routingTablePointer;
    r_adp_prefix_table_t *            prefixTablePointer;
    r_adp_contextinfo_table_t *       contextTablePointer;
    r_adp_blacklisted_nei_table_t *   blacklistTablePointer;
    r_adp_destination_address_set_t * destinationAddressSetPointer;
    uint16_t                          i;
    uint16_t                          j;
    uint16_t                          k = 3u;
    uint8_t                           tmpString[R_ADP_MAX_IB_SIZE];


    switch (attributeId)
    {
        /* One byte elements. */
        case R_ADP_IB_METRIC_TYPE:
        case R_ADP_IB_RLC_ENABLED:
        case R_ADP_IB_ADD_REV_LINK_COST:
        case R_ADP_IB_UNICAST_RREQ_GEN_ENABLE:
        case R_ADP_IB_MAX_HOPS:
        case R_ADP_IB_DEVICE_TYPE:
        case R_ADP_IB_KR:
        case R_ADP_IB_KM:
        case R_ADP_IB_KC:
        case R_ADP_IB_KQ:
        case R_ADP_IB_KH:
        case R_ADP_IB_RREQ_RETRIES:
        case R_ADP_IB_RREQ_WAIT:
        case R_ADP_IB_WEAK_LQI_VALUE:
        case R_ADP_IB_KRT:
        case R_ADP_IB_SECURITY_LEVEL:
        case R_ADP_IB_ACTIVE_KEY_INDEX:
        case R_ADP_IB_DISABLE_DEFAULT_ROUTING:
        case R_ADP_IB_LOW_LQI_VALUE:
        case R_ADP_IB_HIGH_LQI_VALUE:
        case R_ADP_IB_RREP_WAIT:
        case R_ADP_IB_DATATYPE:
        case R_ADP_IB_ENABLE_DATATRANS:
        case R_ADP_IB_DISABLE_RELAY:
        case R_ADP_IB_ROUTE_IND_ENABLE:
        case R_ADP_IB_BEACON_IND_ENABLE:
        case R_ADP_IB_BUFF_IND_DISABLE:
        case R_ADP_IB_KEY_IND_ENABLE:
        case R_ADP_IB_IDS_IND_ENABLE:
        case R_ADP_IB_LOW_LQI_VALUE_RF:
        case R_ADP_IB_HIGH_LQI_VALUE_RF:
        case R_ADP_IB_KQ_RF:
        case R_ADP_IB_KH_RF:
        case R_ADP_IB_KRT_RF:
        case R_ADP_IB_KDC_RF:
        case R_ADP_IB_USE_BACKUPMEDIA:
            R_DEMO_AdpmGetWrap (chId, attributeId, 0, tmpString);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.2X", *(uint8_t *)(tmpString));
#endif
            break;

        /* Two byte elements. */
        case R_ADP_IB_BROADCAST_LOG_TABLE_ENTRY_TTL:
        case R_ADP_IB_COORD_SHORT_ADDRESS:
        case R_ADP_IB_MAX_JOIN_WAIT_TIME:
        case R_ADP_IB_BLACKLIST_TABLE_ENTRY_TTL:
        case R_ADP_IB_ROUTING_TABLE_ENTRY_TTL:
        case R_ADP_IB_NET_TRAVERSAL_TIME:
        case R_ADP_IB_PATH_DISCOVERY_TIME:
        case R_ADP_IB_LOAD_SEQ_NUMBER:
        case R_ADP_IB_ROUTE_TABLE_SIZE:
        case R_ADP_IB_VALID_RTABLE_ENTRIES:
        case R_ADP_IB_LOAD_SEQ_NUM_IND_INTERVAL:
            R_DEMO_AdpmGetWrap (chId, attributeId, 0, tmpString);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.2X%.2X", tmpString[0], tmpString[1]);
#endif
            break;

        case R_ADP_IB_ROUTE_INDEX_BY_ADDR:
            while (1)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\n -> Enter short address (HEX)...");
#endif

                R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

                if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
                {
                    i = hex_string_to_uint16 (g_demo_buff.getStringBuffer);
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("0x%.4X", i);
#endif
                    break;
                }
            }
            R_DEMO_AdpmGetWrap (chId, attributeId, i, tmpString);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.2X%.2X", tmpString[0], tmpString[1]);
#endif
            break;

        case R_ADP_IB_ROUTE_TABLE_BY_ADDR:
            while (1)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\n -> Enter short address (HEX)...");
#endif

                R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

                if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
                {
                    i = hex_string_to_uint16 (g_demo_buff.getStringBuffer);
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("0x%.4X", i);
#endif
                    break;
                }
            }
            if (R_DEMO_AdpmGetWrap (chId, attributeId, i, tmpString) == R_ADP_STATUS_SUCCESS)
            {
                routingTablePointer = (r_adp_routing_table_t *)tmpString;

                if (R_BYTE_ArrToUInt16 (routingTablePointer->validTime))
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\nDstAddress: 0x%.4X NextHopAddr: 0x%.4X mediaType: %d LifeTime(min): 0x%.8X, Route Cost: 0x%.4X, Hop Count: %d",
                                    R_BYTE_ArrToUInt16 (routingTablePointer->R_dest_Addr),
                                    R_BYTE_ArrToUInt16 (routingTablePointer->R_next_Addr),
                                    routingTablePointer->R_media_type,
                                    R_BYTE_ArrToUInt16 (routingTablePointer->validTime),
                                    R_BYTE_ArrToUInt16 (routingTablePointer->R_metric),
                                    routingTablePointer->R_hop_count);
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

        case R_ADP_IB_PREFIX_TABLE:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n --- Prefix Table --- ");
#endif
            k = (allEntries)?R_ADP_PREFIX_TABLE_SIZE : k;
            for (i = 0; i < k; i++)
            {
                R_DEMO_AdpmGetWrap (chId, attributeId, i, tmpString);

                prefixTablePointer = (r_adp_prefix_table_t *)tmpString;

                if (prefixTablePointer->prefixLength > 0)
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\nPrefix Length (bits): %d Valid Lifetime (seconds): %d Prefix Value: 0x",
                                    prefixTablePointer->prefixLength,
                                    R_BYTE_ArrToUInt32(prefixTablePointer->validLifetime));
#endif
                    for (j = 0; j < ((prefixTablePointer->prefixLength + 7) / 8); j++)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf ("%.2X", prefixTablePointer->prefix[j]);
#endif
                    }
                }
                else
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\n Entry not set.");
#endif
                }
            }
            break;

        case R_ADP_IB_CONTEXT_INFORMATION_TABLE:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n --- Context Information Table --- ");
#endif
            k = (allEntries)? R_ADP_CONTEXT_TABLE_SIZE : k;
            for (i = 0; i < k; i++)
            {
                R_DEMO_AdpmGetWrap (chId, attributeId, i, tmpString);

                contextTablePointer = (r_adp_contextinfo_table_t *)tmpString;

                if (contextTablePointer->contextLength > 0)
                {

#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\nCompression Flag: 0x%.2X Context Length (bits): %d \n Valid Lifetime (minutes): %d Prefix Value: 0x",
                                    contextTablePointer->compressionFlag,
                                    contextTablePointer->contextLength,
                                    R_BYTE_ArrToUInt16(contextTablePointer->validLifetime));
#endif
                    for (j = 0; j < ((contextTablePointer->contextLength + 7) / 8); j++)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf ("%.2X", contextTablePointer->context[j]);
#endif
                    }
                }
                else
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\n Entry not set.");
#endif
                }
            }
            break;

        case R_ADP_IB_BROADCAST_LOG_TABLE:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n --- Broadcast Log Table ---");
#endif
            k = (allEntries)? R_ADP_BROADCAST_LOG_TABLE_SIZE : k;
            for (i = 0; i < k; i++)
            {
                /* For larger tables, show in parts. */
                if ((i > 0) &&
                    ((i % R_DEMO_APP_ENTRIES_PER_CYCLE) == 0))
                {
                    R_DEMO_HndWaitEnter();
                }

                R_DEMO_AdpmGetWrap (chId, attributeId, i, tmpString);

                broadcastLogTablePointer = (r_adp_broadcast_log_table_t *)tmpString;

                if (R_BYTE_ArrToUInt16 (broadcastLogTablePointer->validTime) != 0)
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\nTimeStamp(min): 0x%.8X srcAddress: 0x%.4X seqNumber: 0x%.2X",
                                    R_BYTE_ArrToUInt16 (broadcastLogTablePointer->validTime),
                                    R_BYTE_ArrToUInt16 (broadcastLogTablePointer->srcAddr),
                                    broadcastLogTablePointer->seqNumber);
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

        case R_ADP_IB_ROUTING_TABLE:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n --- Routing Table ---");
#endif
            k = (allEntries)? R_DEMO_ADP_ROUTING_TABLE_SIZE : k;
            for (i = 0; i < k; i++)
            {
                /* For larger tables, show in parts. */
                if ((i > 0) &&
                    ((i % R_DEMO_APP_ENTRIES_PER_CYCLE) == 0))
                {
                    R_DEMO_HndWaitEnter();
                }


                /* Get next element */
                R_DEMO_AdpmGetWrap (chId, attributeId, i, tmpString);

                routingTablePointer = (r_adp_routing_table_t *)tmpString;

                if (R_BYTE_ArrToUInt16 (routingTablePointer->validTime))
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\nDst:0x%.4X Next:0x%.4X mediaType:%d LifeTime(min): %d, RCost:0x%.4X, HCount:%d",
                                    R_BYTE_ArrToUInt16 (routingTablePointer->R_dest_Addr),
                                    R_BYTE_ArrToUInt16 (routingTablePointer->R_next_Addr),
                                    routingTablePointer->R_media_type,
                                    R_BYTE_ArrToUInt16 (routingTablePointer->validTime),
                                    R_BYTE_ArrToUInt16 (routingTablePointer->R_metric),
                                    routingTablePointer->R_hop_count);
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

        case R_ADP_IB_GROUP_TABLE:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n --- Group Table ---");
#endif
            k = (allEntries)? R_ADP_GROUP_TABLE_SIZE : k;
            for (i = 0; i < k; i++)
            {
                /* For larger tables, show in parts. */
                if ((i > 0) &&
                    ((i % R_DEMO_APP_ENTRIES_PER_CYCLE) == 0))
                {
                    R_DEMO_HndWaitEnter();
                }

                R_DEMO_AdpmGetWrap (chId, attributeId, i, tmpString);

                groupTablePointer = (r_adp_group_table_t *)tmpString;
                if (groupTablePointer->validState != 0)
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\n validState: 0x%02X Address: 0x%.4X ", groupTablePointer->validState, R_BYTE_ArrToUInt16 (groupTablePointer->memberAddr));
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

        case R_ADP_IB_SOFT_VERSION:
        {
            r_adp_soft_version_t * pAdpVer = (r_adp_soft_version_t *)tmpString;
            R_DEMO_AdpmGetWrap (chId, attributeId, 0, tmpString);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n --- ADP Version :0x%02X%02X\r\n", pAdpVer->adpVersion[0], pAdpVer->adpVersion[1]);
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf (" --- MAC Version :0x%02X%02X\r\n", pAdpVer->macVersion[0], pAdpVer->macVersion[1]);
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf (" --- LMAC,PHY Version :0x%02X%02X\r\n", pAdpVer->dspVersion[0], pAdpVer->dspVersion[1]);
#endif
            break;
        }

        case R_ADP_IB_BLACKLIST_TABLE:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n --- Blacklist Table ---");
#endif
            k = (allEntries)? R_ADP_BLACKLIST_TABLE_SIZE : k;
            for (i = 0; i < k; i++)
            {
                /* For larger tables, show in parts. */
                if ((i > 0) &&
                    ((i % R_DEMO_APP_ENTRIES_PER_CYCLE) == 0))
                {
                    R_DEMO_HndWaitEnter();
                }

                R_DEMO_AdpmGetWrap (chId, attributeId, i, tmpString);
                blacklistTablePointer = (r_adp_blacklisted_nei_table_t *)tmpString;

                if (R_BYTE_ArrToUInt16 (blacklistTablePointer->validTime) != 0)
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\nAddress: 0x%.4X mediaType: %d", R_BYTE_ArrToUInt16 (blacklistTablePointer->B_neighbour_address), blacklistTablePointer->mediaType);
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

        case R_ADP_IB_DESTINATION_ADDRESS_SET:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n --- Destination address  ---");
#endif
            for (i = 0; i < R_ADP_DESTINATION_ADDRESS_SET_SIZE; i++)
            {
                R_DEMO_AdpmGetWrap (chId, attributeId, i, tmpString);
                destinationAddressSetPointer = (r_adp_destination_address_set_t *)tmpString;
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nvalidState: %d ", destinationAddressSetPointer->validState);
#endif
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nDstAddress: 0x%.4X ", R_BYTE_ArrToUInt16 (destinationAddressSetPointer->dstAddr));
#endif
            }
            break;

        default:

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Unknown ID");
#endif
            R_DEMO_AdpmGetWrap (chId, attributeId, 0, tmpString);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.2X%.2X%.2X%.2X", tmpString[0], tmpString[1], tmpString[2], tmpString[3]);
#endif
            break;
    } /* switch */

    return R_RESULT_SUCCESS;
} /* get_ib_entry */
/******************************************************************************
   End of function  get_ib_entry
******************************************************************************/


/******************************************************************************
* Function Name: set_ib_entry
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static r_result_t set_ib_entry (uint8_t chId, const r_adp_ib_id_t attributeId)
{
    uint8_t                     inputData8;
    r_adp_contextinfo_table_t * contextTablePointer;
    r_adp_prefix_table_t *      prefixTablePointer;
    uint8_t                     tempArray[R_ADP_MAX_IB_SIZE];
    uint16_t                    index = 0;
    uint8_t                     i;
    uint32_t                    tmpU32;
    uint16_t                    tmpU16;

    switch (attributeId)
    {
        /* One byte elements. */
        case R_ADP_IB_METRIC_TYPE:
        case R_ADP_IB_RLC_ENABLED:
        case R_ADP_IB_ADD_REV_LINK_COST:
        case R_ADP_IB_UNICAST_RREQ_GEN_ENABLE:
        case R_ADP_IB_MAX_HOPS:
        case R_ADP_IB_DEVICE_TYPE:
        case R_ADP_IB_KR:
        case R_ADP_IB_KM:
        case R_ADP_IB_KC:
        case R_ADP_IB_KQ:
        case R_ADP_IB_KH:
        case R_ADP_IB_RREQ_RETRIES:
        case R_ADP_IB_RREQ_WAIT:
        case R_ADP_IB_WEAK_LQI_VALUE:
        case R_ADP_IB_KRT:
        case R_ADP_IB_SECURITY_LEVEL:
        case R_ADP_IB_ACTIVE_KEY_INDEX:
        case R_ADP_IB_DEFAULT_COORD_ROUTE_ENABLED:
        case R_ADP_IB_DISABLE_DEFAULT_ROUTING:
        case R_ADP_IB_LOW_LQI_VALUE:
        case R_ADP_IB_HIGH_LQI_VALUE:
        case R_ADP_IB_RREP_WAIT:
        case R_ADP_IB_ROUTE_IND_ENABLE:
        case R_ADP_IB_BEACON_IND_ENABLE:
        case R_ADP_IB_BUFF_IND_DISABLE:
        case R_ADP_IB_DATATYPE:
        case R_ADP_IB_ENABLE_DATATRANS:
        case R_ADP_IB_DISABLE_RELAY:
        case R_ADP_IB_KEY_IND_ENABLE:
        case R_ADP_IB_IDS_IND_ENABLE:
        case R_ADP_IB_LOW_LQI_VALUE_RF:
        case R_ADP_IB_HIGH_LQI_VALUE_RF:
        case R_ADP_IB_KQ_RF:
        case R_ADP_IB_KH_RF:
        case R_ADP_IB_KRT_RF:
        case R_ADP_IB_KDC_RF:
        case R_ADP_IB_USE_BACKUPMEDIA:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 8-bit input (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 4)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.2X", hex_string_to_uint8 (g_demo_buff.getStringBuffer));
#endif
                tempArray[0] = hex_string_to_uint8 (g_demo_buff.getStringBuffer);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            break;

        /* Two byte elements. */
        case R_ADP_IB_BROADCAST_LOG_TABLE_ENTRY_TTL:
        case R_ADP_IB_COORD_SHORT_ADDRESS:
        case R_ADP_IB_MAX_JOIN_WAIT_TIME:
        case R_ADP_IB_BLACKLIST_TABLE_ENTRY_TTL:
        case R_ADP_IB_ROUTING_TABLE_ENTRY_TTL:
        case R_ADP_IB_PATH_DISCOVERY_TIME:
        case R_ADP_IB_NET_TRAVERSAL_TIME:
        case R_ADP_IB_LOAD_SEQ_NUMBER:
        case R_ADP_IB_LOAD_SEQ_NUM_IND_INTERVAL:

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 16-bit input (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.4X", hex_string_to_uint16 (g_demo_buff.getStringBuffer));
#endif
                R_BYTE_UInt16ToArr (hex_string_to_uint16 (g_demo_buff.getStringBuffer), tempArray);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            break;

        case R_ADP_IB_GROUP_TABLE:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 8-bit group table index (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 4)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.2X", hex_string_to_uint8 (g_demo_buff.getStringBuffer));
#endif

                inputData8 = hex_string_to_uint8 (g_demo_buff.getStringBuffer);

                index = inputData8;
            }
            else
            {
                return R_RESULT_SUCCESS;
            }

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 16-bit group table address (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
            {
                tempArray[0] = R_TRUE;
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.4X", hex_string_to_uint16 (g_demo_buff.getStringBuffer));
#endif
                R_BYTE_UInt16ToArr (hex_string_to_uint16 (g_demo_buff.getStringBuffer), &tempArray[1]);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            break;

        case R_ADP_IB_PREFIX_TABLE:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 8-bit prefix table index (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 4)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.2X", hex_string_to_uint8 (g_demo_buff.getStringBuffer));
#endif

                inputData8 = hex_string_to_uint8 (g_demo_buff.getStringBuffer);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            prefixTablePointer = (r_adp_prefix_table_t *)tempArray;

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter prefix length in byte...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) > 0)
            {
                g_demo_buff.getStringBuffer[strlen ((char *)g_demo_buff.getStringBuffer)] = '\0';
                prefixTablePointer->prefixLength = (uint8_t)(8 * atoi ((char const *)g_demo_buff.getStringBuffer)); // Conversion to bit
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.2X", prefixTablePointer->prefixLength / 8);
#endif
            }

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter valid lifetime (seconds)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) > 0)
            {
                g_demo_buff.getStringBuffer[strlen ((char *)g_demo_buff.getStringBuffer)] = '\0';
                tmpU32 = (uint32_t)atoi ((char const *)g_demo_buff.getStringBuffer);
                R_BYTE_UInt32ToArr (tmpU32, prefixTablePointer->validLifetime);
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("%d", tmpU32);
#endif
            }

            /* Set prefix. */
            for (i = 0; i < (prefixTablePointer->prefixLength / 8); i++)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\n -> Enter prefix byte %d (HEX): ", i);
#endif

                R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

                if (strlen ((char *)g_demo_buff.getStringBuffer) == 4)
                {
                    prefixTablePointer->prefix[i] = hex_string_to_uint8 (g_demo_buff.getStringBuffer);
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("0x%.2X", prefixTablePointer->prefix[i]);
#endif
                }
            }

            break;

        case R_ADP_IB_CONTEXT_INFORMATION_TABLE:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 8-bit context table index (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 4)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.2X", hex_string_to_uint8 (g_demo_buff.getStringBuffer));
#endif

                inputData8 = hex_string_to_uint8 (g_demo_buff.getStringBuffer);

            }
            else
            {
                return R_RESULT_SUCCESS;
            }

            contextTablePointer = (r_adp_contextinfo_table_t *)tempArray;

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter context length in byte...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) > 0)
            {
                g_demo_buff.getStringBuffer[strlen ((char *)g_demo_buff.getStringBuffer)] = '\0';
                contextTablePointer->contextLength = (uint8_t)(8 * atoi ((char const *)g_demo_buff.getStringBuffer)); // Conversion to bit
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("%d", contextTablePointer->contextLength);
#endif
            }

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter valid lifetime (minutes)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) > 0)
            {
                g_demo_buff.getStringBuffer[strlen ((char *)g_demo_buff.getStringBuffer)] = '\0';
                tmpU16 = (uint16_t)atoi ((char const *)g_demo_buff.getStringBuffer);
                R_BYTE_UInt16ToArr (tmpU16, contextTablePointer->validLifetime);
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("%d", tmpU16);
#endif
            }

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter compression flag value...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) > 0)
            {
                g_demo_buff.getStringBuffer[strlen ((char *)g_demo_buff.getStringBuffer)] = '\0';
                contextTablePointer->compressionFlag = (uint8_t)atoi ((char const *)g_demo_buff.getStringBuffer);
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("%d", contextTablePointer->compressionFlag);
#endif
            }

            /* Set prefix. */
            for (i = 0; i < (contextTablePointer->contextLength / 8); i++)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\n -> Enter contex byte %d (HEX): ", i);
#endif

                R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

                if (strlen ((char *)g_demo_buff.getStringBuffer) == 4)
                {
                    contextTablePointer->context[i] = hex_string_to_uint8 (g_demo_buff.getStringBuffer);
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("0x%.2X", contextTablePointer->context[i]);
#endif
                }
            }
            break;

        case R_ADP_IB_DESTINATION_ADDRESS_SET:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 8-bit Destination address set index (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 4)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.2X", hex_string_to_uint8 (g_demo_buff.getStringBuffer));
#endif

                inputData8 = hex_string_to_uint8 (g_demo_buff.getStringBuffer);

                index = inputData8;
            }
            else
            {
                return R_RESULT_SUCCESS;
            }

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 16-bit destination address (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.4X", hex_string_to_uint16 (g_demo_buff.getStringBuffer));
#endif
                R_BYTE_UInt16ToArr (hex_string_to_uint16 (g_demo_buff.getStringBuffer), ((r_adp_destination_address_set_t *)tempArray)->dstAddr);
                ((r_adp_destination_address_set_t *)tempArray)->validState = R_TRUE;
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
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
                R_BYTE_UInt32ToArr (hex_string_to_uint32 (g_demo_buff.getStringBuffer), tempArray);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }

            break;

    } /* switch */

    R_DEMO_AdpmSetWrap (chId, attributeId, index, tempArray);

    return R_RESULT_SUCCESS;
} /* set_ib_entry */
/******************************************************************************
   End of function  set_ib_entry
******************************************************************************/


/******************************************************************************
* Function Name: get_mac_pib_entry
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static r_result_t get_mac_pib_entry (uint8_t chId, const r_g3mac_ib_id_t attributeId, uint8_t allEntries)
{
    r_g3mac_mac_neighbor_table_t * neighborTableElement;
    r_g3mac_mac_pos_table_t *      posTableElement;
    r_g3mac_device_table_t *       deviceTableElement;
    uint16_t                       i;
    uint16_t                       j;
    uint16_t                       k = 3u;
    uint16_t                       numElems;
    r_g3mac_mlme_get_req_t         mlmeGetReq;
    r_g3mac_mlme_get_cnf_t *       mlmeGetCfm;

    /* Set request structure. */
    mlmeGetReq.pibAttributeId = attributeId;
    mlmeGetReq.pibAttributeIndex = 0;

    switch (attributeId)
    {
        /* One byte elements. */
        case R_G3MAC_IB_ACKWAIT_DURATION:
        case R_G3MAC_IB_MAX_BE:
        case R_G3MAC_IB_BSN:
        case R_G3MAC_IB_DSN:
        case R_G3MAC_IB_MAX_CSMABACKOFFS:
        case R_G3MAC_IB_MIN_BE:
        case R_G3MAC_IB_PROMISCUOUS_MODE:
        case R_G3MAC_IB_MAX_FRAME_RETRIES:
        case R_G3MAC_IB_TIMESTAMP_SUPPORTED:
        case R_G3MAC_IB_SECURITY_ENABLED:
        case R_G3MAC_IB_DUPLICATE_DETECTION_TTL:
        case R_G3MAC_IB_HIGHPRIORITY_WINDOWSIZE:
        case R_G3MAC_IB_CSMA_FAIRNESS_LIMIT:
        case R_G3MAC_IB_BEACON_RAND_WIN_LENGTH:
        case R_G3MAC_IB_A:
        case R_G3MAC_IB_K:
        case R_G3MAC_IB_MINCWATTEMPTS:
        case R_G3MAC_IB_CENELEC_LEGACY_MODE:
        case R_G3MAC_IB_FCC_LEGACY_MODE:
        case R_G3MAC_IB_BROADCAST_MAX_CW_EANBLE:
        case R_G3MAC_IB_TRANSMIT_ATTEN:
        case R_G3MAC_IB_PLC_DISABLE:
        case R_G3MAC_IB_COHERENT_TRANSMISSION:
        case R_G3MAC_IB_KEY_VALIDATE:
        case R_G3MAC_IB_UNICAST_DATA_TX_TIMEOUT:
        case R_G3MAC_IB_BROADCAST_DATA_TX_TIMEOUT:
        case R_G3MAC_IB_BEACON_REQUEST_TX_TIMEOUT:
        case R_G3MAC_IB_TX_GAIN:
        case R_G3MAC_IB_TMR_IND_ENABLE:
        case R_G3MAC_IB_KEEP_MOD_RETRY_NUM:
        case R_G3MAC_IB_NEI_UPDATE_AFTER_RETRANSMIT:
        case R_G3MAC_IB_COMM_STATUS_IND_MASK:
        case R_G3MAC_IB_OFFSET_SNR:
        case R_G3MAC_IB_THRESH_CARRIER_NUM:
        case R_G3MAC_IB_THRESH_TONENUM_PER_MAP:
        case R_G3MAC_IB_THRESH_SNR_DIFFERENTIAL:
        case R_G3MAC_IB_THRESH_SNR_COHERENT:
        case R_G3MAC_IB_DISABLE_BEACON:
        case R_G3MAC_IB_DISABLE_TMRES:
        case R_G3MAC_IB_REF_RSSI_DB:
        case R_G3MAC_IB_DISABLE_TXGAIN_TMR:
        case R_G3MAC_IB_ENABLE_DEBUG:
        case R_G3MAC_IB_POS_RECENT_ENTRY_TH:
        case R_G3MAC_IB_TRICKLE_MIN_LQI_VALUE:
        case R_G3MAC_IB_PHY_TX_POWER:
        case R_G3MAC_IB_PHY_TX_DIGITAL_PREAMBLE_GAIN:
        case R_G3MAC_IB_PHY_TX_DIGITAL_GAIN:
        case R_G3MAC_IB_PHY_TXENB_POLARITY:
        case R_G3MAC_IB_PHY_TX_WAIT_TIME:
        case R_G3MAC_IB_PHY_TX_BREAK:
        case R_G3MAC_IB_PHY_TX_ACK_GAIN:
        case R_G3MAC_IB_PHY_SATT_CTRL_DISABLE:
        case R_G3MAC_IB_PHY_TXRX_DISABLE:
        case R_G3MAC_IB_PHY_ZCD_IND_POLARITY:
        case R_G3MAC_IB_PHY_TPGA_CAJ:
        case R_G3MAC_IB_PHY_SATT_POLARITY:
        case R_G3MAC_IB_PHY_PREAMBLE_NUM:
        case R_G3MAC_IB_PHY_RX_GAIN_CUR:
        case R_G3MAC_IB_LML_LQI_ADJ_BND_HI:
        case R_G3MAC_IB_LML_LQI_ADJ_BND_LO:
        case R_G3MAC_IB_LML_LQI_ADJ_SLOPE_HI:
        case R_G3MAC_IB_LML_LQI_ADJ_SLOPE_LO:
            R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.2X", *(uint8_t *)(mlmeGetCfm->pibAttributeValue));
#endif
            break;

        /* Two byte elements. */
        case R_G3MAC_IB_PANID:
        case R_G3MAC_IB_SHORTADDRESS:
        case R_G3MAC_IB_RCCOORD:
        case R_G3MAC_IB_NEIGHBOUR_TABLE_SIZE:
        case R_G3MAC_IB_DEVICE_TABLE_SIZE:
        case R_G3MAC_IB_VALID_NEITABLE_ENTRIES:
        case R_G3MAC_IB_VALID_DEVTABLE_ENTRIES:
        case R_G3MAC_IB_VALID_POSTABLE_ENTRIES:
        case R_G3MAC_IB_MIN_NEITABLE_ENTRIES:
        case R_G3MAC_IB_TOTAL_NEITABLE_ENTRIES:
        case R_G3MAC_IB_TOTAL_POSTABLE_ENTRIES:
        case R_G3MAC_IB_POS_RECENT_ENTRY:
        case R_G3MAC_IB_PHY_TX_FILTER_SCALE:
        case R_G3MAC_IB_PHY_AC_PHASE_OFFSET:
            R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.4X", R_BYTE_ArrToUInt16 ((mlmeGetCfm->pibAttributeValue)));
#endif
            break;

        /* Four byte elements. */
        case R_G3MAC_IB_FRAME_COUNTER:
        case R_G3MAC_IB_TXDATAPACKET_COUNT:
        case R_G3MAC_IB_RXDATAPACKET_COUNT:
        case R_G3MAC_IB_TXCMDPACKET_COUNT:
        case R_G3MAC_IB_RXCMDPACKET_COUNT:
        case R_G3MAC_IB_CSMAFAIL_COUNT:
        case R_G3MAC_IB_CSMANOACK_COUNT:
        case R_G3MAC_IB_RXDATABROADCAST_COUNT:
        case R_G3MAC_IB_TXDATABROADCAST_COUNT:
        case R_G3MAC_IB_BADCRC_COUNT:
        case R_G3MAC_IB_TMR_TTL:
        case R_G3MAC_IB_POS_TABLE_ENTRY_TTL:
        case R_G3MAC_IB_FRAME_CNT_IND_INTERVAL:
        case R_G3MAC_IB_TMR_REQ_LEAD_TIME_BEF_TTL:
            R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.8X", R_BYTE_ArrToUInt32 ((mlmeGetCfm->pibAttributeValue)));
#endif
            break;

        case R_G3MAC_IB_STATISTICS:
        case R_G3MAC_IB_PHY_STATISTICS:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 16-bit index (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.4X", hex_string_to_uint16 (g_demo_buff.getStringBuffer));
#endif

                mlmeGetReq.pibAttributeIndex = hex_string_to_uint16 (g_demo_buff.getStringBuffer);
            }
            else
            {
                return R_RESULT_FAILED;
            }
            R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.8X", R_BYTE_ArrToUInt32 ((mlmeGetCfm->pibAttributeValue)));
#endif
            break;

        case R_G3MAC_IB_EXTADDRESS:
            R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("Extended address: 0x%.2X", *(uint8_t *)(mlmeGetCfm->pibAttributeValue));
#endif
            for (i = 1; i < 8; i++)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("%.2X", *(uint8_t *)(mlmeGetCfm->pibAttributeValue + i));
#endif
            }
            break;

        case R_G3MAC_IB_NEIGHBOUR_INDEX_BY_SHORT_ADDR:
        case R_G3MAC_IB_DEVICE_INDEX_BY_SHORT_ADDR:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 16-bit address (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.4X", hex_string_to_uint16 (g_demo_buff.getStringBuffer));
#endif
                mlmeGetReq.pibAttributeIndex = hex_string_to_uint16 (g_demo_buff.getStringBuffer);
            }
            else
            {
                return R_RESULT_FAILED;
            }

            R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.4X", R_BYTE_ArrToUInt16 ((mlmeGetCfm->pibAttributeValue)));
#endif
            break;

        case R_G3MAC_IB_NEIGHBOUR_TABLE_BY_SHORT_ADDR:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 16-bit address (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.4X", hex_string_to_uint16 (g_demo_buff.getStringBuffer));
#endif

                mlmeGetReq.pibAttributeIndex = hex_string_to_uint16 (g_demo_buff.getStringBuffer);
            }
            else
            {
                return R_RESULT_FAILED;
            }

            R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);

            neighborTableElement = (r_g3mac_mac_neighbor_table_t *)(mlmeGetCfm->pibAttributeValue);

            if ((R_BYTE_ArrToUInt32 (neighborTableElement->tmrValidTime)) && (R_G3MAC_STATUS_SUCCESS == mlmeGetCfm->status))
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nTxGain: 0x%.2X \nTxRes: 0x%.2X \nShortAddress: 0x%.4X, \nToneMap: 0x%.2X%.2X%.2X, \nModScheme: 0x%.2X, \nPhaseDiff: 0x%.2X, \nModulation: 0x%.2X, \nTMRValid: 0x%.8X, \nrevLQI: 0x%.2X",
                                neighborTableElement->sta.bit.txGain,
                                neighborTableElement->sta.bit.txRes,
                                R_BYTE_ArrToUInt16 (neighborTableElement->shortAddress),
                                neighborTableElement->tonemap[0],
                                neighborTableElement->tonemap[1],
                                neighborTableElement->tonemap[2],
                                neighborTableElement->mod.bit.pms,
                                neighborTableElement->mod.bit.phaseDiff,
                                neighborTableElement->mod.bit.modType,
                                R_BYTE_ArrToUInt32 (neighborTableElement->tmrValidTime),
                                neighborTableElement->revLqi);
#endif
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nTxCoeff: 0x%.2X%.2X%.2X%.2X%.2X%.2X",
                                neighborTableElement->txCoeff[0],
                                neighborTableElement->txCoeff[1],
                                neighborTableElement->txCoeff[2],
                                neighborTableElement->txCoeff[3],
                                neighborTableElement->txCoeff[4],
                                neighborTableElement->txCoeff[5]);
#endif
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nFwdLqi: 0x%.2X", neighborTableElement->fwdLqi);
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\n Entry not set.");
#endif
            }
            break;

        case R_G3MAC_IB_NEIGHBOUR_TABLE:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\nReading number of Neighbour Table entries.");
#endif

            /* Set request structure to read number of elements. */
            mlmeGetReq.pibAttributeId = (uint16_t)R_G3MAC_IB_NEIGHBOUR_TABLE_SIZE;
            mlmeGetReq.pibAttributeIndex = 0;

            R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);

            numElems = R_BYTE_ArrToUInt16 ((mlmeGetCfm->pibAttributeValue));

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.4X", numElems);
#endif

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n --- Neighbour Table ---");
#endif

            /* Set request structure to read the elements. */
            mlmeGetReq.pibAttributeId = (uint16_t)R_G3MAC_IB_NEIGHBOUR_TABLE;
            mlmeGetReq.pibAttributeIndex = 0;

            k = (allEntries) ? numElems : k;
            for (i = 0; i < k; i++)
            {
                /* For larger tables, show in parts. */
                if ((i > 0) &&
                    ((i % R_DEMO_APP_ENTRIES_PER_CYCLE) == 0))
                {
                    R_DEMO_HndWaitEnter();
                }

                R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);

                neighborTableElement = (r_g3mac_mac_neighbor_table_t *)(mlmeGetCfm->pibAttributeValue);

                if (R_BYTE_ArrToUInt32 (neighborTableElement->tmrValidTime))
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\nTxGain: 0x%.2X \nTxRes: 0x%.2X \nShortAddress: 0x%.4X, \nToneMap: 0x%.2X%.2X%.2X, \nModScheme: 0x%.2X, \nPhaseDiff: 0x%.2X, \nModulation: 0x%.2X, \nTMRValid: 0x%.8X, \nrevLQI: 0x%.2X",
                                    neighborTableElement->sta.bit.txGain,
                                    neighborTableElement->sta.bit.txRes,
                                    R_BYTE_ArrToUInt16 (neighborTableElement->shortAddress),
                                    neighborTableElement->tonemap[0],
                                    neighborTableElement->tonemap[1],
                                    neighborTableElement->tonemap[2],
                                    neighborTableElement->mod.bit.pms,
                                    neighborTableElement->mod.bit.phaseDiff,
                                    neighborTableElement->mod.bit.modType,
                                    R_BYTE_ArrToUInt32 (neighborTableElement->tmrValidTime),
                                    neighborTableElement->revLqi);
#endif
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\nTxCoeff: 0x%.2X%.2X%.2X%.2X%.2X%.2X",
                                    neighborTableElement->txCoeff[0],
                                    neighborTableElement->txCoeff[1],
                                    neighborTableElement->txCoeff[2],
                                    neighborTableElement->txCoeff[3],
                                    neighborTableElement->txCoeff[4],
                                    neighborTableElement->txCoeff[5]);
#endif
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\nFwdLqi: 0x%.2X", neighborTableElement->fwdLqi);
#endif
                }
                else
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\n Entry not set.");
#endif
                }

                mlmeGetReq.pibAttributeIndex++;
            }
            break;

        case R_G3MAC_IB_TONEMASK:
            R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("Tone mask: 0x%.2X", mlmeGetCfm->pibAttributeValue[0]);
#endif
            for (i = 1; i < 9; i++)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf (" ,0x%.2X", mlmeGetCfm->pibAttributeValue[i]);
#endif
            }
            break;

        case R_G3MAC_IB_KEY_TABLE:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\nReading number of key table entries.");
#endif

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n --- MAC key table ---");
#endif
            mlmeGetReq.pibAttributeIndex = 0;

            for (i = 0; i < R_G3MAC_KEY_NUM; i++)
            {
                /* Set request structure to read the elements. */
                mlmeGetReq.pibAttributeId = (uint16_t)R_G3MAC_IB_KEY_VALIDATE;

                R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);

                if (0x01 == (*(uint8_t *)(mlmeGetCfm->pibAttributeValue)))
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("Key with index 0x%.2X valid.", mlmeGetCfm->pibAttributeIndex);
#endif
                }
                else
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("Key with index 0x%.2X invalid.", mlmeGetCfm->pibAttributeIndex);
#endif
                }

                /* Set request structure to read the elements. */
                mlmeGetReq.pibAttributeId = (uint16_t)R_G3MAC_IB_KEY_TABLE;

                R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);

                if (R_ADP_STATUS_SUCCESS == mlmeGetCfm->status)
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("Key value: 0x");
#endif

                    for (j = 0; j < 16; j++)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf ("%.2X", *(uint8_t *)(mlmeGetCfm->pibAttributeValue + j));
#endif
                    }
                }
                else
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("Key not set.");
#endif
                }

                mlmeGetReq.pibAttributeIndex++;
            }
            break;

        case R_G3MAC_IB_DEVICE_TABLE:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\nReading number of Neighbour Table entries.");
#endif

            /* Set request structure to read number of elements. */
            mlmeGetReq.pibAttributeId = (uint16_t)R_G3MAC_IB_DEVICE_TABLE_SIZE;
            mlmeGetReq.pibAttributeIndex = 0;

            R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);

            numElems = R_BYTE_ArrToUInt16 ((mlmeGetCfm->pibAttributeValue));

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.4X", numElems);
#endif

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n --- Device Table ---");
#endif

            /* Set request structure to read the elements. */
            mlmeGetReq.pibAttributeId = (uint16_t)R_G3MAC_IB_DEVICE_TABLE;
            mlmeGetReq.pibAttributeIndex = 0;

            k = (allEntries) ? numElems : k;
            for (i = 0; i < k; i++)
            {
                /* For larger tables, show in parts. */
                if ((i > 0) &&
                    ((i % R_DEMO_APP_ENTRIES_PER_CYCLE) == 0))
                {
                    R_DEMO_HndWaitEnter();
                }

                R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);

                deviceTableElement = (r_g3mac_device_table_t *)(mlmeGetCfm->pibAttributeValue);

                if (R_BYTE_ArrToUInt32 (deviceTableElement->updatedTime))
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\nSecurity Status: 0x%.2X \nSource Address: 0x%.4X \nFrame Counter: 0x%.8X",
                                    R_BYTE_ArrToUInt32 (deviceTableElement->updatedTime) ? R_TRUE : R_FALSE,
                                    R_BYTE_ArrToUInt16 (deviceTableElement->shortAddress),
                                    R_BYTE_ArrToUInt32 (deviceTableElement->frameCounter));
#endif
                }
                else
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\n Entry not set.");
#endif
                }

                mlmeGetReq.pibAttributeIndex++;
            }
            break;

        case R_G3MAC_IB_SOFT_VERSION:
        {
            r_g3mac_soft_version_t * pAdpVer;
            R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);
            pAdpVer = (r_g3mac_soft_version_t *)mlmeGetCfm->pibAttributeValue;
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n --- MAC Version :0x%02X%02X\r\n", pAdpVer->macVersion[0], pAdpVer->macVersion[1]);
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf (" --- LMAC,PHY Version :0x%02X%02X\r\n", pAdpVer->dspVersion[0], pAdpVer->dspVersion[1]);
#endif
            break;
        }

        case R_G3MAC_IB_DEVICE_TABLE_BY_SHORT_ADDR:
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 16-bit address (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.4X", hex_string_to_uint16 (g_demo_buff.getStringBuffer));
#endif
                mlmeGetReq.pibAttributeIndex = hex_string_to_uint16 (g_demo_buff.getStringBuffer);
            }
            else
            {
                return R_RESULT_FAILED;
            }

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\nDevice Table short address.");
#endif
            R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);

            if (R_G3MAC_STATUS_SUCCESS != mlmeGetCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nStatus:0x%02X\n", mlmeGetCfm->status);
#endif
            }
            else
            {
                r_g3mac_device_table_t * pDstDev = (r_g3mac_device_table_t *)mlmeGetCfm->pibAttributeValue;

#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nindex: %d", mlmeGetCfm->pibAttributeIndex);
#endif
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nshortAddress: 0x%04X", R_BYTE_ArrToUInt16 (pDstDev->shortAddress));
#endif
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nframeCounter : 0x%08lX", R_BYTE_ArrToUInt32 (pDstDev->frameCounter));
#endif
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nupdatedTime  : 0x%08lX", R_BYTE_ArrToUInt32 (pDstDev->updatedTime));
#endif
            }
            break;
        }

        case R_G3MAC_IB_POS_TABLE:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\nReading number of POS Table entries.");
#endif

            /* Set request structure to read number of elements. */
            mlmeGetReq.pibAttributeId = (uint16_t)R_G3MAC_IB_NEIGHBOUR_TABLE_SIZE;
            mlmeGetReq.pibAttributeIndex = 0;

            R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);

            numElems = R_BYTE_ArrToUInt16 ((mlmeGetCfm->pibAttributeValue));

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.4X", numElems);
#endif

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n --- Pos Table ---");
#endif

            /* Set request structure to read the elements. */
            mlmeGetReq.pibAttributeId = (uint16_t)R_G3MAC_IB_POS_TABLE;
            mlmeGetReq.pibAttributeIndex = 0;

            k = (allEntries) ? numElems : k;
            for (i = 0; i < k; i++)
            {
                /* For larger tables, show in parts. */
                if ((i > 0) &&
                    ((i % R_DEMO_APP_ENTRIES_PER_CYCLE) == 0))
                {
                    R_DEMO_HndWaitEnter();
                }

                R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);

                posTableElement = (r_g3mac_mac_pos_table_t *)(mlmeGetCfm->pibAttributeValue);

                if (R_BYTE_ArrToUInt32 (posTableElement->posValidTime))
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\nShortAddress: 0x%.4X \nposValidTime: 0x%.8X, \nfwdLQI: 0x%.2X",
                                    R_BYTE_ArrToUInt16 (posTableElement->shortAddress),
                                    R_BYTE_ArrToUInt32 (posTableElement->posValidTime),
                                    posTableElement->fwdLqi);
#endif
                }
                else
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\n Entry not set.");
#endif
                }

                mlmeGetReq.pibAttributeIndex++;
            }
            break;

        case R_G3MAC_IB_POS_TABLE_BY_SHORT_ADDR:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 16-bit address (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.4X", hex_string_to_uint16 (g_demo_buff.getStringBuffer));
#endif

                mlmeGetReq.pibAttributeIndex = hex_string_to_uint16 (g_demo_buff.getStringBuffer);
            }
            else
            {
                return R_RESULT_FAILED;
            }

            R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);

            posTableElement = (r_g3mac_mac_pos_table_t *)(mlmeGetCfm->pibAttributeValue);

            if (R_BYTE_ArrToUInt32 (posTableElement->posValidTime))
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nShortAddress: 0x%.4X \nposValidTime: 0x%.8X, \nfwdLQI: 0x%.2X",
                                R_BYTE_ArrToUInt16 (posTableElement->shortAddress),
                                R_BYTE_ArrToUInt32 (posTableElement->posValidTime),
                                posTableElement->fwdLqi);
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\n Entry not set.");
#endif
            }

            break;

        default:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Unknown ID");
#endif
            R_DEMO_MlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.8lX", R_BYTE_ArrToUInt32 ((mlmeGetCfm->pibAttributeValue)));
#endif
            break;

    } /* switch */

    return R_RESULT_SUCCESS;
} /* get_mac_pib_entry */
/******************************************************************************
   End of function  get_mac_pib_entry
******************************************************************************/


/******************************************************************************
* Function Name: set_mac_pib_entry
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static r_result_t set_mac_pib_entry (uint8_t chId, const r_g3mac_ib_id_t attributeId)
{
    uint16_t                 i = 0;
    r_g3mac_mlme_set_req_t   mlmeSetReq;
    r_g3mac_mlme_set_cnf_t * mlmeSetCfm;

    uint8_t                  tempArray[R_ADP_MAX_IB_SIZE];

    /* Set request structure. */
    mlmeSetReq.pibAttributeId = attributeId;
    mlmeSetReq.pibAttributeIndex = 0;
    mlmeSetReq.pibAttributeValue = tempArray;

    switch (attributeId)
    {
        /* One byte elements. */
        case R_G3MAC_IB_ACKWAIT_DURATION:
        case R_G3MAC_IB_MAX_BE:
        case R_G3MAC_IB_BSN:
        case R_G3MAC_IB_DSN:
        case R_G3MAC_IB_MAX_CSMABACKOFFS:
        case R_G3MAC_IB_MIN_BE:
        case R_G3MAC_IB_PROMISCUOUS_MODE:
        case R_G3MAC_IB_MAX_FRAME_RETRIES:
        case R_G3MAC_IB_SECURITY_ENABLED:
        case R_G3MAC_IB_DUPLICATE_DETECTION_TTL:
        case R_G3MAC_IB_HIGHPRIORITY_WINDOWSIZE:
        case R_G3MAC_IB_CSMA_FAIRNESS_LIMIT:
        case R_G3MAC_IB_BEACON_RAND_WIN_LENGTH:
        case R_G3MAC_IB_A:
        case R_G3MAC_IB_K:
        case R_G3MAC_IB_MINCWATTEMPTS:
        case R_G3MAC_IB_BROADCAST_MAX_CW_EANBLE:
        case R_G3MAC_IB_TRANSMIT_ATTEN:
        case R_G3MAC_IB_PLC_DISABLE:

        case R_G3MAC_IB_COHERENT_TRANSMISSION:
        case R_G3MAC_IB_UNICAST_DATA_TX_TIMEOUT:
        case R_G3MAC_IB_BROADCAST_DATA_TX_TIMEOUT:
        case R_G3MAC_IB_BEACON_REQUEST_TX_TIMEOUT:
        case R_G3MAC_IB_TX_GAIN:
        case R_G3MAC_IB_TMR_IND_ENABLE:
        case R_G3MAC_IB_KEEP_MOD_RETRY_NUM:
        case R_G3MAC_IB_NEI_UPDATE_AFTER_RETRANSMIT:
        case R_G3MAC_IB_COMM_STATUS_IND_MASK:
        case R_G3MAC_IB_OFFSET_SNR:
        case R_G3MAC_IB_THRESH_CARRIER_NUM:
        case R_G3MAC_IB_THRESH_TONENUM_PER_MAP:
        case R_G3MAC_IB_THRESH_SNR_DIFFERENTIAL:
        case R_G3MAC_IB_THRESH_SNR_COHERENT:
        case R_G3MAC_IB_DISABLE_BEACON:
        case R_G3MAC_IB_DISABLE_TMRES:
        case R_G3MAC_IB_REF_RSSI_DB:
        case R_G3MAC_IB_DISABLE_TXGAIN_TMR:
        case R_G3MAC_IB_ENABLE_DEBUG:
        case R_G3MAC_IB_POS_RECENT_ENTRY_TH:
        case R_G3MAC_IB_TRICKLE_MIN_LQI_VALUE:
        case R_G3MAC_IB_KEY_VALIDATE:
        case R_G3MAC_IB_PHY_TX_POWER:
        case R_G3MAC_IB_PHY_TX_DIGITAL_PREAMBLE_GAIN:
        case R_G3MAC_IB_PHY_TX_DIGITAL_GAIN:
        case R_G3MAC_IB_PHY_TXENB_POLARITY:
        case R_G3MAC_IB_PHY_TX_WAIT_TIME:
        case R_G3MAC_IB_PHY_TX_BREAK:
        case R_G3MAC_IB_PHY_TX_ACK_GAIN:
        case R_G3MAC_IB_PHY_SATT_CTRL_DISABLE:
        case R_G3MAC_IB_PHY_TXRX_DISABLE:
        case R_G3MAC_IB_PHY_ZCD_IND_POLARITY:
        case R_G3MAC_IB_PHY_TPGA_CAJ:
        case R_G3MAC_IB_PHY_SATT_POLARITY:
        case R_G3MAC_IB_PHY_PREAMBLE_NUM:
        case R_G3MAC_IB_LML_LQI_ADJ_BND_HI:
        case R_G3MAC_IB_LML_LQI_ADJ_BND_LO:
        case R_G3MAC_IB_LML_LQI_ADJ_SLOPE_HI:
        case R_G3MAC_IB_LML_LQI_ADJ_SLOPE_LO:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 8-bit input (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 4)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.2X", hex_string_to_uint8 (g_demo_buff.getStringBuffer));
#endif

                mlmeSetReq.pibAttributeValue[0] = hex_string_to_uint8 (g_demo_buff.getStringBuffer);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            R_DEMO_MlmeSet (chId, &mlmeSetReq, &mlmeSetCfm);
            break;

        /* Two byte elements. */
        case R_G3MAC_IB_PANID:
        case R_G3MAC_IB_SHORTADDRESS:
        case R_G3MAC_IB_RCCOORD:
        case R_G3MAC_IB_MIN_NEITABLE_ENTRIES:
        case R_G3MAC_IB_PHY_TX_FILTER_SCALE:
        case R_G3MAC_IB_PHY_AC_PHASE_OFFSET:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 16-bit input (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.4X", hex_string_to_uint16 (g_demo_buff.getStringBuffer));
#endif

                R_BYTE_UInt16ToArr (hex_string_to_uint16 (g_demo_buff.getStringBuffer), mlmeSetReq.pibAttributeValue);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            R_DEMO_MlmeSet (chId, &mlmeSetReq, &mlmeSetCfm);
            break;

        /* Four byte elements. */
        case R_G3MAC_IB_FRAME_COUNTER:
        case R_G3MAC_IB_TMR_TTL:
        case R_G3MAC_IB_POS_TABLE_ENTRY_TTL:
        case R_G3MAC_IB_FRAME_CNT_IND_INTERVAL:
        case R_G3MAC_IB_TMR_REQ_LEAD_TIME_BEF_TTL:
        case R_G3MAC_IB_PHY_STATISTICS:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 32-bit input (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 10)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.8lX", hex_string_to_uint32 (g_demo_buff.getStringBuffer));
#endif

                R_BYTE_UInt32ToArr (hex_string_to_uint32 (g_demo_buff.getStringBuffer), mlmeSetReq.pibAttributeValue);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            R_DEMO_MlmeSet (chId, &mlmeSetReq, &mlmeSetCfm);
            break;

        case R_G3MAC_IB_NEIGHBOUR_TABLE_BY_SHORT_ADDR:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("-> Writing of this attribute is currently not supported.");
#endif
            break;

        case R_G3MAC_IB_TONEMASK:
            while (i < 9)
            {
                i = 0;
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\n -> Enter tone mask element %d (HEX, 8bit)...", i);
#endif

                R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

                if (strlen ((char *)g_demo_buff.getStringBuffer) == 4)
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("0x%.2X", hex_string_to_uint8 (g_demo_buff.getStringBuffer));
#endif

                    mlmeSetReq.pibAttributeValue[i] = hex_string_to_uint8 (g_demo_buff.getStringBuffer);

                    i++;
                }
            }
            R_DEMO_MlmeSet (chId, &mlmeSetReq, &mlmeSetCfm);
            break;
        case R_G3MAC_IB_TIMESTAMP_SUPPORTED:
        case R_G3MAC_IB_KEY_TABLE:
        case R_G3MAC_IB_DEVICE_TABLE:
        case R_G3MAC_IB_TXDATAPACKET_COUNT:
        case R_G3MAC_IB_RXDATAPACKET_COUNT:
        case R_G3MAC_IB_TXCMDPACKET_COUNT:
        case R_G3MAC_IB_RXCMDPACKET_COUNT:
        case R_G3MAC_IB_CSMAFAIL_COUNT:
        case R_G3MAC_IB_CSMANOACK_COUNT:
        case R_G3MAC_IB_RXDATABROADCAST_COUNT:
        case R_G3MAC_IB_TXDATABROADCAST_COUNT:
        case R_G3MAC_IB_BADCRC_COUNT:
        case R_G3MAC_IB_NEIGHBOUR_TABLE:
        case R_G3MAC_IB_CENELEC_LEGACY_MODE:
        case R_G3MAC_IB_FCC_LEGACY_MODE:
        case R_G3MAC_IB_POS_TABLE:
        case R_G3MAC_IB_NEIGHBOUR_TABLE_SIZE:
        case R_G3MAC_IB_DEVICE_TABLE_SIZE:
        case R_G3MAC_IB_VALID_NEITABLE_ENTRIES:
        case R_G3MAC_IB_VALID_DEVTABLE_ENTRIES:
        case R_G3MAC_IB_EXTADDRESS:
        case R_G3MAC_IB_SOFT_VERSION:
        case R_G3MAC_IB_DEVICE_TABLE_BY_SHORT_ADDR:
        case R_G3MAC_IB_NEIGHBOUR_INDEX_BY_SHORT_ADDR:
        case R_G3MAC_IB_DEVICE_INDEX_BY_SHORT_ADDR:
        case R_G3MAC_IB_VALID_POSTABLE_ENTRIES:
        case R_G3MAC_IB_POS_TABLE_BY_SHORT_ADDR:
        case R_G3MAC_IB_TOTAL_NEITABLE_ENTRIES:
        case R_G3MAC_IB_TOTAL_POSTABLE_ENTRIES:
        case R_G3MAC_IB_POS_RECENT_ENTRY:
        case R_G3MAC_IB_STATISTICS:
        case R_G3MAC_IB_PHY_RX_GAIN_CUR:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\nWrite access to attribute not supported by demo application.");
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
                R_STDIO_Printf ("0x%.8lX", hex_string_to_uint32 (g_demo_buff.getStringBuffer));
#endif

                R_BYTE_UInt32ToArr (hex_string_to_uint32 (g_demo_buff.getStringBuffer), mlmeSetReq.pibAttributeValue);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            R_DEMO_MlmeSet (chId, &mlmeSetReq, &mlmeSetCfm);
            break;

    } /* switch */

    return R_RESULT_SUCCESS;
} /* set_mac_pib_entry */
/******************************************************************************
   End of function  set_mac_pib_entry
******************************************************************************/

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
/******************************************************************************
* Function Name: get_adp_table
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void get_adp_table (uint8_t chId)
{

    get_ib_entry(chId, R_ADP_IB_ROUTING_TABLE, R_FALSE);
    get_ib_entry(chId, R_ADP_IB_BLACKLIST_TABLE, R_FALSE);
    if ((R_MEDIA_TYPE0_PLC == g_demo_config.mediaType) || (R_MEDIA_TYPE2_BOTH == g_demo_config.mediaType))
    get_mac_pib_entry(chId, R_G3MAC_IB_POS_TABLE, R_FALSE);
    if ((R_MEDIA_TYPE1_RF == g_demo_config.mediaType) || (R_MEDIA_TYPE2_BOTH == g_demo_config.mediaType))
        get_rfmac_pib_entry(chId, R_G3RFMAC_IB_POSTABLE_RF);

    return;
} /* get_adp_table */
/******************************************************************************
   End of function  get_adp_table
******************************************************************************/


/******************************************************************************
* Function Name: set_adp_table
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void set_adp_table (uint8_t chId)
{
    r_adp_routing_table_t *     pRouteTable;
    r_adp_blacklisted_nei_table_t *   pBlacklistTable;
    uint8_t                     tempArray[R_ADP_MAX_IB_SIZE];
    uint16_t                    i;
    uint16_t                    index = 0;
    
    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 1 - route table");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 2 - black listed");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n c - all clear");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n z - Return");
#endif
        
        R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

        if (strlen ((char *)g_demo_buff.getStringBuffer) == 1)
        {
            switch (g_demo_buff.getStringBuffer[0])
            {
            case '1':
                if (R_DEMO_AdpmGetWrap(chId, R_ADP_IB_VALID_RTABLE_ENTRIES, 0, tempArray) != R_ADP_STATUS_SUCCESS)
                {
                    return;
                }
                index = tempArray[1];

                pRouteTable = (r_adp_routing_table_t *)tempArray;
                R_memset(pRouteTable, 0, sizeof(r_adp_routing_table_t));

#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\n -> Enter 32-bit dest and next address (HEX)...");
#endif
                R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);
                R_BYTE_UInt32ToArr(hex_string_to_uint32(g_demo_buff.getStringBuffer), pRouteTable->R_dest_Addr);
                
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\n -> Enter 8-bit media type (HEX)...");
#endif
                R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);
                pRouteTable->R_media_type = hex_string_to_uint8 (g_demo_buff.getStringBuffer);

                pRouteTable->R_metric[1] = 0xFF; /* interim */
                pRouteTable->R_hop_count = 0x4u; /* interim */
                pRouteTable->validTime[1] = 0xFF; /* interim */
                
                R_DEMO_AdpmSetWrap(chId, R_ADP_IB_ROUTING_TABLE, index, tempArray);
                break;

            case '2':
                pBlacklistTable = (r_adp_blacklisted_nei_table_t *)tempArray;

                for (i = 0; i < 16; i++)
                {
                    if (R_DEMO_AdpmGetWrap(chId, R_ADP_IB_BLACKLIST_TABLE, 0, tempArray) != R_ADP_STATUS_SUCCESS)
                    {
                        return;
                    }
                    if (0 == R_BYTE_ArrToUInt16(pBlacklistTable->validTime))
                    {
                        break;
                    }
                }
                index = i;

                R_memset(pBlacklistTable, 0, sizeof(r_adp_blacklisted_nei_table_t));

#ifdef __PLC__DEBUG__
                R_STDIO_Printf("\n -> Enter 16-bit nei address (HEX)...");
#endif
                R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);
                R_BYTE_UInt16ToArr(hex_string_to_uint16(g_demo_buff.getStringBuffer), pBlacklistTable->B_neighbour_address);

#ifdef __PLC__DEBUG__
                R_STDIO_Printf("\n -> Enter 8-bit media type (HEX)...");
#endif
                R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);
                pBlacklistTable->mediaType = hex_string_to_uint8(g_demo_buff.getStringBuffer);

                pBlacklistTable->validTime[1] = 0xFF; /* interim */

                R_DEMO_AdpmSetWrap(chId, R_ADP_IB_BLACKLIST_TABLE, index, tempArray);
                break;

            case 'c':
                R_memset(tempArray, 0, sizeof(r_adp_routing_table_t));
                for (i = 0; i < 16; i++)
                {
                    R_DEMO_AdpmSetWrap(chId, R_ADP_IB_ROUTING_TABLE, i, tempArray);
                }
                for (i = 0; i < 16; i++)
                {
                    R_DEMO_AdpmSetWrap(chId, R_ADP_IB_BLACKLIST_TABLE, i, tempArray);
                }

                break;

            case 'z':
                return;
            } /* switch */
        }
    } /* while */
} /* set_adp_table */
/******************************************************************************
   End of function  set_adp_table
******************************************************************************/

/******************************************************************************
* Function Name: R_DEMO_TestMenu
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_TestMenu (void)
{
    /* Configuration Menu */
    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\f-------------------Test Menu-----------------------");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 4 - Get ADP Table");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 5 - Set ADP Table");
#endif
        if ((R_MEDIA_TYPE0_PLC == g_demo_config.mediaType) || (R_MEDIA_TYPE2_BOTH == g_demo_config.mediaType))
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n p - disable PLC");
#endif
        if ((R_MEDIA_TYPE1_RF == g_demo_config.mediaType) || (R_MEDIA_TYPE2_BOTH == g_demo_config.mediaType))
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n r - disable RF");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n z - Return");
#endif

        R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

        if (strlen ((char *)g_demo_buff.getStringBuffer) == 1)
        {
            switch (g_demo_buff.getStringBuffer[0])
            {
                case '4':
                    get_adp_table (R_DEMO_G3_USE_PRIMARY_CH);
                    break;

                case '5':
                    set_adp_table (R_DEMO_G3_USE_PRIMARY_CH);
                    break;

                case 'p':
                    g_demo_config.plcDisabled ^= R_TRUE;
                    R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3MAC_IB_PLC_DISABLE, 0, &g_demo_config.plcDisabled);
#ifdef __PLC__DEBUG__
                    if (g_demo_config.plcDisabled)
                        R_STDIO_Printf("\n -> PLC disabled");
                    else
                        R_STDIO_Printf("\n -> PLC enabled");
#endif
                    break;

                case 'r':
                    g_demo_config.rfDisabled ^= R_TRUE;
                    R_DEMO_RfMlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3RFMAC_IB_DISABLE_PHY_RF, 0, &g_demo_config.rfDisabled);
#ifdef __PLC__DEBUG__                    
                    if (g_demo_config.rfDisabled)
                        R_STDIO_Printf("\n -> RF disabled");
                    else
                        R_STDIO_Printf("\n -> RF enabled");
#endif
                    break;
                
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
} /* menu_config */
/******************************************************************************
   End of function  R_DEMO_TestMenu
******************************************************************************/

/******************************************************************************
* Function Name: get_rfmac_pib_entry
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static r_result_t get_rfmac_pib_entry (uint8_t chId, const r_g3rfmac_ib_id_t attributeId)
{
    r_g3rfmac_mlme_get_req_t       mlmeGetReq;
    r_g3rfmac_mlme_get_cnf_t *     mlmeGetCfm;
    r_g3rfmac_mac_pos_table_t *    posTableElement;

    uint16_t                       i;
    uint16_t                       k = 3u;


    /* Set request structure. */
    mlmeGetReq.pibAttributeId = attributeId;
    mlmeGetReq.pibAttributeIndex = 0;

    switch (attributeId)
    {
        /* One byte elements. */
        case R_G3RFMAC_IB_PHY_TRANSMIT_POWER:
        case R_G3RFMAC_IB_PHY_RSSI_OUTPUT_OFFSET:
        case R_G3RFMAC_IB_PHY_CCA_BANDWIDTH:
        case R_G3RFMAC_IB_PHY_FSK_FEC_RX_ENA:
        case R_G3RFMAC_IB_PHY_FSK_FEC_TX_ENA:
        case R_G3RFMAC_IB_PHY_ANTENNA_SWITCH_ENA:
        case R_G3RFMAC_IB_PHY_ANTENNA_SELECT_TX:
        case R_G3RFMAC_IB_PHY_REGULATORY_MODE:
        case R_G3RFMAC_IB_PHY_AGC_WAIT_GAIN_OFFSET:
        case R_G3RFMAC_IB_PHY_CCA_VTH_OFFSET:
        case R_G3RFMAC_IB_PHY_GPIO0_SETTING:
        case R_G3RFMAC_IB_PROMISCUOUS_MODE:
        case R_G3RFMAC_IB_RX_ON_WHEN_IDLE:
        case R_G3RFMAC_IB_SECURITY_ENABLED:
        case R_G3RFMAC_IB_METRICSENABLED:
        case R_G3RFMAC_IB_RF_FRONTEND_GAIN:
        case R_G3RFMAC_IB_BEACON_RAND_WIN_LENGTH:
        case R_G3RFMAC_IB_DSN_RF:
        case R_G3RFMAC_IB_MAXBE_RF:
        case R_G3RFMAC_IB_MAXCSMABACKOFFS_RF:
        case R_G3RFMAC_IB_MAXFRAMERETRIES_RF:
        case R_G3RFMAC_IB_MINBE_RF:
        case R_G3RFMAC_IB_TIMESTAMPSUPPORTED_RF:
        case R_G3RFMAC_IB_DUPLICATEDETECTIONTTL_RF:
        case R_G3RFMAC_IB_EBRFILTER_RF:
        case R_G3RFMAC_IB_BEACONAUTORESPOND_RF:
        case R_G3RFMAC_IB_USEENHANCEDBEACON_RF:
        case R_G3RFMAC_IB_EBSN_RF:
        case R_G3RFMAC_IB_OPEMODE_RF:
        case R_G3RFMAC_IB_CHANNEL_NUMBER_RF:
        case R_G3RFMAC_IB_DUTY_CYCLE_USAGE_RF:
        case R_G3RFMAC_IB_DUTY_CYCLE_THRESHOLD_RF:
        case R_G3RFMAC_IB_DISABLE_PHY_RF:
        case R_G3RFMAC_IB_FREQBAND_RF:
        case R_G3RFMAC_IB_TRANSMITATTEN_RF:
        case R_G3RFMAC_IB_ADAPTIVEPOWERSTEP:
        case R_G3RFMAC_IB_ADAPTIVEPOWERHIGHBOUND:
        case R_G3RFMAC_IB_ADAPTIVEPOWERLOWBOUND:
        case R_G3RFMAC_IB_MAC_MAX_CCA_ATTEMPTS_RETRIES:
        case R_G3RFMAC_IB_POS_TABLE_ENTRY_TTL:
        case R_G3RFMAC_IB_KEY_STATE:
        case R_G3RFMAC_IB_MAC_G3CERTCOMPATIBILITY_VERSION:
            R_DEMO_RfMlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.2X", *(uint8_t *)(mlmeGetCfm->pibAttributeValue));
#endif
            break;

        /* Two byte elements. */
        case R_G3RFMAC_IB_PHY_CCA_DURATION:
        case R_G3RFMAC_IB_PHY_FSK_PREAMBLE_LENGTH:
        case R_G3RFMAC_IB_PHY_CCA_VTH:
        case R_G3RFMAC_IB_PHY_ANTENNA_SWITCH_ENA_TIMING:
        case R_G3RFMAC_IB_PHY_DATA_RATE:
        case R_G3RFMAC_IB_PHY_RMODET_ON_MAX:
        case R_G3RFMAC_IB_PHY_RMODET_OFF_MIN:
        case R_G3RFMAC_IB_PANID:
        case R_G3RFMAC_IB_SHORTADDRESS:
        case R_G3RFMAC_IB_RCCOORD:
        case R_G3RFMAC_IB_DUTY_CYCLE_PERIOD_RF:
        case R_G3RFMAC_IB_DUTY_CYCLE_LIMIT_RF:
        case R_G3RFMAC_IB_BEACON_RAND_WIN_LENGTH_RF:
        case R_G3RFMAC_IB_MAC_INITIAL_RETRY_TIME:
        case R_G3RFMAC_IB_MAC_MAXIMUM_RETRY_TIME:
            R_DEMO_RfMlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.4X", R_BYTE_ArrToUInt16 ((mlmeGetCfm->pibAttributeValue)));
#endif
            break;

        /* Four byte elements. */
        case R_G3RFMAC_IB_FRAMECNT_IND_INTERVAL:
        case R_G3RFMAC_IB_FRAMECOUNTER_RF:
        case R_G3RFMAC_IB_COUNTEROCTETS_RF:
        case R_G3RFMAC_IB_RETRYCOUNT_RF:
        case R_G3RFMAC_IB_MULTIPLERETRYCOUNT_RF:
        case R_G3RFMAC_IB_TXFAILCOUNT_RF:
        case R_G3RFMAC_IB_TXSUCCESSCOUNT_RF:
        case R_G3RFMAC_IB_FCSERRORCOUNT_RF:
        case R_G3RFMAC_IB_SECURITYFAILURE_RF:
        case R_G3RFMAC_IB_DUPLICATEFRAMECOUNT_RF:
        case R_G3RFMAC_IB_RXSUCCESSCOUNT_RF:
            R_DEMO_RfMlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.8X", R_BYTE_ArrToUInt32 ((mlmeGetCfm->pibAttributeValue)));
#endif
            break;

        /* Table elements. */
        case R_G3RFMAC_IB_POSTABLE_RF:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n --- Pos Table ---");
#endif

            /* Set request structure to read the elements. */
            mlmeGetReq.pibAttributeId = (uint16_t)R_G3RFMAC_IB_POSTABLE_RF;
            mlmeGetReq.pibAttributeIndex = 0;

            for (i = 0; i < k; i++)
            {
                /* For larger tables, show in parts. */
                if ((i > 0) &&
                    ((i % R_DEMO_APP_ENTRIES_PER_CYCLE) == 0))
                {
                    R_DEMO_HndWaitEnter();
                }

                R_DEMO_RfMlmeGet(chId, &mlmeGetReq, &mlmeGetCfm);

                posTableElement = (r_g3rfmac_mac_pos_table_t *)(mlmeGetCfm->pibAttributeValue);

                if ((R_G3RFMAC_STATUS_SUCCESS == mlmeGetCfm->status)&&(posTableElement->posValidTime))
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\nShortAddress: 0x%.4X \nposValidTime: 0x%.2X, \nfwdLQI: 0x%.2X, revLQI: 0x%.2X, \ndutyCycle: 0x%.2X, fwdTxPwrOfst: 0x%.2X, revTxPwrOfst: 0x%.2X\n\n",
                                    R_BYTE_ArrToUInt16 (posTableElement->shortAddress),
                                    posTableElement->posValidTime,
                                    posTableElement->fwdLqi, posTableElement->revLqi,
                                    posTableElement->dutyCycle, posTableElement->fwdTxPwrOfst, posTableElement->revTxPwrOfst
                                    );
#endif
                }
                else
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\n Entry not set.");
#endif
                }

                mlmeGetReq.pibAttributeIndex++;
            }
            break;

        case R_G3RFMAC_IB_DEVICETABLE_RF:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n --- Dev Table ---");
#endif

            /* Set request structure to read the elements. */
            mlmeGetReq.pibAttributeId = (uint16_t)R_G3RFMAC_IB_DEVICETABLE_RF;
            mlmeGetReq.pibAttributeIndex = 0;

            for (i = 0; i < k; i++)
            {
                /* For larger tables, show in parts. */
                if ((i > 0) &&
                    ((i % R_DEMO_APP_ENTRIES_PER_CYCLE) == 0))
                {
                    R_DEMO_HndWaitEnter();
                }

                R_DEMO_RfMlmeGet(chId, &mlmeGetReq, &mlmeGetCfm);

                if (R_G3RFMAC_STATUS_SUCCESS == mlmeGetCfm->status)
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\nindex: %d", mlmeGetCfm->pibAttributeIndex);
#endif
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\nshortAddress: 0x0x%02X%02X", mlmeGetCfm->pibAttributeValue[0], mlmeGetCfm->pibAttributeValue[1]);
#endif
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\nframeCounter : 0x0x%02X%02X%02X%02X", mlmeGetCfm->pibAttributeValue[2], mlmeGetCfm->pibAttributeValue[3], mlmeGetCfm->pibAttributeValue[4], mlmeGetCfm->pibAttributeValue[5]);
#endif
                }
                else
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\n Entry not set.");
#endif
                }

                    mlmeGetReq.pibAttributeIndex++;
            }
            break;

        case R_G3RFMAC_IB_EXTADDRESS:
            R_DEMO_RfMlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("Extended address: 0x%.2X", *(uint8_t *)(mlmeGetCfm->pibAttributeValue));
#endif
            for (i = 1; i < 8; i++)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("%.2X", *(uint8_t *)(mlmeGetCfm->pibAttributeValue + i));
#endif
            }
            break;

        case R_G3RFMAC_IB_SOFT_VERSION:
        {
            r_g3rfmac_soft_version_t * pAdpVer;
            R_DEMO_RfMlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);
            pAdpVer = (r_g3rfmac_soft_version_t *)mlmeGetCfm->pibAttributeValue;
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n --- RFMAC Version :0x%02X%02X%02X\r\n", pAdpVer->macVersion[0], pAdpVer->macVersion[1], pAdpVer->macVersion[2]);
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf (" --- PHY Version :0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n", pAdpVer->dspVersion[0], pAdpVer->dspVersion[1],pAdpVer->dspVersion[2], pAdpVer->dspVersion[3],pAdpVer->dspVersion[4], pAdpVer->dspVersion[5],pAdpVer->dspVersion[6], pAdpVer->dspVersion[7],pAdpVer->dspVersion[8], pAdpVer->dspVersion[9]);
#endif
            break;
        }

        case R_G3RFMAC_IB_DEVICE_TABLE_BY_SHORT_ADDR:
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 16-bit address (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.4X", hex_string_to_uint16 (g_demo_buff.getStringBuffer));
#endif
                mlmeGetReq.pibAttributeIndex = hex_string_to_uint16 (g_demo_buff.getStringBuffer);
            }
            else
            {
                return R_RESULT_FAILED;
            }

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\nDevice Table short address.");
#endif
            R_DEMO_RfMlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);

            if (R_G3RFMAC_STATUS_SUCCESS != mlmeGetCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nStatus:0x%02X\n", mlmeGetCfm->status);
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nindex: %d", mlmeGetCfm->pibAttributeIndex);
#endif
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nshortAddress: 0x0x%02X%02X", mlmeGetCfm->pibAttributeValue[0], mlmeGetCfm->pibAttributeValue[1]);
#endif
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nframeCounter : 0x0x%02X%02X%02X%02X", mlmeGetCfm->pibAttributeValue[2], mlmeGetCfm->pibAttributeValue[3], mlmeGetCfm->pibAttributeValue[4], mlmeGetCfm->pibAttributeValue[5]);
#endif
            }
            break;
        }
        case R_G3RFMAC_IB_POS_TABLE_BY_SHORT_ADDR:
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 16-bit address (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.4X", hex_string_to_uint16 (g_demo_buff.getStringBuffer));
#endif
                mlmeGetReq.pibAttributeIndex = hex_string_to_uint16 (g_demo_buff.getStringBuffer);
            }
            else
            {
                return R_RESULT_FAILED;
            }

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\nPos Table short address.");
#endif
            R_DEMO_RfMlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);

            if (R_G3RFMAC_STATUS_SUCCESS != mlmeGetCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nStatus:0x%02X\n", mlmeGetCfm->status);
#endif
            }
            else
            {
                posTableElement = (r_g3rfmac_mac_pos_table_t *)(mlmeGetCfm->pibAttributeValue);
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nShortAddress: 0x%.4X \nposValidTime: 0x%.2X, \nfwdLQI: 0x%.2X, revLQI: 0x%.2X, \ndutyCycle: 0x%.2X, fwdTxPwrOfst: 0x%.2X, revTxPwrOfst: 0x%.2X\n\n",
                                R_BYTE_ArrToUInt16 (posTableElement->shortAddress),
                                posTableElement->posValidTime,
                                posTableElement->fwdLqi, posTableElement->revLqi,
                                posTableElement->dutyCycle, posTableElement->fwdTxPwrOfst, posTableElement->revTxPwrOfst
                                );
#endif
            }
            break;
        }

        case R_G3RFMAC_IB_KEY_TABLE:
        case R_G3RFMAC_IB_RXFRAMECOUNTER_FLUSH:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\nRead access to attribute not supported by demo application.");
#endif
            break;

        default:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Unknown ID");
#endif
            R_DEMO_RfMlmeGet (chId, &mlmeGetReq, &mlmeGetCfm);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("0x%.8lX", R_BYTE_ArrToUInt32 ((mlmeGetCfm->pibAttributeValue)));
#endif
            break;

    } /* switch */

    return R_RESULT_SUCCESS;
} /* get_rfmac_pib_entry */
/******************************************************************************
   End of function  get_rfmac_pib_entry
******************************************************************************/


/******************************************************************************
* Function Name: set_rfmac_pib_entry
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static r_result_t set_rfmac_pib_entry (uint8_t chId, const r_g3rfmac_ib_id_t attributeId)
{
    uint16_t                 i = 0;
    r_g3rfmac_mlme_set_req_t   mlmeSetReq;
    r_g3rfmac_mlme_set_cnf_t * mlmeSetCfm;

    uint8_t                  tempArray[R_ADP_MAX_IB_SIZE];

    /* Set request structure. */
    mlmeSetReq.pibAttributeId = attributeId;
    mlmeSetReq.pibAttributeIndex = 0;
    mlmeSetReq.pibAttributeValue = tempArray;

    switch (attributeId)
    {
        /* One byte elements. */
        case R_G3RFMAC_IB_PHY_TRANSMIT_POWER:
        case R_G3RFMAC_IB_PHY_RSSI_OUTPUT_OFFSET:
        case R_G3RFMAC_IB_PHY_CCA_BANDWIDTH:
        case R_G3RFMAC_IB_PHY_FSK_FEC_RX_ENA:
        case R_G3RFMAC_IB_PHY_FSK_FEC_TX_ENA:
        case R_G3RFMAC_IB_PHY_ANTENNA_SWITCH_ENA:
        case R_G3RFMAC_IB_PHY_ANTENNA_SELECT_TX:
        case R_G3RFMAC_IB_PHY_REGULATORY_MODE:
        case R_G3RFMAC_IB_PHY_AGC_WAIT_GAIN_OFFSET:
        case R_G3RFMAC_IB_PHY_CCA_VTH_OFFSET:
        case R_G3RFMAC_IB_PHY_GPIO0_SETTING:
        case R_G3RFMAC_IB_PROMISCUOUS_MODE:
        case R_G3RFMAC_IB_RX_ON_WHEN_IDLE:
        case R_G3RFMAC_IB_METRICSENABLED:
        case R_G3RFMAC_IB_RXFRAMECOUNTER_FLUSH:
        case R_G3RFMAC_IB_RF_FRONTEND_GAIN:
        case R_G3RFMAC_IB_BEACON_RAND_WIN_LENGTH:
        case R_G3RFMAC_IB_DSN_RF:
        case R_G3RFMAC_IB_MAXBE_RF:
        case R_G3RFMAC_IB_MAXCSMABACKOFFS_RF:
        case R_G3RFMAC_IB_MAXFRAMERETRIES_RF:
        case R_G3RFMAC_IB_MINBE_RF:
        case R_G3RFMAC_IB_DUPLICATEDETECTIONTTL_RF:
        case R_G3RFMAC_IB_EBRFILTER_RF:
        case R_G3RFMAC_IB_BEACONAUTORESPOND_RF:
        case R_G3RFMAC_IB_USEENHANCEDBEACON_RF:
        case R_G3RFMAC_IB_EBSN_RF:
        case R_G3RFMAC_IB_OPEMODE_RF:
        case R_G3RFMAC_IB_CHANNEL_NUMBER_RF:
        case R_G3RFMAC_IB_DUTY_CYCLE_THRESHOLD_RF:
        case R_G3RFMAC_IB_DISABLE_PHY_RF:
        case R_G3RFMAC_IB_FREQBAND_RF:
        case R_G3RFMAC_IB_TRANSMITATTEN_RF:
        case R_G3RFMAC_IB_ADAPTIVEPOWERSTEP:
        case R_G3RFMAC_IB_ADAPTIVEPOWERHIGHBOUND:
        case R_G3RFMAC_IB_ADAPTIVEPOWERLOWBOUND:
        case R_G3RFMAC_IB_CONTINUOUS_TX_ENABLED_RF:
        case R_G3RFMAC_IB_MAC_MAX_CCA_ATTEMPTS_RETRIES:
        case R_G3RFMAC_IB_POS_TABLE_ENTRY_TTL:
        case R_G3RFMAC_IB_KEY_STATE:
        case R_G3RFMAC_IB_MAC_G3CERTCOMPATIBILITY_VERSION:

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 8-bit input (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 4)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.2X", hex_string_to_uint8 (g_demo_buff.getStringBuffer));
#endif

                mlmeSetReq.pibAttributeValue[0] = hex_string_to_uint8 (g_demo_buff.getStringBuffer);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            R_DEMO_RfMlmeSet (chId, &mlmeSetReq, &mlmeSetCfm);
            break;

        /* Two byte elements. */
        case R_G3RFMAC_IB_PHY_CCA_DURATION:
        case R_G3RFMAC_IB_PHY_FSK_PREAMBLE_LENGTH:
        case R_G3RFMAC_IB_PHY_CCA_VTH:
        case R_G3RFMAC_IB_PHY_ANTENNA_SWITCH_ENA_TIMING:
        case R_G3RFMAC_IB_PHY_RMODET_ON_MAX:
        case R_G3RFMAC_IB_PHY_RMODET_OFF_MIN:
        case R_G3RFMAC_IB_PANID:
        case R_G3RFMAC_IB_SHORTADDRESS:
        case R_G3RFMAC_IB_RCCOORD:
        case R_G3RFMAC_IB_DUTY_CYCLE_PERIOD_RF:
        case R_G3RFMAC_IB_DUTY_CYCLE_LIMIT_RF:
        case R_G3RFMAC_IB_BEACON_RAND_WIN_LENGTH_RF:
        case R_G3RFMAC_IB_MAC_INITIAL_RETRY_TIME:
        case R_G3RFMAC_IB_MAC_MAXIMUM_RETRY_TIME:

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 16-bit input (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.4X", hex_string_to_uint16 (g_demo_buff.getStringBuffer));
#endif

                R_BYTE_UInt16ToArr (hex_string_to_uint16 (g_demo_buff.getStringBuffer), mlmeSetReq.pibAttributeValue);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            R_DEMO_RfMlmeSet (chId, &mlmeSetReq, &mlmeSetCfm);
            break;

        /* Four byte elements. */
        case R_G3RFMAC_IB_FRAMECNT_IND_INTERVAL:
        case R_G3RFMAC_IB_FRAMECOUNTER_RF:

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 32-bit input (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 10)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.8lX", hex_string_to_uint32 (g_demo_buff.getStringBuffer));
#endif

                R_BYTE_UInt32ToArr (hex_string_to_uint32 (g_demo_buff.getStringBuffer), mlmeSetReq.pibAttributeValue);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            R_DEMO_RfMlmeSet (chId, &mlmeSetReq, &mlmeSetCfm);
            break;

        /* Table elements. */
        case R_G3RFMAC_IB_POSTABLE_RF:
        case R_G3RFMAC_IB_DEVICETABLE_RF:
        case R_G3RFMAC_IB_POS_TABLE_BY_SHORT_ADDR:
        case R_G3RFMAC_IB_DEVICE_TABLE_BY_SHORT_ADDR:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 16-bit address (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 6)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.4X", hex_string_to_uint16 (g_demo_buff.getStringBuffer));
#endif
                mlmeSetReq.pibAttributeIndex = hex_string_to_uint16 (g_demo_buff.getStringBuffer);
            }
            else
            {
                return R_RESULT_FAILED;
            }

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 32-bit input (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 10)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.8lX", hex_string_to_uint32 (g_demo_buff.getStringBuffer));
#endif

                R_BYTE_UInt32ToArr (hex_string_to_uint32 (g_demo_buff.getStringBuffer), mlmeSetReq.pibAttributeValue);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            R_DEMO_RfMlmeSet (chId, &mlmeSetReq, &mlmeSetCfm);
            break;

        case R_G3RFMAC_IB_KEY_TABLE:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 8-bit key index (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 4)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.2X", hex_string_to_uint8 (g_demo_buff.getStringBuffer));
#endif

                mlmeSetReq.pibAttributeValue[0] = hex_string_to_uint8 (g_demo_buff.getStringBuffer);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }

#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 128-bit key (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 34)
            {
                hex_string_128bit_to_uint8_arr(g_demo_buff.getStringBuffer, mlmeSetReq.pibAttributeValue);
                for (i = 0; i < 16; i++)
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("%.2X", *(uint8_t *)(mlmeSetReq.pibAttributeValue + i));
#endif
                }
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            R_DEMO_RfMlmeSet (chId, &mlmeSetReq, &mlmeSetCfm);
            break;

        case R_G3RFMAC_IB_EXTADDRESS:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n -> Enter 64-bit address (HEX)...");
#endif

            R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

            if (strlen ((char *)g_demo_buff.getStringBuffer) == 18)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("0x%.16lX%", hex_string_to_uint64 (g_demo_buff.getStringBuffer));
#endif

                R_BYTE_UInt64ToArr (hex_string_to_uint64 (g_demo_buff.getStringBuffer), mlmeSetReq.pibAttributeValue);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            R_DEMO_RfMlmeSet (chId, &mlmeSetReq, &mlmeSetCfm);
            break;

        case R_G3RFMAC_IB_PHY_DATA_RATE:
        case R_G3RFMAC_IB_TIMESTAMPSUPPORTED_RF:
        case R_G3RFMAC_IB_SECURITY_ENABLED:
        case R_G3RFMAC_IB_COUNTEROCTETS_RF:
        case R_G3RFMAC_IB_RETRYCOUNT_RF:
        case R_G3RFMAC_IB_MULTIPLERETRYCOUNT_RF:
        case R_G3RFMAC_IB_TXFAILCOUNT_RF:
        case R_G3RFMAC_IB_TXSUCCESSCOUNT_RF:
        case R_G3RFMAC_IB_FCSERRORCOUNT_RF:
        case R_G3RFMAC_IB_SECURITYFAILURE_RF:
        case R_G3RFMAC_IB_DUPLICATEFRAMECOUNT_RF:
        case R_G3RFMAC_IB_RXSUCCESSCOUNT_RF:
        case R_G3RFMAC_IB_DUTY_CYCLE_USAGE_RF:
        case R_G3RFMAC_IB_SOFT_VERSION:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\nWrite access to attribute not supported by demo application.");
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
                R_STDIO_Printf ("0x%.8lX", hex_string_to_uint32 (g_demo_buff.getStringBuffer));
#endif

                R_BYTE_UInt32ToArr (hex_string_to_uint32 (g_demo_buff.getStringBuffer), mlmeSetReq.pibAttributeValue);
            }
            else
            {
                return R_RESULT_SUCCESS;
            }
            R_DEMO_RfMlmeSet (chId, &mlmeSetReq, &mlmeSetCfm);
            break;

    } /* switch */

    return R_RESULT_SUCCESS;
} /* set_rfmac_pib_entry */
/******************************************************************************
   End of function  set_rfmac_pib_entry
******************************************************************************/
#endif /* R_DEFINE_DISABLE_HYBRID R_DEFINE_DISABLE_APP_HYBRID */


