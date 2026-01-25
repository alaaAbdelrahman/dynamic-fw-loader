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
*        $Rev: 9177 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2022-03-16 15:27:29 +0900#$
* Description :
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include "r_typedefs.h"
#include "r_stdio_api.h"
#include "r_byte_swap.h"

/* g3 part */
#include "r_c3sap_api.h"
#include "r_g3_param2text.h"


#include "r_demo_app.h"
#include "r_demo_nvm_process.h"
#include "r_demo_print.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#ifdef __PLC__DEBUG__
#define APL_PRINTF(x, ...)  (R_STDIO_Printf (__VA_ARGS__))
#endif
/******************************************************************************
Typedef definitions
******************************************************************************/
typedef uint32_t (* FP_CONV_IBID2TXT)(uint16_t id, const uint8_t ** txt, uint8_t * len);

/******************************************************************************
Private global variables and functions
******************************************************************************/
/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/
/******************************************************************************
Functions
******************************************************************************/



/******************************************************************************
* Function Name: r_demo_print_version
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_print_version ( void )
{
#ifdef __PLC__DEBUG__
    APL_PRINTF (0xF, "\n\n==========================================================\r\n\r\n");
#endif

#ifdef __PLC__DEBUG__
    APL_PRINTF (0xF, "           RENESAS G3PLC DEMO APPLICATION         \r\n\r\n");
#endif

#ifdef __PLC__DEBUG__
    APL_PRINTF (0xF, "==========================================================\n");
#endif
#ifdef __PLC__DEBUG__
    APL_PRINTF (0xF, "                      (build date: "__DATE__ " "__TIME__ ")\r\n");
#endif
}
/******************************************************************************
   End of function  r_demo_print_version
******************************************************************************/


/******************************************************************************
* Function Name: r_demo_print_cert_version
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_print_cert_version (uint8_t hwVer, uint16_t swVer)
{
#ifdef __PLC__DEBUG__
    APL_PRINTF (0xF, "\n\n==========================================================\r\n\r\n");
#endif

#ifdef __PLC__DEBUG__
    APL_PRINTF (0xF, "        RENESAS G3PLC CERTIFICATION APP (ver %01d.%02d.%02d)         \r\n\r\n", hwVer, swVer >> 8, swVer & 0xFF);
#endif

#ifdef __PLC__DEBUG__
    APL_PRINTF (0xF, "==========================================================\n");
#endif
#ifdef __PLC__DEBUG__
    APL_PRINTF (0xF, "                      (build date: "__DATE__ " "__TIME__ ")\r\n");
#endif
}
/******************************************************************************
   End of function  r_demo_print_cert_version
******************************************************************************/

/******************************************************************************
* Function Name: r_demo_print_devinfo_eui64
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void r_demo_print_devinfo_eui64 (uint8_t dev_id, uint8_t * eui64)
{
    uint8_t * pC;

    UNUSED (dev_id);

    pC = eui64;
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "  EUI64               :");
#endif
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "0x%02X%02X_%02X%02X_%02X%02X_%02X%02X\r\n",
                pC[0], pC[1], pC[2], pC[3], pC[4], pC[5], pC[6], pC[7]);
#endif
}
/******************************************************************************
   End of function  r_demo_print_devinfo_eui64
******************************************************************************/

/******************************************************************************
* Function Name: r_demo_print_devinfo_psk
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void r_demo_print_devinfo_psk (uint8_t dev_id, uint8_t * psk)
{
    uint8_t * pC;

    UNUSED (dev_id);

    pC = psk;
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "  PSK                 :");
#endif
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "0x%02X%02X_%02X%02X_%02X%02X_%02X%02X_%02X%02X_%02X%02X_%02X%02X_%02X%02X\r\n",
                pC[0], pC[1], pC[2], pC[3], pC[4], pC[5], pC[6], pC[7],
                pC[8], pC[9], pC[10], pC[11], pC[12], pC[13], pC[14], pC[15]);
#endif
}
/******************************************************************************
   End of function  r_demo_print_devinfo_psk
******************************************************************************/


