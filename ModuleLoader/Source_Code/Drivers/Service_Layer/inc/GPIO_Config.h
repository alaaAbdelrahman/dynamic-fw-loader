/*************************************************************
**************************************************************
**      File Name      : GPIO Config.h                     **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 15/3/2023                          **
**      Description    : this file include the configuration**
**      between the APP and MCAL                            **
**************************************************************
*************************************************************/





#ifndef GPIO_DRV_H_
#define GPIO_DRV_H_




/************************************************************************************************************************
*************************************************************************************************************************
                                 **                   Include section                        **
*************************************************************************************************************************
*************************************************************************************************************************
************************************************************************************************************************/









/************************************************************************************************************************
*************************************************************************************************************************
                                 **                   Enum section                        **
*************************************************************************************************************************
*************************************************************************************************************************
************************************************************************************************************************/
  








/************************************************************************************************************************
*************************************************************************************************************************
                                 **                   #define section                        **
*************************************************************************************************************************
*************************************************************************************************************************
************************************************************************************************************************/


  /*************************************************************
  **************************************************************
  **                   GPIO Read&write config                 **
  **************************************************************
  *************************************************************/
#define S_GPIO_SetBits(Port, PIN) 					GPIOBToF_SetBits(Port, PIN)
#define S_GPIO_ClearBits(Port, PIN)	 			GPIOBToF_ResetBits(Port, PIN)
#define S_GPIOA_SetBits(Port, PIN) 			     GPIOA_SetBits(Port,PIN)
#define S_GPIOA_ClearBits(Port, PIN)	 			GPIOA_ResetBits(Port, PIN)
#define S_GPIO_GetBit(Port, PIN)                       GPIOBToF_ReadInputDataBit(Port, PIN)
#define S_GPIOA_GetBit(Port, PIN)	                    GPIOA_ReadInputDataBit(Port, PIN)
#define S_GPIO_ToggleBits(Port, PIN) 	               toggle_pin(Port, PIN)












/*************************************************************
**************************************************************
**                  Ports Config Section                    **
**************************************************************
*************************************************************/
#define S_PORT_A                      GPIOA           
#define S_PORT_B                      GPIOB
#define S_PORT_C                      GPIOC
#define S_PORT_D                      GPIOD
#define S_PORT_E                      GPIOE
#define S_PORT_F                      GPIOF


/*************************************************************
**************************************************************
**                  PIN Config Section                      **
**************************************************************
*************************************************************/
#define S_GPIO_Pin_0                  GPIO_Pin_0
#define S_GPIO_Pin_1                  GPIO_Pin_1
#define S_GPIO_Pin_2                  GPIO_Pin_2
#define S_GPIO_Pin_3                  GPIO_Pin_3
#define S_GPIO_Pin_4                  GPIO_Pin_4
#define S_GPIO_Pin_5                  GPIO_Pin_5
#define S_GPIO_Pin_6                  GPIO_Pin_6
#define S_GPIO_Pin_7                  GPIO_Pin_7
#define S_GPIO_Pin_8                  GPIO_Pin_8
#define S_GPIO_Pin_9                  GPIO_Pin_9
#define S_GPIO_Pin_10                 GPIO_Pin_10
#define S_GPIO_Pin_11                 GPIO_Pin_11
#define S_GPIO_Pin_12                 GPIO_Pin_12
#define S_GPIO_Pin_13                 GPIO_Pin_13
#define S_GPIO_Pin_14                 GPIO_Pin_14
#define S_GPIO_Pin_15                 GPIO_Pin_15
#define S_GPIO_Pin_All                GPIO_Pin_All

/*************************************************************
**************************************************************
**                  PIN_Mode Config Section                 **
**************************************************************
*************************************************************/

 #define S_GPIO_Mode_INPUT            GPIO_Mode_INPUT      
 #define S_GPIO_Mode_OUTPUT_CMOS      GPIO_Mode_OUTPUT_CMOS
 #define S_GPIO_Mode_OUTPUT_OD        GPIO_Mode_OUTPUT_OD    
 #define S_GPIO_Mode_INOUT_OD         GPIO_Mode_INOUT_OD   
 #define S_GPIO_Mode_INOUT_CMOS       GPIO_Mode_INOUT_CMOS 
 #define S_GPIO_Mode_FORBIDDEN        GPIO_Mode_FORBIDDEN  





/*************************************************************
**************************************************************
**            Dot Matrix LCD Configuration                  **
**************************************************************
*************************************************************/
#define Dot_Matrix_port         S_PORT_D
#define SDA                     S_GPIO_Pin_3 
#define SCL                     S_GPIO_Pin_4
#define A0                      S_GPIO_Pin_5
#define RES                     S_GPIO_Pin_6
#define CS                      S_GPIO_Pin_7
/*SPI Configuration*/
#define SPI_CS        S_GPIO_Pin_9
#define SPI_MOSI      S_GPIO_Pin_12
#define SPI_MISO      S_GPIO_Pin_11
#define SPI_CLK       S_GPIO_Pin_10

#ifdef EM330_BOARD
#define LCD_POWER_PORT          S_PORT_E
#define LCD_POWER_PIN           S_GPIO_Pin_15
#else
#define LCD_POWER_PORT          S_PORT_E
#define LCD_POWER_PIN           S_GPIO_Pin_7
#endif

/**************************************************************/





#if 0
/*************************************************************
**************************************************************
**            Dot Matrix LCD Configuration                  **
**************************************************************
*************************************************************/
#define Dot_Matrix_port         S_PORT_D
#define LCD_BACK_LIGHT_PORT     S_PORT_D
#define LCD_POWER_PORT          S_PORT_D

