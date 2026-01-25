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
 * File Name : r_g3rfmac_sap.h
 * Description : G3 RF MAC layer API
 ******************************************************************************/
#include "r_g3mac_sap.h"

#ifndef R_G3RFMAC_SAP_H
#define R_G3RFMAC_SAP_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_G3RFMAC_MAX_IB_SIZE       (0x40u)
#define R_G3RFMAC_KEY_NUM           (2u)
#define R_G3RFMAC_MAX_DATA_LENGTH   (0x0400)    /* T.B.D */

#define R_RFMAC_FREQ_BAND_863         (0x00)
#define R_RFMAC_FREQ_BAND_866         (0x01)
#define R_RFMAC_FREQ_BAND_870         (0x02)
#define R_RFMAC_FREQ_BAND_915         (0x03)
#define R_RFMAC_FREQ_BAND_915A        (0x04)
#define R_RFMAC_FREQ_BAND_915B        (0x05)
#define R_RFMAC_FREQ_BAND_915C        (0x06)
#define R_RFMAC_FREQ_BAND_920         (0x07)

#define R_RFMAC_FSK_OPEMODE_1         (0x01)
#define R_RFMAC_FSK_OPEMODE_2         (0x02)
#define R_RFMAC_FSK_OPEMODE_3         (0x03)
#define R_RFMAC_FSK_OPEMODE_4         (0x04)
#define R_RFMAC_FSK_OPEMODE_5         (0x05)
#define R_RFMAC_FSK_OPEMODE_6         (0x06)

#define R_RFMAC_MAKE_BAND(freqband, opemode)  ((uint16_t)((freqband << 8u) | opemode))
/******************************************************************************
Macro definitions
******************************************************************************/

