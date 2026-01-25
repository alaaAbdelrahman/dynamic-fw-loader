 /**
  ******************************************************************************
  * @file    Driver_ISO7816.h 
  * @author  VT Application Team
  * @version V1.0.0
  * @date    
  * @brief   ISO7816 library.
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
#ifndef __DRIVER_ISO7816_H
#define __DRIVER_ISO7816_H

/* Includes ------------------------------------------------------------------*/
#include "Driver_Common.h"
#include "type_def.h"

/* Macros --------------------------------------------------------------------*/
/* ISO7816 API version */
#define ARM_ISO7816_API_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(0x01, 0x00)

/****** ISO7816 Control Codes *****/

#define ARM_ISO7816_CONTROL_Pos              0
#define ARM_ISO7816_CONTROL_Msk             (0x1FUL << ARM_ISO7816_CONTROL_Pos)

/*----- ISO7816 Control Codes: Mode -----*/
#define ARM_ISO7816_MODE_HALFDUPLEX         (0x01UL << ARM_ISO7816_CONTROL_Pos) ///< ISO7816 Half-duplex transmit/receive; arg = baudrate

/*----- ISO7816 Control Codes: Mode Parameters: Bit Order -----*/
#define ARM_ISO7816_BIT_ORDER_Pos            8
#define ARM_ISO7816_BIT_ORDER_Msk           (0x01UL << ARM_ISO7816_BIT_ORDER_Pos)
#define ARM_ISO7816_MSB_LSB                 (0x00UL << ARM_ISO7816_BIT_ORDER_Pos) ///< Bit order from MSB to LSB (default)
#define ARM_ISO7816_LSB_MSB                 (0x01UL << ARM_ISO7816_BIT_ORDER_Pos) ///< Bit order from LSB to MSB

/*----- ISO7816 Control Codes: Mode Parameters: ACK Length -----*/
#define ARM_ISO7816_ACKLEN_Pos               9
#define ARM_ISO7816_ACKLEN_Msk              (0x01UL << ARM_ISO7816_ACKLEN_Pos)
#define ARM_ISO7816_ACKLEN_1                (0x00UL << ARM_ISO7816_ACKLEN_Pos) ///< ACK Length is 1 bit (default)
#define ARM_ISO7816_ACKLEN_2                (0x01UL << ARM_ISO7816_ACKLEN_Pos) ///< ACK Length is 2 bits

/*----- ISO7816 Control Codes: Mode Parameters: Automatic re-transmit -----*/
#define ARM_ISO7816_AUTO_RETX_Pos            10
#define ARM_ISO7816_AUTO_RETX_Msk           (0x01UL << ARM_ISO7816_AUTO_RETX_Pos)
#define ARM_ISO7816_AUTO_RETX_0             (0x00UL << ARM_ISO7816_AUTO_RETX_Pos) ///< Disable automatic re-transmit mode (default)
#define ARM_ISO7816_AUTO_RETX_1             (0x01UL << ARM_ISO7816_AUTO_RETX_Pos) ///< Enable automatic re-transmit mode 

/*----- ISO7816 Control Codes: Mode Parameters: Automatic re-receive -----*/
#define ARM_ISO7816_AUTO_RERX_Pos            11
#define ARM_ISO7816_AUTO_RERX_Msk           (0x01UL << ARM_ISO7816_AUTO_RERX_Pos)
#define ARM_ISO7816_AUTO_RERX_0             (0x00UL << ARM_ISO7816_AUTO_RERX_Pos) ///< Disable automatic re-receive mode (default)
#define ARM_ISO7816_AUTO_RERX_1             (0x01UL << ARM_ISO7816_AUTO_RERX_Pos) ///< Enable automatic re-receive mode

/*----- ISO7816 Control Codes: Mode Parameters: Parity -----*/
#define ARM_ISO7816_PARITY_Pos               12
#define ARM_ISO7816_PARITY_Msk              (0x01UL << ARM_ISO7816_PARITY_Pos)
#define ARM_ISO7816_PARITY_EVEN             (0x00UL << ARM_ISO7816_PARITY_Pos) ///< Even parity (default)
#define ARM_ISO7816_PARITY_ODD              (0x01UL << ARM_ISO7816_PARITY_Pos) ///< Odd parity