#define BACK_LIGHT_PIN          S_GPIO_Pin_2
#define SDA                     S_GPIO_Pin_3 
#define SCL                     S_GPIO_Pin_4
#define A0                      S_GPIO_Pin_5
#define RES                     S_GPIO_Pin_6
#define CS                      S_GPIO_Pin_7


#define LCD_POWER_PIN           S_GPIO_Pin_15
/*SPI Configuration*/
#define SPI_CS        S_GPIO_Pin_9
#define SPI_MOSI      S_GPIO_Pin_12
#define SPI_MISO      S_GPIO_Pin_11
#define SPI_CLK       S_GPIO_Pin_10
/**************************************************************/

#endif



/*************************************************************
**************************************************************
**                  SPI Pins Section                        **
**************************************************************
*************************************************************/
#define SPI_2_CS                      S_GPIO_Pin_0
#define SPI_2_CLK                     S_GPIO_Pin_1
#define SPI_2_MISO                    S_GPIO_Pin_2
#define SPI_2_MOSI                    S_GPIO_Pin_3

/*************************************************************
**************************************************************
**               Software SPI Pins Section                  **
**************************************************************
*************************************************************/
#if defined EM130_BOARD

#define HARD_SPI_CS_PORT        S_PORT_C
#define HARD_SPI_CS_PIN	     S_GPIO_Pin_0
#define HARD_SPI_WP_PORT        S_PORT_E
#define HARD_SPI_WP_PIN	     S_GPIO_Pin_11
#define HARD_SPI_CLK_PORT       S_PORT_C
#define HARD_SPI_CLK_PIN       S_GPIO_Pin_1
#define HARD_SPI_MISO_PORT     S_PORT_C
#define HARD_SPI_MISO_PIN      S_GPIO_Pin_2
#define HARD_SPI_MOSI_PORT      S_PORT_C
#define HARD_SPI_MOSI_PIN      S_GPIO_Pin_3


#elif defined EM210_BOARD

#define HARD_SPI_WP_PORT        S_PORT_C
#define HARD_SPI_WP_PIN	     S_GPIO_Pin_0
#define HARD_SPI_CS_PORT        S_PORT_A
#define HARD_SPI_CS_PIN	     S_GPIO_Pin_15
#define HARD_SPI_CLK_PORT       S_PORT_C
#define HARD_SPI_CLK_PIN       S_GPIO_Pin_1
#define HARD_SPI_MISO_PORT     S_PORT_C
#define HARD_SPI_MISO_PIN      S_GPIO_Pin_2
#define HARD_SPI_MOSI_PORT      S_PORT_C
#define HARD_SPI_MOSI_PIN      S_GPIO_Pin_3


#elif defined EM330_BOARD 

#define HARD_SPI_WP_PORT    S_PORT_B
#define HARD_SPI_WP_PIN	   S_GPIO_Pin_8
#define HARD_SPI_CS_PORT    S_PORT_B
#define HARD_SPI_CS_PIN	   S_GPIO_Pin_9
#define HARD_SPI_CLK_PORT   S_PORT_B
#define HARD_SPI_CLK_PIN    S_GPIO_Pin_10
#define HARD_SPI_MOSI_PORT  S_PORT_B
#define HARD_SPI_MOSI_PIN   S_GPIO_Pin_12
#define HARD_SPI_MISO_PORT  S_PORT_B
#define HARD_SPI_MISO_PIN   S_GPIO_Pin_11


#elif defined MTR_THREE_PH   // for old version

#define SOFTSPI_WP_PORT    S_PORT_C
#define SOFTSPI_WP_PIN	S_GPIO_Pin_15

#define SOFTSPI_MISO_PORT  S_PORT_C
#define SOFTSPI_MISO_PIN  S_GPIO_Pin_14

#define SOFTSPI_CS_PORT    S_PORT_C
#define SOFTSPI_CS_PIN	S_GPIO_Pin_13

#define SOFTSPI_CLK_PORT   S_PORT_E
#define SOFTSPI_CLK_PIN  S_GPIO_Pin_10


#define SOFTSPI_MOSI_PORT  S_PORT_E
#define SOFTSPI_MOSI_PIN  S_GPIO_Pin_11





#else

#define SOFTSPI_WP_PORT    S_PORT_B
#define SOFTSPI_WP_PIN	   S_GPIO_Pin_8

#define SOFTSPI_CS_PORT    S_PORT_B
#define SOFTSPI_CS_PIN	   S_GPIO_Pin_9

#define SOFTSPI_CLK_PORT   S_PORT_B
#define SOFTSPI_CLK_PIN    S_GPIO_Pin_10


#define SOFTSPI_MOSI_PORT  S_PORT_B
#define SOFTSPI_MOSI_PIN   S_GPIO_Pin_12


#define SOFTSPI_MISO_PORT  S_PORT_B
#define SOFTSPI_MISO_PIN   S_GPIO_Pin_11

#endif

/*************************************************************
**************************************************************
**                  MODEM  Section                          **
**************************************************************
*************************************************************/
#if (MicroController == Micro_V94XX)
/* Moved to internal according to Excel Sheet */

#define MODEM_TX0_PORT                UART_TX2_PORT                 
#define MODEM_TX0_PIN                 UART_TX2_PIN                  
#define MODEM_RX0_PORT                UART_RX2_PORT                 
#define MODEM_RX0_PIN                 UART_RX2_PIN                  

#else
#define MODEM_TX0_PORT                S_PORT_B
#define MODEM_TX0_PIN                 S_GPIO_Pin_2
#define MODEM_RX0_PORT                S_PORT_A
#define MODEM_RX0_PIN                 S_GPIO_Pin_12
#endif



