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
* File Name   : r_demo_common.c
*    @version
*        $Rev: 4801 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2019-08-01 13:37:33 +0900#$
* Description :
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include <stdlib.h>
#include "r_typedefs.h"
#include "r_config.h"
#include "r_byte_swap.h"
#include "r_c3sap_api.h"

#include "r_g3mac_statistics.h"
#include "r_io_vec.h"
#include "r_ipv6_headers.h"
#include "r_udp_headers.h"
#include "r_icmp_v6.h"
#include "r_check_sum.h"
#include "r_demo_app.h"
#include "r_demo_common.h"
#include "r_demo_api.h"
#include "r_stdio_api.h"
#include "r_demo_parameters.h"


/******************************************************************************
Macro definitions
******************************************************************************/
#define R_ICMP_ID_OFFSET                   (4u)     /*!< ICMP header offset for identifier */
#define R_ICMP_SEQ_OFFSET                  (6u)     /*!< ICMP header offset for sequence number */
/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Private global variables and functions
******************************************************************************/
/******************************************************************************
* Function Name: generate_linklocal_ipv6_address
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void generate_linklocal_ipv6_address (const uint16_t panId,
                                             const uint16_t address,
                                             uint8_t *      pAddr);

/******************************************************************************
* Function Name: handle_remote_getset_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static r_result_t handle_remote_getset_req (const uint8_t *      nsduIn,
                                            const r_ipv6_hdr_t * ipv6HdrIn,
                                            uint16_t *           ipPayloadLength,
                                            r_boolean_t *        allFramesSent);
/******************************************************************************
Exported global variables
******************************************************************************/
extern r_demo_entity_t g_demo_entity;
extern r_demo_buff_t   g_demo_buff;
extern r_demo_config_t g_demo_config;

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Functions
******************************************************************************/
/******************************************************************************
* Function Name: R_DEMO_GenerateUdpFrame
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_DEMO_GenerateUdpFrame (const uint16_t n,
                              const uint16_t panId,
                              const uint16_t srcAddress,
                              const uint16_t dstAddress)
{

    uint16_t     i;
    uint16_t     sumWrittenBytes;

    r_ipv6_hdr_t ipv6Hdr;
    r_udp_hdr_t  udpHdr;

    /* Set payload to increasing sequence */
    for (i = 0; i < n; i++)
    {
        *(uint8_t *)(((g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) + R_UDP_HEADER_SIZE) + i) = (uint8_t)(i % 256);
    }

    /* Prepare NSDU */
    ipv6Hdr.version       = 0x06;
    ipv6Hdr.trafficClass  = 0x00;
    ipv6Hdr.flowLabel     = 0x00;
    ipv6Hdr.payloadLength = (uint16_t)(R_UDP_HEADER_SIZE + n);
    ipv6Hdr.nextHdr       = R_IPV6_NEXT_HDR_UDP; // UDP
    ipv6Hdr.hopLimit      = 0x01;
    generate_linklocal_ipv6_address (panId, srcAddress, ipv6Hdr.src);

    if (0xFFFF == dstAddress)
    {
        /* All nodes address (broadcast) */
        ipv6Hdr.dst[0]  = 0xFF;
        ipv6Hdr.dst[1]  = 0x02;
        ipv6Hdr.dst[2]  = 0x00;
        ipv6Hdr.dst[3]  = 0x00;
        ipv6Hdr.dst[4]  = 0x00;
        ipv6Hdr.dst[5]  = 0x00;
        ipv6Hdr.dst[6]  = 0x00;
        ipv6Hdr.dst[7]  = 0x00;
        ipv6Hdr.dst[8]  = 0x00;
        ipv6Hdr.dst[9]  = 0x00;
        ipv6Hdr.dst[10]  = 0x00;
        ipv6Hdr.dst[11]  = 0x00;
        ipv6Hdr.dst[12]  = 0x00;
        ipv6Hdr.dst[13]  = 0x00;
        ipv6Hdr.dst[14]  = 0x00;
        ipv6Hdr.dst[15]  = 0x01;
    }
    else
    {
        /* Group addresses (multicast) Sec. 9 RFC4944 */
        generate_linklocal_ipv6_address (panId, dstAddress, ipv6Hdr.dst);
    }

    udpHdr.srcPort = 0xABCD;
    udpHdr.dstPort = 0xF0BF;
    udpHdr.length = (uint16_t)(R_UDP_HEADER_SIZE + n);

    /* Pack IPv6 header. */
    if (R_IPV6_PackHeader (&ipv6Hdr,
                           g_demo_buff.Nsdu,
                           &sumWrittenBytes) == R_RESULT_SUCCESS)
    {
        /* Compute UDP checksum. */
        if (R_UDP_ComputeCheckSum (&ipv6Hdr.src[0],
                                   &ipv6Hdr.dst[0],
                                   (g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) + R_UDP_HEADER_SIZE,
                                   &udpHdr,
                                   &udpHdr.checkSum) == R_RESULT_SUCCESS)
        {
            /* Pack UDP header. */
            if (R_UDP_PackHeader (&udpHdr,
                                  g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) == R_RESULT_SUCCESS)
            {
                /* Set data request */
                r_adp_adpd_data_req_t   req;
                r_adp_adpd_data_cnf_t * pcnf;

                req.discoverRoute   = g_demo_config.discoverRoute;
                req.qualityOfService   = g_demo_config.qualityOfService;
                req.pNsdu              = g_demo_buff.Nsdu;
                req.nsduLength     = (uint16_t)((n + R_IPV6_HEADER_SIZE) + R_UDP_HEADER_SIZE);
                req.nsduHandle     = g_demo_entity.nsduHandle++;

                /* Call ADPD-DATA function. */
                R_DEMO_AdpdData (R_DEMO_G3_USE_PRIMARY_CH, &req, &pcnf);

            }
        }
    }
} /* R_DEMO_GenerateUdpFrame */
/******************************************************************************
   End of function  R_DEMO_GenerateUdpFrame
******************************************************************************/

