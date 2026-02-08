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

/******************************************************************************
* File Name     : r_fw_download.c
* Device(s)     : RX631
* Tool-chain    : KPIT GNURX-ELF 15.01
* H/W platform  : G-CPX / EU-CPX2 / G-CPX3
* Description   : Sample software
******************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "r_bsp_api.h"
#include "r_app_timer.h"
#include "r_fw_download.h"
#include "r_config.h"
#include "r_c3sap_sys_if.h"
/******************************************************************************
Macro definitions
******************************************************************************/
#define COM_BAUDRATECHG                 (0xA0u)                              /* Baudrate change command */
#define COM_BAUDRATECHG_ADV             (0xA1u)                              /* Advanced Baudrate change command */

#define COM_BAUDRATECHG_RESP_4SEG       (0xAAu)                              /* Baudrate change command response */
#define COM_BAUDRATECHG_RESP_5SEG       (0xABu)                              /* Baudrate change command response */
#define COM_BAUDRATE_1                  (0xC1u)                              /* Baudrate command 1 */
#define COM_BAUDRATE_ACCEPT             (0xCFu)                              /* Baudrate command ACCEPPT */
#define COM_BOOT_END                    (0xB0u)                              /* Boot end command */
#define COM_BOOT_REQUEST                (0x80u)                              /* Request command */

#define MODE_BOOT_IDLE                  (0x00u)                              /* Idle (command wait) */
#define MODE_BOOT_BUSY                  (0x01u)                              /* Transmitting */
#define MODE_BOOT_TIMEOUT               (0x02u)                              /* Transmit timeout */
#define MODE_BOOT_END                   (0x03u)                              /* CPX download mode end */

#define FWDL_INFO_TABLE_CHKSUM_OFFSET   (0x0Cu)                              /* Offset of info table checksum */
#define FWDL_INFO_TABLE_START_OFFSET    (0x10u)                              /* Offset of info table */
/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables
******************************************************************************/
static const FAR uint8_t *pplc_firmware_table = NULL;
static const FAR uint8_t *prf_firmware_table = NULL;
static const FAR uint8_t *pit_pointer;
static uint32_t         it_remain;
static const uint8_t *  pseg_pointer;
static uint32_t         seg_remain;
static volatile uint8_t boot_mode           = MODE_BOOT_IDLE;               /* CPX Boot mode state */
static uint8_t          com_buffer[2];
static uint8_t          dl_baud;
static uint8_t          cmd_baud;
static uint32_t         dl_timeout;
static uint8_t          plcfw_seg_num;
static uint8_t          rffw_seg_num;

/******************************************************************************
Functions
******************************************************************************/
static void fw_dl_cpx_tx_finished (void);
static void fw_dl_rx_byte_handle (const uint8_t rx_byte);
static r_result_t fw_dl_start_fw_download (void);
static r_result_t fw_dl_check_contents (const FAR uint8_t * pfirmware_pointer);
static uint32_t fw_dl_arr2uint32 (const uint8_t parr[]);
static r_result_t fw_dl_check_fw_match (const uint8_t * p_fw_plc, const uint8_t * p_fw_rf);

static void fw_dl_generate_reset (void);
static r_result_t fw_dl_download_monitor (void);

/******************************************************************************
* Function Name:fw_dl_generate_reset
* Description  : Generates CPX reset signal
* Arguments    : None
* Return Value : None
******************************************************************************/
/*!
   \fn          static void fw_dl_generate_reset(void)
   \brief       Generates CPX reset signal
 */
static void fw_dl_generate_reset (void)
{
    R_BSP_Cpx3Reset();
}
/******************************************************************************
   End of function  fw_dl_generate_reset
******************************************************************************/

/******************************************************************************
* Function Name     : fw_dl_download_monitor
* Description       : Monitors CPX firmware download process
* Argument          : None
* Return Value      : R_RESULT_FAILED when timed out/error or
                      R_RESULT_SUCCESS when download finished
******************************************************************************/

/*!
   \fn          static r_result_t fw_dl_download_monitor(void)
   \brief       Monitors CPX firmware download process
   \return      R_RESULT_FAILED when timed out/error or
                R_RESULT_SUCCESS when download finished
 */
