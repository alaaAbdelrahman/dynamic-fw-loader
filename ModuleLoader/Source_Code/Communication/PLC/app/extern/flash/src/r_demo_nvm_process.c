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
* File Name   : r_demo_nvm_process.c
*    @version
*        $Rev: 8167 $
*    @last editor
*        $Author: a5089763 $
*    @date  
*        $Date:: 2021-12-29 13:39:59 +0900#$
* Description : 
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/

#include "r_typedefs.h"
#include "r_config.h"
#include "r_bsp_api.h"
#include "r_crc32_api.h"
#include "r_c3sap_api.h"
#include "r_byte_swap.h"
#include "r_demo_app.h"
#include "r_demo_nvm_process.h"
#include "r_flash_lib.h"
#include "r_flash_usage_map.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Private global variables and functions
******************************************************************************/

/*----------------------------------------------------------------------------
  Flash block area table
  
   != 0xFF:Used area(DF block number)
   == 0xFF:Reserved area

    const uint8_t g_nvm_block[(R_NVM_COMMON_CH + 1)][R_FLASH_START_DIV3] = {

           +---R_NVM_ID_DEVICE_CONFIG
           |     +---R_NVM_ID_BACKUP
           |     |     +---R_NVM_ID_SETTING
           |     |     |
        { 0x00, 0x01, 0x02, 0xFF, 0xFF }, <= PRIMARY CH
        { 0x05, 0x06, 0x07, 0xFF, 0xFF }, <= SECONDARY CH
        { 0x0A, 0x0B, 0x0C, 0x0D, 0x0E }  <= COMMON CH
           |     |     |     |      |
           |     |     |     |      +---R_NVM_ID_RANDWAIT
           |     |     |     +---R_NVM_ID_FWMANAGE_BACKUP
           |     |     +---R_NVM_ID_FWMANAGE_UPDATE
           |     +---R_NVM_ID_FWMANAGE_BOOT
           +---R_NVM_ID_BOOT_MODE
    };

-----------------------------------------------------------------------------*/
#if (defined BSP_MCU_RX231) || (defined BSP_MCU_GROUP_RA6M4)
const uint8_t g_nvm_block[(R_NVM_COMMON_CH + 1)][R_FLASH_START_DIV3] = {
    { 0x00, 0x01, 0x02, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
    { 0x03, 0x04, 0x05, 0x06, 0x07 }
};
#elif (defined BSP_MCU_RX631)
const uint8_t g_nvm_block[(R_NVM_COMMON_CH + 1)][R_FLASH_START_DIV3] = {
    { 0x00, 0x01, 0x02, 0xFF, 0xFF },
    { 0x05, 0x06, 0x07, 0xFF, 0xFF },
    { 0x0A, 0x0B, 0x0C, 0x0D, 0x0E }
};

#elif ( (defined BSP_MCU_RX651) || (defined BSP_MCU_RX65N) )
const uint8_t g_nvm_block[(R_NVM_COMMON_CH + 1)][R_FLASH_START_DIV3] = {
    { 0x00, 0x01, 0x02, 0xFF, 0xFF },
    { 0x03, 0x04, 0x05, 0xFF, 0xFF },
    { 0x0A, 0x0B, 0x0C, 0x0D, 0x0E }
};
#elif (defined MCU_RL78G13)
const uint8_t g_nvm_block[(R_NVM_COMMON_CH + 1)][R_FLASH_START_DIV3] = {
    { 0x00, 0x01, 0x02, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
    { 0x03, 0x04, 0x05, 0x06, 0x07 }
};
#else
const uint8_t g_nvm_block[(R_NVM_COMMON_CH + 1)][R_FLASH_START_DIV3] = {
	{ 0x00, 0x01, 0x02, 0xFF, 0xFF },
	{ 0x05, 0x06, 0x07, 0xFF, 0xFF },
	{ 0x0A, 0x0B, 0x0C, 0x0D, 0x0E }
};
#endif /* #if (defined MCU_RX231) */

/******************************************************************************
Exported global variables
******************************************************************************/
/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/
const uint8_t g_rom_nvm_cert_coordaddr[] = {
    0x00, 0x00,
};
const uint8_t g_rom_nvm_cert_gmk[2][16] = {
    {0xAF, 0x4D, 0x6D, 0xCC, 0xF1, 0x4D, 0xE7, 0xC1, 0xC4, 0x23, 0x5E, 0x6F, 0xEF, 0x6C, 0x15, 0x1F},
    {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88}
};

const uint8_t g_rom_nvm_syncword[] = {
    0xBA, 0xC0, 0x45, 0x3F,
};

const uint8_t g_rom_nvm_cert_eui64[] = {
    0xF8, 0x72, 0x6F, 0xBA, 0x00, 0x00, 0x00, 0x01
};
const uint8_t g_rom_nvm_cert_psk[] = {
    0x7F, 0xFE, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t g_rom_nvm_cert_panid[] = {
    0x78, 0x1D,
};
const uint8_t g_rom_nvm_cert_extid_p[] = {
    0x48, 0x45, 0x4D, 0x53
};
const uint8_t g_rom_nvm_cert_extid_s[] = {
    0x53, 0x4D,
};

/******************************************************************************
Functions
******************************************************************************/

/******************************************************************************
* Function Name: r_demo_get_port_bit
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint8_t r_demo_get_port_bit (uint8_t bit)
{
    return ( R_BSP_GetDipSwBit(bit) ^ 0x01);
} /* r_demo_get_port_bit */
/******************************************************************************
   End of function  r_demo_get_port_bit
******************************************************************************/


/******************************************************************************
* Function Name: r_demo_et_gen_dev_config
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_et_gen_dev_config (uint8_t * param, r_cap_dev_cfg_t * pdev, const r_demo_config_t* pdemoconf)
{
    R_memset (pdev, 0, sizeof (r_cap_dev_cfg_t));

    /* EUI64 */
    R_BYTE_UInt64ToArr(pdemoconf->deviceEUI64, pdev->extendedAddress);
    R_memcpy (&pdev->extendedAddress[6], param, 2);

    /* PSK */
    R_memcpy (pdev->psk, pdemoconf->pskKey, sizeof (pdemoconf->pskKey));

    /* COORDADDR */
    R_BYTE_UInt16ToArr(pdemoconf->coordShortAddress, pdev->coordAddr);

    /* PANID */
    pdev->panid[0] = (uint8_t)(param[1] << 4);
    if (param[1] >> 4)
    {
        pdev->panid[0] = (uint8_t)(pdev->panid[0] + 0x4u);
    }
    else
    {
        /**/
    }
    pdev->panid[1] = param[0];

    /* GMK */
    R_memcpy (pdev->gmk[0], pdemoconf->gmk0, sizeof(pdemoconf->gmk0));
    R_memcpy (pdev->gmk[1], pdemoconf->gmk1, sizeof(pdemoconf->gmk1));

    /* EXTID */
    pdev->extID.length = pdemoconf->extId.extIDLength;
    R_memcpy (pdev->extID.id, pdemoconf->extId.extID, pdemoconf->extId.extIDLength);

    return;
} /* r_demo_et_gen_dev_config */
/******************************************************************************
   End of function  r_demo_et_gen_dev_config
******************************************************************************/



/******************************************************************************
* Function Name: r_demo_et_gen_cert_dev_config
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_et_gen_cert_dev_config (r_cap_dev_cfg_t * pdevcfg, uint8_t coord)
{
    uint8_t i;
    R_memset (pdevcfg, 0, sizeof (r_cap_dev_cfg_t));

    /* EUI64 */
    R_memcpy (pdevcfg->extendedAddress, g_rom_nvm_cert_eui64, sizeof (g_rom_nvm_cert_eui64));
    if (coord)
    {
        pdevcfg->extendedAddress[7] = 0;
    }


    /* PSK */
    R_memcpy (pdevcfg->psk, g_rom_nvm_cert_psk, sizeof (g_rom_nvm_cert_psk));

    /* COORDADDR */
    R_memcpy (pdevcfg->coordAddr, g_rom_nvm_cert_coordaddr, sizeof (g_rom_nvm_cert_coordaddr));

    /* PANID */
    R_memcpy (pdevcfg->panid, g_rom_nvm_cert_panid, sizeof (g_rom_nvm_cert_panid));

    /* GMK */
    R_memcpy (pdevcfg->gmk, g_rom_nvm_cert_gmk, sizeof (g_rom_nvm_cert_gmk));

    /* EXTID */
    if (coord)
    {
        pdevcfg->extID.length = APL_EXTID_S_LEN;
        R_memcpy(pdevcfg->extID.id, g_rom_nvm_cert_extid_s, sizeof(g_rom_nvm_cert_extid_s));
        for (i = 0; i < (pdevcfg->extID.length - (sizeof(g_rom_nvm_cert_extid_s))); i++)
        {
            pdevcfg->extID.id[i + (sizeof(g_rom_nvm_cert_extid_s))] = i;
        }
    }
    else
    {
        pdevcfg->extID.length = APL_EXTID_P_LEN;
        R_memcpy(pdevcfg->extID.id, g_rom_nvm_cert_extid_p, sizeof(g_rom_nvm_cert_extid_p));
        for (i = 0; i < (pdevcfg->extID.length - (sizeof(g_rom_nvm_cert_extid_p))); i++)
        {
            pdevcfg->extID.id[i + (sizeof(g_rom_nvm_cert_extid_p))] = i;
        }
    }

    return;
} /* r_demo_et_gen_cert_dev_config */
/******************************************************************************
   End of function  r_demo_et_gen_cert_dev_config
******************************************************************************/

/******************************************************************************
* Function Name: r_demo_et_read_demo_mode
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_et_read_demo_mode (uint8_t * out_ptr)
{
    r_nvm_config_t * pconfig = (r_nvm_config_t *)r_demo_flash_tmp_buff ();

    r_demo_nvm_read (R_NVM_COMMON_CH, R_NVM_ID_BOOT_MODE, sizeof (r_nvm_config_t), (uint8_t *)pconfig);
    R_memcpy (out_ptr, (uint8_t *)&pconfig->demoMode, sizeof (r_demo_mode_t));
    return;
}
/******************************************************************************
   End of function  r_demo_et_read_demo_mode
******************************************************************************/

/******************************************************************************
* Function Name: r_demo_et_read_dev_config
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_et_read_dev_config (uint8_t id, uint8_t * out_ptr)
{
    r_nvm_config_t * pconfig = (r_nvm_config_t *)r_demo_flash_tmp_buff ();

    r_demo_nvm_read (R_NVM_COMMON_CH, R_NVM_ID_BOOT_MODE, sizeof (r_nvm_config_t), (uint8_t *)pconfig);
    R_memcpy (out_ptr, (uint8_t *)&pconfig->devCfg[id], sizeof (r_cap_dev_cfg_t));
    return;
}
/******************************************************************************
   End of function  r_demo_et_read_dev_config
******************************************************************************/

/******************************************************************************
* Function Name: r_demo_et_gen_backup_crc
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint32_t r_demo_et_gen_backup_crc(r_demo_backup_payload_t * pbkup)
{
    r_iovec_t io;
    uint32_t crc32;

    io.length = sizeof(r_demo_backup_payload_t);
    io.paddress = (uint8_t *)pbkup;
    crc32 = R_CRC_CalcCrc32(&io, 0xFFFFFFFFu, 1u);
    return crc32;
}
/******************************************************************************
   End of function  r_demo_et_gen_backup_crc
******************************************************************************/

/******************************************************************************
* Function Name: r_demo_et_read_backup
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t r_demo_et_read_backup(uint8_t id, uint8_t * out_ptr)
{
    r_demo_backup_t           bkup_full;
    r_demo_backup_payload_t * pbkup = (r_demo_backup_payload_t *)out_ptr;

    r_demo_nvm_read (id, R_NVM_ID_BACKUP, sizeof (r_demo_backup_t), (uint8_t *)&bkup_full);

    if (R_BYTE_ArrToUInt32(bkup_full.crc32) == r_demo_et_gen_backup_crc(&bkup_full.bkup))
    {
        R_memcpy((uint8_t *)pbkup, (const uint8_t *)&bkup_full.bkup, sizeof(r_demo_backup_payload_t));
        return R_RESULT_SUCCESS;
    }
    else
    {
        R_memset((uint8_t *)pbkup, 0u, sizeof(r_demo_backup_payload_t));
        return R_RESULT_FAILED;
    }
}
/******************************************************************************
   End of function  r_demo_et_read_backup
******************************************************************************/


/******************************************************************************
* Function Name: r_demo_et_gen_backup
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint16_t r_demo_et_gen_backup (r_demo_backup_payload_t * in_bkup, uint8_t * out_ptr)
{
    r_demo_backup_t * pbkup = (r_demo_backup_t *)out_ptr;

    R_memset ((uint8_t *)pbkup, 0, sizeof (r_demo_backup_t));

    R_memcpy ((uint8_t *)&pbkup->bkup, (const uint8_t *)in_bkup, sizeof (r_demo_backup_payload_t));

    R_BYTE_UInt32ToArr(r_demo_et_gen_backup_crc(&pbkup->bkup), pbkup->crc32);

    return sizeof (r_demo_backup_t);
}
/******************************************************************************
   End of function  r_demo_et_gen_backup
******************************************************************************/

/******************************************************************************
* Function Name: r_demo_nvm_init
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_nvm_init(void)
{
    r_demo_flash_init();
}
/******************************************************************************
   End of function  r_demo_nvm_init
******************************************************************************/

/******************************************************************************
* Function Name: r_demo_nvm_read
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t r_demo_nvm_read (uint8_t id, uint8_t simbol, uint16_t byte_len, uint8_t * out_ptr)
{
    uint32_t  tmp_l = (uint32_t)NULL;
    uint8_t * ptr   = NULL;


    tmp_l = R_FLASH_GET_ADDR (id, simbol);


    r_demo_flash_read (&ptr, tmp_l, byte_len);
    R_memcpy (out_ptr, ptr, byte_len);

    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  r_demo_nvm_read
******************************************************************************/



/******************************************************************************
* Function Name: r_demo_nvm_write
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint8_t r_demo_nvm_write (uint8_t id, uint8_t simbol, uint16_t byte_len, uint8_t * in_ptr)
{
    uint8_t ret;
    uint32_t tmp_l = (uint32_t)NULL;

    tmp_l = R_FLASH_GET_ADDR (id, simbol);
    ret = r_demo_flash_write (tmp_l, in_ptr, byte_len);

    return ret;
}
/******************************************************************************
   End of function  r_demo_nvm_write
******************************************************************************/



/******************************************************************************
* Function Name: r_demo_nvm_erase
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint8_t r_demo_nvm_erase (uint8_t id, uint8_t simbol)
{
    uint8_t ret;
    uint32_t tmp_l = (uint32_t)NULL;

    tmp_l = R_FLASH_GET_ADDR (id, simbol);
    ret = r_demo_flash_erase (tmp_l, R_FLASH_ITEM_SIZE);

    return ret;
}
/******************************************************************************
   End of function  r_demo_nvm_erase
******************************************************************************/


/******************************************************************************
* Function Name: r_demo_nvm_config_init
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_nvm_config_init (const r_boolean_t force_init, const r_demo_config_t* pdemoconf)
{
    r_nvm_config_t * pconfig = (r_nvm_config_t *)r_demo_flash_tmp_buff ();
    uint8_t          addr[2] = {0x01, 0x01};
    r_result_t       err = R_RESULT_FAILED;

    err = r_demo_nvm_read (R_NVM_COMMON_CH, R_NVM_ID_BOOT_MODE, sizeof (r_nvm_config_t), (uint8_t *)pconfig);
    if (R_RESULT_FAILED == err)
    {
        /* reading blank high performance data flash (made of cells with differential storage) does not return 0xFF but 
           random data with a tendency to previously written and erased data. This is why detecting if this type of 
           data flash is initialized by reading magic value SyncWord is not reliable. 
           Instead, r_demo_nvm_read function called above, performs a blank check and aborts before the read operation 
           if there is _any_ blank cell in the read area, thus forcing initialization even if it was not requested */
        pconfig->SyncWord[0] = 0;
    }

    /* Check if already initialized and if initialization is forced. */
    if ((R_memcmp (g_rom_nvm_syncword, pconfig->SyncWord, 4) == 0) &&
        (R_FALSE == force_init))
    {
        /* Already initialized and initialization not forced, return here. */
        return;
    }

    R_memset (&pconfig->demoMode, 0, sizeof (r_demo_mode_t));
    pconfig->demoMode.bandPlan = pdemoconf->bandPlan;
    R_memcpy (pconfig->demoMode.tonemask, pdemoconf->tonemask, 9);

    r_demo_et_gen_dev_config (addr, &pconfig->devCfg[0], pdemoconf);
    addr[1] |= 0x10;
    r_demo_et_gen_dev_config (addr, &pconfig->devCfg[1], pdemoconf);

    R_memcpy (pconfig->SyncWord, g_rom_nvm_syncword, sizeof (g_rom_nvm_syncword));

    r_demo_nvm_write (R_NVM_COMMON_CH, R_NVM_ID_BOOT_MODE, sizeof (r_nvm_config_t), (uint8_t *)pconfig);
}
/******************************************************************************
   End of function  r_demo_nvm_config_init
******************************************************************************/


