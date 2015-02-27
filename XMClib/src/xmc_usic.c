/*
 * Copyright (C) 2015 Infineon Technologies AG. All rights reserved.
 *
 * Infineon Technologies AG (Infineon) is supplying this software for use with
 * Infineon's microcontrollers.
 * This file can be freely distributed within development tools that are
 * supporting such microcontrollers.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 */
 /**
 * @file xmc_usic.c
 * @date 16 Feb, 2015
 * @version 1.0.0
 *
 * @brief USIC driver for XMC microcontroller family.
 *
 * History <br>
 *
 * Version 1.0.0 Initial <br>
 */

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/

#include "xmc_usic.h"
#include "xmc_scu.h"

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#define USIC_CH_INPR_Msk       (0x7UL)

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/

XMC_DRIVER_VERSION_t XMC_USIC_GetDriverVersion(void)
{
  XMC_DRIVER_VERSION_t version;

  version.major = (uint8_t)XMC_USIC_MAJOR_VERSION;
  version.minor = (uint8_t)XMC_USIC_MINOR_VERSION;
  version.patch = (uint8_t)XMC_USIC_PATCH_VERSION;

  return version;
}
void XMC_USIC_CH_Enable(XMC_USIC_CH_t *const channel)
{
  XMC_ASSERT("XMC_USIC_CH_Enable: channel not valid", XMC_USIC_CHECK_CH(channel));
  
  if ((channel == XMC_USIC0_CH0) || (channel == XMC_USIC0_CH1))
  {
    XMC_USIC_Enable(XMC_USIC0);
  }
#if defined(USIC1)
  else if((channel == XMC_USIC1_CH0) || (channel == XMC_USIC1_CH1))
  {
    XMC_USIC_Enable(XMC_USIC1);
  }
#endif
#if defined(USIC2)
  else if((channel == XMC_USIC2_CH0) || (channel == XMC_USIC2_CH1))
  {
    XMC_USIC_Enable(XMC_USIC2);
  }
#endif
  else
  {
    XMC_ASSERT("USIC module not available", 0U/*Always*/);
  }
  /* USIC channel switched on*/
  channel->KSCFG = (USIC_CH_KSCFG_MODEN_Msk | USIC_CH_KSCFG_BPMODEN_Msk);
  while ((channel->KSCFG & USIC_CH_KSCFG_MODEN_Msk) == 0U)
  {
    /* Wait till the channel is enabled */
  }

  /* Set USIC channel in IDLE mode */
  channel->CCR &= (uint32_t)~USIC_CH_CCR_MODE_Msk;
}

void XMC_USIC_CH_Disable(XMC_USIC_CH_t *const channel)
{
  channel->KSCFG = (uint32_t)((channel->KSCFG & (~USIC_CH_KSCFG_MODEN_Msk)) | USIC_CH_KSCFG_BPMODEN_Msk);
}

XMC_USIC_CH_STATUS_t XMC_USIC_CH_SetBaudrate(XMC_USIC_CH_t *const channel, uint32_t rate, uint32_t oversampling)
{
  XMC_USIC_CH_STATUS_t status;
  
  uint32_t peripheral_clock;
  
  uint32_t clock_divider;
  uint32_t clock_divider_min;

  uint32_t pdiv;
  uint32_t pdiv_int;
  uint32_t pdiv_int_min;
  
  uint32_t pdiv_frac;
  uint32_t pdiv_frac_min;

  /* The rate and peripheral clock are divided by 100 to be able to use only 32bit arithmetic */
  if ((rate >= 100U) && (oversampling != 0U))
  {
    peripheral_clock = XMC_SCU_CLOCK_GetPeripheralClockFrequency() / 100U;
    rate = rate / 100U;

    clock_divider_min = 1U;
    pdiv_int_min = 1U;
    pdiv_frac_min = 0x3ffU;

    for(clock_divider = 1023U; clock_divider > 0U; --clock_divider)
    {
      pdiv = ((peripheral_clock * clock_divider) / (rate * oversampling));
      pdiv_int = pdiv >> 10U;
      pdiv_frac = pdiv & 0x3ffU;

      if ((pdiv_int < 1024U) && (pdiv_frac < pdiv_frac_min))
      {
        pdiv_frac_min = pdiv_frac;
        pdiv_int_min = pdiv_int;
        clock_divider_min = clock_divider;
      }
    }

    channel->FDR = XMC_USIC_CH_BRG_CLOCK_DIVIDER_MODE_FRACTIONAL |
                   (clock_divider_min << USIC_CH_FDR_STEP_Pos);

    channel->BRG = (channel->BRG & ~(USIC_CH_BRG_DCTQ_Msk | USIC_CH_BRG_PDIV_Msk)) | 
                   ((oversampling - 1U) << USIC_CH_BRG_DCTQ_Pos) |
                   ((pdiv_int_min - 1U) << USIC_CH_BRG_PDIV_Pos);
                    
    status = XMC_USIC_CH_STATUS_OK;
  }
  else 
  {
    status = XMC_USIC_CH_STATUS_ERROR;
  }
  
  return status;
}

