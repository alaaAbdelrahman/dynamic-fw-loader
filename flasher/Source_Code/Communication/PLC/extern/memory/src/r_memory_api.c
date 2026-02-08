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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
******************************************************************************/

/******************************************************************************
* File Name     : r_memory_api.c
* Device(s)     : RX631
* Tool-chain    : KPIT GNURX-ELF 15.01
* H/W platform  : G-CPX / EU-CPX2 / G-CPX3
* Description   : Sample software
******************************************************************************/

/*******************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : 09.08.2013 1.00    First Release
 ******************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include <stdlib.h>
#include "r_typedefs.h"
#include "r_memory_api.h"
#include "r_config.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/
/*!
    \enum r_memory_status_t
    \brief Enumeration for the memory element status
 */
typedef enum
{
    R_MEMORY_STATUS_FREE,
    R_MEMORY_STATUS_USED

} r_memory_status_t;

/*!
    \struct r_memory_element_t
    \brief Structure for the memory array elements
 */
typedef struct
{
    const uint8_t*        pdata;
    uint16_t              size;
    r_memory_status_t     status;

} r_memory_element_t;

/*!
    \struct r_memory_t
    \brief Structure for the memory management
 */
typedef struct
{
    const uint8_t*      pmemory;
    uint16_t            max_mem_size;
    r_memory_element_t  mem_array[R_MEMORY_MAX_ELEMENTS];

} r_memory_t;

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
/******************************************************************************
* Static variables
******************************************************************************/
static r_memory_t mem_mgmt[R_MEMORY_MAX_INSTANCES]; /* no initialization required here */

/******************************************************************************
* Local function headers
******************************************************************************/
static r_result_t find_free_region(const uint8_t instance_id,
                                   const uint16_t size,
                                   uint16_t* pindex,
                                   uint16_t* premaining_size);

static void merge_free_regions(const uint8_t instance_id);

static r_result_t find_consecutive_free_regions(const uint8_t instance_id,
                                                uint16_t* pindex1,
                                                uint16_t* pindex2);

/******************************************************************************
* Extern variables
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
Function implementations
******************************************************************************/

/******************************************************************************
* Function Name:R_MEMORY_Init
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_MEMORY_Init(const uint8_t instance_id,
                         const uint8_t* pmem_pool,
                         const uint16_t mem_size)
{
    if (((instance_id >= R_MEMORY_MAX_INSTANCES) || (NULL == pmem_pool)) || (0u   == mem_size ))
    {
        return R_RESULT_FAILED;
    }
    else
    {
        /* do nothing */
    }

    mem_mgmt[instance_id].pmemory      = pmem_pool;
    mem_mgmt[instance_id].max_mem_size = mem_size;

    return R_MEMORY_Flush(instance_id);
}
/******************************************************************************
   End of function  R_MEMORY_Init
******************************************************************************/

/******************************************************************************
* Function Name:R_MEMORY_Flush
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_MEMORY_Flush(const uint8_t instance_id)
{
    uint16_t i;

    if ((instance_id >= R_MEMORY_MAX_INSTANCES) ||
        (NULL == mem_mgmt[instance_id].pmemory))
    {
        return R_RESULT_FAILED;
    }
    else
    {
        /* do nothing */
    }

    for (i = 0; i < R_MEMORY_MAX_ELEMENTS; i++)
    {
        mem_mgmt[instance_id].mem_array[i].pdata  = NULL;
        mem_mgmt[instance_id].mem_array[i].size   = 0;
        mem_mgmt[instance_id].mem_array[i].status = R_MEMORY_STATUS_FREE;
    }

    /* all memory is free after flush */
    mem_mgmt[instance_id].mem_array[0].pdata = mem_mgmt[instance_id].pmemory;
    mem_mgmt[instance_id].mem_array[0].size  = mem_mgmt[instance_id].max_mem_size;

    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_MEMORY_Flush
******************************************************************************/

