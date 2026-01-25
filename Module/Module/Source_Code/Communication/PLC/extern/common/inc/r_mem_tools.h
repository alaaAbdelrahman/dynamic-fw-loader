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
* File Name     : r_mem_tools.h
* Device(s)     : RX631
* Tool-chain    : KPIT GNURX-ELF 15.01
* H/W platform  : G-CPX / EU-CPX2 / G-CPX3
* Description   : Sample software
******************************************************************************/

#ifndef R_MEM_TOOLS_H
#define R_MEM_TOOLS_H

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
/***********************************************************************
* Function Name     : R_MEM_MemCmpZero
* Description       : Checks if memory location is all-zero
* Argument          : input : Pointer to memory location
*                   : length : Length of memory to be checked
* Return Value      : Zero if memory location is all-zero, sum of memory bytes otherwise
***********************************************************************/
/*!
   \fn          uint32_t R_MEM_MemCmpZero(const uint8_t input[],
                      const uint32_t length);
   \brief       Checks if memory location is all-zero
   \param[in]   input Pointer to memory location
   \param[in]   length Length of memory to be checked
   \return      Zero if memory location is all-zero, sum of memory bytes otherwise
 */
uint32_t R_MEM_MemCmpZero(const uint8_t input[],
                     const uint32_t length);

/***********************************************************************
* Function Name     : R_MEM_MemCmpZeroBit
* Description       : Comparison of a bit string with zero
* Argument          : ptr1 : Pointer to the first byte where the string to be compared is contained
*                   : bitOffset : Offset in the first byte to the bit where the string to be compared begins
*                   : numBits : Number of bits to be compared with zero from ptr1 + bitOffset
* Return Value      : Zero if memory location is all-zero, sum of memory bits otherwise
***********************************************************************/
/*!
   \fn          int32_t R_MEM_MemCmpZeroBit(const uint8_t ptr1[],
                        const uint8_t bitOffset,
                        uint32_t numBits);
   \brief       Comparison of a bit string with zero
   \param[in]   ptr1 Pointer to the first byte where the string to be compared is contained
   \param[in]   bitOffset Offset in the first byte to the bit where the string to be compared begins
   \param[in]   numBits Number of bits to be compared with zero from ptr1 + bitOffset
   \return      Zero if memory location is all-zero, sum of memory bits otherwise
 */
int32_t R_MEM_MemCmpZeroBit(const uint8_t ptr1[],
                        const uint8_t bitOffset,
                        uint32_t numBits);
                        
/***********************************************************************
* Function Name     : R_MEM_MemCmpBit
* Description       : Compare memory chunks bit-wise
* Argument          : ptr1 : Pointer to memory location one
*                   : ptr2 : Pointer to memory location two
*                   : bitOffset : Offset in bits to where the bit string starts
* Return Value      : Zero if memory locations are identical
***********************************************************************/
/*!
   \fn          int32_t R_MEM_MemCmpBit(const uint8_t ptr1[],
                                    const uint8_t ptr2[],
                                    const uint8_t bitOffset,
                                    const uint32_t numBits);
   \brief       Compare memory chunks bit-wise
   \param[in]   ptr1 Pointer to memory location one
   \param[in]   ptr2 Pointer to memory location two
   \param[in]   bitOffset Offset in bits to where the bit string starts
   \param[in]   numBits Number of bits to be compared
   \return      Zero if memory locations are identical
 */
int32_t R_MEM_MemCmpBit(const uint8_t ptr1[],
                    const uint8_t ptr2[],
                    const uint8_t bitOffset,
                    const uint32_t numBits);

/***********************************************************************
* Function Name     : R_MEM_MemCpyBit
* Description       : Copy memory chunk from dst to src bit-wise
* Argument          : dst : Pointer to destination memory location
*                   : src : Pointer to source memory location
*                   : numBits : Size in bites to be copied from source to destination
* Return Value      : None
***********************************************************************/
/*!
   \fn          void R_MEM_MemCpyBit(uint8_t dst[],
                                 const uint8_t src[],
                                 const uint32_t numBits);
   \brief       Copy memory chunk from dst to src bit-wise
   \param[in]   dst Pointer to destination memory location
   \param[in]   src Pointer to source memory location
   \param[in]   numBits Size in bites to be copied from source to destination
 */
void R_MEM_MemCpyBit(uint8_t dst[],
                 const uint8_t src[],
                 const uint32_t numBits);

#endif /* R_MEM_TOOLS_H */
