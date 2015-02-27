/*
 * Copyright (C) 2015 Infineon Technologies AG. All rights reserved.
 *
 * Infineon Technologies AG (Infineon) is supplying this software for use with Infineon's microcontrollers.
 * This file can be freely distributed within development tools that are supporting such microcontrollers.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY 
 * REASON WHATSOEVER.
 *
 */

/**
 * @file xmc_hrpwm_map.h
 * @date 16 Feb, 2015
 * @version 1.0.2
 *
 * @brief HRPWM low level driver API prototype definition for XMC family of microcontrollers <br>
 *
 * <b>Detailed description of file</b> <br>
 * APIs provided in this file cover the following functional blocks of HRPWM: <br>
 * -- High Resolution Channel (APIs prefixed with XMC_HRPWM_HRC_) <br>
 * ---Comparator and Slope Generator (APIs prefixed with XMC_HRPWM_CSG_)
 *
 * History
 *
 * Version 1.0.0 Initial version <br>
 * Version 1.0.2 Updated version due to change in xmc_hrpwm.h file<br>
 */
 
/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "xmc_hrpwm.h"

#ifndef XMC_HRPWM_MAP_H
#define XMC_HRPWM_MAP_H

#if ((UC_DEVICE == XMC4400) || (UC_DEVICE == XMC4200) || (UC_DEVICE == XMC4100))
/* CSG0 - General input to control Blanking and Switch of the Comparator */
#define XMC_HRPWM_CSG0_BL_P1_4        XMC_HRPWM_CSG_INPUT_SEL_IA
#define XMC_HRPWM_CSG0_BL_SCU_GSHR0   XMC_HRPWM_CSG_INPUT_SEL_IB
#define XMC_HRPWM_CSG0_BL_CCU80_ST0   XMC_HRPWM_CSG_INPUT_SEL_IC
#define XMC_HRPWM_CSG0_BL_CCU80_ST1   XMC_HRPWM_CSG_INPUT_SEL_ID
#define XMC_HRPWM_CSG0_BL_CCU80_ST2   XMC_HRPWM_CSG_INPUT_SEL_IE
#define XMC_HRPWM_CSG0_BL_CCU80_ST3   XMC_HRPWM_CSG_INPUT_SEL_IF
#define XMC_HRPWM_CSG0_BL_CCU40_ST0   XMC_HRPWM_CSG_INPUT_SEL_IG
#define XMC_HRPWM_CSG0_BL_CCU41_ST0   XMC_HRPWM_CSG_INPUT_SEL_IH
#define XMC_HRPWM_CSG0_BL_HRPWM_QOUT0 XMC_HRPWM_CSG_INPUT_SEL_II
#define XMC_HRPWM_CSG0_BL_HRPWM_QOUT1 XMC_HRPWM_CSG_INPUT_SEL_IJ
#define XMC_HRPWM_CSG0_BL_CCU40_SR0   XMC_HRPWM_CSG_INPUT_SEL_IK
#define XMC_HRPWM_CSG0_BL_CCU41_SR0   XMC_HRPWM_CSG_INPUT_SEL_IL
#define XMC_HRPWM_CSG0_BL_HRPWM_SR2   XMC_HRPWM_CSG_INPUT_SEL_IM
#define XMC_HRPWM_CSG0_BL_HRPWM_SR3   XMC_HRPWM_CSG_INPUT_SEL_IN
#define XMC_HRPWM_CSG0_BL_ERU1_IOUT0  XMC_HRPWM_CSG_INPUT_SEL_IO
#define XMC_HRPWM_CSG0_BL_ERU1_IOUT1  XMC_HRPWM_CSG_INPUT_SEL_IP

/* CSG0 - General input to control start/stop/trigger for Slope Control Logic */
#define XMC_HRPWM_CSG0_SC_SCU_GSHR0   XMC_HRPWM_CSG_INPUT_SEL_IB
#define XMC_HRPWM_CSG0_SC_CCU80_ST0   XMC_HRPWM_CSG_INPUT_SEL_IC
#define XMC_HRPWM_CSG0_SC_CCU80_ST1   XMC_HRPWM_CSG_INPUT_SEL_ID
#define XMC_HRPWM_CSG0_SC_CCU80_ST2   XMC_HRPWM_CSG_INPUT_SEL_IE
#define XMC_HRPWM_CSG0_SC_CCU80_ST3   XMC_HRPWM_CSG_INPUT_SEL_IF
#define XMC_HRPWM_CSG0_SC_CCU40_ST0   XMC_HRPWM_CSG_INPUT_SEL_IG
#define XMC_HRPWM_CSG0_SC_CCU41_ST0   XMC_HRPWM_CSG_INPUT_SEL_IH
#define XMC_HRPWM_CSG0_SC_HRPWM_QOUT0 XMC_HRPWM_CSG_INPUT_SEL_II
#define XMC_HRPWM_CSG0_SC_HRPWM_QOUT1 XMC_HRPWM_CSG_INPUT_SEL_IJ
#define XMC_HRPWM_CSG0_SC_CCU40_SR0   XMC_HRPWM_CSG_INPUT_SEL_IK
#define XMC_HRPWM_CSG0_SC_CCU41_SR0   XMC_HRPWM_CSG_INPUT_SEL_IL
#define XMC_HRPWM_CSG0_SC_HRPWM_C0O   XMC_HRPWM_CSG_INPUT_SEL_IM
#define XMC_HRPWM_CSG0_SC_HRPWM_SR3   XMC_HRPWM_CSG_INPUT_SEL_IN
#define XMC_HRPWM_CSG0_SC_ERU1_IOUT0  XMC_HRPWM_CSG_INPUT_SEL_IO
#define XMC_HRPWM_CSG0_SC_ERU1_IOUT1  XMC_HRPWM_CSG_INPUT_SEL_IP

