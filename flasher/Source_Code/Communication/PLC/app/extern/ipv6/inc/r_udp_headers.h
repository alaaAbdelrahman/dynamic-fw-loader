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
* http://www.renesas.com/disclaimer*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
 * File Name   : r_udp_headers.h
 * Version     : 1.0
 * Description : This module contains the header packing and unpacking for UDP
 ******************************************************************************/
/*!
   \file      r_udp_headers.h
   \version   1.0
   \brief     This module contains the header packing and unpacking for UDP
 */

#ifndef R_UDP_HEADERS_H
#define R_UDP_HEADERS_H

/******************************************************************************
   Macro definitions
******************************************************************************/
#define R_UDP_HEADER_SIZE (8u)  /*!< UDP header size in bytes */

/******************************************************************************
   Typedef definitions
*******************************************************************************/

/*!
    \struct r_udp_hdr_t
    \brief UDP header structure
 */
typedef struct
{
    uint16_t srcPort;  //! Source port number
    uint16_t dstPort;  //! Destination port number
    uint16_t length;   //! UDP Length
    uint16_t checkSum; //! Checksum

} r_udp_hdr_t;

/******************************************************************************
   Exported global variables
******************************************************************************/

/******************************************************************************
   Exported global functions (to be accessed by other files)
******************************************************************************/

/*!
    \fn r_result_t R_UDP_PackHeader(const r_udp_hdr_t* udpHdr,
                                     uint8_t output[]);
    \brief Packing for the UDP header
    \param[in] udpHdr Pointer to a structure describing the UDP header
    \param[in] output  Pointer to a buffer where header is going to be packed
    \return R_IPV6_RESULT_ILLEGAL_NULL_POINTER or R_IPV6_RESULT_SUCCESS
 */
r_result_t R_UDP_PackHeader(const r_udp_hdr_t * udpHdr,
                                     uint8_t output[]);

/*!
    \fn r_result_t R_UDP_UnpackHeader(const uint8_t* input,
                                     r_udp_hdr_t* udpHdr);
    \brief Unpacking for the UDP header
    \param[in] input  Pointer to a buffer containing the packed UDP header
    \param[in] udpHdr Pointer to a structure where the UDP header will be unpacked
    \return R_IPV6_RESULT_ILLEGAL_NULL_POINTER or R_IPV6_RESULT_SUCCESS
 */
r_result_t R_UDP_UnpackHeader(const uint8_t* input,
                                       r_udp_hdr_t* udpHdr);

/*!
    \fn r_result_t R_UDP_ComputeCheckSum(const uint8_t* ipv6SrcAddr,
                                      const uint8_t* ipv6DstAddr,
                                      const uint8_t* udpPayload,
                                      const r_udp_hdr_t* udpHdr,
                                      uint16_t* checkSum);
    \brief UDP checksum computation function
    \param[in] ipv6SrcAddr  Pointer to a buffer containing the IPv6 source address
    \param[in] ipv6DstAddr  Pointer to a buffer containing the IPv6 destination address
    \param[in] udpPayload  Pointer to a buffer containing the UDP payload
    \param[in] udpHdr Pointer to a structure containing the unpacked UDP header
    \param[out] checkSum Pointer to a location where the checksum will be written to
    \return R_IPV6_RESULT_ILLEGAL_NULL_POINTER or R_IPV6_RESULT_SUCCESS
 */
r_result_t R_UDP_ComputeCheckSum(const uint8_t* ipv6SrcAddr,
                                          const uint8_t* ipv6DstAddr,
                                          const uint8_t* udpPayload,
                                          const r_udp_hdr_t* udpHdr,
                                          uint16_t* checkSum);

#endif /* R_IPV6_HEADERS_H */
