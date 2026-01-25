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
* File Name     : r_byte_swap.h
* Device(s)     : RX631
* Tool-chain    : KPIT GNURX-ELF 15.01
* H/W platform  : G-CPX / EU-CPX2 / G-CPX3
* Description   : Sample software
******************************************************************************/

#ifndef R_BYTE_SWAP_H
#define R_BYTE_SWAP_H

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/***********************************************************************
* Function Name     : R_BYTE_UInt16ToArr
* Description       : uint16_t to array conversion
* Argument          : const uint16_t val Value to be converted
*                   : uint8_t p_arr[] Array used for the conversion
* Return Value      : none
***********************************************************************/
/*!
   \fn          void R_BYTE_UInt16ToArr (uint16_t val, uint8_t p_arr[])
   \brief       Convert an unsigned 16-bit integer to array of 8-bit values
   \details     Convert an unsigned 16-bit integer to array of 8-bit values
   \param[in]   val Value to be converted
   \param[out]  p_arr Pointer to an 8-bit array to store the converted value
   \return      void
 */
void R_BYTE_UInt16ToArr (const uint16_t val,
                         uint8_t        p_arr[]);

/***********************************************************************
* Function Name     : R_BYTE_UInt16ToArr
* Description       : uint32_t to array conversion
* Argument          : const uint32_t val Value to be converted
*                   : uint8_t p_arr[] Array used for the conversion
* Return Value      : none
***********************************************************************/
/*!
   \fn          void R_BYTE_UInt32ToArr (uint32_t val, uint8_t p_arr[])
   \brief       Convert an unsigned 32-bit integer to array of 8-bit values
   \details     Convert an unsigned 32-bit integer to array of 8-bit values
   \param[in]   val Value to be converted
   \param[out]  p_arr Pointer to an 8-bit array to store the converted value
   \return      void
 */
void R_BYTE_UInt32ToArr (const uint32_t val,
                         uint8_t        p_arr[]);

/***********************************************************************
* Function Name     : R_BYTE_Uint64ToArr
* Description       : uint64_t to array conversion
* Argument          : const uint64_t val Value to be converted
*                   : uint8_t p_arr[] Array used for the conversion
* Return Value      : none
***********************************************************************/
/*!
   \fn          void R_BYTE_Uint64ToArr (uint64_t val, uint8_t p_arr[])
   \brief       Convert an unsigned 64-bit integer to array of 8-bit values
   \details     Convert an unsigned 64-bit integer to array of 8-bit values, taking into account the endianness as defined in _BIG_ENDIAN_
   \param[in]   val Value to be converted
   \param[out]  p_arr Pointer to an 8-bit array to store the converted value
   \return      void
 */
void R_BYTE_UInt64ToArr (const uint64_t val,
                         uint8_t        p_arr[]);

/***********************************************************************
* Function Name     : R_BYTE_ArrToUInt16
* Description       : Array to uint16_t conversion
* Argument          : uint8_t p_arr[] Array used for the conversion
* Return Value      : uint16_t value
***********************************************************************/
/*!
   \fn          uint16_t R_BYTE_ArrToUInt16 (const uint8_t p_arr[])
   \brief       Convert an array of 8-bit values to unsigned 16-bit integer
   \details     Convert an array of 8-bit values to unsigned 16-bit integer
   \param[in]   p_arr Pointer to an 8-bit array to store the converted value
   \return      16-bit value
 */
uint16_t R_BYTE_ArrToUInt16 (const uint8_t p_arr[]);

/***********************************************************************
* Function Name     : R_BYTE_ArrToUInt32
* Description       : Array to uint32_t conversion
* Argument          : uint8_t p_arr[] Array used for the conversion
* Return Value      : uint32_t value
***********************************************************************/
/*!
   \fn          uint32_t R_BYTE_ArrToUInt32 (const uint8_t p_arr[])
   \brief       Convert an array of 8-bit values to unsigned 32-bit integer
   \details     Convert an array of 8-bit values to unsigned 32-bit integer
   \param[in]   p_arr Pointer to an 8-bit array to store the converted value
   \return      32-bit value
 */
uint32_t R_BYTE_ArrToUInt32 (const uint8_t p_arr[]);

/***********************************************************************
* Function Name     : R_BYTE_ArrToUInt64
* Description       : Array to uint64_t conversion
* Argument          : uint8_t p_arr[] Array used for the conversion
* Return Value      : uint64_t value
***********************************************************************/
/*!
   \fn          uint64_t R_BYTE_ArrToUInt64(const uint8_t p_arr[])
   \brief       Convert an array of 8-bit values to unsigned 64-bit integer
   \details     Convert an array of 8-bit values to unsigned 64-bit integer, taking into account the endianness as defined in _BIG_ENDIAN_
   \param[in]   p_arr Pointer to an 8-bit array to store the converted value
   \return      64-bit value
 */
uint64_t R_BYTE_ArrToUInt64 (const uint8_t p_arr[]);

#if (defined R_SYNERGY_PRIME_PLC)
/***********************************************************************
* Function Name     : R_BYTE_ByteRev16
* Description       : Switch endian of input value.
* Argument          : uint8_t* pdata Pointer to the value to switch endian.
* Return Value      : uint16_t The endian switched value.
***********************************************************************/
/*!
   \fn          uint16_t R_BYTE_ByteRev16(const uint8_t* pdata)
   \brief       Switch endian of input value.
   \details     Input AA BB Output BB AA
   \param[in]   Start pointer of 16-bit variable
   \return      16-bit value
 */
uint16_t R_BYTE_ByteRev16(const uint8_t* pdata);

/***********************************************************************
* Function Name     : R_BYTE_ByteRev32
* Description       : Switch endian of input value.
* Argument          : uint8_t* pdata Pointer to the value to switch endian.
* Return Value      : uint16_t The endian switched value.
***********************************************************************/
/*!
   \fn          uint32_t R_BYTE_ByteRev32(const uint8_t* pdata);
   \brief       Switch endian of input value.
   \details     Input AA BB CC DD Output DD CC BB AA
   \param[in]   Start pointer of 32-bit variable
   \return      32-bit value
 */
uint32_t R_BYTE_ByteRev32(const uint8_t* pdata);
#endif /* defined R_SYNERGY_PRIME_PLC */
#endif /* R_BYTE_SWAP_H */

