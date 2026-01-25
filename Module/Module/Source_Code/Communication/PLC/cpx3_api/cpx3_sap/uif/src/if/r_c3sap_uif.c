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
 * File Name : uif_sap.c
 * Description : UART Interface layer API
 ******************************************************************************/
/*===========================================================================*/
/* Includes                                                                  */
/*===========================================================================*/
#include "r_typedefs.h"
#include "r_c3sap_config.h"
#include "r_c3sap_sys_if.h"
#include "r_c3sap_uif_drv.h"
#include "r_uif_sap_if.h"
#include "r_uif_thread_if.h"

/*===========================================================================*/
/* Macro definitions                                                         */
/*===========================================================================*/
#define R_UIF_TX_LINEAR_BUFF_SIZE       ( R_UIF_COMMAND_LEN_MAX*2 )
#define R_UIF_TX_RING_BUFF_SIZE         ( 0u )                                                      /* 0Kbyte (No ring buffer is used) */

#define R_UIF_RX_LINEAR_BUFF_SIZE       ( R_UIF_COMMAND_LEN_MAX )
#define R_UIF_RX_MIN_RING_BUFF_SIZE     ( 1024u )                                                   /* 1Kbyte (For 10ms at 1Mbps baud rate)*/
#define R_UIF_RX_CALC_RING_BUFF_SIZE    ( R_UIF_COMMAND_LEN_MAX*2 + R_UIF_COMMAND_LEN_MAX*2/3 )     /* R_UIF_COMMAND_LEN_MAX*1.33 */
#define R_UIF_RX_RING_BUFF_SIZE         ( (R_UIF_RX_MIN_RING_BUFF_SIZE>R_UIF_RX_CALC_RING_BUFF_SIZE)?R_UIF_RX_MIN_RING_BUFF_SIZE:R_UIF_RX_CALC_RING_BUFF_SIZE ) /* MAX(RX_MIN_RING_BUFF_SIZE,RX_CALC_RING_BUFF_SIZE)*/

#define R_UIF_CORE_MEM_SIZE             ( R_UIF_RX_LINEAR_BUFF_SIZE + R_UIF_TX_LINEAR_BUFF_SIZE + R_UIF_TX_RING_BUFF_SIZE + R_UIF_RX_RING_BUFF_SIZE + 500u )
#define R_UIF_DBGLOG_SIZE               (0x200)     /* 512byte */

/*===========================================================================*/
/* Typedef definitions                                                       */
/*===========================================================================*/
typedef enum
{
    R_UIF_LOG_ID_REQ,
    R_UIF_LOG_ID_REQ_ERROR,
    R_UIF_LOG_ID_RCV,
    R_UIF_LOG_ID_RCV_RES,
}r_uif_log_id_t;



typedef struct
{
    uint8_t     block;
    uint8_t     *pbuffer;
    uint32_t    size;
}r_uif_rcv_info_t;


typedef struct
{
    uint8_t             core_use_memory[R_UIF_CORE_MEM_SIZE];
    r_uif_rcv_info_t    rcv_info;
    r_uif_state_t       tx_state;
    r_uif_state_t       rx_state;
}r_uif_sap_info_t;

/*===========================================================================*/
/* Typedef definitions                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* variables                                                                 */
/*===========================================================================*/
static const uint32_t uif_baudrate[] = {
    9600
    , 19200
    , 38400
    , 57600
    , 115200
    , 230400
    , 300000
    , 375000
    , 460800
    , 500000
    , 750000
    , 937500
    , 1000000
};

/* OS resource */
static r_uif_sap_info_t         uif_info[R_MAX_UIF_PORT_NUM] = {0};

/*===========================================================================*/
/* Function prototype                                                        */
/*===========================================================================*/
static uint8_t uif_get_port_id(uint8_t block_id);
static uint8_t uif_get_block_id(uint8_t port_id);
/*===========================================================================*/
/* Functions                                                                 */
/*===========================================================================*/

/*****************************************************************************/
/*                           Function for System                             */
/*****************************************************************************/

/******************************************************************************
* Function Name:R_UIF_Init
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_UIF_Init( uint8_t block_id, r_uif_baud_t baud )
{
    uint8_t                 ret;
    r_uif_memsetup_info_t   mem_info;
    uint8_t                 port_id = uif_get_port_id(block_id);
    r_uif_sap_info_t        *psap_info = &uif_info[port_id];
    void                    *pend;
    
    mem_info.size_info.tx_buff_size_byte    = R_UIF_TX_RING_BUFF_SIZE;
    mem_info.size_info.rx_buff_size_byte    = R_UIF_RX_RING_BUFF_SIZE;
    mem_info.size_info.baud_rate            = uif_baudrate[baud];
    mem_info.size_info.command_len_max      = R_UIF_COMMAND_LEN_MAX;
    mem_info.pstart                         = psap_info->core_use_memory;
    mem_info.alloc_byte_size                = R_UIF_CORE_MEM_SIZE;
    
    ret = (uint8_t)R_UIF_InitCore( port_id, &mem_info, &pend );
    if ( 0u != ret )
    {
        return R_RESULT_FAILED;
    }
    
    psap_info->tx_state  = R_UIF_STATE_IDLE;
    psap_info->rx_state  = R_UIF_STATE_IDLE;
    
    uif_drvif_uart_init( port_id, uif_baudrate[baud] );
    
    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_UIF_Init
******************************************************************************/