/* CSG1 - General input to control Blanking and Switch of the Comparator */
#define XMC_HRPWM_CSG1_BL_P2_4        XMC_HRPWM_CSG_INPUT_SEL_IA
#define XMC_HRPWM_CSG1_BL_SCU_GSHR0   XMC_HRPWM_CSG_INPUT_SEL_IB
#define XMC_HRPWM_CSG1_BL_CCU80_ST0   XMC_HRPWM_CSG_INPUT_SEL_IC
#define XMC_HRPWM_CSG1_BL_CCU80_ST1   XMC_HRPWM_CSG_INPUT_SEL_ID
#define XMC_HRPWM_CSG1_BL_CCU80_ST2   XMC_HRPWM_CSG_INPUT_SEL_IE
#define XMC_HRPWM_CSG1_BL_CCU80_ST3   XMC_HRPWM_CSG_INPUT_SEL_IF
#define XMC_HRPWM_CSG1_BL_CCU40_ST1   XMC_HRPWM_CSG_INPUT_SEL_IG
#define XMC_HRPWM_CSG1_BL_CCU41_ST1   XMC_HRPWM_CSG_INPUT_SEL_IH
#define XMC_HRPWM_CSG1_BL_HRPWM_QOUT1 XMC_HRPWM_CSG_INPUT_SEL_II
#define XMC_HRPWM_CSG1_BL_HRPWM_QOUT2 XMC_HRPWM_CSG_INPUT_SEL_IJ
#define XMC_HRPWM_CSG1_BL_CCU40_SR1   XMC_HRPWM_CSG_INPUT_SEL_IK
#define XMC_HRPWM_CSG1_BL_CCU41_SR1   XMC_HRPWM_CSG_INPUT_SEL_IL
#define XMC_HRPWM_CSG1_BL_HRPWM_SR2   XMC_HRPWM_CSG_INPUT_SEL_IM
#define XMC_HRPWM_CSG1_BL_HRPWM_SR3   XMC_HRPWM_CSG_INPUT_SEL_IN
#define XMC_HRPWM_CSG1_BL_ERU1_IOUT0  XMC_HRPWM_CSG_INPUT_SEL_IO
#define XMC_HRPWM_CSG1_BL_ERU1_IOUT1  XMC_HRPWM_CSG_INPUT_SEL_IP

/* CSG1 - General input to control start/stop/trigger for Slope Control Logic */
#define XMC_HRPWM_CSG1_SC_SCU_GSHR0   XMC_HRPWM_CSG_INPUT_SEL_IB
#define XMC_HRPWM_CSG1_SC_CCU80_ST0   XMC_HRPWM_CSG_INPUT_SEL_IC
#define XMC_HRPWM_CSG1_SC_CCU80_ST1   XMC_HRPWM_CSG_INPUT_SEL_ID
#define XMC_HRPWM_CSG1_SC_CCU80_ST2   XMC_HRPWM_CSG_INPUT_SEL_IE
#define XMC_HRPWM_CSG1_SC_CCU80_ST3   XMC_HRPWM_CSG_INPUT_SEL_IF
#define XMC_HRPWM_CSG1_SC_CCU40_ST1   XMC_HRPWM_CSG_INPUT_SEL_IG
#define XMC_HRPWM_CSG1_SC_CCU41_ST1   XMC_HRPWM_CSG_INPUT_SEL_IH
#define XMC_HRPWM_CSG1_SC_HRPWM_QOUT1 XMC_HRPWM_CSG_INPUT_SEL_II
#define XMC_HRPWM_CSG1_SC_HRPWM_QOUT2 XMC_HRPWM_CSG_INPUT_SEL_IJ
#define XMC_HRPWM_CSG1_SC_CCU40_SR1   XMC_HRPWM_CSG_INPUT_SEL_IK
#define XMC_HRPWM_CSG1_SC_CCU41_SR1   XMC_HRPWM_CSG_INPUT_SEL_IL
#define XMC_HRPWM_CSG1_SC_HRPWM_C1O   XMC_HRPWM_CSG_INPUT_SEL_IM
#define XMC_HRPWM_CSG1_SC_HRPWM_SR3   XMC_HRPWM_CSG_INPUT_SEL_IN
#define XMC_HRPWM_CSG1_SC_ERU1_IOUT0  XMC_HRPWM_CSG_INPUT_SEL_IO
#define XMC_HRPWM_CSG1_SC_ERU1_IOUT1  XMC_HRPWM_CSG_INPUT_SEL_IP

