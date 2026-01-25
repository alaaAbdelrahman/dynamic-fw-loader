
#include "GPIO_Interface.h"

#include "I2C_Interface.h"
#include "config.h"

#ifdef HW_Crypto_Chip
extern volatile uint8_t ATEC_TWI;
extern volatile uint8_t ATEC_WAKE;
#endif
void S_I2C_INIT(uint8_t Slave_Address, uint8_t DevType)
{
  
  I2C_InitType InitStruct ;
  
   /* Initialize the AssertAcknowledge member */  
  InitStruct.AssertAcknowledge = S_I2C_ASSERTACKNOWLEDGE_DISABLE;
  /* Initialize the ClockSource member */
#ifdef HW_Crypto_Chip
  if(!ATEC_WAKE){
#endif
    if(DevType==S_RFID_I2C)
      InitStruct.ClockSource = I2C_CLOCKSOURCE_APBD120;
    else
      InitStruct.ClockSource = S_I2C_CLOCKSOURCE_APBD;
#ifdef HW_Crypto_Chip
  }
  else
    InitStruct.ClockSource = I2C_CLOCKSOURCE_APBD120;
#endif 
  /* Initialize the GeneralCallAck member */  
  InitStruct.GeneralCallAck = S_I2C_GENERALCALLACK_DISABLE;
  /* Initialize the SlaveAddr member */  
  InitStruct.SlaveAddr = Slave_Address;  
  
  //I2C_StructInit(&InitStruct);
  
  S_I2C_DeInit();
  S_I2C_Init(&InitStruct);
  S_I2C_Dis_EN(ENABLE);
  
  
  
}
/**********************************************RFID*********************************************/
uint8_t S_I2C_Write(uint16_t slave_address , uint32_t location, uint8_t* data_buffer,uint16_t data_size ,uint16_t page_size,uint8_t DevType)
{
  uint16_t ret_val ;
  /* to access Device addresses > 0xFFFF, set bit no.1 in slave_address */
  if(location > 0xFFFF)
    slave_address |= 0x02;
  
  I2C_WRType I2C_WRInitStruct ;
  if(DevType==S_EEPROM_I2C)
  {
    I2C_WRInitStruct.SubAddrType = S_I2C_SUBADDR_2BYTE;
#ifndef EEPROM_PAGE_256
    I2C_WRInitStruct.PageRange = 64L;//page_size;
#else
    I2C_WRInitStruct.PageRange = 256L;//page_size;
#endif
  }
  else
  {
    I2C_WRInitStruct.SubAddrType = S_I2C_SUBADDR_1BYTE;
    I2C_WRInitStruct.PageRange = page_size;//page_size;
  }
  I2C_WRInitStruct.SlaveAddr = slave_address;
  I2C_WRInitStruct.Length = data_size;
  I2C_WRInitStruct.pBuffer = data_buffer;
  I2C_WRInitStruct.SubAddress = location;
  I2C_WRInitStruct.device_type=DevType;
  S_I2C_INIT(slave_address, DevType);
  ret_val =  S_I2C_MasterWriteBytes(&I2C_WRInitStruct);
  S_I2C_SendStop();
  if(ret_val)
  {
    return S_FAIL ;
  }
  
  else
  {
    return S_SUCCESS ;
  }
}
uint8_t S_I2C_Read(uint16_t slave_address , uint32_t location, uint8_t* data_buffer,uint16_t data_size ,uint16_t page_size,uint8_t DevType)
{
  uint16_t ret_val ;
  /* to access Device addresses > 0xFFFF, set bit no.1 in slave_address */
  if(location > 0xFFFF)
    slave_address |= 0x02;
  
  I2C_WRType I2C_WRInitStruct ;
  if(DevType==S_EEPROM_I2C)
  {
    I2C_WRInitStruct.SubAddrType = S_I2C_SUBADDR_2BYTE;
    #ifndef EEPROM_PAGE_256
    I2C_WRInitStruct.PageRange = 64L;//page_size;
    #else
    I2C_WRInitStruct.PageRange = 256L;//page_size;
    #endif
    }
    else
    {
    I2C_WRInitStruct.SubAddrType = S_I2C_SUBADDR_1BYTE;
    I2C_WRInitStruct.PageRange = page_size;//page_size;
  }
  I2C_WRInitStruct.SlaveAddr = slave_address;
  I2C_WRInitStruct.Length = data_size;
  I2C_WRInitStruct.pBuffer = data_buffer;
  I2C_WRInitStruct.SubAddress = location;
  I2C_WRInitStruct.device_type=DevType;
  S_I2C_INIT(slave_address, DevType);
  ret_val =  S_I2C_MasterReadBytes(&I2C_WRInitStruct);
  S_I2C_SendStop();
  if(ret_val)
  {
    return S_FAIL ;
  }
  
  else
  {
    return S_SUCCESS ;
  }
}
/*************************************KeyPad****************************************/
uint8_t S_I2C_Key_TX_frame(uint8_t device_address, uint8_t data_len, uint8_t * data_buf)
{
  uint16_t ret_val ;
  
  I2C_WRType I2C_WRInitStruct ;
  
  I2C_WRInitStruct.SubAddrType = S_I2C_SUBADDR_1BYTE;
  I2C_WRInitStruct.SlaveAddr = device_address;
  I2C_WRInitStruct.PageRange = 1;
  I2C_WRInitStruct.Length = data_len-1;
  I2C_WRInitStruct.SubAddress = (uint16_t)data_buf[0];
  I2C_WRInitStruct.pBuffer = &data_buf[1];
  I2C_WRInitStruct.device_type=S_RFID_I2C;
  S_I2C_INIT(device_address, S_RFID_I2C); // Remove this component and put it in Each state of Kepad Task to Increase Speed of RFID.
  ret_val =  S_I2C_MasterWriteBytes(&I2C_WRInitStruct);
  S_I2C_SendStop();
  if(ret_val)
  {
    return S_FAIL ;
  }
  
  else
  {
    return S_SUCCESS ;
  } 
}
uint8_t S_I2C_Key_RX_frame(uint8_t device_address, uint8_t data_len, uint8_t * data_buf)
{
  uint16_t ret_val ;
  
  I2C_WRType I2C_WRInitStruct ;
  
  I2C_WRInitStruct.SubAddrType = S_I2C_SUBADDR_1BYTE;
  I2C_WRInitStruct.SlaveAddr = device_address;
  I2C_WRInitStruct.PageRange = 1;
  I2C_WRInitStruct.Length = data_len;
  I2C_WRInitStruct.SubAddress = (uint16_t)data_buf[0];
  I2C_WRInitStruct.pBuffer = &data_buf[0];
  I2C_WRInitStruct.device_type = S_RFID_I2C;
  S_I2C_INIT(device_address, S_RFID_I2C); // Remove this component and put it in Each state of RFID Task to Increase Speed of RFID.
  ret_val =  S_I2C_MasterReadBytes(&I2C_WRInitStruct);
  S_I2C_SendStop();
  
  if(ret_val)
  {
    return S_FAIL;
  }
  
  else
  {
    return S_SUCCESS;
  }
}  