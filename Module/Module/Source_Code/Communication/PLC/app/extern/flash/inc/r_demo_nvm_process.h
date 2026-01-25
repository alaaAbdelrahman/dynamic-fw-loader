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
* File Name   : r_demo_nvm_process.h
*    @version
*        $Rev: 9206 $
*    @last editor
*        $Author: a5089763 $
*    @date  
*        $Date:: 2022-03-17 14:06:30 +0900#$
* Description : 
******************************************************************************/

#ifndef R_DEMO_NVM_PROCESS_H
#define R_DEMO_NVM_PROCESS_H


#include "r_typedefs.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define APL_EXTID_P_LEN  (0x24u)
#define APL_EXTID_S_LEN  (0x22u)
#define PORT_ID_INVALID  (0xFFu)
#define R_NVM_COMMON_CH  (2u)            /*!< indicate common use area on flash */


/******************************************************************************
Typedef enum definitions
******************************************************************************/
typedef enum
{
    /* individually area per ch */
    R_NVM_ID_DEVICE_CONFIG = 0,
    R_NVM_ID_BACKUP,
    R_NVM_ID_SETTING,
} r_nvm_id_t;

typedef enum
{
    /* common area each ch */
    R_NVM_ID_BOOT_MODE = 0,
    R_NVM_ID_FWMANAGE_BOOT,
    R_NVM_ID_FWMANAGE_UPDATE,
    R_NVM_ID_FWMANAGE_BACKUP,
    R_NVM_ID_RANDWAIT,
} r_nvm_id_common_t;

typedef enum
{
    NVM_CFGPART_ALL = 0,
    NVM_CFGPART_EUI64,
    NVM_CFGPART_PSK,
    NVM_CFGPART_COORDADDR,
    NVM_CFGPART_PANID,
    NVM_CFGPART_GMK0,
    NVM_CFGPART_GMK1,
    NVM_CFGPART_EXTID_P,
    NVM_CFGPART_EXTID_S,
    NVM_CFGPART_END,
} r_demo_nvm_dev_cinfig_id_t;

typedef enum
{
    PORT_ID_DEVICE_TYPE             = 0x0,
    PORT_ID_BANDPLAN_0              = 0x1,
    PORT_ID_BANDPLAN_1              = 0x2,
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    PORT_ID_RF_JPBAND               = 0x3,
    PORT_ID_RF_ONLY                 = 0x4,
    PORT_ID_HYBRID                  = 0x5,
#else
    PORT_ID_ROUTE_TYPE              = 0x3,
#endif
    PORT_ID_CERT_MODE               = 0x6,
    PORT_ID_AUTO_MODE               = 0x7,
} r_demo_port_id_t;

/******************************************************************************
Typedef definitions
******************************************************************************/

typedef struct
{
    uint8_t length;
    uint8_t id[36];
} r_demo_extid_t;

typedef struct
{
    uint8_t        extendedAddress[8];
    uint8_t        psk[16];
    uint8_t        coordAddr[2];
    uint8_t        panid[2];
    uint8_t        gmk[2][16];
    r_demo_extid_t extID;
} r_cap_dev_cfg_t;

typedef struct
{
    uint8_t                 FrameCounter[4];
    uint8_t                 LoadSeqNo[2];
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    uint8_t                 FrameCounterRF[4];
#endif
} r_demo_backup_payload_t;


typedef struct
{
    uint8_t                 crc32[4];
    r_demo_backup_payload_t bkup;
    uint32_t                rsv; //nvm write size alignment
} r_demo_backup_t;


typedef struct
{
    uint8_t wait[4];
} r_demo_randwait_t;

typedef struct
{
    uint8_t                 SyncWord[4];
    r_demo_mode_t           demoMode;
    r_cap_dev_cfg_t         devCfg[2];
    uint8_t                 cpxbootarea;
    uint32_t                rsv2; //nvm write size alignment
}r_nvm_config_t;

/******************************************************************************
Functions prototype
******************************************************************************/
uint8_t    r_demo_get_port_bit (uint8_t bit);
void       r_demo_nvm_init (void);
r_result_t r_demo_nvm_read (uint8_t id, uint8_t simbol, uint16_t byte_len, uint8_t * out_ptr);
uint8_t    r_demo_nvm_write (uint8_t id, uint8_t simbol, uint16_t byte_len, uint8_t * in_ptr);
uint8_t    r_demo_nvm_erase (uint8_t id, uint8_t simbol);

void       r_demo_nvm_config_init (const r_boolean_t force_init, const r_demo_config_t* pDemoConf);
void       r_demo_nvm_config_edit (uint8_t id, uint8_t * param, const r_demo_config_t* pDemoConf);
void       r_demo_nvm_backup_write (uint8_t id, r_demo_backup_payload_t * bkup);

void       r_demo_et_gen_dev_config (uint8_t * param, r_cap_dev_cfg_t * pDev, const r_demo_config_t* pDemoConf);
void       r_demo_et_gen_cert_dev_config (r_cap_dev_cfg_t * pDevCfg, uint8_t coord);
void       r_demo_et_read_demo_mode(uint8_t * out_ptr);
void       r_demo_et_read_dev_config (uint8_t id, uint8_t * out_ptr);
uint32_t   r_demo_et_gen_backup_crc(r_demo_backup_payload_t * pBkup);
r_result_t r_demo_et_read_backup(uint8_t id, uint8_t * out_ptr);
uint16_t   r_demo_et_gen_backup (r_demo_backup_payload_t * in_bkup, uint8_t * out_ptr);


#endif /* R_DEMO_NVM_PROCESS_H */
