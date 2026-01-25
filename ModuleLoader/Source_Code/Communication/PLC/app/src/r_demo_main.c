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
 * File Name   : r_demo_main.c
 *    @version
 *        $Rev: 11089 $
 *    @last editor
 *        $Author: a5089763 $
 *    @date
 *        $Date:: 2022-07-12 11:09:31 +0900#$
 * Description :
 ******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include "Flash.h"
#include "r_typedefs.h"
#include "r_config.h"
#include "r_stdio_api.h"
#if !defined(R_DEFINE_OPEN_RX)
#include "r_bsp_api.h"
#include "r_byte_swap.h"
#include "r_app_timer.h"
#include "r_mem_tools.h"
#include "r_fw_download.h"
#endif /* R_DEFINE_OPEN_ARM */

/* g3 part */
#include "r_c3sap_api.h"

/* app part */
#include "r_demo_tools.h"
#include "r_demo_app.h"
#include "r_demo_sys.h"
#include "r_demo_app_thread.h"
#include "r_demo_app_eap.h"

#include "r_demo_nvm_process.h"
#include "r_demo_main.h"

#include "g3v2021_eap_g3both_cpx3_4band_none_v0600_28072022.h"
extern const uint8_t g_cpxprogtbl[];
const uint8_t *g_cpxrfprogtbl = NULL;
#define FW_SIZE (uint32_t)228964U
/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/
#define R_DEMO_DL_OFFSET_TIME (30000)
/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/
#if !defined(R_DEFINE_OPEN_RX)
static const uint32_t r_demo_baud_table[] =
    {
        9600,
        19200,
        38400,
        57600,
        115200,
        230400,
        300000,
        375000,
        460800,
        500000,
        750000,
        937500,
        1000000};
#endif

r_demo_buff_t g_demo_buff;
r_demo_entity_t g_demo_entity;
uint8_t g_demo_dump_segment[R_APP_DUMP_SEGMENT_BUFF_SIZE];
extern const r_demo_config_t g_cdemo_config;
extern r_demo_config_t g_demo_config;

volatile r_demo_g3_cb_str_t g_g3cb[R_G3_CH_MAX];

/******************************************************************************
Private global variables and functions
******************************************************************************/

/******************************************************************************
Functions
******************************************************************************/
void erase_flash_sector_from_to(uint16_t start, uint16_t end)
{
    for (uint16_t i = start; i <= end; i++)
    {
        FLASH_gEraseSector_ByIndex(i, 80);
    }
}

uint8_t write_fw_toflash(void)
{
    uint8_t flash_addr[4];
    volatile uint8_t written = 0;
    volatile uint8_t verified = 0;
    uint8_t page[256] = {0};
    uint32_t remaining_size = 0;
    memset(flash_addr, 0, 4);
    erase_flash_sector_from_to(800, 860);
    for (uint32_t i = 0; i < sizeof(g_cpxprogtbl); i += 256)
    {
        remaining_size = sizeof(g_cpxprogtbl) - i;
        remaining_size = (remaining_size > 256) ? 256 : remaining_size;
        memset(page, 0x00, 256);
        memcpy(page, ((g_cpxprogtbl) + i), remaining_size);
        ConvertUint32ToBytes(flash_addr, (800 * 4096 + i));
        if (FLASH_gWrite(flash_addr, page, (uint16_t)remaining_size))
        {
            written = 0;
            break;
        }
        else
            written = 1;
    }
    for (uint32_t i = 0; i < sizeof(g_cpxprogtbl); i += 256)
    {
        ConvertUint32ToBytes(flash_addr, (800 * 4096 + i));
        remaining_size = sizeof(g_cpxprogtbl) - i;
        remaining_size = (remaining_size > 256) ? 256 : remaining_size;
        memset(page, 0x00, 256);
        if (FLASH_gRead(flash_addr, page, (uint16_t)remaining_size))
        {
            verified = 0;
            break;
        }
        else if (memcmp(((g_cpxprogtbl) + i), page, remaining_size))
        {
            verified = 0;
            break;
        }
        else
            verified = 1;
    }
    return written && verified;
}
/******************************************************************************
 * Function Name: R_CPX3_Boot
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
static r_result_t R_CPX3_Boot(const FAR uint8_t *pCpxFw, const FAR uint8_t *pRfFw)
{
    r_sys_callback_t callbacks;
    r_sys_boot_info_t bootInfo;
    uint32_t fw_size;

    bootInfo.p_fw_plc = pCpxFw;
    bootInfo.p_fw_rf = pRfFw;
    bootInfo.dl_baud = 12u;
    bootInfo.cmd_baud = 12u;
    fw_size = FW_SIZE;
    if (NULL != bootInfo.p_fw_rf)
    {
        fw_size = FW_SIZE + sizeof(g_cpxrfprogtbl);
    }

    bootInfo.dl_timeout = ((((fw_size) * 10) * 1000) / (r_demo_baud_table[bootInfo.dl_baud])) + R_DEMO_DL_OFFSET_TIME;

    if (R_DEMO_InitSysCallBack(&callbacks) != R_RESULT_SUCCESS)
    {
        return R_RESULT_FAILED;
    }

    if (R_SYS_Init(&bootInfo, &callbacks) != R_RESULT_SUCCESS)
    {
        return R_RESULT_FAILED;
    }
    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_CPX3_Boot
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_ModemBoot
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
r_result_t R_DEMO_ModemBoot(void)
{
    /* This is the code performing the FW download */
    /* Start firmware download. */

    if (R_CPX3_Boot(g_cpxprogtbl, g_cpxrfprogtbl) != R_RESULT_SUCCESS)
    {
        /* Indicate error. */
        R_DEMO_LED(R_DEMO_G3_USE_PRIMARY_CH, R_DEMO_LED_ALERT);

        return R_RESULT_FAILED;
    }
    else
    {
        /* Indicate that CPX start-up finished sucessfully. */
        R_DEMO_LED(R_DEMO_G3_USE_PRIMARY_CH, R_DEMO_LED_BOOT);
    }
    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_DEMO_ModemBoot
