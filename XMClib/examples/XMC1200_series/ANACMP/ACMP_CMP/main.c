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
 * @file
 * @date 20 Feb, 2015
 * @version 1.0.0
 *
 * @brief ACMP example for XMC1200
 *
 * In this example, ACMP1 slice is configured to receive VRef on its IN-P pad.
 * Bitfield CMP_OUT of register ANACMP1 changes its state based on the input applied on IN-N pad.
 *
 * History <br>
 *
 * Version 1.0.0 Initial <br>
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include <xmc_acmp.h>

/*********************************************************************************************************************
 * GLOBAL DATA
 ********************************************************************************************************************/

/* ACMP Slice configuration */
XMC_ACMP_CONFIG_t g_acmp_config = 
{
  .filter_disable = 0U,
  .output_invert = 0U,
  .anacmp = XMC_ACMP_HYSTERESIS_20
};

/*********************************************************************************************************************
 * MAIN APPLICATION
 ********************************************************************************************************************/

int main(void)
{
  XMC_ACMP_Init(COMPARATOR, 1, &g_acmp_config);

  /* Connect REF to IN-P */
  XMC_ACMP_SetInput(COMPARATOR, 1, XMC_ACMP_INP_SOURCE_REF_DIV);

  /* Users may now apply inputs on P2.6 and evaluate the state of ANACMP1:CMP_OUT bit*/
  while(1U)
  {
    /* Infinite loop */
  }
}
