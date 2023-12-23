#include <driver/config.h>
#include "ch334f.h"

void ch334f_init() {

  // initialize gpio
  HAL_GPIO_Init(GPIO_PORT_CH334F_RST, &GPIO_CH334F_RST);

  // todo: initialize iic and emulate eeprom

  // release ch334f reset pin
  // ch334f_enable(false);
}

void ch334f_enable(bool enable) {
   HAL_GPIO_WritePin(GPIO_PORT_CH334F_RST, GPIO_CH334F_RST.Pin, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