/******************************************************************************
* Function Name: r_demo_print_devinfo_coordshort
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void r_demo_print_devinfo_coordshort (uint8_t dev_id, uint8_t * coordAddr)
{
    uint8_t * pC;

    UNUSED (dev_id);

    pC = coordAddr;
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "  CoordAddr           :");
#endif
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "0x%02X%02X\r\n\r\n", pC[0], pC[1]);
#endif
}
/******************************************************************************
   End of function  r_demo_print_devinfo_coordshort
******************************************************************************/




/******************************************************************************
* Function Name: r_demo_print_devinfo_panid
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void r_demo_print_devinfo_panid (uint8_t dev_id, uint8_t * panid)
{
    uint8_t * pC;

    UNUSED (dev_id);

    pC = panid;
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "  Start PanID         :");
#endif
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "0x%02X%02X\r\n", pC[0], pC[1]);
#endif
}
/******************************************************************************
   End of function  r_demo_print_devinfo_panid
******************************************************************************/



/******************************************************************************
* Function Name: r_demo_print_gmk_1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void r_demo_print_gmk_1 (uint8_t dev_id, uint8_t * gmk, uint8_t index)
{
    uint8_t * pC;

    UNUSED (dev_id);

    pC = gmk;
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "  GMK[index%d]         :", index);
#endif
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "0x%02X%02X_%02X%02X_%02X%02X_%02X%02X_%02X%02X_%02X%02X_%02X%02X_%02X%02X\r\n",
                pC[0], pC[1], pC[2], pC[3], pC[4], pC[5], pC[6], pC[7],
                pC[8], pC[9], pC[10], pC[11], pC[12], pC[13], pC[14], pC[15]);
#endif
}
/******************************************************************************
   End of function  r_demo_print_gmk_1
******************************************************************************/



/******************************************************************************
* Function Name: r_demo_print_devinfo_extid_1
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void r_demo_print_devinfo_extid_1 (uint8_t dev_id, r_demo_extid_t * extid, uint8_t index)
{
    uint8_t * pC;
    uint8_t   i;

    UNUSED (dev_id);

    pC = extid->id;

#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "  own EXT ID(%02dByte)      :0x", extid->length);
#endif

    for (i = 0; i < extid->length; i++)
    {
        if (((i & 0xF) == 0) && (0 != i))
        {
#ifdef __PLC__DEBUG__
            APL_PRINTF (dev_id, "\r\n                      ");
#endif
        }
        if (((i & 0x1) == 0) && (0 != i))
        {
#ifdef __PLC__DEBUG__
            APL_PRINTF (dev_id, "_");
#endif
        }
#ifdef __PLC__DEBUG__
        APL_PRINTF (dev_id, "%02X", pC[i]);
#endif
    }
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "\r\n");
#endif

} /* r_demo_print_devinfo_extid_1 */
/******************************************************************************
   End of function  r_demo_print_devinfo_extid_1
******************************************************************************/


/******************************************************************************
* Function Name: r_demo_print_gmk
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void r_demo_print_gmk (uint8_t dev_id, r_cap_dev_cfg_t * config)
{
    r_demo_print_gmk_1 (dev_id, config->gmk[0], 0);
    r_demo_print_gmk_1 (dev_id, config->gmk[1], 1);
}
/******************************************************************************
   End of function  r_demo_print_gmk
******************************************************************************/



/******************************************************************************
* Function Name: r_demo_print_devinfo_extid
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void r_demo_print_devinfo_extid (uint8_t dev_id, r_cap_dev_cfg_t * config)
{
    r_demo_print_devinfo_extid_1(dev_id, &config->extID, 0);
}
/******************************************************************************
   End of function  r_demo_print_devinfo_extid
******************************************************************************/


/******************************************************************************
* Function Name: r_demo_print_devinfo
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_print_devinfo (uint8_t dev_id, r_cap_dev_cfg_t * config)
{
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "  ----- Device Config -----\r\n\r\n");
#endif

    r_demo_print_devinfo_eui64 (dev_id, config->extendedAddress);

    r_demo_print_devinfo_psk (dev_id, config->psk);

    r_demo_print_devinfo_coordshort (dev_id, config->coordAddr);

}
/******************************************************************************
   End of function  r_demo_print_devinfo
******************************************************************************/


