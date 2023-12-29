#ifndef _APP_BOARD_LED15093_H
#define _APP_BOARD_LED15093_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
  led15093_event_connected,
  led15093_event_update,
} led15093_event_t;

typedef void (* led15093_passive_cb_t)(led15093_event_t e);

/**
 * @brief led board init
 */
void led15093_init();

/**
 * @brief mux uart tx port acts as game button output
 *
 * to find more information, please see `config.h`
 */
void led15093_mux_uart_tx(uint8_t* data, size_t length);

/**
 * @brief set callback handler
 *
 * @param cb callback
 */
void led15093_set_callback(led15093_passive_cb_t cb);

/**
 * @brief exti nvic handler. see stm32f4xx_it.c
 */
void __exti_usart1_handler();

/**
 * @brief exti nvic handler. see stm32f4xx_it.c
 */
void __exti_dma2_s2_handler();

#endif /* _APP_BOARD_LED15093_H */
