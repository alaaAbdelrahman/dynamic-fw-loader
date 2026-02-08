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
* File Name   : r_icmp_v6.h
*    @version
*        $Rev: 1918 $
*    @last editor
*        $Author: a5089763 $
*    @date  
*        $Date:: 2016-08-02 10:24:25 +0900#$
* Description : 
******************************************************************************/

#ifndef R_ICMP_V6_H
#define R_ICMP_V6_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_ICMP_PADDING_BOUNDARY                     (8u)                                     /*!< Alignment of the bytes in ICMP messaged */

#define R_ICMP_HEADER_LENGTH                        (4u)                                     /*!< Length of the pure ICMP header */
#define R_ICMP_ROUTER_SOLICITATION_HEADER_LENGTH    (8u)                                     /*!< Length of the header specific to RS messages */
#define R_ICMP_ROUTER_ADVERTISEMENT_HEADER_LENGTH   (16u)                                    /*!< Length of the header specific to RA messages */
#define R_ICMP_NEIGHBOR_SOLICITATION_HEADER_LENGTH  (24u)                                    /*!< Length of the header specific to NS messages */
#define R_ICMP_NEIGHBOR_ADVERTISEMENT_HEADER_LENGTH (24u)                                    /*!< Length of the header specific to NA messages */
#define R_ICMP_ERROR_MESSAGE_HEADER_LENGTH          (8u)                                     /*!< Length of the header for an ICMP error message (incl. information field) */
#define R_ICMP_ECHO_MESSAGE_HEADER_LENGTH           (8u)                                     /*!< Length of the header for an ICMP echo message (incl. identifier and sequence fields) */
#define R_ICMP_PIO_LENGTH                           (4u)                                     /*!< Length of the prefix information option (x 8 bytes) */

#define R_ICMP_NEIGHBOR_SOLICITATION_IOVEC_MIN      (3u)                                     /*!< Minimum number of elements in an iovec for Neighbor Solicitation message */
#define R_ICMP_NEIGHBOR_ADVERTISEMENT_IOVEC_MIN     (2u)                                     /*!< Minimum number of elements in an iovec for Neighbor Advertisement message */
#define R_ICMP_ECHO_MESSAGE_IOVEC_MIN               (1u)                                     /*!< Minimum number of elements in an iovec for Echo message */
#define R_ICMP_ERROR_MESSAGE_IOVEC_MIN              (1u)                                     /*!< Minimum number of elements in an iovec for Error message */
#define R_ICMP_IOVEC_LEN                            (R_ICMP_NEIGHBOR_SOLICITATION_IOVEC_MIN) /*!< Number of elements in an iovec for ICMP messages, including options */

/*!< Maximum number of bytes that can be added as arbitrary data to an echo request or reply, without including ICMP header */
#define R_ICMP_MAX_ECHO_DATA_LENGTH                 (R_MAX_MTU_SIZE - R_IPV6_HEADER_SIZE - R_ICMP_HEADER_LENGTH)

/*!< Maximum number of bytes that can be added as arbitrary data to an error message, including ICMP header */
#define R_ICMP_MAX_ERROR_DATA_LENGTH                (R_MAX_MTU_SIZE - R_IPV6_HEADER_SIZE)

/*!< Maximum number of bytes from the invoking packet that are allowed to be included in an ICMP error message
 * without including ICMP header length (must be a multiple of R_ICMP_PADDING_BOUNDARY) */
#define R_ICMP_MAX_INVOKING_PACKET_LENGTH           (((R_ICMP_MAX_ERROR_DATA_LENGTH - R_ICMP_HEADER_LENGTH - 4u) >> 3u) << 3u)


/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
   Enumeration definitions
******************************************************************************/

/*!
    \enum r_icmp_result_t
    \brief Enumeration type for the potential results of function calls in relation to ICMP
 */
typedef enum
{
    R_ICMP_RESULT_SUCCESS       = 0, /*!< Processing successful */
    R_ICMP_RESULT_FAILED        = 1, /*!< Processing failed */
    R_ICMP_RESULT_INVALID_VALUE = 2, /*!< Invalid value of data */
    R_ICMP_RESULT_NULL_POINTER  = 3, /*!< Input to a function was null pointer */
    R_ICMP_RESULT_BUFFER_ERROR  = 4, /*!< Some error occurred in buffer usage */
    R_ICMP_RESULT_UNKNOWN       = 5, /*!< Default value to set before proper values have been set */

} r_icmp_result_t;

/*!
    \enum r_icmp_type_t
    \brief Enumeration type for the defined type values of different ICMP messages
 */