void R_DEMO_SendUdpFrame (const uint16_t n,
                              const uint16_t panId,
                              const uint16_t srcAddress,
                              const uint16_t dstAddress,
                              uint8_t* msg)
{

    uint16_t     i;
    uint16_t     sumWrittenBytes;

    r_ipv6_hdr_t ipv6Hdr;
    r_udp_hdr_t  udpHdr;

    /* Set payload to increasing sequence */
    for (i = 0; i < n; i++)
    {
        *(uint8_t *)(((g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) + R_UDP_HEADER_SIZE) + i) = msg[i];
    }

    /* Prepare NSDU */
    ipv6Hdr.version       = 0x06;
    ipv6Hdr.trafficClass  = 0x00;
    ipv6Hdr.flowLabel     = 0x00;
    ipv6Hdr.payloadLength = (uint16_t)(R_UDP_HEADER_SIZE + n);
    ipv6Hdr.nextHdr       = R_IPV6_NEXT_HDR_UDP; // UDP
    ipv6Hdr.hopLimit      = 0x01;
    generate_linklocal_ipv6_address (panId, srcAddress, ipv6Hdr.src);

    if (0xFFFF == dstAddress)
    {
        /* All nodes address (broadcast) */
        ipv6Hdr.dst[0]  = 0xFF;
        ipv6Hdr.dst[1]  = 0x02;
        ipv6Hdr.dst[2]  = 0x00;
        ipv6Hdr.dst[3]  = 0x00;
        ipv6Hdr.dst[4]  = 0x00;
        ipv6Hdr.dst[5]  = 0x00;
        ipv6Hdr.dst[6]  = 0x00;
        ipv6Hdr.dst[7]  = 0x00;
        ipv6Hdr.dst[8]  = 0x00;
        ipv6Hdr.dst[9]  = 0x00;
        ipv6Hdr.dst[10]  = 0x00;
        ipv6Hdr.dst[11]  = 0x00;
        ipv6Hdr.dst[12]  = 0x00;
        ipv6Hdr.dst[13]  = 0x00;
        ipv6Hdr.dst[14]  = 0x00;
        ipv6Hdr.dst[15]  = 0x01;
    }
    else
    {
        /* Group addresses (multicast) Sec. 9 RFC4944 */
        generate_linklocal_ipv6_address (panId, dstAddress, ipv6Hdr.dst);
    }

    udpHdr.srcPort = 0xABCD;
    udpHdr.dstPort = 0xF0BF;
    udpHdr.length = (uint16_t)(R_UDP_HEADER_SIZE + n);

    /* Pack IPv6 header. */
    if (R_IPV6_PackHeader (&ipv6Hdr,
                           g_demo_buff.Nsdu,
                           &sumWrittenBytes) == R_RESULT_SUCCESS)
    {
        /* Compute UDP checksum. */
        if (R_UDP_ComputeCheckSum (&ipv6Hdr.src[0],
                                   &ipv6Hdr.dst[0],
                                   (g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) + R_UDP_HEADER_SIZE,
                                   &udpHdr,
                                   &udpHdr.checkSum) == R_RESULT_SUCCESS)
        {
            /* Pack UDP header. */
            if (R_UDP_PackHeader (&udpHdr,
                                  g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) == R_RESULT_SUCCESS)
            {
                /* Set data request */
                r_adp_adpd_data_req_t   req;
                r_adp_adpd_data_cnf_t * pcnf;

                req.discoverRoute   = g_demo_config.discoverRoute;
                req.qualityOfService   = g_demo_config.qualityOfService;
                req.pNsdu              = g_demo_buff.Nsdu;
                req.nsduLength     = (uint16_t)((n + R_IPV6_HEADER_SIZE) + R_UDP_HEADER_SIZE);
                req.nsduHandle     = g_demo_entity.nsduHandle++;

                /* Call ADPD-DATA function. */
                R_DEMO_AdpdData (R_DEMO_G3_USE_PRIMARY_CH, &req, &pcnf);

            }
        }
    }
} /* R_DEMO_GenerateUdpFrame */