/******************************************************************************
* Function Name: r_demo_print_bakup
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_print_bakup (uint8_t dev_id, uint8_t * param)
{
    r_demo_backup_payload_t * pBkup = (r_demo_backup_payload_t *)param;

    UNUSED (dev_id);

#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "  ----- Preserved Info -----\r\n\r\n");
#endif
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "  Prev FrameCounter   :0x%08X\r\n", R_BYTE_ArrToUInt32(pBkup->FrameCounter));
#endif
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "  Prev FrameCounterRF :0x%08X(%d)\r\n", R_BYTE_ArrToUInt32(pBkup->FrameCounterRF), R_BYTE_ArrToUInt32(pBkup->FrameCounterRF));
#endif
#endif
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "  Prev LoadSeqNo      :0x%04X\r\n", R_BYTE_ArrToUInt16(pBkup->LoadSeqNo));
#endif
}
/******************************************************************************
   End of function  r_demo_print_bakup
******************************************************************************/

/******************************************************************************
* Function Name: r_demo_print_bandplan
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_print_bandplan (r_g3_bandplan_t bandPlan)
{
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "\r\n----------------------------------PLC BandPlan: ");
#endif

    if (R_G3_BANDPLAN_CENELEC_A == bandPlan)
    {
#ifdef __PLC__DEBUG__
        APL_PRINTF (dev_id, "CENELEC-A");
#endif
    }
    else if (R_G3_BANDPLAN_CENELEC_B == bandPlan)
    {
#ifdef __PLC__DEBUG__
        APL_PRINTF (dev_id, "CENELEC-B");
#endif
    }
    else if (R_G3_BANDPLAN_ARIB == bandPlan)
    {
#ifdef __PLC__DEBUG__
        APL_PRINTF (dev_id, "ARIB");
#endif
    }
    else if (R_G3_BANDPLAN_FCC == bandPlan)
    {
#ifdef __PLC__DEBUG__
        APL_PRINTF (dev_id, "FCC");
#endif
    }
    else
    {
        /**/
    }
} /* r_demo_print_bandplan */
/******************************************************************************
   End of function  r_demo_print_bandplan
******************************************************************************/

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
/******************************************************************************
* Function Name: r_demo_print_mediatype
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_print_mediatype (uint8_t mediaType)
{
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "\r\n------------------------------------- ");
#endif
    if (R_MEDIA_TYPE0_PLC == mediaType)
    {
#ifdef __PLC__DEBUG__
        APL_PRINTF (dev_id, "PLC Only");
#endif
    }
    else if (R_MEDIA_TYPE1_RF == mediaType)
    {
#ifdef __PLC__DEBUG__
        APL_PRINTF (dev_id, "RF Only");
#endif
    }
    else
    {
#ifdef __PLC__DEBUG__
        APL_PRINTF (dev_id, "Hybrid");
#endif
    }
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, " Mode ---\r\n");
#endif
} /* r_demo_print_mediatype */
/******************************************************************************
   End of function  r_demo_print_mediatype
******************************************************************************/
#endif

