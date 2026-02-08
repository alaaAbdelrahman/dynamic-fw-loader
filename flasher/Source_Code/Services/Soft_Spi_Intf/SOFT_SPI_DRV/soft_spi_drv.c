/*
 * soft_spi_drv.c
 *
 * Created: 03/05/2018 09:58:30 ص
 *  Author: reda.abdelhameid
 */ 
//#include <compiler.h>
//#include "pio.h"
#include "GPIO_Interface.h"
//#include "pmc.h"
#include "soft_spi_drv.h"
#if defined (SW_SPI)



void softspi_delay (void)
{
	//__NOP();
	//__NOP();
	/*__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();*/
}
//static void softspi_clk_low(void)
//{
//	
//	Reset_bit_GPIO(SOFTSPI_CLK_PORT,SOFTSPI_CLK_PIN);
//	//S_GPIO_Clear_Bits(SOFTSPI_CLK_PORT, SOFTSPI_CLK_PIN);
//	////softspi_delay ();
//}
//
//static void softspi_clk_high(void)
//{
//	Set_bit_GPIO(SOFTSPI_CLK_PORT,SOFTSPI_CLK_PIN);
//	//S_GPIO_Set_Bits(SOFTSPI_CLK_PORT, SOFTSPI_CLK_PIN);
//	//softspi_delay();
//}
//
//static void softspi_mosi_low(void)
//{
//	Reset_bit_GPIO(SOFTSPI_MOSI_PORT, SOFTSPI_MOSI_PIN);
//	//S_GPIO_Clear_Bits(SOFTSPI_CLK_PORT, SOFTSPI_CLK_PIN);
//	//softspi_delay();
//}
//
//static void softspi_mosi_high(void)
//{
//	Set_bit_GPIO(SOFTSPI_MOSI_PORT, SOFTSPI_MOSI_PIN);
//	//S_GPIO_Set_Bits(SOFTSPI_CLK_PORT, SOFTSPI_CLK_PIN);
//	//softspi_delay();
//}

static void softspi_write_bit(uint8_t x, uint8_t m)
{
	Reset_bit_GPIO(SOFTSPI_CLK_PORT,SOFTSPI_CLK_PIN); //softspi_clk_low();
	if (x & m) Set_bit_GPIO(SOFTSPI_MOSI_PORT, SOFTSPI_MOSI_PIN);/*softspi_mosi_high();*/ else Reset_bit_GPIO(SOFTSPI_MOSI_PORT, SOFTSPI_MOSI_PIN);/*softspi_mosi_low();*/
	Set_bit_GPIO(SOFTSPI_CLK_PORT,SOFTSPI_CLK_PIN);//softspi_clk_high();
}


static void softspi_read_bit(uint8_t* x, uint8_t i)
{

	Reset_bit_GPIO(SOFTSPI_CLK_PORT,SOFTSPI_CLK_PIN);//softspi_clk_low();
	Set_bit_GPIO(SOFTSPI_CLK_PORT,SOFTSPI_CLK_PIN);//softspi_clk_high();
	

	
	//if (SOFTSPI_MISO_PIN & SOFTSPI_MISO_MASK) *x |= 1 << i;
	if(Read_bit_GPIO(SOFTSPI_MISO_PORT/*, PIO_TYPE_PIO_INPUT*/, SOFTSPI_MISO_PIN))
	{
				*x |= (1 << i);
	}
	
	
}

void spi_soft_init_master(void)
{	
    /* cs*/
  S_GPIO_Clear_Bits(SOFTSPI_CS_PORT,SOFTSPI_CS_PIN);
  S_GpioInit_PIN(SOFTSPI_CS_PORT, SOFTSPI_CS_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Set_Bits(SOFTSPI_CS_PORT, SOFTSPI_CS_PIN);   
  /* MISO*/
  
  S_GPIO_Clear_Bits(SOFTSPI_MISO_PORT, SOFTSPI_MISO_PIN);
  S_GpioInit_PIN(SOFTSPI_MISO_PORT, SOFTSPI_MISO_PIN ,S_GPIO_Mode_INPUT ); 
  /* MOSI */

  S_GPIO_Clear_Bits(SOFTSPI_MOSI_PORT, SOFTSPI_MOSI_PIN|SOFTSPI_CLK_PIN);
  S_GpioInit_PIN(SOFTSPI_MOSI_PORT, SOFTSPI_MOSI_PIN|SOFTSPI_CLK_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Set_Bits(SOFTSPI_MOSI_PORT, SOFTSPI_MOSI_PIN|SOFTSPI_CLK_PIN); 
 
  /*clk*/
  S_GPIO_Clear_Bits(SOFTSPI_CLK_PORT, SOFTSPI_CLK_PIN);
  S_GpioInit_PIN(SOFTSPI_CLK_PORT, SOFTSPI_CLK_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Set_Bits(SOFTSPI_CLK_PORT, SOFTSPI_CLK_PIN); 
        // pmc_enable_periph_clk(ID_PIOA);	
	// pio_set_output(SOFTSPI_CS_PORT, SOFTSPI_CS_PIN, HIGH, DISABLE, DISABLE);
	 //pio_set_output(SOFTSPI_CLK_PORT, SOFTSPI_CLK_PIN, HIGH, DISABLE, ENABLE);			/*SPI_SCL*/
	 //pio_set_output(SOFTSPI_MOSI_PORT, SOFTSPI_MOSI_PIN, HIGH, DISABLE, ENABLE);			/*SPI MOSI */
	 //pio_set_input(SOFTSPI_MISO_PORT, SOFTSPI_MISO_PIN, PIO_DEFAULT);					/*SPI MISO */

}

void spi_soft_write_byte(uint8_t byte)
{
	uint8_t index;
	for(index=8;index>0;index--)
	{
		softspi_write_bit(byte, (1 << (index-1)));
	}
}

uint8_t spi_soft_read_byte(void)
{
	uint8_t byte = 0,index;
	for(index=8;index>0;index--)
	{
		softspi_read_bit(&byte, index-1);
	}
	return byte;
}

#endif