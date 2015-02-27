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
 * @file xmc_eru.c
 * @date 16 Feb, 2015
 * @version 1.0.0
 *
 * @brief ERU driver for XMC microcontroller family.
 *
 * History <br>
 *
 * Version 1.0.0 Initial version <br>
 */

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/

#include "xmc_eru.h"

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/

#define ERU_EXISEL_BITSIZE (4UL) /* Used to set the input for path A and path B based on the channel */
#define ERU_EXISEL_INPUT_BITSIZE (2UL)

#define XMC_ERU_ETL_CHECK_INPUT_A(input) \
    ((input == XMC_ERU_ETL_INPUT_A0) || \
     (input == XMC_ERU_ETL_INPUT_A1) || \
     (input == XMC_ERU_ETL_INPUT_A2) || \
     (input == XMC_ERU_ETL_INPUT_A3))

#define XMC_ERU_ETL_CHECK_INPUT_B(input) \
    ((input == XMC_ERU_ETL_INPUT_B0) || \
     (input == XMC_ERU_ETL_INPUT_B1) || \
     (input == XMC_ERU_ETL_INPUT_B2) || \
     (input == XMC_ERU_ETL_INPUT_B3))

#define XMC_ERU_ETL_CHECK_STATUS_FLAG_MODE(mode) \
    ((mode == XMC_ERU_ETL_STATUS_FLAG_MODE_SWCTRL) || \
     (mode == XMC_ERU_ETL_STATUS_FLAG_MODE_HWCTRL))

#define XMC_ERU_ETL_CHECK_EVENT_SOURCE(source) \
    ((source == XMC_ERU_ETL_SOURCE_A)              || \
     (source == XMC_ERU_ETL_SOURCE_B)              || \
     (source == XMC_ERU_ETL_SOURCE_A_OR_B)         || \
     (source == XMC_ERU_ETL_SOURCE_A_AND_B)        || \
     (source == XMC_ERU_ETL_SOURCE_NOT_A)          || \
     (source == XMC_ERU_ETL_SOURCE_NOT_A_OR_B)     || \
     (source == XMC_ERU_ETL_SOURCE_NOT_A_AND_B)    || \
     (source == XMC_ERU_ETL_SOURCE_NOT_B)          || \
     (source == XMC_ERU_ETL_SOURCE_A_OR_NOT_B)     || \
     (source == XMC_ERU_ETL_SOURCE_A_AND_NOT_B)    || \
     (source == XMC_ERU_ETL_SOURCE_NOT_A_OR_NOT_B) || \
     (source == XMC_ERU_ETL_SOURCE_NOT_A_AND_NOT_B))

#define XMC_ERU_ETL_CHECK_TRIGGER_EDGE(edge) \
    ((edge == XMC_ERU_ETL_EDGE_DETECTION_DISABLED) || \
     (edge == XMC_ERU_ETL_EDGE_DETECTION_RISING) || \
     (edge == XMC_ERU_ETL_EDGE_DETECTION_FALLING) || \
     (edge == XMC_ERU_ETL_EDGE_DETECTION_BOTH))

#define XMC_ERU_ETL_CHECK_TRIGGER_CHANNEL(channel) \
    ((channel == XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL0) || \
     (channel == XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL1) || \
     (channel == XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL2) || \
     (channel == XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL3))

#define XMC_ERU_OGU_CHECK_PATTERN_INPUT(input) \
    ((input == XMC_ERU_OGU_PATTERN_DETECTION_INPUT0) || \
     (input == XMC_ERU_OGU_PATTERN_DETECTION_INPUT1) || \
     (input == XMC_ERU_OGU_PATTERN_DETECTION_INPUT2) || \
     (input == XMC_ERU_OGU_PATTERN_DETECTION_INPUT3))

#define XMC_ERU_OGU_CHECK_PERIPHERAL_TRIGGER(trigger) \
    ((trigger == XMC_ERU_OGU_PERIPHERAL_TRIGGER1) || \
     (trigger == XMC_ERU_OGU_PERIPHERAL_TRIGGER2) || \
     (trigger == XMC_ERU_OGU_PERIPHERAL_TRIGGER3))

