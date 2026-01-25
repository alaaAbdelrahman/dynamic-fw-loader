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
* http://www.renesas.com/disclaimer*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
 * File Name   : r_io_vec.h
 * Version     : 1.0
 * Description : This module implements input/ouput vector structures
 ******************************************************************************/

/*!
   \file      r_io_vec.h
   \version   1.00
   \brief     This module implements input/ouput vector structures
 */

/*
 * Prevent nested inclusions
 */
#ifndef R_IO_VEC_H
#define R_IO_VEC_H

/******************************************************************************
   Macro definitions
******************************************************************************/

#define IOVEC_CREATE(name, length) \
    r_iovec_element_t ioVecVector ## name [(length)]; \
    r_iovec_class_t (name) = { 0, 0, 0, 0 };

#define IOVEC_INIT(name) \
    (&(name))->pio_vec       = ioVecVector ## name; \
    (&(name))->num_elements = sizeof(ioVecVector ## name) / sizeof(r_iovec_element_t); \
    R_IO_VecRese(&(name));

/******************************************************************************
   Typedef definitions
******************************************************************************/

/*!
    \struct r_iovec_element_t
    \brief Structure for collecting pointers to data chunks
 */
typedef struct
{
    const uint8_t* paddress; //!< Address where the data starts
    uint32_t       length;  //!< Length of data in bytes

} r_iovec_element_t;

/*!
    \struct r_iovec_class_t
    \brief Class structure for collecting pointers to data chunks
 */
typedef struct
{
    r_iovec_element_t* pio_vec;        //!< Actual pio_vec Elements
    uint32_t           total_length;  //!< Total length in bytes in all elements
    uint8_t            num_elements;  //!< Maximum number of elements available
    uint8_t            used_elements; //!< Used number of elements in class

} r_iovec_class_t;

/******************************************************************************
   Exported global variables
******************************************************************************/

/******************************************************************************
   Exported global functions (to be accessed by other files)
******************************************************************************/

/*!
    \fn void R_IO_VecRese(r_iovec_class_t* ioVecClass);
    \brief This function initializes the iovec class (setting pointers and lengths to NULL/zero)
    \param[in] ioVecClass Pointer to iovec class to be initialized
    \return None
 */
void R_IO_VecRese(r_iovec_class_t* ioVecClass);

/*!
    \fn r_result_t R_IO_VecAppendElement(r_iovec_class_t* ioVecClass, const uint8_t* address, const uint16_t length)
    \brief This function sets a new element in an iovec class as long as elements are available
           The new element is appended after the last used element
    \param[in] ioVecClass Pointer to iovec class
    \param[in] address Pointer for the new element
    \param[in] length Length for the new element
    \return R_RESULT_SUCCESS when successfully written, R_RESULT_FAILED otherwise
 */
r_result_t R_IO_VecAppendElement(r_iovec_class_t* ioVecClass,
                              const uint8_t* address,
                              const uint16_t length);

/*!
    \fn r_result_t R_IO_VecPrependElement(r_iovec_class_t* ioVecClass, const uint8_t* address, const uint16_t length)
    \brief This function sets a new element in an iovec class as long as elements are available
           The new element is prepended before the first used element
    \param[in] ioVecClass Pointer to iovec class
    \param[in] address Pointer for the new element
    \param[in] length Length for the new element
    \return R_RESULT_SUCCESS when successfully written, R_RESULT_FAILED otherwise
 */
r_result_t R_IO_VecPrependElement(r_iovec_class_t* ioVecClass,
                               const uint8_t* address,
                               const uint16_t length);

/*!
    \fn r_result_t R_IO_VecToContinous(const r_iovec_class_t* ioVecClass, uint8_t* buffer, const uint32_t bufferLength, uint32_t* writtenBytes)
    \brief This function copies all pio_vec elements of a class into a continous memory provided by buffer
    \param[in]  ioVecClass Pointer to iovec class
    \param[in]  buffer Pointer to continuous buffer
    \param[in]  bufferLength Length of continuous buffer
    \param[out] writtenBytes Number of bytes written to continous buffer
    \return R_RESULT_SUCCESS when successfully written, R_RESULT_FAILED otherwise
 */
r_result_t R_IO_VecToContinous(const r_iovec_class_t* ioVecClass,
                            uint8_t* buffer,
                            const uint32_t bufferLength,
                            uint32_t* writtenBytes);

/*!
    \fn r_result_t R_IO_VecCopy(const r_iovec_class_t* srcIoVecClass,
                     r_iovec_class_t* dstIoVecClass)
    \brief This function appends all elements of srcIoVecClass to dstIoVecClass, if a sufficient number of elements is available in dstIoVecClass
    \param[in]  srcIoVecClass Pointer to source iovec class
    \param[in]  dstIoVecClass Pointer to destination iovec class
    \return R_RESULT_SUCCESS when successfully copied, R_RESULT_FAILED otherwise
 */
r_result_t R_IO_VecCopy(const r_iovec_class_t* srcIoVecClass,
                     r_iovec_class_t* dstIoVecClass);

#endif /* R_IO_VEC_H */
