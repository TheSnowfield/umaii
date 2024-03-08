#include <config.h>
#include <sys/debug.h>
#include <hardware/common.h>
#include <hardware/mpr121.h>

#include "touchpanel.h"

static UART_HandleTypeDef _uart3 = {
  .Instance = USART3,
  .Init.BaudRate = UMAIIDX_TOUCHPANEL_BAUDRATE,
  .Init.WordLength = UART_WORDLENGTH_8B,
  .Init.StopBits = UART_STOPBITS_1,
  .Init.Parity = UART_PARITY_NONE,
  .Init.Mode = UART_MODE_TX_RX,
  .Init.HwFlowCtl = UART_HWCONTROL_NONE,
  .Init.OverSampling = UART_OVERSAMPLING_16
};

void touchpanel_init() {

  __HAL_RCC_USART3_CLK_ENABLE(); {

    // init gpio pinmux
    HAL_GPIO_Init(GPIOC, &(GPIO_InitTypeDef) {
      .Pin = GPIO_PIN_10 | GPIO_PIN_11,
      .Mode = GPIO_MODE_AF_PP,
      .Pull = GPIO_NOPULL,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
      .Alternate = GPIO_AF7_USART3
    });

    // init usart
    HAL_UART_Init(&_uart3);
  }

  // init mpr121 driver
  mpr121_init();
  // mpr121_set_irqcb(cb_mpr121_touched);
  // mpr121_enable_irq(true);

  while(true) {
    uint16_t _touch = mpr121_get_touch();
    //log_info("tp", "%d", _touch);
  }

}

void touchpanel_enable_irq() {

}
