#ifndef _SYS_HARDWARE_GAMEBTN_H
#define _SYS_HARDWARE_GAMEBTN_H

#include <stdbool.h>

typedef enum {
  gamebtn_stat_0 = 0b00000001,
  gamebtn_stat_1 = 0b00000010,
  gamebtn_stat_2 = 0b00000100,
  gamebtn_stat_3 = 0b00001000,
  gamebtn_stat_4 = 0b00010000,
  gamebtn_stat_5 = 0b00100000,
  gamebtn_stat_6 = 0b01000000,
  gamebtn_stat_7 = 0b10000000,
  gamebtn_stat_1p = 0x8000,
} gamebtn_stat_t;

/*
* @brief button masks.
         normal button are use 0-7pins, 15pin for 1p button.
*/
#define GAMEBTN_IOMASK 0b1000000011111111
//                       1XXXXXXX00000000
//                       5XXXXXXX76543210

typedef void (*gamebtn_irqcb_t)(gamebtn_stat_t new, gamebtn_stat_t old);

/**
 * @brief initialize game buttons
 */
void gamebtn_init();

/**
 * @brief enable irq
 *
 * @param en enable
 */
void gamebtn_enable_irq(bool en);

/**
 * @brief get current button status
 *
 * @return gamebtn_stat_t
 */
gamebtn_stat_t gamebtn_get_stat();

/**
 * @brief exti nvic handler. see stm32f4xx_it.c
 *
 * @param pin interrupt pin number
 */
extern void __exti_gamebtn_handler(uint16_t pin);

#endif /* _SYS_HARDWARE_GAMEBTN_H */