/******************************************************************************
* Function Name: R_DEMO_ReplyUdpFrame
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_DEMO_ReplyUdpFrame (const uint8_t *      nsduIn,
                           const r_ipv6_hdr_t * ipv6HdrIn)
{
    uint16_t     ipPayloadLength;
    r_boolean_t  allFramesSent = R_FALSE;
    uint16_t     sumWrittenBytes;

    r_ipv6_hdr_t ipv6HdrOut;
    r_udp_hdr_t  udpHdrIn;
    r_udp_hdr_t  udpHdrOut;

    /* Create ioVec element. Two elements for message and dispatch. */
    IOVEC_CREATE (ioVec, 1)
    IOVEC_INIT (ioVec)

    /* Parse UDP header. */
    R_UDP_UnpackHeader (nsduIn + R_IPV6_HEADER_SIZE,
                        &udpHdrIn);

    /* Check if destination port is the expected one. */
    if (0xD000 == udpHdrIn.dstPort)
    {
        if (R_BYTE_ArrToUInt32 ((nsduIn + R_IPV6_HEADER_SIZE) + R_UDP_HEADER_SIZE) == R_CAP_SYNCWORD)
        {
            /* Handle remote get set request. */
            if (handle_remote_getset_req (nsduIn,
                                          ipv6HdrIn,
                                          &ipPayloadLength,
                                          &allFramesSent) != R_RESULT_SUCCESS)
            {
                /* Message creation failed, return. */
                return;
            }

        }
        else
        {
            return;
        }
    }
    else if (((0xF0BF == udpHdrIn.dstPort) ||
              (0xF0B8 == udpHdrIn.dstPort)) ||
             (0xF0B0 == udpHdrIn.dstPort))
    {
        /* Copy payload. */
        R_memcpy ((g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) + R_UDP_HEADER_SIZE,
                  (nsduIn + R_IPV6_HEADER_SIZE) + R_UDP_HEADER_SIZE,
                  ipv6HdrIn->payloadLength - R_UDP_HEADER_SIZE);

        /* Payload length identical to incoming message. */
        ipPayloadLength = ipv6HdrIn->payloadLength;


        if (R_DEMO_MODE_CERT == g_demo_config.appMode)
        {
            r_demo_udp_responder_t *pUdpResp = (r_demo_udp_responder_t*)((g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) + R_UDP_HEADER_SIZE);
            if (R_DEMO_UDP_MSG_REQ == pUdpResp->msgType)
            {
                pUdpResp->msgType = R_DEMO_UDP_MSG_REP;
            }
            else if (R_DEMO_UDP_MSG_REP == pUdpResp->msgType)
            {
                /* discard */
                return;
            }
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
            else if (R_DEMO_UDP_MSG_RFREQ == pUdpResp->msgType)
            {
                pUdpResp->msgType = R_DEMO_UDP_MSG_RFREP;
                ((r_demo_udp_responder_rfrep_t *)pUdpResp)->status = R_DEMO_RfBandSwitch(&pUdpResp->rfConfig);
                ipPayloadLength = R_UDP_HEADER_SIZE + 2;
            }
            else if (R_DEMO_UDP_MSG_RFREP == pUdpResp->msgType)
            {
                /* discard */
                return;
            }
            else if (R_DEMO_UDP_MSG_RFCONTX == pUdpResp->msgType)
            {
                R_DEMO_RfContinuousTx();
            }
#endif
            else if (R_DEMO_UDP_MSG_ETTCFGREQ == pUdpResp->msgType)
            {
                pUdpResp->msgType = R_DEMO_UDP_MSG_ETTCFGREP;
                ((r_demo_udp_responder_ettcfg_t *)pUdpResp)->status = R_DEMO_SetIbForETTDLL353738(((r_demo_udp_responder_ettcfg_t *)pUdpResp)->status);
                ipPayloadLength = R_UDP_HEADER_SIZE + 2;
            }
            else if (R_DEMO_UDP_MSG_ETTCFGREP == pUdpResp->msgType)
            {
                /* discard */
                return;
            }
        }
    }
    else
    {
        /* ignore the port */
        return;
    }

    /* Prepare NSDU */
    ipv6HdrOut.version       = ipv6HdrIn->version;
    ipv6HdrOut.trafficClass = ipv6HdrIn->trafficClass;
    ipv6HdrOut.flowLabel     = ipv6HdrIn->flowLabel;
    ipv6HdrOut.payloadLength = ipPayloadLength;
    ipv6HdrOut.nextHdr       = ipv6HdrIn->nextHdr;
    ipv6HdrOut.hopLimit     = ipv6HdrIn->hopLimit;

    if (0xFF == ipv6HdrIn->dst[0])
    {
        /* Multicast case use LinlLocal src address */
        generate_linklocal_ipv6_address (g_demo_entity.panId, g_demo_entity.shortAddress, ipv6HdrOut.src);
    }
    else
    {
        R_memcpy (ipv6HdrOut.src, ipv6HdrIn->dst, 16);
    }
    R_memcpy (ipv6HdrOut.dst, ipv6HdrIn->src, 16);

    udpHdrOut.srcPort = udpHdrIn.dstPort;
    udpHdrOut.dstPort = udpHdrIn.srcPort;
    udpHdrOut.length = ipPayloadLength;

    /* Pack IPv6 header. */
    if (R_IPV6_PackHeader (&ipv6HdrOut,
                           g_demo_buff.Nsdu,
                           &sumWrittenBytes) == R_RESULT_SUCCESS)
    {

        /* Compute UDP checksum. */
        if (R_UDP_ComputeCheckSum (&ipv6HdrOut.src[0],
                                   &ipv6HdrOut.dst[0],
                                   (g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) + R_UDP_HEADER_SIZE,
                                   &udpHdrOut,
                                   &udpHdrOut.checkSum) == R_RESULT_SUCCESS)
        {
            /* Pack UDP header. */
            if (R_UDP_PackHeader (&udpHdrOut,
                                  g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) == R_RESULT_SUCCESS)
            {
                /* Set data request */
                r_adp_adpd_data_req_t   req;
                r_adp_adpd_data_cnf_t * cnf;

                req.discoverRoute       = R_TRUE;
                req.qualityOfService    = R_G3MAC_QOS_NORMAL;
                req.pNsdu               = g_demo_buff.Nsdu;
                req.nsduLength          = (uint16_t)(ipv6HdrOut.payloadLength + R_IPV6_HEADER_SIZE);
                req.nsduHandle          = g_demo_entity.nsduHandle++;

                /* Call ADPD-DATA function. */
                R_DEMO_AdpdData (R_DEMO_G3_USE_PRIMARY_CH, &req, &cnf);
            }
        }
    }
} /* R_DEMO_ReplyUdpFrame */
/******************************************************************************
   End of function R_DEMO_ReplyUdpFrame
******************************************************************************/

