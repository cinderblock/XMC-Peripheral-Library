/******************************************************************************
 * @file     system_XMC1400.c
 * @brief    Device specific initialization for the XMC1300-Series according 
 * to CMSIS
 * @version  V1.0
 * @date     14 April 2015
 *
 * @note
 * Copyright (C) 2012-2015 Infineon Technologies AG. All rights reserved.

 *
 * @par
 * Infineon Technologies AG (Infineon) is supplying this software for use with 
 * Infineon?s microcontrollers.
 *   
 * This file can be freely distributed within development tools that are 
 * supporting such microcontrollers.
 *  
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/
/*
 * *************************** Change history ********************************
 * V1.0, 7 May 2015, JFT : initial version
 */

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/

#include <XMC1400.h>
#include "system_XMC1400.h"

/*******************************************************************************
 * MACROS
 *******************************************************************************/

/* Define WEAK attribute */
#if !defined(__WEAK)
#if defined ( __CC_ARM )
#define __WEAK __attribute__ ((weak))
#elif defined ( __ICCARM__ )
#define __WEAK __weak
#elif defined ( __GNUC__ )
#define __WEAK __attribute__ ((weak))
#elif defined ( __TASKING__ )
#define __WEAK __attribute__ ((weak))
#endif
#endif

#define DCO1_FREQUENCY (48000000U)

/*
//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
*/

/*
// <h> Clock configuration
*/

/*
//    <o> External crystal frequency [Hz]
//       <8000000=> 8MHz
//       <12000000=> 12MHz
//       <16000000=> 16MHz
//       <20000000=> 20MHz
//    <i> Defines external crystal frequency
//    <i> Default: 20MHz
*/
#define OSCHP_FREQUENCY (20000000U)

/*
//    <o> DCLK clock source selection
//       <0=> Internal oscillator DCO1 (48MHz)
//       <1=> External crystal oscillator
//    <i> Default: Internal oscillator DCO1 (48MHz)
*/
#define DCLK_CLOCK_SRC 0
#define DCLK_CLOCK_SRC_DCO1 0
#define DCLK_CLOCK_SRC_EXT_XTAL 1

/*
//    <o> OSCHP external oscillator mode
//       <0=> Crystal mode
//       <1=> External clock direct input mode
//    <i> Default: Crystal mode
*/
#define OSCHP_MODE 0
#define OSCHP_MODE_XTAL (0 << SCU_ANALOG_ANAOSCHPCTRL_MODE_Pos)
#define OSCHP_MODE_DIRECT (1 << SCU_ANALOG_ANAOSCHPCTRL_MODE_Pos)

/*
//    <o> RTC clock source selection
//       <0=> Internal oscillator DCO2 (32768Hz)
//       <1=> External crystal oscillator
//       <2=> External clock
//    <i> Default: Internal oscillator DCO2 (32768Hz)
*/
#define RTC_CLOCK_SRC 0
#define RTC_CLOCK_SRC_DCO2 (0 << SCU_CLK_CLKCR_RTCCLKSEL_Pos)
#define RTC_CLOCK_SRC_EXT_XTAL (1 << SCU_CLK_CLKCR_RTCCLKSEL_Pos)
/*
//    <o> PCLK clock source selection
//       <0=> MCLK
//       <1=> 2xMCLK
//    <i> Default: 2xMCLK
*/
#define PCLK_CLOCK_SRC 1
#define PCLK_CLOCK_SRC_MCLK (0 << SCU_CLK_CLKCR_RTCCLKSEL_Pos)
#define PCLK_CLOCK_SRC_2XMCLK (1 << SCU_CLK_CLKCR_RTCCLKSEL_Pos)

/*
//-------- <<< end of configuration section >>> ------------------
*/

/*******************************************************************************
 * GLOBAL VARIABLES
 *******************************************************************************/

#if defined ( __CC_ARM )
uint32_t SystemCoreClock __attribute__((at(0x20003FFC)));
#elif defined ( __ICCARM__ )
__no_init uint32_t SystemCoreClock;
#elif defined ( __GNUC__ )
uint32_t SystemCoreClock __attribute__((section(".no_init")));
#elif defined ( __TASKING__ )
uint32_t SystemCoreClock __at( 0x20003FFC );
#endif

/*******************************************************************************
 * LOCAL FUNCTIONS
 *******************************************************************************/
static void delay(uint32_t cycles)
{
  volatile uint32_t i;

  for(i = 0UL; i < cycles ;++i)
  {
    __NOP();
  }
}

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/

__WEAK void SystemInit(void)
{    
  SystemCoreSetup();
  SystemCoreClockSetup();
}

