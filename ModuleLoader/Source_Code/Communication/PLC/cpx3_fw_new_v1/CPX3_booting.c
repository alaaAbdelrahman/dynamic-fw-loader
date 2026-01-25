/*
 ============================================================================
 Name        : CPX3_booting.c
 Author      : salah ramoud
 Description : CPX3 booting handle
 Date        : 30/09/2024
 ============================================================================
 */


/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "config.h"
#include "r_typedefs.h"
#include "r_bsp_api.h"
#include "r_app_timer.h"
#include "r_fw_download.h"
#include "r_config.h"
#include "r_c3sap_sys_if.h"
#include "CPX3_booting.h"

#include "v94xx_tmr.h"
#include "GPIO_Interface.h"
#include "GPIO_Config.h"
#include "v94xx_gpio.h"
#include "Timer_Config.h"
#include "v94xx_dma.h"
#include "Flash.h"

/******************************************************************************
Macro definitions
******************************************************************************/

#define COM_BAUDRATECHG (0xA0u)           /* Baudrate change command */
#define COM_BAUDRATECHG_ADV (0xA1u)       /* Advanced Baudrate change command */
#define COM_BAUDRATECHG_RESP_4SEG (0xAAu) /* Baudrate change command response */
#define COM_BAUDRATE_1 (0xC1u)            /* Baudrate command 1 */
#define COM_BAUDRATE_ACCEPT (0xCFu)       /* Baudrate command ACCEPPT */
#define COM_BOOT_END (0xB0u)              /* Boot end command */
#define COM_BOOT_REQUEST (0x80u)          /* Request command */
//#define COM_BOOT_END (0x7Eu)              /* Boot end command */



#define INFO_TABLE_SIZE (80)
#define FW_START_ADDRESS_FLASH_MEMORY (800 * 4096)

/******************************************************************************
                    global variables
******************************************************************************/

CPX3_BootingState CPX3_booting_current_state = CPX3_INIT_READ_INFOTABLE;
uint8_t booting_mode = 0; /* CPX Boot mode state */


/************************DMA variables**************************************** */
extern uint8_t flag_framend;
extern uint8_t DMA_Buffer[1536];
uint16_t DMA_The_last_Index = 0;

/********************* Flash varibles*******************************************/
uint8_t info_table[INFO_TABLE_SIZE] = {0};
uint8_t flash_page_data[256] = {0};
uint32_t Data_seg_location;
uint32_t num_pages_for_segment = 0;
uint32_t segment_size = 0;
uint8_t *ptr_data_sent;  // pointer point to the data that will be sent in R_BSP_UartSend

/*******************UART handle variables **************************************/
UartState uart_Handle_state = UART_READY_TO_RECIEVE;

/*******************monitor variables ******************************************/
extern uint32_t timer_counter_old;
extern uint32_t tick_count;
uint32_t timer_counter_new = 0;
uint16_t time_out=100;



/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


/******************************************************************************
 * Function Name: monitor_Timeout
 * Description :handling time out
 * Arguments : timeout
 * Return Value : UART state 
 ******************************************************************************/
UartState monitor_Timeout(uint8_t timeout)
{

    UartState state = UART_SEND_SUCCESS;

    if (flag_framend == 0)
    {
        timer_counter_new = tick_count;
        if ((timer_counter_new - timer_counter_old) >= timeout)
        {
            // TIMEOUT
            timer_counter_old = 0;
            timer_counter_new = 0;
            state = UART_SEND_TIMEOUT;
            return state;
        }
        // BUSY
        state = UART_SEND_BUSY;
        return state;
    }
    else
    {
        timer_counter_old = 0;
        timer_counter_new = 0;
        state = UART_SEND_SUCCESS;
        return state;
    }
}/* monitor_Timeout */
/******************************************************************************
   End of function  monitor_Timeout
******************************************************************************/


/******************************************************************************
 * Function Name:CPX3_Booting
 * Description :handling CPX3 booting squuence
 * Arguments : void
 * Return Value : void
 ******************************************************************************/
Error_State CPX3_Booting(void)
{
    Error_State ret = SUCCESS;

    if (CPX3_booting_current_state != CPX3_INIT_FINISHED)
    {
        ret = (Error_State)CPX3_Booting_Init(); /*Initialize the booting process*/
        if (ret != SUCCESS)
        {
            return ret;
        }
    }
    else
    {
        ret = CPX3_UART_DataHandler();
        if (ret != SUCCESS)
        {
            return ret;
        }
    }

    return SUCCESS;
}/* CPX3_Booting */
/******************************************************************************
   End of function  CPX3_Booting
******************************************************************************/



