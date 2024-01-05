#ifndef _APP_BOARD_LED15093_H
#define _APP_BOARD_LED15093_H

#include <stdint.h>
#include <stddef.h>

#include "led15093_cmd.h"
#include "led15093_frame.h"

typedef void (* led15093_passive_cb_t)(led15093_frame_t* f);

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
void __exti_dma2s2_handler();

#endif /* _APP_BOARD_LED15093_H */
