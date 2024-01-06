#ifndef _APP_BOARD_LED15070_H
#define _APP_BOARD_LED15070_H

#include <stdint.h>
#include <stddef.h>

#include "led15070_cmd.h"
#include "led15070_frame.h"

typedef void (* led15070_passive_cb_t)(led15070_frame_t* f);

/**
 * @brief led board init
 */
void led15070_init();

/**
 * @brief mux uart tx port acts as game button output
 *
 * to find more information, please see `config.h`
 */
void led15070_mux_uart_tx(uint8_t* data, size_t length);

/**
 * @brief set callback handler
 *
 * @param cb callback
 */
void led15070_set_callback(led15070_passive_cb_t cb);

/**
 * @brief exti nvic handler. see stm32f4xx_it.c
 */
void __exti_usart1_handler();

/**
 * @brief exti nvic handler. see stm32f4xx_it.c
 */
void __exti_dma2s2_handler();

#endif /* _APP_BOARD_LED15070_H */
