#ifndef _APP_LIGHT_H
#define _APP_LIGHT_H

#include <stdint.h>
#include <stddef.h>

#define MODE(mode, flag) ((1 << mode) | flag)

typedef enum {
  light_flag_static,
  light_flag_anim
} light_flag_t;

typedef enum {
  light_mode_off           = MODE(0, light_flag_static),
  light_mode_full_white    = MODE(1, light_flag_static),
  light_mode_rainbow_spin  = MODE(2, light_flag_anim),
  light_mode_white_spin    = MODE(3, light_flag_anim),
  light_mode_white_breath  = MODE(4, light_flag_anim),
  light_mode_passive       = MODE(5, light_flag_static)
} light_mode_t;

#undef MODE

/**
 * @brief init the ws2812 strip
 *
 * @param count led count
 */
void light_init(size_t count);

/**
 * @brief set the show mode
 *
 * @param mode the mode
 * @param speed the speed (light_flag_anim only)
 */
void light_show_mode(light_mode_t mode, uint32_t speed);

#endif /* _SYS_HARDWARE_LIGHT_H */
