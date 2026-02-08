/******************************************************************************
* Includes
******************************************************************************/
#include <stdlib.h>
#include "r_typedefs.h"
#include "r_config.h"
#include "r_stdio_api.h"
#include "r_crc32_api.h"
#include "crc32_table.h" // Include the precomputed CRC table

/******************************************************************************
* Macro definitions
******************************************************************************/
#define R_CRC_INITIAL_REMAINDER      (0x00000000uL)
#define R_CRC_FINAL_XOR_VALUE        (0x00000000uL)
#define R_CRC_WIDTH                  (32uL)

/******************************************************************************
* Static variables
******************************************************************************/

/******************************************************************************
* Local function headers
******************************************************************************/
static uint32_t crc_calc_crc32_software(const r_iovec_t iovec[], uint32_t residue, uint8_t iovec_length);

/******************************************************************************
* Function implementations
******************************************************************************/

/******************************************************************************
* Function Name: R_CRC_InitCRC32
* Description : Initializes CRC32 table. No need to recompute at runtime with precomputed table.
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_CRC_InitCRC32(void)
{
    // Initialization is no longer needed if using precomputed table.
    return R_RESULT_SUCCESS;
}

/******************************************************************************
* Function Name: R_CRC_CalcCrc32
* Description : Computes the CRC32 value for the given input.
* Arguments : 
* Return Value : 
******************************************************************************/
uint32_t R_CRC_CalcCrc32(const r_iovec_t iovec[], uint32_t residue, uint8_t iovec_length)
{
    uint32_t crc = 0;

    if (NULL == iovec)
    {
        return 0u;
    }

    crc = crc_calc_crc32_software(iovec, residue, iovec_length);

    return crc ^ R_CRC_FINAL_XOR_VALUE;
}

/******************************************************************************
* Function Name: R_CRC_ValidateCrc32
* Description : Validates the CRC32 value against the received value.
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_CRC_ValidateCrc32(const uint8_t pinput[], uint16_t length)
{
    r_iovec_t iovec[1];
    uint32_t crc_result;
    uint32_t recv_crc;

    iovec[0].paddress = pinput;
    iovec[0].length  = (uint32_t)(length - 4u);

    crc_result = R_CRC_CalcCrc32(iovec, R_CRC_INITIAL_REMAINDER, 1u);

    recv_crc = (uint32_t)(((((uint32_t)pinput[length - 4] << 24u)  +
                            ((uint32_t)pinput[length - 3] << 16u)) +
                            ((uint32_t)pinput[length - 2] <<  8u)) +
                            ((uint32_t)pinput[length - 1]       ));
    if (crc_result == recv_crc)
    {
        return R_RESULT_SUCCESS;
    }
    else
    {
        return R_RESULT_FAILED;
    }
}

/******************************************************************************
* Function Name: crc_calc_crc32_software
* Description : Computes CRC32 using precomputed table.
* Arguments : 
* Return Value : 
******************************************************************************/
static uint32_t crc_calc_crc32_software(const r_iovec_t iovec[], uint32_t residue, uint8_t iovec_length)
{
    uint32_t byte;
    uint8_t  data;
    uint8_t  i;

    if (NULL == iovec)
    {
        return 0u;
    }

    for (i = 0u ; i < iovec_length ; i++)
    {
        for (byte = 0u ; byte < iovec[i].length ; ++byte)
        {
            data    = (uint8_t)((*(iovec[i].paddress + byte)) ^ (residue >> (R_CRC_WIDTH - 8u)));
            residue = crc32_table[data] ^ ((residue & 0x00FFFFFFu) << 0x8u);
        }
    }

    return residue;
}