/******************************************************************************
* Function Name:find_free_region
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
static r_result_t find_free_region(const uint8_t instance_id,
                                   const uint16_t size,
                                   uint16_t* pindex,
                                   uint16_t* premaining_size)
{
    uint16_t i;
    r_result_t result = R_RESULT_FAILED; /* will be set to success if it is the case */

    if (((((instance_id >= R_MEMORY_MAX_INSTANCES)  ||
           (NULL == mem_mgmt[instance_id].pmemory)) ||
           (NULL == pindex))                        ||
           (NULL == premaining_size))               ||
           (size > mem_mgmt[instance_id].max_mem_size))
    {
        return R_RESULT_FAILED;
    }
    else
    {
        /* do nothing */
    }

    for (i = 0; i < R_MEMORY_MAX_ELEMENTS; i++)
    {
        if (0u == size)
        {
            if (((R_MEMORY_STATUS_FREE == mem_mgmt[instance_id].mem_array[i].status)  &&
                 (NULL                 == mem_mgmt[instance_id].mem_array[i].pdata )) &&
                 (0u                   == mem_mgmt[instance_id].mem_array[i].size  ))
            {
                *pindex = i; /* the table index where a free element is available */

                result = R_RESULT_SUCCESS;
                break; /* stop searching */
            }
            else
            {
                /* do nothing */
            }
        }
        else if (((R_MEMORY_STATUS_FREE == mem_mgmt[instance_id].mem_array[i].status)  &&
                  (NULL                 != mem_mgmt[instance_id].mem_array[i].pdata )) &&
                  (size                 <= mem_mgmt[instance_id].mem_array[i].size  ))
        {
            *pindex = i; /* the table index where an element with at least size bytes is available */

            if (size < mem_mgmt[instance_id].mem_array[i].size)
            {
                *premaining_size = (uint16_t)(mem_mgmt[instance_id].mem_array[i].size - size); /* left over bytes */
            }
            else
            {
                /* do nothing */
            }
            result = R_RESULT_SUCCESS;
            break; /* stop searching */ /* allow 2 breaks in for this for loop */
        }
        else
        {
            /* do nothing */
        }
    }

    return result;
}
/******************************************************************************
   End of function  find_free_region
******************************************************************************/

/******************************************************************************
* Function Name:R_MEMORY_Malloc
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
uint8_t* R_MEMORY_Malloc(const uint8_t instance_id,
                         const uint16_t size)
{
    uint8_t* paddr = NULL;
    uint16_t table_idx = 0;
    uint16_t rest_bytes1 = 0;
    uint16_t rest_bytes2 = 0;

    if ((((instance_id >= R_MEMORY_MAX_INSTANCES)  ||
          (NULL == mem_mgmt[instance_id].pmemory)) ||
          (0u == size))                            ||
          (size > mem_mgmt[instance_id].max_mem_size))
    {
        return NULL; /* bad parameters or not enough memory */
    }
    else
    {
        /* do nothing */
    }

    /* find a region of size bytes which is free */
    if (find_free_region(instance_id, size, &table_idx, &rest_bytes1) == R_RESULT_SUCCESS) /* if a region has been found */
    {
        paddr = (uint8_t*)mem_mgmt[instance_id].mem_array[table_idx].pdata; /* return val of this fct is not const */

        mem_mgmt[instance_id].mem_array[table_idx].size   = size;
        mem_mgmt[instance_id].mem_array[table_idx].status = R_MEMORY_STATUS_USED;

        /* if not all size described by this element is used, set the remaining bytes to free */
        if ((0u != rest_bytes1) && (NULL != paddr))
        {
            /* find a location in the memory array which is free */
            if (find_free_region(instance_id, 0, &table_idx, &rest_bytes2) == R_RESULT_SUCCESS)
            {
                mem_mgmt[instance_id].mem_array[table_idx].pdata = paddr + size;
                mem_mgmt[instance_id].mem_array[table_idx].size  = rest_bytes1; /* the rest_bytes1 from 2 calls ago */

                /* the status remains R_MEMORY_STATUS_FREE */
            }
            else
            {
                /* no place in the array for the remaining bytes, will re-assign them during freeing or re-allocation */
                /* Marker 1 */
            }
        }
        else
        {
            /* do nothing */
        }
    }
    else
    {
        /* NULL will be returned */
    }

    return paddr;
}
/******************************************************************************
   End of function  R_MEMORY_Malloc
******************************************************************************/