/*----- ISO7816 Control Codes: Mode Parameters: Automatic re-transmit Parameters: Times of re-transmit -----*/
#define ARM_ISO7816_TX_RETRY_Pos             13
#define ARM_ISO7816_TX_RETRY_Msk            (0x03UL << ARM_ISO7816_TX_RETRY_Pos)
#define ARM_ISO7816_TX_RETRY_2              (0x02UL << ARM_ISO7816_TX_RETRY_Pos) ///< Re-transmit times: 2
#define ARM_ISO7816_TX_RETRY_3              (0x03UL << ARM_ISO7816_TX_RETRY_Pos) ///< Re-transmit times: 3

/*----- ISO7816 Control Codes: Mode Parameters: Automatic re-receive Parameters: Times of re-receive -----*/
#define ARM_ISO7816_RX_RETRY_Pos             15
#define ARM_ISO7816_RX_RETRY_Msk            (0x03UL << ARM_ISO7816_RX_RETRY_Pos)
#define ARM_ISO7816_RX_RETRY_2              (0x02UL << ARM_ISO7816_RX_RETRY_Pos) ///< Re-receive times: 2
#define ARM_ISO7816_RX_RETRY_3              (0x03UL << ARM_ISO7816_RX_RETRY_Pos) ///< Re-receive times: 3

/*----- ISO7816 Control Codes: Miscellaneous Controls  -----*/
#define ARM_ISO7816_SET_BAUDRATE            (0x10UL << ARM_ISO7816_CONTROL_Pos) ///< Configure baudrate; arg = baudrate
#define ARM_ISO7816_SET_CLKDIV              (0x11UL << ARM_ISO7816_CONTROL_Pos) ///< Configure ISO7816 clock divider; arg = clock divider ratio
#define ARM_ISO7816_SET_CLKOUTPUT           (0x12UL << ARM_ISO7816_CONTROL_Pos) ///< arg=ENABLE,enable clock output; arg=DISABLE,disable clock output
#define ARM_ISO7816_ABORT_TRANSMIT          (0x13UL << ARM_ISO7816_CONTROL_Pos) ///< Abort current data transmit; arg = NULL
#define ARM_ISO7816_ABORT_RECEIVE           (0x14UL << ARM_ISO7816_CONTROL_Pos) ///< Abort current data receive; arg = NULL
#define ARM_ISO7816_CMD_CONTROL             (0x15UL << ARM_ISO7816_CONTROL_Pos) ///< Enable control; arg = ENABLE: Enable ISO7816; DISABLE: disable ISO7816

