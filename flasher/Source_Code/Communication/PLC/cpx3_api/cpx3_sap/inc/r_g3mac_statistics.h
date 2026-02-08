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
* File Name   : r_g3mac_statistics.h
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
   \file      r_g3mac_statistics.h
   \version   1.00
   \brief     This is the public header template file for the statistics module
 */

/*
 * Prevent nested inclusions
 */
#ifndef R_G3MAC_STATISTICS_H
#define R_G3MAC_STATISTICS_H

/******************************************************************************
Macro definitions
******************************************************************************/

/*#define R_G3MAC_STATS_DATA_OUT_SUCCESS           (R_G3MAC_STATS_TXDATAPACKETCOUNT)     //!< Unsigned counter containing the number of MSDU unicast data packet supplied to the lower layers for transmission as success.*/
/*#define R_G3MAC_STATS_DATA_IN_DELIVERS_UNICAST   (R_G3MAC_STATS_RXDATAPACKETCOUNT)     //!< Unsigned counter containing the number of received MSDU unicast packets successfully delivered . */
/*#define R_G3MAC_STATS_CMD_OUT_BCNREQ_SUCCESS     (R_G3MAC_STATS_TXCMDPACKETCOUNT)      //!< Unsigned counter containing the number of MAC beacon request packet supplied to the lower layers for transmission as success. */
/*#define R_G3MAC_STATS_CMD_OUT_TMRES_SUCCESS      (R_G3MAC_STATS_TXCMDPACKETCOUNT)      //!< Unsigned counter containing the number of MAC tonemap response packet supplied to the lower layers for transmission as success. */
/*#define R_G3MAC_STATS_CMD_IN_BCNREQ              (R_G3MAC_STATS_RXCMDPACKETCOUNT)      //!< Unsigned counter containing the number of received MAC beacon request packet. */
/*#define R_G3MAC_STATS_CMD_IN_TMRES               (R_G3MAC_STATS_RXCMDPACKETCOUNT)      //!< Unsigned counter containing the number of received MAC tonemap response packet. */
/*#define R_G3MAC_STATS_DATA_OUT_BCAST_SUCCESS     (R_G3MAC_STATS_TXDATABROADCASTCOUNT)  //!< Unsigned counter containing the number of MSDU unicast data packet supplied to the lower layers for transmission as failure.*/
/*#define R_G3MAC_STATS_DATA_IN_DELIVERS_BCAST     (R_G3MAC_STATS_RXDATABROADCASTCOUNT)  //!< Unsigned counter containing the number of received MSDU broadcast packets successfully delivered . */

/******************************************************************************
Typedef definitions
******************************************************************************/
/*!
    \enum r_mac_status_t
    \brief Enumeration of MAC status values
 */