/******************************************************************************
* Function Name:R_MEMORY_Free
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
void R_MEMORY_Free(const uint8_t instance_id,
                   const uint8_t* ptr)
{
    uint16_t i;

    if ((instance_id < R_MEMORY_MAX_INSTANCES) &&
        (NULL != ptr))
    {
        /* find the address to be freed */
        for (i = 0; i < R_MEMORY_MAX_ELEMENTS; i++)
        {
            if (mem_mgmt[instance_id].mem_array[i].pdata == ptr)
            {
                mem_mgmt[instance_id].mem_array[i].status = R_MEMORY_STATUS_FREE;
                break;
            }
            else
            {
                /* do nothing */
            }
        }

        /* merge consecutive free regions */
        merge_free_regions(instance_id);
    }
    else
    {
        /* do nothing */
    }
}
/******************************************************************************
   End of function  R_MEMORY_Free
******************************************************************************/

/******************************************************************************
* Function Name:merge_free_regions
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
static void merge_free_regions(const uint8_t instance_id)
{
    uint16_t index1;
    uint16_t index2;
    uint16_t i;
    r_boolean_t all_mem_free = R_TRUE;

    if (instance_id >= R_MEMORY_MAX_INSTANCES)
    {
        return; /* bad parameters */
    }
    else
    {
        /* do nothing */
    }

    /* find consecutive free regions in the memory array and merge them as one bigger region */
    while (find_consecutive_free_regions(instance_id, &index1, &index2) == R_RESULT_SUCCESS) /* as long as 2 consecutive regions have been found */
    {
        /* increase region 1 by the size of region 2 */
        mem_mgmt[instance_id].mem_array[index1].size = (uint16_t)(mem_mgmt[instance_id].mem_array[index1].size + 
                                                                  mem_mgmt[instance_id].mem_array[index2].size);

        /* the starting address and status of region 1 remain unchanged */

        /* free array element for region 2 */
        mem_mgmt[instance_id].mem_array[index2].pdata = NULL;
        mem_mgmt[instance_id].mem_array[index2].size  = 0;

        /* the status remains unchanged for region 2 */
    }

    /* in case there are some bytes that could not be addressed in the array at Marker 1, add them to the array again here */
    /* if all the region is free make sure one element in the array contains all memory */
    /* adding of the bytes back to the array is done here only when the whole memory is unused */
    for (i = 0; i < R_MEMORY_MAX_ELEMENTS; i++)
    {
        if (R_MEMORY_STATUS_FREE != mem_mgmt[instance_id].mem_array[i].status)
        {
            all_mem_free = R_FALSE;
            break;
        }
        else
        {
            /* do nothing */
        }
    }

    if (R_TRUE == all_mem_free)
    {
        /* find the element describing the memory region */
        for (i = 0; i < R_MEMORY_MAX_ELEMENTS; i++)
        {
            if ((NULL != mem_mgmt[instance_id].mem_array[i].pdata) && (0u != mem_mgmt[instance_id].mem_array[i].size))
            {
                if (mem_mgmt[instance_id].mem_array[i].size != mem_mgmt[instance_id].max_mem_size)
                {
                    mem_mgmt[instance_id].mem_array[i].size = mem_mgmt[instance_id].max_mem_size;
                }
                else
                {
                    /* do nothing */
                }
                break;
            }
            else
            {
                 /* do nothing */
            }
        }
    }
    else
    {
        /* do nothing */
    }
}
/******************************************************************************
   End of function  merge_free_regions
******************************************************************************/

