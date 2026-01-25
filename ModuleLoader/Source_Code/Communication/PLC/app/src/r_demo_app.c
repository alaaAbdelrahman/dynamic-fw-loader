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
 * File Name   : r_demo_app.c
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
#include <stdlib.h>
#include <string.h>
#include "r_typedefs.h"
#include "r_stdio_api.h"
#if !defined(R_DEFINE_OPEN_RX)
#include "r_config.h"
#include "r_bsp_api.h"
#endif
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
#include "r_demo_config.h"
#include "r_demo_statistics.h"
#include "r_demo_ib_access.h"
#include "r_demo_app_ver.h"
#include "r_demo_parameters.h"
#include "r_demo_os_wrapper.h"

//---------------------------------
extern uint8_t cpx3_current_task;
uint8_t current_AppResetTask = CPX3_ADP_SET_CONFIG;
uint8_t config_monitor = 0;

extern volatile void *current_config_flag;
extern uint32_t current_config_id;
extern uint8_t current_AppSetIpContent;
uint8_t get_handle_state = 1;
extern uint16_t DMA_The_last_Index;
extern uint8_t DMA_Buffer[1536];

uint8_t start_config_set = 0;

// extern r_adp_adpm_discovery_cnf_t *disCfm;

// r_adp_adpm_discovery_req_t disReq;
// r_adp_adpm_discovery_cnf_t *disCfm;
uint16_t NetworkCount = 0;
// extern uint32_t g3ctrl_signal;

//---------------------------------

// extern uint16_t iec_62056_21_isr_index;
// extern uint8_t iec_62056_21_isr_buffer[500];
// uint32_t rcv_packet_cnt = 0;
/******************************************************************************
Macro definitions
******************************************************************************/
/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

/*!
   \fn static r_result_t menu_route_discovery(void);
   \brief  Menu for route discovery
 */
static r_result_t menu_route_discovery(void);

/*!
   \fn static r_result_t menu_adpd_data(void);
   \brief  Menu for data sending
 */
static r_result_t menu_adpd_data(void);

/*!
   \fn static r_result_t menu_path_discovery(void);
   \brief  Menu for path discovery
 */
static r_result_t menu_path_discovery(void);

static r_result_t menu_bandplan(void);
static r_result_t menu_flash(void);
static r_result_t menu_network_start(uint16_t panId);
static r_result_t menu_dump_data(void);

/******************************************************************************
Exported global variables
******************************************************************************/
extern const r_demo_config_t g_cdemo_config;
extern r_demo_config_t g_demo_config;
extern r_demo_entity_t g_demo_entity;
extern r_demo_buff_t g_demo_buff;

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Functions
******************************************************************************/

/*===========================================================================*/
/*    G3MAC callback                                                        */
/*===========================================================================*/
/******************************************************************************
 * Function Name: R_DEMO_AppHandleMcpsDataInd
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_AppHandleMcpsDataInd(const r_g3mac_mcps_data_ind_t *ind)
{
    if (R_TRUE == g_demo_config.verboseEnabled)
    {

        uint8_t *pAddr;
        uint32_t sz;
        uint16_t i;

#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> McpsData Ind (ch[%d])...", 1);
#endif

#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n    srcAddrMode=%X srcPanId=0x%04X srcAddr=",
                       ind->srcAddrMode, ind->srcPanId);
#endif
        pAddr = (uint8_t *)ind->srcAddr;
        if (R_G3MAC_ADDR_MODE_EXTENDED == ind->srcAddrMode)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("0x%02X%02X%02X%02X%02X%02X%02X%02X\n",
                           pAddr[0], pAddr[1], pAddr[2], pAddr[3], pAddr[4], pAddr[5], pAddr[6], pAddr[7]);
#endif
        }
        else
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("0x%02X%02X\n", pAddr[0], pAddr[1]);
#endif
        }

#ifdef __PLC__DEBUG__
        R_STDIO_Printf("    dstAddrMode=%X dstPanId=0x%04X dstAddr=",
                       ind->dstAddrMode, ind->dstPanId);
#endif
        pAddr = (uint8_t *)ind->dstAddr;
        if (R_G3MAC_ADDR_MODE_EXTENDED == ind->dstAddrMode)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("0x%02X%02X%02X%02X%02X%02X%02X%02X\n",

                           pAddr[0], pAddr[1], pAddr[2], pAddr[3], pAddr[4], pAddr[5], pAddr[6], pAddr[7]);
#endif
        }
        else
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("0x%02X%02X\n", pAddr[0], pAddr[1]);
#endif
        }

#ifdef __PLC__DEBUG__
        R_STDIO_Printf("    msduLength=0x%03X(%dB)\n    msdu=", ind->msduLength, ind->msduLength);
#endif

        sz = (ind->msduLength < R_DEMO_APP_MSDU_PRINT_MAXLEN) ? ind->msduLength : R_DEMO_APP_MSDU_PRINT_MAXLEN;

        for (i = 0; i < sz; i++)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("%02X", ind->pMsdu[i]);
#endif
        }
        if (sz < ind->msduLength)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("...");
#endif
        }
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n    msduLinkQuality=0x%02X SecurityLevel=%X\n",
                       ind->msduLinkQuality, ind->securityLevel);
#endif
    }
} /* R_DEMO_AppHandleMcpsDataInd */
/******************************************************************************
   End of function  R_DEMO_AppHandleMcpsDataInd
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AppHandleMacTmrRcvInd
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_AppHandleMacTmrRcvInd(const r_g3mac_mlme_tmr_receive_ind_t *ind)
{
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Received TMR Receive indication with srcAddr=0x%.2X%.2X ", ind->srcAddr[0], ind->srcAddr[1]);
#endif
    }
}
/******************************************************************************
   End of function  R_DEMO_AppHandleMacTmrRcvInd
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AppHandleMacTmrTransmitInd
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_AppHandleMacTmrTransmitInd(const r_g3mac_mlme_tmr_transmit_ind_t *ind)
{
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
        uint8_t *pAddr = (uint8_t *)ind->dstAddr;
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Received TMR Transmit indication with dstAddrMode=%d dstAddr=", ind->dstAddrMode);
#endif
        if (R_G3MAC_ADDR_MODE_EXTENDED == ind->dstAddrMode)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("0x%02X%02X%02X%02X%02X%02X%02X%02X\n",
                           pAddr[0], pAddr[1], pAddr[2], pAddr[3], pAddr[4], pAddr[5], pAddr[6], pAddr[7]);
#endif
        }
        else
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("0x%02X%02X\n", pAddr[0], pAddr[1]);
#endif
        }
#ifdef __PLC__DEBUG__
        R_STDIO_Printf(" pms=%d mod=%d", ind->pms, ind->mod);
#endif
    }
} /* R_DEMO_AppHandleMacTmrTransmitInd */
/******************************************************************************
   End of function  R_DEMO_AppHandleMacTmrTransmitInd
******************************************************************************/

