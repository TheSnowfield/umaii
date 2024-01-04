/**
 * UmaiiDX Control Board (AGPL)
 * UmaiiDX (c) 2024
 *
 * This file is used to configure the UmaiiDX control board firmware,
 * contains several options for fine-tuning the board features.
 * Feel free to adjust these options to reach your best game experience!
 *
 * After you edit this file every time, re-compile the project,
 * and upload to the board is needed.
 *
 * Happy enjoy! （＾∀＾●）ﾉｼ
 *
 * Inputs / Outputs:
 * TouchPanel [COM]
 * GameButton, 1P, Coin, Service, Test [HID(IO4) / KBD(SEGATOOL) / COM(SEGATOOL CUSTOM IO)]
 * Light [COM (TX(slave) is optional)]
 *
 * The default serial port number assignment is:
 * Player 1: TouchPanel(COM3), LED(COM21/COM25/COM29/COM33)
 * Player 2: TouchPanel(COM4), LED(COM23/COM27/COM31/COM35)
 * CardReader: COM1
 * COM20? COM24? COM28? COM32?
 */

#ifndef _CONFIG_H
#define _CONFIG_H

/**
 * @brief enable serial log print
 * this option is CONFLICT with game button `UMAIIDX_LIGHT_MUX_GAMEBTN`
 *
 * while `UMAIIDX_ENABLE_LOGPRINT` is enabled, debug logs
 * will print to serial, which is using the muxed TX serial port of the light control.
 */
#define UMAIIDX_ENABLE_LOGPRINT
#ifdef UMAIIDX_ENABLE_LOGPRINT
  #define UMAIIDX_LOG_FMTSIZE 256
#endif

/**
 * @brief predefined photo interrupter types.
 * don't change them
 */
#define PNP_NORMAL_CLOSED 0
#define PNP_NORMAL_OPEN   1
#define NPN_NORMAL_CLOSED 2
#define NPN_NORMAL_OPEN   3

/**
 * @brief photo interrupter type
 */
#ifndef UMAIIDX_PHOTO_INTERRUPTER
  #define UMAIIDX_PHOTO_INTERRUPTER NPN_NORMAL_OPEN
#endif

/**
 * @brief touch panel serial baud rate
 * the default value is 9600.
 * a higher baud rate causes low transmission time and reduces the input latency,
 * but increase data error rate (referenced from wch334p datasheet).
 *
 * maybe segatool support a custom baud rate?
 * I'm not sure, but you still can work around it by patching the game.
 */
#ifndef UMAIIDX_TOUCHPANEL_BAUDRATE
  #define UMAIIDX_TOUCHPANEL_BAUDRATE 9600
#endif

/**
 * @brief enable io4 emulation
 * enabled this function, all button input
 * operations will be transmitted over usb hid (io4 emulation over jvs protocol).
 *
 * after commenting UMAIIDX_GAMEBTN_HID, the input will be transmitted over serial.
 * serial mode must be supported by segatool custom io (mai2.dll).
 */
#define UMAIIDX_GAMEBTN_HID
#ifdef UMAIIDX_GAMEBTN_HID
  #define BOARD_15257_VID 0x0ca3
  #define BOARD_15257_PID 0x0021
  #define BOARD_15257_MANUFACTURER "Sega Corp."
  #define BOARD_15257_DESCRIPTION "I/O CONTROL BD"
#elif defined UMAIIDX_ENABLE_LOGPRINT
  #error Please disable the log feature `UMAIIDX_ENABLE_LOGPRINT` \
  or enable gamtbtn io4 emulation `UMAIIDX_GAMEBTN_HID`. \
  They're using the same uart output port, conflict.
#else

/**
 * @brief mux serial TX port of light control as gamebtn output
 * game will report the light control board is in the BAD state, then ignore the error.
 */
  #define UMAIIDX_LIGHT_MUX_GAMEBTN
#endif

/**
 * @brief light control serial baud rate
 */
// #define UMAIIDX_LIGHT_CONTROL
#ifndef UMAIIDX_LIGHT_BAUDRATE
  #define UMAIIDX_LIGHT_BAUDRATE 115200
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
#define UMAIIDX_LIGHT_BOOTANIM
#ifdef UMAIIDX_LIGHT_BOOTANIM
  #define BOOTANIM_TYPE light_mode_white_breath
  #define BOOTANIM_SPEED 300
#endif

#endif /* _CONFIG_H */
