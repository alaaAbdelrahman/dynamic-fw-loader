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
 * File Name   : r_demo_cert_app.c
 *    @version
 *        $Rev: 2372 $
 *    @last editor
 *        $Author: troba $
 *    @date
 *        $Date:: 2017-04-26 17:49:29 +0200#$
 * Description :
 ******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include <string.h>
#include "r_typedefs.h"
#include "r_stdio_api.h"
#include "r_byte_swap.h"
#include "r_config.h"

#include "r_io_vec.h"

/* g3 part */
#include "r_demo_config.h"
#include "r_c3sap_api.h"
#include "r_demo_app.h"
#include "r_demo_app_eap.h"
#include "r_demo_nvm_process.h"
#include "r_demo_print.h"
#include "r_demo_tools.h"
#include "r_demo_api.h"
#include "r_demo_metric_computation.h"
#include "r_ipv6_headers.h"
#include "r_udp_headers.h"
#include "r_icmp_v6.h"
#include "r_check_sum.h"
#include "r_demo_common.h"
#include "r_demo_parameters.h"

//------------------------
extern uint8_t cpx3_current_task;
extern uint8_t config_monitor;
uint8_t current_AppSetIpContent = CPX3_TX_GAIN;

uint8_t current_AppPresetProcess = CPX3_SET_IB_FRAME_COUNTER;

extern uint8_t get_handle_state;

//-------------------------
/******************************************************************************
Macro definitions
******************************************************************************/
/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static r_demo_backup_payload_t sDemoBackup;

/******************************************************************************
Exported global variables
******************************************************************************/
extern r_demo_config_t g_demo_config;

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/
static uint8_t g_group_table_entry_8001[] = {0x01, 0x80, 0x01};
static uint8_t g_group_table_entry_8567[] = {0x01, 0x85, 0x67};
static uint8_t g_contextinfo_table_entry_00[] = {0x01, 0x50, 0xFE, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x1D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF};
static uint8_t g_contextinfo_table_entry_01[] = {0x01, 0x30, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF};

#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
static const int8_t g_demo_rfmac_supportBand[R_RFMAC_FREQ_BAND_MAX][R_RFMAC_FSK_OPEMODE_TABLE] = {
    {(int8_t)EU1_863MHZ_MODE1, (int8_t)EU1_863MHZ_MODE2}, /* R_RFMAC_FREQ_BAND_863 */
    {(int8_t)EU1_866MHZ_MODE1, (int8_t)EU1_866MHZ_MODE2}, /* R_RFMAC_FREQ_BAND_866 */
    {(int8_t)EU2_870MHZ_MODE1, (int8_t)EU2_870MHZ_MODE2}, /* R_RFMAC_FREQ_BAND_870 */
    {(int8_t)US_915MHZ_MODE1, (int8_t)US_915MHZ_MODE3},   /* R_RFMAC_FREQ_BAND_915 */
    {(int8_t)US_915AMHZ_MODE1, (int8_t)US_915AMHZ_MODE4}, /* R_RFMAC_FREQ_BAND_915a */
    {(int8_t)US_915BMHZ_MODE1, (int8_t)US_915BMHZ_MODE4}, /* R_RFMAC_FREQ_BAND_915b */
    {(int8_t)US_915CMHZ_MODE1, (int8_t)US_915CMHZ_MODE4}, /* R_RFMAC_FREQ_BAND_915c */
    {(int8_t)JP_920MHZ_MODE1, (int8_t)JP_920MHZ_MODE6}    /* R_RFMAC_FREQ_BAND_920 */
};
#endif