/******************************************************************************
* Function Name:R_UIF_Deinit
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_UIF_Deinit( uint8_t block_id )
{
    uint8_t                 ret;
    uint8_t                 port_id = uif_get_port_id(block_id);
    r_uif_sap_info_t        *psap_info = &uif_info[port_id];
    
    ret = (uint8_t)R_UIF_DeInitCore( port_id );
    if ( 0u != ret )
    {
        return R_RESULT_FAILED;
    }

    
    psap_info->tx_state  = R_UIF_STATE_UNINIT;
    psap_info->rx_state  = R_UIF_STATE_UNINIT;
    
    uif_drvif_uart_deinit( port_id );
    
    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_UIF_Deinit
******************************************************************************/

/******************************************************************************
* Function Name:R_UIF_SendCmd
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_UIF_SendCmd( uint8_t dst_block_id, uint8_t src_block_id, uint8_t *pbuffer, uint32_t size, uint8_t reserved )
{
    r_result_t       status = R_RESULT_SUCCESS;
    r_sys_status_t   ret;
    uint8_t          port_id = uif_get_port_id(dst_block_id);
    r_uif_sap_info_t *psap_info = &uif_info[port_id];
    
    
    ret = R_UIF_TxReqMain( port_id, src_block_id, size, pbuffer, reserved );
    if ( R_SYS_STATUS_SUCCESS == ret )
    {
        psap_info->tx_state = R_UIF_STATE_BUSY;
        R_UIF_TxCyclicMain( port_id );
    }
    else
    {
        status = R_RESULT_FAILED;
    }
    
    return status;
}
/******************************************************************************
   End of function  R_UIF_SendCmd
******************************************************************************/

/******************************************************************************
* Function Name:R_UIF_GetTxState
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_uif_state_t R_UIF_GetTxState(uint8_t block_id )
{
    uint8_t          port_id = uif_get_port_id(block_id);
    r_uif_sap_info_t *psap_info = &uif_info[port_id];
    
    return psap_info->tx_state;
}
/******************************************************************************
   End of function  R_UIF_GetTxState
******************************************************************************/

/******************************************************************************
* Function Name:R_UIF_EventProcess
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
void R_UIF_EventProcess( void )
{
    uint8_t port_id;
    
    for (port_id = 0;port_id <R_MAX_UIF_PORT_NUM;port_id++) /* PRQA S 2877 EOF */
    {
        if (R_UIF_STATE_UNINIT != uif_info[port_id].rx_state)
        {
            R_UIF_RxCyclicMain( port_id );
            if( R_UIF_STATE_BUSY == uif_info[port_id].rx_state )
            {
                R_UIF_CbRxReceiveCmd( port_id, uif_info[port_id].rcv_info.block, uif_info[port_id].rcv_info.pbuffer, uif_info[port_id].rcv_info.size );
                uif_info[port_id].rx_state = R_UIF_STATE_IDLE;
            }
        }
        
    }
}
/******************************************************************************
   End of function  R_UIF_EventProcess
******************************************************************************/

/*****************************************************************************/
/*                           Function for Core                               */
/*****************************************************************************/

/******************************************************************************
* Function Name:R_UIF_ReceiveCmd
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_sys_status_t R_UIF_ReceiveCmd(uint8_t port_id, uint8_t dst_block_id, uint8_t *pbuffer, uint32_t size)
{
    r_result_t          status;
    r_uif_sap_info_t    *psap_info  = &uif_info[port_id];
    uint8_t             src_block_id = uif_get_block_id(port_id);
    st_sys_recv_cmd_t   bin_cmd;


    bin_cmd.dst_block_id = dst_block_id;
    bin_cmd.src_block_id = src_block_id;
    bin_cmd.pbuffer      = pbuffer;
    bin_cmd.size         = size;
    status = R_SYS_ReceiveCmd( &bin_cmd );
    if ( R_RESULT_SUCCESS != status )
    {
        return R_SYS_STATUS_FAILURE;
    }
    psap_info->rx_state = R_UIF_STATE_BUSY;
    
    psap_info->rcv_info.block     = dst_block_id;
    psap_info->rcv_info.pbuffer   = pbuffer;
    psap_info->rcv_info.size      = size;
    
    return R_SYS_STATUS_SUCCESS;
}
/******************************************************************************
   End of function  R_UIF_ReceiveCmd
******************************************************************************/


/******************************************************************************
* Function Name:R_UIF_TxComplete
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
void R_UIF_TxComplete( uint8_t port_id )
{
    r_uif_sap_info_t    *psap_info  = &uif_info[port_id];
    psap_info->tx_state = R_UIF_STATE_IDLE;
}
/******************************************************************************
   End of function  R_UIF_TxComplete
******************************************************************************/

/******************************************************************************
* Function Name:uif_get_port_id
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
static uint8_t uif_get_port_id(uint8_t block_id)
{
    uint8_t port_id;
    
    if( R_C3SAP_BLOCKID_UARTIF_0 == block_id )
    {
        port_id = 0;
    }
    else
    {
        port_id = 1;
    }
    
    return port_id;
}
/******************************************************************************
   End of function  uif_get_port_id
******************************************************************************/

/******************************************************************************
* Function Name:uif_get_block_id
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
static uint8_t uif_get_block_id(uint8_t port_id)
{
    uint8_t block_id = 0;
    
    if( 0 == port_id )
    {
        block_id = R_C3SAP_BLOCKID_UARTIF_0;
    }
    else if( 1 == port_id )
    {
        block_id = R_C3SAP_BLOCKID_UARTIF_1;
    }
    else
    {
        /* do nothing */
    }
    
    return block_id;
}
/******************************************************************************
   End of function  uif_get_block_id
******************************************************************************/