/*************************************************************
**************************************************************
**                   Power Fail  Section                    **
**************************************************************
*************************************************************/

#ifdef ADE7953_ENABLE
/*! Power fail port.*/
#define PFAIL_PORT                    S_PORT_E
/*! Power fail pin.*/
#define PFAIL_PIN                     S_GPIO_Pin_7
#endif

/*************************************************************
**************************************************************
**              Metering V9340  Section                  **
**************************************************************
*************************************************************/

#define UART_V9340_PORT_TX     S_PORT_B
#define UART_V9340_PORT_RX     S_PORT_A
#define UART_V9340_TX_PIN     S_GPIO_Pin_2
#define UART_V9340_RX_PIN     S_GPIO_Pin_12

/*************************************************************
**************************************************************
**              WakeUp Pin  Section                  **
**************************************************************
*************************************************************/
//#define   WakeUpPin                 S_GPIO_Pin_7




/*************************************************************
**************************************************************
**                 Communication Modem Power                **
**************************************************************
*************************************************************/
#ifdef EM122U_BOARD
#define   Modem_Power_PORT             S_PORT_A
#define   Modem_Power_PIN             S_GPIO_Pin_2
#endif

/*************************************************************
**************************************************************
**                     Metering Reset                       **
**************************************************************
*************************************************************/

#ifdef EM210_BOARD

#define   ADE_Meter_Reset_PORT        S_PORT_A
#define   ADE_Meter_Reset_PIN         S_GPIO_Pin_5

#else

#ifdef EM_DL130_BOARD
#define   ADE_Meter_Reset_PORT        S_PORT_C
#define   ADE_Meter_Reset_PIN         S_GPIO_Pin_13
#else
#define   ADE_Meter_Reset_PORT        S_PORT_A
#define   ADE_Meter_Reset_PIN         S_GPIO_Pin_11
#endif


#endif 


/*************************************************************
**************************************************************
**                    ADE7953_MCU_ChipSelect                **
**************************************************************
*************************************************************/

#define   CS_GPIO_PORT         S_PORT_F /*!<MCU chip select interface port.*/       
#define   CS_GPIO_PIN          S_GPIO_Pin_5 /*!<MCU chip select pin.*/


/*************************************************************
**************************************************************
**               EM110_BOARD Section                        **
**************************************************************
*************************************************************/
#define UART_TX2_PORT                 S_PORT_B
#define UART_TX2_PIN                  S_GPIO_Pin_4
#define UART_RX2_PORT                 S_PORT_A
#define UART_RX2_PIN                  S_GPIO_Pin_14

#if !(MicroController == Micro_V94XX)
#define UART_RX3_PORT                 S_PORT_A
#define UART_RX3_PIN                  S_GPIO_Pin_15
#define UART_TX3_PORT                 S_PORT_B
#define UART_TX3_PIN                  S_GPIO_Pin_5
#endif
/*************************************************************
**************************************************************
**                    KeyPad Section                        **
**************************************************************
*************************************************************/
#define MCU_WAKEUP_CTL_PORT           S_PORT_F
#define MCU_WAKEUP_CTL_PIN            S_GPIO_Pin_0

#ifdef KEYPAD_FEATURE
#define TOUCH_CTL_PORT                S_PORT_A
#define TOUCH_CTL_PIN                 S_GPIO_Pin_5
#endif
//Not Used in our code Ahmed_elkomya


/*************************************************************
**************************************************************
**            DEBUG_HANG_LOOPING Section                    **
**************************************************************
*************************************************************/
#define DEBUG_PORT                    S_PORT_E
#define DEBUG_PIN                     S_GPIO_Pin_1                 


/*************************************************************
**************************************************************
**                  contact card Pins                       **
**************************************************************
*************************************************************/
#ifdef EM330_BOARD
#define CNTCT_CRD_PORT	                     S_PORT_B
#define CNTCT_CRD_DTCT_PORT	             S_PORT_A
#define CRD_DTCT_PIN	                     S_GPIO_Pin_14
#define CRD_RST_PIN	                     S_GPIO_Pin_4
#elif defined (EM130_BOARD)

#define CNTCT_CRD_PORT	             S_PORT_C
#define CNTCT_CRD_DTCT_PORT          S_PORT_C
#define CRD_DTCT_PIN	             S_GPIO_Pin_7
#define CRD_RST_PIN	                  S_GPIO_Pin_5

#else

#define CNTCT_CRD_PORT	             S_PORT_C
#define CNTCT_CRD_DTCT_PORT          S_PORT_C
#define CRD_DTCT_PIN	             S_GPIO_Pin_0
#define CRD_RST_PIN	                  S_GPIO_Pin_2

#endif
/*************************************************************
**************************************************************
**                     RF Pins                            **
**************************************************************
*************************************************************/
#define RF_CTL_PORT                   S_PORT_E
#define RF_CTL1_PIN                   S_GPIO_Pin_14
#define RF_CTL2_PIN                   S_GPIO_Pin_13


//////////////////////////////////////////////////////////


#if defined (SW_SPI) && defined (EM130_BOARD)                  //software spi for single phase 

#define SOFTSPI_WP_PORT          S_PORT_E
#define SOFTSPI_WP_PIN	        S_GPIO_Pin_11
#define SOFTSPI_CS_PORT          S_PORT_C
#define SOFTSPI_CS_PIN	        S_GPIO_Pin_7
#define SOFTSPI_CLK_PORT         S_PORT_C
#define SOFTSPI_CLK_PIN          S_GPIO_Pin_4
#define SOFTSPI_MOSI_PORT        S_PORT_C
#define SOFTSPI_MOSI_PIN         S_GPIO_Pin_6
#define SOFTSPI_MISO_PORT        S_PORT_C
#define SOFTSPI_MISO_PIN         S_GPIO_Pin_5