/******************************************************************************
* Function Name: r_demo_nvm_config_edit
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_nvm_config_edit (uint8_t id, uint8_t * param, const r_demo_config_t* pdemoconf)
{
    r_nvm_config_t * pconfig = (r_nvm_config_t *)r_demo_flash_tmp_buff ();
    r_demo_nvm_read (R_NVM_COMMON_CH, R_NVM_ID_BOOT_MODE, sizeof (r_nvm_config_t), (uint8_t *)pconfig);
    r_demo_et_gen_dev_config (param, &pconfig->devCfg[id], pdemoconf);
    r_demo_nvm_write (R_NVM_COMMON_CH, R_NVM_ID_BOOT_MODE, sizeof (r_nvm_config_t), (uint8_t *)pconfig);
    return;
}
/******************************************************************************
   End of function  r_demo_nvm_config_edit
******************************************************************************/


/******************************************************************************
* Function Name: r_demo_nvm_backup_write
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_nvm_backup_write (uint8_t id, r_demo_backup_payload_t * bkup)
{
    uint8_t * pbuff = r_demo_flash_tmp_buff ();
    uint16_t  length;

    length = r_demo_et_gen_backup (bkup, pbuff);
    r_demo_nvm_write (id, R_NVM_ID_BACKUP, length, pbuff);
}
/******************************************************************************
   End of function  r_demo_nvm_backup_write
******************************************************************************/