/*===========================================================================*/
/*    ADP callback                                                           */
/*===========================================================================*/
/******************************************************************************
 * Function Name: R_DEMO_AppHandleBufferInd
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_AppHandleBufferInd(const r_adp_adpm_buffer_ind_t *ind)
{
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Received buffer indication with status: 0x%.2X", ind->bufferReady);
#endif
    }
}
/******************************************************************************
   End of function  R_DEMO_AppHandleBufferInd
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AppHandleKeyStateInd
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_AppHandleKeyStateInd(const r_adp_adpm_key_state_ind_t *ind)
{
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Key state indication index %d with status: 0x%.2X", ind->keyIndex, ind->keyState);
#endif
    }
}
/******************************************************************************
   End of function  R_DEMO_AppHandleKeyStateInd
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AppHandleStatusInd
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_AppHandleStatusInd(const r_adp_adpm_network_status_ind_t *ind)
{
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Received nwk status ind from mediaType:%d with status: %s(0x%.2X)", ind->mediaType, R_G3_Status2Text(R_G3_INFO_LAYER_ADP, ind->status), ind->status);
#endif
#else
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Received network status indication with status: %s(0x%.2X)", R_G3_Status2Text(R_G3_INFO_LAYER_ADP, ind->status), ind->status);
#endif
#endif
    }
}
/******************************************************************************
   End of function  R_DEMO_AppHandleStatusInd
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AppHandlePathDiscInd
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_AppHandlePathDiscInd(const r_adp_adpm_path_discovery_ind_t *ind)
{
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Received PREQ indication with originator address: 0x%.4X", ind->origAddr);
#endif
    }
}
/******************************************************************************
   End of function  R_DEMO_AppHandlePathDiscInd
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AppHandleFrameCntInd
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_AppHandleFrameCntInd(r_adp_adpm_framecounter_ind_t *ind)
{ // volatile static uint32_t value;
    // value = 256;
    // Delay_ms(15000);
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Received FrameCounter indication with counter value: 0x%.8lX mediaType:%d", ind->frameCounter, ind->mediaType);
#endif
#else
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Received FrameCounter indication with counter value: 0x%.8lX", 256);
#endif
#endif
    }
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    R_DEMO_AppPreserveProcess(ind->frameCounter, ind->mediaType);
#else
    R_DEMO_AppPreserveProcess(ind->frameCounter); // test case
#endif
}
/******************************************************************************
   End of function  R_DEMO_AppHandleFrameCntInd
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AppHandleRouteInd
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_AppHandleRouteInd(const r_adp_adpm_route_update_ind_t *ind)
{
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Received Route Update indication with dstAddr=0x%.2X%.2X", ind->dstAddr[0], ind->dstAddr[1]);
#endif
    }
}
/******************************************************************************
   End of function  R_DEMO_AppHandleRouteInd
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AppHandleLoadInd
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_AppHandleLoadInd(const r_adp_adpm_load_seq_num_ind_t *ind)
{
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Received Load Sequence Number indication with adpLoadSeqNumber=0x%.2X%.2X", ind->adpLoadSeqNumber[0], ind->adpLoadSeqNumber[1]);
#endif
    }
}
/******************************************************************************
   End of function  R_DEMO_AppHandleLoadInd
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AppHandleRrepInd
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_AppHandleRrepInd(const r_adp_adpm_rrep_ind_t *ind)
{
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Received Rrep indication with rrepOriginator=0x%.2X%.2X rrepDestination=0x%.2X%.2X", ind->rrepOriginator[0], ind->rrepOriginator[1], ind->rrepDestination[0], ind->rrepDestination[1]);
#endif
    }
}
/******************************************************************************
   End of function  R_DEMO_AppHandleRrepInd
******************************************************************************/

///******************************************************************************
//* Function Name: R_DEMO_AppHandleDataIndication
//* Description :
//* Arguments :
//* Return Value :
//******************************************************************************/
// void R_DEMO_AppHandleDataIndication (const r_adp_adpd_data_ind_t * ind)
//{
//    uint32_t     i;
//    r_ipv6_hdr_t ipv6Hdr;
//
//    if (R_TRUE == g_demo_config.verboseEnabled)
//    {
// #ifdef __PLC__DEBUG__
//        //R_STDIO_Printf ("\n -> Received data frame (length: %d, quality: %d)", (ind->nsduLength) - 48, ind->linkQualityIndicator);
// #endif
//
// #ifdef __PLC__DEBUG__
//        //R_STDIO_Printf ("\n------- Received NSDU ------\n");
// #endif
//        //Delay_ms(10);
//        for (i = 0; i < (ind->nsduLength) - 48; i++)
//        {
// #ifdef __PLC__DEBUG__
//            //R_STDIO_Printf ("%.2X", ind->pNsdu[i+48]);
// #endif
//            iec_62056_21_rx_char_isr(ind->pNsdu[i + 48]);
//        }
//        //rcv_packet_cnt++;
////        memcpy(&iec_62056_21_isr_buffer[iec_62056_21_isr_index],&ind->pNsdu[48],(ind->nsduLength) - 48);
////        iec_62056_21_isr_index += (ind->nsduLength) - 48;
////
////        if(iec_62056_21_isr_index >= sizeof(iec_62056_21_isr_buffer))
////        {
////          iec_62056_21_isr_index = 0;
////        }
//        //IEC_62056_21_CLR_INT_FLG();
//    }
//
//    if (R_IPV6_UnpackHeader (ind->pNsdu,
//                             &ipv6Hdr) == R_RESULT_SUCCESS)
//    {
//        if (R_IPV6_NEXT_HDR_ICMPV6 == ipv6Hdr.nextHdr)
//        {
//            R_DEMO_ReplyIcmpRequest (ind->pNsdu,
//                                     &ipv6Hdr);
//        }
//        else if (R_IPV6_NEXT_HDR_UDP == ipv6Hdr.nextHdr)
//        {
//            R_DEMO_ReplyUdpFrame (ind->pNsdu,
//                                  &ipv6Hdr);
//
//        }
//        else if (R_IPV6_NEXT_HDR_HOP_BY_HOP == ipv6Hdr.nextHdr)
//        {
//            R_DEMO_ReplyIcmpRequestExtHeaders (ind->pNsdu,
//                                               &ipv6Hdr);
//        }
//        else
//        {
//            /* Do nothing. */
//        }
//    }
//
//} /* R_DEMO_AppHandleDataIndication */
///******************************************************************************
//   End of function  R_DEMO_AppHandleDataIndication
//******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AppHandleLeaveIndication
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_AppHandleLeaveIndication(void)
{
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Device has been kicked from the network. Resetting.");
#endif
    }

    R_DEMO_LED(R_DEMO_G3_USE_PRIMARY_CH, R_DEMO_LED_BOOT);

    /* Call ADPM reset. Depending on short address assignment policiy
       of the coordinator, macFrameCounter should be restored afterwards. */
    R_DEMO_AppResetDevice();

    /* Start network join after being kicked */
    // R_DEMO_AppJoinNetwork();

} /* R_DEMO_AppHandleLeaveIndication */
/******************************************************************************
   End of function  R_DEMO_AppHandleLeaveIndication
******************************************************************************/

