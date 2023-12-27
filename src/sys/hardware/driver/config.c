#include "config.h"

GPIO_InitTypeDef GPIO_SWDEBUG = {
  .Pin = GPIO_PIN_13 | GPIO_PIN_14,
  .Mode = GPIO_MODE_AF_PP,
  .Speed = GPIO_SPEED_FREQ_HIGH,
  .Alternate = GPIO_AF0_SWJ
};

GPIO_InitTypeDef GPIO_CH334F_RST = {
  .Pin = GPIO_PIN_7,
  .Mode = GPIO_MODE_OUTPUT_PP,
  .Speed = GPIO_SPEED_LOW,
  .Pull = GPIO_PULLDOWN
};

RTC_HandleTypeDef rtc = {
  .Instance = RTC,
};