/* CSG2 - General input to control Blanking and Switch of the Comparator */
#define XMC_HRPWM_CSG2_BL_P2_5        XMC_HRPWM_CSG_INPUT_SEL_IA
#define XMC_HRPWM_CSG2_BL_SCU_GSHR0   XMC_HRPWM_CSG_INPUT_SEL_IB
#define XMC_HRPWM_CSG2_BL_CCU80_ST0   XMC_HRPWM_CSG_INPUT_SEL_IC
#define XMC_HRPWM_CSG2_BL_CCU80_ST1   XMC_HRPWM_CSG_INPUT_SEL_ID
#define XMC_HRPWM_CSG2_BL_CCU80_ST2   XMC_HRPWM_CSG_INPUT_SEL_IE
#define XMC_HRPWM_CSG2_BL_CCU80_ST3   XMC_HRPWM_CSG_INPUT_SEL_IF
#define XMC_HRPWM_CSG2_BL_CCU40_ST2   XMC_HRPWM_CSG_INPUT_SEL_IG
#define XMC_HRPWM_CSG2_BL_CCU41_ST2   XMC_HRPWM_CSG_INPUT_SEL_IH
#define XMC_HRPWM_CSG2_BL_HRPWM_QOUT2 XMC_HRPWM_CSG_INPUT_SEL_II
#define XMC_HRPWM_CSG2_BL_HRPWM_QOUT3 XMC_HRPWM_CSG_INPUT_SEL_IJ
#define XMC_HRPWM_CSG2_BL_CCU40_SR2   XMC_HRPWM_CSG_INPUT_SEL_IK
#define XMC_HRPWM_CSG2_BL_CCU41_SR2   XMC_HRPWM_CSG_INPUT_SEL_IL
#define XMC_HRPWM_CSG2_BL_HRPWM_SR2   XMC_HRPWM_CSG_INPUT_SEL_IM
#define XMC_HRPWM_CSG2_BL_HRPWM_SR3   XMC_HRPWM_CSG_INPUT_SEL_IN
#define XMC_HRPWM_CSG2_BL_ERU1_IOUT0  XMC_HRPWM_CSG_INPUT_SEL_IO
#define XMC_HRPWM_CSG2_BL_ERU1_IOUT1  XMC_HRPWM_CSG_INPUT_SEL_IP

/* CSG2 - General input to control start/stop/trigger for Slope Control Logic */
#define XMC_HRPWM_CSG2_SC_SCU_GSHR0   XMC_HRPWM_CSG_INPUT_SEL_IB
#define XMC_HRPWM_CSG2_SC_CCU80_ST0   XMC_HRPWM_CSG_INPUT_SEL_IC
#define XMC_HRPWM_CSG2_SC_CCU80_ST1   XMC_HRPWM_CSG_INPUT_SEL_ID
#define XMC_HRPWM_CSG2_SC_CCU80_ST2   XMC_HRPWM_CSG_INPUT_SEL_IE
#define XMC_HRPWM_CSG2_SC_CCU80_ST3   XMC_HRPWM_CSG_INPUT_SEL_IF
#define XMC_HRPWM_CSG2_SC_CCU40_ST2   XMC_HRPWM_CSG_INPUT_SEL_IG
#define XMC_HRPWM_CSG2_SC_CCU41_ST2   XMC_HRPWM_CSG_INPUT_SEL_IH
#define XMC_HRPWM_CSG2_SC_HRPWM_QOUT2 XMC_HRPWM_CSG_INPUT_SEL_II
#define XMC_HRPWM_CSG2_SC_HRPWM_QOUT3 XMC_HRPWM_CSG_INPUT_SEL_IJ
#define XMC_HRPWM_CSG2_SC_CCU40_SR2   XMC_HRPWM_CSG_INPUT_SEL_IK
#define XMC_HRPWM_CSG2_SC_CCU41_SR2   XMC_HRPWM_CSG_INPUT_SEL_IL
#define XMC_HRPWM_CSG2_SC_HRPWM_C2O   XMC_HRPWM_CSG_INPUT_SEL_IM
#define XMC_HRPWM_CSG2_SC_HRPWM_SR3   XMC_HRPWM_CSG_INPUT_SEL_IN
#define XMC_HRPWM_CSG2_SC_ERU1_IOUT0  XMC_HRPWM_CSG_INPUT_SEL_IO
#define XMC_HRPWM_CSG2_SC_ERU1_IOUT1  XMC_HRPWM_CSG_INPUT_SEL_IP

#endif

#endif  /* XMC_HRPWM_MAP_H */
