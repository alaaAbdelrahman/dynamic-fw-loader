/******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
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
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
******************************************************************************/
/******************************************************************************
   Includes   <System Includes> , "Project Includes"
******************************************************************************/
//#include "r_bsp.h"
//#include "r_flash_targets.h"            /* For MCU_DATA_FLASH_SIZE_BYTES */
#include "cmsis_iar.h"
/******************************************************************************
* File Name     : r_bsp_api.h
* Device(s)     : RX631/RX651/RX231
* Tool-chain    : KPIT GNURX-ELF 16.01 / CC-RX 2.05
* H/W platform  : G-CPX3/B-CPX3/
* Description   : Sample software
******************************************************************************/
#ifndef R_BSP_API_H
#define R_BSP_API_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_UART_IPR          (0x09u)     /* UART RX priority */
#define R_TIMER_IPR         (0x07u)     /* Free Run Timer priority */
#define R_CMT_CPX_IPR       (0x06u)     /* CMT Timer priority ( CPX SAP Thread ) */
#define R_CMT_APP_IPR       (0x02u)     /* CMT Timer priority ( APP Thread ) */

#define FW_ADDRESS (uint8_t*)0x40000000
/******************************************************************************
Typedef definitions
******************************************************************************/
typedef void (*r_bsp_callback_t)(void);             // Callback function type with void parameter list
typedef void (*r_bsp_uart_rx_callback_t)(uint8_t);  // Callback function type with parameter list

/*!
   \enum r_bsp_led_t
   \brief Enumeration for the LEDs
 */
typedef enum
{
    R_BSP_LED_4, // LED 4, available on EU-OFDM-PLC and G-CPX
    R_BSP_LED_5, // LED 5, available on EU-OFDM-PLC and G-CPX
    R_BSP_LED_NUM
} r_bsp_led_t;


/*!
   \enum r_bsp_boot_t
   \brief Enumeration for the CPX BOOT MODE
 */
typedef enum
{
    R_BSP_BOOT_UART, // UART Boot
    R_BSP_BOOT_SROM, // S-ROM boot
} r_bsp_boot_t;

/*!
   \enum r_bsp_uart_t
   \brief Enumeration for different UARTs
 */
typedef enum
{
    R_BSP_UART_MCU_CPX = 0,     // UART between RX and CPX
    R_BSP_UART_MCU_HOST,    // UART between RX and host
    R_BSP_UART_MCU_DEBUG,
    R_BSP_UART_MAX,
} r_bsp_uart_t;

/*!
   \enum r_bsp_timer_id_t
   \brief Enumeration for different TAU TIMER IDs
 */
typedef enum
{
    R_BSP_TIMER_ID_0,       // TAU TIMER sw timer
    R_BSP_TIMER_ID_1,       // CM TIMER  app thread prosessing
    R_BSP_TIMER_ID_2,       // CM TIMER  c3sap threadd  prosessing
    R_BSP_TIMER_ID_MAX
} r_bsp_timer_id_t;

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
/***********************************************************************
* Function Name     : R_BSP_SetBoardType
* Description       : Configures board type to be used
* Argument          : uint8_t boardType The type of the board to be configured
* Return Value      : R_RESULT_SUCCESS if type known, R_RESULT_FAILED otherwise
***********************************************************************/
/*!
   \fn          r_result_t R_BSP_SetBoardType(uint8_t boardType)
   \brief       Configures board type to be used
   \param[in]   boardType Board type to be used
   \return      R_RESULT_SUCCESS if type known, R_RESULT_FAILED otherwise
 */
r_result_t R_BSP_SetBoardType(uint8_t boardType);

/******************************************************************************
* Function Name     : R_BSP_GetDipSwBit
* Description       : Get DIP switch value
* Arguments         : uint8_t bit DIP switch bit
* Return Value      : DIP switch bit value
******************************************************************************/
/*!
   \fn          uint8_t R_BSP_GetDipSwBit(uint8_t bit)
   \brief       Get DIP switch value
   \param[in]   bit DIP switch bit
   \return      DIP switch bit value
 */
uint8_t R_BSP_GetDipSwBit(uint8_t bit);

/******************************************************************************
* Function Name     : R_BSP_GetDipSwByte
* Description       : Get DIP switch value
* Arguments         : void
* Return Value      : DIP switch byte value
******************************************************************************/
/*!
   \fn          uint8_t R_BSP_GetDipSwByte(void)
   \brief       Get DIP switch value
   \param[in]   void
   \return      DIP switch byte value
 */
