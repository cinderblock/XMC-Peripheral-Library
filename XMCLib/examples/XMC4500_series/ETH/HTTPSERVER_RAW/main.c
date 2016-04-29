/*
 * Copyright (C) 2015-2016 Infineon Technologies AG. All rights reserved.
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
 * @date 20 April,2016
 * @version 1.0.2
 *
 * @brief ETH HTTP server demo example using the netconn interface
 *
 * History <br>
 *
 * Version 1.0.0 
 * - Initial
 *
 * Version 1.0.2
 * - Stability and speed improvements
 */

#include <xmc_gpio.h>
#include <xmc_eru.h>

#include <lwip/timers.h>
#include <lwip/netif.h>
#include <lwip/init.h>
#include <netif/etharp.h>
#include <ethernetif.h>
#include "httpserver_raw/httpd.h"

#if LWIP_DHCP == 1
#include <lwip/dhcp.h>
#endif

#define LED1 P1_1
#define LED2 P1_0

/*Static IP ADDRESS*/
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   0
#define IP_ADDR3   10

/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   0
#define GW_ADDR3   1

struct netif xnetif;

void LWIP_Init(void)
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;

#if LWIP_DHCP == 0
  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
#else
  ipaddr.addr = 0;
  netmask.addr = 0;
  gw.addr = 0;
#endif

  lwip_init();

  /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
  struct ip_addr *netmask, struct ip_addr *gw,
  void *state, err_t (* init)(struct netif *netif),
  err_t (* input)(struct pbuf *p, struct netif *netif))

  Adds your network interface to the netif_list. Allocate a struct
  netif and pass a pointer to this structure as the first argument.
  Give pointers to cleared ip_addr structures when using DHCP,
  or fill them with sane numbers otherwise. The state pointer may be NULL.

  The init function pointer must point to a initialization function for
  your ethernet netif interface. The following code illustrates it's use.*/
  netif_add(&xnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  /*  Registers the default network interface.*/
  netif_set_default(&xnetif);

  /* Set Ethernet link flag */
  xnetif.flags |= NETIF_FLAG_LINK_UP;

#if LWIP_DHCP == 1
  dhcp_start(&xnetif);
#else
  /* When the netif is fully configured this function must be called.*/
  netif_set_up(&xnetif);
#endif

}

#define BUTTON2 P1_15

XMC_ERU_ETL_CONFIG_t button_event_generator_config =
{
  .input_a = ERU1_ETL1_INPUTA_P1_15,
  .source = XMC_ERU_ETL_SOURCE_A,
  .edge_detection = XMC_ERU_ETL_EDGE_DETECTION_FALLING,
  .status_flag_mode = XMC_ERU_ETL_STATUS_FLAG_MODE_HWCTRL,
  .enable_output_trigger = true,
  .output_trigger_channel = XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL0
};

XMC_ERU_OGU_CONFIG_t button_event_detection_config =
{
  .service_request = XMC_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER
};

uint32_t button_count = 0;

void ERU1_0_IRQHandler(void)
{
  button_count++;
}

/* Initialisation of functions to be used with CGi*/
//  CGI handler to switch LED status
const char *ledcontrol_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  if(strcmp(pcValue[0], "led1") == 0)
  {
    XMC_GPIO_ToggleOutput(LED1);
  }
  else {
    XMC_GPIO_ToggleOutput(LED2);
  }
  return "/index.htm";
}

const char *data_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  return "/data.ssi";
}

tCGI led_handler_struct[] =
{
  {
    .pcCGIName = "/ledcontrol.cgi",
    .pfnCGIHandler = ledcontrol_handler
  },
  {
   .pcCGIName = "/data.cgi",
   .pfnCGIHandler = data_handler
  }
};

int cgi_init(void)
{
  http_set_cgi_handlers(led_handler_struct, 2);

  return 0;
}

/**
 * Initialize SSI handlers
 */
const char *TAGS[]={"b"};

uint16_t button_handler(int iIndex, char *pcInsert, int iInsertLen)
{
  *pcInsert = (char)(((button_count / 100) % 10) + 0x30U);
  pcInsert++;
  *pcInsert = (char)(((button_count / 10) % 10) + 0x30U);
  pcInsert++;
  *pcInsert = (char)((button_count % 10) + 0x30U);

  return 3;
}

void ssi_init(void)
{
  http_set_ssi_handler(button_handler, (char const **)TAGS, 1);
}

int main(void)
{
  XMC_ERU_ETL_Init(ERU1_ETL1, &button_event_generator_config);
  XMC_ERU_OGU_Init(ERU1_OGU0, &button_event_detection_config);

  NVIC_SetPriority(ERU1_0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 63, 0));
  NVIC_EnableIRQ(ERU1_0_IRQn);

  XMC_GPIO_SetMode(LED1, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
  XMC_GPIO_SetMode(LED2, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);

  SysTick_Config(SystemCoreClock / 1000);

  LWIP_Init();
  httpd_init();
  cgi_init();
  ssi_init();

  while(1)
  {
    sys_check_timeouts();
  }
}