typedef enum
{
    R_G3RFMAC_IB_PHY_TRANSMIT_POWER             =   0x0002,
    R_G3RFMAC_IB_PHY_RSSI_OUTPUT_OFFSET         =   0x0006,
    R_G3RFMAC_IB_PHY_CCA_BANDWIDTH              =   0x0007,
    R_G3RFMAC_IB_PHY_CCA_DURATION               =   0x0008,
    R_G3RFMAC_IB_PHY_FSK_PREAMBLE_LENGTH        =   0x0009,
    R_G3RFMAC_IB_PHY_FSK_FEC_RX_ENA             =   0x000E,
    R_G3RFMAC_IB_PHY_FSK_FEC_TX_ENA             =   0x000F,
    R_G3RFMAC_IB_PHY_ANTENNA_SWITCH_ENA         =   0x0013,
    R_G3RFMAC_IB_PHY_ANTENNA_SELECT_TX          =   0x0015,
    R_G3RFMAC_IB_PHY_REGULATORY_MODE            =   0x001A,
    R_G3RFMAC_IB_PHY_AGC_WAIT_GAIN_OFFSET       =   0x001B,
    R_G3RFMAC_IB_PHY_CCA_VTH                    =   0x001C,
    R_G3RFMAC_IB_PHY_CCA_VTH_OFFSET             =   0x001D,
    R_G3RFMAC_IB_PHY_ANTENNA_SWITCH_ENA_TIMING  =   0x001E,
    R_G3RFMAC_IB_PHY_GPIO0_SETTING              =   0x001F,
    R_G3RFMAC_IB_PHY_DATA_RATE                  =   0x0021,
    R_G3RFMAC_IB_PHY_RMODET_ON_MAX              =   0x0024,
    R_G3RFMAC_IB_PHY_RMODET_OFF_MIN             =   0x0025,
    
    R_G3RFMAC_IB_PANID                          =   0x0050,
    R_G3RFMAC_IB_PROMISCUOUS_MODE               =   0x0051,
    R_G3RFMAC_IB_RX_ON_WHEN_IDLE                =   0x0052,
    R_G3RFMAC_IB_SHORTADDRESS                   =   0x0053,
    R_G3RFMAC_IB_SECURITY_ENABLED               =   0x005D,
    R_G3RFMAC_IB_FRAMECNT_IND_INTERVAL          =   0x0062,
    R_G3RFMAC_IB_KEY_TABLE                      =   0x0071,
    R_G3RFMAC_IB_METRICSENABLED                 =   0x0082,
    R_G3RFMAC_IB_RXFRAMECOUNTER_FLUSH           =   0x00A7,
    R_G3RFMAC_IB_RF_FRONTEND_GAIN               =   0x00A9,
    R_G3RFMAC_IB_POS_TABLE_ENTRY_TTL            =   0x010E,
    R_G3RFMAC_IB_RCCOORD                        =   0x010F,
    R_G3RFMAC_IB_BEACON_RAND_WIN_LENGTH         =   0x0111,
    
    R_G3RFMAC_IB_DSN_RF                         =   0x0200,
    R_G3RFMAC_IB_MAXBE_RF                       =   0x0201,
    R_G3RFMAC_IB_MAXCSMABACKOFFS_RF             =   0x0202,
    R_G3RFMAC_IB_MAXFRAMERETRIES_RF             =   0x0203,
    R_G3RFMAC_IB_MINBE_RF                       =   0x0204,
    R_G3RFMAC_IB_TIMESTAMPSUPPORTED_RF          =   0x0205,
    R_G3RFMAC_IB_DEVICETABLE_RF                 =   0x0206,
    R_G3RFMAC_IB_FRAMECOUNTER_RF                =   0x0207,
    R_G3RFMAC_IB_DUPLICATEDETECTIONTTL_RF       =   0x0208,
    R_G3RFMAC_IB_COUNTEROCTETS_RF               =   0x0209,
    R_G3RFMAC_IB_RETRYCOUNT_RF                  =   0x020A,
    R_G3RFMAC_IB_MULTIPLERETRYCOUNT_RF          =   0x020B,
    R_G3RFMAC_IB_TXFAILCOUNT_RF                 =   0x020C,
    R_G3RFMAC_IB_TXSUCCESSCOUNT_RF              =   0x020D,
    R_G3RFMAC_IB_FCSERRORCOUNT_RF               =   0x020E,
    R_G3RFMAC_IB_SECURITYFAILURE_RF             =   0x020F,
    R_G3RFMAC_IB_DUPLICATEFRAMECOUNT_RF         =   0x0210,
    R_G3RFMAC_IB_RXSUCCESSCOUNT_RF              =   0x0211,
    R_G3RFMAC_IB_EBRFILTER_RF                   =   0x0214,
    R_G3RFMAC_IB_BEACONAUTORESPOND_RF           =   0x0216,
    R_G3RFMAC_IB_USEENHANCEDBEACON_RF           =   0x0217,
    R_G3RFMAC_IB_EBSN_RF                        =   0x021B,
    R_G3RFMAC_IB_POSTABLE_RF                    =   0x021D,
    R_G3RFMAC_IB_OPEMODE_RF                     =   0x021E,
    R_G3RFMAC_IB_CHANNEL_NUMBER_RF              =   0x021F,
    R_G3RFMAC_IB_DUTY_CYCLE_USAGE_RF            =   0x0220,
    R_G3RFMAC_IB_DUTY_CYCLE_PERIOD_RF           =   0x0221,
    R_G3RFMAC_IB_DUTY_CYCLE_LIMIT_RF            =   0x0222,
    R_G3RFMAC_IB_DUTY_CYCLE_THRESHOLD_RF        =   0x0223,
    R_G3RFMAC_IB_DISABLE_PHY_RF                 =   0x0224,
    R_G3RFMAC_IB_FREQBAND_RF                    =   0x0225,
	R_G3RFMAC_IB_TRANSMITATTEN_RF               =   0x0226,
	R_G3RFMAC_IB_ADAPTIVEPOWERSTEP              =   0x0227,
	R_G3RFMAC_IB_ADAPTIVEPOWERHIGHBOUND         =   0x0228,
	R_G3RFMAC_IB_ADAPTIVEPOWERLOWBOUND          =   0x0229,
    R_G3RFMAC_IB_BEACON_RAND_WIN_LENGTH_RF      =   0x0230,
    R_G3RFMAC_IB_MAC_MAX_CCA_ATTEMPTS_RETRIES   =   0x0232,
    R_G3RFMAC_IB_MAC_INITIAL_RETRY_TIME         =   0x0234,
    R_G3RFMAC_IB_MAC_MAXIMUM_RETRY_TIME         =   0x0235,

    R_G3RFMAC_IB_KEY_STATE                        =   0x0807,
    R_G3RFMAC_IB_EXTADDRESS                       =   0x0808,
    R_G3RFMAC_IB_SOFT_VERSION                     =   0x0809,
    R_G3RFMAC_IB_DEVICE_TABLE_BY_SHORT_ADDR       =   0x080F,
    R_G3RFMAC_IB_POS_TABLE_BY_SHORT_ADDR          =   0x0813,
	R_G3RFMAC_IB_CONTINUOUS_TX_ENABLED_RF         =   0x0814,
	R_G3RFMAC_IB_MAC_G3CERTCOMPATIBILITY_VERSION  =   0x0815,

} r_g3rfmac_ib_id_t;

typedef uint8_t r_g3rfmac_cb_status_t;

