#include "config.h"
#if (MicroController == Micro_V85XX)
/**
  ******************************************************************************
  * @file    Driver_ISO7816.c 
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

/* Includes ------------------------------------------------------------------*/
#include "v85XX.h"
#include "v85xx_clk.h"
#include "Driver_ISO7816.h"
# define ISO7816_MODULE_ENABLED
#ifdef ISO7816_MODULE_ENABLED

/* Private define/constants --------------------------------------------------*/

#define   ISO7816_INFO_RW_MASK            (0x13UL)
#define   ISO7816_INFO_RC_MASK            (0xECUL)

/* ISO7816 driver version */
#define ARM_ISO7816_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(3, 0)

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
  ARM_ISO7816_API_VERSION,
  ARM_ISO7816_DRV_VERSION
};

/* ISO7816 Driver Capabilities */
static const ARM_ISO7816_CAPABILITIES DriverCapabilities = {
  1, /* Support half-duplex transmit/receive */
  1, /* Support automatically re-transmit */
  1  /* Support automatically re-receive */
};

/* ISO78160 Resources */
static ISO7816_INFO ISO78160_Info = { 0 };
static ISO7816_TRANSFER_INFO ISO78160_Xfer;
static ISO7816_RESOURCES ISO78160_Resources =
{
  ISO78160,
  &ISO78160_Info,
  &ISO78160_Xfer
};

/* ISO78161 Resources */
static ISO7816_INFO ISO78161_Info = { 0 };
static ISO7816_TRANSFER_INFO ISO78161_Xfer;
static ISO7816_RESOURCES ISO78161_Resources =
{
  ISO78161,
  &ISO78161_Info,
  &ISO78161_Xfer
};

volatile uint32_t ctrl_msk = 0;
/* Static functions ----------------------------------------------------------*/

/**
  \fn          ARM_DRIVER_VERSION ARM_ISO7816_GetVersion(void)
  \brief       Get ISO7816 driver version.
  \return      \ref ARM_DRV_VERSION
*/
static ARM_DRIVER_VERSION ARM_ISO7816_GetVersion(void)
{
  return DriverVersion;
}

/**
  \fn          ARM_ISO7816_CAPABILITIES ARM_ISO7816_GetCapabilities(void)
  \brief       Get driver capabilities.
  \return      \ref ARM_ISO7816_CAPABILITIES
*/
static ARM_ISO7816_CAPABILITIES ARM_ISO7816_GetCapabilities(void)
{
  return DriverCapabilities;
}

