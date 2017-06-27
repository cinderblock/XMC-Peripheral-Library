/******************************************************************************
 * @file:    system_tle987x.h
 * @purpose: CMSIS Cortex-M3 Device Peripheral Access Layer Header File
 *           for the Infineon TLE987x Device Series 
 * @version: V0.01
 * @date:    15. Oct. 2013
 *----------------------------------------------------------------------------
 *
 * Copyright (C) 2009 ARM Limited. All rights reserved.
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


#ifndef __SYSTEM_TLE987x_H
#define __SYSTEM_TLE987x_H

#ifdef __cplusplus
 extern "C" {
#endif 
/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define XTAL        (  5000000UL)        /* Oscillator frequency              */
#define OSC_CLK     (       XTAL)        /* Main oscillator frequency         */

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemFrequency variable.
 */
extern void SystemInit (void);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_TLE987x_H */
