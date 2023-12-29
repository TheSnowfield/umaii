#ifndef _APP_BOARD_LED15093_CMD_H
#define _APP_BOARD_LED15093_CMD_H

/**
 * best thanks to segatool for offering this reference!
 * :3
 */

#include <stdint.h>

/**
 * @brief command byte of led 15093 board
 */
typedef enum {
  led15093_cmd_reset = 0x10,
  led15093_cmd_set_timeout = 0x11,
  led15093_cmd_unk1 = 0x12,
  led15093_cmd_set_disable_response = 0x14,
  led15093_cmd_set_id = 0x18,
  led15093_cmd_clear_id = 0x19,
  led15093_cmd_set_max_bright = 0x3f,
  led15093_cmd_update_led = 0x81,
  led15093_cmd_set_imm_led = 0x82,
  led15093_cmd_set_fade_led = 0x83,
  led15093_cmd_set_fade_level = 0x84,
  led15093_cmd_set_fade_shift = 0x85,
  led15093_cmd_set_auto_shift = 0x86,
  led15093_cmd_get_board_info = 0xf0,
  led15093_cmd_get_board_status = 0xf1,
  led15093_cmd_get_fw_sum = 0xf2,
  led15093_cmd_get_protocol_ver = 0xf3,
  led15093_cmd_set_bootmode = 0xfd,
  led15093_cmd_fw_update = 0xfe
} led15093_cmd_t;

/**
 * @brief status of led 15093 board
 */
typedef enum {
  led15093_status_ok = 0x01,
  led15093_status_err_checksum = 0x02,
  led15093_status_err_parity = 0x03,
  led15093_status_err_framing = 0x04,
  led15093_status_err_overrun = 0x05,
  led15093_status_err_buffer_overflow = 0x06,
} led15093_status_t;

typedef enum {
  led15093_report_ok = 0x01,
  led15093_report_wait = 0x02,
  led15093_report_err1 = 0x03,
  led15093_report_err2 = 0x04,
} led15093_report_t;

typedef struct {
  uint8_t sync;
  uint8_t dest;
  uint8_t source;
  uint8_t length;
  uint8_t payload[255];
  uint8_t checksum;
} led15093_frame_t;

#endif /* _APP_BOARD_LED15093_CMD_H */