#define XMC_ERU_OGU_CHECK_SERIVCE_REQUEST(service) \
    ((service == XMC_ERU_OGU_SERVICE_REQUEST_DISABLED) || \
     (service == XMC_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER) || \
     (service == XMC_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER_AND_PATTERN_MATCH) || \
     (service == XMC_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER_AND_PATTERN_MISMATCH))
 
/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/
/* Return version (major, minor and patch number) of the driver.*/
 XMC_DRIVER_VERSION_t XMC_ERU_GetDriverVersion(void)
{
  XMC_DRIVER_VERSION_t version;

  version.major = (uint8_t)XMC_ERU_MAJOR_VERSION;
  version.minor = (uint8_t)XMC_ERU_MINOR_VERSION;
  version.patch = (uint8_t)XMC_ERU_PATCH_VERSION;

  return version;
}

/* Initializes the selected ERU_ETLx channel with the config structure.  */
void XMC_ERU_ETL_Init(XMC_ERU_t *const eru,
                      const uint8_t channel,
                      const XMC_ERU_ETL_CONFIG_t *const config)
{ 
  XMC_ASSERT("XMC_ERU_ETL_Init:Invalid Module Pointer", XMC_ERU_CHECK_MODULE_PTR(eru));
  XMC_ASSERT("XMC_ERU_ETL_Init:Invalid Channel Number", (channel < 4U));

  XMC_ERU_Enable(eru);

  eru->EXISEL = (eru->EXISEL & 
                 ~((uint32_t)(ERU_EXISEL_EXS0A_Msk | ERU_EXISEL_EXS0B_Msk) << (channel * ERU_EXISEL_BITSIZE))) |
                (config->input << (channel * (uint32_t)ERU_EXISEL_BITSIZE));
  
  eru->EXICON[channel] = config->raw;
}

/* Initializes the selected ERU_OGUy channel with the config structure.  */
void XMC_ERU_OGU_Init(XMC_ERU_t *const eru,
                      const uint8_t channel,
                      const XMC_ERU_OGU_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_ERU_OGU_Init:Invalid Module Pointer", XMC_ERU_CHECK_MODULE_PTR(eru));
  XMC_ASSERT("XMC_ERU_OGU_Init:Invalid Channel Number", (channel < 4U));

  XMC_ERU_Enable(eru);

  eru->EXOCON[channel] = config->raw;
}

/* Configures the event source for path A and path B, with selected input_a and input_b respectively.*/
void XMC_ERU_ETL_SetInput(XMC_ERU_t *const eru,
                          const uint8_t channel,
                          const XMC_ERU_ETL_INPUT_A_t input_a,
                          const XMC_ERU_ETL_INPUT_B_t input_b)
{
  XMC_ASSERT("XMC_ERU_ETL_SetInput:Invalid Module Pointer", XMC_ERU_CHECK_MODULE_PTR(eru));
  XMC_ASSERT("XMC_ERU_ETL_SetInput:Invalid Channel Number", (channel < 4U));
  XMC_ASSERT("XMC_ERU_ETL_SetInput:Invalid A", XMC_ERU_ETL_CHECK_INPUT_A(input_a));
  XMC_ASSERT("XMC_ERU_ETL_SetInput:Invalid B", XMC_ERU_ETL_CHECK_INPUT_B(input_b));

  eru->EXISEL = (eru->EXISEL & ~((uint32_t)(ERU_EXISEL_EXS0A_Msk | ERU_EXISEL_EXS0B_Msk) << (channel * ERU_EXISEL_BITSIZE))) |
                (((uint32_t)input_a | (uint32_t)(input_b << ERU_EXISEL_INPUT_BITSIZE))  << (channel * ERU_EXISEL_BITSIZE));
}

/* Select input path combination along with polarity for event generation by setting (SS, NA, NB) bits of 
   EXICONx(x = [0 to 3]) register */
