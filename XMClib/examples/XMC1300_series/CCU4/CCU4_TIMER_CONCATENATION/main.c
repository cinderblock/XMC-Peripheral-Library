#include <xmc_common.h>
#include <xmc_scu.h>
#include <xmc_ccu4.h>

const XMC_SCU_CLOCK_CONFIG_t clock_config =
{
  .idiv = 1,
  .pclk_src = XMC_SCU_CLOCK_PCLKSRC_DOUBLE_MCLK
};

XMC_CCU4_SLICE_COMPARE_CONFIG_t timer_config =
{
  .timer_mode = XMC_CCU4_SLICE_TIMER_COUNT_MODE_EA,
  .monoshot = XMC_CCU4_SLICE_TIMER_REPEAT_MODE_REPEAT,
  .prescaler_initval = 6,
};

void SystemCoreClockSetup(void)
{
  XMC_SCU_CLOCK_Init(&clock_config);
}

void CCU40_0_IRQHandler(void)
{
  static uint32_t ticks = 0UL;

  ++ticks;
}

int main(void)
{
  XMC_CCU4_Init(CCU40, XMC_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR);
  XMC_CCU4_SLICE_CompareInit(CCU40_CC40, &timer_config);
  XMC_CCU4_SLICE_SetTimerPeriodMatch(CCU40_CC40, 0xffff);

  timer_config.timer_concatenation = true;
  XMC_CCU4_SLICE_CompareInit(CCU40_CC41, &timer_config);
  XMC_CCU4_SLICE_SetTimerPeriodMatch(CCU40_CC41, 0xf);

  XMC_CCU4_SLICE_EnableEvent(CCU40_CC41, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);
  XMC_CCU4_SLICE_SetInterruptNode(CCU40_CC41, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH, 0);

  NVIC_SetPriority(CCU40_0_IRQn, 3);
  NVIC_EnableIRQ(CCU40_0_IRQn);

  XMC_CCU4_StartPrescaler(CCU40);

  XMC_CCU4_EnableShadowTransfer(CCU40, XMC_CCU4_SHADOW_TRANSFER_SLICE_0 | XMC_CCU4_SHADOW_TRANSFER_SLICE_1);

  XMC_CCU4_EnableClock(CCU40, 1);
  XMC_CCU4_SLICE_StartTimer(CCU40_CC41);

  XMC_CCU4_EnableClock(CCU40, 0);
  XMC_CCU4_SLICE_StartTimer(CCU40_CC40);

  while(1);
}