#ifdef   EM_DL130_BOARD
#define  S_REST_PORT_RF          S_PORT_C
#define  S_RESET_PIN_RF          S_GPIO_Pin_9
#else 
#define  S_REST_PORT_RF          S_PORT_B
#define  S_RESET_PIN_RF          S_GPIO_Pin_3
#endif

#endif

#if defined (SW_SPI) && defined (EM330_BOARD)                  //software spi for three phase 

#define SOFTSPI_WP_PORT          S_PORT_E
#define SOFTSPI_WP_PIN	        S_GPIO_Pin_11
#define SOFTSPI_CS_PORT          S_PORT_A
#define SOFTSPI_CS_PIN	        S_GPIO_Pin_14
#define SOFTSPI_CLK_PORT         S_PORT_D
#define SOFTSPI_CLK_PIN          S_GPIO_Pin_13
#define SOFTSPI_MOSI_PORT        S_PORT_B
#define SOFTSPI_MOSI_PIN         S_GPIO_Pin_5
#define SOFTSPI_MISO_PORT        S_PORT_A
#define SOFTSPI_MISO_PIN         S_GPIO_Pin_15

#define  S_REST_PORT_RF                             S_PORT_E
#define  S_RESET_PIN_RF                           S_GPIO_Pin_14
#endif




#define S_PORT_SoftW_SPI_RF                        SOFTSPI_CS_PORT
#define S_CS_PIN_SoftW_SPI_RF                      SOFTSPI_CS_PIN

#define S_PORT_GPIO_RF                           S_PORT_A
#define  S_PIN_GPIO_0_RF                          S_GPIO_Pin_13
#define  S_PIN_GPIO_1_RF                          S_GPIO_Pin_1   //port B just 
#define  S_PIN_GPIO_2_RF                          S_GPIO_Pin_15

//////////////////////////////////////////////////////////
/***********************Three Phase***************************
**************************************************************
**                       RF Pins                          **
**************************************************************
*************************************************************/
#ifdef MTR_THREE_PH
#define RF_RX_PORT                    S_PORT_B //uart2
#define MCU_RX_RF_RX                  S_GPIO_Pin_4
#define RF_TX_PORT                    S_PORT_A
#define MCU_TX_RF_TX                  S_GPIO_Pin_14
#endif
/***********************EM122U Or Other***********************
**************************************************************
**                 Calibration Port and Pins                **
**************************************************************
*************************************************************/
#if (MicroController == Micro_V94XX) || defined (EM330_BOARD)

#define CALIB_JUMBER_PORT             S_PORT_D
#define CALIB_JUMBER_PIN              S_GPIO_Pin_1

#else 

#define CALIB_JUMBER_PORT             S_PORT_A
#ifdef EM122U_BOARD
#define CALIB_JUMBER_PIN              S_GPIO_Pin_5
#else
#ifndef EM330_BOARD
#define CALIB_JUMBER_PIN              S_GPIO_Pin_3
#endif 
#endif

#endif /* MicroController */
/*************************************************************
**************************************************************
**                    Relay section                         **
**************************************************************
*************************************************************/
#define RELAY_PORT                    S_PORT_E
#define RELAY_CONNECT                 S_GPIO_Pin_8//relay close
#define RELAY_DISCONNECT              S_GPIO_Pin_9/// relay open

#ifdef MTR_INDIRECT
#define SRELAY_PORT_OUT               S_PORT_B //uart2
#define SRELAY_0_CONNECT              S_GPIO_Pin_4
#define S_RELAY_DISCONNECT_PORT       S_PORT_A
#define RELAY_0_DISCONNECT            S_GPIO_Pin_14
#endif
/*************************************************************
**************************************************************
**                 Reactive section                         **
**************************************************************
*************************************************************/





/*************************************************************
**************************************************************
**                 Neutral section                         **
**************************************************************
*************************************************************/
#ifdef EM330_BOARD
#define Missing_Neutral_Port          S_PORT_C
#define Missing_Neutral_Pin           S_GPIO_Pin_12
#endif

/*************************************************************
**************************************************************
**                   BUZZER section                         **
**************************************************************
*************************************************************/
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
#define BUZZER_PORT                   S_PORT_E
#define BUZZER_PIN      	             S_GPIO_Pin_0
#else
#define BUZZER_PORT     	             S_PORT_B
#define BUZZER_PIN       	        S_GPIO_Pin_6
#endif
/*************************************************************
**************************************************************
**                     LED section                          **
**************************************************************
*************************************************************/
#if (MicroController == Micro_V94XX)
#define TMPR_LED_PORT                   S_PORT_C
#define TMPR_LED_PIN          	        S_GPIO_Pin_12
#ifdef EM_DL130_BOARD
#define CRDT_LED_PORT                      S_PORT_A                    
#define CRDT_LED_PIN                       S_GPIO_Pin_1
#else
#define CRDT_LED_PORT                      S_PORT_F                    
#define CRDT_LED_PIN                       S_GPIO_Pin_0
#endif

#else

#define TMPR_LED_PORT                      S_PORT_F
#define TMPR_LED_PIN          	          S_GPIO_Pin_1
#define CRDT_LED_PORT                      S_PORT_F                    
#define CRDT_LED_PIN                       S_GPIO_Pin_0

#endif
//#define LED_PORT            	        S_PORT_F
//#define LED_ERROR_PIN      	        S_GPIO_Pin_1
//#define LED_3_PORT         	        S_PORT_E
//#define LED_3_PIN          	        S_GPIO_Pin_1