typedef enum
{
    R_G3MAC_STATS_TXDATAPACKETCOUNT = 0x00,                /*!< Statistic counter of successfully transmitted unicast MSDUs */
    R_G3MAC_STATS_RXDATAPACKETCOUNT,                       /*!< Statistic counter of successfully received unicast MSDUs */
    R_G3MAC_STATS_TXCMDPACKETCOUNT,                        /*!< Statistic counter of successfully transmitted command packets */
    R_G3MAC_STATS_RXCMDPACKETCOUNT,                        /*!< Statistic counter of successfully received command packets */
    R_G3MAC_STATS_CSMAFAILCOUNT,                           /*!< Counts the number of times the CSMA back-offs reach macMaxCSMABackoffs */
    R_G3MAC_STATS_CSMANOACKCOUNT,                          /*!< Counts the number of times an ACK is not received while transmitting a unicast data and cmd frame (The loss of ACK is attributed to collisions) */
    R_G3MAC_STATS_RXDATABROADCASTCOUNT,                    /*!< Statistic counter of successfully received broadcast frames */
    R_G3MAC_STATS_TXDATABROADCASTCOUNT,                    /*!< Statistic counter of the number of broadcast frames sent */
    R_G3MAC_STATS_BADCRCCOUNT,                             /*!< Statistic counter of the number of frames received with bad CRC */

    R_G3MAC_STATS_STD_END,

    R_G3MAC_STATS_ALL_IN_RECEIVES = R_G3MAC_STATS_STD_END, /*!< Unsigned counter containing the number of received all kind of packet from LMAC */
    R_G3MAC_STATS_ALL_IN_DISCARDS,                         /*!< Unsigned counter containing the number of received packet from LMAC discarded y parameter error.  */
    R_G3MAC_STATS_ALL_OUT_TRANSMITS,                       /*!< Unsigned counter containing the number of MSDU supplied to the lower layers for transmission. */
    R_G3MAC_STATS_ALL_OUT_SUCCESS,                         /*!< Unsigned counter containing the number of MSDU supplied to the lower layers for transmission as success. */
    R_G3MAC_STATS_ALL_OUT_FAILURE,                         /*!< Unsigned counter containing the number of MSDU supplied to the lower layers for transmission as failure. */
    R_G3MAC_STATS_ALL_OUT_FAILURE_LR,                      /*!< Unsigned counter containing the number of MSDU supplied to the lower layers for transmission as NackRetryOver. */

    R_G3MAC_STATS_DATA_IN_RECEIVES,                        /*!< Unsigned counter containing the number of received MSDU except broadcast . */
    R_G3MAC_STATS_DATA_IN_RECEIVES_BCAST,                  /*!< Unsigned counter containing the number of received MSDU as broadcast . */
    R_G3MAC_STATS_DATA_IN_REASM_REQDS,                     /*!< Unsigned counter containing the number of received MSDU segment reassembled. */
    R_G3MAC_STATS_DATA_IN_REASM_DISCARDS,                  /*!< Unsigned counter containing the number of received MSDU segment discard. */
    R_G3MAC_STATS_DATA_IN_REASM_OKS,                       /*!< Unsigned counter containing the number of received MSDU segment reassembled successfully . */
    R_G3MAC_STATS_DATA_IN_DISCARDS_DEVICETABLE,            /*!< Unsigned counter containing the number of received MSDU discarded by device tables filter. */
    R_G3MAC_STATS_DATA_IN_DISCARDS_SECURITY,               /*!< Unsigned counter containing the number of received MSDU discarded by decrypt error. */
    R_G3MAC_STATS_DATA_IN_DELIVERS_UNICAST,                /*!< Unsigned counter containing the number of received MSDU unicast packets successfully delivered . */
    R_G3MAC_STATS_DATA_IN_DELIVERS_BCAST,                  /*!< Unsigned counter containing the number of received MSDU broadcast packets successfully delivered .  */
    R_G3MAC_STATS_DATA_OUT_TRANSMITS,                      /*!< Unsigned counter containing the number of MSDU unicast data packet supplied to the lower layers for transmission(include retransmission). */
    R_G3MAC_STATS_DATA_OUT_TRANSMITS_BCAST,                /*!< Unsigned counter containing the number of MSDU broadcast data packet supplied to the lower layers for transmission. */
    R_G3MAC_STATS_DATA_OUT_RETRANSMISSION,                 /*!< Unsigned counter containing the number of MSDU unicast data packet supplied to the lower layers only for retransmission. */

    R_G3MAC_STATS_DATA_OUT_SUCCESS,                        /*!< Unsigned counter containing the number of MSDU unicast data packet supplied to the lower layers for transmission as success. */
    R_G3MAC_STATS_DATA_OUT_FAILURE,                        /*!< Unsigned counter containing the number of MSDU broadcast data packet supplied to the lower layers for transmission as success. */

    R_G3MAC_STATS_DATA_OUT_BCAST_SUCCESS,                  /*!< Unsigned counter containing the number of MSDU unicast data packet supplied to the lower layers for transmission as failure. */
    R_G3MAC_STATS_DATA_OUT_BCAST_FAILURE,                  /*!< Unsigned counter containing the number of MSDU broadcast data packet supplied to the lower layers for transmission as failure. */

    R_G3MAC_STATS_CMD_IN_BCNREQ,                           /*!< Unsigned counter containing the number of received MAC beacon request packet. */
    R_G3MAC_STATS_CMD_IN_BCN,                              /*!< Unsigned counter containing the number of received MAC beacon packet. */
    R_G3MAC_STATS_CMD_IN_TMREQ,                            /*!< Unsigned counter containing the number of received MSDU packet with tmr=1. */
    R_G3MAC_STATS_CMD_IN_TMRES,                            /*!< Unsigned counter containing the number of received MAC tonemap response packet. */
    R_G3MAC_STATS_CMD_OUT_BCNREQ_TRANSMITS,                /*!< Unsigned counter containing the number of MAC beacon request packet supplied to the lower layers for transmission. */
    R_G3MAC_STATS_CMD_OUT_BCNREQ_SUCCESS,                  /*!< Unsigned counter containing the number of MAC beacon request packet supplied to the lower layers for transmission as success. */
    R_G3MAC_STATS_CMD_OUT_BCNREQ_FAILLURE,                 /*!< Unsigned counter containing the number of MAC beacon request packet supplied to the lower layers for transmission as failure. */
    R_G3MAC_STATS_CMD_OUT_BCN_TRANSMITS,                   /*!< Unsigned counter containing the number of MAC beacon packet supplied to the lower layers for transmission. */
    R_G3MAC_STATS_CMD_OUT_BCN_SUCCESS,                     /*!< Unsigned counter containing the number of MAC beacon packet supplied to the lower layers for transmission as success. */
    R_G3MAC_STATS_CMD_OUT_BCN_FAILLURE,                    /*!< Unsigned counter containing the number of MAC beacon packet supplied to the lower layers for transmission as failure. */
    R_G3MAC_STATS_CMD_OUT_TMRES_TRANSMITS,                 /*!< Unsigned counter containing the number of MAC tonemap response packet supplied to the lower layers for transmission. */
    R_G3MAC_STATS_CMD_OUT_TMRES_SUCCESS,                   /*!< Unsigned counter containing the number of MAC tonemap response packet supplied to the lower layers for transmission as success. */
    R_G3MAC_STATS_CMD_OUT_TMRES_FAILLURE,                  /*!< Unsigned counter containing the number of MAC tonemap response packet supplied to the lower layers for transmission as failure. */

    R_G3MAC_STATS_MCPS_DATA_REQD,                          /*!< Unsigned counter containing the number of received MCPS-DATA.request . */
    R_G3MAC_STATS_MCPS_DATA_SUCCESS,                       /*!< Unsigned counter containing the number of received MCPS-DATA.confirm as success . */
    R_G3MAC_STATS_MCPS_DATA_FAILURE,                       /*!< Unsigned counter containing the number of received MCPS-DATA.confirm as failure . */
    R_G3MAC_STATS_MCPS_DATA_FAILURE_NOACK,                 /*!< Unsigned counter containing the number of received MCPS-DATA.confirm as failure which status is NO ACK. */
    R_G3MAC_STATS_MCPS_DATA_FAILURE_CSMAFAILURE,           /*!< Unsigned counter containing the number of received MCPS-DATA.confirm as failure which status is CSMA FAILURE. */
    R_G3MAC_STATS_MCPS_DATA_FAILURE_LMLABORT,              /*!< Unsigned counter containing the number of received MCPS-DATA.confirm as failure which status is timeout by maxXXXDataTxTimeout. */
    R_G3MAC_STATS_MCPS_DATA_IND,                           /*!< Unsigned counter containing the number of issued MCPS-DATA.indication . */
    R_G3MAC_STATS_MCPS_TMR_RECEIVE_IND,                    /*!< Unsigned counter containing the number of issued MLME-TMR_RECEIVE.indication . */
    R_G3MAC_STATS_MCPS_TMR_TRANSMIT_IND,                   /*!< Unsigned counter containing the number of issued MLME-TMR_TRANSMIT.indication . */

    R_G3MAC_STATS_MCPS_SUCCESS_UC_ROBUST,                  /*!< Unsigned counter containing the number of transmission success in unicast with robust modulation. */
    R_G3MAC_STATS_MCPS_SUCCESS_UC_BPSK,                    /*!< Unsigned counter containing the number of transmission success in unicast with BPSK modulation. */
    R_G3MAC_STATS_MCPS_SUCCESS_UC_QPSK,                    /*!< Unsigned counter containing the number of transmission success in unicast with QPSK modulation. */
    R_G3MAC_STATS_MCPS_SUCCESS_UC_8PSK,                    /*!< Unsigned counter containing the number of transmission success in unicast with 8PSK modulation. */
    R_G3MAC_STATS_MCPS_FAILURE_UC_ROBUST,                  /*!< Unsigned counter containing the number of transmission failure in unicast with robust modulation. */
    R_G3MAC_STATS_MCPS_FAILURE_UC_BPSK,                    /*!< Unsigned counter containing the number of transmission failure in unicast with BPSK modulation. */
    R_G3MAC_STATS_MCPS_FAILURE_UC_QPSK,                    /*!< Unsigned counter containing the number of transmission failure in unicast with QPSK modulation. */
    R_G3MAC_STATS_MCPS_FAILURE_UC_8PSK,                    /*!< Unsigned counter containing the number of transmission failure in unicast with 8PSK modulation. */

    R_G3MAC_STATS_MCPS_SUCCESS_CSMA_NCW3,                  /*!< Unsigned counter containing the number of transmission success with back-off exponent of Normal contention window size = 3. */
    R_G3MAC_STATS_MCPS_SUCCESS_CSMA_NCW4,                  /*!< Unsigned counter containing the number of transmission success with back-off exponent of Normal contention window size = 4. */
    R_G3MAC_STATS_MCPS_SUCCESS_CSMA_NCW5,                  /*!< Unsigned counter containing the number of transmission success with back-off exponent of Normal contention window size = 5. */
    R_G3MAC_STATS_MCPS_SUCCESS_CSMA_NCW6,                  /*!< Unsigned counter containing the number of transmission success with back-off exponent of Normal contention window size = 6. */
    R_G3MAC_STATS_MCPS_SUCCESS_CSMA_NCW7,                  /*!< Unsigned counter containing the number of transmission success with back-off exponent of Normal contention window size = 7. */
    R_G3MAC_STATS_MCPS_SUCCESS_CSMA_NCW8,                  /*!< Unsigned counter containing the number of transmission success with back-off exponent of Normal contention window size = 8. */
    R_G3MAC_STATS_MCPS_SUCCESS_CSMA_NCW9,                  /*!< Unsigned counter containing the number of transmission success with back-off exponent of Normal contention window size = 9. */
    R_G3MAC_STATS_MCPS_SUCCESS_CSMA_NCW10,                 /*!< Unsigned counter containing the number of transmission success with back-off exponent of Normal contention window size = 10. */
    R_G3MAC_STATS_MCPS_FAILURE_CSMA_NCW3,                  /*!< Unsigned counter containing the number of transmission failure with back-off exponent of Normal contention window size = 3. */
    R_G3MAC_STATS_MCPS_FAILURE_CSMA_NCW4,                  /*!< Unsigned counter containing the number of transmission failure with back-off exponent of Normal contention window size = 4. */
    R_G3MAC_STATS_MCPS_FAILURE_CSMA_NCW5,                  /*!< Unsigned counter containing the number of transmission failure with back-off exponent of Normal contention window size = 5. */
    R_G3MAC_STATS_MCPS_FAILURE_CSMA_NCW6,                  /*!< Unsigned counter containing the number of transmission failure with back-off exponent of Normal contention window size = 6. */
    R_G3MAC_STATS_MCPS_FAILURE_CSMA_NCW7,                  /*!< Unsigned counter containing the number of transmission failure with back-off exponent of Normal contention window size = 7. */
    R_G3MAC_STATS_MCPS_FAILURE_CSMA_NCW8,                  /*!< Unsigned counter containing the number of transmission failure with back-off exponent of Normal contention window size = 8. */
    R_G3MAC_STATS_MCPS_FAILURE_CSMA_NCW9,                  /*!< Unsigned counter containing the number of transmission failure with back-off exponent of Normal contention window size = 9. */
    R_G3MAC_STATS_MCPS_FAILURE_CSMA_NCW10,                 /*!< Unsigned counter containing the number of transmission failure with back-off exponent of Normal contention window size = 10. */

    R_G3MAC_STATS_END,
} r_g3mac_statistics_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#endif /* R_G3MAC_STATISTICS_H */