/**
  \fn          int32_t ARM_ISO7816_Initialize(ARM_ISO7816_SignalEvent_t cb_event, ISO7816_RESOURCES *iso7816)
  \brief       Initialize ISO7816 Interface.
  \param[in]   cb_event  Pointer to \ref ARM_ISO7816_SignalEvent
  \param[in]   iso7816   Pointer to ISO7816 resources
  \return      \ref execution_status
*/
static int32_t ARM_ISO7816_Initialize(ARM_ISO7816_SignalEvent_t cb_event, ISO7816_RESOURCES *iso7816)
{
  /* Initialize ISO7816 Run-Time Resources */
  iso7816->info->cb_event            = cb_event;
  iso7816->info->status.busy         = 0;
  iso7816->info->status.rx_overflow  = 0;
  iso7816->info->status.rx_parityerr = 0;
  iso7816->info->status.tx_ackerr    = 0;
  
  /* Clear transfer information */ 
  memset(iso7816->xfer, 0, sizeof(ISO7816_TRANSFER_INFO));
  
  /* Enable Clock of ISO7816 */
  if (iso7816 == &ISO78160_Resources)
  {
    CLK_APBPeriphralCmd(CLK_APBPERIPHRAL_ISO78160, ENABLE);
  }
  else
  {
    CLK_APBPeriphralCmd(CLK_APBPERIPHRAL_ISO78161, ENABLE);
  }
  
  /* ISO7816 state: initialized */
  iso7816->info->state = ISO7816_INITIALIZED;
    
  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t ARM_ISO7816_Uninitialize(ISO7816_RESOURCES *iso7816)
  \brief       Uninitialize ISO7816 Interface.
  \param[in]   iso7816     Pointer to ISO7816 resources
  \return      \ref execution_status
*/
static int32_t ARM_ISO7816_Uninitialize(ISO7816_RESOURCES *iso7816)
{
  /* Disable ISO7816 */
  iso7816->reg->CFG &= ~ISO7816_CFG_EN;
  iso7816->reg->CLK &= ~ISO7816_CLK_CLKEN;
 /* Clear transfer information */ 
  memset(iso7816->xfer, 0, sizeof(ISO7816_TRANSFER_INFO));
  memset(iso7816->info, 0, sizeof(ISO7816_INFO));
  
  return ARM_DRIVER_OK;
}


/**
  \fn          ARM_ISO7816_Send(const void *data, uint32_t num, ISO7816_RESOURCES *iso7816)
  \brief       Start sending data to ISO7816 transmitter.
  \param[in]   data     Pointer to buffer with data to send to ISO7816 transmitter
  \param[in]   num      Number of data items to send
  \param[in]   iso7816  Pointer to ISO7816 resources
  \return      \ref execution_status
*/
static int32_t ARM_ISO7816_Send(const void *data, uint32_t num, ISO7816_RESOURCES *iso7816)
{
  /* Parameter check */
  if ((data == NULL) || (num == 0))                 return ARM_DRIVER_ERROR_PARAMETER;
  if (!(iso7816->info->state & ISO7816_CONFIGURED)) return ARM_DRIVER_ERROR;
  if (iso7816->info->status.busy)                   return ARM_DRIVER_ERROR_BUSY;
  
  iso7816->info->status.busy      = 1;
  iso7816->info->status.tx_ackerr = 0;
  
  iso7816->xfer->tx_buf = (uint8_t *)data;
  iso7816->xfer->rx_buf = NULL;
  
  iso7816->xfer->tx_num = num;
  iso7816->xfer->tx_cnt = 0;
  
  iso7816->xfer->tx_retry_num = iso7816->info->tx_retry;
  
  /* Enable transmit interrupt */
  iso7816->reg->CFG |= ISO7816_CFG_SDIE;
  iso7816->reg->INFO |= ISO7816_INFO_SDIF;
  
  /* Send first data, enter interrupt service */
  iso7816->xfer->tx_cnt = 1;
  iso7816->reg->DATA = *(iso7816->xfer->tx_buf++);
  
  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t ARM_ISO7816_Receive(void *data, uint32_t num, ISO7816_RESOURCES *iso7816)
  \brief       Start receiving data from ISO7816 receiver.
  \param[out]  data  Pointer to buffer for data to receive from ISO7816 receiver
  \param[in]   num      Number of data items to receive
  \param[in]   iso7816  Pointer to ISO7816 resources
  \return      \ref execution_status
*/
static int32_t ARM_ISO7816_Receive(void *data, uint32_t num, ISO7816_RESOURCES *iso7816)
{
  /* Parameter check */
  if ((data == NULL) || (num == 0))                 return ARM_DRIVER_ERROR_PARAMETER;
  if (!(iso7816->info->state & ISO7816_CONFIGURED)) return ARM_DRIVER_ERROR;
  if (iso7816->info->status.busy)                   return ARM_DRIVER_ERROR_BUSY;
  
  iso7816->info->status.busy         = 1;
  iso7816->info->status.rx_overflow  = 0;
  iso7816->info->status.rx_parityerr = 0;
  
  iso7816->xfer->tx_buf = NULL;
  iso7816->xfer->rx_buf = (uint8_t *)data;
  
  iso7816->xfer->rx_num = num;
  iso7816->xfer->rx_cnt = 0; 
  
  iso7816->xfer->rx_retry_num = iso7816->info->rx_retry; 
  
  /* Enable receive interrupt */
  iso7816->reg->CFG |= ISO7816_CFG_RCIE;
  iso7816->reg->INFO |= ISO7816_INFO_RCIF;
  
  /* Enable overflow interrupt */
  iso7816->reg->CFG |= ISO7816_CFG_OVIE;
  iso7816->reg->INFO |= ISO7816_INFO_OVIF;
  
  return ARM_DRIVER_OK;
}

/**
  \fn          uint32_t ARM_ISO7816_GetTxCount(ISO7816_RESOURCES *iso7816)
  \brief       Get Transmit data count.
  \param[in]   iso7816  Pointer to ISO7816 resources
  \return      number of data Transmit
*/
static uint32_t ARM_ISO7816_GetTxCount(ISO7816_RESOURCES *iso7816)
{
  if (!(iso7816->info->state & ISO7816_CONFIGURED)) return 0;
  
  return (iso7816->xfer->tx_cnt);
}

/**
  \fn          uint32_t ARM_ISO7816_GetRxCount(ISO7816_RESOURCES *iso7816)
  \brief       Get Receive data count.
  \param[in]   iso7816  Pointer to ISO7816 resources
  \return      number of data Receive
*/
static uint32_t ARM_ISO7816_GetRxCount(ISO7816_RESOURCES *iso7816)
{
  if (!(iso7816->info->state & ISO7816_CONFIGURED)) return 0;
  
  return (iso7816->xfer->rx_cnt);
}

/**
  \fn          int32_t ARM_ISO7816_Control(uint32_t control, uint32_t arg, ISO7816_RESOURCES *iso7816)
  \brief       Control ISO7816 Interface.
  \param[in]   control  Operation
  \param[in]   arg         Argument of operation (optional)
  \param[in]   iso7816     Pointer to ISO7816 resources
  \return      common \ref execution_status and driver specific \ref iso7816_execution_status
*/
static int32_t ARM_ISO7816_Control(uint32_t control, uint32_t arg, ISO7816_RESOURCES *iso7816)
{
  uint32_t pclk;
  uint32_t div;
  ctrl_msk = (uint32_t)(control & (uint32_t)0x1F);
  /* Abort current data transmit */
  if ((control & ARM_ISO7816_CONTROL_Msk) == ARM_ISO7816_ABORT_TRANSMIT)
  {
    /* Disable ISO7816 */
   // iso7816->reg->CFG &= ~ISO7816_CFG_EN;
    /* Disable transmit interrupt */
    iso7816->reg->CFG &= ~ISO7816_CFG_SDIE;
    iso7816->reg->INFO |= ISO7816_INFO_SDIF;
    /* Clear transfer information */ 
    memset(iso7816->xfer, 0, sizeof(ISO7816_TRANSFER_INFO));
    iso7816->info->status.busy = 0;
    
    /* Enable ISO7816 */
    //iso7816->reg->CFG |= ISO7816_CFG_EN;
    return ARM_DRIVER_OK;
  }
  
  /* Abort current data receive */
  if ((control & ARM_ISO7816_CONTROL_Msk) == ARM_ISO7816_ABORT_RECEIVE)
  {
    /* Disable ISO7816 */
    // iso7816->reg->CFG &= ~ISO7816_CFG_EN;
    /* Disable receive interrupt */
    iso7816->reg->CFG &= ~ISO7816_CFG_RCIE;
	iso7816->reg->INFO |= ISO7816_INFO_RCIF;
    
    /* Clear transfer information */ 
    memset(iso7816->xfer, 0, sizeof(ISO7816_TRANSFER_INFO));
    iso7816->info->status.busy = 0;
    
    /* Enable ISO7816 */
    //iso7816->reg->CFG |= ISO7816_CFG_EN;
    return ARM_DRIVER_OK;
  }
  
  if (iso7816->info->status.busy)   return ARM_DRIVER_ERROR_BUSY;

  switch (control & ARM_ISO7816_CONTROL_Msk)
  {
	  default:
		  return ARM_DRIVER_ERROR_UNSUPPORTED;
	  
  /* Mode: half-duplex */
	  case ARM_ISO7816_MODE_HALFDUPLEX:
		  iso7816->info->mode = ARM_ISO7816_MODE_HALFDUPLEX;
		  break;
	  
  /* Configure baudrate */
	  case ARM_ISO7816_SET_BAUDRATE:
		  /* Get PCLK */
		  pclk = CLK_GetPCLKFreq();
		  
		  div = 0x10000 - (pclk / arg);
		  iso7816->reg->BAUDDIVH = (div >> 8) & ISO7816_BAUDDIVH;
		  iso7816->reg->BAUDDIVL = div & ISO7816_BAUDDIVL;
		  return ARM_DRIVER_OK;
	  
  /* Configure ISO7816 clock divider ratio */
	  case ARM_ISO7816_SET_CLKDIV:
		  if (!(arg & ISO7816_CLK_CLKDIV))    return ARM_DRIVER_ERROR_PARAMETER;
		  iso7816->reg->CLK &= ~ISO7816_CLK_CLKDIV;
		  iso7816->reg->CLK |= arg;
		  return ARM_DRIVER_OK;
	  
  /* Control ISO7816 clock output enable */
	  case ARM_ISO7816_SET_CLKOUTPUT:
		  if      (arg == ENABLE)    iso7816->reg->CLK |= ISO7816_CLK_CLKEN;
		  else if (arg == DISABLE)   iso7816->reg->CLK &= ~ISO7816_CLK_CLKEN;
		  else                       return ARM_DRIVER_ERROR_PARAMETER;
		  return ARM_DRIVER_OK;
	  
  /* Enable control */
	  case ARM_ISO7816_CMD_CONTROL:
		  if (arg == ENABLE)
			  iso7816->reg->CFG |= ISO7816_CFG_EN;
		  else if (arg == DISABLE)
			  iso7816->reg->CFG &= ~ISO7816_CFG_EN;
		  else
			  return ARM_DRIVER_ERROR_PARAMETER;
		  return ARM_DRIVER_OK;
  }
  
  /* Configure Bit Order */
  if ((control & ARM_ISO7816_BIT_ORDER_Msk) == ARM_ISO7816_MSB_LSB)
  {
    /* From MSB to LSB */
    iso7816->reg->INFO &= ~ISO7816_INFO_LSB;
  }
  else
  {
    /* From LSB to MSB */
    iso7816->reg->INFO |= ISO7816_INFO_LSB;
  }
  
  /* Configure ACK Length */
  if ((control & ARM_ISO7816_ACKLEN_Msk) == ARM_ISO7816_ACKLEN_1)
  {
    /* ACK Length: 1 bit */
    iso7816->reg->CFG &= ~ISO7816_CFG_ACKLEN;
  }
  else
  {
    /* ACK Length: 2 bits */
    iso7816->reg->CFG |= ISO7816_CFG_ACKLEN;
  }
  
  /* Control Automatic re-transmit */
  if ((control & ARM_ISO7816_AUTO_RETX_Msk) == ARM_ISO7816_AUTO_RETX_0)
  {
    /* Disable Automatic re-transmit */
    iso7816->info->retx_mode = 0;
    iso7816->info->tx_retry = 0;
    iso7816->reg->CFG &= ~ISO7816_CFG_AUTOSD;
  }
  else
  {
    /* Enable Automatic re-transmit */
    iso7816->info->retx_mode = 1;
    iso7816->reg->CFG |= ISO7816_CFG_AUTOSD; 
    
    /* Configure times of re-transmit */
    if ((control & ARM_ISO7816_TX_RETRY_Msk) == ARM_ISO7816_TX_RETRY_2)   
      iso7816->info->tx_retry = 2;
    else if ((control & ARM_ISO7816_TX_RETRY_Msk) == ARM_ISO7816_TX_RETRY_3)   
      iso7816->info->tx_retry = 3;
    else                                                                       
      return ARM_ISO7816_ERROR_RETRY_TIME;
  }
  
  /* Control Automatic Re-receive */
  if ((control & ARM_ISO7816_AUTO_RERX_Msk) == ARM_ISO7816_AUTO_RETX_0)
  {
    /* Disable Automatic re-receive */
    iso7816->info->rerx_mode = 0;
    iso7816->info->rx_retry = 0;
    iso7816->reg->CFG &= ~ISO7816_CFG_AUTORC;
  }
  else
  {
    /* Enable Automatic re-receive */
    iso7816->info->rerx_mode = 1;
    iso7816->reg->CFG |= ISO7816_CFG_AUTORC;
    
    /* Configure times of re-receive */
    if ((control & ARM_ISO7816_RX_RETRY_Msk) == ARM_ISO7816_RX_RETRY_2)
      iso7816->info->rx_retry = 2;
    else if ((control & ARM_ISO7816_RX_RETRY_Msk) == ARM_ISO7816_RX_RETRY_3)
      iso7816->info->rx_retry = 3;
    else
      return ARM_ISO7816_ERROR_RETRY_TIME;
  }
  
  /* Configure parity */
  if ((control & ARM_ISO7816_PARITY_Msk) == ARM_ISO7816_PARITY_EVEN)
  {
    /* Even parity */
    iso7816->reg->CFG &= ~ISO7816_CFG_CHKP;
  }
  else
  {
    /* Odd parity */
    iso7816->reg->CFG |= ISO7816_CFG_CHKP;
  }
  
  /* Configure baudrate */
  pclk = CLK_GetPCLKFreq(); /* Get PCLK */
  div = 0x10000 - (pclk / arg);
  iso7816->reg->BAUDDIVH = (div >> 8) & ISO7816_BAUDDIVH;
  iso7816->reg->BAUDDIVL = div & ISO7816_BAUDDIVL;
  
  iso7816->info->state |= ISO7816_CONFIGURED;
  
  return ARM_DRIVER_OK;
}

/**
  \fn          ARM_ISO7816_STATUS ARM_ISO7816_GetStatus(ISO7816_RESOURCES *iso7816)
  \brief       Get ISO7816 status.
  \param[in]   iso7816  Pointer to ISO7816 resources
  \return      ISO7816 status \ref ARM_ISO7816_STATUS
*/
static ARM_ISO7816_STATUS ARM_ISO7816_GetStatus(ISO7816_RESOURCES *iso7816)
{
  ARM_ISO7816_STATUS status;
  
  status.busy         = iso7816->info->status.busy;
  status.rx_overflow  = iso7816->info->status.rx_overflow;
  status.rx_parityerr = iso7816->info->status.rx_parityerr;
  status.tx_ackerr    = iso7816->info->status.tx_ackerr;
  
  return (status);
}

void ARM_ISO7816_SignalEvent(uint32_t event)
{
    // function body
}

/**
  \fn          void ARM_ISO7816_IRQHandler (ISO7816_RESOURCES *iso7816)
  \brief       ISO7816  Interrupt handler.
  \param[in]   iso7816  Pointer to ISO7816 resources
*/
static void ARM_ISO7816_IRQHandler(ISO7816_RESOURCES *iso7816)
{
  uint32_t iso7816_info;
  volatile uint8_t dummy_data;
  
  /* Clear flag */
  iso7816_info = iso7816->reg->INFO;
  iso7816->reg->INFO = iso7816_info;
  
  /* Transmit handle */
  if ((iso7816_info & ISO7816_INFO_SDIF) && (iso7816->xfer->tx_buf) && (iso7816->reg->CFG & ISO7816_CFG_SDIE))
  {
    /* Automatic re-transmit mode, re-send data */
    if ((iso7816->info->retx_mode) && (iso7816_info & ISO7816_INFO_SDERR))
    {
      /* Retry time: 2 */
      if (iso7816->xfer->tx_retry_num == 2)
      {
        /* Wait transmit done, receive ACK */
        while (!(iso7816->reg->INFO & ISO7816_INFO_SDIF));
        if (iso7816->reg->INFO & ISO7816_INFO_SDERR)
        {
          /* Disable re-transmit mode */
          iso7816->reg->CFG &= ~ISO7816_CFG_AUTOSD;
          /* Clear flag */
          iso7816->reg->INFO |= ISO7816_INFO_SDERR;
          iso7816->info->retx_mode = 0;
          iso7816->info->status.tx_ackerr = 1;
          if (iso7816->info->cb_event)
            iso7816->info->cb_event(ARM_ISO7816_EVENT_RETX_FAIL);
        }
      }
      /* Retry time: 3 */
      else
      {
        /* Wait transmit done, receive ACK */
        while (!(iso7816->reg->INFO & ISO7816_INFO_SDIF));
        if (iso7816->reg->INFO & ISO7816_INFO_SDERR)
        {
          /* Clear flag */
          iso7816->reg->INFO |= ISO7816_INFO_SDERR;
          /* Wait transmit done, receive ACK */
          while (!(iso7816->reg->INFO & ISO7816_INFO_SDIF));
          if (iso7816->reg->INFO & ISO7816_INFO_SDERR)
          {
            /* Disable re-transmit mode */
            iso7816->reg->CFG &= ~ISO7816_CFG_AUTOSD;
            /* Clear flag */
            iso7816->reg->INFO |= ISO7816_INFO_SDERR;
            iso7816->info->retx_mode = 0;
            iso7816->info->status.tx_ackerr = 1;
            if (iso7816->info->cb_event)
              iso7816->info->cb_event(ARM_ISO7816_EVENT_RETX_FAIL);
          }
        }
      }
      /* Clear flag */
      iso7816_info = iso7816->reg->INFO;
      iso7816->reg->INFO = iso7816_info;
      /* Clear ACK error flag when re-send data */
      iso7816_info &= ~ISO7816_INFO_SDERR;
    }
    
    /* Transmit data */
    if (iso7816->xfer->tx_cnt < iso7816->xfer->tx_num)
    {
      iso7816->reg->DATA = *(iso7816->xfer->tx_buf++);
      iso7816->xfer->tx_cnt++;
    }
    
    /* Transmit complete */
    if (iso7816->xfer->tx_cnt == iso7816->xfer->tx_num)
    {
      /* disable tx interrupt */
      iso7816->reg->CFG &= ~ISO7816_CFG_SDIE;
      iso7816->info->status.busy = 0;
      if (iso7816->info->cb_event)    
      {
        iso7816->info->cb_event(ARM_ISO7816_EVENT_TRANSMIT_COMPLETE);
      }
    }
    
    /* Error handle */
    if (iso7816_info & ISO7816_INFO_SDERR)
    {
      iso7816->info->status.tx_ackerr = 1;
      if (iso7816->info->cb_event)
        iso7816->info->cb_event(ARM_ISO7816_EVENT_ACK_ERROR);
    }
  }
  
  /* Receive handle */
  if ((iso7816_info & ISO7816_INFO_RCIF) && (iso7816->xfer->rx_buf) && (iso7816->reg->CFG & ISO7816_CFG_RCIE))
  {
    /* Automatic re-receive mode, receive data error,  response ACK as 0 */
    if ((iso7816->info->rerx_mode) && (iso7816_info & ISO7816_INFO_RCERR))
    {
      /* Retry time: 1 */
      if (iso7816->xfer->rx_retry_num == 2)
      {
        /* Wait receive interrupt */
        while (!(iso7816->reg->INFO & ISO7816_INFO_RCIF));
        if (iso7816->reg->INFO & ISO7816_INFO_RCERR)
        {
          /* Disable re-receive mode */
          iso7816->reg->CFG &= ~ISO7816_CFG_AUTORC;
          /* Clear flag */
          iso7816->reg->INFO |= ISO7816_INFO_RCERR;
          iso7816->info->rerx_mode = 0;
          iso7816->info->status.rx_parityerr = 1;
          if (iso7816->info->cb_event)
            iso7816->info->cb_event(ARM_ISO7816_EVENT_RERX_FAIL);
        }
      }
      /* Retry time: 3 */
      else
      {
        /* Wait receive interrupt */
        while (!(iso7816->reg->INFO & ISO7816_INFO_RCIF));
        if (iso7816->reg->INFO & ISO7816_INFO_RCERR)
        {
          /* Clear flag */
          iso7816->reg->INFO |= ISO7816_INFO_RCERR;
          /* Wait receive interrupt */
          while (!(iso7816->reg->INFO & ISO7816_INFO_RCIF));
          if (iso7816->reg->INFO & ISO7816_INFO_RCERR)
          {
            /* Disable re-receive mode */
            iso7816->reg->CFG &= ~ISO7816_CFG_AUTORC;
            /* Clear flag */
            iso7816->reg->INFO |= ISO7816_INFO_RCERR;
            iso7816->info->rerx_mode = 0;
            iso7816->info->status.rx_parityerr = 1;
            if (iso7816->info->cb_event)
              iso7816->info->cb_event(ARM_ISO7816_EVENT_RERX_FAIL);
          }
        }
      }
      /* Clear flag */
      iso7816_info = iso7816->reg->INFO;
      iso7816->reg->INFO = iso7816_info;
      /* Clear receive overflow flag when re-receive data */
      iso7816_info &= ~ISO7816_INFO_OVIF;
    }
    
    /* Receive data */
    if (iso7816->xfer->rx_cnt < iso7816->xfer->rx_num)
    {
      *(iso7816->xfer->rx_buf++) = iso7816->reg->DATA;
      iso7816->xfer->rx_cnt++;
    }
    if (iso7816->xfer->rx_cnt == iso7816->xfer->rx_num)
    {
      /* Disable rx interrupt */
      iso7816->reg->CFG &= ~ISO7816_CFG_RCIE;
      iso7816->info->status.busy = 0;
      if (iso7816->info->cb_event)
        iso7816->info->cb_event(ARM_ISO7816_EVENT_RECEIVE_COMPLETE);
    }
    
    /* Error handle */
    if (iso7816_info & ISO7816_INFO_OVIF) ///< Receive overflow
    {
      iso7816->info->status.rx_overflow = 1;
      if (iso7816->info->cb_event)
        iso7816->info->cb_event(ARM_ISO7816_EVENT_RX_OVERFLOW);
    }
    if (iso7816_info & ISO7816_INFO_RCERR) ///< Receive data parity error
    {
      iso7816->info->status.rx_parityerr = 1;
      if (iso7816->info->cb_event)
        iso7816->info->cb_event(ARM_ISO7816_EVENT_RX_PARITY_ERROR);      
    }
  }
}


/* Driver Control Block ------------------------------------------------------*/

/****** ISO78160 Driver Control Block *****/
static int32_t              ARM_ISO78160_Initialize    (ARM_ISO7816_SignalEvent_t pSignalEvent) { return ARM_ISO7816_Initialize     (pSignalEvent, &ISO78160_Resources); }
static int32_t              ARM_ISO78160_Uninitialize  (void)                                   { return ARM_ISO7816_Uninitialize   (&ISO78160_Resources); }
static int32_t              ARM_ISO78160_Send          (const void *data, uint32_t num)         { return ARM_ISO7816_Send           (data, num, &ISO78160_Resources); }
static int32_t              ARM_ISO78160_Receive       (      void *data, uint32_t num)         { return ARM_ISO7816_Receive        (data, num, &ISO78160_Resources); }
static uint32_t             ARM_ISO78160_GetTxCount    (void)                                   { return ARM_ISO7816_GetTxCount     (&ISO78160_Resources); }
static uint32_t             ARM_ISO78160_GetRxCount    (void)                                   { return ARM_ISO7816_GetRxCount     (&ISO78160_Resources); }
static int32_t              ARM_ISO78160_Control       (uint32_t control, uint32_t arg)         { return ARM_ISO7816_Control        (control, arg, &ISO78160_Resources); }
static ARM_ISO7816_STATUS   ARM_ISO78160_GetStatus     (void)                                   { return ARM_ISO7816_GetStatus      (&ISO78160_Resources); }
       void                 ARM_ISO78160_IRQHandler    (void)                                   {        ARM_ISO7816_IRQHandler     (&ISO78160_Resources); }
ARM_DRIVER_ISO7816 Driver_ISO78160 = {
  ARM_ISO7816_GetVersion,
  ARM_ISO7816_GetCapabilities,
  ARM_ISO78160_Initialize,
  ARM_ISO78160_Uninitialize,
  ARM_ISO78160_Send,
  ARM_ISO78160_Receive,
  ARM_ISO78160_GetTxCount,
  ARM_ISO78160_GetRxCount,
  ARM_ISO78160_Control,
  ARM_ISO78160_GetStatus
};

/****** ISO78161 Driver Control Block *****/
static int32_t              ARM_ISO78161_Initialize    (ARM_ISO7816_SignalEvent_t pSignalEvent) { return ARM_ISO7816_Initialize     (pSignalEvent, &ISO78161_Resources); }
static int32_t              ARM_ISO78161_Uninitialize  (void)                                   { return ARM_ISO7816_Uninitialize   (&ISO78161_Resources); }
static int32_t              ARM_ISO78161_Send          (const void *data, uint32_t num)         { return ARM_ISO7816_Send           (data, num, &ISO78161_Resources); }
static int32_t              ARM_ISO78161_Receive       (      void *data, uint32_t num)         { return ARM_ISO7816_Receive        (data, num, &ISO78161_Resources); }
static uint32_t             ARM_ISO78161_GetTxCount    (void)                                   { return ARM_ISO7816_GetTxCount     (&ISO78161_Resources); }
static uint32_t             ARM_ISO78161_GetRxCount    (void)                                   { return ARM_ISO7816_GetRxCount     (&ISO78161_Resources); }
static int32_t              ARM_ISO78161_Control       (uint32_t control, uint32_t arg)         { return ARM_ISO7816_Control        (control, arg, &ISO78161_Resources); }
static ARM_ISO7816_STATUS   ARM_ISO78161_GetStatus     (void)                                   { return ARM_ISO7816_GetStatus      (&ISO78161_Resources); }
       void                 ARM_ISO78161_IRQHandler    (void)                                   {        ARM_ISO7816_IRQHandler     (&ISO78161_Resources); }
ARM_DRIVER_ISO7816 Driver_ISO78161 = {
  ARM_ISO7816_GetVersion,
  ARM_ISO7816_GetCapabilities,
  ARM_ISO78161_Initialize,
  ARM_ISO78161_Uninitialize,
  ARM_ISO78161_Send,
  ARM_ISO78161_Receive,
  ARM_ISO78161_GetTxCount,
  ARM_ISO78161_GetRxCount,
  ARM_ISO78161_Control,
  ARM_ISO78161_GetStatus
};

#endif /* ISO7816_MODULE_ENABLED */

#endif /* MicroController */
/******************* (C) COPYRIGHT Vango Technologies, Inc *****END OF FILE****/