/******************************************************************************
* Function Name: r_demo_print_config
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_print_config (uint8_t dev_id, r_demo_config_t * demo, r_cap_dev_cfg_t * config, r_demo_backup_payload_t * bkup)
{
    uint8_t * toneMask = demo->tonemask;
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "\r\n==========================================================\r\n");
#endif
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    r_demo_print_mediatype(demo->mediaType);

    if (R_MEDIA_TYPE1_RF != demo->mediaType)
    {
#endif

        if (R_G3_BANDPLAN_CENELEC_A == demo->bandPlan)
        {
#ifdef __PLC__DEBUG__
            APL_PRINTF(dev_id, "  PLC BandPlan        :CENELEC-A\r\n");
#endif
#ifdef __PLC__DEBUG__
            APL_PRINTF(dev_id, "  toneMask            :0x000000000%X%02X%02X%02X%02X\r\n", toneMask[4] & 0xF, toneMask[5], toneMask[6], toneMask[7], toneMask[8]);
#endif
        }
        else if (R_G3_BANDPLAN_CENELEC_B == demo->bandPlan)
        {
#ifdef __PLC__DEBUG__
            APL_PRINTF(dev_id, "  PLC BandPlan        :CENELEC-B\r\n");
#endif
#ifdef __PLC__DEBUG__
            APL_PRINTF(dev_id, "  toneMask            :0x00000000000000%02X%02X\r\n", toneMask[7], toneMask[8]);
#endif
        }
        else if (R_G3_BANDPLAN_ARIB == demo->bandPlan)
        {
#ifdef __PLC__DEBUG__
            APL_PRINTF(dev_id, "  PLC BandPlan        :ARIB\r\n");
#endif
#ifdef __PLC__DEBUG__
            APL_PRINTF(dev_id, "  toneMask            :0x0000%02X%02X%02X%02X%02X%02X%02X\r\n", (toneMask[2] & 0x3), toneMask[3], toneMask[4], toneMask[5], toneMask[6], toneMask[7], toneMask[8]);
#endif
        }
        else if (R_G3_BANDPLAN_FCC == demo->bandPlan)
        {
#ifdef __PLC__DEBUG__
            APL_PRINTF(dev_id, "  PLC BandPlan        :FCC\r\n");
#endif
#ifdef __PLC__DEBUG__
            APL_PRINTF(dev_id, "  toneMask            :0x%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n", toneMask[0], toneMask[1], toneMask[2], toneMask[3], toneMask[4], toneMask[5], toneMask[6], toneMask[7], toneMask[8]);
#endif
        }
        else
        {
            /**/
        }

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    }
    if (R_MEDIA_TYPE0_PLC != demo->mediaType)
    {
#ifdef __PLC__DEBUG__
        APL_PRINTF(dev_id, "  RF FreqBand         :%d\r\n", demo->bandPlanRf.macFrequencyBand_RF);
#endif
#ifdef __PLC__DEBUG__
        APL_PRINTF(dev_id, "  RF OpeMode          :%d\r\n", demo->bandPlanRf.macOperatingMode_RF);
#endif
    }
#endif

    if (R_ADP_DEVICE_TYPE_COORDINATOR == demo->devType)
    {
        if (R_FALSE == demo->extIdFlg)
        {
#ifdef __PLC__DEBUG__
            APL_PRINTF (dev_id, "  Device Type         :Coordinator\r\n\r\n");
#endif
            r_demo_print_devinfo (dev_id, config);
            r_demo_print_gmk (dev_id, config);
            r_demo_print_devinfo_panid (dev_id, config->panid);
        }
        else
        {
#ifdef __PLC__DEBUG__
            APL_PRINTF (config->bootCfg.id, "  Device Type         :Coordinator\r\n\r\n");
#endif
            r_demo_print_devinfo (dev_id, config);
            r_demo_print_devinfo_extid (dev_id, config);
            r_demo_print_gmk (dev_id, config);
            r_demo_print_devinfo_panid (dev_id, config->panid);
        }
#ifdef __PLC__DEBUG__
        APL_PRINTF (dev_id, "  activeKeyIndex      :");
#endif
#ifdef __PLC__DEBUG__
        APL_PRINTF (dev_id, "0x%02X\r\n\r\n", demo->activeKeyIndex);
#endif
    }
    else
    {
        if (R_FALSE == demo->extIdFlg)
        {
#ifdef __PLC__DEBUG__
            APL_PRINTF (config->bootCfg.id, "  Device Type         :Peer\r\n\r\n");
#endif
            r_demo_print_devinfo (dev_id, config);
        }
        else
        {
#ifdef __PLC__DEBUG__
            APL_PRINTF (config->bootCfg.id, "  Device Type         :Peer\r\n\r\n");
#endif
            r_demo_print_devinfo (dev_id, config);
            r_demo_print_devinfo_extid (dev_id, config);
        }
    }

    if (NULL != bkup)
    {
        r_demo_print_bakup (dev_id, (uint8_t *)bkup);
    }

#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "\r\n==========================================================\r\n\r\n");
#endif

} /* r_demo_print_config */
/******************************************************************************
   End of function  r_demo_print_config
******************************************************************************/


