#include <stm32f4xx.h>
#include <manager/resource.h>
#include <driver/config.h>

#include "fault.h"

#pragma GCC push_options
#pragma GCC optimize ("O0")

void panic() {
  __disable_irq();
  for(;;);
}

#pragma GCC pop_options

void fault() {
  fault_ex(err_ok);
  NVIC_SystemReset();
}

void fault_ex(status_t reason) {
  // HAL_RTCEx_BKUPWrite(&rtc, RTC_BKUPREG_RESET, reason);
  // NVIC_SystemReset();

  extern void __blink_fault_reason(status_t reason);
  __blink_fault_reason(reason);
}

status_t get_fault_reason() {
  return (status_t) HAL_RTCEx_BKUPRead(&rtc, RTC_BKUPREG_RESET);
}

void assert(uint8_t cond, status_t reason) {
  if (!cond) fault_ex(reason);
}