/*===========================================================================*/
/*    APP                                                                    */
/*===========================================================================*/
/******************************************************************************
 * Function Name: R_DEMO_AppMainMenuProcSAP
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
static void R_DEMO_AppMainMenuProcSAP(void)
{
    /* Device Main Menu */
    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\f----------------------ADP Common Service---------------------");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 1 - Send data frame");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 2 - Start route discovery");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 3 - Start path discovery");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 4 - Scan for devices (Discovery)");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 5 - Configuration (IB)");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 6 - Reset ADP");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 7 - Statistics/Log");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 8 - Toggle verbose mode");
#endif
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 9 - test mode");
#endif
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 0 - Data Flash Menu");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n d - Dump request");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n z - Return ");
#endif

        R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

        if (strlen((char *)g_demo_buff.getStringBuffer) == 1)
        {
            switch (g_demo_buff.getStringBuffer[0])
            {
            case '0':
                menu_flash();
                break;

            case '1':
                menu_adpd_data();
                R_DEMO_HndWaitEnter();
                break;

            case '2':
                menu_route_discovery();
                R_DEMO_HndWaitEnter();
                break;

            case '3':
                menu_path_discovery();
                R_DEMO_HndWaitEnter();
                break;

            case '4':
                //  R_DEMO_AppNetworkDiscovery(R_SCAN_DURATION); FORCED COMMENT
                R_DEMO_HndWaitEnter();
                break;

            case '5':
                R_DEMO_MenuConfig();
                break;

            case '6':

                /* Call ADPM reset. Depending on short address assignment policiy
                   of the coordinator, macFrameCounter should be restored afterwards. */
                R_DEMO_AppResetDevice();
                R_DEMO_HndWaitEnter();
                break;

            case '7':
                R_DEMO_MenuStatistics();
                break;

            case '8':

                /* Toggle verbose mode */
                g_demo_config.verboseEnabled ^= 1;
                break;
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
            case '9':
                R_DEMO_TestMenu();
                break;
#endif
            case 'd':
                menu_dump_data();
                R_DEMO_HndWaitEnter();
                break;

            case 'z':
                return;

            default:
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("\n\n Invalid option! \n");
#endif
                break;
            } /* switch */
        }
        else
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n\n Invalid option! \n");
#endif
        }
    }
} /* R_DEMO_AppMainMenuProcSAP */
/******************************************************************************
   End of function  R_DEMO_AppMainMenuProcSAP
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AppMainMenuProc
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_AppMainMenuProc(void)
{
    r_adp_adpm_network_leave_cnf_t *nwlCfm;

    /* Device Main Menu */
    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n\f----------------------Peer Main Menu---------------------");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n---PAN ID: 0x%.4X --- ShortAddr: 0x%.4X --- Verbose:", g_demo_entity.panId, g_demo_entity.shortAddress);
#endif
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf(" On ---");
#endif
        }
        else
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf(" Off --");
#endif
        }
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
        r_demo_print_mediatype(g_demo_config.mediaType);
#endif
        r_demo_print_bandplan(g_demo_config.bandPlan);
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 1 - Re-Join a network(with discovery) after SAP-Reset");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 2 - Leave a network");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 4 - Toggle Mac promiscuous mode");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 7 - G3 common SAP");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 8 - Toggle verbose mode");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n s - SYS Menu");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 0 - Data Flash Menu");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n z - Return");
#endif

        R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

        if (strlen((char *)g_demo_buff.getStringBuffer) == 1)
        {
            switch (g_demo_buff.getStringBuffer[0])
            {
            case '0':
                menu_flash();
                break;

            case '1':

                /* Reset device. */
                if (R_DEMO_AppResetDevice() != R_RESULT_SUCCESS)
                {
                    R_DEMO_HndFatalError();
                }
                //  R_DEMO_AppJoinNetwork();

                R_DEMO_HndWaitEnter();
                break;

            case '2':
                if ((R_DEMO_AdpmNetworkLeave(R_DEMO_G3_USE_PRIMARY_CH, &nwlCfm) == R_RESULT_SUCCESS) &&
                    (R_ADP_STATUS_SUCCESS == nwlCfm->status))
                {
                    /* Call ADPM reset. Depending on short address assignment policiy
                       of the coordinator, macFrameCounter should be restored afterwards. */
                    R_DEMO_AppResetDevice();
                }

                R_DEMO_HndWaitEnter();
                break;

#ifndef R_DEFINE_DISABLE_UMAC
            case '4':
                g_demo_config.macPromiscuousEnabled ^= 1;
                R_DEMO_AppToggleMacPromiscuous(g_demo_config.macPromiscuousEnabled);
                break;
#endif
            case '7':
                R_DEMO_AppMainMenuProcSAP();
                R_DEMO_HndWaitEnter();
                break;

            case '8':

                /* Toggle verbose mode */
                g_demo_config.verboseEnabled ^= 1;
                break;

            case 's':
                R_DEMO_AppSysMenu();
                break;

            case 'z':
                return;

            default:
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("\n\n Invalid option! \n");
#endif
                break;
            } /* switch */
        }
        else
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n\n Invalid option! \n");
#endif
        }
    }
} /* R_DEMO_AppMainMenuProc */
/******************************************************************************
   End of function  R_DEMO_AppMainMenuProc
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AppMainMenuProcLbs
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_AppMainMenuProcLbs(void)
{

    while (1)
    {

#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n\f------------------Coordinator Main Menu-----------------------");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n---PAN ID: 0x%.4X --- ShortAddr: 0x%.4X --- Verbose:", g_demo_entity.panId, g_demo_entity.shortAddress);
#endif
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf(" On --------");
#endif
        }
        else
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf(" Off -------");
#endif
        }
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
        r_demo_print_mediatype(g_demo_config.mediaType);
#endif
        r_demo_print_bandplan(g_demo_config.bandPlan);
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 1 - Re-Start Network after SAP-Reset");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 2 - Show connected devices");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 3 - Kick device");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 4 - Toggle Mac promiscuous mode");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 5 - GMK removal");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 7 - G3 common SAP");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 8 - Toggle verbose mode");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n s - SYS Menu");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 0 - Data Flash Menu");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n z - Return");
#endif

        R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

        if (strlen((char *)g_demo_buff.getStringBuffer) == 1)
        {
            switch (g_demo_buff.getStringBuffer[0])
            {
            case '0':
                menu_flash();
                break;

            case '1':

                /* Reset device. */
                if (R_DEMO_AppResetDevice() != R_RESULT_SUCCESS)
                {
                    R_DEMO_HndFatalError();
                }

                menu_network_start(g_demo_config.panId);
                R_DEMO_HndWaitEnter();

                break;

            case '2':

                /* Show device list */
                R_LBS_ShowDeviceList();

                R_DEMO_HndWaitEnter();
                break;

            case '3':
                R_LBS_ShowDeviceList();
                while (1)
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf("\n -> Select device to kick No (0-F)...");
#endif

                    R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

                    if (strlen((char *)g_demo_buff.getStringBuffer) == 1)
                    {
                        R_LBS_KickDeviceNo(hex_string_to_uint8(g_demo_buff.getStringBuffer));
                        break;
                    }
                }
                R_DEMO_HndWaitEnter();
                break;

            case '5':
                R_LBS_ShowDeviceList();
                while (1)
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf("\n -> Select device to keyRemove No (0-F)...");
#endif

                    R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

                    if (strlen((char *)g_demo_buff.getStringBuffer) == 1)
                    {
                        uint8_t dev_no = hex_string_to_uint8(g_demo_buff.getStringBuffer);
                        while (1)
                        {
#ifdef __PLC__DEBUG__
                            R_STDIO_Printf("\n -> Select Remove Key inddex (0 or 1)...");
#endif

                            R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

                            if (strlen((char *)g_demo_buff.getStringBuffer) == 1)
                            {
                                R_LBS_RemoveKeyDev(dev_no, hex_string_to_uint8(g_demo_buff.getStringBuffer));
                                break;
                            }
                        }
                        break;
                    }
                }
                R_DEMO_HndWaitEnter();
                break;

