/******************************************************************************
 * @file:    system_tle987x.c
 * @purpose: CMSIS Cortex-M3 Device Peripheral Access Layer Source File
 *           for the IFX TLE987x Devices
 * @version: V0.2
 * @date:    15. Oct. 2012
 *----------------------------------------------------------------------------
 *
 * Copyright (C) 2008 ARM Limited. All rights reserved.
 *
 * ARM Limited (ARM) is supplying this software for use with Cortex-M3 
 * processor based microcontrollers.  This file can be freely distributed 
 * within development tools that are supporting such ARM based processors. 
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/


#include <tle_device.h>
#include <scu.h>
#include <wdt1.h>
#include "bsl_defines.h"
/*----------------------------------------------------------------------------
  Define BSL parameter
 *----------------------------------------------------------------------------*/
#if (NAC_NAD_EN == 1)
/*lint -e950 Suppressing MISRA 2012 Directive 2.1 */ \
#if defined( __CC_ARM )
const uint32 p_NACNAD  __attribute__((at(NACStart),used)) = (uint32)NAD_NAC;
#elif defined ( __GNUC__ )
const uint32_t p_NACNAD __attribute__ ((section (".secNACNAD"))) = (uint32_t)NAD_NAC;
#else
#errror "ERROR: Compiler not supported"    
#endif
/*lint +e950 */
#endif

void SystemInit(void)
{
	/* Vector table relocation */
  CPU->VTOR.reg	  = ProgFlashStart; 
  SCU_ClkInit();
  WDT1_Init();

  /* erratas */
  /* clear VBAT_UV/OV IE flag */
  SCUPM->SYS_SUPPLY_IRQ_CTRL.reg &= ~(uint32)(0x11);
	
  /* stop Timer21 */
  TIMER21->T2CON.bit.TR2 = 0;
  /* clear IE flags */
  TIMER21->T2CON1.reg = 0;
  /* clear IS flags */
  TIMER21->T2ICLR.reg = (uint8)0xC0;
}