typedef enum
{
    R_ICMP_ERROR_DESTINATION_UNREACHABLE   = 1,   /*!< Destination unreachable error type */
    R_ICMP_ERROR_PACKET_TOO_BIG            = 2,   /*!< Packet too big error type */
    R_ICMP_ERROR_TIME_EXCEEDED             = 3,   /*!< Time exceeded error type */
    R_ICMP_ERROR_PARAMETER_PROBLEM         = 4,   /*!< Parameter problem error type */
    R_ICMP_PRIVATE_EXPERIMENT_ERR_100      = 100, /*!< Private experimentation (100) */
    R_ICMP_PRIVATE_EXPERIMENT_ERR_101      = 101, /*!< Private experimentation (101) */
    R_ICMP_RESERVED_ERR_EXPANSION          = 127, /*!< Reserved for expansion of ICMPv6 error messages */
    R_ICMP_ECHO_REQUEST                    = 128, /*!< Echo Request type */
    R_ICMP_ECHO_REPLY                      = 129, /*!< Echo Reply type */
    R_ICMP_ROUTER_SOLICITATION             = 133, /*!< Router Solicitation type */
    R_ICMP_ROUTER_ADVERTISEMENT            = 134, /*!< Router Advertisement type */
    R_ICMP_NEIGHBOR_SOLICITATION           = 135, /*!< Neighbor Solicitation type */
    R_ICMP_NEIGHBOR_ADVERTISEMENT          = 136, /*!< Neighbor Advertisement type */
    R_ICMP_RPL                             = 155, /*!< RPL Control Message type */
    R_ICMP_PRIVATE_EXPERIMENTATION_INF_200 = 200, /*!< Private experimentation (200) */
    R_ICMP_PRIVATE_EXPERIMENTATION_INF_201 = 201, /*!< Private experimentation (201) */
    R_ICMP_RESERVED_INF_EXPANSION          = 255, /*!< Reserved for expansion of ICMPv6 informational messages */

} r_icmp_type_t;

/*!
    \enum r_icmp_error_code_t
    \brief Enumeration type for the defined code values of a destination unreachable ICMP error messages
 */
typedef enum
{
    R_ICMP_ERROR_NO_ROUTE_TO_DESTINATION    = 0, /*!< [Destination Unreachable] No route to destination error code */
    R_ICMP_ERROR_HOP_LIMIT_EXCEEDED_TRANSIT = 0, /*!< [Time Exceeded]           Hop limit exceeded in transit */
    R_ICMP_ERROR_ERRONEOUS_FIELD            = 0, /*!< [Parameter Problem]       Erroneous header field encountered */
    R_ICMP_ERROR_COMM_WITH_DEST_PROHIBITED  = 1, /*!< [Destination Unreachable] Communication with destination is administratively prohibited error code */
    R_ICMP_ERROR_FRAG_REASS_TIME_EXCEEDED   = 1, /*!< [Time Exceeded]           Fragment reassembly time exceeded */
    R_ICMP_ERROR_UNRECOGNIZED_NEXT_HEADER   = 1, /*!< [Parameter Problem]       Unrecognized Next Header type encountered */
    R_ICMP_ERROR_BEYOND_SCOPE               = 2, /*!< [Destination Unreachable] Beyond scope of source address error code */
    R_ICMP_ERROR_UNRECOGNIZED_IPV6_OPTION   = 2, /*!< [Parameter Problem]       Unrecognized IPv6 option encountered */
    R_ICMP_ERROR_ADDRESS_UNREACHABLE        = 3, /*!< [Destination Unreachable] Address unreachable error code */
    R_ICMP_ERROR_PORT_UNREACHABLE           = 4, /*!< [Destination Unreachable] Port unreachable error code */
    R_ICMP_ERROR_SOURCE_ADDR_FAILED_INGRESS = 5, /*!< [Destination Unreachable] Source address failed ingress/egress policy */
    R_ICMP_ERROR_REJECT_ROUTE_TO_DEST       = 6, /*!< [Destination Unreachable] Reject route to destination error code */
    R_ICMP_ERROR_CODE_LAST

} r_icmp_error_code_t;

/*!
    \enum r_icmp_option_type_t
    \brief Enumeration type for the defined values of different ICMP options
 */

typedef enum
{
    R_ICMP_OPTION_SLLA               = 1, /*!< Source Link Layer Address Option */
    R_ICMP_OPTION_TLLA               = 2, /*!< Target Link Layer Address Option */
    R_ICMP_OPTION_PREFIX_INFORMATION = 3, /*!< Prefix Information Option */
    R_ICMP_OPTION_REDIRECTED_HEADER  = 4, /*!< Redirected Header Option */
    R_ICMP_OPTION_MTU                = 5, /*!< MTU Option */

} r_icmp_option_type_t;

/******************************************************************************
   Message Type Definition
 *******************************************************************************/

/*!
    \struct r_icmp_header_t
    \brief Structure for the header of an ICMP message
 */
typedef struct
{
    uint8_t  type;        /*!< ICMPv6 information message type */
    uint8_t  code;        /*!< Identifies the code of control message */
    uint8_t  checksum[2]; /*!< one's complement sum of the entire ICMPv6 including the Pseudo-header, followed by complementing the checksum itself. */

} r_icmp_header_t; /*!< Structure for the base ICMPv6 header */

/*!
    \struct r_icmp_echo_header_t
    \brief Structure for the header of ICMP echo request and reply messages
 */