#ifndef R_DEFINE_DISABLE_UMAC

            case '4':
                g_demo_config.macPromiscuousEnabled ^= 1;
                R_DEMO_AppToggleMacPromiscuous(g_demo_config.macPromiscuousEnabled);
                break;
#endif
            case '7':
                R_DEMO_AppMainMenuProcSAP();
                R_DEMO_HndWaitEnter();
                break;
#ifdef R_DEFINE_ENABLE_TEST_MODE
            case '9':
            {
                r_eap_clientinfo_table_t Tbl = {0};
                uint8_t eui[] = {0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD};
                Tbl.access_ctrl = R_TRUE;
                R_memcpy(Tbl.extended_address, eui, 8);
                R_BYTE_UInt16ToArr(0x0101, Tbl.network_address);
                Tbl.valid_state = R_DEMO_CSTATE_VALID;

                R_DEMO_EapmSetClientinfoWrap(R_DEMO_G3_USE_PRIMARY_CH, &Tbl);
            }
            break;
#endif

            case '8':

                /* Toggle verbose mode */
                g_demo_config.verboseEnabled ^= 1;
                break;

            case 's':
                R_DEMO_AppSysMenu();
                break;

            case 'z':
                return;

            default:
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("\n\n Invalid option! \n");
#endif
                break;
            } /* switch */
        }
        else
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n\n Invalid option! \n");
#endif
        }
    }
} /* R_DEMO_AppMainMenuProcLbs */
/******************************************************************************
   End of function  R_DEMO_AppMainMenuProcLbs
******************************************************************************/
/******************************************************************************
 * Function Name: R_DEMO_AppMainMenu
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_AppMainMenu(void)
{
    r_demo_mode_t pdemoConfig;

    while (1)
    {
        /* Printf demo version. */
        r_demo_print_version();

        /* Initialize data flash if not yet done. */
        r_demo_nvm_config_init(R_FALSE, &g_cdemo_config);

        /* Initialize g_demo_config */
        r_demo_et_read_demo_mode((uint8_t *)&pdemoConfig);
        R_memcpy(&g_demo_config.tonemask[0], (uint8_t *)&pdemoConfig.tonemask[0], 9);

        while (1)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n-------------------Start menu ------------------------");
#endif
            r_demo_print_bandplan(g_demo_config.bandPlan);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n 1 - Demo Application");
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n 4 - Toggle Mac Promiscuous Mode");
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n 7 - Toggle Verbose Mode");
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n 8 - Change PLC BandPlan");
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n s - SYS Menu");
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n 0 - Data Flash Menu");
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n z - Return");
#endif

            R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

            if (strlen((char *)g_demo_buff.getStringBuffer) == 1)
            {
                if ('1' == g_demo_buff.getStringBuffer[0])
                {
                    g_demo_config.appMode = R_DEMO_MODE_AUTO;
                    break;
                }
#ifndef R_DEFINE_DISABLE_UMAC
                else if ('4' == g_demo_buff.getStringBuffer[0])
                {
                    g_demo_config.macPromiscuousEnabled ^= 1;
                    R_DEMO_AppToggleMacPromiscuous(g_demo_config.macPromiscuousEnabled);
                    return;
                }
#endif
                else if ('7' == g_demo_buff.getStringBuffer[0])
                {
                    g_demo_config.verboseEnabled ^= 1;
                    return;
                }
                else if ('8' == g_demo_buff.getStringBuffer[0])
                {
                    menu_bandplan();
                    return;
                }
                else if ('s' == g_demo_buff.getStringBuffer[0])
                {
                    R_DEMO_AppSysMenu();
                    return;
                }
                else if ('z' == g_demo_buff.getStringBuffer[0])
                {
                    return;
                }
                else if ('0' == g_demo_buff.getStringBuffer[0])
                {
                    menu_flash();
                    return;
                }
                else
                {
                    /**/
                }
            }
        }
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
        /* Set media type */
        while (1)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n-------------------Set media type------------------------");
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n 0 - PLC only");
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n 1 - RF Only");
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n 2 - Hybrid(PLC/RF) (default)");
#endif

            R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

            if (strlen((char *)g_demo_buff.getStringBuffer) == 1)
            {
                if ('0' == g_demo_buff.getStringBuffer[0])
                {
                    g_demo_config.mediaType = R_MEDIA_TYPE0_PLC;
                    break;
                }
                else if ('1' == g_demo_buff.getStringBuffer[0])
                {
                    g_demo_config.mediaType = R_MEDIA_TYPE1_RF;
                    break;
                }
                else
                {
                    /**/
                }
            }
            g_demo_config.mediaType = R_MEDIA_TYPE2_BOTH;
            break;
        }

#endif
        /* Set device type */
        while (1)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n-------------------Set device type------------------------");
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n 1 - Peer");
#endif
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("\n 2 - Coordinator");
#endif

            R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

            if (strlen((char *)g_demo_buff.getStringBuffer) == 1)
            {
                if ('1' == g_demo_buff.getStringBuffer[0])
                {
                    g_demo_config.devType = R_ADP_DEVICE_TYPE_PEER;
                    break;
                }
                else if ('2' == g_demo_buff.getStringBuffer[0])
                {
                    g_demo_config.devType = R_ADP_DEVICE_TYPE_COORDINATOR;
                    break;
                }
                else
                {
                    /**/
                }
            }
        }

        /* Enter demo application */
        R_DEMO_App();
    }
} /* R_DEMO_AppMainMenu */
/******************************************************************************
   End of function  R_DEMO_AppMainMenu
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AppNetworkDiscovery
 * Description :
 * Arguments : duretion of discovery
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_AppNetworkDiscovery(uint8_t duration)
{
    r_adp_adpm_discovery_req_t disReq;
     r_adp_adpm_discovery_cnf_t *disCfm;
    r_result_t state = R_RESULT_SUCCESS;
    /* Set scan duration (default randomization time plus one). */
    disReq.duration = duration;
    state = R_DEMO_AdpmDiscovery(R_DEMO_G3_USE_PRIMARY_CH, &disReq, &disCfm);
    return state;
    /* Call Discovery function. */
    // if (R_DEMO_AdpmDiscovery(R_DEMO_G3_USE_PRIMARY_CH, &disReq, &disCfm) == R_RESULT_SUCCESS)
    // {
    //     return disCfm->PANCount;
    // }
    // else
    // {
    //     return 0;
    // }
} /* R_DEMO_AppNetworkDiscovery */
/******************************************************************************
   End of function  R_DEMO_AppNetworkDiscovery
******************************************************************************/