******************************************************************************/

/******************************************************************************
 * Function Name: R_DEMO_ModemReboot
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/
void R_DEMO_ModemReboot(void)
{
    R_memset((uint8_t *)&g_demo_buff, 0, sizeof(r_demo_buff_t));
    R_memset((uint8_t *)&g_demo_entity, 0, sizeof(r_demo_entity_t));

    g_demo_config.macPromiscuousEnabled = R_FALSE;

    /* Boot the modem. */
    R_DEMO_ModemBoot();
}
/******************************************************************************
   End of function  R_DEMO_ModemReboot
******************************************************************************/

/******************************************************************************
 * Function Name: plc_init
 * Description :
 * Arguments :
 * Return Value :
 ******************************************************************************/

void plc_init(void)
{
    r_demo_mode_t demo_mode;                                                      // local
    volatile r_demo_mode_ch_t *pModeCh = &demo_mode.ch[R_DEMO_G3_USE_PRIMARY_CH]; // local

    R_TIMER_Init(R_HW_TIMER_TICK_PERIOD);
    g_demo_config = g_cdemo_config;

    /* Boot the modem. */
    if (R_DEMO_ModemBoot() != R_RESULT_SUCCESS)
    {
        /* Indicate error. */
        R_DEMO_LED(R_DEMO_G3_USE_PRIMARY_CH, R_DEMO_LED_ALERT);
        while (1)
        { /**/
        };
    }

    /* Initialize demo nvm */
    r_demo_nvm_init();

    /* Initialize demo application thread. */
    R_DEMO_AppThreadInit();

    /* Initialize data flash if not yet done. */
    r_demo_nvm_config_init(R_TRUE, &g_cdemo_config);
    pModeCh->startMode = R_DEMO_MODE_CUI;
    pModeCh->g3mode = R_G3_MODE_ADP;
    g_demo_config.bandPlan = (r_g3_bandplan_t)demo_mode.bandPlan;
    g_demo_config.appMode = (r_demo_operation_mode_t)pModeCh->startMode;
    g_demo_config.devType = (R_G3_MODE_EAP == (pModeCh->g3mode & 0xF)) ? R_ADP_DEVICE_TYPE_COORDINATOR : R_ADP_DEVICE_TYPE_PEER;
    g_demo_config.extIdFlg = pModeCh->extIdFlg;

    R_memset((uint8_t *)&g_demo_buff, 0, sizeof(r_demo_buff_t));
    R_memset((uint8_t *)&g_demo_entity, 0, sizeof(r_demo_entity_t));
    uint8_t arr[9] = {143, 217, 254, 123, 157, 14, 82, 21, 202};
    memcpy(demo_mode.tonemask, arr, 9);
    demo_mode.ch[0].g3mode = R_G3_MODE_ADP;
    demo_mode.ch[0].startMode = 0;
    demo_mode.ch[0].extIdFlg = 0;
    demo_mode.ch[1].g3mode = 46;
    demo_mode.ch[1].startMode = 128;
    demo_mode.ch[1].extIdFlg = 239;
    g_demo_config.extIdFlg = 0;
    g_demo_config.bandPlan = R_G3_BANDPLAN_CENELEC_A;
    g_demo_config.appMode = R_DEMO_MODE_AUTO;
    g_demo_config.devType = R_ADP_DEVICE_TYPE_PEER;

    /* Start demo application directly. */
    R_DEMO_App();
}