__WEAK void SystemCoreSetup(void)
{
}

__WEAK void SystemCoreClockSetup(void)
{
  /* Clock setup done during SSW using the CLOCK_VAL1 and CLOCK_VAL2 defined in vector table */

  /* disable bit protection */
  SCU_GENERAL->PASSWD = 0x000000C0UL; 

#if DCLK_CLOCK_SRC != DCLK_CLOCK_SRC_DCO1
  
  /* OSCHP source selection - OSC mode */
  SCU_ANALOG->ANAOSCHPCTRL = (SCU_ANALOG->ANAOSCHPCTRL & ~SCU_ANALOG_ANAOSCHPCTRL_MODE_Msk) |
		                     OSCHP_MODE;

  if (OSCHP_GetFrequency() > 20000000U)
  {
	SCU_ANALOG->ANAOSCHPCTRL |= SCU_ANALOG_ANAOSCHPCTRL_HYSCTRL_Msk;
  }

  /* Oscillator start-up time, max = 10ms */
  delay(480000);

  /* Enable oscillator watchdog*/
  SCU_CLK->OSCCSR |= SCU_CLK_OSCCSR_XOWDEN_Msk;

  /* Restart detection */
  SCU_INTERRUPT->SRCLR1 = SCU_INTERRUPT_SRCLR1_LOECI_Msk;
  SCU_CLK->OSCCSR |= SCU_CLK_OSCCSR_XOWDRES_Msk;

  /* Wait a few DCO2 cycles for the update of the clock detection result */
  while(SCU_CLK->OSCCSR & SCU_CLK_OSCCSR_XOWDRES_Msk);
  delay(7200);

  while(SCU_INTERRUPT->SRRAW1 & SCU_INTERRUPT_SRRAW1_LOECI_Msk);

  /* DCLK source using DCO1 */
  SCU_CLK->CLKCR1 |= SCU_CLK_CLKCR1_DCLKSEL_Msk;

#else
  /* Enable oscillator watchdog */
  SCU_CLK->OSCCSR |= SCU_CLK_OSCCSR_OWDEN_Msk;

  /* Restart oscillator watchdog */
  SCU_CLK->OSCCSR |= SCU_CLK_OSCCSR_OWDRES_Msk;
  /* Wait a few DCO2 cycles for the update of the clock detection result */
  while(SCU_CLK->OSCCSR & SCU_CLK_OSCCSR_OWDRES_Msk);
  delay(7200);

  /* check clock is ok */
  while((SCU_CLK->OSCCSR & (SCU_CLK_OSCCSR_OSC2L_Msk | SCU_CLK_OSCCSR_OSC2H_Msk)) != 0U);
#endif
  
  /* Update PCLK selection mux. */
  SCU_CLK->CLKCR = (1023UL <<SCU_CLK_CLKCR_CNTADJ_Pos) |
		           RTC_CLOCK_SRC |
				   PCLK_CLOCK_SRC;

  /* enable bit protection */
  SCU_GENERAL->PASSWD = 0x000000C3UL;

  SystemCoreClockUpdate();
}

__WEAK void SystemCoreClockUpdate(void)
{
  static uint32_t IDIV, FDIV;

  IDIV = ((SCU_CLK->CLKCR) & SCU_CLK_CLKCR_IDIV_Msk) >> SCU_CLK_CLKCR_IDIV_Pos;
  FDIV = (((SCU_CLK->CLKCR) & SCU_CLK_CLKCR_FDIV_Msk) >> SCU_CLK_CLKCR_FDIV_Pos) |
         (((SCU_CLK->CLKCR1) & SCU_CLK_CLKCR1_FDIV_Msk) << 8);
  
  if (IDIV != 0)
  {
    /* Fractional divider is enabled and used */
#if DCLK_CLOCK_SRC == DCLK_CLOCK_SRC_DCO1
    SystemCoreClock = ((uint32_t)((DCO1_FREQUENCY << 6U) / ((IDIV << 10) + FDIV))) << 4U;
#else
    SystemCoreClock = ((uint32_t)((OSCHP_GetFrequency() << 6U) / ((IDIV << 10) + FDIV))) << 4U;
#endif	
  }
  else
  {
    /* Fractional divider bypassed. */
#if DCLK_CLOCK_SRC == DCLK_CLOCK_SRC_DCO1
    SystemCoreClock = DCO1_FREQUENCY;
#else
    SystemCoreClock = OSCHP_GetFrequency();
#endif	
  }
}

__WEAK uint32_t OSCHP_GetFrequency(void)
{
  return OSCHP_FREQUENCY;
}