/******************************************************************************
* Function Name: r_demo_print_frame_conter
* Description :
* Arguments :
* Return Value :
******************************************************************************/
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
void r_demo_print_frame_conter (uint8_t id, uint32_t FrameCounter, uint8_t mediaType)
{
    UNUSED (id);
#ifdef __PLC__DEBUG__
    APL_PRINTF (id, "\r\n#### [ Stored FrameCounter = 0x%08X(%d) mediaType:%d ] ####\r\n", FrameCounter, FrameCounter, mediaType);
#endif
}
#else
void r_demo_print_frame_conter (uint8_t id, uint32_t FrameCounter)
{
    UNUSED (id);
#ifdef __PLC__DEBUG__
    APL_PRINTF (id, "\r\n#### [ Stored FrameCounter = 0x%08X ] ####\r\n", FrameCounter);
#endif
}
#endif
/******************************************************************************
   End of function  r_demo_print_frame_conter
******************************************************************************/



/******************************************************************************
* Function Name: r_demo_print_devinfo_all
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_print_devinfo_all (uint8_t dev_id, r_cap_dev_cfg_t * config)
{
#ifdef __PLC__DEBUG__
    APL_PRINTF (dev_id, "  ----- Device Config -----\r\n\r\n");
#endif

    r_demo_print_devinfo_eui64 (dev_id, config->extendedAddress);

    r_demo_print_devinfo_psk (dev_id, config->psk);

    r_demo_print_devinfo_coordshort (dev_id, config->coordAddr);

    r_demo_print_devinfo_panid (dev_id, config->panid);

    r_demo_print_gmk_1 (dev_id, config->gmk[0], 0);
    r_demo_print_gmk_1 (dev_id, config->gmk[1], 1);

    r_demo_print_devinfo_extid_1 (dev_id, &config->extID, 0);

} /* r_demo_print_devinfo_all */
/******************************************************************************
   End of function  r_demo_print_devinfo_all
******************************************************************************/


/******************************************************************************
* Function Name: r_demo_print_devinfo_sel
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_print_devinfo_sel (uint8_t dev_id, uint8_t part, uint8_t * param)
{

    switch (part)
    {
        case NVM_CFGPART_ALL:
            r_demo_print_devinfo_all (dev_id, (r_cap_dev_cfg_t *)param);
            break;

        case NVM_CFGPART_EUI64:
            r_demo_print_devinfo_eui64 (dev_id, param);
            break;

        case NVM_CFGPART_PSK:
            r_demo_print_devinfo_psk (dev_id, param);
            break;

        case NVM_CFGPART_COORDADDR:
            r_demo_print_devinfo_coordshort (dev_id, param);
            break;

        case NVM_CFGPART_PANID:
            r_demo_print_devinfo_panid (dev_id, param);
            break;

        case NVM_CFGPART_GMK0:
            r_demo_print_gmk_1 (dev_id, param, 0);
            break;

        case NVM_CFGPART_GMK1:
            r_demo_print_gmk_1 (dev_id, param, 1);
            break;

        case NVM_CFGPART_EXTID_P:
            r_demo_print_devinfo_extid_1 (dev_id, (r_demo_extid_t *)param, 0);
            break;

        case NVM_CFGPART_EXTID_S:
            r_demo_print_devinfo_extid_1 (dev_id, (r_demo_extid_t *)param, 1);
            break;

        default:
            break;
    } /* switch */

    return;
} /* r_demo_print_devinfo_sel */
/******************************************************************************
   End of function  r_demo_print_devinfo_sel
******************************************************************************/




