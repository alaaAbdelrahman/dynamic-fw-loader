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
* File Name   : r_uif_statistics.h
*    @version
*        $Rev: 3177 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2017-04-26 14:59:37 +0900#$
* Description :
******************************************************************************/


/*                                                                  COMMENT_E  */

/*!
   \file      r_uif_statistics.h
   \version   1.00
   \brief     This is the public header template file for the statistics module
 */

/*
 * Prevent nested inclusions
 */
#ifndef R_UIF_STATISTICS_H
#define R_UIF_STATISTICS_H

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/
/*!
    \enum r_uif_status_t
    \brief Enumeration of UIF status values
 */
typedef enum
{
    R_UIF_STATS_RX_START        =   0,
    R_UIF_STATS_RX_IN_CRC_ERROR =   R_UIF_STATS_RX_START,       /*!< Statistic counter of the number of detected CRC error on received HDLC packet */
    R_UIF_STATS_RX_IN_SYNC_BREAK,                               /*!< Statistic counter of the number of received invalid length HCLC packet. */
    R_UIF_STATS_RX_IN_TIMEOUT,                                  /*!< Statistic counter of the number of timeout during receiving HDLC packet. */
    R_UIF_STATS_RX_IN_LENGTH_ERROR,                             /*!< Statistic counter of the number of received length over from HCLC sync word (0x7E) */
    R_UIF_STATS_RX_IN_BUFF_OVERFLOW,                            /*!< Statistic counter of the number of detected received buffer overflow */
    R_UIF_STATS_RX_IN_SECURITY_SUCCESS,                         /*!< Statistic counter of the number of decrypt success for received secure packet. */
    R_UIF_STATS_RX_IN_SECURITY_ERROR,                           /*!< Statistic counter of the number of decrypt error for received secure packet. */
    R_UIF_STATS_RX_IN_DISCARD,                                  /*!< Statistic counter of the number of discarded packet during asynchronous. */
    R_UIF_STATS_RX_OUT_COMMAND_REQD,                            /*!< Statistic counter of successfully transmitted received HDLC packet after parsing command. */
    R_UIF_STATS_RX_OUT_COMMAND_REQD_ERROR,                      /*!< Statistic counter of error transmitted received HDLC packets */
    R_UIF_STATS_RX_OUT_COMMAND_SUCCESS,                         /*!< Statistic counter of successfully command buffer released from Destination */
    R_UIF_STATS_RX_END,

    R_UIF_STATS_TX_START          =   R_UIF_STATS_RX_END,
    R_UIF_STATS_TX_IN_RCV_COMMAND =   R_UIF_STATS_TX_START,     /*!< Statistic counter of successfully encapsulated HDLC packet. */
    R_UIF_STATS_TX_IN_RCV_ERROR,                                /*!< Statistic counter of error on requested command packet for TX. */
    R_UIF_STATS_TX_IN_SECURITY_SUCCESS,                         /*!< Statistic counter of the number of encrypt success for transmitted secure packet. */
    R_UIF_STATS_TX_IN_SECURITY_ERROR,                           /*!< Statistic counter of the number of encrypt error for transmitted secure packet. */
    R_UIF_STATS_TX_OUT_COMMAND_REQD,                            /*!< Statistic counter of successfully transmitted requested command after encapsulated HDLC packet. */
    R_UIF_STATS_TX_OUT_COMMAND_SUCCESS,                         /*!< Statistic counter of successfully transmitted HDLC */
    R_UIF_STATS_TX_OUT_TIMEOUT,                                 /*!< Statistic counter of timeout error on transmitted HDLC */
    R_UIF_STATS_TX_DRV_ERROR,                                   /*!< Statistic counter of sci driver error on transmitted HDLC */
    R_UIF_STATS_TX_END,
    R_UIF_STATS_RXTX_END = R_UIF_STATS_TX_END,
} r_uif_statistics_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#endif /* R_UIF_STATISTICS_H */