typedef enum
{
    /*  These are MAC statuses  */
    R_G3RFMAC_STATUS_SUCCESS                 =   0x00,
    R_G3RFMAC_STATUS_DUTY_CYCLE_REACHED      =   0xD0,
    R_G3RFMAC_STATUS_TX_ACTIVE               =   0xF2
} r_g3rfmac_status_t;

typedef enum
{
    R_G3RFMAC_TGET_RX_BLF_ERR_WHEN_MCPS_DATA_INDICATION          = 0x01,
    R_G3RFMAC_TGET_RX_BLF_ERR_WHEN_MLME_BEACON_NOTIFY_INDICATION = 0x06,
    R_G3RFMAC_TGET_RX_BLF_ERR_WHEN_MLME_COMM_STATUS_INDICATION   = 0x07
} r_g3rfmac_warning_status_t;


typedef enum
{
    EU1_863MHZ_MODE1 = R_RFMAC_MAKE_BAND(R_RFMAC_FREQ_BAND_863, R_RFMAC_FSK_OPEMODE_1),
    EU1_863MHZ_MODE2 = R_RFMAC_MAKE_BAND(R_RFMAC_FREQ_BAND_863, R_RFMAC_FSK_OPEMODE_2),
    EU1_866MHZ_MODE1 = R_RFMAC_MAKE_BAND(R_RFMAC_FREQ_BAND_866, R_RFMAC_FSK_OPEMODE_1),
    EU1_866MHZ_MODE2 = R_RFMAC_MAKE_BAND(R_RFMAC_FREQ_BAND_866, R_RFMAC_FSK_OPEMODE_2),
    EU2_870MHZ_MODE1 = R_RFMAC_MAKE_BAND(R_RFMAC_FREQ_BAND_870, R_RFMAC_FSK_OPEMODE_1),
    EU2_870MHZ_MODE2 = R_RFMAC_MAKE_BAND(R_RFMAC_FREQ_BAND_870, R_RFMAC_FSK_OPEMODE_2),
    US_915MHZ_MODE1  = R_RFMAC_MAKE_BAND(R_RFMAC_FREQ_BAND_915, R_RFMAC_FSK_OPEMODE_1),
    US_915MHZ_MODE3  = R_RFMAC_MAKE_BAND(R_RFMAC_FREQ_BAND_915, R_RFMAC_FSK_OPEMODE_3),
    US_915AMHZ_MODE1 = R_RFMAC_MAKE_BAND(R_RFMAC_FREQ_BAND_915A, R_RFMAC_FSK_OPEMODE_1),
    US_915AMHZ_MODE4 = R_RFMAC_MAKE_BAND(R_RFMAC_FREQ_BAND_915A, R_RFMAC_FSK_OPEMODE_4),
    US_915BMHZ_MODE1 = R_RFMAC_MAKE_BAND(R_RFMAC_FREQ_BAND_915B, R_RFMAC_FSK_OPEMODE_1),
    US_915BMHZ_MODE4 = R_RFMAC_MAKE_BAND(R_RFMAC_FREQ_BAND_915B, R_RFMAC_FSK_OPEMODE_4),
    US_915CMHZ_MODE1 = R_RFMAC_MAKE_BAND(R_RFMAC_FREQ_BAND_915C, R_RFMAC_FSK_OPEMODE_1),
    US_915CMHZ_MODE4 = R_RFMAC_MAKE_BAND(R_RFMAC_FREQ_BAND_915C, R_RFMAC_FSK_OPEMODE_4),
    JP_920MHZ_MODE1  = R_RFMAC_MAKE_BAND(R_RFMAC_FREQ_BAND_920, R_RFMAC_FSK_OPEMODE_1),
    JP_920MHZ_MODE6  = R_RFMAC_MAKE_BAND(R_RFMAC_FREQ_BAND_920, R_RFMAC_FSK_OPEMODE_6),
} r_rfmac_band_setting_t;

typedef struct
{
    uint8_t shortAddress[2];
    uint8_t fwdLqi;
    uint8_t revLqi;
    uint8_t dutyCycle;
    uint8_t fwdTxPwrOfst;
    uint8_t revTxPwrOfst;
    uint8_t posValidTime;
} r_g3rfmac_mac_pos_table_t;


typedef struct
{
    uint8_t scrAddr[2];
    uint8_t frameCounter[4];
} r_g3rfmac_device_table_t;


typedef struct
{
    uint8_t key[16];
} r_g3rfmac_key_table_t;


typedef struct
{
    uint8_t keyState;
} r_g3rfmac_key_valid_table_t;


typedef struct
{
    uint8_t macVersion[3];
    uint8_t dspVersion[10];
} r_g3rfmac_soft_version_t;


/*===========================================================================*/
/* For MAC SAP IF STRUCT                                                     */
/*===========================================================================*/