/******************************************************************************
 * Function Name:CPX3_Booting_Init
 * Description :handling CPX3 booting init
 * Arguments : void
 * Return Value : void
 ******************************************************************************/
CPX3_BootingState CPX3_Booting_Init(void)
{
    CPX3_BootingState ret = CPX3_INIT_SUCCESS;

    switch (CPX3_booting_current_state)
    {

    case CPX3_INIT_READ_INFOTABLE:

        if (FW_InfoTable_Flash_Read() == FLASH_READ_SUCCESS)
        {
            CPX3_booting_current_state = CPX3_INIT_CHECKSUM;
        }
        else
        {
            CPX3_booting_current_state = CPX3_INIT_READ_INFOTABLE;
            return (CPX3_BootingState)FLASH_READ_INFOTABLE_FAIL;
        }
        break;

    case CPX3_INIT_CHECKSUM:
        if (CHECKSUM_SUCCESS == infotable_checksum(info_table))
        {
            CPX3_booting_current_state = CPX3_INIT_UART_CONFIG;
        }
        else
        {
            CPX3_booting_current_state = CPX3_INIT_READ_INFOTABLE;
            return (CPX3_BootingState)CHECKSUM_FAIL;
        }
        break;

    case CPX3_INIT_UART_CONFIG:
        if (R_RESULT_SUCCESS == R_BSP_ConfigureUart(R_BSP_UART_MCU_CPX, 115200u, 0, 0))
        {
            CPX3_booting_current_state = CPX3_INIT_PLC_RESET;
        }
        else
        {
            CPX3_booting_current_state = CPX3_INIT_UART_CONFIG;
        }
        break;
     case CPX3_INIT_PLC_RESET:
        CPX3_reset(); // after this ready to receive the first byte 0X80
        CPX3_booting_current_state = CPX3_INIT_FINISHED;
        break;

    default:
        // Do Nothing
        break;
    }
    return CPX3_INIT_SUCCESS;
} /* CPX3_Booting_Init */
/******************************************************************************
   End of function  CPX3_Booting_Init
******************************************************************************/


/******************************************************************************
 * Function Name:FW_InfoTable_Flash_Read
 * Description :read infotable from flash
 * Arguments : void
 * Return Value : FLASH_READ_SUCCESS or FLASH_READ_FAIL
 ******************************************************************************/
FlashReadState FW_InfoTable_Flash_Read(void)
{
    uint8_t flash_addr[4];
    ConvertUint32ToBytes(flash_addr, FW_START_ADDRESS_FLASH_MEMORY);

    if (SUCCESS != FLASH_gRead(flash_addr, info_table, INFO_TABLE_SIZE))
    {
        return FLASH_READ_INFOTABLE_FAIL;
    }

    return FLASH_READ_SUCCESS;

} /* FW_InfoTable_Flash_Read */
/******************************************************************************
   End of function  FW_InfoTable_Flash_Read
*******************************************************************************/


/******************************************************************************
 * Function Name:infotable_checksum
 * Description :compute checksum for infotable
 * Arguments : pointer point to infotable
 * Return Value : CHECKSUM_FAIL or CHECKSUM_SUCCESS
 ******************************************************************************/
ChecksumState infotable_checksum(uint8_t *pfirmware_pointer)
{
    uint32_t calc_sum = 0;
    uint32_t table_sum;
    uint32_t table_size;
    uint32_t i;

    if (NULL == pfirmware_pointer)
    {
        return CHECKSUM_FAIL;
    }
    table_sum = Fw_arr2uint32(&pfirmware_pointer[FWDL_INFO_TABLE_CHKSUM_OFFSET]);
    table_size = Fw_arr2uint32(&pfirmware_pointer[FWDL_INFO_TABLE_START_OFFSET]);

    if (table_size <= 256)
    {
        for (i = 0; i < table_size; i++)
        {
            calc_sum += pfirmware_pointer[FWDL_INFO_TABLE_START_OFFSET + i];
        }
        calc_sum = ~calc_sum;

        if (table_sum == calc_sum)
        {
            return CHECKSUM_SUCCESS;
        }
    }

    return CHECKSUM_FAIL;

} /* infotable_checksum */
/******************************************************************************
   End of function  infotable_checksum
******************************************************************************/


/******************************************************************************
 * Function Name:Fw_arr2uint32
 *Description :Function to convert an array of 4 bytes into a uint32_t value
 * Arguments : an array of 4 bytes
 * Return Value :  combined uint32_t value
 ******************************************************************************/
