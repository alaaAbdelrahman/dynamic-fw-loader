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

/*******************************************************************************
 * File Name : r_c3sap_g3_convert.h
 * Description : G3 layer convert
 ******************************************************************************/

#ifndef R_C3SAP_G3_CONVERT_H
#define R_C3SAP_G3_CONVERT_H

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

/* for G3CTRL */
r_result_t R_G3CTRL_ReqStr2Bin (uint16_t funcId, void * pin_str, uint16_t max_len, uint8_t * pout_bin, uint16_t * pout_len);
r_result_t R_G3CTRL_CbBin2Str (uint16_t funcId, uint8_t * pin_bin, uint16_t max_len, void * pout_str, uint16_t * pout_len);

/* for MAC */
r_result_t R_G3MAC_ReqStr2Bin (uint16_t funcId, void * pin_str, uint16_t max_len, uint8_t * pout_bin, uint16_t * pout_len);
r_result_t R_G3MAC_CbBin2Str (uint16_t funcId, uint8_t * pin_bin, uint16_t max_len, void * pout_str, uint16_t * pout_len);

/* for ADP */
r_result_t R_ADP_ReqStr2Bin (uint16_t funcId, void * pin_str, uint16_t max_len, uint8_t * pout_bin, uint16_t * pout_len);
r_result_t R_ADP_CbBin2Str (uint16_t funcId, uint8_t * pin_bin, uint16_t max_len, void * pout_str, uint16_t * pout_len);

/* for EAP */
r_result_t R_EAP_ReqStr2Bin (uint16_t funcId, void * pin_str, uint16_t max_len, uint8_t * pout_bin, uint16_t * pout_len);
r_result_t R_EAP_CbBin2Str (uint16_t funcId, uint8_t * pin_bin, uint16_t max_len, void * pout_str, uint16_t * pout_len);

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
r_result_t R_G3RFMAC_ReqStr2Bin (uint16_t funcId, void * pin_str, uint16_t max_len, uint8_t * pout_bin, uint16_t * pout_len);
r_result_t R_G3RFMAC_CbBin2Str (uint16_t funcId, uint8_t * pin_bin, uint16_t max_len, void * pout_str, uint16_t * pout_len);
#endif

#endif /* R_C3SAP_G3_CONVERT_H */