/* RF-MCPS-DATA */
typedef struct
{
    uint8_t                 srcAddrMode;
    uint8_t                 dstAddrMode;
    uint16_t                dstPanId;
    uint8_t                 dstAddr[8];
    uint16_t                msduLength;
    uint8_t                 msduHandle;
    uint8_t                 txOptions;
    uint8_t                 secured;
    uint8_t                 keyIndex;
    uint8_t                 frameControlOptions;
    uint16_t                headerIELength;
    uint16_t                payloadIELength;
    uint8_t *               pHeaderIE;
    uint8_t *               pPayloadIE;
    uint8_t *               pMsdu;
} r_g3rfmac_mcps_data_req_t;

typedef struct
{
    uint8_t                 msduHandle;
    r_g3mac_cb_status_t     status;
} r_g3rfmac_mcps_data_cnf_t;

typedef struct
{
    uint8_t                 srcAddrMode;
    uint16_t                srcPanId;
    uint8_t                 srcAddr[8];
    uint8_t                 dstAddrMode;
    uint16_t                dstPanId;
    uint8_t                 dstAddr[8];
    uint16_t                msduLength;
    uint8_t                 msduLinkQuality;
    uint8_t                 dsn;
    uint8_t                 secured;
    uint8_t                 keyIndex;
    uint16_t                headerIELength;
    uint16_t                payloadIELength;
    uint8_t *               pHeaderIE;
    uint8_t *               pPayloadIE;
    uint8_t *               pMsdu;
} r_g3rfmac_mcps_data_ind_t;

/* RF-MLME-RESET  */
typedef struct
{
    uint8_t                 setDefaultPIB;
} r_g3rfmac_mlme_reset_req_t;

typedef struct
{
    r_g3rfmac_cb_status_t   status;
} r_g3rfmac_mlme_reset_cnf_t;

/* RF-MLME-GET */
typedef struct
{
    uint16_t                pibAttributeId;
    uint16_t                pibAttributeIndex;
} r_g3rfmac_mlme_get_req_t;

typedef struct
{
    r_g3rfmac_cb_status_t   status;
    uint16_t                pibAttributeId;
    uint16_t                pibAttributeIndex;
    uint8_t                 pibAttributeValue[R_G3RFMAC_MAX_IB_SIZE];
} r_g3rfmac_mlme_get_cnf_t;

/* RF-MLME-SET */
typedef struct
{
    uint16_t                pibAttributeId;
    uint16_t                pibAttributeIndex;
    uint8_t *               pibAttributeValue;
} r_g3rfmac_mlme_set_req_t;

typedef struct
{
    r_g3rfmac_cb_status_t   status;
    uint16_t                pibAttributeId;
    uint16_t                pibAttributeIndex;
} r_g3rfmac_mlme_set_cnf_t;

/* RF-MLME-SCAN */
typedef struct
{
    uint8_t                 scanDuration;
} r_g3rfmac_mlme_scan_req_t;

typedef struct
{
    r_g3rfmac_cb_status_t   status;
} r_g3rfmac_mlme_scan_cnf_t;


/* RF-MLME-START */
typedef struct
{
    uint16_t                panId;
} r_g3rfmac_mlme_start_req_t;

typedef struct
{
    r_g3rfmac_cb_status_t   status;
} r_g3rfmac_mlme_start_cnf_t;

typedef struct
{
    uint16_t panId;
    uint16_t address;
    uint8_t  linkQuality;
} r_g3rfmac_pan_descriptor_t;


/* RF-MLME-BEACON-NOTIFY */
typedef struct
{
    r_g3rfmac_pan_descriptor_t  panDescriptor;
    uint8_t                     ebsn;
    uint8_t                     beaconPayloadLength;
    uint8_t                     *pBeaconPayload;
} r_g3rfmac_mlme_bcn_notify_ind_t;


/*  RF-MLME-COMM-STATUS */
typedef struct
{
    uint16_t                panId;
    uint8_t                 srcAddrMode;
    uint8_t                 srcAddr[8];
    uint8_t                 dstAddrMode;
    uint8_t                 dstAddr[8];
    r_g3rfmac_cb_status_t   status;
    uint8_t                 secured;
    uint8_t                 keyIndex;
} r_g3rfmac_mlme_comm_status_ind_t;

/* RF-MLME-FRAMECOUNT */
typedef struct
{
    uint32_t                frameCounter;
} r_g3rfmac_mlme_framecount_ind_t;

/* RF-MLME-WARNING */
typedef struct
{
    uint8_t                 status;             /* r_g3rfmac_warning_status_t */
} r_g3rfmac_mlme_warning_ind_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#endif  /*  R_G3RFMAC_SAP_H   */

