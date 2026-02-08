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
* File Name   : r_udp_headers.c
*    @version
*        $Rev: 3835 $
*    @last editor
*        $Author: a5089763 $
*    @date  
*        $Date:: 2018-03-27 17:29:09 +0900#$
* Description : 
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include <string.h>
#include "r_typedefs.h"
#include "r_io_vec.h"
#include "r_ipv6_headers.h"
#include "r_udp_headers.h"
#include "r_check_sum.h"
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
* Function Name: R_UDP_PackHeader
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_UDP_PackHeader(const r_udp_hdr_t* udpHdr,
                                     uint8_t output[])
{
    if ((NULL == output) || (NULL == udpHdr))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    R_BYTE_UInt16ToArr(udpHdr->srcPort, output);
    R_BYTE_UInt16ToArr(udpHdr->dstPort, output + 2);
    R_BYTE_UInt16ToArr(udpHdr->length, output + 4);
    R_BYTE_UInt16ToArr(udpHdr->checkSum, output + 6);

    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_UDP_PackHeader
******************************************************************************/


/******************************************************************************
* Function Name: R_UDP_UnpackHeader
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_UDP_UnpackHeader(const uint8_t input[],
                                       r_udp_hdr_t* udpHdr)
{
    if ((NULL == input) || (NULL == udpHdr))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    udpHdr->srcPort  = R_BYTE_ArrToUInt16(input);
    udpHdr->dstPort  = R_BYTE_ArrToUInt16(input + 2);
    udpHdr->length   = R_BYTE_ArrToUInt16(input + 4);
    udpHdr->checkSum = R_BYTE_ArrToUInt16(input + 6);

    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_UDP_UnpackHeader
******************************************************************************/


/******************************************************************************
* Function Name: R_UDP_ComputeCheckSum
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_UDP_ComputeCheckSum(const uint8_t* ipv6SrcAddr,
                                          const uint8_t* ipv6DstAddr,
                                          const uint8_t* udpPayload,
                                          const r_udp_hdr_t* udpHdr,
                                          uint16_t* checkSum)
{
    uint8_t         pseudoHeaderLowerPart[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t         udpHdrBuffer[8];
    r_result_t      result;
    r_udp_hdr_t     localUdpHdr;
    uint16_t        tmpSum;
    IOVEC_CREATE(iovec, 6)
    IOVEC_INIT(iovec)

    if ((((NULL == ipv6SrcAddr) || (NULL == ipv6DstAddr)) || ((NULL == udpPayload) || (NULL == udpHdr))) || (NULL == checkSum))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    if (udpHdr->length < 8u) /* 8 is the minimum length, i.e. only UDP header */
    {
        return R_RESULT_FAILED;
    }

    localUdpHdr.srcPort  = udpHdr->srcPort;
    localUdpHdr.dstPort  = udpHdr->dstPort;
    localUdpHdr.length   = udpHdr->length;
    localUdpHdr.checkSum = 0;

    /* prepare the iovec for the checksum computation */

    /* add IPv6 source address */
    result = R_IO_VecAppendElement(&iovec, ipv6SrcAddr, 16);

    if (R_RESULT_SUCCESS == result)
    {
        result = R_IO_VecAppendElement(&iovec, ipv6DstAddr, 16);
    }

    if (R_RESULT_SUCCESS == result)
    {
        /* prepare lower part with length field and next header field (always UDP) */
        R_BYTE_UInt16ToArr(udpHdr->length, &pseudoHeaderLowerPart[2]);
        pseudoHeaderLowerPart[7] = (uint8_t)R_IPV6_NEXT_HDR_UDP;

        /* add lower part */
        result = R_IO_VecAppendElement(&iovec, pseudoHeaderLowerPart, 8);
    }

    if (R_RESULT_SUCCESS == result)
    {
        /* prepare UDP header */
        result = R_UDP_PackHeader(&localUdpHdr, udpHdrBuffer);
    }

    if (R_RESULT_SUCCESS == result)
    {
        result = R_IO_VecAppendElement(&iovec, udpHdrBuffer, 8);
    }

    if (R_RESULT_SUCCESS == result)
    {
        result = R_IO_VecAppendElement(&iovec, udpPayload, (uint16_t)(udpHdr->length - R_UDP_HEADER_SIZE));
    }

    if (R_RESULT_SUCCESS == result)
    {
        result = R_CS_ComputeCheckSum(&iovec, &tmpSum);
    }

    if (R_RESULT_SUCCESS == result)
    {
        if (0 == tmpSum)
        {
            tmpSum = 0xFFFF;
        }
        *checkSum = tmpSum;
    }

    return result;
}
/******************************************************************************
   End of function  R_UDP_ComputeCheckSum
******************************************************************************/
