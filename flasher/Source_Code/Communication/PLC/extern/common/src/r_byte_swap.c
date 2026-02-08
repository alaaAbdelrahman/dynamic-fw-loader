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

/******************************************************************************
* File Name     : r_byte_swap.c
* Device(s)     : RX631
* Tool-chain    : KPIT GNURX-ELF 15.01
* H/W platform  : G-CPX / EU-CPX2 / G-CPX3
* Description   : Sample software
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "r_byte_swap.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

/******************************************************************************
Function implementations
******************************************************************************/

/******************************************************************************
* Function Name:R_BYTE_UInt16ToArr
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_BYTE_UInt16ToArr (const uint16_t val,
                         uint8_t        parr[])
{
    if (NULL == parr)
    {
        return;
    }

    parr[0] = (uint8_t)(val >> 8u);
    parr[1] = (uint8_t)(val);
}
/******************************************************************************
   End of function  R_BYTE_UInt16ToArr
******************************************************************************/

/******************************************************************************
* Function Name:R_BYTE_UInt32ToArr
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_BYTE_UInt32ToArr (const uint32_t val,
                         uint8_t        parr[])
{
    if (NULL == parr)
    {
        return;
    }

    parr[0] = (uint8_t)(val >> 24u);
    parr[1] = (uint8_t)(val >> 16u);
    parr[2] = (uint8_t)(val >> 8u);
    parr[3] = (uint8_t)(val);
}
/******************************************************************************
   End of function  R_BYTE_UInt32ToArr
******************************************************************************/

/******************************************************************************
* Function Name:R_BYTE_UInt64ToArr
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_BYTE_UInt64ToArr (const uint64_t val,
                         uint8_t        parr[])
{
    if (NULL == parr)
    {
        return;
    }

    parr[0] = (uint8_t)(val >> 56u);
    parr[1] = (uint8_t)(val >> 48u);
    parr[2] = (uint8_t)(val >> 40u);
    parr[3] = (uint8_t)(val >> 32u);
    parr[4] = (uint8_t)(val >> 24u);
    parr[5] = (uint8_t)(val >> 16u);
    parr[6] = (uint8_t)(val >> 8u);
    parr[7] = (uint8_t)(val);
}
/******************************************************************************
   End of function  R_BYTE_UInt64ToArr
******************************************************************************/

/******************************************************************************
* Function Name:R_BYTE_ArrToUInt16
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint16_t R_BYTE_ArrToUInt16 (const uint8_t parr[])
{
    uint32_t val;

    if (NULL == parr)
    {
        return 0xFFFFu;
    }

    val = ((uint32_t)(parr[0]) << 8u) + (uint32_t)(parr[1]);
    return (uint16_t)val;
}
/******************************************************************************
   End of function  R_BYTE_ArrToUInt16
******************************************************************************/

/******************************************************************************
* Function Name:R_BYTE_ArrToUInt32
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint32_t R_BYTE_ArrToUInt32 (const uint8_t parr[])
{
    uint32_t val;

    if (NULL == parr)
    {
        return 0xFFFFFFFFu;
    }

    val = (((uint32_t)(parr[0]) << 24u) + ((uint32_t)(parr[1]) << 16u)) +
          (((uint32_t)(parr[2]) << 8u) + (uint32_t)(parr[3]));
    return val;
}
/******************************************************************************
   End of function  R_BYTE_ArrToUInt32
******************************************************************************/

/******************************************************************************
* Function Name:R_BYTE_ArrToUInt64
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint64_t R_BYTE_ArrToUInt64 (const uint8_t parr[])
{
    uint64_t val;

    if (NULL == parr)
    {
        return 0xFFFFFFFFFFFFFFFFu; /* PRQA S 1254 */ /* uint64_t is not defined on windows. */
    }

    val = ((((uint64_t)(parr[0]) << 56u) + ((uint64_t)(parr[1]) << 48u)) +
           (((uint64_t)(parr[2]) << 40u) + ((uint64_t)(parr[3]) << 32u))) +
          ((((uint64_t)(parr[4]) << 24u) + ((uint64_t)(parr[5]) << 16u)) +
           (((uint64_t)(parr[6]) << 8u) + (uint64_t)(parr[7])));
    return val;
}
/******************************************************************************
   End of function  R_BYTE_ArrToUInt64
******************************************************************************/

#if (defined R_SYNERGY_PRIME_PLC)
/******************************************************************************
* Function Name:R_BYTE_ByteRev16
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
uint16_t R_BYTE_ByteRev16 (const uint8_t* pdata)
{
    uint16_t buf;
    uint8_t* pbuf;
    const uint8_t* psrc;

    if (NULL == pdata)
    {
        return 0;
    }

    pbuf = (uint8_t *)&buf;
    psrc = (pdata + sizeof(uint8_t));

    if (NULL == psrc)
    {
        return 0;
    }

    *(pbuf++) = *(psrc--);
    *(pbuf  ) = *(psrc  );

    return buf;
}
/******************************************************************************
   End of function  R_BYTE_ByteRev16
******************************************************************************/

/******************************************************************************
* Function Name:R_BYTE_ByteRev32
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
uint32_t R_BYTE_ByteRev32 (const uint8_t* pdata)
{
    uint32_t buf;
    uint8_t* pbuf;
    const uint8_t* psrc;

    if (NULL == pdata)
    {
        return 0;
    }

    pbuf = (uint8_t *)&buf;
    psrc = (pdata + ((sizeof(uint8_t) * 3)));

    *(pbuf++) = *(psrc--); /* PRQA S 2814,2824 */
    *(pbuf++) = *(psrc--); /* PRQA S 2814 */
    *(pbuf++) = *(psrc--); /* PRQA S 2814 */
    *(pbuf  ) = *(psrc  );

    return buf;
}
/******************************************************************************
   End of function  R_BYTE_ByteRev32
******************************************************************************/
#endif /* R_SYNERGY_PRIME_PLC */