static r_result_t fw_dl_download_monitor (void)
{
    /* Check if firmware download has finished successfully or if a timeout occurred after 2s. */
    r_result_t result   = R_RESULT_FAILED;


    /* Start one shot timer with don't care handle */
    R_TIMER_TimerOneShotOn (R_TIMER_ID_FW_DL, dl_timeout, 0x00u);           // R_HANDLE_DONT_CARE
    
    /* Wait for time to elapse or until complete transfer has been flagged.*/
    while ((R_TIMER_TimerOneShotOn (R_TIMER_ID_FW_DL, 1u, 0x00u) != R_RESULT_SUCCESS) && (MODE_BOOT_END != boot_mode))
    {
        R_BSP_NOP();
    }
    
    /* Deactivate the timer */
    R_TIMER_TimerOneShotOff (R_TIMER_ID_FW_DL, 0x00u);


    if (MODE_BOOT_END == boot_mode)
    {
        result = R_RESULT_SUCCESS;
    }

    boot_mode = MODE_BOOT_IDLE;                                             /* Reset boot_mode back to idle */

    return result;
} /* fw_dl_download_monitor */
/******************************************************************************
   End of function  fw_dl_download_monitor
******************************************************************************/

/******************************************************************************
* Function Name     : fw_dl_rx_byte_handle
* Description       : UART Rx byte finished handing function
* Argument          : rx_byte : the byte that has been received
* Return Value      : none
******************************************************************************/
/*!
   \fn          static void fw_dl_rx_byte_handle(const uint8_t rx_byte)
   \brief       UART Rx byte finished handing function
   \param[in]   rx_byte the byte that has been received
 */
static void fw_dl_rx_byte_handle (const uint8_t rx_byte)
{
    const FAR uint8_t   *ptemp;
    uint32_t            offset_adr;

    /* NULL pointer check for stored firmware image. */
    if (NULL == pplc_firmware_table)
    {
        return;
    }

    switch (rx_byte)
    {
        case COM_BAUDRATECHG:
        case COM_BAUDRATE_ACCEPT:
            if (MODE_BOOT_IDLE == boot_mode)
            {
                /* SCI initialize */
                R_BSP_UartSetBaudrate(R_BSP_UART_MCU_CPX, R_SYS_GetDirectBaud(dl_baud));
                /* Transmit baudrate change command response */


                com_buffer[0] = COM_BAUDRATECHG_RESP_4SEG;
                if(NULL != prf_firmware_table)
                {
                    com_buffer[0] = COM_BAUDRATECHG_RESP_5SEG;
                }
                pit_pointer   = com_buffer;
                it_remain     = 1uL;
                seg_remain    = 0uL;

                /* Start transmit */
                R_BSP_UartSend (R_BSP_UART_MCU_CPX, pit_pointer, it_remain);
            }
            break;

        case COM_BAUDRATECHG_ADV:
            if (MODE_BOOT_IDLE == boot_mode)
            {
                com_buffer[0] = COM_BAUDRATE_1;
                com_buffer[1] = (uint8_t)((dl_baud << 4) | cmd_baud);
                pit_pointer   = (const FAR uint8_t *)com_buffer;
                it_remain     = 2uL;
                seg_remain    = 0uL;

                /* Start transmit */
                R_BSP_UartSend (R_BSP_UART_MCU_CPX, pit_pointer, it_remain);
            }
            break;

        case COM_BOOT_END: /* Download finished. Set flag. */
            boot_mode = MODE_BOOT_END;
            break;

        default:
            if (COM_BOOT_REQUEST == (uint8_t)(rx_byte & 0xF0))
            {
                /* Program transfer command. */
                if (MODE_BOOT_IDLE == boot_mode)
                {
                    if ((rx_byte & 0x0F) > (rffw_seg_num + plcfw_seg_num))
                    {
                        /* invalid segment number */
                        return;
                    }

                    boot_mode = MODE_BOOT_BUSY;

                    if((rx_byte & 0x0F) < plcfw_seg_num)
                    {

                        ptemp     = (const FAR uint8_t *)((r_uintptr_t)pplc_firmware_table + (r_uintptr_t)((rx_byte & 0x0F) << 4));
                    }
                    else
                    {
                        ptemp     = (const FAR uint8_t *)((r_uintptr_t)prf_firmware_table);
                    }
                    if (NULL != ptemp)
                    {
                        offset_adr  = fw_dl_arr2uint32(&ptemp[0]);

                        /* Compute info table pointer */
                        pit_pointer  = (const FAR uint8_t *)&ptemp[4];
                        it_remain    = 12u;

                        /* Compute segment program pointer */

                        if((rx_byte & 0x0F) < plcfw_seg_num)
                        {
                            pseg_pointer = (const FAR uint8_t *)((r_uintptr_t)pplc_firmware_table + (r_uintptr_t)offset_adr);

                        }
                        else
                        {
                            pseg_pointer = (const FAR uint8_t *)((r_uintptr_t)prf_firmware_table + (r_uintptr_t)offset_adr);
                        }
                        seg_remain  = fw_dl_arr2uint32(&ptemp[8]);

                        /* Info Table Transmission */
                        R_BSP_UartSend (R_BSP_UART_MCU_CPX, pit_pointer, it_remain);
                    }
                }
                else
                {
                    /* Illegal timing */
                }
            }
            else
            {
                /* Undefined command */
            }
            break;
    } /* switch */
} /* fw_dl_rx_byte_handle */
/******************************************************************************
   End of function  fw_dl_rx_byte_handle
******************************************************************************/

