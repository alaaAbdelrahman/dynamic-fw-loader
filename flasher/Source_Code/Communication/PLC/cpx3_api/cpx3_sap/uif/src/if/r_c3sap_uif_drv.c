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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : r_uif_drv.c
*    @version
*        $Rev: 10962 $
*    @last editor
*        $Author: a5089763 $
*    @date  
*        $Date:: 2022-07-04 15:41:45 +0900#$
* Description : 
******************************************************************************/
/*===========================================================================*/
/* Includes                                                                  */
/*===========================================================================*/
#include "r_typedefs.h"
#include "r_c3sap_config.h"
#include "r_uif_thread_if.h"
#include "r_bsp_api.h"
#include "r_crc32_api.h"
#include "r_c3sap_uif_drv.h"
volatile uint8_t tx1 = 0;
/*===========================================================================*/
/* Macro definitions                                                         */
/*===========================================================================*/
#define R_UIF_HDLC_SCODE               (0x7Eu)

/*===========================================================================*/
/* Typedef definitions                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Function prototype                                                        */
/*===========================================================================*/
static void uif_int_rx_byte_ch0 (uint8_t r_data);
static void uif_int_tx_ch0 (void);

/*===========================================================================*/
/* variables                                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Functions                                                                 */
/*===========================================================================*/

/*****************************************************************************/
/*                           Function for Core                               */
/*****************************************************************************/
/******************************************************************************
* Function Name : R_UIF_DrvUartSend
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
r_sys_status_t R_UIF_DrvUartSend(uint8_t port_id, uint8_t *pdata, uint16_t length)
{
    r_sys_status_t ret = R_SYS_STATUS_SUCCESS;
    
    UNUSED(port_id);
    tx1 = 1;
    if (R_BSP_UartSend (R_BSP_UART_MCU_CPX, pdata, length) != R_RESULT_SUCCESS)
    {
        ret = R_SYS_STATUS_DRV_ERROR;
    }
    tx1 = 0;
    return ret;
}
/******************************************************************************
   End of function  R_UIF_DrvUartSend
******************************************************************************/

/******************************************************************************
* Function Name:R_UIF_DrvCrc32
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_sys_status_t R_UIF_DrvCrc32(uint8_t port_id, uint16_t length, uint8_t *pin, uint32_t initial,  uint8_t *pcrc_res)
{
    r_iovec_t iovec;
    uint32_t  result;
    
    UNUSED (port_id);
    
    if ((NULL == pin) || (NULL == pcrc_res))
    {
        return R_SYS_STATUS_INVALID_PARAMETER;
    }
    
    iovec.paddress = pin;
    iovec.length   = length;
    
    result         = R_CRC_CalcCrc32 (&iovec, initial, 1u);
    
    pcrc_res[0]    = (uint8_t)((result >> 24) & 0xFFu);
    pcrc_res[1]    = (uint8_t)((result >> 16) & 0xFFu);
    pcrc_res[2]    = (uint8_t)((result >> 8) & 0xFFu);
    pcrc_res[3]    = (uint8_t)(result & 0xFFu);
    
    return R_SYS_STATUS_SUCCESS;
}
/******************************************************************************
   End of function  R_UIF_DrvCrc32
******************************************************************************/


/*****************************************************************************/
/*                           Function for If                                 */
/*****************************************************************************/
/******************************************************************************
* Function Name : uif_drvif_uart_init
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
r_sys_status_t uif_drvif_uart_init (uint8_t ch_id, uint32_t baud_rate)
{
    r_sys_status_t ret = R_SYS_STATUS_SUCCESS;
    r_result_t     drv_ret;
    
    /* Configure CPX UART */
    if (R_UIF_PORT_0 == ch_id)
    {
        drv_ret = R_BSP_ConfigureUart (R_BSP_UART_MCU_CPX,
                                       baud_rate,
                                       &uif_int_tx_ch0,
                                       &uif_int_rx_byte_ch0);
    }
    else
    {
        drv_ret = R_RESULT_FAILED;
    }
    
    if (R_RESULT_SUCCESS != drv_ret)
    {
        ret = R_SYS_STATUS_DRV_ERROR;
    }
    
    return ret;
} /* uif_drvif_uart_init */
/******************************************************************************
   End of function  uif_drvif_uart_init
******************************************************************************/

/******************************************************************************
* Function Name:uif_drvif_uart_deinit
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_sys_status_t uif_drvif_uart_deinit (uint8_t ch_id)
{

#if !defined(__RENESAS__)
    UNUSED(ch_id);
#endif
    
    r_sys_status_t ret = R_SYS_STATUS_SUCCESS;
    
    
    return ret;

} /* uif_drvif_uart_deinit */
/******************************************************************************
   End of function  uif_drvif_uart_deinit
******************************************************************************/

/*****************************************************************************/
/*                           Local Function                                  */
/*****************************************************************************/

/******************************************************************************
* Function Name:uif_int_rx_byte_ch0
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
static void uif_int_rx_byte_ch0 (uint8_t r_data)
{
    R_UIF_IntRxByte (R_UIF_PORT_0, r_data);
    
    if (R_UIF_HDLC_SCODE == r_data)
    {
        /* do nothing */
    }
}
/******************************************************************************
   End of function  uif_int_rx_byte_ch0
******************************************************************************/

/******************************************************************************
* Function Name:uif_int_tx_ch0
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
static void uif_int_tx_ch0 (void)
{
    R_UIF_TxComplete (0);
    R_UIF_IntTx (R_UIF_PORT_0);
}
/******************************************************************************
   End of function  uif_int_tx_ch0
******************************************************************************/