uint32_t Fw_arr2uint32(uint8_t parr[])
{
    uint32_t val;

    if (NULL == parr)
    {
        return 0xFFFFFFFFu;
    }

    val = (((uint32_t)(parr[3]) << 24u) + ((uint32_t)(parr[2]) << 16u)) +
          (((uint32_t)(parr[1]) << 8u) + (uint32_t)(parr[0]));
    return val;
} /* Fw_arr2uint32 */
/******************************************************************************
   End of function  Fw_arr2uint32
******************************************************************************/


/******************************************************************************
 * Function Name:CPX3_reset
 * Description  : Generates CPX reset signal
 * Arguments    : None
 * Return Value : None
 ******************************************************************************/
void CPX3_reset(void)
{
    R_BSP_Cpx3Reset();
} /* CPX3_reset */
/******************************************************************************
   End of function  CPX3_reset
******************************************************************************/


/******************************************************************************
 * Function Name:CPX3_UART_DataHandler
 * Description  : handle receive byte OR continue reading data segment and send it to CPX3
 * Arguments    : None
 * Return Value : None
 ******************************************************************************/
Error_State CPX3_UART_DataHandler(void)
{
    uint32_t size = DMA_GetPackLenTransferred(1) + DMA_GetFrameLenTransferred(1) * 256;
    uint32_t loop_counter = 0;
    Error_State ret = SUCCESS;

    ret = (Error_State)monitor_Timeout(time_out);
    if (ret == (Error_State)UART_SEND_TIMEOUT)
    {
        return (Error_State)UART_SEND_TIMEOUT;
    }
    else if (ret == (Error_State)UART_SEND_BUSY)
    {
        return (Error_State)UART_SEND_BUSY;
    }
    else
    {

        if (uart_Handle_state == UART_READY_TO_RECIEVE)
        {

            while (size != DMA_The_last_Index)
            {

                ret = MCU_RECEIVE_BYTE_Handle(DMA_Buffer[DMA_The_last_Index]);
                DMA_The_last_Index++;
                DMA_The_last_Index = DMA_The_last_Index % 1536;
                if (ret != SUCCESS)
                {
                    return ret;
                }

                loop_counter++;
                if (loop_counter > sizeof(DMA_Buffer))
                {
                    DMA_The_last_Index = DMA_GetPackLenTransferred(1) + DMA_GetFrameLenTransferred(1) * 256;
                    break;
                }
            }
        }
        else 
        {
            ret = (Error_State)CPX3_Booting_FlashRead_UartSend();
            if (ret != SUCCESS)
            {
                return ret;
            }
        }
    }

    return SUCCESS;
} /* CPX3_UART_DataHandler */
/******************************************************************************
   End of function  CPX3_UART_DataHandler
******************************************************************************/


/******************************************************************************
 * Function Name: CPX3_Booting_Send_Infotable
 * Description  : handle data that will be sent to CPX3
 * Arguments    : receive byte from CPX3
 * Return Value : success or failed
 ******************************************************************************/
Error_State CPX3_Booting_Send_Infotable(uint8_t rx_byte)
{
    Error_State ret = SUCCESS;
    uint8_t *ptr_infotable_sector=NULL;
    uint8_t *ptr_start_FW = info_table;
    uint32_t offset_adr = 0;
    static uint8_t segment_number = 0;

    uint8_t *ptr_start_infotable = ptr_start_FW + FWDL_INFO_TABLE_START_OFFSET; // set start of infotable

    segment_number = (uint8_t)(rx_byte & 0x0F); // compute segment number

    if (segment_number > 4)
    {
        return (Error_State)SEGMENT_NUMBER_ERROR;
    }
    // Make pointer point to segment sector in Info table
    ptr_infotable_sector = (uint8_t *)((uint32_t)ptr_start_infotable + (uint32_t)(segment_number << 4));

    if (NULL == ptr_infotable_sector)
    {
        return (Error_State)INFOTABLE_CONTENT_INVALID;
    }

    uart_Handle_state = UART_NOT_READY_TO_RECIEVE; // Dont receive new bytes till handle current byte

    // Setup Info Table to be sent
    offset_adr = Fw_arr2uint32(&ptr_infotable_sector[0]); // first 4 bytes in sector

    /* Compute info table pointer */
    ptr_data_sent = (uint8_t *)&ptr_infotable_sector[4]; // second 4 bytes in sector

    // Address of current data segment
    Data_seg_location = (FW_START_ADDRESS_FLASH_MEMORY + 16 + (uint32_t)offset_adr);

    // size of segment
    segment_size = Fw_arr2uint32(&ptr_infotable_sector[8]); // Third 4 bytes in sector

    // compute number of pages
    num_pages_for_segment = ((segment_size / 256) + 1);

    // Info Table Transmission 
    R_BSP_UartSend(R_BSP_UART_MCU_CPX, ptr_data_sent, 12);
    time_out=100;
    return SUCCESS;

} /* CPX3_Booting_Send_Infotable */
/******************************************************************************
   End of function  CPX3_Booting_Send_Infotable
******************************************************************************/


