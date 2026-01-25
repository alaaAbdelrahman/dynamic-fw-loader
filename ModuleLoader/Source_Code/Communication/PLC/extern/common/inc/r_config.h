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
* File Name     : r_config.h
* Device(s)     : RX631
* Tool-chain    : KPIT GNURX-ELF 15.01
* H/W platform  : G-CPX / EU-CPX2 / G-CPX3
* Description   : Configuration Setting
******************************************************************************/

/*
 * Prevent nested inclusions
 */
#ifndef R_CONFIG_H
#define R_CONFIG_H


/******************************************************************************
   Macro definitions
******************************************************************************/
#ifndef R_BOARD_TYPE
//#error "R_BOARD_TYPE not defined"
#define  R_BOARD_TYPE R_BOARD_G_HYBRID
/* Define in the project macro. */
/* ex. R_BOARD_TYPE = 5 */
#endif

#define R_BOARD_NOT_SET         (0)
#define R_BOARD_G_CPX3          (5)
#define R_BOARD_RX231RSK        (6)
#define R_BOARD_J80D1           (9)
#define R_BOARD_J80D2           (10)
#define R_BOARD_G_HYBRID        (11)
#define R_BOARD_G_HYBRID_TEST   (12)
#define R_BOARD_RA6M4RSK        (13)

#if ( R_BOARD_TYPE == R_BOARD_G_CPX3 )
/* Hardware configuration. */
/* The following defines are used by the flash API */
/* MCU that is used. */
/* MCU type : RX631. */
#define R_BOARD_DSW_NUM             (8)
#define R_BOARD_TPGA_CAJ            (0)
#define R_BOARD_CODE_FLASH_ENABLE   (1)

#elif ( R_BOARD_TYPE == R_BOARD_RX231RSK )
/* MCU type : RX231. */
#define R_BOARD_DSW_NUM             (0)
#define R_BOARD_TPGA_CAJ            (0)
#define R_BOARD_CODE_FLASH_ENABLE   (0)


#elif ( R_BOARD_TYPE == R_BOARD_J80D1 )
/* MCU type : RX651. */
#define R_BOARD_DSW_NUM             (8)
#define R_BOARD_EEPROM              (1)
#define R_BOARD_TPGA_CAJ            (7)
#define R_BOARD_CODE_FLASH_ENABLE   (1)


#elif ( R_BOARD_TYPE == R_BOARD_J80D2 )
/* MCU type : RL78G13. */
#define R_RL78_PLC                  (1)
#define MCU_RL78G13                 (1)
#define R_BOARD_DSW_NUM             (8)
#define R_BOARD_TPGA_CAJ            (7)
#define R_BOARD_CODE_FLASH_ENABLE   (0)

#elif ( ( R_BOARD_TYPE == R_BOARD_G_HYBRID ) || (R_BOARD_TYPE == R_BOARD_G_HYBRID_TEST) )
/* MCU type : RX651. */
#define R_BOARD_DSW_NUM             (8)
#define R_BOARD_TPGA_CAJ            (0)
#define R_BOARD_CODE_FLASH_ENABLE   (1)


#elif ( R_BOARD_TYPE == R_BOARD_RA6M4RSK )
/* MCU type : RX651. */
#define R_BOARD_DSW_NUM             (0)
#define R_BOARD_TPGA_CAJ            (0)
#define R_BOARD_CODE_FLASH_ENABLE   (0)

#endif /* R_BOARD_TYPE */



/******************************************************************************
* Defines
******************************************************************************/

//#define R_SYS_BAUD_1000000              12
//#define R_DEFINE_DISABLE_APP_HYBRID
//#define __VANGO__

/* Memory management defines */
#define R_MEMORY_MAX_ELEMENTS           (20u)            /* Max number of allocated memory elements at one time */
#define R_MEMORY_MAX_INSTANCES          (1u)             /* Max number of memory instances */
#define R_MEM_INSTANCE_DEMO             (0u)             /* Name for memory instance used for serial interface / demo application */
/* CPX communication defines */
#define R_CPX_TX_FW_DL_RETRIES          (5u)             /* Maximum number of firmware download retries */

#define QUEUE_SIZE_TIMER                (16u)            /* Length of queue used for timeout events */

/* Thread timing defines */
#define R_CPX_THREAD_PERIOD             (3u)             /* Period between two thread calls in ms */
#define R_APP_THREAD_PERIOD             (100u)           /* Period between two thread calls in ms */

/* Timer defines */
#define R_MAX_TIMER_ONE_SHOT_INSTANCES  (3u)             /* Number of timer instances for this project */
#define R_HW_TIMER_TICK_PERIOD          (10u)            /* The HW timer tick period in ms of the HW timer */
#define R_TIMER_ID_CPX                  (0u)             /* Id of the first timer instance to be used */
#define R_TIMER_ID_FW_DL                (R_TIMER_ID_CPX) /* Id of the FW DL module, in PRIME only one timer instance is used */
#define R_TIMER_ID_APP                  (1u)             /* Id of the second timer instance to be used */
#define R_TIMER_ID_LED                  (2u)             /* Id of the led timer instance to be used  */

/* Frame lengths. */
#define R_MAX_MTU_SIZE                  (350)          /* The maximum number of octets that can be transmitted as ADP payload field */

/* NetXDuo IPv6 stack configuration. */
#define R_NETX_PACKET_POOL_SIZE         (4096u)          /* Size of NetXDuo packet pool in bytes */
#define R_NETX_THREAD_STACK_SIZE        (2048u)          /* Size of NetXDuo thread stack in bytes */
#define R_NETX_UDP_QUEUE_MAXIMUM        (16u)            /* Number of queue items stored in socket operation */


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#endif /* R_CONFIG_H */

