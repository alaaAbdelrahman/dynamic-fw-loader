
/*
 ============================================================================
 Name        : CPX3_booting.h
 Author      : salah ramoud
 Description : Header file for CPX3 booting  
 Date        : 30/09/2024
 ============================================================================
 */




#ifndef CPX3_BOOTING_H_
#define CPX3_BOOTING_H_


/*****************************************************************************
 *                         Types Declarations 
******************************************************************************/


/* Rradflash States Enum */
typedef enum {
    FLASH_READ_SUCCESS=0,      
    FLASH_READ_PAGE_FAIL=0x11,            
    FLASH_READ_INFOTABLE_FAIL
} FlashReadState;



/* Checksum States Enum */
typedef enum {
    CHECKSUM_SUCCESS=0,         
    CHECKSUM_FAIL=21
} ChecksumState;



/* Checksum States Enum */
typedef enum {
    SUCCESS = 0,
    SEGMENT_NUMBER_ERROR,
    INFOTABLE_CONTENT_INVALID,
} Error_State;



/* Booting States Enum */
typedef enum {
    CPX3_INIT_SUCCESS=0,
    CPX3_INIT_READ_INFOTABLE ,
    CPX3_INIT_CHECKSUM ,
    CPX3_INIT_UART_CONFIG ,
    CPX3_INIT_PLC_RESET,
    CPX3_INIT_FINISHED

} CPX3_BootingState;



/* UART States Enum */
typedef enum {

    UART_SEND_SUCCESS=0,
    UART_NOT_READY_TO_RECIEVE,
    UART_READY_TO_RECIEVE,
    UART_SEND_BUSY =0xF7u,
    UART_SEND_TIMEOUT
} UartState;



#define MODE_BOOT_END     (0xffu) /* CPX download mode end */




/******************************From Renasas ***********************************/

#include "r_typedefs.h"
#define FWDL_INFO_TABLE_CHKSUM_OFFSET   (0x0Cu)        /* Offset of info table checksum */
#define FWDL_INFO_TABLE_START_OFFSET    (0x10u)        /* Offset of info table */



/******************************************************************************
 *                          Function Prototypes
 ******************************************************************************/


/*
 * Description :Function to handle CPX3 booting squuence
 */
Error_State CPX3_Booting(void);

/*
* Description :Function to reset CPX3 module
*/
void CPX3_reset (void);

/*
 * Description :Function to convert an array of 4 bytes into a uint32_t value
 */
uint32_t Fw_arr2uint32 ( uint8_t parr[]);

/*
* Description :Function to compute checksum for infotable
*/
ChecksumState infotable_checksum (uint8_t * pfirmware_pointer);

/* 
* Description :Function to handle UART Rx byte  handling
*/
Error_State MCU_RECEIVE_BYTE_Handle( uint8_t rx_byte);

/*
 * Description : function to read segment data from the flash and send it 
 */
FlashReadState CPX3_Booting_FlashRead_UartSend(void);

/*
 *Description :function to read infotable from flash 
 */
FlashReadState FW_InfoTable_Flash_Read(void);

/*
* Description :function to handle receive byte OR continue reading data segment and send it to CPX3
*/
Error_State CPX3_UART_DataHandler(void);

/*
* Description :function to handle  CPX3 booting init
*/
CPX3_BootingState CPX3_Booting_Init(void);




#endif /* CPX3_BOOTING_H_ */