/*----- ISO7816 Argument Codes: clock divider ratio  -----*/
#define	ARM_ISO7816_APBD_1	    ISO7816_CLK_CLKDIV_1
#define	ARM_ISO7816_APBD_2	    ISO7816_CLK_CLKDIV_2
#define	ARM_ISO7816_APBD_3	    ISO7816_CLK_CLKDIV_3
#define	ARM_ISO7816_APBD_4	    ISO7816_CLK_CLKDIV_4
#define	ARM_ISO7816_APBD_5	    ISO7816_CLK_CLKDIV_5
#define	ARM_ISO7816_APBD_6	    ISO7816_CLK_CLKDIV_6
#define	ARM_ISO7816_APBD_7	    ISO7816_CLK_CLKDIV_7
#define	ARM_ISO7816_APBD_8	    ISO7816_CLK_CLKDIV_8
#define	ARM_ISO7816_APBD_9	    ISO7816_CLK_CLKDIV_9
#define	ARM_ISO7816_APBD_10	    ISO7816_CLK_CLKDIV_10
#define	ARM_ISO7816_APBD_11	    ISO7816_CLK_CLKDIV_11
#define	ARM_ISO7816_APBD_12	    ISO7816_CLK_CLKDIV_12
#define	ARM_ISO7816_APBD_13	    ISO7816_CLK_CLKDIV_13
#define	ARM_ISO7816_APBD_14	    ISO7816_CLK_CLKDIV_14
#define	ARM_ISO7816_APBD_15	    ISO7816_CLK_CLKDIV_15
#define	ARM_ISO7816_APBD_16	    ISO7816_CLK_CLKDIV_16
#define	ARM_ISO7816_APBD_17	    ISO7816_CLK_CLKDIV_17
#define	ARM_ISO7816_APBD_18	    ISO7816_CLK_CLKDIV_18
#define	ARM_ISO7816_APBD_19	    ISO7816_CLK_CLKDIV_19
#define	ARM_ISO7816_APBD_20	    ISO7816_CLK_CLKDIV_20
#define	ARM_ISO7816_APBD_21	    ISO7816_CLK_CLKDIV_21
#define	ARM_ISO7816_APBD_22	    ISO7816_CLK_CLKDIV_22
#define	ARM_ISO7816_APBD_23	    ISO7816_CLK_CLKDIV_23
#define	ARM_ISO7816_APBD_24	    ISO7816_CLK_CLKDIV_24
#define	ARM_ISO7816_APBD_25	    ISO7816_CLK_CLKDIV_25
#define	ARM_ISO7816_APBD_26	    ISO7816_CLK_CLKDIV_26
#define	ARM_ISO7816_APBD_27	    ISO7816_CLK_CLKDIV_27
#define	ARM_ISO7816_APBD_28	    ISO7816_CLK_CLKDIV_28
#define	ARM_ISO7816_APBD_29	    ISO7816_CLK_CLKDIV_29
#define	ARM_ISO7816_APBD_30	    ISO7816_CLK_CLKDIV_30
#define	ARM_ISO7816_APBD_31	    ISO7816_CLK_CLKDIV_31
#define	ARM_ISO7816_APBD_32	    ISO7816_CLK_CLKDIV_32
#define	ARM_ISO7816_APBD_33	    ISO7816_CLK_CLKDIV_33
#define	ARM_ISO7816_APBD_34     ISO7816_CLK_CLKDIV_34
#define	ARM_ISO7816_APBD_35	    ISO7816_CLK_CLKDIV_35
#define	ARM_ISO7816_APBD_36	    ISO7816_CLK_CLKDIV_36
#define	ARM_ISO7816_APBD_37	    ISO7816_CLK_CLKDIV_37
#define	ARM_ISO7816_APBD_38	    ISO7816_CLK_CLKDIV_38
#define	ARM_ISO7816_APBD_39	    ISO7816_CLK_CLKDIV_39
#define	ARM_ISO7816_APBD_40	    ISO7816_CLK_CLKDIV_40
#define	ARM_ISO7816_APBD_41	    ISO7816_CLK_CLKDIV_41
#define	ARM_ISO7816_APBD_42	    ISO7816_CLK_CLKDIV_42
#define	ARM_ISO7816_APBD_43	    ISO7816_CLK_CLKDIV_43
#define	ARM_ISO7816_APBD_44	    ISO7816_CLK_CLKDIV_44
#define	ARM_ISO7816_APBD_45	    ISO7816_CLK_CLKDIV_45
#define	ARM_ISO7816_APBD_46	    ISO7816_CLK_CLKDIV_46
#define	ARM_ISO7816_APBD_47	    ISO7816_CLK_CLKDIV_47
#define	ARM_ISO7816_APBD_48	    ISO7816_CLK_CLKDIV_48
#define	ARM_ISO7816_APBD_49	    ISO7816_CLK_CLKDIV_49
#define	ARM_ISO7816_APBD_50	    ISO7816_CLK_CLKDIV_50
#define	ARM_ISO7816_APBD_51	    ISO7816_CLK_CLKDIV_51
#define	ARM_ISO7816_APBD_52	    ISO7816_CLK_CLKDIV_52
#define	ARM_ISO7816_APBD_53	    ISO7816_CLK_CLKDIV_53
#define	ARM_ISO7816_APBD_54	    ISO7816_CLK_CLKDIV_54
#define	ARM_ISO7816_APBD_55	    ISO7816_CLK_CLKDIV_55
#define	ARM_ISO7816_APBD_56	    ISO7816_CLK_CLKDIV_56
#define	ARM_ISO7816_APBD_57	    ISO7816_CLK_CLKDIV_57
#define	ARM_ISO7816_APBD_58	    ISO7816_CLK_CLKDIV_58
#define	ARM_ISO7816_APBD_59	    ISO7816_CLK_CLKDIV_59
#define	ARM_ISO7816_APBD_60	    ISO7816_CLK_CLKDIV_60
#define	ARM_ISO7816_APBD_61	    ISO7816_CLK_CLKDIV_61
#define	ARM_ISO7816_APBD_62	    ISO7816_CLK_CLKDIV_62
#define	ARM_ISO7816_APBD_63	    ISO7816_CLK_CLKDIV_63
#define	ARM_ISO7816_APBD_64	    ISO7816_CLK_CLKDIV_64
#define	ARM_ISO7816_APBD_65	    ISO7816_CLK_CLKDIV_65
#define	ARM_ISO7816_APBD_66	    ISO7816_CLK_CLKDIV_66
#define	ARM_ISO7816_APBD_67	    ISO7816_CLK_CLKDIV_67
#define	ARM_ISO7816_APBD_68 	  ISO7816_CLK_CLKDIV_68
#define	ARM_ISO7816_APBD_69	    ISO7816_CLK_CLKDIV_69
#define	ARM_ISO7816_APBD_70	    ISO7816_CLK_CLKDIV_70
#define	ARM_ISO7816_APBD_71	    ISO7816_CLK_CLKDIV_71
#define	ARM_ISO7816_APBD_72	    ISO7816_CLK_CLKDIV_72
#define	ARM_ISO7816_APBD_73	    ISO7816_CLK_CLKDIV_73
#define	ARM_ISO7816_APBD_74	    ISO7816_CLK_CLKDIV_74
#define	ARM_ISO7816_APBD_75	    ISO7816_CLK_CLKDIV_75
#define	ARM_ISO7816_APBD_76	    ISO7816_CLK_CLKDIV_76
#define	ARM_ISO7816_APBD_77	    ISO7816_CLK_CLKDIV_77
#define	ARM_ISO7816_APBD_78	    ISO7816_CLK_CLKDIV_78
#define	ARM_ISO7816_APBD_79	    ISO7816_CLK_CLKDIV_79
#define	ARM_ISO7816_APBD_80	    ISO7816_CLK_CLKDIV_80
#define	ARM_ISO7816_APBD_81	    ISO7816_CLK_CLKDIV_81
#define	ARM_ISO7816_APBD_82	    ISO7816_CLK_CLKDIV_82
#define	ARM_ISO7816_APBD_83	    ISO7816_CLK_CLKDIV_83
#define	ARM_ISO7816_APBD_84	    ISO7816_CLK_CLKDIV_84
#define	ARM_ISO7816_APBD_85	    ISO7816_CLK_CLKDIV_85
#define	ARM_ISO7816_APBD_86	    ISO7816_CLK_CLKDIV_86
#define	ARM_ISO7816_APBD_87	    ISO7816_CLK_CLKDIV_87
#define	ARM_ISO7816_APBD_88	    ISO7816_CLK_CLKDIV_88
#define	ARM_ISO7816_APBD_89	    ISO7816_CLK_CLKDIV_89
#define	ARM_ISO7816_APBD_90	    ISO7816_CLK_CLKDIV_90
#define	ARM_ISO7816_APBD_91	    ISO7816_CLK_CLKDIV_91
#define	ARM_ISO7816_APBD_92	    ISO7816_CLK_CLKDIV_92
#define	ARM_ISO7816_APBD_93	    ISO7816_CLK_CLKDIV_93
#define	ARM_ISO7816_APBD_94	    ISO7816_CLK_CLKDIV_94
#define	ARM_ISO7816_APBD_95	    ISO7816_CLK_CLKDIV_95
#define	ARM_ISO7816_APBD_96	    ISO7816_CLK_CLKDIV_96
#define	ARM_ISO7816_APBD_97	    ISO7816_CLK_CLKDIV_97
#define	ARM_ISO7816_APBD_98	    ISO7816_CLK_CLKDIV_98
#define	ARM_ISO7816_APBD_99	    ISO7816_CLK_CLKDIV_99
#define	ARM_ISO7816_APBD_100	  ISO7816_CLK_CLKDIV_100
#define	ARM_ISO7816_APBD_101	  ISO7816_CLK_CLKDIV_101
#define	ARM_ISO7816_APBD_102	  ISO7816_CLK_CLKDIV_102
#define	ARM_ISO7816_APBD_103	  ISO7816_CLK_CLKDIV_103
#define	ARM_ISO7816_APBD_104	  ISO7816_CLK_CLKDIV_104
#define	ARM_ISO7816_APBD_105	  ISO7816_CLK_CLKDIV_105
#define	ARM_ISO7816_APBD_106	  ISO7816_CLK_CLKDIV_106
#define	ARM_ISO7816_APBD_107	  ISO7816_CLK_CLKDIV_107
#define	ARM_ISO7816_APBD_108	  ISO7816_CLK_CLKDIV_108
#define	ARM_ISO7816_APBD_109	  ISO7816_CLK_CLKDIV_109
#define	ARM_ISO7816_APBD_110	  ISO7816_CLK_CLKDIV_110
#define	ARM_ISO7816_APBD_111	  ISO7816_CLK_CLKDIV_111
#define	ARM_ISO7816_APBD_112	  ISO7816_CLK_CLKDIV_112
#define	ARM_ISO7816_APBD_113	  ISO7816_CLK_CLKDIV_113
#define	ARM_ISO7816_APBD_114	  ISO7816_CLK_CLKDIV_114
#define	ARM_ISO7816_APBD_115	  ISO7816_CLK_CLKDIV_115
#define	ARM_ISO7816_APBD_116	  ISO7816_CLK_CLKDIV_116
#define	ARM_ISO7816_APBD_117	  ISO7816_CLK_CLKDIV_117
#define	ARM_ISO7816_APBD_118	  ISO7816_CLK_CLKDIV_118
#define	ARM_ISO7816_APBD_119	  ISO7816_CLK_CLKDIV_119
#define	ARM_ISO7816_APBD_120	  ISO7816_CLK_CLKDIV_120
#define	ARM_ISO7816_APBD_121	  ISO7816_CLK_CLKDIV_121
#define	ARM_ISO7816_APBD_122	  ISO7816_CLK_CLKDIV_122
#define	ARM_ISO7816_APBD_123	  ISO7816_CLK_CLKDIV_123
#define	ARM_ISO7816_APBD_124	  ISO7816_CLK_CLKDIV_124
#define	ARM_ISO7816_APBD_125	  ISO7816_CLK_CLKDIV_125
#define	ARM_ISO7816_APBD_126	  ISO7816_CLK_CLKDIV_126
#define	ARM_ISO7816_APBD_127	  ISO7816_CLK_CLKDIV_127
#define	ARM_ISO7816_APBD_128	  ISO7816_CLK_CLKDIV_128