/******************************************************************************
* Function Name: r_demo_disp_ib_info
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_disp_ib_info (uint8_t Layer)
{
    uint16_t     end_cnt = 0;

    uint16_t     i;
    const char * pTxt;
    uint8_t      len     = 0;


    switch (Layer)
    {
        case R_G3_LAYER_UMAC:
            end_cnt = 0x122;
            break;
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
        case R_G3_LAYER_RFMAC:
            end_cnt = 0xFF;
            break;
#endif
        case R_G3_LAYER_ADP:
            end_cnt = 0xFF;
            break;

        case R_G3_LAYER_EAP:
            end_cnt = 0xFF;
            break;

        default:
            break;
    }

#ifdef __PLC__DEBUG__
    APL_PRINTF (0xF, "\r\n==========================================================\r\n\r\n");
#endif

#ifdef __PLC__DEBUG__
    APL_PRINTF (0xF, "   ID: (length) names\r\n");
#endif

    if (R_G3_LAYER_UMAC == Layer)
    {
        for (i = 0; i < end_cnt; i++)
        {
            pTxt = R_G3_Ibid2Text (Layer, i, &len);
            if (0xFF == len)
            {
                continue;
            }
            if (len)
            {
#ifdef __PLC__DEBUG__
                APL_PRINTF (0xF, "  0x%04X: (%dB) %s\r\n", i, len, pTxt);
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                APL_PRINTF (0xF, "  0x%04X: <T>   %s\r\n", i, pTxt);
#endif
            }
        }

        for (i = 0x800; i < 0x855; i++)
        {
            pTxt = R_G3_Ibid2Text (Layer, i, &len);
            if (0xFF == len)
            {
                continue;
            }
            if (len)
            {
#ifdef __PLC__DEBUG__
                APL_PRINTF (0xF, "  0x%04X: (%dB) %s\r\n", i, len, pTxt);
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                APL_PRINTF (0xF, "  0x%04X: <T>   %s\r\n", i, pTxt);
#endif
            }
        }
        for (i = 0x8300; i < 0x8355; i++)
        {
            pTxt = R_G3_Ibid2Text (Layer, i, &len);
            if (0xFF == len)
            {
                continue;
            }
            if (len)
            {
#ifdef __PLC__DEBUG__
                APL_PRINTF (0xF, "  0x%04X: (%dB) %s\r\n", i, len, pTxt);
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                APL_PRINTF (0xF, "  0x%04X: <T>   %s\r\n", i, pTxt);
#endif
            }
        }

    }
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    else if (R_G3_LAYER_RFMAC == Layer)
    {
        for (i = 0; i < 0x01FF; i++)
        {
            pTxt = R_G3_Ibid2Text (Layer, i, &len);
            if (0xFF == len)
            {
                continue;
            }
            if (len)
            {
#ifdef __PLC__DEBUG__
                APL_PRINTF (0xF, "  0x%04X: (%dB) %s\r\n", i, len, pTxt);
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                APL_PRINTF (0xF, "  0x%04X: <T>   %s\r\n", i, pTxt);
#endif
            }
        }

        for (i = 0x0200; i < 0x0280; i++)
        {
            pTxt = R_G3_Ibid2Text (Layer, i, &len);
            if (0xFF == len)
            {
                continue;
            }
            if (len)
            {
#ifdef __PLC__DEBUG__
                APL_PRINTF (0xF, "  0x%04X: (%dB) %s\r\n", i, len, pTxt);
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                APL_PRINTF (0xF, "  0x%04X: <T>   %s\r\n", i, pTxt);
#endif
            }
        }
        for (i = 0x800; i < 0x820; i++)
        {
            pTxt = R_G3_Ibid2Text (Layer, i, &len);
            if (0xFF == len)
            {
                continue;
            }
            if (len)
            {
#ifdef __PLC__DEBUG__
                APL_PRINTF (0xF, "  0x%04X: (%dB) %s\r\n", i, len, pTxt);
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                APL_PRINTF (0xF, "  0x%04X: <T>   %s\r\n", i, pTxt);
#endif
            }
        }
    }
#endif
    else
    {
        for (i = 0; i < end_cnt; i++)
        {
            pTxt = R_G3_Ibid2Text (Layer, i, &len);
            if (0xFF == len)
            {
                continue;
            }
            if (len)
            {
#ifdef __PLC__DEBUG__
                APL_PRINTF (0xF, "  0x%02X: (%dB) %s\r\n", i, len, pTxt);
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                APL_PRINTF (0xF, "  0x%02X: <T>   %s\r\n", i, pTxt);
#endif
            }
        }

    }
} /* r_demo_disp_ib_info */
/******************************************************************************
   End of function  r_demo_disp_ib_info
******************************************************************************/

