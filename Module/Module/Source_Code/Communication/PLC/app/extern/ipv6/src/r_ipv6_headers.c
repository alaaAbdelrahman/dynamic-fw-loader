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
* File Name   : r_ipv6_headers.c
*    @version
*        $Rev: 3113 $
*    @last editor
*        $Author: a5089763 $
*    @date  
*        $Date:: 2017-04-14 21:00:02 +0900#$
* Description : 
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include <string.h>
#include "r_typedefs.h"
#include "r_ipv6_headers.h"
#include "r_byte_swap.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Private global variables and functions
******************************************************************************/
/******************************************************************************
Exported global variables
******************************************************************************/
/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/
/******************************************************************************
Functions
******************************************************************************/



/******************************************************************************
* Function Name: R_IPV6_PackHeader
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_IPV6_PackHeader(const r_ipv6_hdr_t* ipv6Hdr,
                             uint8_t output[],
                             uint16_t* sumWrittenBytes)
{
    if (((NULL == output) || (NULL == ipv6Hdr)) || (NULL == sumWrittenBytes))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    /* pack the header */

    /* leftmost 4 bits of the first byte contain the version */
    output[0] = (uint8_t)((uint32_t)(ipv6Hdr->version) << 0x04u);

    /* traffic class is on byte 0 and 1 (8 bits) */
    output[0] |= (uint8_t)((uint32_t)ipv6Hdr->trafficClass >> 0x04u);
    output[1]  = (uint8_t)(((uint32_t)(ipv6Hdr->trafficClass) & 0x0Fu) << 4u);

    /* 20 bit flow label takes bytes 1,2 and 3 */
    output[1] |= (uint8_t)(ipv6Hdr->flowLabel >> 16u);
    output[2]  = (uint8_t)(ipv6Hdr->flowLabel >> 8u);
    output[3]  = (uint8_t)ipv6Hdr->flowLabel;

    /* 2 byte payload length */
    output[4] = (uint8_t)((uint32_t)ipv6Hdr->payloadLength >> 8u);
    output[5] = (uint8_t)((uint32_t)ipv6Hdr->payloadLength & 0xFFu);

    /* 1 byte next header */
    output[6] = ipv6Hdr->nextHdr;

    /* 1 byte hop limit */
    output[7] = ipv6Hdr->hopLimit;

    R_memcpy(&output[8], ipv6Hdr->src, 16);  //16 byte source address
    R_memcpy(&output[24], ipv6Hdr->dst, 16); //16 byte destination address

    *sumWrittenBytes = (uint16_t)((*sumWrittenBytes) + 40u);

    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_IPV6_PackHeader
******************************************************************************/


/******************************************************************************
* Function Name: R_IPV6_UnpackHeader
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_IPV6_UnpackHeader(const uint8_t input[],
                               r_ipv6_hdr_t* ipv6Hdr)
{
    if ((NULL == input) || (NULL == ipv6Hdr))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    /* unpack the header */

    /* leftmost 4 bits of the first byte contain the version */
    ipv6Hdr->version = input[0] >> 0x04u;

    /* traffic class is on byte 0 and 1 (8 bits) */
    ipv6Hdr->trafficClass = (uint8_t)((((uint32_t)input[0] & 0x0Fu) << 0x04u) | ((uint32_t)input[1] >> 0x04u));

    /* 20 bit flow label takes bytes 1,2 and 3 */
    ipv6Hdr->flowLabel = ((((uint32_t)input[1] & 0x0Fu) << 16uL) | ((uint32_t)input[2] << 8uL)) | (uint32_t)input[3];

    /* 2 byte payload length */
    ipv6Hdr->payloadLength = (uint16_t)(((uint32_t)input[4] << 8uL) | (uint32_t)input[5]);

    /* 1 byte next header */
    ipv6Hdr->nextHdr = input[6]; /* allow this kind of casting here */ //PRQA S 1482, 4342

    /* 1 byte hop limit */
    ipv6Hdr->hopLimit = input[7];

    R_memcpy(ipv6Hdr->src, &input[8], 16);  //16 byte source address
    R_memcpy(ipv6Hdr->dst, &input[24], 16); //16 byte destination address

    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_IPV6_UnpackHeader
******************************************************************************/

/******************************************************************************
* Function Name: R_IPV6_PackTrafficClassHeader
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_IPV6_PackTrafficClassHeader(const r_ipv6_traffic_class_t* trafficClass,
                                              uint8_t output[])
{
    /* Parameter check */
    if ((NULL == output) || (NULL == trafficClass))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    /* leftmost 6 bits of the first byte contain the DSCP (see RFC3168) */
    output[0] = (uint8_t)((uint32_t)trafficClass->dscp << 0x02u); /* DSCP */

    /* other 2 bits contain the ECN */
    output[0] = (uint8_t)(output[0] | (trafficClass->ecn & 0x03u)); /* ECN */

    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_IPV6_PackTrafficClassHeader
******************************************************************************/


/******************************************************************************
* Function Name: R_IPV6_UnpackTrafficClassHeader
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_IPV6_UnpackTrafficClassHeader(const uint8_t input[],
                                                r_ipv6_traffic_class_t* trafficClass)
{
    if ((NULL == input) || (NULL == trafficClass))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    /* leftmost 6 bits of the first byte contain the DSCP (see RFC3168) */
    trafficClass->dscp = input[0] >> 0x02u;

    /* other 2 bits contain the ECN */
    trafficClass->ecn = input[0] & 0x03u;

    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_IPV6_UnpackTrafficClassHeader
******************************************************************************/


/******************************************************************************
* Function Name: R_IPV6_BuildPseudoHeader
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_IPV6_BuildPseudoHeader(const r_ipv6_hdr_t* const p_ipv6Hdr,
                                         const uint16_t length,
                                         uint8_t p_buffer[])
{
    r_result_t res = R_RESULT_FAILED;

    /* Safety checks */
    if ((NULL != p_ipv6Hdr) && (NULL != p_buffer))
    {
        R_memcpy(p_buffer, p_ipv6Hdr->src, 16);
        R_memcpy(&(p_buffer[16]), p_ipv6Hdr->dst, 16);
        R_BYTE_UInt32ToArr((uint32_t)length, &(p_buffer[32]));
        R_memset(&(p_buffer[36]), 0, 3);
        p_buffer[39] = p_ipv6Hdr->nextHdr;

        /* Update return value */
        res = R_RESULT_SUCCESS;
    }

    return res;
}
/******************************************************************************
   End of function  R_IPV6_BuildPseudoHeader
******************************************************************************/
