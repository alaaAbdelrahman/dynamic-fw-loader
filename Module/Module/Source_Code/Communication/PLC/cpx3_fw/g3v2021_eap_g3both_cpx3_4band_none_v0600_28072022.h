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
* Copyright (C) 2015-2017 Renesas Electronics Corporation. All rights reserved.
******************************************************************************/

/*******************************************************************************
 * File Name   : cpx3_fw_release.h
 * Description : CPX3 FW binary data
 ******************************************************************************/
#ifndef CPX3_FW_RELEASE_H
#define CPX3_FW_RELEASE_H

const uint8_t g_cpxprogversion[] = {
    0x03, 0x01, 0x00, 0x0F, 0x00, 0x12, 0xA6, 0x8E, 0x03, 0x13, 0x03, 0x13, 0x03, 0x13, 0x2B, 0x9F, 
};

const uint8_t g_cpxprogtbl[] = {
    0x43, 0x70, 0x58, 0x73, 0x42, 0x6F, 0x4F, 0x74, 0x48, 0x65, 0x41, 0x64, 0x33, 0xEE, 0xFF, 0xFF,
    0x40, 0x00, 0x00, 0x00, 0x00, 0xD0, 0x07, 0x20, 0x51, 0x0F, 0x00, 0x00, 0xC9, 0x5F, 0xF8, 0xFF,
    0x91, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x02, 0x48, 0x01, 0x6B, 0x01, 0x00, 0x89, 0x94, 0x4A, 0xFF,
    0x92, 0x7A, 0x01, 0x00, 0x00, 0x00, 0x00, 0x48, 0x31, 0x24, 0x00, 0x00, 0xC9, 0xEC, 0xED, 0xFF,
    0xC3, 0x9E, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x91, 0xDF, 0x01, 0x00, 0x12, 0x1A, 0x10, 0xFF
};
#endif