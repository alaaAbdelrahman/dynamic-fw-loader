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
* File Name   : r_flash_lib.c
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
static uint8_t s_demo_flash_item[R_FLASH_ITEM_SIZE];
#if ( R_FLASH_ITEM_SIZE < R_FLASH_BLOCK_SIZE )
static uint8_t s_demo_flash_rmem[R_FLASH_BLOCK_SIZE];
#endif

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
* Function Name: r_demo_flash_init
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint8_t r_demo_flash_init( void )
{
    if( R_RESULT_SUCCESS != R_BSP_ConfigureFlash() )
    {
        return (uint8_t)R_RESULT_FAILED;
    }
    return (uint8_t)R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  r_demo_flash_init
******************************************************************************/


/******************************************************************************
* Function Name: r_demo_flash_write
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint8_t r_demo_flash_write(uint32_t df_dstaddr, uint8_t *src_buff, uint32_t size_byte)
{
#if   defined (R_BOARD_EEPROM)
    r_result_t ret;

    ret = R_BSP_FlashWrite(df_dstaddr, src_buff, size_byte);

	return (uint8_t)ret;
#else

#if ( R_FLASH_ITEM_SIZE >= R_FLASH_BLOCK_SIZE )
    r_result_t ret;
    uint32_t erase_byte = (uint32_t)(((size_byte + R_FLASH_BLOCK_SIZE) - 1) & (uint32_t)R_FLASH_BLOCK_MASK);
    uint32_t write_byte = (uint32_t)((size_byte + (R_FLASH_WRITE_LINE - 1)) & (~(R_FLASH_WRITE_LINE - 1)));
    
    ret = R_BSP_FlashErase(df_dstaddr, erase_byte);

    if(R_RESULT_SUCCESS == ret)
    {
        ret = R_BSP_FlashWrite(df_dstaddr, src_buff, write_byte);
    }
    
    return (uint8_t)ret;
#else
    uint8_t ret;
    uint32_t offset_size = df_dstaddr & (R_FLASH_BLOCK_SIZE - 1);
    uint32_t erase_addr = df_dstaddr & (uint32_t)R_FLASH_BLOCK_MASK;
    
    if((offset_size + size_byte) > R_FLASH_BLOCK_SIZE )
    {
        return (uint8_t)R_RESULT_FAILED;
    }
    
    R_memcpy((void *)s_demo_flash_rmem, (const void *)erase_addr, R_FLASH_BLOCK_SIZE );     /* 1Block cpy to buff */
    R_memcpy(&s_demo_flash_rmem[offset_size], src_buff, size_byte);
    

    ret = R_BSP_FlashErase(erase_addr,R_FLASH_BLOCK_SIZE);  /* 1Block erase */
    if (R_RESULT_SUCCESS == ret)
    {
        ret = R_BSP_FlashWrite(erase_addr, s_demo_flash_rmem, (uint32_t)R_FLASH_BLOCK_SIZE); /* 1Block write */
    }
    
    return (uint8_t)ret;
#endif

#endif /* defined (R_BOARD_EEPROM) */
}
/******************************************************************************
   End of function  r_demo_flash_write
******************************************************************************/

/******************************************************************************
* Function Name: r_demo_flash_tmp_buff
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint8_t* r_demo_flash_tmp_buff( void )
{
    return s_demo_flash_item;
}
/******************************************************************************
   End of function  r_demo_flash_tmp_buff
******************************************************************************/

/******************************************************************************
* Function Name: r_demo_flash_erase
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint8_t r_demo_flash_erase(uint32_t df_dstaddr, uint32_t size_byte)
{
#if   defined (R_BOARD_EEPROM)
    return R_RESULT_SUCCESS;
#else /* defined (R_BOARD_EEPROM) */
    r_result_t ret;
    uint32_t  erase_byte = (uint32_t)(((size_byte + R_FLASH_BLOCK_SIZE) - 1) & (uint32_t)R_FLASH_BLOCK_MASK);

#if ( R_FLASH_ITEM_SIZE >= R_FLASH_BLOCK_SIZE )
    ret = R_BSP_FlashErase(df_dstaddr,erase_byte);
#if 1
    /* Future Remove */
    R_memset(s_demo_flash_item, 0, erase_byte);
    if(R_RESULT_SUCCESS == ret)
    {
        ret = R_BSP_FlashWrite(df_dstaddr, s_demo_flash_item, erase_byte);
    }
#endif
    
    return (uint8_t)ret;
#else
    uint32_t offset_size = df_dstaddr & (R_FLASH_BLOCK_SIZE - 1);
    uint32_t erase_addr = df_dstaddr & (uint32_t)R_FLASH_BLOCK_MASK;
    
    if((offset_size + size_byte) > R_FLASH_BLOCK_SIZE )
    {
        return R_RESULT_FAILED;
    }
    
    R_memcpy((void *)s_demo_flash_rmem, (const void *)erase_addr, R_FLASH_BLOCK_SIZE );     /* 1Block cpy to buff */
    
    ret = R_BSP_FlashErase(erase_addr, R_FLASH_BLOCK_SIZE);                                 /* 1Block erase */
    
    R_memset(&s_demo_flash_rmem[offset_size], 0, size_byte);
    if (R_RESULT_SUCCESS == ret)
    {
        ret = R_BSP_FlashWrite(erase_addr,s_demo_flash_rmem, R_FLASH_BLOCK_SIZE);
    }
    
    return (uint8_t)ret;
#endif
#endif /* defined (R_BOARD_EEPROM) */
}
/******************************************************************************
   End of function  r_demo_flash_erase
******************************************************************************/

/******************************************************************************
* Function Name: r_demo_flash_read
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void r_demo_flash_read(uint8_t **dst_buff, r_size_t df_srcaddr, uint16_t size_byte)
{
    *dst_buff = s_demo_flash_item;

    R_BSP_FlashRead(s_demo_flash_item,df_srcaddr,size_byte);
}
/******************************************************************************
   End of function  r_demo_flash_read
******************************************************************************/


