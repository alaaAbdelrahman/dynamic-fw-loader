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
* File Name   : r_eap_statistics.h
*    @version
*        $Rev: 9602 $
*    @last editor
*        $Author: a0202438 $
*    @date
*        $Date:: 2022-04-08 13:53:43 +0900#$
* Description :
******************************************************************************/


/*                                                                  COMMENT_E  */

/*!
   \file      r_eap_statistics.h
   \version   1.00
   \brief     This is the public header template file for the statistics module
 */

/*
 * Prevent nested inclusions
 */
#ifndef R_EAP_STATISTICS_H
#define R_EAP_STATISTICS_H

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
    R_EAP_STATS_LBP_IN_RECEIVES = 0x00,                             /*!< Unsigned counter containing the number of received LBP packet from ADP */
    R_EAP_STATS_LBP_IN_RECEIVES_JOINRERQ,                           /*!< Unsigned counter containing the number of received LBP join request packet */
    R_EAP_STATS_LBP_IN_RECEIVES_LEAVE,                              /*!< Unsigned counter containing the number of received LBP leave request packet */
    R_EAP_STATS_LBP_IN_DISCARD,                                     /*!< Unsigned counter containing the number of received LBP packet discarded due to errors in processing */
    R_EAP_STATS_LBP_OUT_TRANSMITS,                                  /*!< Unsigned counter containing the number of LBP supplied to the ADP layers for transmission. */
    R_EAP_STATS_LBP_OUT_SUCCESS,                                    /*!< Unsigned counter containing the number of LBP supplied to the ADP layers for transmission as success. */
    R_EAP_STATS_LBP_OUT_FAILURE,                                    /*!< Unsigned counter containing the number of LBP supplied to the ADP layers for transmission as failure. */

    R_EAP_STATS_EAPPSK_SUCCESS,                                     /*!< Unsigned counter containing the number of success count of EAP-PSK (include Join and Gmktransmission during Rekey) */
    R_EAP_STATS_EAPPSK_FAILURE_MSG2_MACP,                           /*!< Unsigned counter containing the number of mismatch count of MACP on eap message 2 */
    R_EAP_STATS_EAPPSK_FAILURE_MSG4_EAX,                            /*!< Unsigned counter containing the number of mismatch count of EAX on eap message 4 */
    R_EAP_STATS_EAPPSK_FAILURE_MSG4_PCH,                            /*!< Unsigned counter containing the number of error count of PCH on eap message 4 */

    R_EAP_STATS_NEWDEVICE,                                          /*!< Unsigned counter containing the number of Issued EAPM-NEWDEVICE.indication */
    R_EAP_STATS_JOIN_SUCCESS,                                       /*!< Unsigned counter containing the number of Issued EAPM-NETWORK-JOIN.indication as success */
    R_EAP_STATS_JOIN_FAILURE,                                       /*!< Unsigned counter containing the number of Issued EAPM-NETWORK-JOIN.indication as failure */
    R_EAP_STATS_JOIN_DISCARD,                                       /*!< Unsigned counter containing the number of Issued EAPM-NETWORK-JOIN.indication as discard */
    R_EAP_STATS_KICK_SUCCESS,                                       /*!< Unsigned counter containing the number of success count of Kick */
    R_EAP_STATS_KICK_FAILURE,                                       /*!< Unsigned counter containing the number of failure count of Kick */
    R_EAP_STATS_GMKTRANS_SUCCESS,                                   /*!< Unsigned counter containing the number of success count of GMK transmission during Rekey */
    R_EAP_STATS_GMKTRANS_FAILURE,                                   /*!< Unsigned counter containing the number of failure count of GMK transmission during Rekey */
    R_EAP_STATS_GMKACTIVATE_SUCCESS,                                /*!< Unsigned counter containing the number of success count of GMK activation during Rekey */
    R_EAP_STATS_GMKACTIVATE_FAILURE,                                /*!< Unsigned counter containing the number of failure count of GMK activation during Rekey */

    R_EAP_STATS_INVALIDQUEUE,                                       /*!< Unsigned counter containing the number of detect invalid queue */

    R_EAP_STATS_GMKREMOVAL_SUCCESS,                                /*!< Unsigned counter containing the number of success count of GMK removal after Rekey */
    R_EAP_STATS_GMKREMOVAL_FAILURE,                                /*!< Unsigned counter containing the number of failure count of GMK removal after Rekey */

    R_EAP_STATS_END,
} r_eap_statistics_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#endif /* R_EAP_STATISTICS_H */