/******************************************************************************
Functions
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_GetDemoConfig
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_GetDemoConfig(void)
{
    r_cap_dev_cfg_t config;

    /* GET DEV INFO */
    if (R_DEMO_MODE_CERT == g_demo_config.appMode)
    {
        r_demo_et_gen_cert_dev_config(&config, g_demo_config.devType);
        if (R_ADP_DEVICE_TYPE_COORDINATOR == g_demo_config.devType)
        {
            config.extendedAddress[7] = 0;
        }
    }
    else
    {
        r_demo_et_read_dev_config(R_DEMO_G3_USE_PRIMARY_CH, (uint8_t *)&config);
    }

    g_demo_config.deviceEUI64 = R_BYTE_ArrToUInt64(config.extendedAddress);
    R_memcpy(g_demo_config.pskKey, config.psk, 16);
    R_memcpy(g_demo_config.gmk0, config.gmk[0], 16);
    R_memcpy(g_demo_config.gmk1, config.gmk[1], 16);
    R_memcpy(&g_demo_config.extId, &config.extID, sizeof(r_g3_config_extid_t));

    g_demo_config.panId = R_BYTE_ArrToUInt16(config.panid);
    g_demo_config.coordShortAddress = R_BYTE_ArrToUInt16(config.coordAddr);

    /* GET BACKUP INFO */
    if (r_demo_et_read_backup(R_DEMO_G3_USE_PRIMARY_CH, (uint8_t *)&sDemoBackup) == R_RESULT_SUCCESS)
    {
        r_demo_print_config(R_DEMO_G3_USE_PRIMARY_CH, &g_demo_config, &config, &sDemoBackup);
    }
    else
    {
        r_demo_print_config(R_DEMO_G3_USE_PRIMARY_CH, &g_demo_config, &config, NULL);
    }

} /* R_DEMO_GetDemoConfig */
/******************************************************************************
   End of function  R_DEMO_GetDemoConfig
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_SetIbForETTDLL353738
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_demo_config_status_t R_DEMO_SetIbForETTDLL353738(r_boolean_t activate)
{
    uint8_t tmpArray[R_ADP_MAX_IB_SIZE];

#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    if (R_MEDIA_TYPE2_BOTH == g_demo_config.mediaType) // Hybrid only
    {
        tmpArray[0] = activate;
        if (R_DEMO_RfMlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3RFMAC_IB_DISABLE_PHY_RF, 0, (uint8_t *)tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_CONFIG_STATUS_FAILED;
        }
    }
#endif

    /* Parameters related to Trickle algorithm for broadcast data messages (ETT_DLL_37): */
    tmpArray[0] = activate;
    if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_TRICKLE_DATA_ENABLED, 0, (uint8_t *)tmpArray) != R_ADP_STATUS_SUCCESS)
    {
        return R_CONFIG_STATUS_FAILED;
    }

    /* Parameter related to RREQ jitter algorithm (ETT_DLL_35): */
    R_BYTE_UInt16ToArr((activate) ? 1500u : 0, tmpArray);
    if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_DELAY_LOW_LQI, 0, (uint8_t *)tmpArray) != R_ADP_STATUS_SUCCESS)
    {
        return R_CONFIG_STATUS_FAILED;
    }

    R_BYTE_UInt16ToArr((activate) ? 100u : 0, tmpArray);
    if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_DELAY_HIGH_LQI, 0, (uint8_t *)tmpArray) != R_ADP_STATUS_SUCCESS)
    {
        return R_CONFIG_STATUS_FAILED;
    }

    /* Parameters related to Trickle Cluster forwarding algorithm for RREQ (ETT_DLL_38): */
    tmpArray[0] = activate;
    if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_CLUSTER_TRICKLE_ENABLED, 0, (uint8_t *)tmpArray) != R_ADP_STATUS_SUCCESS)
    {
        return R_CONFIG_STATUS_FAILED;
    }

    return R_CONFIG_STATUS_SUCCESS;
} /* R_DEMO_GetDemoConfig */
/******************************************************************************
   End of function  R_DEMO_SetIbForETTDLL353738
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_SetIbContent
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_SetIbContent(void)
{
    /* Local variables */
    uint32_t i;
    static uint8_t tmpArray[R_ADP_MAX_IB_SIZE];
    uint8_t txPower;
    uint8_t txEnablePolarity;
    uint8_t txWaitTime;
    uint8_t txDigitalPreambleGain;
    uint8_t txDigitalGain;
    uint16_t txFilterScale;
    uint8_t tpGaCaj;
    r_adp_destination_address_set_t *padp_destination_address_set;

    /************** PHY Settings **************/

    /* Check band plan. */
    switch (g_demo_config.bandPlan)
    {
    case R_G3_BANDPLAN_CENELEC_A:
        txPower = 12;
        txFilterScale = 0x0E31;
        txDigitalPreambleGain = 28;
        txDigitalGain = 25;
        txEnablePolarity = 0;
        txWaitTime = 0;
        break;

    case R_G3_BANDPLAN_CENELEC_B:
        txPower = 12;
        txFilterScale = 0x0FED;
        txDigitalPreambleGain = 28;
        txDigitalGain = 25;
        txEnablePolarity = 0;
        txWaitTime = 0;
        break;

    case R_G3_BANDPLAN_ARIB:
        txPower = 18;
        txFilterScale = 0x01C9;
        txDigitalPreambleGain = 25;
        txDigitalGain = 22;
        txEnablePolarity = 0;
        txWaitTime = 0;
        break;

    case R_G3_BANDPLAN_FCC:
        txPower = 18;
        txFilterScale = 0x0286;
        txDigitalPreambleGain = 25;
        txDigitalGain = 22;
        txEnablePolarity = 0;
        txWaitTime = 0;
        break;

    default:
        return R_RESULT_SUCCESS;
    } /* switch */
    tpGaCaj = R_BOARD_TPGA_CAJ;

    switch (current_AppSetIpContent)
    {
    case CPX3_TX_GAIN:
        /* Set tx gain. */
        tmpArray[0] = txPower;
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, (uint16_t)R_G3MAC_IB_PHY_TX_POWER, 0, tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {

            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_TX_FILTER_SCALE;

        break;

    case CPX3_TX_FILTER_SCALE:
        /* Set tx filter scale. */
        R_BYTE_UInt16ToArr(txFilterScale, tmpArray);
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, (uint16_t)R_G3MAC_IB_PHY_TX_FILTER_SCALE, 0, tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_TX_DIGITAL_PREAMBLE_GAIN;

        break;

    case CPX3_TX_DIGITAL_PREAMBLE_GAIN:

        /* Set tx digital preamble gain. */
        tmpArray[0] = txDigitalPreambleGain;
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, (uint16_t)R_G3MAC_IB_PHY_TX_DIGITAL_PREAMBLE_GAIN, 0, tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_TX_DIGITAL_GAIN;

        break;

    case CPX3_TX_DIGITAL_GAIN:
        /* Set tx digital gain. */
        tmpArray[0] = txDigitalGain;
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, (uint16_t)R_G3MAC_IB_PHY_TX_DIGITAL_GAIN, 0, tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_TX_ENABLE_POLARITY;

        break;
    case CPX3_TX_ENABLE_POLARITY:
        /* Set tx enable polarity. */
        tmpArray[0] = txEnablePolarity;
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, (uint16_t)R_G3MAC_IB_PHY_TXENB_POLARITY, 0, tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_TX_WAIT_TIME;

        break;

    case CPX3_TX_WAIT_TIME:
        /* Set tx wait time. */
        tmpArray[0] = txWaitTime;
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, (uint16_t)R_G3MAC_IB_PHY_TX_WAIT_TIME, 0, tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_TX_WAIT_TIME_2;

        break;

    case CPX3_TX_WAIT_TIME_2:
        /* Set tx wait time_2. */
        tmpArray[0] = tpGaCaj;
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, (uint16_t)R_G3MAC_IB_PHY_TPGA_CAJ, 0, tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_SATT_CTRL_DISABLE;

        break;

    case CPX3_SATT_CTRL_DISABLE:
        /* Set SATT Ctrl disable. */
        tmpArray[0] = R_VAL_PHY_SATT_CTRL_DISABLE;
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, (uint16_t)R_G3MAC_IB_PHY_SATT_CTRL_DISABLE, 0, tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_SET_FRAME_COUNTER_IND;

        break;
        /************** MAC PIB Settings **************/

    case CPX3_SET_FRAME_COUNTER_IND:
        /* Set frame counter indication interval. */
        R_BYTE_UInt32ToArr(R_FRAMECOUNT_INTERVAL, tmpArray);
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3MAC_IB_FRAME_CNT_IND_INTERVAL, 0, tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_SET_TONE_MAP_IND;

        break;

    case CPX3_SET_TONE_MAP_IND:
        /* Set tone map indication. */
        tmpArray[0] = (uint8_t)(R_VAL_MAC_TMR_IND_ENABLE);
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3MAC_IB_TMR_IND_ENABLE, 0, (uint8_t *)tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_SET_OFFSET_SNR_FOR_ADAPTIVE_TONE_MAP;

        break;

    case CPX3_SET_OFFSET_SNR_FOR_ADAPTIVE_TONE_MAP:
        /* Set offset snr for adaptive tone map. */
        if (R_G3_BANDPLAN_CENELEC_A == g_demo_config.bandPlan)
        {
            tmpArray[0] = (uint8_t)(R_VAL_MAC_OFFSET_SNR_CA);
        }
        else if (R_G3_BANDPLAN_ARIB == g_demo_config.bandPlan)
        {
            tmpArray[0] = (uint8_t)(R_VAL_MAC_OFFSET_SNR_ARIB);
        }
        else if (R_G3_BANDPLAN_FCC == g_demo_config.bandPlan)
        {
            tmpArray[0] = (uint8_t)(R_VAL_MAC_OFFSET_SNR_FCC);
        }
        else
        {
            tmpArray[0] = (uint8_t)(0x00);
        }
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3MAC_IB_OFFSET_SNR, 0, (uint8_t *)tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_GET_TONE_MASK;

        break;

    case CPX3_GET_TONE_MASK:
        /* Get tone mask. */
        R_DEMO_MlmeGetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3MAC_IB_TONEMASK, 0, (uint8_t *)tmpArray);
        get_handle_state = 0;
        current_AppSetIpContent = CPX3_HANDLE_GET_TONE_MASK;
        break;

    case CPX3_HANDLE_GET_TONE_MASK:
        R_DEMO_MlmeGetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3MAC_IB_TONEMASK, 0, (uint8_t *)tmpArray);
        get_handle_state = 1;
        config_monitor = 0;
        current_AppSetIpContent = CPX3_SET_TONE_MASK;

        break;
        //-------------------------------------------------------------
    case CPX3_SET_TONE_MASK:
        /* Set tone mask. */

        for (i = 0; i < 9; i++)
        {
            tmpArray[i] &= g_demo_config.tonemask[i];
        }
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3MAC_IB_TONEMASK, 0, (uint8_t *)tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_SET_MAX_BE;

        break;
    case CPX3_SET_MAX_BE:

        tmpArray[0] = (uint8_t)(R_VAL_MAC_MAX_BE);
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3MAC_IB_MAX_BE, 0, (uint8_t *)tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_SET_MIN_BE;

        break;

    case CPX3_SET_MIN_BE:
        /* Set Min Be */
        tmpArray[0] = (uint8_t)(R_VAL_MAC_MIN_BE);
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3MAC_IB_MIN_BE, 0, (uint8_t *)tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_SET_SET_MAX_A;

        break;

    case CPX3_SET_SET_MAX_A:
        /* Set max A */
        tmpArray[0] = (uint8_t)(R_VAL_MAC_A);
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3MAC_IB_A, 0, (uint8_t *)tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_SET_MAC_BEACONE_RAND_WL;

        break;

    case CPX3_SET_MAC_BEACONE_RAND_WL:

        /* Set macBeaconRandomizationWindowLength */
        tmpArray[0] = (uint8_t)(R_VAL_MAC_BEACON_RAND_WIDTH);
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3MAC_IB_BEACON_RAND_WIN_LENGTH, 0, (uint8_t *)tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_SET_MAC_BROADCASET_DATA_TX_TIMEOUT;

        break;
    case CPX3_SET_MAC_BROADCASET_DATA_TX_TIMEOUT:
        /* Set macBroadcastDataTxTimeout */
        tmpArray[0] = (uint8_t)(R_VAL_MAC_BCAST_TX_TIMEOUT);
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3MAC_IB_BROADCAST_DATA_TX_TIMEOUT, 0, (uint8_t *)tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_SET_MAC_RETRY_MAX_CW;

        break;
    case CPX3_SET_MAC_RETRY_MAX_CW:
        /* Set macRetryMaxCw */
        tmpArray[0] = (uint8_t)(R_VAL_MAC_RETRY_MAC_CW);
        if (R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, 0x1003u, 0, (uint8_t *)tmpArray) != R_G3MAC_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_SET_DEVICE_TYPE;

        break;
        /* ZIad*/
        /************** ADP IB Settings **************/

    case CPX3_SET_DEVICE_TYPE:
        /* Set the device type. */
        if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_DEVICE_TYPE, 0, (uint8_t *)&g_demo_config.devType) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }

        current_AppSetIpContent = CPX3_ACTIVATE_ADPM_ROUTE_UPD_IND;
        break;

    case CPX3_ACTIVATE_ADPM_ROUTE_UPD_IND:
        /* Activate ADPM-ROUTE-UPDATE.indication. */
        tmpArray[0] = (uint8_t)(R_VAL_ADP_ROUTE_IND_ENABLE);
        if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_ROUTE_IND_ENABLE, 0, (uint8_t *)tmpArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_ACTIVATE_ADPM_RREP_IND;
        break;
    case CPX3_ACTIVATE_ADPM_RREP_IND:
        /* Activate ADPM-RREP.indication. */
        tmpArray[0] = (uint8_t)(R_VAL_ADP_RREP_IND_ENABLE);
        if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_RREP_IND_ENABLE, 0, (uint8_t *)tmpArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_SET_GROUP_TABLE_ENTERIES;
        break;
    case CPX3_SET_GROUP_TABLE_ENTERIES:
        /* Set group table entries. */
        if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_GROUP_TABLE, 0, (uint8_t *)g_group_table_entry_8001) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_SET_ADP_DEFAULT_COORD_ROUTE_ENABLE;
        break;
    case CPX3_SET_ADP_DEFAULT_COORD_ROUTE_ENABLE:
        /* Set adpDefaultCoordRouteEnabled for CCTT187. */
        if (R_ADP_DEVICE_TYPE_PEER == g_demo_config.devType)
        {
            tmpArray[0] = (uint8_t)(R_VAL_ADP_DEFAULT_COORD_ROUTE_ENABLED);

            if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_DEFAULT_COORD_ROUTE_ENABLED, 0, (uint8_t *)tmpArray) != R_ADP_STATUS_SUCCESS)
            {
                return R_RESULT_FAILED;
            }
        }
        current_AppSetIpContent = CPX3_VAL_APD_MAX_HOPS;
        break;
    case CPX3_VAL_APD_MAX_HOPS:
        // R_VAL_ADP_MAX_HOPS
        tmpArray[0] = R_VAL_ADP_MAX_HOPS;
        if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_MAX_HOPS, 0, (uint8_t *)tmpArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_VAL_APD_LOW_LQI;
        break;
    case CPX3_VAL_APD_LOW_LQI:
        // R_VAL_ADP_LOW_LQI
        tmpArray[0] = R_VAL_ADP_LOW_LQI;
        if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_LOW_LQI_VALUE, 0, (uint8_t *)tmpArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_VAL_APD_HIGH_LQI;
        break;
    case CPX3_VAL_APD_HIGH_LQI:
        // R_VAL_ADP_HIGH_LQI
        tmpArray[0] = R_VAL_ADP_HIGH_LQI;
        if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_HIGH_LQI_VALUE, 0, (uint8_t *)tmpArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_VAL_APD_WEAK_LQI;
        break;
    case CPX3_VAL_APD_WEAK_LQI:
        // R_VAL_ADP_WEAK_LQI
        tmpArray[0] = R_VAL_ADP_WEAK_LQI;
        if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_WEAK_LQI_VALUE, 0, (uint8_t *)tmpArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_VAL_APD_KQ;
        break;
    case CPX3_VAL_APD_KQ:
        // R_VAL_ADP_KQ
        tmpArray[0] = R_VAL_ADP_KQ;
        if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_KQ, 0, (uint8_t *)tmpArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_VAL_APD_KRT;
        break;
    case CPX3_VAL_APD_KRT:
        // R_VAL_ADP_KRT
        tmpArray[0] = R_VAL_ADP_KRT;
        if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_KRT, 0, (uint8_t *)tmpArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_VAL_APD_MAX_JOIN_WAIT_TIME;
        break;
    case CPX3_VAL_APD_MAX_JOIN_WAIT_TIME:
        // R_VAL_ADP_MAX_JOIN_WAIT_TIME
        if (R_ADP_DEVICE_TYPE_PEER == g_demo_config.devType)
        {
            R_BYTE_UInt16ToArr(R_VAL_ADP_MAX_JOIN_WAIT_TIME, tmpArray);
            if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_MAX_JOIN_WAIT_TIME, 0, (uint8_t *)tmpArray) != R_ADP_STATUS_SUCCESS)
            {
                return R_RESULT_FAILED;
            }
        }
        current_AppSetIpContent = CPX3_VAL_APD_NET_TRANSVERAL_TIME;
        break;
    case CPX3_VAL_APD_NET_TRANSVERAL_TIME:
        // R_VAL_ADP_NET_TRAVERSAL_TIME
        R_BYTE_UInt16ToArr(R_VAL_ADP_NET_TRAVERSAL_TIME, tmpArray);
        if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_NET_TRAVERSAL_TIME, 0, (uint8_t *)tmpArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        current_AppSetIpContent = CPX3_VAL_APD_PREP_WAIT;
        break;
    case CPX3_VAL_APD_PREP_WAIT:
        // R_VAL_ADP_RREP_WAIT
        tmpArray[0] = R_VAL_ADP_RREP_WAIT;
        if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_RREP_WAIT, 0, (uint8_t *)tmpArray) != R_ADP_STATUS_SUCCESS)
        {
            return R_RESULT_FAILED;
        }
        cpx3_current_task = CPX3_APP_PRESET_PROCESS;
        current_AppSetIpContent = CPX3_TX_GAIN;
        break;

    default:
        break;
    }

    /************** MAC PIB Settings **************/

    /************** ADP IB Settings **************/

    return R_RESULT_SUCCESS;
} /* R_DEMO_SetIbContent */
/******************************************************************************
   End of function  R_DEMO_SetIbContent
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_SetContext0forCert
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_SetContext0forCert(uint16_t panId)
{
    uint8_t tmpArray[R_ADP_MAX_IB_SIZE];

    R_memcpy(tmpArray, g_contextinfo_table_entry_00, sizeof(g_contextinfo_table_entry_00));
    tmpArray[10] = (uint8_t)(panId >> 8);
    tmpArray[11] = (uint8_t)(panId & 0xFF);

    /* Set context table entry 0. */
    if (R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_CONTEXT_INFORMATION_TABLE, 0, (uint8_t *)tmpArray) != R_ADP_STATUS_SUCCESS)
    {
        return R_RESULT_FAILED;
    }

    return R_RESULT_SUCCESS;
} /* R_DEMO_SetContext0forCert */
/******************************************************************************
   End of function  R_DEMO_SetContext0forCert
******************************************************************************/

