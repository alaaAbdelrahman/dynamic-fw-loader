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

/******************************************************************************
* File Name     : r_memory_api.h
* Device(s)     : RX631
* Tool-chain    : KPIT GNURX-ELF 15.01
* H/W platform  : G-CPX / EU-CPX2 / G-CPX3
* Description   : Sample software
******************************************************************************/

/*
 * Prevent nested inclusions
 */
#ifndef R_MEMORY_API_H
#define R_MEMORY_API_H

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/***********************************************************************
* Function Name     : R_MEMORY_Init
* Description       : This function initializes a memory management entity
* Argument          : instance_id : Id of the memory instance
*                     pmem_pool : Pointer to a chunk of memory to be managed by the management entity
*                     mem_size : Size in bytes of the memoryPool
* Return Value      : R_RESULT_FAILED or R_RESULT_SUCCESS
***********************************************************************/
/*!
    \fn r_result_t R_MEMORY_Init(const uint8_t instance_id,
                                 const uint8_t* pmem_pool,
                                 const uint16_t mem_size);
    \brief This function initializes a memory management entity
    \param[in] instance_id Id of the memory instance
    \param[in] pmem_pool Pointer to a chunk of memory to be managed by the management entity
    \param[in] mem_size Size in bytes of the memoryPool
    \return R_RESULT_FAILED or R_RESULT_SUCCESS
 */
r_result_t R_MEMORY_Init(const uint8_t instance_id,
                         const uint8_t* pmem_pool,
                         const uint16_t mem_size);

/***********************************************************************
* Function Name     : R_MEMORY_Flush
* Description       : This function flushes a memory management entity
* Argument          : instance_id : Id of the memory instance
* Return Value      : R_RESULT_FAILED or R_RESULT_SUCCESS
***********************************************************************/
/*!
    \fn r_result_t R_MEMORY_Flush(const uint8_t instance_id);
    \brief This function flushes a memory management entity
    \param[in] instance_id Id of the memory instance
    \return R_RESULT_FAILED or R_RESULT_SUCCESS
 */
r_result_t R_MEMORY_Flush(const uint8_t instance_id);

/***********************************************************************
* Function Name     : R_MEMORY_Malloc
* Description       : This function allocates size bytes of memory for the instance with id instance_id
* Argument          : instance_id : Id of the memory instance
*                     size : Number of bytes requested
* Return Value      : Address where the allocated chunk of size bytes begins. NULL if the allocation failed.
***********************************************************************/
/*!
    \fn uint8_t* R_MEMORY_Malloc(const uint8_t instance_id,
                                 const uint16_t size);
    \brief This function allocates size bytes of memory for the instance with id instance_id
    \param[in] instance_id Id of the memory instance
    \param[in] size Number of bytes requested
    \return Address where the allocated chunk of size bytes begins. NULL if the allocation failed.
 */
uint8_t* R_MEMORY_Malloc(const uint8_t instance_id,
                         const uint16_t size);

/***********************************************************************
* Function Name     : R_MEMORY_Realloc
* Description       : Reallocates a memory region that has been allocated with R_MEMORY_Malloc. In this implementation the memory region can only be
                          decreased in size. The memory region will not be increased or moved to a new location. The function will also fail if called
                          with NULL for ptr.
* Argument          : instance_id : Id of the memory instance
*                     ptr : The address that will be reallocated.
*                     size : Target size in bytes of the memory region to be re-sized
* Return Value      : NULL if the reallocation has failed, else ptr
***********************************************************************/
/*!
    \fn uint8_t* R_MEMORY_Realloc(const uint8_t instance_id,
                                  uint8_t* ptr,
                                  const uint16_t size);
    \brief Reallocates a memory region that has been allocated with R_MEMORY_Malloc. In this implementation the memory region can only be
            decreased in size. The memory region will not be increased or moved to a new location. The function will also fail if called
            with NULL for ptr.
    \param[in] instance_id Id of the memory instance
    \param[in] ptr The address that will be reallocated.
    \param[in] size Target size in bytes of the memory region to be re-sized
    \return NULL if the reallocation has failed, else ptr
 */
uint8_t* R_MEMORY_Realloc(const uint8_t instance_id,
                          uint8_t* ptr,
                          const uint16_t size);

/***********************************************************************
* Function Name     : R_MEMORY_Free
* Description       : This function frees a memory region
* Argument          : instance_id Id of the memory instance
*                     ptr The address that will be freed. The number of bytes that were allocated at that address will be freed.
* Return Value      : None
***********************************************************************/
/*!
    \fn void R_MEMORY_Free(const uint8_t instance_id,
                           const uint8_t* ptr);
    \brief This function frees a memory region
    \param[in] instance_id Id of the memory instance
    \param[in] ptr The address that will be freed. The number of bytes that were allocated at that address will be freed.
    \return void. For a NULL pointer the function does nothing. For a pointer that has not been returned by R_MEMORY_Malloc or R_MEMORY_Realloc the function does nothing.
 */
void R_MEMORY_Free(const uint8_t instance_id,
                   const uint8_t* ptr);

#endif /* R_MEMORY_API_H */
