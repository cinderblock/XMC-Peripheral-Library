
/**
 * @file xmc_eth_mac.c
 * @date 2015-10-27
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.2 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-09-01:
 *     - Add clock gating control in enable/disable APIs
 *     - Add transmit polling if run out of buffers
 *
 * 2015-06-20:
 *     - Initial
 *
 * @endcond
 */

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/

#include <xmc_ecat.h>

#if defined (ECAT0)

#include <xmc_scu.h>

/*******************************************************************************
 * MACROS
 *******************************************************************************/

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/
void XMC_ECAT_Enable(void)
{
  XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_ECAT0);
  XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_ECAT0);

  while (XMC_SCU_RESET_IsPeripheralResetAsserted(XMC_SCU_PERIPHERAL_RESET_ECAT0) == true);
}

void XMC_ECAT_Disable(void)
{
  XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_ECAT0);
  while (XMC_SCU_RESET_IsPeripheralResetAsserted(XMC_SCU_PERIPHERAL_RESET_ECAT0) == false);

  XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_ECAT0);
}

XMC_ECAT_STATUS_t XMC_ECAT_Init(void)
{
  XMC_ECAT_Enable();

  return XMC_ECAT_STATUS_OK;
}

void XMC_ECAT_EnableEvent(uint32_t event)
{
  ECAT0->AL_EVENT_MASK |= event;
}

void XMC_ECAT_DisableEvent(uint32_t event)
{
  ECAT0->AL_EVENT_MASK &= ~event;
}

uint32_t XMC_ECAT_GetEventStatus(void)
{
  return (ECAT0->AL_EVENT_REQ);
}

void XMC_ECAT_DisableSyncManChannel(const uint8_t channel)
{
  ((ECAT0_SM_Type *)((uint8_t *)ECAT0_SM0 + (channel * 8U)))->SM_PDI_CTR |= 0x1U;
}

void XMC_ECAT_EnableSyncManChannel(const uint8_t channel)
{
  ((ECAT0_SM_Type *)((uint8_t *)ECAT0_SM0 + (channel * 8U)))->SM_PDI_CTR &= ~0x1U;
}

XMC_ECAT_STATUS_t XMC_ECAT_ReadPhy(uint8_t phy_addr, uint8_t reg_addr, uint16_t *data)
{
  XMC_ECAT_STATUS_t status;

  ECAT0->MII_PHY_ADR = phy_addr;
  ECAT0->MII_PHY_REG_ADR = reg_addr;

  ECAT0->MII_CONT_STAT |= 0x0100U;  /* read instruction */
  while ((ECAT0->MII_CONT_STAT & ECAT_MII_CONT_STAT_BUSY_Msk) != 0U);
  
  if ((ECAT0->MII_CONT_STAT & ECAT_MII_CONT_STAT_ERROR_Msk) != 0U)
  {
    ECAT0->MII_CONT_STAT &= ~ECAT_MII_CONT_STAT_CMD_REG_Msk; /* Clear error */
    status = XMC_ECAT_STATUS_ERROR;
  }
  else
  {
    *data = (uint16_t)ECAT0->MII_PHY_DATA;
    status = XMC_ECAT_STATUS_OK;
  }

  return status;
}

XMC_ECAT_STATUS_t XMC_ECAT_WritePhy(uint8_t phy_addr, uint8_t reg_addr, uint16_t data)
{
  XMC_ECAT_STATUS_t status;

  ECAT0->MII_PHY_ADR = phy_addr;
  ECAT0->MII_PHY_REG_ADR = reg_addr;
  ECAT0->MII_PHY_DATA = data;

  ECAT0->MII_CONT_STAT |= 0x0200U;  /* write instruction */
  while ((ECAT0->MII_CONT_STAT & ECAT_MII_CONT_STAT_BUSY_Msk) != 0U);

  if ((ECAT0->MII_CONT_STAT & ECAT_MII_CONT_STAT_ERROR_Msk) != 0U)
  {
    ECAT0->MII_CONT_STAT &= ~ECAT_MII_CONT_STAT_CMD_REG_Msk; /* Clear error */
    status = XMC_ECAT_STATUS_ERROR;
  }
  else
  {
    status = XMC_ECAT_STATUS_OK;
  }

  return status;
}

#endif