/******************************************************************************
 * Function Name: menu_route_discovery
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
static r_result_t menu_route_discovery(void)
{
    r_adp_adpm_route_disc_req_t rdisReq;
    r_adp_adpm_route_disc_cnf_t *rdisCfm;

    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Enter destination address (HEX)...");
#endif

        R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

        if (strlen((char *)g_demo_buff.getStringBuffer) == 6)
        {
            R_BYTE_UInt16ToArr(hex_string_to_uint16(g_demo_buff.getStringBuffer), rdisReq.dstAddress);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("0x%.4X", R_BYTE_ArrToUInt16(rdisReq.dstAddress));
#endif
            break;
        }
    }
    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Enter maximum number of hops...");
#endif

        R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

        if (strlen((char *)g_demo_buff.getStringBuffer) == 1)
        {
            g_demo_buff.getStringBuffer[1] = '\0';
            rdisReq.maxHops = (uint8_t)atoi((char const *)g_demo_buff.getStringBuffer);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("%d", rdisReq.maxHops);
#endif
            break;
        }
    }

    /* Call route discovery function. */
    R_DEMO_AdpmRouteDiscovery(R_DEMO_G3_USE_PRIMARY_CH, &rdisReq, &rdisCfm);

    return R_RESULT_SUCCESS;
} /* menu_route_discovery */
/******************************************************************************
   End of function  menu_route_discovery
******************************************************************************/

/******************************************************************************
 * Function Name: menu_adpd_data
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
static r_result_t menu_adpd_data(void)
{
    uint16_t dstAddress;
    uint16_t frameLength;
    uint16_t i;
    uint16_t nrOfFrames;

    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Enter destination address (HEX)...");
#endif

        R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

        if (strlen((char *)g_demo_buff.getStringBuffer) == 6)
        {
            dstAddress = hex_string_to_uint16(g_demo_buff.getStringBuffer);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("0x%.4X", dstAddress);
#endif
            break;
        }
    }
    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Enter payload length...");
#endif

        R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

        if (strlen((char *)g_demo_buff.getStringBuffer) > 0)
        {
            g_demo_buff.getStringBuffer[strlen((char *)g_demo_buff.getStringBuffer)] = '\0';
            frameLength = (uint16_t)atoi((char const *)g_demo_buff.getStringBuffer);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("%d", frameLength);
#endif
            break;
        }
    }
    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Enter number of frames to send...");
#endif

        R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

        if (strlen((char *)g_demo_buff.getStringBuffer) > 0)
        {
            g_demo_buff.getStringBuffer[strlen((char *)g_demo_buff.getStringBuffer)] = '\0';
            nrOfFrames = (uint16_t)atoi((char const *)g_demo_buff.getStringBuffer);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("%d", nrOfFrames);
#endif
            break;
        }
    }

    /* Check length */
    if (((frameLength + R_IPV6_HEADER_SIZE) + R_UDP_HEADER_SIZE) > R_DEMO_APP_NSDU_BUFFER_SIZE)
    {
        frameLength = (R_DEMO_APP_NSDU_BUFFER_SIZE - R_IPV6_HEADER_SIZE) - R_UDP_HEADER_SIZE;
    }

    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Allow route discovery (1:yes/0:no)...");
#endif

        R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

        if (strlen((char *)g_demo_buff.getStringBuffer) == 1)
        {
            g_demo_buff.getStringBuffer[strlen((char *)g_demo_buff.getStringBuffer)] = '\0';
            g_demo_config.discoverRoute = (uint8_t)atoi((char const *)g_demo_buff.getStringBuffer);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("%d", g_demo_config.discoverRoute);
#endif
            break;
        }
    }

    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Priority (0:normal/1:high)...");
#endif

        R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

        if (strlen((char *)g_demo_buff.getStringBuffer) == 1)
        {
            g_demo_buff.getStringBuffer[strlen((char *)g_demo_buff.getStringBuffer)] = '\0';
            if (atoi((char const *)g_demo_buff.getStringBuffer) == 0x01u)
            {
                g_demo_config.qualityOfService = R_G3MAC_QOS_HIGH;
            }
            else
            {
                g_demo_config.qualityOfService = R_G3MAC_QOS_NORMAL;
            }
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("%d", g_demo_config.qualityOfService);
#endif
            break;
        }
    }

    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Frame type (1:ICMP/0:UDP)...");
#endif

        R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

        if (strlen((char *)g_demo_buff.getStringBuffer) == 1)
        {

            if (atoi((char const *)g_demo_buff.getStringBuffer) == 0)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("UDP");
#endif

                /* Send frames. */
                for (i = 0; i < nrOfFrames; i++)
                {
                    /* Call UDP frame send function. */
                    R_DEMO_GenerateUdpFrame(frameLength,
                                            g_demo_entity.panId,
                                            g_demo_entity.shortAddress,
                                            dstAddress);
                    // PLC_Send(msg_test,sizeof(msg_test));
                }

                break;
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("ICMP");
#endif

                /* Send frames. */
                for (i = 0; i < nrOfFrames; i++)
                {
                    /* Call UDP frame send function. */
                    R_DEMO_GenerateIcmpRequest(frameLength,
                                               g_demo_entity.panId,
                                               g_demo_entity.shortAddress,
                                               dstAddress);
                }

                break;
            }
        }
    }

    return R_RESULT_SUCCESS;
} /* menu_adpd_data */
/******************************************************************************
   End of function  menu_adpd_data
******************************************************************************/

/******************************************************************************
 * Function Name: menu_path_discovery
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
static r_result_t menu_path_discovery(void)
{
    r_adp_adpm_path_discovery_req_t pdisReq;
    r_adp_adpm_path_discovery_cnf_t *pdisCfm;

    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Enter destination address (HEX)...");
#endif

        R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

        if (strlen((char *)g_demo_buff.getStringBuffer) == 6)
        {
            R_BYTE_UInt16ToArr(hex_string_to_uint16(g_demo_buff.getStringBuffer), pdisReq.dstAddress);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("0x%.4X", R_BYTE_ArrToUInt16(pdisReq.dstAddress));
#endif
            break;
        }
    }

    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> Enter metric type (HEX)...");
#endif

        R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

        if (strlen((char *)g_demo_buff.getStringBuffer) == 4)
        {
            pdisReq.pathMetricType = hex_string_to_uint8(g_demo_buff.getStringBuffer);
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("0x%.2X", pdisReq.pathMetricType);
#endif
            break;
        }
    }

    /* Call path discovery function. */
    R_DEMO_AdpmPathDiscovery(R_DEMO_G3_USE_PRIMARY_CH, &pdisReq, &pdisCfm);

    return R_RESULT_SUCCESS;
} /* menu_path_discovery */
/******************************************************************************
   End of function  menu_path_discovery
******************************************************************************/