/*************************************************************
**************************************************************
**                   Battery section                        **
**************************************************************
*************************************************************/
#define BAT_SENCE_PORT      	        S_PORT_E
#define BAT_SENCE_PIN    	        S_GPIO_Pin_6

/*************************************************************
**************************************************************
**                   Super Cap section                        **
**************************************************************
*************************************************************/
#if (defined (SUPERCAP_FEATURE) &&(defined (EM130_BOARD) || defined (EM330_BOARD) ) )
#define Super_Cap_Enable_PORT      	        S_PORT_E
#define Super_Cap_Enable_PIN    	             S_GPIO_Pin_10
#define Super_Cap_SENCE_PORT      	        S_PORT_E
#define Super_Cap_SENCE_PIN    	        S_GPIO_Pin_7
#endif
/*************************************************************
**************************************************************
**                   Memory section                         **
**************************************************************
*************************************************************/

#define EEPROM_WP_PORT  		        S_PORT_B
#define EEPROM_WP_PIN    	                S_GPIO_Pin_15


/*************************************************************
**************************************************************
**                   BACK LIGHT section                     **
**************************************************************
*************************************************************/

#if (MicroController == Micro_V94XX)
#define LCD_BACK_LIGHT_PORT  	        S_PORT_D
#define BACK_LIGHT_PIN      	        S_GPIO_Pin_2
#else
#define LCD_BACK_LIGHT_PORT  	        S_PORT_E
#define BACK_LIGHT_PIN      	        S_GPIO_Pin_4
#endif 
/*************************************************************
**************************************************************
**            Metering chip for three phase                 **
**************************************************************
*************************************************************/
#if ( defined (MTR_THREE_PH) && !(defined (EM330_BOARD)) )
#define V9203_CS_PORT   	  	        V9203_PORT
#define RF_CTL_PORT      	        S_PORT_E
#define V9203_PORT          	        S_PORT_B

#elif ( defined (MTR_THREE_PH) && (defined (EM330_BOARD)) )
#define RF_CTL_PORT      	        S_PORT_E
#define V9203_CS_PORT   	  	S_PORT_C
#define V9203_PORT          	        S_PORT_C

#endif
/*************************************************************
**************************************************************
**                 Volt DC checking                         **
**************************************************************
*************************************************************/
#define VDCIN_PORT    		        S_PORT_E// power fail
#define VDCIN_PIN      		        S_GPIO_Pin_5
/*************************************************************
**************************************************************
**                Communication PORT D                      **
**************************************************************
*************************************************************/
#define COM_PORT      		        S_PORT_D
#define COM_0_PIN      	   	        S_GPIO_Pin_0
#define COM_1_PIN    		        S_GPIO_Pin_1
#define COM_2_PIN    		        S_GPIO_Pin_2
#define COM_3_PIN    		        S_GPIO_Pin_3
#define COM_4_PIN     		        S_GPIO_Pin_4
#define COM_5_PIN     		        S_GPIO_Pin_5
#define COM_6_PIN     		        S_GPIO_Pin_6
#define COM_7_PIN     		        S_GPIO_Pin_7
/*************************************************************
**************************************************************
**            Three phase segment Screen Section            **
**************************************************************
*************************************************************/
#if (defined(MTR_THREE_PH) && !defined(EM330_BOARD))
#define SEG_PART_C_PORT               S_PORT_C
#define SEG_PART_A_PORT    	        S_PORT_A
#define SEG_PART_B_PORT    	        S_PORT_B
#define SEG_PART_D_PORT    	        S_PORT_D
#define SEG_PART_E_PORT               S_PORT_E

#define SEG_0_PIN      		        S_GPIO_Pin_8
#define SEG_1_PIN       		        S_GPIO_Pin_10
#define SEG_2_PIN         	        S_GPIO_Pin_7
#define SEG_3_PIN      		        S_GPIO_Pin_15
#define SEG_4_PIN      		        S_GPIO_Pin_11
#define SEG_5_PIN       		        S_GPIO_Pin_12
#define SEG_6_PIN        	        S_GPIO_Pin_13
#define SEG_7_PIN      	             S_GPIO_Pin_14
#define SEG_8_PIN       		        S_GPIO_Pin_15
#define SEG_9_PIN       		        S_GPIO_Pin_5
#define SEG_10_PIN     		        S_GPIO_Pin_15
#define SEG_11_PIN      		        S_GPIO_Pin_0
#define SEG_12_PIN      		        S_GPIO_Pin_1
#define SEG_13_PIN      		        S_GPIO_Pin_2
#define SEG_14_PIN       	        S_GPIO_Pin_3
#define SEG_15_PIN    		        S_GPIO_Pin_4
#define SEG_16_PIN    		        S_GPIO_Pin_5
#define SEG_17_PIN    		        S_GPIO_Pin_6
#define SEG_18_PIN    		        S_GPIO_Pin_7
#define SEG_19_PIN    		        S_GPIO_Pin_8
#define SEG_20_PIN    		        S_GPIO_Pin_9
#define SEG_21_PIN     		        S_GPIO_Pin_10
#define SEG_22_PIN     		        S_GPIO_Pin_11
#define SEG_23_PIN     		        S_GPIO_Pin_12
#define SEG_24_PIN     		        S_GPIO_Pin_0
/*************************************************************
**************************************************************
**             Single phase segment Screen Section          **
**************************************************************
*************************************************************/
#elif defined (MTR_SINGLE_PH) || defined (EM330_BOARD)
#define SEG_PART_C_PORT   	        S_PORT_C
#define SEG_PART_A_PORT   	        S_PORT_A
#define SEG_PART_B_PORT   	        S_PORT_B
#define SEG_PART_D_PORT   		   S_PORT_D
#define SEG_PART_E_PORT   		   S_PORT_E
#define SEG_PART_F_PORT  		   S_PORT_F