/******************************************************************************
 * Function Name     : MCU_RECEIVE_BYTE_Handle
 * Description       : UART Rx byte  handling
 * Argument          : rx_byte : the byte that has been received
 * Return Value      : success or failed
 ******************************************************************************/
Error_State MCU_RECEIVE_BYTE_Handle(uint8_t rx_byte)
{
    uint8_t response_buffer[2] = {0};
    uint8_t download_baud = 0xC;
    uint8_t communcation_baud = 0x0C;
    Error_State ret = SUCCESS;

    switch (rx_byte)
    {
    case COM_BAUDRATECHG:
    case COM_BAUDRATE_ACCEPT: // Accept to change baudrate

        // Adjust MCU Baudrate
        R_BSP_UartSetBaudrate(R_BSP_UART_MCU_CPX, 1014706); // need to handle

        response_buffer[0] = COM_BAUDRATECHG_RESP_4SEG;
        ptr_data_sent = response_buffer;
        R_BSP_UartSend(R_BSP_UART_MCU_CPX, ptr_data_sent, 1); /* Start transmit */
        time_out=100;

        break;
    // 2
    case COM_BAUDRATECHG_ADV: // Request to change the baud rate

        response_buffer[0] = COM_BAUDRATE_1;
        // X Continue Baudrate for booting
        // Y Baudrate After booting
        response_buffer[1] = (uint8_t)((download_baud << 4) | communcation_baud);
        ptr_data_sent = (uint8_t *)response_buffer;

        R_BSP_UartSend(R_BSP_UART_MCU_CPX, ptr_data_sent, 2);
        time_out=100;

        break;
    // 4
    case COM_BOOT_END: /* Download finished. Set flag. */
        booting_mode = MODE_BOOT_END;
        return (Error_State)booting_mode;
        break;

    default:
        // 1
        if (COM_BOOT_REQUEST == (uint8_t)(rx_byte & 0xF0))
        {
            ret = CPX3_Booting_Send_Infotable(rx_byte);
            if (ret != SUCCESS)
            {
                return ret;
            }
        }
        else
        {
            /* Undefined command */
        }
        break;
    }

    return SUCCESS;

} /* MCU_RECEIVE_BYTE_Handle */
/******************************************************************************
   End of function  MCU_RECEIVE_BYTE_Handle
******************************************************************************/


/******************************************************************************
 * Function Name     : CPX3_Booting_FlashRead_UartSend
 * Description       : read segment data from the flash and send it
 * Argument          : void
 * Return Value      : success or failed
 ******************************************************************************/
FlashReadState CPX3_Booting_FlashRead_UartSend(void)
{
    FlashReadState flash_read_state = FLASH_READ_SUCCESS;
    uint8_t flash_addr[4];
    static uint32_t page_count = 0;
    static uint32_t address_counter = 0; // address counter of the data segment in flash
    static uint32_t remaining_size = 0;  // remaining size of data segment
    FlashReadState ret = (FlashReadState)SUCCESS;

    ConvertUint32ToBytes(flash_addr, (Data_seg_location + address_counter));
    remaining_size = segment_size - address_counter;
    remaining_size = (remaining_size > 256) ? 256 : remaining_size;
    memset(flash_page_data, 0x00, 256);
    flash_read_state = (FlashReadState)FLASH_gRead(flash_addr, flash_page_data, (uint16_t)remaining_size);

    if (flash_read_state == FLASH_READ_PAGE_FAIL) // Flash read failed
    {
        return FLASH_READ_PAGE_FAIL;
    }

    R_BSP_UartSend(R_BSP_UART_MCU_CPX, flash_page_data, remaining_size); // Info Table Transmission لازم يتهندل هنا
    time_out=100;
    page_count++;
    address_counter += remaining_size;

    if (num_pages_for_segment == page_count)
    {
        page_count = 0;
        uart_Handle_state = UART_READY_TO_RECIEVE;
        address_counter = 0;
        return (FlashReadState)SUCCESS;
    }

    return (FlashReadState)SUCCESS;

} /* CPX3_Booting_FlashRead_UartSend */
/******************************************************************************
   End of function  CPX3_Booting_FlashRead_UartSend
******************************************************************************/
