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
* File Name     : r_byte_swap.c
* Device(s)     : RX631
* Tool-chain    : KPIT GNURX-ELF 15.01
* H/W platform  : G-CPX / EU-CPX2 / G-CPX3
* Description   : Sample software
******************************************************************************/

/******************************************************************************
   Includes   <System Includes> , Project Includes
******************************************************************************/
#include "r_typedefs.h"
#include "r_byte_swap.h"
#include "r_checksum.h"

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

uint32_t R_CHKSUM_Calc16Sum(const uint8_t* p_data, const int16_t data_len, const uint32_t init_sum)
{
    uint32_t sum = init_sum;
    int16_t i;

    for (i = 0; i < data_len; i += 2)
    {
        if (i + 1 < data_len)
        {
            sum += (uint32_t)R_BYTE_ArrToUInt16(&p_data[i]);
        }
        else
        {
            sum += (uint32_t)(p_data[i] << 8);
        }
    }

    return sum;
}

uint16_t R_CHKSUM_Calc16Finalize(uint32_t sum)
{
#define CARRY(x)        (x = (x & 0xffff) + (x >> 16))
    while (sum >> 16)
    {
        CARRY(sum);
    }
    return (uint16_t)(~sum);
}


uint16_t R_CHKSUM_Calc16(const uint8_t *p_data, const int16_t data_len, const uint32_t init_sum)
{
    uint32_t sum = R_CHKSUM_Calc16Sum(p_data, data_len, init_sum);
    return R_CHKSUM_Calc16Finalize(sum);
}