/******************************************************************************
* Function Name:find_consecutive_free_regions
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
static r_result_t find_consecutive_free_regions(const uint8_t instance_id,
                                                uint16_t* pindex1,
                                                uint16_t* pindex2)
{
    uint16_t i;
    uint16_t j;

    if (((instance_id >= R_MEMORY_MAX_INSTANCES) ||
         (NULL == pindex1))                      ||
         (NULL == pindex2))
    {
        return R_RESULT_FAILED;
    }
    else
    {
        /* do nothing */
    }

    for (i = 0; i < R_MEMORY_MAX_ELEMENTS; i++)
    {
        if (((R_MEMORY_STATUS_FREE == mem_mgmt[instance_id].mem_array[i].status)    &&
             (NULL                 != mem_mgmt[instance_id].mem_array[i].pdata ))   &&
             (0u                   != mem_mgmt[instance_id].mem_array[i].size  ))
        {
            /* find if there is another free region starting just after this one */
            for (j = 0; j < R_MEMORY_MAX_ELEMENTS; j++)
            {
                if ((((R_MEMORY_STATUS_FREE == mem_mgmt[instance_id].mem_array[j].status)   &&
                      (NULL                 != mem_mgmt[instance_id].mem_array[j].pdata ))  &&
                      (0u                   != mem_mgmt[instance_id].mem_array[j].size  ))  &&
                      ((mem_mgmt[instance_id].mem_array[i].pdata + mem_mgmt[instance_id].mem_array[i].size)
                                            == mem_mgmt[instance_id].mem_array[j].pdata ))/* condition for the regions being consecutive */
                {
                    /* found 2 regions which are consecutive */
                    *pindex1 = i; /* region j follows after region i */
                    *pindex2 = j; /* region j follows after region i */

                    return R_RESULT_SUCCESS;
                }
                else
                {
                    /* do nothing */
                }
            }
        }
        else
        {
            /* do nothing */
        }
    }

    return R_RESULT_FAILED; /* did not find any consecutive regions */
}
/******************************************************************************
   End of function  find_consecutive_free_regions
******************************************************************************/

/******************************************************************************
* Function Name:R_MEMORY_Realloc
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
uint8_t* R_MEMORY_Realloc(const uint8_t instance_id,
                          uint8_t* ptr,
                          const uint16_t size)
{
    uint16_t i;
    uint16_t index;
    uint16_t rest_bytes1;
    uint16_t rest_bytes2 = 0;
    r_boolean_t found = R_FALSE;
    uint16_t table_idx;

    if ((((instance_id >= R_MEMORY_MAX_INSTANCES)   ||
          (NULL == mem_mgmt[instance_id].pmemory))  ||
          (NULL == ptr ))                           ||
          (0u   == size))
    {
        return NULL; /* failed, no reallocation */
    }
    else
    {
        /* do nothing */
    }

    /* find the pointer to be reallocated in the list */
    for (i = 0; i < R_MEMORY_MAX_ELEMENTS; i++)
    {
        if (((R_MEMORY_STATUS_USED == mem_mgmt[instance_id].mem_array[i].status)    &&
             (ptr                  == mem_mgmt[instance_id].mem_array[i].pdata ))   &&
             (0u                   != mem_mgmt[instance_id].mem_array[i].size  ))
        {
            index = i;
            found = R_TRUE;
            break;
        }
        else
        {
            /* do nothing */
        }
    }

    if (R_TRUE == found)
    {
        if (size > mem_mgmt[instance_id].mem_array[index].size)
        {
            return NULL; /* no enlarging of the region in this implementation */
        }
        else
        {
            rest_bytes1 = (uint16_t)(mem_mgmt[instance_id].mem_array[index].size - size);

            mem_mgmt[instance_id].mem_array[index].size = size;

            /* address and status remain unchanged */

            if (0u != rest_bytes1)
            {
                /* add a new element to the array */
                  /* find a location in the memory array which is free */
                if (find_free_region(instance_id, 0, &table_idx, &rest_bytes2) == R_RESULT_SUCCESS)
                {
                    mem_mgmt[instance_id].mem_array[table_idx].pdata = ptr + size;
                    mem_mgmt[instance_id].mem_array[table_idx].size  = rest_bytes1; /* the rest_bytes1 from 2 calls ago */

                    /* the status remains R_MEMORY_STATUS_FREE */

                    /* merge consecutive free regions */
                    merge_free_regions(instance_id);
                }
                else
                {
                    /* no place in the array for the remaining bytes, will re-assign them during freeing or re-allocation */
                    /* Marker 1 */
                }
            }

            return ptr; /* return the same location if successful */
        }
    }
    else
    {
        return NULL; /* address to be re-allocated has not been found */
    }
}
/******************************************************************************
   End of function  R_MEMORY_Realloc
******************************************************************************/