void XMC_USIC_CH_TXFIFO_Configure(XMC_USIC_CH_t *const channel,
                                  const uint32_t data_pointer,
                                  const XMC_USIC_CH_FIFO_SIZE_t size,
                                  const uint32_t limit)
{
  /* Disable FIFO */
  channel->TBCTR &= (uint32_t)~USIC_CH_TBCTR_SIZE_Msk;

  /* LOF = 0, A standard transmit buffer event occurs when the filling level equals the limit value and gets
   * lower due to transmission of a data word
   * STBTEN = 0, the trigger of the standard transmit buffer event is based on the transition of the fill level
   *  from equal to below the limit, not the fact being below
   */
  channel->TBCTR = (uint32_t)(channel->TBCTR & (uint32_t)~(USIC_CH_TBCTR_LIMIT_Msk |
	                                     USIC_CH_TBCTR_DPTR_Msk |
	                                     USIC_CH_TBCTR_SIZE_Msk)) |
	                 (uint32_t)((limit << USIC_CH_TBCTR_LIMIT_Pos) |
                   (data_pointer << USIC_CH_TBCTR_DPTR_Pos) |
                   ((uint32_t)size << USIC_CH_TBCTR_SIZE_Pos));
}


void XMC_USIC_CH_RXFIFO_Configure(XMC_USIC_CH_t *const channel,
                                  const uint32_t data_pointer,
                                  const XMC_USIC_CH_FIFO_SIZE_t size,
                                  const uint32_t limit)
{
  /* Disable FIFO */
  channel->RBCTR &= (uint32_t)~USIC_CH_RBCTR_SIZE_Msk;

  /* LOF = 1, A standard receive buffer event occurs when the filling level equals the limit value and gets bigger
   *  due to the reception of a new data word
   */
  channel->RBCTR = (uint32_t)((channel->RBCTR & (uint32_t)~(USIC_CH_RBCTR_LIMIT_Msk |
	                                     USIC_CH_RBCTR_DPTR_Msk |
	                                     USIC_CH_RBCTR_LOF_Msk)) |
									 ((limit << USIC_CH_RBCTR_LIMIT_Pos) |
                   (data_pointer << USIC_CH_RBCTR_DPTR_Pos) |
                   ((uint32_t)size << USIC_CH_RBCTR_SIZE_Pos) |
                   (uint32_t)USIC_CH_RBCTR_LOF_Msk));
}

void XMC_USIC_CH_TXFIFO_SetSizeTriggerLimit(XMC_USIC_CH_t *const channel,
                                            const XMC_USIC_CH_FIFO_SIZE_t size,
                                            const uint32_t limit)
{
  /* Disable FIFO */
  channel->TBCTR &= (uint32_t)~USIC_CH_TBCTR_SIZE_Msk;

  /* STBTEN = 0, the trigger of the standard transmit buffer event is based on the transition of the fill level
   *  from equal to below the limit, not the fact being below
   */
  channel->TBCTR = (uint32_t)((uint32_t)(channel->TBCTR & (uint32_t)~USIC_CH_TBCTR_LIMIT_Msk) |
	                 (limit << USIC_CH_TBCTR_LIMIT_Pos) |
                   ((uint32_t)size << USIC_CH_TBCTR_SIZE_Pos));
}


void XMC_USIC_CH_RXFIFO_SetSizeTriggerLimit(XMC_USIC_CH_t *const channel,
                                            const XMC_USIC_CH_FIFO_SIZE_t size,
                                            const uint32_t limit)
{
  /* Disable FIFO */
  channel->RBCTR &= (uint32_t)~USIC_CH_RBCTR_SIZE_Msk;

  channel->RBCTR = (uint32_t)((uint32_t)(channel->RBCTR & (uint32_t)~USIC_CH_RBCTR_LIMIT_Msk) |
                   (limit << USIC_CH_RBCTR_LIMIT_Pos) |
                   ((uint32_t)size << USIC_CH_RBCTR_SIZE_Pos));
}



void XMC_USIC_CH_SetInterruptNodePointer(XMC_USIC_CH_t *const channel,
                                         const XMC_USIC_CH_INTERRUPT_NODE_POINTER_t interrupt_node,
                                         const uint32_t service_request)
{
  channel->INPR = (uint32_t)((channel->INPR & (~(uint32_t)(USIC_CH_INPR_Msk << (uint32_t)interrupt_node))) |
                  (service_request << (uint32_t)interrupt_node));
}

void XMC_USIC_CH_TXFIFO_SetInterruptNodePointer(XMC_USIC_CH_t *const channel,
                                                const XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_t interrupt_node,
                                                const uint32_t service_request)
{
  channel->TBCTR = (uint32_t)((channel->TBCTR & (~(uint32_t)(USIC_CH_INPR_Msk << (uint32_t)interrupt_node))) |
                   (service_request << (uint32_t)interrupt_node));
}

void XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(XMC_USIC_CH_t *const channel,
                                                const XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_t interrupt_node,
                                                const uint32_t service_request)
{
  channel->RBCTR = (uint32_t)((channel->RBCTR & (~(uint32_t)(USIC_CH_INPR_Msk << (uint32_t)interrupt_node))) |
                   (service_request << (uint32_t)interrupt_node));
}