void R_DEMO_Main(void)
{
#if (R_DEFINE_APP_MODE == R_DEMO_APP_MODE_DEMO) || (R_BOARD_DSW_NUM >= 6)
    r_demo_mode_t demo_mode;
#endif
#if (R_BOARD_DSW_NUM >= 6)
    volatile r_demo_mode_ch_t *pModeCh = &demo_mode.ch[R_DEMO_G3_USE_PRIMARY_CH];
#endif

    /* R_BOARD_TYPE must be defined in the project macro */
    R_BSP_SetBoardType(R_BOARD_TYPE);

    /* Activate LEDs on RSK */
    R_BSP_InitLeds();

    /* Set boot mode */
#if (R_DEFINE_APP_BOOT == R_DEMO_APP_SROM_BOOT)
    R_BSP_SetBootMode(R_BSP_BOOT_SROM);
#else
    R_BSP_SetBootMode(R_BSP_BOOT_UART);
#endif

    /* STDIO init. */
    if (R_STDIO_Init(R_STDIO_OUTPUT_UART_MCU_HOST) != R_RESULT_SUCCESS)
    {
        /* Indicate error. */
        R_DEMO_LED(0u, R_DEMO_LED_ALERT);
        while (1)
        { /**/
        };
    }

    /* Initialize timer module */
    if (R_TIMER_Init(R_HW_TIMER_TICK_PERIOD) != R_RESULT_SUCCESS)
    {
        /* Indicate error. */
        R_DEMO_LED(0u, R_DEMO_LED_ALERT);
        while (1)
        { /**/
        };
    }

    g_demo_config = g_cdemo_config;

    /* Boot the modem. */
    if (R_DEMO_ModemBoot() != R_RESULT_SUCCESS)
    {
        /* Indicate error. */
        R_DEMO_LED(R_DEMO_G3_USE_PRIMARY_CH, R_DEMO_LED_ALERT);
        while (1)
        { /**/
        };
    }

    /* Initialize demo nvm */
    r_demo_nvm_init();

    /* Initialize demo application thread. */
    if (R_DEMO_AppThreadInit() != R_RESULT_SUCCESS)
    {
        /* Indicate error. */
        R_DEMO_LED(R_DEMO_G3_USE_PRIMARY_CH, R_DEMO_LED_ALERT);
        while (1)
        { /**/
        };
    }

    /* Initialize data flash if not yet done. */
    r_demo_nvm_config_init(R_FALSE, &g_cdemo_config);

#if (R_DEFINE_APP_MODE == R_DEMO_APP_MODE_DEMO)
    r_demo_et_read_demo_mode((uint8_t *)&demo_mode);
    g_demo_config.verboseEnabled = R_TRUE;

#else /* R_DEMO_APP_MODE_DEMO */

    // #if ( R_BOARD_DSW_NUM >= 6 )
    //     if(
    //         (r_demo_get_port_bit(PORT_ID_BANDPLAN_0) == 0u) &&
    //         (r_demo_get_port_bit(PORT_ID_BANDPLAN_1) == 0u))
    //     {
    demo_mode.bandPlan = R_G3_BANDPLAN_CENELEC_A;
    //    }
    //    else if(
    //        (r_demo_get_port_bit(PORT_ID_BANDPLAN_0) == 0u) &&
    //        (r_demo_get_port_bit(PORT_ID_BANDPLAN_1) == 1u))
    //    {
    //        demo_mode.bandPlan = R_G3_BANDPLAN_ARIB;
    //    }
    //    else if(
    //        (r_demo_get_port_bit(PORT_ID_BANDPLAN_0) == 1u) &&
    //        (r_demo_get_port_bit(PORT_ID_BANDPLAN_1) == 0u))
    //    {
    //        demo_mode.bandPlan = R_G3_BANDPLAN_FCC;
    //    }
    //    else
    //    {
    //        demo_mode.bandPlan = R_G3_BANDPLAN_CENELEC_B;
    //    }
    //    if(r_demo_get_port_bit(PORT_ID_DEVICE_TYPE))
    //    {
    // pModeCh->g3mode = R_G3_MODE_EAP;
    //    }
    //    else
    //    {
    // pModeCh->g3mode = R_G3_MODE_ADP;
    //    }
    //
    // #if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    //    if(r_demo_get_port_bit(PORT_ID_HYBRID))
    //    {
    //        pModeCh->g3mode |= R_G3_MODE_BIT_HYBRID;
    //    }
    //    else if (r_demo_get_port_bit(PORT_ID_RF_ONLY))
    //    {
    //        pModeCh->g3mode |= R_G3_MODE_BIT_RF;
    //    }
    //    if(r_demo_get_port_bit(PORT_ID_RF_JPBAND))
    //    {
    //        demo_mode.bandPlanRf.macFrequencyBand_RF = R_RFMAC_FREQ_BAND_920;
    //        demo_mode.bandPlanRf.macOperatingMode_RF = R_RFMAC_FSK_OPEMODE_1;
    //    }
    //    else
    //    {
    //        demo_mode.bandPlanRf.macFrequencyBand_RF = R_RFMAC_FREQ_BAND_863;
    //        demo_mode.bandPlanRf.macOperatingMode_RF = R_RFMAC_FSK_OPEMODE_1;
    //    }
    // #endif
    //
    // #if(R_DEFINE_APP_MODE == R_DEMO_APP_MODE_CERT)
    //    pModeCh->startMode = R_DEMO_MODE_CERT;
    // #else /* R_DEMO_APP_MODE_CERT */
    //    if(r_demo_get_port_bit(PORT_ID_AUTO_MODE))
    //    {
    //        if(r_demo_get_port_bit(PORT_ID_CERT_MODE))
    //        {
    //            pModeCh->startMode = R_DEMO_MODE_CERT;
    //        }
    //        else
    //        {
    // pModeCh->startMode = R_DEMO_MODE_AUTO;
    //        }
    //    }
    //    else
    //    {
    pModeCh->startMode = R_DEMO_MODE_CUI;
    //    }
    //
    //    pModeCh->extIdFlg = R_FALSE;
    // #endif /* else R_DEMO_APP_MODE_CERT */
    //
    // #endif /* ( R_BOARD_DSW_NUM < 6 ) */

#endif /* else R_DEMO_APP_MODE_DEMO */

#if (R_BOARD_DSW_NUM >= 6)
    g_demo_config.bandPlan = (r_g3_bandplan_t)demo_mode.bandPlan;
    g_demo_config.appMode = (r_demo_operation_mode_t)pModeCh->startMode;
    g_demo_config.devType = (R_G3_MODE_EAP == (pModeCh->g3mode & 0xF)) ? R_ADP_DEVICE_TYPE_COORDINATOR : R_ADP_DEVICE_TYPE_PEER;
    g_demo_config.extIdFlg = pModeCh->extIdFlg;
#if !defined(R_DEFINE_DISABLE_APP_HYBRID)
    g_demo_config.mediaType = pModeCh->g3mode >> 4;
    g_demo_config.bandPlanRf = demo_mode.bandPlanRf;
#endif
#else /* ( R_BOARD_DSW_NUM < 6 ) */

#if (R_DEFINE_APP_MODE == R_DEMO_APP_MODE_CERT)
    g_demo_config.appMode = R_DEMO_MODE_CERT;
#endif

#endif

    if (R_DEMO_MODE_CERT == g_demo_config.appMode)
    {
        if (R_G3_BANDPLAN_ARIB == g_demo_config.bandPlan)
        {
            g_demo_config.extIdFlg = R_TRUE;
            g_demo_config.secyrityF2 = R_TRUE;
        }
        else
        {
            g_demo_config.extIdFlg = R_FALSE;
            g_demo_config.secyrityF2 = R_FALSE;
        }
    }

    while (1)
    {
        R_memset((uint8_t *)&g_demo_buff, 0, sizeof(r_demo_buff_t));
        R_memset((uint8_t *)&g_demo_entity, 0, sizeof(r_demo_entity_t));
        uint8_t arr[9] = {143, 217, 254, 123, 157, 14, 82, 21, 202};
        memcpy(demo_mode.tonemask, arr, 9);
        demo_mode.ch[0].g3mode = R_G3_MODE_ADP;
        demo_mode.ch[0].startMode = 0;
        demo_mode.ch[0].extIdFlg = 0;
        demo_mode.ch[1].g3mode = 46;
        demo_mode.ch[1].startMode = 128;
        demo_mode.ch[1].extIdFlg = 239;
        g_demo_config.extIdFlg = 0;
        //        g_demo_config.bandPlan = R_G3_BANDPLAN_CENELEC_A;
        //        g_demo_config.appMode = R_DEMO_MODE_AUTO;
        // g_demo_config.mediaType = R_MEDIA_TYPE0_PLC;
        //        g_demo_config.devType = R_ADP_DEVICE_TYPE_COORDINATOR;
        if (g_demo_config.appMode)
        {
            /* Start demo application directly. */
            R_DEMO_App();
        }
        else
        {
            /* Start demo application main menu. */
            R_DEMO_AppMainMenu();
            g_demo_config.appMode = R_DEMO_MODE_CUI;
        }
    }
}
/******************************************************************************
   End of function  R_DEMO_Main
******************************************************************************/
