/******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
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
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
******************************************************************************/

/******************************************************************************
* File Name     : r_mem_tools.c
* Device(s)     : RX631
* Tool-chain    : KPIT GNURX-ELF 15.01
* H/W platform  : G-CPX / EU-CPX2 / G-CPX3
* Description   : Sample software
******************************************************************************/

/******************************************************************************
   Includes   <System Includes> , Project Includes
******************************************************************************/
#include "r_typedefs.h"
#include "r_mem_tools.h"

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
* Static variables
******************************************************************************/
static uint8_t get_mask(const uint8_t num_bits);

/******************************************************************************
* Local function headers
******************************************************************************/

/******************************************************************************
* Extern variables
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
Function implementations
******************************************************************************/

/******************************************************************************
* Function Name:R_MEM_MemCmpZero
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
uint32_t R_MEM_MemCmpZero(const uint8_t pinput[],
                      const uint32_t length)
{
    uint32_t i;
    uint32_t return_val = 0;

    if (NULL == pinput)
    {
        return 0xFFu;
    }

    for (i = 0 ; i < length ; i++)
    {
        return_val += pinput[i];
    }

    return return_val;
}
/******************************************************************************
   End of function  R_MEM_MemCmpZero
******************************************************************************/

/******************************************************************************
* Function Name:R_MEM_MemCmpZeroBit
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
int32_t R_MEM_MemCmpZeroBit(const uint8_t ptr1[],
                        const uint8_t bit_offset,
                        uint32_t num_bits)
{
    const uint8_t* ptr_first_byte;
    const uint8_t* ptr_last_byte;
    int32_t        return_val = 0;
    uint32_t       num_full_bytes;
    uint8_t        remaining_bits;
    uint8_t        mask;

    /* checks */
    if ((NULL == ptr1) || (bit_offset > 7u))
    {
        return 0xFF; /* failed */
    }

    /* compare a string of bits with zero starting from ptr1 + bit_offset */

    if (0u == bit_offset)
    {
        num_full_bytes  = num_bits >> 3u;
        remaining_bits = (uint8_t)(num_bits - (num_full_bytes << 3u));
        ptr_first_byte  = ptr1;
    }
    else
    {
        num_full_bytes  = (num_bits - (8UL - (uint32_t)bit_offset)) >> 3u;
        remaining_bits = (uint8_t)( (num_bits - (8UL - (uint32_t)bit_offset)) - (num_full_bytes << 3u));
        ptr_first_byte  = &ptr1[1]; /* next byte */
    }

    /* bits of first byte if offset != 0 */
    if (0u != bit_offset)
    {
        /* Compute mask */
        mask       = get_mask(bit_offset);

        return_val = (int32_t)((ptr1[0]) & (~mask));
    }

    /* bits of full bytes */
    return_val += (int32_t)R_MEM_MemCmpZero(ptr_first_byte, num_full_bytes);

    /* remaining bits of last byte */
    if ((0uL != remaining_bits) && (NULL != ptr_first_byte))
    {
        ptr_last_byte = ptr_first_byte + num_full_bytes;

        if (NULL != ptr_last_byte)
        {
            /* Compute mask */
            mask       = get_mask(remaining_bits);

            return_val += (int32_t)((*ptr_last_byte) & mask);
        }
    }

    return return_val;
}
/******************************************************************************
   End of function  R_MEM_MemCmpZeroBit
******************************************************************************/

/******************************************************************************
* Function Name:R_MEM_MemCmpBit
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
int32_t R_MEM_MemCmpBit(const uint8_t ptr1[],
                    const uint8_t ptr2[],
                    const uint8_t bit_offset,
                    const uint32_t num_bits)
{
    int32_t        return_val = 0;
    const uint8_t* ptr1_last_byte;
    const uint8_t* ptr2_last_byte;
    const uint8_t* ptr1_first_byte;
    const uint8_t* ptr2_first_byte;
    uint32_t       num_full_bytes;
    uint8_t        remaining_bits;
    uint8_t        mask;

    if ((NULL == ptr1) || (NULL == ptr2))
    {
        return 0xFF; /* non zero value */
    }

    /* Compare bits of first byte */
    if (0u == bit_offset)
    {
        num_full_bytes  = num_bits >> 3u;
        remaining_bits = (uint8_t)(num_bits - (num_full_bytes << 3u));
        ptr1_first_byte = ptr1;
        ptr2_first_byte = ptr2;
    }
    else
    {
        num_full_bytes  = (num_bits - (8u - bit_offset)) >> 3u;
        remaining_bits = (uint8_t)( (num_bits - (8u - bit_offset)) - (num_full_bytes << 3u));
        ptr1_first_byte = &ptr1[1];
        ptr2_first_byte = &ptr2[1];
    }

    if (0u != bit_offset)
    {
        /* Compute mask */
        mask = get_mask(bit_offset);

        /* Compare single bits */
        return_val = (int32_t)(((*ptr1) & (~mask)) - ((*ptr2) & (~mask)));
    }

    /* Compare full bytes part */
    return_val += R_memcmp(ptr1_first_byte, ptr2_first_byte, num_full_bytes);

    /* Check if single bits are left for comparision */
    if (((0uL != remaining_bits) && (NULL != ptr1_first_byte)) && (NULL != ptr2_first_byte))
    {
        ptr1_last_byte = ptr1_first_byte + num_full_bytes;
        ptr2_last_byte = ptr2_first_byte + num_full_bytes;

        if ((NULL != ptr1_last_byte) && (NULL != ptr2_last_byte))
        {
            /* Compute mask */
            mask = get_mask(remaining_bits);

            /* Compare single bits */
            return_val += (int32_t)(((*ptr1_last_byte) & mask) - ((*ptr2_last_byte) & mask));
        }
    }

    return return_val;
}
/******************************************************************************
   End of function  R_MEM_MemCmpBit
******************************************************************************/

/******************************************************************************
* Function Name:R_MEM_MemCpyBit
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
void R_MEM_MemCpyBit(uint8_t pdst[],
                 const uint8_t psrc[],
                 const uint32_t num_bits)
{
    uint8_t              mask;
    const uint32_t       num_bytes      = num_bits >> 3u;
    const uint32_t       remaining_bits = num_bits % 8UL;
    uint8_t*             psrc_last_byte;
    uint8_t*             pdst_last_byte;

    if ((NULL == pdst) || (NULL == psrc))
    {
        return;
    }

    psrc_last_byte   = (uint8_t*)psrc + num_bytes;
    pdst_last_byte   = pdst + num_bytes;

    /* First copy full bytes part */
    R_memcpy(pdst, psrc, num_bytes);

    /* Check if single bits are left for copy */
    if (((0uL != remaining_bits) && (NULL != pdst_last_byte)) && (NULL != psrc_last_byte))
    {
        /* Compute mask */
        mask = get_mask((uint8_t)remaining_bits);

        /* Copy single bits */
        (*pdst_last_byte) = (uint8_t)(((*psrc_last_byte) & mask) + ((*pdst_last_byte) & (uint8_t)(~mask)));
    }
}
/******************************************************************************
   End of function  R_MEM_MemCpyBit
******************************************************************************/

/******************************************************************************
* Function Name:get_mask
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
static uint8_t get_mask(const uint8_t num_bits)
{
    uint8_t i;
    uint8_t mask = 0;

    for (i = 0 ; i < num_bits ; i++)
    {
        mask = (uint8_t)(mask + (1u << (7u - i)));
    }

    return mask;
}
/******************************************************************************
   End of function  get_mask
******************************************************************************/
