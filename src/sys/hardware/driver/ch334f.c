#include <hardware/common.h>
#include "ch334f.h"

#define CH334F_PORT GPIOC
#define CH334F_RST GPIO_PIN_7

void ch334f_init() {

  // initialize gpio
  HAL_GPIO_Init(CH334F_PORT, &(GPIO_InitTypeDef) {
    .Pin = CH334F_RST,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Speed = GPIO_SPEED_LOW,
    .Pull = GPIO_PULLDOWN
  });

  // todo: initialize iic and emulate eeprom

  // release ch334f reset pin
  // ch334f_enable(false);
}

void ch334f_enable(bool enable) {
   HAL_GPIO_WritePin(CH334F_PORT, CH334F_RST, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