#define SEG_0_PIN     			   S_GPIO_Pin_2
#define SEG_1_PIN     			   S_GPIO_Pin_3
#define SEG_2_PIN     			   S_GPIO_Pin_8
#define SEG_3_PIN     		   	   S_GPIO_Pin_10
#define SEG_4_PIN    		   	   S_GPIO_Pin_15//S_GPIO_Pin_6
#define SEG_5_PIN     			   S_GPIO_Pin_7
#define SEG_6_PIN    		   	   S_GPIO_Pin_11
#define SEG_7_PIN    			   S_GPIO_Pin_12
#define SEG_8_PIN    			   S_GPIO_Pin_13
#define SEG_9_PIN    			   S_GPIO_Pin_14
#define SEG_10_PIN    			   S_GPIO_Pin_15
#define SEG_11_PIN     			   S_GPIO_Pin_4
#define SEG_12_PIN     			   S_GPIO_Pin_5
#define SEG_13_PIN     			   S_GPIO_Pin_6
#define SEG_14_PIN    			   S_GPIO_Pin_7
#define SEG_15_PIN     			   S_GPIO_Pin_8
#define SEG_16_PIN    			   S_GPIO_Pin_9
#define SEG_17_PIN    			   S_GPIO_Pin_10
#define SEG_18_PIN     			   S_GPIO_Pin_11
#define SEG_19_PIN     			   S_GPIO_Pin_12
#define SEG_20_PIN     			   S_GPIO_Pin_13
#define SEG_21_PIN    			   S_GPIO_Pin_14
#define SEG_22_PIN    			   S_GPIO_Pin_15
#define SEG_23_PIN    			   S_GPIO_Pin_10
#define SEG_24_PIN    			   S_GPIO_Pin_11
#endif
/*************************************************************
**************************************************************
**                   I2C_PORT section                       **
**************************************************************
*************************************************************/
#define I2C_PORT        		        S_PORT_B
#define I2C_DTA_PIN     		        S_GPIO_Pin_14
#define I2C_CLK_PIN     		        S_GPIO_Pin_13

#if (MicroController == Micro_V94XX)
#define I2C_EXTRA_PORT   		   S_PORT_B
#define I2C_EXTRA_PIN    		   S_GPIO_Pin_9
#else
#define I2C_EXTRA_PORT   		   S_PORT_E
#define I2C_EXTRA_PIN    		   S_GPIO_Pin_12

#endif
/*************************************************************
**************************************************************
**                  Switches Section                        **
**************************************************************
*************************************************************/
     /****************Single Phase******************/
#ifdef EM330_BOARD
#define SW_PORT                       S_PORT_A
#define SW_SCROLL_UP_PIN              S_GPIO_Pin_3
#define SW_SCROLL_DOWN_PIN            S_GPIO_Pin_4
#define MD_SW_PIN                     S_GPIO_Pin_6
#define MAG_SW_PIN                    S_GPIO_Pin_8
#else
#define SW_PORT                       S_PORT_A
#define SW_SCROLL_UP_PIN              S_GPIO_Pin_6
#define SW_SCROLL_DOWN_PIN            S_GPIO_Pin_4
#define MD_SW_PIN                     S_GPIO_Pin_2
#define MAG_SW_PIN                    S_GPIO_Pin_8
#define M_SW_PIN                      S_GPIO_Pin_7
#endif


     /****************Three Phase******************/
#ifdef MTR_THREE_PH //#ifdef SPAIN_BOARD
#define TELT_SW_PIN                   S_GPIO_Pin_9 
#define TERMINAL_SW_PIN               S_GPIO_Pin_10
    /*********************************************/
#else
#define TELT_SW_PIN                   S_GPIO_Pin_9

#if (MicroController == Micro_V94XX)
#define TERMINAL_SW_PIN               S_GPIO_Pin_11
#else 
#define TERMINAL_SW_PIN               S_GPIO_Pin_10
#endif
#endif

     /****************ENCLOSER Section******************/
#if defined(MTR_THREE_PH) && !defined(EM330_BOARD)
#define ENCLOSER_COVER_PORT           S_PORT_E
#define ENCLOSER_COVER_PIN           S_GPIO_Pin_0
#define ENCLOSER_SW_PORT              S_PORT_B
#define ENCLOSER_SW_PIN               S_GPIO_Pin_3
#endif
#if defined(MTR_THREE_PH) && defined(EM330_BOARD)
#define ENCLOSER_COVER_PORT           S_PORT_A
#define ENCLOSER_COVER_PIN           S_GPIO_Pin_11
#define ENCLOSER_SW_PORT               S_PORT_E
#define ENCLOSER_SW_PIN                S_GPIO_Pin_2
#define CONTACTOR_PORT               S_PORT_E
#define CONTACTOR_PIN                S_GPIO_Pin_3
#define CONTACTOR_STATUS_PORT         S_PORT_D
#define CONTACTOR_STATUS_PIN      	   S_GPIO_Pin_0
#endif
/*************************************************************
**************************************************************
**                   RFID RESET PIN section                 **
**************************************************************
*************************************************************/
#ifdef MTR_THREE_PH
#ifdef EM330_BOARD
#define RFID_MODULE_ENABLE_PORT       S_PORT_C //IRQ_FCS_PORT
#define RFID_MODULE_ENABLE_PIN        S_GPIO_Pin_9
#else
#define RFID_MODULE_ENABLE_PORT       S_PORT_D //IRQ_FCS_PORT
#define RFID_MODULE_ENABLE_PIN        S_GPIO_Pin_9
#endif