/******************************************************************************
* Function Name: R_DEMO_GenerateIcmpRequest
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_DEMO_GenerateIcmpRequest (const uint16_t n,
                                 const uint16_t panId,
                                 const uint16_t srcAddress,
                                 const uint16_t dstAddress)
{
    uint16_t     i;

    uint16_t     sumWrittenBytes;

    r_ipv6_hdr_t ipv6Hdr;

    /* Create ioVec element. Two elements for message and dispatch. */
    IOVEC_CREATE (ioVec, 2)
    IOVEC_INIT (ioVec)

    /* Set payload to 0xFF. */
    for (i = 0; i < n; i++)
    {
        *(uint8_t *)((g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) + (R_ICMP_ECHO_MESSAGE_HEADER_LENGTH + i)) = 0xFF;
    }

    /* Prepare NSDU */
    ipv6Hdr.version       = 0x06;
    ipv6Hdr.trafficClass  = 0x00;
    ipv6Hdr.flowLabel     = 0x00;
    ipv6Hdr.payloadLength = (uint16_t)(R_ICMP_ECHO_MESSAGE_HEADER_LENGTH + n);
    ipv6Hdr.nextHdr       = R_IPV6_NEXT_HDR_ICMPV6; // UDP
    ipv6Hdr.hopLimit      = 0x01;

    generate_linklocal_ipv6_address (panId, srcAddress, ipv6Hdr.src);

    if (0xFFFF == dstAddress)
    {
        /* All nodes address (broadcast) */
        ipv6Hdr.dst[0]  = 0xFF;
        ipv6Hdr.dst[1]  = 0x02;
        ipv6Hdr.dst[2]  = 0x00;
        ipv6Hdr.dst[3]  = 0x00;
        ipv6Hdr.dst[4]  = 0x00;
        ipv6Hdr.dst[5]  = 0x00;
        ipv6Hdr.dst[6]  = 0x00;
        ipv6Hdr.dst[7]  = 0x00;
        ipv6Hdr.dst[8]  = 0x00;
        ipv6Hdr.dst[9]  = 0x00;
        ipv6Hdr.dst[10]  = 0x00;
        ipv6Hdr.dst[11]  = 0x00;
        ipv6Hdr.dst[12]  = 0x00;
        ipv6Hdr.dst[13]  = 0x00;
        ipv6Hdr.dst[14]  = 0x00;
        ipv6Hdr.dst[15]  = 0x01;
    }
    else
    {
        /* Group addresses (multicast) Sec. 9 RFC4944 */
        generate_linklocal_ipv6_address (panId, dstAddress, ipv6Hdr.dst);
    }

    /* Pack IPv6 header. */
    if (R_IPV6_PackHeader (&ipv6Hdr,
                           g_demo_buff.Nsdu,
                           &sumWrittenBytes) == R_RESULT_SUCCESS)
    {
        /* Create echo message. */
        if (R_ICMP_CreateEchoMessage (&ioVec,
                                      R_TRUE,
                                      0x0102,
                                      0x0506,
                                      n,
                                      (g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) + R_ICMP_ECHO_MESSAGE_HEADER_LENGTH,
                                      g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) == R_ICMP_RESULT_SUCCESS)
        {
            /* Compute ICMP checksum. */
            if (R_ICMP_Checksum (&ipv6Hdr,
                                 &ioVec) == R_ICMP_RESULT_SUCCESS)
            {
                /* Set data request */
                r_adp_adpd_data_req_t   req;
                r_adp_adpd_data_cnf_t * cnf;

                req.discoverRoute   = g_demo_config.discoverRoute;
                req.qualityOfService   = g_demo_config.qualityOfService;
                req.pNsdu              = g_demo_buff.Nsdu;
                req.nsduLength     = (uint16_t)((n + R_IPV6_HEADER_SIZE) + R_ICMP_ECHO_MESSAGE_HEADER_LENGTH);
                req.nsduHandle     = g_demo_entity.nsduHandle++;

                /* Call ADPD-DATA function. */
                R_DEMO_AdpdData (R_DEMO_G3_USE_PRIMARY_CH, &req, &cnf);
            }
        }
    }
} /* R_DEMO_GenerateIcmpRequest */
/******************************************************************************
   End of function  R_DEMO_GenerateIcmpRequest
******************************************************************************/

/******************************************************************************
* Function Name: R_DEMO_ReplyIcmpRequest
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_DEMO_ReplyIcmpRequest (const uint8_t *      nsduIn,
                              const r_ipv6_hdr_t * ipv6HdrIn)
{
    uint16_t     sumWrittenBytes;
    uint16_t     ipPayloadLength;
    r_ipv6_hdr_t ipv6HdrOut;
    r_boolean_t  allFramesSent = R_FALSE;

    /* Create ioVec element. Two elements for message and dispatch. */
    IOVEC_CREATE (ioVec, 2)
    IOVEC_INIT (ioVec)

    /* Pointer check. */
    if ((NULL == nsduIn) ||
        (NULL == ipv6HdrIn))
    {
        return;
    }

    /* Loop until all response packets have been sent. Normally that is only
     a single one, but for larger logging buffers it could be more. */
    while (R_FALSE == allFramesSent)
    {
        /* By default we send only a single frame, so set flag to TRUE. */
        allFramesSent = R_TRUE;

        /* Check if this is an ICMP echo request. */
        if (0x80 == (*(nsduIn + R_IPV6_HEADER_SIZE)))
        {
            /* Check if this is a valid message. */
            if (R_BYTE_ArrToUInt32 ((nsduIn + R_IPV6_HEADER_SIZE) + R_ICMP_ECHO_MESSAGE_HEADER_LENGTH) == R_CAP_SYNCWORD)
            {
                /* Handle remote get set request. */
                if (handle_remote_getset_req (nsduIn,
                                              ipv6HdrIn,
                                              &ipPayloadLength,
                                              &allFramesSent) != R_RESULT_SUCCESS)
                {
                    /* Message creation failed, return. */
                    return;
                }
            }
            else
            {
                /* Copy payload. */
                R_memcpy ((g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) + R_ICMP_ECHO_MESSAGE_HEADER_LENGTH,
                          (nsduIn + R_IPV6_HEADER_SIZE) + R_ICMP_ECHO_MESSAGE_HEADER_LENGTH,
                          ipv6HdrIn->payloadLength - R_ICMP_ECHO_MESSAGE_HEADER_LENGTH);

                /* Payload length identical to incoming message. */
                ipPayloadLength = ipv6HdrIn->payloadLength;
            }

            /* Prepare NSDU */
            ipv6HdrOut.version       = ipv6HdrIn->version;
            ipv6HdrOut.trafficClass = ipv6HdrIn->trafficClass;
            ipv6HdrOut.flowLabel     = ipv6HdrIn->flowLabel;
            ipv6HdrOut.payloadLength = ipPayloadLength;
            ipv6HdrOut.nextHdr       = ipv6HdrIn->nextHdr;
            ipv6HdrOut.hopLimit     = ipv6HdrIn->hopLimit;

            if (0xFF == ipv6HdrIn->dst[0])
            {
                /* Multicast case use LinlLocal src address */
                generate_linklocal_ipv6_address (g_demo_entity.panId, g_demo_entity.shortAddress, ipv6HdrOut.src);
            }
            else
            {
                R_memcpy (ipv6HdrOut.src, ipv6HdrIn->dst, 16);
            }
            R_memcpy (ipv6HdrOut.dst, ipv6HdrIn->src, 16);

            /* Pack IPv6 header. */
            if (R_IPV6_PackHeader (&ipv6HdrOut,
                                   g_demo_buff.Nsdu,
                                   &sumWrittenBytes) == R_RESULT_SUCCESS)
            {
                /* Create echo message. */
                if (R_ICMP_CreateEchoMessage (&ioVec,
                                              R_FALSE,
                                              R_BYTE_ArrToUInt16 ((nsduIn + R_IPV6_HEADER_SIZE) + R_ICMP_ID_OFFSET),
                                              R_BYTE_ArrToUInt16 ((nsduIn + R_IPV6_HEADER_SIZE) + R_ICMP_SEQ_OFFSET),
                                              (uint16_t)(ipv6HdrIn->payloadLength - R_ICMP_ECHO_MESSAGE_HEADER_LENGTH),
                                              (g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) + R_ICMP_ECHO_MESSAGE_HEADER_LENGTH,
                                              g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) == R_ICMP_RESULT_SUCCESS)
                {
                    /* Compute ICMP checksum. */
                    if (R_ICMP_Checksum (&ipv6HdrOut,
                                         &ioVec) == R_ICMP_RESULT_SUCCESS)
                    {
                        /* Set data request */
                        r_adp_adpd_data_req_t   req;
                        r_adp_adpd_data_cnf_t * cnf;

                        req.discoverRoute   = R_TRUE;
                        req.qualityOfService   = R_G3MAC_QOS_NORMAL;
                        req.pNsdu              = g_demo_buff.Nsdu;
                        req.nsduLength     = (uint16_t)(ipv6HdrOut.payloadLength + R_IPV6_HEADER_SIZE);
                        req.nsduHandle     = g_demo_entity.nsduHandle++;

                        /* Call ADPD-DATA function. */
                        R_DEMO_AdpdData (R_DEMO_G3_USE_PRIMARY_CH, &req, &cnf);

                        /* Clear IOVEC for next iteration. */
                        R_IO_VecRese (&ioVec);
                    }
                }
            }
        }
    }
} /* R_DEMO_ReplyIcmpRequest */
/******************************************************************************
   End of function R_DEMO_ReplyIcmpRequest
******************************************************************************/