#if !defined(R_DEFINE_DISABLE_APP_HYBRID)

/******************************************************************************
 * Function Name: R_DEMO_RfBandSwitch
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_demo_config_status_t R_DEMO_RfBandSwitch(r_demo_rf_config_t *pRfConfig)
{
    if (
        (R_RFMAC_FREQ_BAND_MAX <= pRfConfig->macFrequencyBand_RF) || (R_RFMAC_FSK_OPEMODE_MAX < pRfConfig->macOperatingMode_RF) || (0 == pRfConfig->macOperatingMode_RF) || (pRfConfig->freqHopExt))
    {
        return R_CONFIG_STATUS_UNSUPPORTED;
    }

    if (
        (pRfConfig->macOperatingMode_RF != g_demo_rfmac_supportBand[pRfConfig->macFrequencyBand_RF][0]) && (pRfConfig->macOperatingMode_RF != g_demo_rfmac_supportBand[pRfConfig->macFrequencyBand_RF][1]))
    {
        return R_CONFIG_STATUS_UNSUPPORTED;
    }

    if (R_MEDIA_TYPE0_PLC == g_demo_config.mediaType)
    {
        return R_CONFIG_STATUS_UNSUPPORTED;
    }

    if (R_DEMO_RfMlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3RFMAC_IB_FREQBAND_RF, 0, (uint8_t *)&pRfConfig->macFrequencyBand_RF) != R_G3MAC_STATUS_SUCCESS)
    {
        return R_CONFIG_STATUS_FAILED;
    }

    if (R_DEMO_RfMlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3RFMAC_IB_OPEMODE_RF, 0, (uint8_t *)&pRfConfig->macOperatingMode_RF) != R_G3MAC_STATUS_SUCCESS)
    {
        return R_CONFIG_STATUS_FAILED;
    }

    if (R_DEMO_RfMlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3RFMAC_IB_CHANNEL_NUMBER_RF, 0, (uint8_t *)&pRfConfig->chNumber) != R_G3MAC_STATUS_SUCCESS)
    {
        return R_CONFIG_STATUS_FAILED;
    }

    return R_CONFIG_STATUS_SUCCESS;
} /* R_DEMO_RfBandSwitch */
/******************************************************************************
   End of function  R_DEMO_RfBandSwitch
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_RfContinuousTx
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_RfContinuousTx()
{
    uint8_t tmpArray[R_ADP_MAX_IB_SIZE];

    if (R_MEDIA_TYPE0_PLC == g_demo_config.mediaType)
    {
        return;
    }

    tmpArray[0] = R_TRUE;
    if (R_DEMO_RfMlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3RFMAC_IB_CONTINUOUS_TX_ENABLED_RF, 0, (uint8_t *)tmpArray) != R_G3MAC_STATUS_SUCCESS)
    {
        return;
    }
    return;
} /* R_DEMO_RfContinuousTx */
/******************************************************************************
   End of function  R_DEMO_RfBandSwitch
******************************************************************************/
#endif