/****** ISO7816 Event *****/
#define ARM_ISO7816_EVENT_TRANSMIT_COMPLETE    (1UL << 0) ///< Transmit data complete
#define ARM_ISO7816_EVENT_RECEIVE_COMPLETE     (1UL << 1) ///< Receive data complete
#define ARM_ISO7816_EVENT_RX_OVERFLOW          (1UL << 2) ///< Receive overflow
#define ARM_ISO7816_EVENT_ACK_ERROR            (1UL << 3) ///< Transmit: receive ACK is 0
#define ARM_ISO7816_EVENT_RX_PARITY_ERROR      (1UL << 4) ///< eceive data parity error
#define ARM_ISO7816_EVENT_RETX_FAIL            (1UL << 5) ///< automatic re-transmit mode re-send data fail
#define ARM_ISO7816_EVENT_RERX_FAIL            (1UL << 6) ///< automatic re-receive mode re-receive data fail

/* Current driver status flag definition */
#define ISO7816_INITIALIZED                 (1UL << 0) ///< ISO7816 initialized
#define ISO7816_CONFIGURED                  (1UL << 1) ///< ISO7816 configured
    
/****** ISO7816 specific error codes *****/
#define ARM_ISO7816_ERROR_RETRY_TIME        (ARM_DRIVER_ERROR_SPECIFIC - 1)

