#ifndef _CONFIG_H
#define _CONFIG_H

#define PNP_NORMAL_CLOSED 0
#define PNP_NORMAL_OPEN   1
#define NPN_NORMAL_CLOSED 2
#define NPN_NORMAL_OPEN   3

/**
 * @brief enable serial log print
 * this option is CONFLICT with game button(over serial)
 */
#define UMAIIDX_ENABLE_LOGPRINT
#ifdef UMAIIDX_ENABLE_LOGPRINT
  #define UMAIIDX_LOG_FMTSIZE 256
  #define UMAIIDX_LOGPRINT_BAUDRATE 921600
#endif

/**
 * @brief photo interrupter type
 */
#ifndef UMAIIDX_PHOTO_INTERRUPTER
  #define UMAIIDX_PHOTO_INTERRUPTER NPN_NORMAL_OPEN
#endif

/**
 * @brief enable io4 emulation
 * all button input operations will be transmitted via usb hid.
 */
#ifndef UMAIIDX_ENABLE_IO4
  #define UMAIIDX_ENABLE_IO4
#endif

/**
 * @brief touch panel serial baud rate
 * the default value is 9600. higher baud rate causes
 * low transmission time and reduces the latency.
 *
 * maybe segatool supports a custom baud rate? idk
 */
#ifndef UMAIIDX_TOUCHPANEL_BAUDRATE
  #define UMAIIDX_TOUCHPANEL_BAUDRATE 9600
#endif

/**
 * @brief button led effect while game is booting
 * after the game program is connected to the umaiidx control board,
 * the led turned into passive mode and starts accepting the game control.
 *
 * to find all the effect types that can be used,
 * please see `light_mode_t` in app/light.h
 *
 * - light_mode_off: the led is disabled
 * - light_mode_full_white: all leds are full white
 * - light_mode_rainbow_spin: rainbow spinning effect
 * - light_mode_white_spin: white shades spinning effect
 * - light_mode_white_breath: white breathing effect
 * - light_mode_passive: passive mode. receive the control msgs from game program
 */
#define UMAIIDX_ENABLE_BOOTANIM
#ifdef UMAIIDX_ENABLE_BOOTANIM
  #define UMAIIDX_BOOTANIM_TYPE light_mode_white_spin
  #define UMAIIDX_BOOTANIM_SPEED 50
#endif

#endif /* _CONFIG_H */
