#include <stdio.h>
#include <string.h>

#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_gpio.h>
#include <stm32f4xx_hal_uart.h>
#include <sys/debug.h>
#include <sys/tick.h>

#include <usrcfg.h>

#include "logprint.h"

#ifdef UMAIIDX_ENABLE_LOGPRINT

static UART_HandleTypeDef _uart1 = {
  .Instance = USART1,
  .Init.BaudRate= UMAIIDX_LOGPRINT_BAUDRATE,
  .Init.WordLength = UART_WORDLENGTH_8B,
  .Init.StopBits = UART_STOPBITS_1,
  .Init.Parity = UART_PARITY_NONE,
  .Init.Mode = UART_MODE_TX_RX,
  .Init.HwFlowCtl = UART_HWCONTROL_NONE,
  .Init.OverSampling = UART_OVERSAMPLING_16
};

/**
 * @brief print log
 *
 * @param level log level
 * @param tag tag
 * @param fmt format string
 * @param args format args
 */
void __callback_logprint(loglevel_t level, const char* tag, const char* fmt, va_list args) {

  size_t _len;
  char _logbuf[UMAIIDX_LOG_FMTSIZE];
  static char _lv[] = {'D', 'I', 'W', 'E', 'F'};

  // tag
  _len = snprintf(_logbuf, UMAIIDX_LOG_FMTSIZE, "[%d] [%c] [%s] ", tick_get_time(), _lv[level], tag); {
    logprint_print_ex(_logbuf, _len);
  }

  // format log string
  _len = vsnprintf(_logbuf, UMAIIDX_LOG_FMTSIZE, fmt, args); {
    logprint_print_ex(_logbuf, _len);
  }

  logprint_print_ex("\r\n", 2);
}

#endif

void logprint_init() {

#ifdef UMAIIDX_ENABLE_LOGPRINT

  __HAL_RCC_USART1_CLK_ENABLE(); {

    // init gpio pinmux
    HAL_GPIO_Init(GPIOA, &(GPIO_InitTypeDef) {
      .Pin = GPIO_PIN_9 | GPIO_PIN_10,
      .Mode = GPIO_MODE_AF_PP,
      .Pull = GPIO_NOPULL,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
      .Alternate = GPIO_AF7_USART1
    });

    // init usart
    HAL_UART_Init(&_uart1);
  }

  log_set_cb(&__callback_logprint);

#endif
}

void logprint_print(const char* str) {
  logprint_print_ex(str, strlen(str));
}

void logprint_print_ex(const char* str, size_t size) {
#ifdef UMAIIDX_ENABLE_LOGPRINT
  HAL_UART_Transmit(&_uart1, (uint8_t *)str, size, HAL_MAX_DELAY);
#endif
}