/******************************************************************************
 * Function Name: menu_network_start
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
static r_result_t menu_network_start(uint16_t panId)
{
    /* Set PAN ID. */
    r_adp_adpm_network_start_req_t nwsReq;
    r_adp_adpm_network_start_cnf_t *nwsCfm;
    r_eap_eapm_start_cnf_t *eStaCfm;

    /* FEATURE_ACTIVE_SCAN = TRUE for Certification */
    if (
        (R_DEMO_MODE_CERT == g_demo_config.appMode) &&
        (R_G3_BANDPLAN_ARIB != g_demo_config.bandPlan))
    {
        // R_DEMO_AppNetworkDiscovery(R_SCAN_DURATION_CERT); FORCED COMMENT
    }

    nwsReq.panId = panId;

    /* Call network start. */
    if ((R_DEMO_AdpmNetworkStart(R_DEMO_G3_USE_PRIMARY_CH, &nwsReq, &nwsCfm) == R_RESULT_SUCCESS) &&
        (R_ADP_STATUS_SUCCESS == nwsCfm->status))
    {
        g_demo_entity.panId = nwsReq.panId & 0xFCFF;
        g_demo_entity.shortAddress = 0x0000;
    }

    /* Enable processing of transmitting/receiving LBP message between EAP Layer and ADP Layer. */
    if (!((R_DEMO_EapmStart(R_DEMO_G3_USE_PRIMARY_CH, &eStaCfm) == R_RESULT_SUCCESS) &&
          (R_EAP_STATUS_SUCCESS == eStaCfm->status)))
    {
        return R_RESULT_FAILED;
    }

    return R_RESULT_SUCCESS;
} /* menu_network_start */
/******************************************************************************
   End of function  menu_network_start
******************************************************************************/

/******************************************************************************
 * Function Name: menu_flash
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
static r_result_t menu_flash(void)
{
    uint16_t tmp16;
    uint8_t tempArray[16] = {0};
    r_cap_dev_cfg_t config;
    r_demo_backup_payload_t tmpBackup;

    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n\f-------------------Demo Flash Menu-----------------------");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 0 - init dev config and reboot");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 1 - edit dev config and reboot");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 2 - disp all config");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 3 - erase preserved");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n z - Return");
#endif

        R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

        if (strlen((char *)g_demo_buff.getStringBuffer) == 1)
        {
            switch (g_demo_buff.getStringBuffer[0])
            {
            case '0':
            {
                R_memcpy(&g_demo_config, &g_cdemo_config, sizeof(r_demo_config_t));
                r_demo_nvm_config_init(R_TRUE, &g_cdemo_config);
                r_demo_nvm_erase(R_DEMO_G3_USE_PRIMARY_CH, R_NVM_ID_BACKUP);
                r_demo_nvm_erase(R_DEMO_G3_USE_PRIMARY_CH, R_NVM_ID_SETTING);

#ifdef __PLC__DEBUG__
                R_STDIO_Printf("  ------  Success Initialized Device Config on FlashMemory  ------\r\n");
#endif
                R_DEMO_BusyWait(1000);

                R_DEMO_SoftReset();

                break;
            }

            case '1':
                while (1)
                {

#ifdef __PLC__DEBUG__
                    R_STDIO_Printf("\n -> Enter last 16-bit address of EUI64 (HEX) ex)0xXXXX ...");
#endif
                    R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

                    if (strlen((char *)g_demo_buff.getStringBuffer) == 6)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf("0x%.4X", hex_string_to_uint16(g_demo_buff.getStringBuffer));
#endif

                        tmp16 = hex_string_to_uint16(g_demo_buff.getStringBuffer);
                        R_BYTE_UInt16ToArr(tmp16, tempArray);

                        R_memcpy(&g_demo_config, &g_cdemo_config, sizeof(r_demo_config_t));
                        r_demo_nvm_config_edit(R_DEMO_G3_USE_PRIMARY_CH, tempArray, &g_demo_config);
                        r_demo_nvm_erase(R_DEMO_G3_USE_PRIMARY_CH, R_NVM_ID_BACKUP);
                        r_demo_nvm_erase(R_DEMO_G3_USE_PRIMARY_CH, R_NVM_ID_SETTING);

#ifdef __PLC__DEBUG__
                        R_STDIO_Printf("  ------  Success Initialized Device Config on FlashMemory  ------\r\n");
#endif
                        R_DEMO_BusyWait(1000);

                        R_DEMO_SoftReset();
                        break;
                    }
                }
                break;

            case '2':
                r_demo_et_read_dev_config(R_DEMO_G3_USE_PRIMARY_CH, (uint8_t *)&config);
                if (r_demo_et_read_backup(R_DEMO_G3_USE_PRIMARY_CH, (uint8_t *)&tmpBackup) == R_RESULT_SUCCESS)
                {
                    r_demo_print_config(R_DEMO_G3_USE_PRIMARY_CH, &g_demo_config, &config, &tmpBackup);
                }
                else
                {
                    r_demo_print_config(R_DEMO_G3_USE_PRIMARY_CH, &g_demo_config, &config, NULL);
                }
                break;

            case '3':
                r_demo_nvm_erase(R_DEMO_G3_USE_PRIMARY_CH, R_NVM_ID_BACKUP);

#ifdef __PLC__DEBUG__
                R_STDIO_Printf("  ------  Success Erase Preserved info on FlashMemory  ------\r\n");
#endif
                break;

            case 'z':
                return R_RESULT_SUCCESS;

            default:
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("\n\n Invalid option! \n");
#endif
                break;
            } /* switch */
        }
    }
} /* menu_flash */
/******************************************************************************
   End of function  menu_flash
******************************************************************************/

/******************************************************************************
 * Function Name: menu_bandplan
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
static r_result_t menu_bandplan(void)
{
    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n\f-----------------Change BandPlan--------------------");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 0 - Cenelec-A");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 1 - Cenelec-B");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 2 - ARIB");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n 3 - FCC");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n z - Return");
#endif

        R_STDIO_Gets((char *)g_demo_buff.getStringBuffer);

        if (strlen((char *)g_demo_buff.getStringBuffer) == 1)
        {
            switch (g_demo_buff.getStringBuffer[0])
            {
            case '0':
            {
                g_demo_config.bandPlan = R_G3_BANDPLAN_CENELEC_A;
            }
                return R_RESULT_SUCCESS;

            case '1':
            {
                g_demo_config.bandPlan = R_G3_BANDPLAN_CENELEC_B;
            }
                return R_RESULT_SUCCESS;

            case '2':
            {
                g_demo_config.bandPlan = R_G3_BANDPLAN_ARIB;
            }
                return R_RESULT_SUCCESS;

            case '3':
            {
                g_demo_config.bandPlan = R_G3_BANDPLAN_FCC;
            }
                return R_RESULT_SUCCESS;

            case 'z':
                return R_RESULT_SUCCESS;

            default:
#ifdef __PLC__DEBUG__
                R_STDIO_Printf("\n\n Invalid option! \n");
#endif
                break;
            } /* switch */
        }
    }
} /* menu_bandplan */
/******************************************************************************
   End of function  menu_bandplan
******************************************************************************/