/******************************************************************************
 * Function Name: R_DEMO_AppPreserveProcess
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
r_result_t R_DEMO_AppPreserveProcess(uint32_t frameCounter, uint8_t mediaType)
{
    r_demo_backup_payload_t tmpBkup;
    uint8_t tmpArray[R_ADP_MAX_IB_SIZE];
    uint32_t tmp = frameCounter;

    r_demo_et_read_backup(R_DEMO_G3_USE_PRIMARY_CH, (uint8_t *)&tmpBkup);

    if (g_demo_config.secyrityF2)
    {
        return R_RESULT_FAILED;
    }

    if (mediaType)
    {
        tmp += R_FRAMECOUNT_RF_INTERVAL - (tmp % R_FRAMECOUNT_RF_INTERVAL);
        R_BYTE_UInt32ToArr(tmp, tmpBkup.FrameCounterRF);
    }
    else
    {
        tmp = (tmp + R_FRAMECOUNT_INTERVAL - 1) & (0xFFFFFFFF - (R_FRAMECOUNT_INTERVAL - 1));
        R_BYTE_UInt32ToArr(tmp, tmpBkup.FrameCounter);
    }

    if (R_DEMO_AdpmGetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_LOAD_SEQ_NUMBER, 0, tmpArray) != R_ADP_STATUS_SUCCESS)
    {
        return R_RESULT_FAILED;
    }
    R_memcpy(tmpBkup.LoadSeqNo, tmpArray, 2);

    r_demo_nvm_backup_write(R_DEMO_G3_USE_PRIMARY_CH, &tmpBkup);
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
        r_demo_print_frame_conter(R_DEMO_G3_USE_PRIMARY_CH, tmp, mediaType);
    }
    return R_RESULT_SUCCESS;
} /* R_DEMO_AppPreserveProcess */
#else
r_result_t R_DEMO_AppPreserveProcess(uint32_t framecounter)
{
    r_demo_backup_payload_t tmpBkup;
    uint8_t tmpArray[R_ADP_MAX_IB_SIZE];

    R_memset(&tmpBkup, 0, sizeof(tmpBkup));

    if (g_demo_config.secyrityF2)
    {
        return R_RESULT_FAILED;
    }

    R_BYTE_UInt32ToArr(framecounter, tmpBkup.FrameCounter);

    if (R_DEMO_AdpmGetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_LOAD_SEQ_NUMBER, 0, tmpArray) != R_ADP_STATUS_SUCCESS)
    {
        return R_RESULT_FAILED;
    }
    R_memcpy(tmpBkup.LoadSeqNo, tmpArray, 2);

    r_demo_nvm_backup_write(R_DEMO_G3_USE_PRIMARY_CH, &tmpBkup);
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
        r_demo_print_frame_conter(R_DEMO_G3_USE_PRIMARY_CH, framecounter);
    }
    return R_RESULT_SUCCESS;
} /* R_DEMO_AppPreserveProcess */
#endif
/******************************************************************************
   End of function  R_DEMO_AppPreserveProcess
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_AppPresetProcess
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_AppPresetProcess(void)
{

    static uint8_t tmpArray[R_ADP_MAX_IB_SIZE];
    uint32_t tmp32;
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    uint32_t tmp32_2;
#endif
    uint16_t tmp16;

    if (g_demo_config.secyrityF2)
    {
        return R_RESULT_FAILED;
    }

    r_demo_et_read_backup(R_DEMO_G3_USE_PRIMARY_CH, (uint8_t *)&sDemoBackup);

    switch (current_AppPresetProcess)
    {
    case CPX3_SET_IB_FRAME_COUNTER:

        tmp32 = (R_BYTE_ArrToUInt32(sDemoBackup.FrameCounter) + R_FRAMECOUNT_INTERVAL) - 1;
        R_BYTE_UInt32ToArr(tmp32, tmpArray);
        R_DEMO_MlmeSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_G3MAC_IB_FRAME_COUNTER, 0, tmpArray);
        current_AppPresetProcess = CPX3_SET_IB_LOAD_SEQ_NUMBER;

        break;

    case CPX3_SET_IB_LOAD_SEQ_NUMBER:

        tmp16 = (uint16_t)((R_BYTE_ArrToUInt16(sDemoBackup.LoadSeqNo) + R_FRAMECOUNT_INTERVAL) - 1);
        R_BYTE_UInt16ToArr(tmp16, tmpArray);
        R_DEMO_AdpmSetWrap(R_DEMO_G3_USE_PRIMARY_CH, R_ADP_IB_LOAD_SEQ_NUMBER, 0, tmpArray);
        cpx3_current_task = CPX3_APP_WAIT_INSERT;

        break;

    default:
        break;
    }

    return R_RESULT_SUCCESS;
} /* R_DEMO_AppPresetProcess */
/******************************************************************************
   End of function  R_DEMO_AppPresetProcess
******************************************************************************/