uint8_t R_BSP_GetDipSwByte(void);

/******************************************************************************
* Function Name     : R_BSP_InitLeds
* Description       : Initializes the LEDs and turn all LEDs off
* Argument          : none
* Return Value      : none
******************************************************************************/
void R_BSP_InitLeds(void);

/******************************************************************************
* Function Name     : R_BSP_LedOn
* Description       : Turns a LED on
* Argument          : r_bsp_led_t led LED enumeration
* Return Value      : none
******************************************************************************/
void R_BSP_LedOn(r_bsp_led_t led);

/******************************************************************************
* Function Name     : R_BSP_LedOff
* Description       : Turns a LED off
* Argument          : r_bsp_led_t led ED enumeration
* Return Value      : none
******************************************************************************/
void R_BSP_LedOff(r_bsp_led_t led);

/******************************************************************************
* Function Name     : R_BSP_ToggleLed
* Description       : Toggles a LED
* Argument          : r_bsp_led_t led LED enumeration
* Return Value      : none
******************************************************************************/
void R_BSP_ToggleLed(r_bsp_led_t led);

/***********************************************************************
* Function Name     : R_BSP_SetBootMode
* Description       : Select boot mode
* Argument          : r_bsp_boot_t mode Boot mode
* Return Value      : none
***********************************************************************/
/*!
   \fn          void R_BSP_SetBootMode(r_bsp_boot_t mode)
   \brief       Select boot mode
   \param[in]   Boot mode
   \return      None
 */
void R_BSP_SetBootMode(r_bsp_boot_t mode);

/******************************************************************************
* Function Name     : R_BSP_ConfigureTimer
* Description       : Configure the PWM timers 
* Arguments         : timer_id Id of the timer to be configured
*                   : cycle_ms Cycle in which the callback function is notified
*                   : callback p_callback Pointer to the timer callback function which
*                   : will be caled every timer cycle
* Return Value      : Either R_RESULT_SUCCESS ,R_RESULT_FAILED, R_RESULT_BAD_INPUT_ARGUMENTS
******************************************************************************/
r_result_t R_BSP_ConfigureTimer(r_bsp_timer_id_t timer_id, uint32_t cycle_ms, r_bsp_callback_t callback);

/******************************************************************************
* Function Name     : R_BSP_TimerOn
* Description       : Sets the compare match timer on
* Argument          : none
* Return Value      : none
******************************************************************************/
void R_BSP_TimerOn(r_bsp_timer_id_t timer_id);

/******************************************************************************
* Function Name     : R_BSP_TimerOff
* Description       : Sets the compare match timer off
* Argument          : none
* Return Value      : none
******************************************************************************/
void R_BSP_TimerOff(r_bsp_timer_id_t timer_id);

/******************************************************************************
* Function Name     : R_BSP_ConfigureUart
* Description       : Configure the desired UART channel
* Arguments         : uart_id Id of the uart to be configured
*                   : tx_finished_cb Pointer to the uart callback function which
*                   : will be notified when transmission is finished
*                   : rx_finished_cb A pointer to the uart callback function whch
*                   : will be notified when uart data is received
* Return Value      : Either R_RESULT_SUCCESS ,R_RESULT_FAILED, 
*                   : R_RESULT_BAD_INPUT_ARGUMENTS
******************************************************************************/
r_result_t R_BSP_ConfigureUart(r_bsp_uart_t uart_id, uint32_t baud_rate , r_bsp_callback_t tx_finished_cb, r_bsp_uart_rx_callback_t rx_finished_cb );

/******************************************************************************
* Function Name     : R_BSP_UartSetBaudrate
* Description       : Change UART baudrate setting
* Arguments         : uart_id Id of the uart to be configured
*                   : baud_rate new baudrate
* Return Value      : Either R_RESULT_SUCCESS ,R_RESULT_FAILED,
*                   : R_RESULT_BAD_INPUT_ARGUMENTS
******************************************************************************/
r_result_t R_BSP_UartSetBaudrate(r_bsp_uart_t uart_id, uint32_t baud_rate);

/******************************************************************************
* Function Name     : R_BSP_UartSend
* Description       : Send uart data
* Arguments         : uart_id Id of the uart to be configured
*                   : p_data Pointer to the uart data
*                   : size Send data size
* Return Value      : R_RESULT_SUCCESS ,R_RESULT_FAILED, R_RESULT_BAD_INPUT_ARGUMENTS
******************************************************************************/
r_result_t R_BSP_UartSend(r_bsp_uart_t uart_id, const uint8_t* p_data, uint32_t length);

