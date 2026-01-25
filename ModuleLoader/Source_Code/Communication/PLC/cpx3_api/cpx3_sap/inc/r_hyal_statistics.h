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
* File Name   : r_hyal_statistics.h
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
   \file      r_hyal_statistics.h
   \version   1.00
   \brief     This is the public header template file for the statistics module
 */

/*
 * Prevent nested inclusions
 */
#ifndef R_HYAL_STATISTICS_H
#define R_HYAL_STATISTICS_H

#include "r_adp_statistics.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/
/*!
    \enum r_mac_status_t
    \brief Enumeration of MAC status values
 */
typedef enum
{
    /*
       Group: 6lowpan
    */
    R_HYAL_STATS_START = ((R_ADP_STATS_END+0x10)&0xF0),      //aligned by 16elements for future update
    R_HYAL_STATS_DATAREQ_MEDIA_0 = R_HYAL_STATS_START,       /*!< Unsigned counter containing the number of transmission of RF media packet */
    R_HYAL_STATS_DATAREQ_MEDIA_1,
    R_HYAL_STATS_DATAREQ_MEDIA_2,
    R_HYAL_STATS_DATAREQ_MEDIA_3,
    R_HYAL_STATS_DATAREQ_MEDIA_4,
    R_HYAL_STATS_DATACNF_SUCCESS_MEDIA_0,
    R_HYAL_STATS_DATACNF_SUCCESS_MEDIA_1,
    R_HYAL_STATS_DATACNF_SUCCESS_MEDIA_2,
    R_HYAL_STATS_DATACNF_SUCCESS_MEDIA_3,
    R_HYAL_STATS_DATACNF_SUCCESS_MEDIA_4,
    R_HYAL_STATS_DATACNF_ERR_MEDIA_0,
    R_HYAL_STATS_DATACNF_ERR_MEDIA_1,
    R_HYAL_STATS_DATACNF_ERR_MEDIA_2,
    R_HYAL_STATS_DATACNF_ERR_MEDIA_3,
    R_HYAL_STATS_DATACNF_ERR_MEDIA_4,
    R_HYAL_STATS_TX_DATA_PLC,
    R_HYAL_STATS_TX_DATA_RF,
    R_HYAL_STATS_TX_BACKUPED_PLC,
    R_HYAL_STATS_TX_BACKUPED_RF,
    R_HYAL_STATS_TXDISCARD_NONPOS_PLC,
    R_HYAL_STATS_TXDISCARD_NONPOS_RF,
    R_HYAL_STATS_TXERR_NOACK_PLC,
    R_HYAL_STATS_TXERR_NOACK_RF,
    R_HYAL_STATS_TXERR_CA_PLC,
    R_HYAL_STATS_TXERR_CA_RF,
    R_HYAL_STATS_TXERR_DUTYLIMIT_RF,

    R_HYAL_STATS_RX_DATA_PLC,
    R_HYAL_STATS_RX_DATA_RF,
    R_HYAL_STATS_RXDISCARD_DUPLICATE_PLC,
    R_HYAL_STATS_RXDISCARD_DUPLICATE_RF,


    R_HYAL_STATS_END,
} r_hyal_statistics_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#endif /* R_HYAL_STATISTICS_H */

