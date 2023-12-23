#ifndef _APP_LED_H
#define _APP_LED_H

#include <stdbool.h>

/**
 * @brief init on board status led
 *
 */
void led_init();

/**
 * @brief enable or disable the status led
 *
 * @param enable
 */
void led_enable(bool enable);

/**
 * @brief toggle the status led
 *
 */
void led_toggle();

#endif /* _APP_LED_H */