/******************************************************************************
* Function Name: R_DEMO_ReplyIcmpRequestExtHeaders
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_DEMO_ReplyIcmpRequestExtHeaders (const uint8_t *      nsduIn,
                                        const r_ipv6_hdr_t * ipv6HdrIn)
{
    uint16_t     sumWrittenBytes;
    uint16_t     realHopByHopHdrLength;
    uint16_t     realDspOptionsHdrLength;
    r_ipv6_hdr_t ipv6HdrOut;
    uint8_t      nextHdrLength;

    /* Create ioVec element. Two elements for message and dispatch. */
    IOVEC_CREATE (ioVec, 2)
    IOVEC_INIT (ioVec)

    /* get hop by hop header's length */
    nextHdrLength = nsduIn[R_IPV6_HEADER_SIZE + 1];              /* Length of this header in 8-octet units, not including the first 8 octets */
    realHopByHopHdrLength = (uint16_t)((nextHdrLength + 1) * 8); /* real length in bytes */

    /* get destination options header's length */
    nextHdrLength = nsduIn[(R_IPV6_HEADER_SIZE + realHopByHopHdrLength) + 1]; /* Length of this header in 8-octet units, not including the first 8 octets */
    realDspOptionsHdrLength = (uint16_t)((nextHdrLength + 1) * 8);            /* real length in bytes */

    /* Check if this is a request. */
    if (0x80 == (*((nsduIn + R_IPV6_HEADER_SIZE) + (realHopByHopHdrLength + realDspOptionsHdrLength))))
    {
        /* Prepare NSDU */
        ipv6HdrOut.version       = ipv6HdrIn->version;
        ipv6HdrOut.trafficClass = ipv6HdrIn->trafficClass;
        ipv6HdrOut.flowLabel     = ipv6HdrIn->flowLabel;
        ipv6HdrOut.payloadLength = (uint16_t)((ipv6HdrIn->payloadLength - realHopByHopHdrLength) - realDspOptionsHdrLength);
        ipv6HdrOut.nextHdr       = R_IPV6_NEXT_HDR_ICMPV6; /* echo reply */
        ipv6HdrOut.hopLimit     = ipv6HdrIn->hopLimit;

        if (0xFF == ipv6HdrIn->dst[0])
        {
            /* Multicast case use LinlLocal src address */
            generate_linklocal_ipv6_address (g_demo_entity.panId, g_demo_entity.shortAddress, ipv6HdrOut.src);
        }
        else
        {
            R_memcpy (ipv6HdrOut.src, ipv6HdrIn->dst, 16);
        }

        R_memcpy (ipv6HdrOut.dst, ipv6HdrIn->src, 16);

        /* Pack IPv6 header. */
        if (R_IPV6_PackHeader (&ipv6HdrOut,
                               g_demo_buff.Nsdu,
                               &sumWrittenBytes) == R_RESULT_SUCCESS)
        {
            /* Copy payload. */
            R_memcpy ((g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) + R_ICMP_ECHO_MESSAGE_HEADER_LENGTH,
                      ((nsduIn + R_IPV6_HEADER_SIZE) + (realHopByHopHdrLength + realDspOptionsHdrLength)) + R_ICMP_ECHO_MESSAGE_HEADER_LENGTH,
                      (size_t)((uint16_t)((ipv6HdrIn->payloadLength - realHopByHopHdrLength) - realDspOptionsHdrLength) - R_ICMP_ECHO_MESSAGE_HEADER_LENGTH));

            /* Create echo message. */
            if (R_ICMP_CreateEchoMessage (&ioVec,
                                          R_FALSE,
                                          R_BYTE_ArrToUInt16 ((((nsduIn + R_IPV6_HEADER_SIZE) + realHopByHopHdrLength) + realDspOptionsHdrLength) + R_ICMP_ID_OFFSET),
                                          R_BYTE_ArrToUInt16 ((((nsduIn + R_IPV6_HEADER_SIZE) + realHopByHopHdrLength) + realDspOptionsHdrLength) + R_ICMP_SEQ_OFFSET),
                                          (uint16_t)(((ipv6HdrIn->payloadLength - R_ICMP_ECHO_MESSAGE_HEADER_LENGTH) - realHopByHopHdrLength) - realDspOptionsHdrLength),
                                          (g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) + R_ICMP_ECHO_MESSAGE_HEADER_LENGTH,
                                          g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) == R_ICMP_RESULT_SUCCESS)
            {
                /* Compute ICMP checksum. */
                if (R_ICMP_Checksum (&ipv6HdrOut,
                                     &ioVec) == R_ICMP_RESULT_SUCCESS)
                {
                    /* Set data request */
                    r_adp_adpd_data_req_t   req;
                    r_adp_adpd_data_cnf_t * cnf;

                    req.discoverRoute   = R_TRUE;
                    req.qualityOfService   = R_G3MAC_QOS_NORMAL;
                    req.pNsdu              = g_demo_buff.Nsdu;
                    req.nsduLength     = (uint16_t)(ipv6HdrOut.payloadLength + R_IPV6_HEADER_SIZE);
                    req.nsduHandle     = g_demo_entity.nsduHandle++;

                    /* Call ADPD-DATA function. */
                    R_DEMO_AdpdData (R_DEMO_G3_USE_PRIMARY_CH, &req, &cnf);

                }
            }
        }
    }
} /* R_DEMO_ReplyIcmpRequestExtHeaders */
/******************************************************************************
   End of function R_DEMO_ReplyIcmpRequestExtHeaders
******************************************************************************/


