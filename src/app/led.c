#include <sys/debug.h>
#include <hardware/common.h>

#include "led.h"

#define LED_PIN GPIO_PIN_2
#define LED_PORT GPIOD
// GPIO_InitTypeDef GPIO_LED = {
//   .Pin = GPIO_PIN_2,
//   .Mode = GPIO_MODE_OUTPUT_PP,
//   .Speed = GPIO_SPEED_LOW,
//   .Pull = GPIO_PULLDOWN

void led_init() {

  // init led pin
  HAL_GPIO_Init(LED_PORT, &(GPIO_InitTypeDef) {
    .Pin = LED_PIN,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Speed = GPIO_SPEED_LOW,
    .Pull = GPIO_PULLDOWN
  });

  // disable led
  led_enable(false);

  log_info(TAG, "status led init");
}

void led_enable(bool enable) {
   HAL_GPIO_WritePin(LED_PORT, LED_PIN, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void led_toggle() {
   HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
}
