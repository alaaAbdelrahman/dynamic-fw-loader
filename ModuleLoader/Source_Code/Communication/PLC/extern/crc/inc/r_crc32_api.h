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
* File Name     : r_crc32_api.h
* Device(s)     : RX631
* Tool-chain    : KPIT GNURX-ELF 15.01
* H/W platform  : G-CPX / EU-CPX2 / G-CPX3
* Description   : Sample software
******************************************************************************/

/*
 * Prevent nested inclusions
 */
#ifndef R_CRC32_H
#define R_CRC32_H

/******************************************************************************
Typedef definitions
******************************************************************************/

/*!
    \struct r_iovec_t
    \brief Structure for collecting pointers to data chunks
 */
typedef struct
{
    const uint8_t* paddress; //!< Address where the data starts
    uint32_t       length;  //!< Length of data in bytes

} r_iovec_t;


/******************************************************************************
* Functions
******************************************************************************/

/*!
    \fn void R_CRC_InitCRC32(void)
    \brief Populate the partial CRC lookup table.
    \details This function must be rerun any time the CRC standard is changed. If desired, it can be run "offline" and the table results stored in an embedded system's ROM
    \return R_RESULT_SUCCESS
 */
r_result_t R_CRC_InitCRC32(void);

/*!
    \fn uint32_t R_CRC_CalcCrc32(const r_iovec_t iovec[], uint8_t iovecLength)
    \brief Compute the CRC of a given message.
    \details crcInit() must be called first
    \param[in] iovec iovec containing the data to be CRCed
    \param[in] iovecLength length of the iovec
    \return computed CRC
 */
uint32_t R_CRC_CalcCrc32(const r_iovec_t iovec[],
                         uint32_t        residue,
                         uint8_t         iovecLength);

/*!
    \fn r_result_t R_CRC_CheckCrc32(ui r_iovec_t iovec[], uint8_t iovecLength)
    \brief      CValidate the CRC32
    \details    Validate the CRC
    \param[in]  input Data input including appended 4 byte - CRC
    \param[in]  length Data length including CRC in bytes
   \return      Either R_RESULT_SUCCESS or R_RESULT_FAILED
 */
r_result_t R_CRC_ValidateCrc32(const uint8_t input[],
                               uint16_t      length);

#endif /* R_CRC32_H */
