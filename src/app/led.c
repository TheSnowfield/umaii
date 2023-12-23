#include <driver/config.h>
#include "led.h"

void led_init() {
  HAL_GPIO_Init(GPIO_PORT_LED, &GPIO_LED);
  led_enable(false);
}

void led_enable(bool enable) {
   HAL_GPIO_WritePin(GPIO_PORT_LED, GPIO_LED.Pin, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void led_toggle() {
   HAL_GPIO_TogglePin(GPIO_PORT_LED, GPIO_LED.Pin);
}