/* Types ---------------------------------------------------------------------*/

/**
\brief ISO7816 Driver Capabilities.
*/
typedef struct _ARM_ISO7816_CAPABILITIES {
  uint32_t  half_duplex         : 1; ///< Support half-duplex transmit/receive
  uint32_t  auto_retransmit     : 1; ///< Support automatically re-transmit
  uint32_t  auto_rereceive      : 1; ///< Support automatically re-receive
} ARM_ISO7816_CAPABILITIES;

/**
\brief Pointer to \ref ARM_ISO7816_SignalEvent : Signal ISO7816 Event.
*/
typedef void (*ARM_ISO7816_SignalEvent_t) (uint32_t event);

/**
\brief ISO7816 Status.
*/
typedef struct _ARM_ISO7816_STATUS {
  uint32_t busy               : 1;  ///< ISO7816 Busy flag
  uint32_t rx_overflow        : 1;  ///< Receive overflow
  uint32_t tx_ackerr          : 1;  ///< Transmit: receive ACK is 0
  uint32_t rx_parityerr       : 1;  ///< Receive data parity error
} ARM_ISO7816_STATUS;

/**
\brief ISO7816 Information (Run-time).
*/
typedef struct _ISO7816_INFO {
  ARM_ISO7816_SignalEvent_t cb_event;       //< Event Callback
  ARM_ISO7816_STATUS        status;         //< Status flags
  uint8_t                   state;          //< Current ISO7816 state
  uint32_t                  mode;           //< ISO7816 mode
  uint32_t                  retx_mode : 1;  //< 1:enable automatic re-transmit mode; 0:Disable automatic re-transmit mode
  uint32_t                  rerx_mode : 1;  //< 1:enable automatic re-receive mode; 0:Disable automatic re-receive mode
  uint8_t                   tx_retry;       //< Re-transmit times
  uint8_t                   rx_retry;       //< Re-receive times
}ISO7816_INFO;

