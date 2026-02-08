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
 * File Name   : r_ipv6_headers.h
 * Version     : 1.0
 * Description : This module contains the header packing and unpacking for IPv6
 ******************************************************************************/
/*!
   \file      r_ipv6_headers.h
   \version   1.0
   \brief     This module contains the header packing and unpacking for IPv6
 */

#ifndef R_IPV6_HEADERS_H
#define R_IPV6_HEADERS_H

/******************************************************************************
   Macro definitions
******************************************************************************/
#define R_IPV6_HEADER_SIZE     (40u) /*!< IPV6 header length */

/******************************************************************************
   Typedef definitions
******************************************************************************/

/*!
    \enum r_ipv6_next_header_t
    \brief IPv6 next header enumeration
 */
typedef enum
{
    R_IPV6_NEXT_HDR_HOP_BY_HOP  = 0x00, //!< Hop by hop options extension header
    R_IPV6_NEXT_HDR_TCP         = 0x06, //!< TCP header
    R_IPV6_NEXT_HDR_UDP         = 0x11, //!< UDP header
    R_IPV6_NEXT_HDR_IPV6        = 0x29, //!< IPv6 header
    R_IPV6_NEXT_HDR_ROUTING     = 0x2B, //!< Routing header
    R_IPV6_NEXT_HDR_FRAGMENT    = 0x2C, //!< Fragment header
    R_IPV6_NEXT_HDR_ICMPV6      = 0x3A, //!< ICMPv6 header
    R_IPV6_NEXT_HDR_DST_OPTIONS = 0x3C, //!< Destination options
    R_IPV6_NEXT_HDR_NONE        = 0x59, //!< No next header
    R_IPV6_NEXT_HDR_MOBILITY    = 0x87  //!< Mobility header

} r_ipv6_next_header_t;

/*!
    \struct r_ipv6_hdr_t
    \brief IPv6 header structure
 */
typedef struct
{
    uint32_t             flowLabel;     //!< Flow Label
    uint16_t             payloadLength; //!< Length of the IPv6 payload, i.e., the rest of the packet following this IPv6 header, in bytes
    uint8_t              nextHdr;       //!< Next header type
    uint8_t              version;       //!< Version, must be set to 6
    uint8_t              trafficClass;  //!< Traffic class
    uint8_t              hopLimit;      //!< Hop limit
    uint8_t              src[16];       //!< 128 bit IPv6 source address
    uint8_t              dst[16];       //!< 128 bit IPv6 destination address

} r_ipv6_hdr_t;

/*!
    \struct r_ipv6_hdr_cast_t
    \brief Structure for the IPv6 header (to be casted to pointers)
 */
typedef struct
{
    uint8_t vtf[4];           //!< Version, traffic class and flow label
    uint8_t payloadLength[2]; //!< Payload length
    uint8_t nextHdr;          //!< Next header
    uint8_t hopLimit;         //!< Hop limit
    uint8_t src[16];          //!< Source address
    uint8_t dst[16];          //!< Destination address

} r_ipv6_hdr_cast_t;

/*!
    \struct r_ipv6_traffic_class_t
    \brief Traffic class encoding structure
 */
typedef struct
{
    uint8_t dscp;  //!< DSCP field
    uint8_t ecn;   //!< ECN field

} r_ipv6_traffic_class_t;

/******************************************************************************
   Exported global functions (to be accessed by other files)
******************************************************************************/

/*!
    \fn r_result_t R_IPV6_PackHeader(const r_ipv6_hdr_t* ipv6Hdr,
                                      uint8_t output[],
                                      uint16_t* sumWrittenBytes);
    \brief Packing for a IPv6 header
    \param[in] ipv6Hdr Pointer to a structure describing the IPv6 header
    \param[in] output  Pointer to a buffer where the header is to be written
    \param[in] sumWrittenBytes  Pointer to a variable where the number of written bytes to the output buffer is added
    \return R_IPV6_RESULT_ILLEGAL_NULL_POINTER or R_IPV6_RESULT_SUCCESS
 */
r_result_t R_IPV6_PackHeader(const r_ipv6_hdr_t * ipv6Hdr,
                                      uint8_t output[],
                                      uint16_t * sumWrittenBytes);

/*!
    \fn r_result_t R_IPV6_UnpackHeader(const uint8_t* input,
                                      r_ipv6_hdr_t* ipv6Hdr);
    \brief Unpacking for a IPv6 header
    \param[in] input  Pointer to a buffer where the packed header is located
    \param[in] ipv6Hdr Pointer to a structure where the IPv6 header will be unpacked
    \return R_IPV6_RESULT_ILLEGAL_NULL_POINTER, R_IPV6_RESULT_FAILED or R_IPV6_RESULT_SUCCESS
 */
r_result_t R_IPV6_UnpackHeader(const uint8_t* input,
                                        r_ipv6_hdr_t* ipv6Hdr);

/*!
    \fn r_result_t R_IPV6_PackTrafficClassHeader(const r_ipv6_traffic_class_t* trafficClass,
                                              uint8_t output[]);
    \brief Packing for the traffic class header
    \param[in] trafficClass Pointer to a structure describing the traffic class header
    \param[in] output  Pointer to a buffer where the header is to be written
    \return R_IPV6_RESULT_ILLEGAL_NULL_POINTER or R_IPV6_RESULT_SUCCESS
 */
r_result_t R_IPV6_PackTrafficClassHeader(const r_ipv6_traffic_class_t * trafficClass,
                                              uint8_t output[]);

/*!
    \fn r_result_t R_IPV6_UnpackTrafficClassHeader(const uint8_t input[],
                                                r_ipv6_traffic_class_t* trafficClass);
    \brief Unpacking for the traffic class header
    \param[in] input  Pointer to a buffer where the packed header is located
    \param[in] trafficClass Pointer to a structure where the header is going to be unpacked
    \return R_IPV6_RESULT_ILLEGAL_NULL_POINTER or R_IPV6_RESULT_SUCCESS
 */
r_result_t R_IPV6_UnpackTrafficClassHeader(const uint8_t input[],
                                                r_ipv6_traffic_class_t* trafficClass);

/*!
    \fn r_result_t R_IPV6_BuildPseudoHeader(const r_ipv6_hdr_t* const p_ipv6Hdr,
                                                 const uint16_t length,
                                                 uint8_t p_buffer[])
    \brief Builds an IPv6 pseudo header, useful for checksum calculations
    \param[in] p_ipv6Hdr    Pointer to a real IPv6 header on which the pseudo-header is based
    \param[in] length       Length of the data following the IPv6 header
    \param[in] p_buffer     Pointer to a buffer where the pseudo header is to be written
    \return R_IPV6_RESULT_SUCCESS or R_IPV6_RESULT_FAILED
 */
r_result_t R_IPV6_BuildPseudoHeader(const r_ipv6_hdr_t * const p_ipv6Hdr,
                                         const uint16_t length,
                                         uint8_t p_buffer[]);

#endif /* R_IPV6_HEADERS_H */
