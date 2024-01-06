#ifndef _APP_BOARD_LED15070_CMD_H
#define _APP_BOARD_LED15070_CMD_H

#include <stdint.h>

/**
 * @brief command of led15070 board
 */
typedef enum {
  led15070_cmd_reset = 16,
  led15070_cmd_set_timeout = 17,
  led15070_cmd_set_led_gs_8bit = 49,
  led15070_cmd_set_led_gs_8bit_multi = 50,
  led15070_cmd_set_led_gs_8bit_multi_fade = 51,
  led15070_cmd_set_dc_update = 59,
  led15070_cmd_set_led_fet = 57,
  led15070_cmd_set_led_gs_update = 60,
  led15070_cmd_set_dc = 63,
  led15070_cmd_set_eeprom = 123,
  led15070_cmd_get_eeprom = 124,
  led15070_cmd_get_board_info = 240,
  led15070_cmd_get_board_status = 241,
  led15070_cmd_get_protocol_ver = 243,
} led15070_cmd_t;

/**
 * @brief status of led15070 board
 */
typedef enum {
  led15070_status_ok = 0x01,
  led15070_status_err_checksum = 0x02,
  led15070_status_err_parity = 0x03,
  led15070_status_err_framing = 0x04,
  led15070_status_err_overrun = 0x05,
  led15070_status_err_buffer_overflow = 0x06,
} led15070_status_t;

typedef enum {
  led15070_report_ok = 0x01,
  led15070_report_wait = 0x02,
  led15070_report_err1 = 0x03,
  led15070_report_err2 = 0x04,
} led15070_report_t;

#endif /* _APP_BOARD_LED15070_CMD_H */