/******************************************************************************
* Function Name: generate_linklocal_ipv6_address
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void generate_linklocal_ipv6_address (const uint16_t panId,
                                             const uint16_t address,
                                             uint8_t *      pAddr)
{
    pAddr[0]        = 0xFE; // SRC: Prefix 8*8 bits = 64 bits
    pAddr[1]        = 0x80;
    pAddr[2]        = 0x00;
    pAddr[3]        = 0x00;
    pAddr[4]        = 0x00;
    pAddr[5]        = 0x00;
    pAddr[6]        = 0x00;
    pAddr[7]        = 0x00;
    pAddr[8]        = (uint8_t)((panId >> 8) & 0x00FF); // 16-bit PAN ID
    pAddr[9]        = (uint8_t)(panId & 0x00FF);        // 16-bit PAN ID
    pAddr[10]        = 0x00;                            // padding
    pAddr[11]        = 0xFF;
    pAddr[12]        = 0xFE;
    pAddr[13]        = 0x00;
    pAddr[14]        = (uint8_t)((address >> 8) & 0x00FF); // 16 bits short addr
    pAddr[15]        = (uint8_t)(address & 0x00FF);        // 16 bits short addr

} /* generate_linklocal_ipv6_address */
/******************************************************************************
   End of function  generate_linklocal_ipv6_address
******************************************************************************/

/******************************************************************************
* Function Name: load_mac_stats
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static r_result_t load_mac_stats (uint16_t index, uint8_t num, uint8_t * pOut)
{
    uint8_t i;

    for (i = 0; i < num; i++)
    {
        if (R_DEMO_MlmeGetWrap (R_DEMO_G3_USE_PRIMARY_CH, R_G3MAC_IB_STATISTICS, (uint16_t)(index + i), g_demo_buff.getStringBuffer) == R_G3MAC_STATUS_SUCCESS)
        {
            R_memcpy (pOut + (i * 4), g_demo_buff.getStringBuffer, 4);
        }
        else
        {
            return R_RESULT_FAILED;
        }
    }
    return R_RESULT_SUCCESS;
}
/******************************************************************************
End of function  load_mac_stats
******************************************************************************/