/******************************************************************************
 * Function Name: menu_dump_data
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
static r_result_t menu_dump_data(void)
{
    /* G3-Dump start */
    R_DEMO_G3Dump(100000U); /* trigger wate time = 100sec */

    return R_RESULT_SUCCESS;
} /* menu_dump_data */
/******************************************************************************
   End of function  menu_dump_data
******************************************************************************/

#ifndef R_DEFINE_DISABLE_UMAC
/******************************************************************************
 * Function Name: R_DEMO_AppToggleMacPromiscuous
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_AppToggleMacPromiscuous(uint8_t on)
{
    /* Local variables */
    uint8_t tempArray[R_ADP_MAX_IB_SIZE];
    r_g3mac_mlme_reset_req_t req;
    r_g3mac_mlme_reset_cnf_t *resCfm;

    if (on)
    {
        /* Init the MAC */
        if (R_DEMO_MacInit(R_DEMO_G3_USE_SECONDARY_CH) != R_RESULT_SUCCESS)
        {
            R_DEMO_HndFatalError();
        }

        /* Set device configuration. */
        if (R_DEMO_MacSetConfig(R_DEMO_G3_USE_SECONDARY_CH) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }

        req.setDefaultPIB = R_TRUE;
        if (!((R_DEMO_MlmeReset(R_DEMO_G3_USE_SECONDARY_CH, &req, &resCfm) == R_RESULT_SUCCESS) &&
              (R_ADP_STATUS_SUCCESS == resCfm->status)))
        {
            return R_RESULT_FAILED;
        }

        tempArray[0] = R_TRUE;
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_SECONDARY_CH, R_G3MAC_IB_PROMISCUOUS_MODE, 0, tempArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("  ------  Mac Promiscuous Mode On  ------\r\n");
#endif
    }
    else
    {
        req.setDefaultPIB = R_TRUE;
        if (!((R_DEMO_MlmeReset(R_DEMO_G3_USE_SECONDARY_CH, &req, &resCfm) == R_RESULT_SUCCESS) &&
              (R_ADP_STATUS_SUCCESS == resCfm->status)))
        {
            return R_RESULT_FAILED;
        }

        if (R_DEMO_DeInit(R_DEMO_G3_USE_SECONDARY_CH) != R_RESULT_SUCCESS)
        {
            R_DEMO_HndFatalError();
        }

#ifdef __PLC__DEBUG__
        R_STDIO_Printf("  ------  Mac Promiscuous Mode Off  ------\r\n");
#endif
    }

    return R_RESULT_SUCCESS;
} /* R_DEMO_AppToggleMacPromiscuous */
/******************************************************************************
   End of function  R_DEMO_AppToggleMacPromiscuous
******************************************************************************/
#endif /* #ifndef R_DEFINE_DISABLE_UMAC */

/******************************************************************************
 * Function Name: R_DEMO_AppWaitInsert
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
static void R_DEMO_AppWaitInsert(void)
{
    r_demo_nvm_read(R_NVM_COMMON_CH, R_NVM_ID_RANDWAIT, sizeof(r_demo_randwait_t), (uint8_t *)g_demo_config.wait);
    g_demo_config.wait[0] = (uint8_t)(g_demo_config.wait[0] + 53);

    R_DEMO_BusyWait(g_demo_config.wait[0]);

    r_demo_nvm_write(R_NVM_COMMON_CH, R_NVM_ID_RANDWAIT, sizeof(r_demo_randwait_t), (uint8_t *)g_demo_config.wait);
}
/******************************************************************************
   End of function  R_DEMO_AppWaitInsert
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_App
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/

void R_DEMO_App(void)
{
    if (R_DEMO_MODE_CERT == g_demo_config.appMode)
    {
        /* Printf cert version. */
        r_demo_print_cert_version(g_demo_config.modemPlatformType, R_DEMO_VERSION);
    }
    else
    {
        /* Printf demo version. */
        r_demo_print_version();
    }

    R_DEMO_GetDemoConfig();

    cpx3_current_task = CPX3_APD_INIT;

} /* R_DEMO_App */
/******************************************************************************
   End of function  R_DEMO_App
******************************************************************************/

void checkFirstConfig_Indication(void)
{
    uint8_t value = 0;
    uint32_t loop_counter = 0;
    uint32_t size = DMA_GetPackLenTransferred(1) + DMA_GetFrameLenTransferred(1) * 256;
    while (size != DMA_The_last_Index)
    {
        value = DMA_Buffer[DMA_The_last_Index];
        DMA_The_last_Index++;
        DMA_The_last_Index = DMA_The_last_Index % 1536;

        if (value == 0x7E)
        {
            start_config_set = 1;
            break;
        }
        // loop_counter++;
        // // COndition to Terminate the WHile loop in reading DMA_Buffer
        // if (loop_counter > sizeof(DMA_Buffer))
        // {
        //     DMA_The_last_Index = DMA_GetPackLenTransferred(1) + DMA_GetFrameLenTransferred(1) * 256;
        //     break;
        // }
    }
}

// r_result_t ADP_Init_monitor(void)
// {
//     r_result_t result = R_RESULT_SUCCESS;

//     // BUSY
//     if ((g3ctrl_signal) == 0uL)
//     {
//         result = R_RESULT_DEVICE_BUSY;
//     }
//     else
//     {
//         result = R_RESULT_SUCCESS;
//         cpx3_current_task = CPX3_APP_RESET;
//         config_monitor = 0;
//     }

//     return result;
// }

r_result_t CPX3_ADP_Init(void)
{
    r_result_t state = R_RESULT_FAILED;
    if (start_config_set)
    {
        state = R_DEMO_AdpInit(R_DEMO_G3_USE_PRIMARY_CH);
        // Start Sending Configration paramters
        /* Init the ADP */
        if (state == R_RESULT_DEVICE_BUSY)
        {
            return R_RESULT_DEVICE_BUSY;
        }

        else if (state != R_RESULT_SUCCESS)
        {
            Rest_flags();
            R_DEMO_HndFatalError();
        }
    }
    else
    {
        // Check After Booting an Indication for start Sending Configration
        checkFirstConfig_Indication();
    }
}

void CPX3_AppResetDevice(void)
{
    // Handle Monitor and Excute Sending Commands
    /* Reset device. */
    r_result_t ret = R_RESULT_SUCCESS;

    if (config_monitor)
    {
        CPX3_MonitorEventFlag(current_config_flag, current_config_id);
    }
    else
    {
        ret = R_DEMO_AppResetDevice();
        if (ret != R_RESULT_SUCCESS)
        {
            config_monitor = 0;
            cpx3_current_task = CPX3_BOOTING_TASK;
        }
    }
}

