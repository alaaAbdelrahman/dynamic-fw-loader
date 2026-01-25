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
* File Name   : r_icmpv6.c
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
#include "r_config.h"
#include "r_io_vec.h"
#include "r_byte_swap.h"
#include "r_check_sum.h"
#include "r_ipv6_headers.h"
#include "r_icmp_v6.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_ICMP_CHECKSUM_IOVEC_LEN (8) /*!< Number of elements in io_vec for checksum calculation */

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
* Function Name: R_ICMP_CreateEchoMessage
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_icmp_result_t R_ICMP_CreateEchoMessage(r_iovec_class_t* const outIovec,
                                         const r_boolean_t request,
                                         const uint16_t identifier,
                                         const uint16_t sequenceNumber,
                                         const uint16_t echoDataLength,
                                         const uint8_t* const echoData,
                                         uint8_t* const buffer)
{
    r_icmp_result_t             result  = R_ICMP_RESULT_SUCCESS;
    r_icmp_echo_header_t* const pEheader = (r_icmp_echo_header_t*)buffer;
    uint8_t                     swapBuffer[4];

    if (((NULL == buffer) || (NULL == outIovec)) || ((NULL == echoData) && (echoDataLength > 0)))
    {
        return R_ICMP_RESULT_NULL_POINTER;
    }

    if (R_TRUE == request)
    {
        pEheader->header.type = R_ICMP_ECHO_REQUEST;
    }
    else if (R_FALSE == request)
    {
        pEheader->header.type = R_ICMP_ECHO_REPLY;
    }
    else
    {
        return R_ICMP_RESULT_INVALID_VALUE;
    }

    if (echoDataLength > R_ICMP_MAX_ECHO_DATA_LENGTH)
    {
        return R_ICMP_RESULT_INVALID_VALUE;
    }

    pEheader->header.code     = 0;
    R_BYTE_UInt16ToArr(0, pEheader->header.checksum);
    R_BYTE_UInt16ToArr(identifier, swapBuffer);
    R_memcpy((void*)&(pEheader->identifier), (void*)swapBuffer, 2);
    R_BYTE_UInt16ToArr(sequenceNumber, swapBuffer);
    R_memcpy((void*)&(pEheader->sequenceNumber), (void*)swapBuffer, 2);

    if (R_IO_VecAppendElement(outIovec, buffer, R_ICMP_ECHO_MESSAGE_HEADER_LENGTH) != R_RESULT_SUCCESS)
    {
        result = R_ICMP_RESULT_BUFFER_ERROR;
    }
    else if (echoDataLength > 0)
    {
        if (R_IO_VecAppendElement(outIovec, echoData, echoDataLength) != R_RESULT_SUCCESS)
        {
            result = R_ICMP_RESULT_BUFFER_ERROR;
        }
    }
    else
    {
        /**/
    }

    return result;
}
/******************************************************************************
   End of function  R_ICMP_CreateEchoMessage
******************************************************************************/


/******************************************************************************
* Function Name: R_ICMP_Checksum
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_icmp_result_t R_ICMP_Checksum(const r_ipv6_hdr_t* const p_ipv6Hdr,
                                const r_iovec_class_t* const p_icmpIoVec)
{
    uint16_t         checksum;
    uint8_t          pseudoHdrBuf[R_IPV6_HEADER_SIZE];
    r_icmp_header_t* p_icmpHdr;
    r_icmp_result_t  res = R_ICMP_RESULT_UNKNOWN;
    IOVEC_CREATE(io_vec, R_ICMP_CHECKSUM_IOVEC_LEN)
    IOVEC_INIT(io_vec)

    /* Safety checks */
    if ((NULL == p_ipv6Hdr) || (NULL == p_icmpIoVec))
    {
        return R_ICMP_RESULT_NULL_POINTER;
    }

    /* Check header type and number of elements in io_vec */
    if ((R_IPV6_NEXT_HDR_ICMPV6 == p_ipv6Hdr->nextHdr) && (p_icmpIoVec->used_elements <= R_ICMP_CHECKSUM_IOVEC_LEN))
    {
        /* Build pseudo header */
        if (R_IPV6_BuildPseudoHeader(p_ipv6Hdr, (uint16_t)p_icmpIoVec->total_length, pseudoHdrBuf) ==
            R_RESULT_SUCCESS)
        {
            /* Append pseudo header */
            if (R_IO_VecAppendElement(&io_vec, pseudoHdrBuf, sizeof(pseudoHdrBuf)) == R_RESULT_SUCCESS)
            {
                /* Append all elements of ICMP io_vec */
                if (R_IO_VecCopy(p_icmpIoVec, &io_vec) == R_RESULT_SUCCESS)
                {
                    /* Point to ICMP header, save local copy of, and clear existing checksum */
                    p_icmpHdr = (r_icmp_header_t*)(io_vec.pio_vec[1].paddress);
                    R_BYTE_UInt16ToArr(0, (uint8_t*)&(p_icmpHdr->checksum));

                    /* Calculate checksum */
                    if (R_CS_ComputeCheckSum(&io_vec, &checksum) == R_RESULT_SUCCESS)
                    {
                        /* Convert endianness. */
                        R_BYTE_UInt16ToArr(checksum, p_icmpHdr->checksum);

                        res = R_ICMP_RESULT_SUCCESS;
                    }
                }
            }
        }
    }
    else
    {
        /* Wrong next header or too small elements in local io_vec */
        res = R_ICMP_RESULT_INVALID_VALUE;
    }

    return res;
}
/******************************************************************************
   End of function  R_ICMP_Checksum
******************************************************************************/