/******************************************************************************
* Function Name     : fw_dl_cpx_tx_finished
* Description       : UART Tx finished handling function
* Argument          : none
* Return Value      : none
******************************************************************************/
/*!
   \fn          static void fw_dl_cpx_tx_finished(void);
   \brief       UART Tx finished handling function
 */
static void fw_dl_cpx_tx_finished (void)
{
    if (0uL != seg_remain)
    {
        /* Segment Program Transmission */
        R_BSP_UartSend (R_BSP_UART_MCU_CPX, pseg_pointer, seg_remain);
        seg_remain    = 0uL;
    }
    else
    {
        /* finished Segment Program Transmission, receive next message from CPX */
        boot_mode = MODE_BOOT_IDLE;
    }
} /* fw_dl_cpx_tx_finished */
/******************************************************************************
   End of function  fw_dl_cpx_tx_finished
******************************************************************************/

/******************************************************************************
* Function Name     : fw_dl_start_fw_download
* Description       : Initiates firmware download to CPX
* Argument          : pfirmware_pointer : uint8_t pointer to firmware array
* Return Value      : R_RESULT_FAILED when timed out/error or
                      R_RESULT_SUCCESS when download finished
******************************************************************************/
/*!
   \fn          static r_result_t fw_dl_start_fw_download(const uint8_t* pfirmware_pointer)
   \brief       Initiates firmware download to CPX
   \param[in]   pfirmware_pointer uint8_t pointer to firmware array
   \return      R_RESULT_FAILED when timed out/error or
                R_RESULT_SUCCESS when download finished
 */

static r_result_t fw_dl_start_fw_download (void)
{

    /* Generate reset */
    fw_dl_generate_reset ();

    /* Call download monitor. */
    return fw_dl_download_monitor ();
} /* fw_dl_start_fw_download */
/******************************************************************************
   End of function  fw_dl_start_fw_download
******************************************************************************/

/******************************************************************************
* Function Name:R_FW_Download
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_FW_Download (r_sys_boot_info_t * boot_info)
{
    uint16_t    cnt = 0u;
    r_result_t  ret_value = R_RESULT_FAILED;
    
    /* NULL pointer check. */
    if ((NULL == boot_info) || (NULL == boot_info->p_fw_plc))
    {
        return R_RESULT_ILLEGAL_NULL_POINTER;
    }
    
    plcfw_seg_num = fw_dl_check_contents (boot_info->p_fw_plc);
    if (0 ==  plcfw_seg_num)
    {
        return R_RESULT_FAILED;
    }

    if(NULL != boot_info->p_fw_rf)
    {
        r_result_t ret;
        
        rffw_seg_num = fw_dl_check_contents (boot_info->p_fw_rf);
        if (0 ==  rffw_seg_num)
        {
            return R_RESULT_FAILED;
        }
        
        ret = fw_dl_check_fw_match(boot_info->p_fw_plc,boot_info->p_fw_rf);
        
        if( R_RESULT_FAILED == ret )
        {
            return R_RESULT_FAILED;
        }
    }

    
    dl_baud    = boot_info->dl_baud;
    cmd_baud   = boot_info->cmd_baud;
    dl_timeout = boot_info->dl_timeout;
    
    /* Start firmware download. Procedure is repeated until download has
       finished successfully (max R_CPX_TX_FW_DL_RETRIES times). */
    while ((R_RESULT_SUCCESS != ret_value) && (R_CPX_TX_FW_DL_RETRIES >= cnt))
    {
        ret_value = R_BSP_ConfigureUart( R_BSP_UART_MCU_CPX, 115200u, fw_dl_cpx_tx_finished, fw_dl_rx_byte_handle);
        if( ret_value != R_RESULT_SUCCESS )
        {
            return R_RESULT_FAILED;
        }
        
        /* Start UART reception to wait for CPX download request */
        pplc_firmware_table = boot_info->p_fw_plc + FWDL_INFO_TABLE_START_OFFSET;
        if(NULL != boot_info->p_fw_rf)
        {
            prf_firmware_table = boot_info->p_fw_rf + FWDL_INFO_TABLE_START_OFFSET;
        }
        ret_value = fw_dl_start_fw_download();

        /* Increse counter */
        cnt++;
    }
    
    return ret_value;
} /* R_FW_Download */
/******************************************************************************
   End of function  R_FW_Download
******************************************************************************/