void CPX3_AppSetIpContent(void)
{
    r_result_t ret = R_RESULT_SUCCESS;

    if (config_monitor)
    {
        CPX3_MonitorEventFlag(current_config_flag, current_config_id);
    }
    else
    {
        ret = R_DEMO_SetIbContent();
        if (ret != R_RESULT_SUCCESS)
        {
            config_monitor = 0;
            cpx3_current_task = CPX3_BOOTING_TASK;
        }
    }
}

void CPX3_AppPresetProcess(void)
{
    r_result_t ret = R_RESULT_SUCCESS;

    if (config_monitor)
    {
        CPX3_MonitorEventFlag(current_config_flag, current_config_id);
    }
    else
    {
        ret = R_DEMO_AppPresetProcess();
        if (ret != R_RESULT_SUCCESS)
        {
            config_monitor = 0;
            cpx3_current_task = CPX3_BOOTING_TASK;
        }
    }
}

void CPX3_AppWaitInsert(void)
{
    R_DEMO_AppWaitInsert();
    cpx3_current_task = CPX3_APP_GET_NETWORK_PARM;
    config_monitor = 0;
}

void CPX3_AppGetNewtorkParm(void)
{
    /* Join the network. */

    if (config_monitor)
    {
        CPX3_MonitorEventFlag(current_config_flag, current_config_id);
    }
    else
    {
        CPX3_AppGetNewtorkParmHandle();
    }
}

void CPX3_AppScantNewtork(void)
{
    /* Join the network. */
    r_result_t state = R_RESULT_SUCCESS;

    if (config_monitor)
    {
        state = CPX3_MonitorEventFlag(current_config_flag, current_config_id);
        if (state == R_RESULT_SUCCESS)
        {

//           // CPX3_AppScantNewtorkHandle();
//            if (disCfm->PANCount == 0)
//            {
//                cpx3_current_task = CPX3_APP_SCAN_NETWORK;
//            }
//            else
//            {
//                cpx3_current_task = CPX3_APP_JOIN_NETWORK;
//            }
        }
    }
    else
    {
        state = CPX3_AppScantNewtorkHandle();
    }

    // {
    //     if (state == 0)
    // {
    //     // not found
    //     cpx3_current_task = CPX3_APP_SCAN_NETWORK;

    //     return R_RESULT_SUCCESS;

    // }
    // else

    // {
    //     // FOUND NETWORK
    //     cpx3_current_task = CPX3_APP_JOIN_NETWORK;
    //     return R_RESULT_SUCCESS;
    // }
    // }
}

//-------------------------------------
/******************************************************************************
 * Function Name: R_DEMO_AppResetDevice
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_AppResetDevice(void)
{
    /* Local variables */
    uint8_t tempArray[R_ADP_MAX_IB_SIZE];
    r_adp_adpm_reset_cnf_t *resCfm;
    r_eap_eapm_reset_cnf_t *eRstCfm;

    /* Reset PAN ID and short address. */
    g_demo_entity.panId = 0xFFFF;
    g_demo_entity.shortAddress = 0xFFFF;

    switch (current_AppResetTask)
    {
    case CPX3_ADP_SET_CONFIG:
        /* Set device configuration. */
        if (R_DEMO_AdpSetConfig(R_DEMO_G3_USE_PRIMARY_CH) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        break;
    case CPX3_ADPM_RESET:
        /* Reset ADP layer. */
        if (!((R_DEMO_AdpmReset(R_DEMO_G3_USE_PRIMARY_CH, &resCfm) == R_RESULT_SUCCESS) &&
              (R_ADP_STATUS_SUCCESS == resCfm->status)))
        {
            return R_RESULT_FAILED;
        }
        break;

    case CPX3_ADPM_GET_WRAP:
        /* Get firmware version. */
        if (R_DEMO_AdpmGetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_SOFT_VERSION, 0, tempArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        cpx3_current_task = CPX3_APP_SET_IP_CONTENT;
        break;
    default:
        break;
    }

    return R_RESULT_SUCCESS;
} /* R_DEMO_AppResetDevice */
/******************************************************************************
   End of function  R_DEMO_AppResetDevice
******************************************************************************/

#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
/******************************************************************************
 * Function Name: R_DEMO_AppHandleRfMcpsDataInd
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_AppHandleRfMcpsDataInd(const r_g3rfmac_mcps_data_ind_t *ind)
{
    if (R_TRUE == g_demo_config.verboseEnabled)
    {

        uint8_t *pAddr;
        uint32_t sz;
        uint16_t i;

#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n -> RfMcpsData Ind (ch[%d])...", 1);
#endif

#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n    srcAddrMode=%X srcPanId=0x%04X srcAddr=",
                       ind->srcAddrMode, ind->srcPanId);
#endif
        pAddr = (uint8_t *)ind->srcAddr;
        if (R_G3MAC_ADDR_MODE_EXTENDED == ind->srcAddrMode)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("0x%02X%02X%02X%02X%02X%02X%02X%02X\n",
                           pAddr[0], pAddr[1], pAddr[2], pAddr[3], pAddr[4], pAddr[5], pAddr[6], pAddr[7]);
#endif
        }
        else
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("0x%02X%02X\n", pAddr[0], pAddr[1]);
#endif
        }

#ifdef __PLC__DEBUG__
        R_STDIO_Printf("    dstAddrMode=%X dstPanId=0x%04X dstAddr=",
                       ind->dstAddrMode, ind->dstPanId);
#endif

        pAddr = (uint8_t *)ind->dstAddr;
        if (R_G3MAC_ADDR_MODE_EXTENDED == ind->dstAddrMode)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("0x%02X%02X%02X%02X%02X%02X%02X%02X\n",
                           pAddr[0], pAddr[1], pAddr[2], pAddr[3], pAddr[4], pAddr[5], pAddr[6], pAddr[7]);
#endif
        }
        else
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("0x%02X%02X\n", pAddr[0], pAddr[1]);
#endif
        }

#ifdef __PLC__DEBUG__
        R_STDIO_Printf("    msduLength=0x%03X(%dB)\n    msdu=", ind->msduLength, ind->msduLength);
#endif

        sz = (ind->msduLength < R_DEMO_APP_MSDU_PRINT_MAXLEN) ? ind->msduLength : R_DEMO_APP_MSDU_PRINT_MAXLEN;

        for (i = 0; i < sz; i++)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("%02X", ind->pMsdu[i]);
#endif
        }
        if (sz < ind->msduLength)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf("...");
#endif
        }
#ifdef __PLC__DEBUG__
        R_STDIO_Printf("\n    msduLinkQuality=0x%02X Secured=%X\n",
                       ind->msduLinkQuality, ind->secured);
#endif
    }
} /* R_DEMO_AppHandleRfMcpsDataInd */
/******************************************************************************
   End of function  R_DEMO_AppHandleRfMcpsDataInd
******************************************************************************/

#endif