#else
#if defined (EM130_BOARD) && !defined (KEYPAD_FEATURE)
#define RFID_MODULE_ENABLE_PORT       S_PORT_A//IRQ_FCS_PORT
#define RFID_MODULE_ENABLE_PIN        S_GPIO_Pin_5

#else 
#define RFID_MODULE_ENABLE_PORT       S_PORT_D//IRQ_FCS_PORT
#define RFID_MODULE_ENABLE_PIN        S_GPIO_Pin_9
#endif

#endif

/*************************************************************
**************************************************************
**                   Security RESET PIN section             **
**************************************************************
*************************************************************/
#if ( defined (EM130_BOARD) || defined (EM330_BOARD) )  
#define Security_Chip_ENABLE_PORT       S_PORT_C//IRQ_FCS_PORT
#define Security_Chip_ENABLE_PIN        S_GPIO_Pin_10
#endif

/**********************Three Phase****************************
**************************************************************
**                   Metering Section                       **
**************************************************************
*************************************************************/
#if ( defined (MTR_THREE_PH) && !(defined (EM330_BOARD)) )

#define V9203_MISO_PORT               S_PORT_B
#define V9203_MISO_PIN                S_GPIO_Pin_11
#define V9203_MOSI_PORT               S_PORT_B
#define V9203_MOSI_PIN 			   S_GPIO_Pin_12
#define V9203_CS_PIN    		        S_GPIO_Pin_9
#define V9203_CLK_PIN   		        S_GPIO_Pin_10
#elif ( defined (MTR_THREE_PH) && (defined (EM330_BOARD)) )

#define V9203_MISO_PORT                       S_PORT_C
#define V9203_MISO_PIN                        S_GPIO_Pin_2
#define V9203_MOSI_PORT                       S_PORT_C
#define V9203_MOSI_PIN 			      S_GPIO_Pin_3
#define V9203_CS_PIN    		      S_GPIO_Pin_0
#define V9203_CLK_PIN   		      S_GPIO_Pin_1

#endif


#if defined (EM380_BOARD) || defined (EM330_BOARD)
#define MTR_IRQ_PORT                  S_PORT_A
#define MTR_IRQ_PIN                   S_GPIO_Pin_7
#else
#if defined (V9203_ENABLE) || defined (ADE7953_ENABLE)
#define MTR_IRQ_PORT                  S_PORT_A
#define MTR_IRQ_PIN                   S_GPIO_Pin_11
#endif
#endif


/**********************Single Phase***************************
**************************************************************
**                  Internal Metering Section               **
**************************************************************
*************************************************************/
                       /*edit by A_K*/ 
/*
                           S_PORT_B
  CF_Reactive             S_GPIO_Pin_8
                           
                           S_PORT_E 
  CF_Active               S_GPIO_Pin_12

   Internal metering use this pins to accumlate the enegry and get our PULSE    
*/
#define CF_Reactive_PORT       S_PORT_B
#define CF_Reactive_PIN        S_GPIO_Pin_8

#define CF_Active_PORT         S_PORT_E
#define CF_Active_PIN          S_GPIO_Pin_12

/*************************************************************
**************************************************************
**                      Sleep PINs                          **
**************************************************************
*************************************************************/
#if defined (MTR_THREE_PH) && !defined (EM130_BOARD) && !defined(EM210_BOARD)
#define MTR_SLEEP_PORT                S_PORT_B
#define MTR_SLEEP_PIN                 S_GPIO_Pin_8            
#endif

/*************************************************************
**************************************************************
**                 Non Used Pins Section                    **
**************************************************************
*************************************************************/
#define UN_USED_PORTC                 S_PORT_C
#define UN_USED_PORTA                 S_PORT_A
#define UN_USED_PORTB                 S_PORT_B
#define UN_USED_PORTE                 S_PORT_E
#define UN_USED_PIN0                  S_GPIO_Pin_0
#define UN_USED_PIN1                  S_GPIO_Pin_1
#ifdef MTR_THREE_PH
#define UN_USED_PIN2                  S_GPIO_Pin_2
#define UN_USED_PIN3                  S_GPIO_Pin_3

/*#define UN_USED_PORTE                 S_PORT_E
#define UN_USED_PIN11                 S_GPIO_Pin_11
#define UN_USED_PIN10                 S_GPIO_Pin_10
#define UN_USED_PORTC                 S_PORT_C
#define UN_USED_PIN15                 S_GPIO_Pin_15
#define UN_USED_PIN14                 S_GPIO_Pin_14
#define UN_USED_PIN13                 S_GPIO_Pin_13                       
*/
#endif

#define UN_USED_PORTD                 S_PORT_D
#define UN_USED_PIN8                  S_GPIO_Pin_8
#define UN_USED_PIN9                  S_GPIO_Pin_9

#ifdef MTR_SINGLE_PH
#define UN_USED_PIN4                  S_GPIO_Pin_4
#define UN_USED_PIN5                  S_GPIO_Pin_5
#define UN_USED_PIN6                  S_GPIO_Pin_6
#define UN_USED_PIN7                  S_GPIO_Pin_7
#define UN_USED_PIN12                 S_GPIO_Pin_12
#define UN_USED_PIN13                 S_GPIO_Pin_13
#endif


#ifdef MTR_THREE_PH

