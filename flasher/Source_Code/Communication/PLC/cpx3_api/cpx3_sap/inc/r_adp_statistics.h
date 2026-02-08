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
* File Name   : r_adp_statistics.h
*    @version
*        $Rev: 8407 $
*    @last editor
*        $Author: a0202438 $
*    @date
*        $Date:: 2022-01-26 17:47:18 +0900#$
* Description :
******************************************************************************/


/*                                                                  COMMENT_E  */

/*!
   \file      r_adp_statistics.h
   \version   1.00
   \brief     This is the public header template file for the statistics module
 */

/*
 * Prevent nested inclusions
 */
#ifndef R_ADP_STATISTICS_H
#define R_ADP_STATISTICS_H

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
    R_ADP_STATS_LOWPAN_IN_RECEIVES,                         /*!< Unsigned counter containing the number of received ADP 6LoWPAN datagrams (include LBP, LOADng ) */
    R_ADP_STATS_LOWPAN_IN_HDR_ERRORS,                       /*!< Unsigned counter containing the number of received 6LoWPAN datagrams discarded due to errors in the headers for ADP packets */
    R_ADP_STATS_LOWPAN_IN_MESH_RECEIVES,                    /*!< Unsigned counter containing the number of received 6LoWPAN datagrams with a MESH header. */
    R_ADP_STATS_LOWPAN_IN_MESH_FORWDS,                      /*!< Unsigned counter containing the number of received 6LoWPAN datagrams requiring mesh forwarding */
    R_ADP_STATS_LOWPAN_IN_MESH_DELIVERS,                    /*!< Unsigned counter containing the number of received 6LoWPAN datagrams delivered locally. */
    R_ADP_STATS_LOWPAN_IN_REASM_REQDS,                      /*!< Unsigned counter containing the number of received 6LoWPAN fragments reassembled. */
    R_ADP_STATS_LOWPAN_IN_REASM_FAILS,                      /*!< Unsigned counter containing the number of failed reassembled 6LoWPAN fragments. */
    R_ADP_STATS_LOWPAN_IN_REASM_OKS,                        /*!< Unsigned counter containing the number of 6LoWPAN packets successfully reassembled. */
    R_ADP_STATS_LOWPAN_IN_COMP_REQDS,                       /*!< Unsigned counter containing the number of received 6LoWPAN datagrams requiring header decompression. */
    R_ADP_STATS_LOWPAN_IN_COMP_FAILS,                       /*!< Unsigned counter containing the number of received 6LoWPAN datagrams where header decompression failed. */
    R_ADP_STATS_LOWPAN_IN_COMP_OKS,                         /*!< Unsigned counter containing the number of received 6LoWPAN datagrams where header decompression was successful. */
    R_ADP_STATS_LOWPAN_IN_DISCARDS,                         /*!< Unsigned counter containing the number of received 6LoWPAN datagrams discarded due to errors in the processing (include LBP, LOADng ). */
    R_ADP_STATS_LOWPAN_IN_DISCARDS_SECURITY,                /*!< Unsigned counter containing the number of received 6LoWPAN datagrams discarded by securityLevel. */
    R_ADP_STATS_LOWPAN_IN_DISCARDS_BCAST_TBL,               /*!< Unsigned counter containing the number of received 6LoWPAN datagrams discarded by Broadcast log tables. */
    R_ADP_STATS_LOWPAN_IN_DISCARDS_GROUP_TBL,               /*!< Unsigned counter containing the number of received 6LoWPAN datagrams discarded by Group tables. */
    R_ADP_STATS_LOWPAN_IN_DISCARDS_IPV6_BFULL,              /*!< Unsigned counter containing the number of received 6LoWPAN datagrams discarded by full of ipv6 buffer. */
    R_ADP_STATS_LOWPAN_IN_DISCARDS_LBP_BFULL,               /*!< Unsigned counter containing the number of received 6LoWPAN datagrams discarded by full of lbp buffer. */
    R_ADP_STATS_LOWPAN_IN_DISCARDS_RELAY_BFULL,             /*!< Unsigned counter containing the number of received 6LoWPAN datagrams discarded by full of relay buffer. */
    R_ADP_STATS_LOWPAN_IN_DELIVERS,                         /*!< Unsigned counter containing the number of received ADP IPv6 packets successfully delivered. */
    R_ADP_STATS_LOWPAN_IN_DELIVERS_UNICAST,                 /*!< Unsigned counter containing the number of received ADP IPv6 unicast packets successfully delivered . */
    R_ADP_STATS_LOWPAN_IN_DELIVERS_MULTICAST,               /*!< Unsigned counter containing the number of received ADP IPv6 multicast packets successfully delivered . */
    R_ADP_STATS_LOWPAN_OUT_REQUESTS,                        /*!< Unsigned counter containing the number of sent ADP IPv6 packets supplied by the IPv6 layer. */
    R_ADP_STATS_LOWPAN_OUT_COMP_REQDS,                      /*!< Unsigned counter containing the number of sent IPv6 packets for which header compression was attempted. */
    R_ADP_STATS_LOWPAN_OUT_COMP_FAILS,                      /*!< Unsigned counter containing the number of sent IPv6 packets for which header compression failed. */
    R_ADP_STATS_LOWPAN_OUT_COMP_OKS,                        /*!< Unsigned counter containing the number of sent IPv6 packets for which header compression was successful. */
    R_ADP_STATS_LOWPAN_OUT_FRAG_REQDS,                      /*!< Unsigned counter containing the number of sent IPv6 packets that required fragmentation. */
    R_ADP_STATS_LOWPAN_OUT_FRAG_FAILS,                      /*!< Unsigned counter containing the number of sent IPv6 packets discarded due to failed fragmentation. */
    R_ADP_STATS_LOWPAN_OUT_FRAG_OKS,                        /*!< Unsigned counter containing the number of sent IPv6 packets successfully fragmented. */
    R_ADP_STATS_LOWPAN_OUT_FRAG_CREATES,                    /*!< Unsigned counter containing the number of IPv6 packets generated as a result of fragmentation. */
    R_ADP_STATS_LOWPAN_OUT_MESH_HOP_LIMIT_EXCEEDS,          /*!< Unsigned counter containing the number of 6LoWPAN datagrams dropped because the MESH hop limit was exceeded. */
    R_ADP_STATS_LOWPAN_OUT_MESH_NO_ROUTES,                  /*!< Unsigned counter containing the number of 6LoWPAN datagrams with a MESH header dropped because of missing forwarding information. */
    R_ADP_STATS_LOWPAN_OUT_MESH_REQUESTS,                   /*!< Unsigned counter containing the number of 6LoWPAN datagrams to be sent requiring MESH header encapsulation. Covers only local encapsulations. */
    R_ADP_STATS_LOWPAN_OUT_MESH_FORWDS,                     /*!< Unsigned counter containing the number of 6LoWPAN datagrams to be forwarded. */
    R_ADP_STATS_LOWPAN_OUT_MESH_TRANSMITS,                  /*!< Unsigned counter containing the number of 6LoWPAN datagrams that have a MESH header. */
    R_ADP_STATS_LOWPAN_OUT_DISCARDS,                        /*!< Unsigned counter containing the number of ADP 6LoWPAN datagrams to be sent but discarded due to processing limitations. */
    R_ADP_STATS_LOWPAN_OUT_TRANSMITS,                       /*!< Unsigned counter containing the number of ADP 6LoWPAN datagrams supplied to the lower layers for transmission. */
    R_ADP_STATS_LOWPAN_OUT_SUCCESS,                         /*!< Unsigned counter containing the number of ADP 6LoWPAN datagrams supplied to the lower layers for transmission as success. */
    R_ADP_STATS_LOWPAN_OUT_FAILURE,                         /*!< Unsigned counter containing the number of ADP 6LoWPAN datagrams supplied to the lower layers for transmission as failure. */
    R_ADP_STATS_LOWPAN_OUT_FAILURE_NO_ACK,                  /*!< Unsigned counter containing the number of ADP 6LoWPAN datagrams supplied to the lower layers for transmission as failure status No Ack. */
    R_ADP_STATS_LOWPAN_OUT_IPV6_TRANSMITS,                  /*!< Unsigned counter containing the number of ADP 6LoWPAN IPv6 datagrams supplied to the lower layers for transmission. */
    R_ADP_STATS_LOWPAN_OUT_IPV6_SUCCESS,                    /*!< Unsigned counter containing the number of ADP 6LoWPAN IPv6 datagrams supplied to the lower layers for transmission as success. */
    R_ADP_STATS_LOWPAN_OUT_IPV6_FAILURE,                    /*!< Unsigned counter containing the number of ADP 6LoWPAN IPv6 datagrams supplied to the lower layers for transmission as failure. */
    R_ADP_STATS_LOWPAN_OUT_RELAY_TRANSMITS,                 /*!< Unsigned counter containing the number of ADP 6LoWPAN forwarding datagrams supplied to the lower layers for transmission. */
    R_ADP_STATS_LOWPAN_OUT_RELAY_SUCCESS,                   /*!< Unsigned counter containing the number of ADP 6LoWPAN forwarding datagrams supplied to the lower layers for transmission as success. */
    R_ADP_STATS_LOWPAN_OUT_RELAY_FAILURE,                   /*!< Unsigned counter containing the number of ADP 6LoWPAN forwarding datagrams supplied to the lower layers for transmission as failure. */

    /*
       GROUP: 6R_ADP_STATS_LOWPAN_LBP
    */
    R_ADP_STATS_LOWPAN_IN_LBP_RECEIVES,                     /*!< Unsigned counter containing the number of received LBP packets */
    R_ADP_STATS_LOWPAN_IN_LBP_DISCARDS,                     /*!< Unsigned counter containing the number of received LBP packets discarded due to errors in the processing. */
    R_ADP_STATS_LOWPAN_OUT_LBP_DISCARDS,                    /*!< Unsigned counter containing the number of LBP 6LoWPAN datagrams to be sent but discarded due to processing limitations. */
    R_ADP_STATS_LOWPAN_OUT_LBP_REQUESTS,                    /*!< Unsigned counter containing the number of sent LBP IPv6 packets supplied by the IPv6 layer. */
    R_ADP_STATS_LOWPAN_OUT_LBP_TRANSMITS,                   /*!< Unsigned counter containing the number of LBP 6LoWPAN datagrams supplied to the lower layers for transmission. */
    R_ADP_STATS_LOWPAN_OUT_LBP_SUCCESS,                     /*!< Unsigned counter containing the number of LBP 6LoWPAN datagrams supplied to the lower layers for transmission as sucess. */
    R_ADP_STATS_LOWPAN_OUT_LBP_FAILURE,                     /*!< Unsigned counter containing the number of LBP 6LoWPAN datagrams supplied to the lower layers for transmission as failure. */
    R_ADP_STATS_EAPPSK_SUCCESS,                             /*!< Unsigned counter containing the number of success count of EAP-PSK (include Join and Gmktransmission during Rekey)  */
    R_ADP_STATS_EAPPSK_FAILURE_MSG3_MACS,                   /*!< Unsigned counter containing the number of mismatch count of MACS on eap message 3  */
    R_ADP_STATS_EAPPSK_FAILURE_MSG3_EAX,                    /*!< Unsigned counter containing the number of mismatch count of EAX on eap message 3 */
    R_ADP_STATS_EAPPSK_FAILURE_MSG3_PCH,                    /*!< Unsigned counter containing the number of error count of PCH on eap message 3 */


    /*
       GROUP: 6R_ADP_STATS_LOWPAN_LOADNG_
    */
    R_ADP_STATS_LOWPAN_IN_LOADNG_RECEIVES,                  /*!< Unsigned counter containing the number of received LOADNG packets. */
    R_ADP_STATS_LOWPAN_IN_LOADNG_RECEIVES_RREQ,             /*!< Unsigned counter containing the number of received LOADNG RREQ packets. */
    R_ADP_STATS_LOWPAN_IN_LOADNG_RECEIVES_RREP,             /*!< Unsigned counter containing the number of received LOADNG RREP packets. */
    R_ADP_STATS_LOWPAN_IN_LOADNG_RECEIVES_RERR,             /*!< Unsigned counter containing the number of received LOADNG RERR packets. */
    R_ADP_STATS_LOWPAN_IN_LOADNG_RECEIVES_PREQ,             /*!< Unsigned counter containing the number of received LOADNG PREQ packets. */
    R_ADP_STATS_LOWPAN_IN_LOADNG_RECEIVES_PREP,             /*!< Unsigned counter containing the number of received LOADNG PREP packets. */
    R_ADP_STATS_LOWPAN_IN_LOADNG_DISCARDS,                  /*!< Unsigned counter containing the number of received LOADNG packets discarded due to errors in the processing. */
    R_ADP_STATS_LOWPAN_IN_LOADNG_DISCARDS_BY_BLACKLIST,     /*!< Unsigned counter containing the number of received LOADNG packets discarded by blacklisted neighbour tables. */
    R_ADP_STATS_LOWPAN_OUT_LOADNG_DISCARDS,                 /*!< Unsigned counter containing the number of LOADNG packets to be sent but discarded due to processing limitations. */
    R_ADP_STATS_LOWPAN_OUT_LOADNG_REQUESTS_RREQ,            /*!< Unsigned counter containing the number of generated LOADNG RREQ packets. */
    R_ADP_STATS_LOWPAN_OUT_LOADNG_REQUESTS_RREQ_REPAIR,     /*!< Unsigned counter containing the number of generated LOADNG RREQ packets as repair. */
    R_ADP_STATS_LOWPAN_OUT_LOADNG_REQUESTS_PREQ,            /*!< Unsigned counter containing the number of generated LOADNG PREQ packet. */
    R_ADP_STATS_LOWPAN_OUT_LOADNG_TRANSMITS,                /*!< Unsigned counter containing the number of sent LOADNG packets supplied to the lower layers for transmission. */
    R_ADP_STATS_LOWPAN_OUT_LOADNG_SUCCESS,                  /*!< Unsigned counter containing the number of sent LOADNG packets supplied to the lower layers for transmission as success. */
    R_ADP_STATS_LOWPAN_OUT_LOADNG_FAILURE,                  /*!< Unsigned counter containing the number of sent LOADNG packets supplied to the lower layers for transmission as failure. */
    R_ADP_STATS_LOWPAN_OUT_LOADNG_FAILURE_NOACK,            /*!< Unsigned counter containing the number of sent LOADNG packets supplied to the lower layers for transmission as failure which status is NoAck. */
    R_ADP_STATS_LOWPAN_OUT_LOADNG_FAILURE_NOACK_RREQ,       /*!< Unsigned counter containing the number of sent LOADNG packets supplied to the lower layers for transmission as failure which status is NoAck for unicastRREQ. */

    R_ADP_STATS_LOWPAN_OUT_LOADNG_REQUESTS_RREQ_FWD,        /*!< Unsigned counter containing the number of forwarding LOADNG RREQ packets. */
    R_ADP_STATS_CLUSTER_DISCARD_RREQ_FWD           ,        /*!< Unsigned counter containing the number of discarding LOADNG RREQ packets due to ETT38. */

    R_ADP_STATS_TRICKE_DISCARD,                             /*!< Unsigned counter containing the number of discarded packets due to "C > K". */
    R_ADP_STATS_LOWPAN_IN_LOADNG_RCV_RERR_UNKNOWNCODE,      /*!< Unsigned counter containing the number of received LOADNG RERR packets which error_code is unknown. */

    R_ADP_STATS_TXDISCARD_BLACK_PLC = 78,
    R_ADP_STATS_TXDISCARD_BLACK_RF,

    R_ADP_STATS_END,
} r_adp_statistics_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#endif /* R_ADP_STATISTICS_H */