/**
\brief ISO7816 Transfer Information (Run-time).
*/
typedef struct _ISO7816_TRANSFER_INFO {
  uint32_t              tx_num;       // Total number of transmit
  uint32_t              rx_num;       // Total number of receive
  uint8_t              *tx_buf;       // Pointer to out data buffer
  uint8_t              *rx_buf;       // Pointer to in data buffer
  uint32_t              tx_cnt;       // Number of data transmit
  uint32_t              rx_cnt;       // Number of data receive
  uint8_t               tx_retry_num; // Re-transmit times
  uint8_t               rx_retry_num; // Re-receive times 
} ISO7816_TRANSFER_INFO;

/**
\brief ISO7816 Resources.
*/
typedef struct {
  ISO7816_TypeDef         *reg;       ///< ISO7816 Peripheral registers interface
  ISO7816_INFO            *info;      ///< ISO7816 information
  ISO7816_TRANSFER_INFO   *xfer;      ///< ISO7816 Transfer information
} const ISO7816_RESOURCES;

/**
\brief Access structure of the ISO7816 Driver.
*/
typedef struct _ARM_DRIVER_ISO7816 {
  
/**
  * @brief  Get ISO7816 driver version.
  * @param  None
  * @retval ARM_DRIVER_VERSION
  */
  ARM_DRIVER_VERSION        (*GetVersion)      (void);                                ///< Pointer to \ref ARM_ISO7816_GetVersion : Get driver version.
  
  
/**
  * @brief  Get ISO7816 driver capabilities.
  * @param  None
  * @retval ARM_ISO7816_CAPABILITIES
  */    
  ARM_ISO7816_CAPABILITIES  (*GetCapabilities) (void);                                ///< Pointer to \ref ARM_ISO7816_GetCapabilities : Get driver capabilities.
  
  
/**
  * @brief  Initialize ISO7816 Interface.
  *
  * @note   This function is called when ISO7816 starts operation and performs the 
  *         following:
  *             - Initializes the resources needed for the ISO7816 interface.
  *             - Registers the ARM_ISO7816_SignalEvent_t callback function.
  *
  * @note   ARM_ISO78160_IRQHandler() or ARM_ISO78161_IRQHandler() will call callback 
  *         function when ISO7816 callback-event happen.
  *         Necessary condition : put ARM_ISO78160_IRQHandler() or ARM_ISO78161_IRQHandler()
  *                               in ISO7816-IRQ function(ISO78160_IRQHandler or 
  *                               ISO78161_IRQHandler)
  *
  * @note   ISO7816 call-back function's parameters:
  *             ARM_ISO7816_EVENT_TRANSMIT_COMPLETE
  *             ARM_ISO7816_EVENT_RECEIVE_COMPLETE
  *             ARM_ISO7816_EVENT_RX_OVERFLOW
  *             ARM_ISO7816_EVENT_ACK_ERROR
  *             ARM_ISO7816_EVENT_RX_PARITY_ERROR
  *             ARM_ISO7816_EVENT_RETX_FAIL
  *             ARM_ISO7816_EVENT_RERX_FAIL
  *
  * @param  cb_event  pointer to the ARM_ADC_SignalEvent_t callback function; 
  *                   use a NULL pointer when no callback signals are required.
  *
  * @retval ARM_DRIVER_OK
  */    
  int32_t                   (*Initialize)      (ARM_ISO7816_SignalEvent_t cb_event);  ///< Pointer to \ref ARM_ISO7816_Initialize : Initialize ISO7816 Interface.
  
  
/**
  * @brief  Uninitialize ISO7816 Interface.
  * @note   This function is called when the middleware component stops operation 
  *         and releases the software.
  * @param  None
  * @retval ARM_DRIVER_OK
  */  
  int32_t                   (*Uninitialize)    (void);                                ///< Pointer to \ref ARM_ISO7816_Uninitialize : De-initialize ISO7816 Interface.  
  
/**
  * @brief  Start sending data to ISO7816 transmitter.
  * @param  [in]data  Pointer to buffer with data to send to ISO7816 transmitter
  *         [in]num   Number of data items to send
  * @retval ARM_DRIVER_ERROR_PARAMETER
  *         ARM_DRIVER_ERROR
  *         ARM_DRIVER_ERROR_BUSY
  *         ARM_DRIVER_OK
  */   
  int32_t                   (*Send)            (const void *data, uint32_t num);      ///< Pointer to \ref ARM_ISO7816_Send : Start sending data to ISO7816 Interface.
  
  
/**
  * @brief  Start receiving data from ISO7816 receiver.
  * @param  [out]data   Pointer to buffer for data to receive from ISO7816 receiver
  *         [in]num     Number of data items to receive
  * @retval ARM_DRIVER_ERROR_PARAMETER
  *         ARM_DRIVER_ERROR
  *         ARM_DRIVER_ERROR_BUSY
  *         ARM_DRIVER_OK
  */   
  int32_t                   (*Receive)         (      void *data, uint32_t num);      ///< Pointer to \ref ARM_ISO7816_Receive : Start receiving data from ISO7816 Interface.
  
  
/**
  * @brief  Get Transmitted data count.
  * @param  None
  * @retval number of data Transmitted
  */   
  uint32_t                  (*GetTxCount)      (void);                                ///< Pointer to \ref ARM_ISO7816_GetTxCount : Get transmitted data count.
  
  
/**
  * @brief  Get Received data count.
  * @param  None
  * @retval number of data Received
  */   
  uint32_t                  (*GetRxCount)      (void);                                ///< Pointer to \ref ARM_ISO7816_GetTxCount : Get received data count.
  
  
/**
  * @brief  Control ISO7816 Interface.  
  *
  * @param  Group1: ISO7816 Base configuration
  *             [in]control:
  *                 Mode:
  *                     ARM_ISO7816_MODE_HALFDUPLEX
  *                 Bit Order:
  *                     ARM_ISO7816_MSB_LSB 
  *                     ARM_ISO7816_LSB_MSB
  *                 ACK Length:
  *                     ARM_ISO7816_ACKLEN_1
  *                     ARM_ISO7816_ACKLEN_2
  *                 Automatic re-transmit:
  *                     ARM_ISO7816_AUTO_RETX_0
  *                     ARM_ISO7816_AUTO_RETX_1
  *                 Automatic re-receive:
  *                     ARM_ISO7816_AUTO_RERX_0
  *                     ARM_ISO7816_AUTO_RERX_1
  *                 Parity:
  *                     ARM_ISO7816_PARITY_EVEN
  *                     ARM_ISO7816_PARITY_ODD
  *                 Times of re-transmit: be valid when ARM_ISO7816_AUTO_RETX_1
  *                     ARM_ISO7816_TX_RETRY_2
  *                     ARM_ISO7816_TX_RETRY_3
  *                 Times of re-receive:  be valid when ARM_ISO7816_AUTO_RERX_1
  *                     ARM_ISO7816_RX_RETRY_2
  *                     ARM_ISO7816_RX_RETRY_3
  *               [in]arg : baudrate
  *
  *         Group2: configure baudrate
  *             [in]control : ARM_ISO7816_SET_BAUDRATE
  *             [in]arg     : baudrate
  *
  *         Group3: Configure Clock divider
  *             [in]control : ARM_ISO7816_SET_CLKDIV
  *             [in]arg     : ARM_ISO7816_APBD_1 ~ ARM_ISO7816_APBD_128
  *
  *         Group4: Clock output enable control
  *             [in]control : ARM_ISO7816_SET_CLKOUTPUT
  *             [in]arg     : ENABLE or DISABLE
  *
  *         Group5: Abort transmit operation
  *             [in]control : ARM_ISO7816_ABORT_TRANSMIT
  *             [in]arg     : NULL
  *
  *         Group6: Abort receive operation
  *             [in]control : ARM_ISO7816_ABORT_RECEIVE
  *             [in]arg     : NULL
  *
  *         Group7: ISO7816 enable control
  *             [in]control : ARM_ISO7816_CMD_CONTROL
  *             [in]arg     : ENABLE or DISABLE
  * 
  * @retval ARM_DRIVER_OK
  *         ARM_DRIVER_ERROR_UNSUPPORTED
  *         ARM_ADC_ERROR_CONVMODE
  *                 
  */   
  int32_t                   (*Control)         (uint32_t control, uint32_t arg);      ///< Pointer to \ref ARM_ISO7816_Control : Control ISO7816 Interface.
  
  
/**
  * @brief  Get ISO7816 status.
  * @param  None
  * @retval ARM_ISO7816_STATUS
  */    
  ARM_ISO7816_STATUS        (*GetStatus)       (void);                                ///< Pointer to \ref ARM_ISO7816_GetStatus : Get ISO7816 status.
} const ARM_DRIVER_ISO7816;

/**
  * @brief  ISO7816 Interrupt handler.
  * @note   Put this function in ISO7816-IRQ function if callback signals are required.
  * @param  None
  * @retval None
  */
void ARM_ISO78160_IRQHandler(void);
void ARM_ISO78161_IRQHandler(void);

extern ARM_DRIVER_ISO7816 Driver_ISO78160;
extern ARM_DRIVER_ISO7816 Driver_ISO78161;

#endif /* __DRIVER_ISO7816_H */

/******************* (C) COPYRIGHT Vango Technologies, Inc *****END OF FILE****/