#define UN_USED_PORTA                 S_PORT_A
#define UN_USED_PIN4                  S_GPIO_Pin_4
#define UN_USED_PIN5                  S_GPIO_Pin_5
#define UN_USED_PIN6                  S_GPIO_Pin_6
#define UN_USED_PIN7                  S_GPIO_Pin_7
#define UN_USED_PIN12                 S_GPIO_Pin_12

#define UN_USED_PIN10                 S_GPIO_Pin_10
#define UN_USED_PIN11                 S_GPIO_Pin_11

#define UN_USED_PIN13                 S_GPIO_Pin_13

#define UN_USED_PORTB                 S_PORT_B
#define UN_USED_PIN2                  S_GPIO_Pin_2
#define UN_USED_PIN3                  S_GPIO_Pin_3
#endif
/*************************************************************
**************************************************************
**                    GPRS Section                          **
**************************************************************
*************************************************************/
#ifdef GPRS_FEATURE



       /****************Three Phase******************/
#ifdef MTR_THREE_PH
#define GPRS_PORT   		        S_PORT_E
#define GPRS_POWER_PIN  		        S_GPIO_Pin_13///////cmntcmntcmnt es2al eslam
#else
#define GPRS_PORT   		        S_PORT_B
#define GPRS_POWER_PIN   	        S_GPIO_Pin_0
#endif
      /*********************************************/
#endif
#ifdef EM130_BOARD
#define    GPIO_M_1_PORT                 S_PORT_C
#define    GPIO_M_1_PIN                  S_GPIO_Pin_13

#define    GPIO_M_2_PORT                 S_PORT_C 
#define    GPIO_M_2_PIN                  S_GPIO_Pin_14

#define    GPIO_M_3_PORT                 S_PORT_C
#define    GPIO_M_3_PIN                  S_GPIO_Pin_15

#define    MBUS_GPIO_M_2_PORT                  S_PORT_A
#define    MBUS_GPIO_M_2_PIN                  S_GPIO_Pin_15

#elif defined (EM330_BOARD)
#define    GPIO_M_1_PORT                 S_PORT_C
#define    GPIO_M_1_PIN                  S_GPIO_Pin_5

#define    GPIO_M_2_PORT                 S_PORT_C 
#define    GPIO_M_2_PIN                  S_GPIO_Pin_4

#define    GPIO_M_3_PORT                 S_PORT_D
#define    GPIO_M_3_PIN                  S_GPIO_Pin_2

#endif
/*************************************************************
**************************************************************
**            RS485 Communication with PLC Section          **
**************************************************************
*************************************************************/
#if (defined(EM122U_BOARD) || defined(EM122_BOARD) || defined (EM130_BOARD) || defined (EM330_BOARD))

#ifdef EM_DL130_BOARD
#define  RS485_ENABLE_Pwr_PORT            S_PORT_E
#define  RS485_ENABLE_Pwr_PIN             S_GPIO_Pin_3
#define RS485_UART_TX_PORT                S_PORT_B
#define RS485_UART_TX_PIN                 S_GPIO_Pin_4
#define RS485_UART_RX_PORT                S_PORT_A
#define RS485_UART_RX_PIN                 S_GPIO_Pin_14
#endif

#define RS485_PORT_OUT                S_PORT_E
#define RS485_PIN              	   S_GPIO_Pin_1
#else
#define RS485_PORT_OUT        	   S_PORT_A
#define RS485_PIN              	   S_GPIO_Pin_7
#endif
#ifndef MTR_THREE_PH
#define PLC_RESET_PORT                S_PORT_E
#define PLC_RESET_PIN                 S_GPIO_Pin_15
#endif
/*************************************************************
**************************************************************
**               AMR Communication Section                  **
**************************************************************
*************************************************************/

#if defined (EM130_BOARD) || defined(EM210_BOARD)
#ifdef EM_DL130_BOARD

#define MCU_RX_AMR_TX_PORT            S_PORT_A
#define MCU_RX_AMR_TX_PIN             S_GPIO_Pin_14
#define MCU_TX_AMR_RX_PORT            S_PORT_B
#define MCU_TX_AMR_RX_PIN             S_GPIO_Pin_4

#else

#define MCU_RX_AMR_TX_PORT            S_PORT_A
#define MCU_RX_AMR_TX_PIN             S_GPIO_Pin_14
#define MCU_TX_AMR_RX_PORT            S_PORT_B
#define MCU_TX_AMR_RX_PIN             S_GPIO_Pin_4
#endif


#else

#define MCU_RX_AMR_TX_PORT            S_PORT_A
#define MCU_RX_AMR_TX_PIN             S_GPIO_Pin_12
#define MCU_TX_AMR_RX_PORT            S_PORT_B
#define MCU_TX_AMR_RX_PIN             S_GPIO_Pin_2
#endif

/*************************************************************
**************************************************************
**            Optical Communication Section                 **
**************************************************************
*************************************************************/
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
#define MCU_RX_OP_TXD_PORT            S_PORT_B
#define MCU_RX_OP_TXD_PIN             S_GPIO_Pin_6
#define MCU_RX_OP_RXD_PORT            S_PORT_B
#define MCU_RX_OP_RXD_PIN             S_GPIO_Pin_0
#else
#define MCU_RX_OP_TXD_PORT            S_PORT_B
#define MCU_RX_OP_TXD_PIN             S_GPIO_Pin_1
#define MCU_RX_OP_RXD_PORT            S_PORT_B
#define MCU_RX_OP_RXD_PIN             S_GPIO_Pin_7
#endif

#ifdef OPTICAL_CTRL_FEATURE
#define Opt_VDD_Ctrl_PORT             S_PORT_C
#define Opt_VDD_Ctrl_PIN              S_GPIO_Pin_8
#endif

/********************************************************************/



#endif