/******************************************************************************
* Function Name: handle_remote_getset_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static r_result_t handle_remote_getset_req (const uint8_t *      nsduIn,
                                            const r_ipv6_hdr_t * ipv6HdrIn,
                                            uint16_t *           ipPayloadLength,
                                            r_boolean_t *        allFramesSent)
{
    uint16_t                   checkSum;

    r_cap_command_header_t *   capCommandHdrIn;
    r_cap_command_header_t *   capCommandHdrOut;
    r_cap_cmd_capm_set_req_t * capSetRequest;
    r_cap_cmd_capm_get_req_t * capGetRequest;
    r_cap_cmd_capm_set_cnf_t * capSetConfirm;
    r_cap_cmd_capm_get_cnf_t * capGetConfirm;

    /* Create ioVec element. Two elements for message and dispatch. */
    IOVEC_CREATE (ioVec, 2)
    IOVEC_CREATE (ioVecChecksum, 1)

    UNUSED (ipv6HdrIn);
    UNUSED (allFramesSent);

    IOVEC_INIT (ioVec)
    IOVEC_INIT (ioVecChecksum)

    /* Set pointer for response message. */
    capCommandHdrOut    = (r_cap_command_header_t *)((g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) + sizeof (r_cap_icmp_header_t));
    capSetConfirm    = (r_cap_cmd_capm_set_cnf_t *)((g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) + (sizeof (r_cap_icmp_header_t) + sizeof (r_cap_command_header_t)));
    capGetConfirm    = (r_cap_cmd_capm_get_cnf_t *)((g_demo_buff.Nsdu + R_IPV6_HEADER_SIZE) + (sizeof (r_cap_icmp_header_t) + sizeof (r_cap_command_header_t)));

    /* Cast NSDU pointer to CAP command header structure and to ICMP CAP header structure. */
    capCommandHdrIn = (r_cap_command_header_t *)((nsduIn + R_IPV6_HEADER_SIZE) + sizeof (r_cap_icmp_header_t));

    /* Check if this is a set or get request. */
    if (R_BYTE_ArrToUInt16 (capCommandHdrIn->CmdID) == 0x0802)
    {
        /* Handle SET request, fist cast pointer. */
        capSetRequest = (r_cap_cmd_capm_set_req_t *)((nsduIn + R_IPV6_HEADER_SIZE) + (sizeof (r_cap_icmp_header_t) + sizeof (r_cap_command_header_t)));

        /* Set length. */
        R_BYTE_UInt16ToArr (sizeof (r_cap_cmd_capm_set_cnf_t), capCommandHdrOut->length);

        /* Set common elements for reply message. */
        R_memcpy (capSetConfirm->index, capSetRequest->index, sizeof (capSetRequest->index));
        R_memcpy (capSetConfirm->id, capSetRequest->id, sizeof (capSetRequest->id));
        capSetConfirm->type = capSetRequest->type;

        /* Switch over types. */
        switch (capSetRequest->type)
        {
            case R_CAP_TYPE_ID_GET_SET_ADP:

                /* Handle ADPM-SET.request. */
                if (R_DEMO_AdpmSetWrap (R_DEMO_G3_USE_PRIMARY_CH, (r_adp_ib_id_t)R_BYTE_ArrToUInt16 (capSetRequest->id), R_BYTE_ArrToUInt16 (capSetRequest->index), capSetRequest->payload) == R_ADP_STATUS_SUCCESS)
                {
                    /* Set status. */
                    R_BYTE_UInt32ToArr (CAP_SUCCESS, capSetConfirm->Status);
                }
                else
                {
                    /* Set status. */
                    R_BYTE_UInt32ToArr (CAP_FAILURE, capSetConfirm->Status);
                }
                break;

            case R_CAP_TYPE_ID_GET_SET_MAC:

                /* Handle MLME-SET.request. */
                if (R_DEMO_MlmeSetWrap (R_DEMO_G3_USE_PRIMARY_CH, R_BYTE_ArrToUInt16 (capSetRequest->id), R_BYTE_ArrToUInt16 (capSetRequest->index), capSetRequest->payload) == R_G3MAC_STATUS_SUCCESS)
                {
                    /* Set status. */
                    R_BYTE_UInt32ToArr (CAP_SUCCESS, capSetConfirm->Status);
                }
                else
                {
                    /* Set status. */
                    R_BYTE_UInt32ToArr (CAP_FAILURE, capSetConfirm->Status);
                }
                break;

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
            case R_CAP_TYPE_ID_GET_SET_RFMAC:

				/* Handle RF-MLME-SET.request. */
				if (R_DEMO_RfMlmeSetWrap (R_DEMO_G3_USE_PRIMARY_CH, R_BYTE_ArrToUInt16 (capSetRequest->id), R_BYTE_ArrToUInt16 (capSetRequest->index), capSetRequest->payload) == R_G3RFMAC_STATUS_SUCCESS)
				{
					/* Set status. */
					R_BYTE_UInt32ToArr (CAP_SUCCESS, capSetConfirm->Status);
				}
				else
				{
					/* Set status. */
					R_BYTE_UInt32ToArr (CAP_FAILURE, capSetConfirm->Status);
				}
				break;
#endif
            case R_CAP_TYPE_ID_GET_SET_STATS:

                if (R_DEMO_AppClearInfo (R_DEMO_G3_USE_PRIMARY_CH, R_G3_INFO_TYPE_BIT_STATS) == R_RESULT_SUCCESS)
                {
                    /* Set status. */
                    R_BYTE_UInt32ToArr (CAP_SUCCESS, capSetConfirm->Status);
                }
                else
                {
                    /* Set status. */
                    R_BYTE_UInt32ToArr (CAP_FAILURE, capSetConfirm->Status);
                }
                break;
                
             case R_CAP_TYPE_ID_APP_CMD:
                
                switch( (capSetRequest->id[0]<<8) | capSetRequest->id[1] )
                {
                    case R_CAP_APP_CMD_TYPE_ID_START_DUMP:
                        {
                            /* G3-Dump start */
                            R_DEMO_G3Dump(100000U); /* trigger wate time = 100sec */
                        }
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        } /* switch */

    }
    else if (R_BYTE_ArrToUInt16 (capCommandHdrIn->CmdID) == 0x0803)
    {
        /* Handle GET request, fist cast pointer. */
        capGetRequest = (r_cap_cmd_capm_get_req_t *)((nsduIn + R_IPV6_HEADER_SIZE) + (sizeof (r_cap_icmp_header_t) + sizeof (r_cap_command_header_t)));

        /* Set common elements for reply message. */
        R_memcpy (capGetConfirm->index, capGetRequest->index, sizeof (capGetRequest->index));
        R_memcpy (capGetConfirm->id, capGetRequest->id, sizeof (capGetRequest->id));
        capGetConfirm->type = capGetRequest->type;

        /* Switch over types. */
        switch (capGetRequest->type)
        {
            case R_CAP_TYPE_ID_GET_SET_ADP:

                /* Handle ADPM-GET.request. */

                if (R_DEMO_AdpmGetWrap (R_DEMO_G3_USE_PRIMARY_CH, (r_adp_ib_id_t)R_BYTE_ArrToUInt16 (capGetRequest->id), R_BYTE_ArrToUInt16 (capGetRequest->index), g_demo_buff.getStringBuffer) == R_ADP_STATUS_SUCCESS)
                {
                    /* Set length. */
                    R_BYTE_UInt16ToArr (sizeof (r_cap_cmd_capm_get_cnf_t), capCommandHdrOut->length);

                    /* Fill structure. */
                    R_memcpy (capGetConfirm->payload, g_demo_buff.getStringBuffer, R_CAP_CAPSET_MAX_LEN);

                    /* Set status. */
                    R_BYTE_UInt32ToArr (CAP_SUCCESS, capGetConfirm->Status);
                }
                else
                {
                    /* Set status. */
                    R_BYTE_UInt32ToArr (CAP_FAILURE, capGetConfirm->Status);
                }
                break;

            case R_CAP_TYPE_ID_GET_SET_MAC:

                /* Handle ADPM-GET.request. */
                if (R_DEMO_MlmeGetWrap (R_DEMO_G3_USE_PRIMARY_CH, (r_g3mac_ib_id_t)R_BYTE_ArrToUInt16 (capGetRequest->id), R_BYTE_ArrToUInt16 (capGetRequest->index), g_demo_buff.getStringBuffer) == R_G3MAC_STATUS_SUCCESS)
                {
                    /* Set length. */
                    R_BYTE_UInt16ToArr (R_CAP_CAPSET_MAX_LEN, capCommandHdrOut->length);

                    /* Fill structure. */
                    R_memcpy (capGetConfirm->payload, g_demo_buff.getStringBuffer, R_CAP_CAPSET_MAX_LEN);

                    /* Set status. */
                    R_BYTE_UInt32ToArr (CAP_SUCCESS, capGetConfirm->Status);
                }
                else
                {
                    /* Set status. */
                    R_BYTE_UInt32ToArr (CAP_FAILURE, capGetConfirm->Status);
                }
                break;
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
            case R_CAP_TYPE_ID_GET_SET_RFMAC:

				/* Handle ADPM-GET.request. */
				if (R_DEMO_RfMlmeGetWrap (R_DEMO_G3_USE_PRIMARY_CH, (r_g3rfmac_ib_id_t)R_BYTE_ArrToUInt16 (capGetRequest->id), R_BYTE_ArrToUInt16 (capGetRequest->index), g_demo_buff.getStringBuffer) == R_G3RFMAC_STATUS_SUCCESS)
				{
					/* Set length. */
					R_BYTE_UInt16ToArr (R_CAP_CAPSET_MAX_LEN, capCommandHdrOut->length);

					/* Fill structure. */
					R_memcpy (capGetConfirm->payload, g_demo_buff.getStringBuffer, R_CAP_CAPSET_MAX_LEN);

					/* Set status. */
					R_BYTE_UInt32ToArr (CAP_SUCCESS, capGetConfirm->Status);
				}
				else
				{
					/* Set status. */
					R_BYTE_UInt32ToArr (CAP_FAILURE, capGetConfirm->Status);
				}
				break;
#endif
            case R_CAP_TYPE_ID_GET_SET_STATS:
            {
                r_result_t res = R_RESULT_FAILED;
                switch (R_BYTE_ArrToUInt16 (capGetRequest->index))
                {
                    case R_CAP_STATS_INDEX_MAC_SND:
                        if (load_mac_stats (R_G3MAC_STATS_DATA_OUT_SUCCESS, 4u, capGetConfirm->payload) == R_RESULT_SUCCESS)
                        {
                            res = load_mac_stats (R_G3MAC_STATS_MCPS_DATA_SUCCESS, 4u, &capGetConfirm->payload[16u]);
                        }
                        break;

                    case R_CAP_STATS_INDEX_MAC_MOD:
                        res = load_mac_stats (R_G3MAC_STATS_MCPS_SUCCESS_UC_ROBUST, 8u, capGetConfirm->payload);
                        break;

                    case R_CAP_STATS_INDEX_MAC_CSMA1:
                        res = load_mac_stats (R_G3MAC_STATS_MCPS_SUCCESS_CSMA_NCW3, 8u, capGetConfirm->payload);
                        break;

                    case R_CAP_STATS_INDEX_MAC_CSMA2:
                        res = load_mac_stats (R_G3MAC_STATS_MCPS_FAILURE_CSMA_NCW3, 8u, capGetConfirm->payload);
                        break;

                    default:
                        break;
                } /* switch */

                /* Set length. */
                R_BYTE_UInt16ToArr (sizeof (r_cap_cmd_capm_get_cnf_t), capCommandHdrOut->length);

                /* Get status. */
                R_BYTE_UInt32ToArr ((uint32_t)((R_RESULT_SUCCESS == res) ? CAP_SUCCESS : CAP_FAILURE), capSetConfirm->Status);
                break;
            }

            default:
                break;
        } /* switch */

    }
    else
    {
        /* Do nothing. */
    }

    if ( R_FALSE == capCommandHdrIn->AckReq )
    {
        /* should not transmit reply */
        return R_RESULT_FAILED;
    }

    /* Prepare remaining CAP command header parts. */
    R_BYTE_UInt16ToArr (0, capCommandHdrOut->CheckSum);                                                                          // Initialize checksum to zero
    R_BYTE_UInt16ToArr (0, capCommandHdrOut->Reserve);                                                                           // Reserve set to zero
    R_memcpy (capCommandHdrOut->CmdID, capCommandHdrIn->CmdID, sizeof (capCommandHdrOut->CmdID));                                // Command ID identical to request
    R_memcpy (capCommandHdrOut->SequenceNumber, capCommandHdrIn->SequenceNumber, sizeof (capCommandHdrOut->SequenceNumber));     // Sequence number identical to request
    R_memcpy (capCommandHdrOut->Syncword, capCommandHdrIn->Syncword, sizeof (capCommandHdrOut->Syncword));                       // Syncword identical to request
    capCommandHdrOut->AckReq = 0x00;                                                                                             // Not relevant for confirms
    capCommandHdrOut->Type   = 0x03;                                                                                             // Confirm

    /* Add command header and following payload to ioVec. */
    R_IO_VecAppendElement (&ioVecChecksum,
                           (uint8_t *)&capCommandHdrOut->Syncword,
                           (uint16_t)(sizeof (r_cap_command_header_t) + R_BYTE_ArrToUInt16 (capCommandHdrOut->length)));

    /* Compute checksum. */
    R_CS_ComputeCheckSum (&ioVecChecksum,
                          &checkSum);

    /* Set checksum. */
    R_BYTE_UInt16ToArr (checkSum, capCommandHdrOut->CheckSum);

    /* Set IPv6 payload length. */
    *ipPayloadLength = (uint16_t)((R_BYTE_ArrToUInt16 (capCommandHdrOut->length) + sizeof (r_cap_command_header_t)) + R_ICMP_ECHO_MESSAGE_HEADER_LENGTH);

    return R_RESULT_SUCCESS;
} /* handle_remote_getset_req */
/******************************************************************************
   End of function  handle_remote_getset_req
******************************************************************************/