/******************************************************************************
* Function Name     : R_BSP_ConfigureFlash
* Description       : Configure the Flash access api
* Arguments         : p_data Pointer to the uart data
*                   : none
* Return Value      : R_RESULT_SUCCESS ,R_RESULT_FAILED
******************************************************************************/
r_result_t R_BSP_ConfigureFlash( void );

/******************************************************************************
* Function Name     : R_BSP_FlashWrite
* Description       : Write data flash
* Arguments         : dst_addr destination flash address
*                   : src_buff source address
*                   : size_byte write size
* Return Value      : R_RESULT_SUCCESS ,R_RESULT_FAILED
******************************************************************************/
r_result_t R_BSP_FlashWrite( uint32_t dst_addr, uint8_t *src_buff, uint32_t size_byte );

/******************************************************************************
* Function Name     : R_BSP_FlashRead
* Description       : Read data flash
* Arguments         : dst_buff destination ram address
*                   : src_addr source flash address
*                   : size_byte read size
* Return Value      : R_RESULT_SUCCESS ,R_RESULT_FAILED
******************************************************************************/
r_result_t R_BSP_FlashRead( uint8_t *dst_buff, uint32_t src_addr, uint32_t size_byte );

/******************************************************************************
* Function Name     : R_BSP_FlashErase
* Description       : Erase data flash
* Arguments         : dst_addr erasing flash address (64byte alignment)
*                   : size_byte erase size
* Return Value      : R_RESULT_SUCCESS ,R_RESULT_FAILED,R_RESULT_BAD_INPUT_ARGUMENTS
******************************************************************************/
r_result_t R_BSP_FlashErase( uint32_t dst_addr, uint32_t size_byte );

/***********************************************************************
* Function Name     : R_BSP_CheckInStack
* Description       : MCU-dependant check if pointer resides in the stack
* Argument          : p_parameter: Address to check
* Return Value      : Either R_RESULT_SUCCESS or R_RESULT_FAILED
***********************************************************************/
/*!
   \fn          r_result_t R_BSP_CheckInStack(const uint8_t* p_parameter)
   \brief       MCU-dependant check if pointer resides in the stack
   \details     MCU-dependant check if pointer resides in the stack
   \param[in]   p_parameter: Address to check
   \return      Either R_RESULT_SUCCESS or R_RESULT_FAILED
 */
r_result_t R_BSP_CheckInStack(const uint8_t* p_parameter);

/***********************************************************************
* Function Name     : R_BSP_SoftReset
* Description       : Initiate MCU reset
* Argument          : none
* Return Value      : none
***********************************************************************/
/*!
   \fn          void R_BSP_SoftReset(void)
   \brief       Initiate MCU reset
   \return      void
 */
void R_BSP_SoftReset(void);

/***********************************************************************
* Function Name     : R_BSP_Cpx3Reset
* Description       : Reset the CPX2
* Argument          : none
* Return Value      : none
***********************************************************************/
/*!
   \fn          void R_BSP_Cpx3Reset(void)
   \brief       CPX2 reset
   \return      void
 */
void R_BSP_Cpx3Reset(void);

/***********************************************************************
* Function Name     : R_BSP_EI
* Description       : Enable interrupt
* Argument          : none
* Return Value      : none
***********************************************************************/
static inline void R_BSP_EI(void)
{
    __enable_irq();   // Enable interrupts;
}
/************************************************************************
   End of function  R_BSP_EI
************************************************************************/

/***********************************************************************
* Function Name     : R_BSP_DI
* Description       : Disable interrupt
* Argument          : none
* Return Value      : none
***********************************************************************/
static inline void R_BSP_DI(void)
{
    __disable_irq(); // Disable interrupts
}
/************************************************************************
   End of function  R_BSP_DI
************************************************************************/

/***********************************************************************
* Function Name     : R_BSP_NOP
* Description       : Nop
* Argument          : none
* Return Value      : none
***********************************************************************/
void R_BSP_NOP(void);

/************************************************************************
   End of function  R_BSP_NOP
************************************************************************/

void Send_DMA_UART(uint32_t tx_size, uint8_t* buff);

#endif /* R_BSP_API_H */