void XMC_ERU_ETL_SetSource(XMC_ERU_t *const eru,
                          const uint8_t channel,
                          const XMC_ERU_ETL_SOURCE_t source)
{
  XMC_ASSERT("XMC_ERU_ETL_SetSource:Invalid Module Pointer", XMC_ERU_CHECK_MODULE_PTR(eru));
  XMC_ASSERT("XMC_ERU_ETL_SetSource:Invalid Channel Number", (channel < 4U));
  XMC_ASSERT("XMC_ERU_ETL_SetSource:Invalid Source", XMC_ERU_ETL_CHECK_EVENT_SOURCE(source));

  eru->EXICON_b[channel].SS = (uint8_t)source;
}

/* Configure event trigger edge/s by setting (RE, FE) bits of EXICONx(x = [0 to 3]) register.*/
void XMC_ERU_ETL_SetEdgeDetection(XMC_ERU_t *const eru,
                                  const uint8_t channel,
                                  const XMC_ERU_ETL_EDGE_DETECTION_t edge_detection)
{
  XMC_ASSERT("XMC_ERU_ETL_SetEdgeDetection:Invalid Module Pointer", XMC_ERU_CHECK_MODULE_PTR(eru));
  XMC_ASSERT("XMC_ERU_ETL_SetEdgeDetection:Invalid Channel Number", (channel < 4U));
  XMC_ASSERT("XMC_ERU_ETL_SetEdgeDetection:Invalid Trigger Edge", XMC_ERU_ETL_CHECK_TRIGGER_EDGE(edge_detection));

  eru->EXICON_b[channel].ED = (uint8_t)edge_detection;
}

/* Set the status flag bit(FL) in EXICONx(x = [0 to 3]). */
void XMC_ERU_ETL_SetStatusFlagMode(XMC_ERU_t *const eru,
                                   const uint8_t channel,
                                   const XMC_ERU_ETL_STATUS_FLAG_MODE_t mode)
{
  XMC_ASSERT("XMC_ERU_ETL_SetStatusFlagMode:Invalid Module Pointer", XMC_ERU_CHECK_MODULE_PTR(eru));
  XMC_ASSERT("XMC_ERU_ETL_SetStatusFlagMode:Invalid Channel Number", (channel < 4U));
  XMC_ASSERT("XMC_ERU_ETL_SetStatusFlagMode:Invalid Status Flag Mode", XMC_ERU_ETL_CHECK_STATUS_FLAG_MODE(mode));

  eru->EXICON_b[channel].LD = (uint8_t)mode;
}

/* Configure which Channel of OGUy(Output gating unit y = [0 to 3]) to be mapped by the trigger pulse generated by 
 * ETLx(Event Trigger Logic, x = [0 to 3]) by setting (OCS and PE) bit fields. */
void XMC_ERU_ETL_EnableOutputTrigger(XMC_ERU_t *const eru,
                                     const uint8_t channel,
                                     const XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL_t trigger)
{
  XMC_ASSERT("XMC_ERU_ETL_EnableOutputTrigger:Invalid Module Pointer", XMC_ERU_CHECK_MODULE_PTR(eru));
  XMC_ASSERT("XMC_ERU_ETL_EnableOutputTrigger:Invalid Channel Number", (channel < 4U));
  XMC_ASSERT("XMC_ERU_ETL_EnableOutputTrigger:Invalid Output Channel", XMC_ERU_ETL_CHECK_TRIGGER_CHANNEL(trigger));

  eru->EXICON_b[channel].OCS = (uint8_t)trigger;
  eru->EXICON_b[channel].PE = (uint8_t)true;
}

/* Disables the trigger pulse generation by clearing the (PE) of the EXICONx(x = [0 to 3]). */
void XMC_ERU_ETL_DisableOutputTrigger(XMC_ERU_t *const eru, const uint8_t channel)
{
  XMC_ASSERT("XMC_ERU_ETL_DisableOutputTrigger:Invalid Module Pointer", XMC_ERU_CHECK_MODULE_PTR(eru));
  XMC_ASSERT("XMC_ERU_ETL_DisableOutputTrigger:Invalid Channel Number", (channel < 4U));

  eru->EXICON_b[channel].PE = false;
}