/******************************************************************************
* Function Name:R_FW_CheckFwMatch
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_FW_CheckFwMatch (const uint8_t * p_fw_plc, const uint8_t * p_fw_rf)
{
    return fw_dl_check_fw_match(p_fw_plc,p_fw_rf);
} /* R_FW_CheckFwMatch */
/******************************************************************************
   End of function  R_FW_CheckFwMatch
******************************************************************************/


/******************************************************************************
* Function Name:R_BYTE_ArrToUInt32
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint32_t fw_dl_arr2uint32 (const uint8_t parr[])
{
    uint32_t val;
    
    if (NULL == parr)
    {
        return 0xFFFFFFFFu;
    }
    
    val = (((uint32_t)(parr[3]) << 24u) + ((uint32_t)(parr[2]) << 16u)) +
          (((uint32_t)(parr[1]) << 8u) + (uint32_t)(parr[0]));
    return val;
}
/******************************************************************************
   End of function  R_BYTE_ArrToUInt32
******************************************************************************/

/******************************************************************************
* Function Name:fw_dl_check_contents
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static r_result_t fw_dl_check_contents (const FAR uint8_t * pfirmware_pointer)
{
    const FAR uint8_t   *ptemp = pfirmware_pointer;
    uint32_t            calc_sum = 0;
    uint32_t            table_sum;
    uint32_t            table_size;
    uint32_t            i;
    uint8_t             seg_num;
    
    /* NULL pointer check. */
    if (NULL == pfirmware_pointer)
    {
        return (r_result_t)0;
    }
    
    table_sum  = fw_dl_arr2uint32(&ptemp[FWDL_INFO_TABLE_CHKSUM_OFFSET]);
    table_size = fw_dl_arr2uint32(&ptemp[FWDL_INFO_TABLE_START_OFFSET]);
    
    if (table_size <= 256)
    {
        for (i = 0; i < table_size; i++)
        {
            calc_sum += ptemp[FWDL_INFO_TABLE_START_OFFSET + i];
        }
        calc_sum = ~calc_sum;
        
        if (table_sum != calc_sum)
        {
            seg_num = 0;
        }
        else
        {
            seg_num = (uint8_t)(table_size >> 4);
        }
    }
    else
    {
        seg_num = 0;
    }
    
    return (r_result_t)seg_num;

} /* fw_dl_check_contents */
/******************************************************************************
   End of function  fw_dl_check_contents
******************************************************************************/

/******************************************************************************
* Function Name:fw_dl_check_fw_match
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static r_result_t fw_dl_check_fw_match (const uint8_t * p_fw_plc, const uint8_t * p_fw_rf)
{
    uint32_t plc_table_crc;
    uint32_t rf_verify_crc;
    uint32_t offset;
    uint32_t size;
    r_result_t ret;
    
    plc_table_crc  = fw_dl_arr2uint32(&p_fw_plc[FWDL_INFO_TABLE_CHKSUM_OFFSET]);
    offset         = fw_dl_arr2uint32(&p_fw_rf[FWDL_INFO_TABLE_START_OFFSET]) + FWDL_INFO_TABLE_START_OFFSET;
    size           = fw_dl_arr2uint32(&p_fw_rf[FWDL_INFO_TABLE_START_OFFSET+8]);
    rf_verify_crc  = fw_dl_arr2uint32(&p_fw_rf[offset+size]);
    
    if( plc_table_crc == rf_verify_crc )
    {
        ret = R_RESULT_SUCCESS;
    }
    else
    {
        ret = R_RESULT_FAILED;
    }
    
    return ret;
    
} /* fw_dl_check_fw_match */
/******************************************************************************
   End of function  fw_dl_check_fw_match
******************************************************************************/



