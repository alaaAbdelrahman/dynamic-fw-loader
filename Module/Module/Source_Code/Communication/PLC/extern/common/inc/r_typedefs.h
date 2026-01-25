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
* File Name     : r_typedefs.h
* Device(s)     : RX631
* Tool-chain    : KPIT GNURX-ELF 15.01
* H/W platform  : G-CPX / EU-CPX2 / G-CPX3
* Description   : Sample software
******************************************************************************/

/*
 * Prevent nested inclusions
 */
#ifndef R_TYPEDEFS_H
#define R_TYPEDEFS_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "r_config.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_FALSE  (0u)                     /* Boolean false */
#define R_TRUE   (1u)                     /* Boolean true */
#define UNUSED(x)               ((void)(x))

#define R_strlen(s)             (strlen (s))
#define R_strcmp(s1, s2)        (strcmp ((s1), (s2)))
#define R_strncmp(s1, s2, sz)   (strncmp ((s1), (s2), (sz)))
#define R_memcmp(s1, s2, sz)    (memcmp ((s1), (s2), (sz)))
#define R_memcpy(dst, src, sz)  (memcpy ((dst), (src), (sz)))
#define R_memset(dst, c, sz)    (memset ((dst), (c), (sz)))
#define R_strcpy(s, t)          (strcpy ((s), (t)))
#define R_strcat(s, t)          (strcat ((s), (t)))

#define R_VOLATILE  volatile

#if defined(_WIN64) 
typedef uint64_t                r_uintptr_t;
typedef uint64_t                r_size_t;
typedef int64_t                 r_intptr_t;
#else /* defined(_WIN64)  */
#if defined (R_RL78_PLC)
#define FAR __far
typedef uint16_t                r_uintptr_t;
typedef uint16_t                r_size_t;
typedef int16_t                 r_intptr_t;
#else /*(defined R_RL78_PLC) */
#define FAR
typedef uint32_t                r_uintptr_t;
typedef uint32_t                r_size_t;
typedef int32_t                 r_intptr_t;
#endif /* defined (R_RL78_PLC) */
#endif /* defined(_WIN64)  */

/******************************************************************************
Typedef definitions
******************************************************************************/


/*!
    \brief Boolean type
 */
typedef uint8_t r_boolean_t;

/*!
    \enum r_result_t
    \brief Structure for the results of the mac processes
 */
typedef enum
{
    R_RESULT_SUCCESS              = 0x00, /*!< Success */
    R_RESULT_FAILED               = 0x01, /*!< Process failed */
    R_RESULT_BAD_INPUT_ARGUMENTS  = 0x02, /*!< Bad arguments/attributes */
    R_RESULT_ILLEGAL_NULL_POINTER = 0x03, /*!< Input to a function was null pointer */
    R_RESULT_INVALID_REQUEST      = 0x04, /*!< Not accepted request */
    R_RESULT_TIMEOUT              = 0x05,
    R_RESULT_ABORTED              = 0x06,
    R_RESULT_MEMORY_NOT_ENOUGH    = 0x07,
    R_RESULT_NOT_SUPPORTED        = 0x08,
    R_RESULT_CONTINUE_NEEDED      = 0x09,
    R_RESULT_DEVICE_BUSY          = 0xF7, /*!< Device busy (prime only) */
    R_RESULT_FINISHED_PROCESSING  = 0xFE, /*!< Finished processing (prime only) */
    R_RESULT_UNKNOWN              = 0xFF  /*!< Default value to set before proper values have been set */
} r_result_t;

typedef r_result_t e_result_t; /* GSCE Coding Rule V500 */

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/


#endif /* R_TYPEDEFS_H */

