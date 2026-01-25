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
* File Name   : r_demo_common.h
*    @version
*        $Rev: 3203 $
*    @last editor
*        $Author: a0202438 $
*    @date
*        $Date:: 2017-05-10 11:25:47 +0900#$
* Description :
******************************************************************************/

#ifndef R_DEMO_COMMON_H
#define R_DEMO_COMMON_H

#include "r_demo_config.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum
{
    R_DEMO_UDP_MSG_REQ = 0x01,
    R_DEMO_UDP_MSG_REP = 0x02,
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    R_DEMO_UDP_MSG_RFREQ = 0x05,
    R_DEMO_UDP_MSG_RFREP = 0x06,
#endif
    R_DEMO_UDP_MSG_ETTCFGREQ = 0x07,
    R_DEMO_UDP_MSG_ETTCFGREP = 0x08,
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    R_DEMO_UDP_MSG_RFCONTX = 0x09,
#endif
} r_demo_udp_message_type_t;


/*!
    \struct r_demo_udp_responder_t
    \brief Structure for RF extension in UDP payload
 */
typedef struct
{
    uint8_t              msgType;       /*!< 0x05 is defined */
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    r_demo_rf_config_t   rfConfig;
#endif
} r_demo_udp_responder_t;

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
/*!
    \struct r_demo_udp_responder_t
    \brief Structure for RF extension in UDP payload
 */
typedef struct
{
    uint8_t              msgType;       /*!< 0x06 is defined */
    uint8_t              status;
} r_demo_udp_responder_rfrep_t;

#endif
typedef struct
{
    uint8_t              msgType;       /*!< 0x06 is defined */
    uint8_t              status;
} r_demo_udp_responder_ettcfg_t;


/******************************************************************************
Functions prototype
******************************************************************************/
/******************************************************************************
* Function Name     : R_DEMO_GenerateUdpFrame
* Description       : Generates an UDP frame
* Argument          : n : Length of the UDP payload
                      panId : The networks PAN ID
                      srcAddress : The short source address
                      dstAddress : The short destination address
* Return Value      : None
******************************************************************************/
/*!
   \fn void R_DEMO_GenerateUdpFrame(const uint16_t n,
                                    const uint16_t panId,
                                    const uint16_t srcAddress,
                                    const uint16_t dstAddress)
   \brief Generates an UDP frame
   \param[in] n : Length of the UDP payload
   \param[in] panId : The networks PAN ID
   \param[in] srcAddress : The short source address
   \param[in] dstAddress : The short destination address
 */
void R_DEMO_GenerateUdpFrame (const uint16_t n,
                              const uint16_t panId,
                              const uint16_t srcAddress,
                              const uint16_t dstAddress);

void R_DEMO_SendUdpFrame (const uint16_t n,
                              const uint16_t panId,
                              const uint16_t srcAddress,
                              const uint16_t dstAddress,
                              uint8_t* msg);


/*!
   \fn static void generate_icmp_request(const uint16_t n,
                                  const uint16_t panId,
                                  const uint16_t srcAddress,
                                  const uint16_t dstAddress);
   \brief Creates ICMP request frame
 */
void R_DEMO_GenerateIcmpRequest (const uint16_t n,
                                 const uint16_t panId,
                                 const uint16_t srcAddress,
                                 const uint16_t dstAddress);

/*!
   \fn void reply_icmp_request(const uint8_t* nsduIn,
                        const r_ipv6_hdr_t* ipv6HdrIn);
   \brief Creates ICMP response
 */
void R_DEMO_ReplyIcmpRequest (const uint8_t *      nsduIn,
                              const r_ipv6_hdr_t * ipv6HdrIn);

/*!
   \fn void reply_udp_frame(const uint8_t* nsduIn,
                     const r_ipv6_hdr_t* ipv6HdrIn);
   \brief Creates UDP response
 */
void R_DEMO_ReplyUdpFrame (const uint8_t *      nsduIn,
                           const r_ipv6_hdr_t * ipv6HdrIn);

/*!
   \fn void void reply_icmp_request_ext_headers(const uint8_t* nsduIn,
                                    const r_ipv6_hdr_t* ipv6HdrIn);
   \brief Creates ICMP response
 */
void R_DEMO_ReplyIcmpRequestExtHeaders (const uint8_t *      nsduIn,
                                        const r_ipv6_hdr_t * ipv6HdrIn);

#endif /* R_DEMO_COMMON_H */