typedef struct
{
    r_icmp_header_t header;                                /*!< ICMPv6 header */
    uint16_t        identifier;                            /*!< An identifier to aid in matching echo requests and replies */
    uint16_t        sequenceNumber;                        /*!< A sequence number to aid in matching echo requests and replies */
    uint8_t         echoData[R_ICMP_MAX_ECHO_DATA_LENGTH]; /*!< Arbitrary data (length must be a multiple of R_ICMP_PADDING_BOUNDARY) */

} r_icmp_echo_header_t;

/*!
    \struct r_icmp_option_t
    \brief Structure for the contents of an ICMP option
 */
typedef struct
{
    uint8_t type;        /*!< The ICMPv6 option type */
    uint8_t length;      /*!< The length of the option field in units of 8 octets */
    uint8_t payload[62]; /*!< The actual content of the option */

} r_icmp_option_t;


/******************************************************************************
Functions prottype
******************************************************************************/
/*!
    r_icmp_result_t R_ICMP_CreateEchoMessage (r_iovec_class_t* outIovec, r_boolean_t request, uint16_t identifier, uint16_t sequenceNumber, uint16_t echoDataLength, uint8_t* echoData, uint8_t* buffer);
        \brief This function creates an ICMP error message and appends it to the iovec passed to it
        \param[in] outIovec iovec that will be appended with the newly created error message
        \param[in] request Boolean to decide whether the message to be created is a request (R_TRUE) or a reply (R_FALSE)
        \param[in] identifier An identifier to aid in matching echo requests and replies
        \param[in] sequenceNumber A sequence number to aid in matching echo requests and replies
        \param[in] echoDataLength The number of bytes in the arbitrary echo data
        \param[in] echoData Zero or more bytes of arbitrary data
        \param[out] buffer buffer to be used for assembling the echo request
        \return R_ICMP_RESULT_SUCCESS, R_ICMP_RESULT_NULL_POINTER, R_ICMP_RESULT_BUFFER_ERROR or R_ICMP_RESULT_INVALID_LENGTH
 */
r_icmp_result_t R_ICMP_CreateEchoMessage(r_iovec_class_t* const outIovec,
                                         const r_boolean_t request,
                                         const uint16_t identifier,
                                         const uint16_t sequenceNumber,
                                         const uint16_t echoDataLength,
                                         const uint8_t* const echoData,
                                         uint8_t* const buffer);

/*!
    r_icmp_result_t r_icmp_result_t R_ICMP_Checksum(const r_ipv6_hdr_t* const p_ipv6Hdr,
                                                    const r_iovec_class_t* const p_icmpIoVec)
        \brief This function calculates the checksum of an ICMP message from an iovec containing it
        \param[in]  p_ipv6Hdr    Pointer to an IPv6 header for the ICMP message
        \param[in]  p_icmpIoVec  Pointer to an iovec containing the ICMP message
        \return R_ICMP_RESULT_SUCCESS, R_ICMP_RESULT_NULL_POINTER, R_ICMP_RESULT_INVALID_VALUE or R_ICMP_RESULT_UNKNOWN
 */
r_icmp_result_t R_ICMP_Checksum(const r_ipv6_hdr_t* const p_ipv6Hdr,
                                const r_iovec_class_t* const p_icmpIoVec);

/*!
    r_icmp_result_t R_ICMP_IsValid(const r_ipv6_hdr_t* const p_ipv6Hdr,
                                   const r_iovec_class_t* const p_icmpIoVec,
                                   uint8_t* const p_code,
                                   r_boolean_t* const p_result)
        \brief This function checks if an ICMP message is valid or not. This includes
               checking the checksum and that the code is 0
        \param[in]  p_ipv6Hdr   Pointer to an IPv6 header for the ICMP message
        \param[in]  p_icmpIoVec Pointer to an iovec containing the ICMP message
        \param[out] p_code      Pointer to a location where to store the ICMP code
        \param[out] p_result    Pointer to a location where to store the result of the check
        \return R_ICMP_RESULT_SUCCESS or R_ICMP_RESULT_NULL_POINTER
 */
r_icmp_result_t R_ICMP_IsValid(const r_ipv6_hdr_t* const p_ipv6Hdr,
                               const r_iovec_class_t* const p_icmpIoVec,
                               uint8_t* const p_code,
                               r_boolean_t* const p_result);

/*!
    r_icmp_result_t R_ICMP_ExtractToIoVec(const uint8_t message[],
                                          const uint16_t length,
                                          r_iovec_class_t* const p_icmpIoVec)
        \brief This function extracts from a message buffer into an ioVec structure
        \param[in]  type        Type of ICMP message
        \param[in]  message     Message buffer, containing the ICMP message and possible options
        \param[in]  length      Length of the ICMP message
        \param[in]  p_icmpIoVec Pointer to an iovec on which to store the ICMP message
        \return R_ICMP_RESULT_SUCCESS or other error messages
 */
r_icmp_result_t R_ICMP_ExtractToIoVec(const r_icmp_type_t type,
                                      const uint8_t message[],
                                      const uint16_t length,
                                      r_iovec_class_t* const p_icmpIoVec);


#endif /* _R_ICMP_V6_H */