/* Configures ERU_ETLx(x = [0 to 3]) for pattern match detection by setting IPENx(x = [0 to 3) and GEEN bits. */
void XMC_ERU_OGU_EnablePatternDetection(XMC_ERU_t *const eru,
                                        const uint8_t channel,
                                        const XMC_ERU_OGU_PATTERN_DETECTION_INPUT_t input)
{
  XMC_ASSERT("XMC_ERU_OGU_EnablePatternDetection:Invalid Module Pointer", XMC_ERU_CHECK_MODULE_PTR(eru));
  XMC_ASSERT("XMC_ERU_OGU_EnablePatternDetection:Invalid Channel Number", (channel < 4U));
  XMC_ASSERT("XMC_ERU_OGU_EnablePatternDetection:Invalid Pattern input", XMC_ERU_OGU_CHECK_PATTERN_INPUT(input));

  eru->EXOCON_b[channel].IPEN = (uint8_t)input;
  eru->EXOCON_b[channel].GEEN = true;
}

/* Disable the pattern detection by clearing (GEEN) bit. */
void XMC_ERU_OGU_DisablePatternDetection(XMC_ERU_t *const eru, const uint8_t channel)
{
  XMC_ASSERT("XMC_ERU_OGU_DisablePatternDetection:Invalid Module Pointer", XMC_ERU_CHECK_MODULE_PTR(eru));
  XMC_ASSERT("XMC_ERU_OGU_DisablePatternDetection:Invalid Channel Number", (channel < 4U));

  eru->EXOCON_b[channel].GEEN = false;
}

/* Configures peripheral trigger input, by setting (ISS) bit. */
void XMC_ERU_OGU_EnablePeripheralTrigger(XMC_ERU_t *const eru,
                                         const uint8_t channel,
                                         const XMC_ERU_OGU_PERIPHERAL_TRIGGER_t peripheral_trigger)
{
  XMC_ASSERT("XMC_ERU_OGU_EnablePeripheralTrigger:Invalid Module Pointer", XMC_ERU_CHECK_MODULE_PTR(eru));
  XMC_ASSERT("XMC_ERU_OGU_EnablePeripheralTrigger:Invalid Channel Number", (channel < 4U));
  XMC_ASSERT("XMC_ERU_OGU_EnablePeripheralTrigger:Invalid Peripheral Trigger Input", 
              XMC_ERU_OGU_CHECK_PERIPHERAL_TRIGGER(peripheral_trigger));

  eru->EXOCON_b[channel].ISS = (uint8_t)peripheral_trigger;
}

/* Disables event generation based on peripheral trigger by clearing (ISS) bit. */
void XMC_ERU_OGU_DisablePeripheralTrigger(XMC_ERU_t *const eru,
                                          const uint8_t channel)
{
  XMC_ASSERT("XMC_ERU_OGU_DisablePeripheralTrigger:Invalid Module Pointer", XMC_ERU_CHECK_MODULE_PTR(eru));
  XMC_ASSERT("XMC_ERU_OGU_DisablePeripheralTrigger:Invalid Channel Number", (channel < 4U));

  eru->EXOCON_b[channel].ISS = (uint8_t)0;
}

/* Configures the gating scheme for service request generation by setting (GP) bit. */
void XMC_ERU_OGU_SetServiceRequestMode(XMC_ERU_t *const eru,
                                       const uint8_t channel,
                                       const XMC_ERU_OGU_SERVICE_REQUEST_t mode)
{
  XMC_ASSERT("XMC_ERU_OGU_SetServiceRequestMode:Invalid Module Pointer", XMC_ERU_CHECK_MODULE_PTR(eru));
  XMC_ASSERT("XMC_ERU_OGU_SetServiceRequestMode:Invalid Channel Number", (channel < 4U));
  XMC_ASSERT("XMC_ERU_OGU_SetServiceRequestMode:Invalid Service Request Mode", XMC_ERU_OGU_CHECK_SERIVCE_REQUEST(mode));

  eru->EXOCON_b[channel].GP = (uint8_t)mode;

}
