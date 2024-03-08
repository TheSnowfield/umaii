#ifndef _APP_BOARD_LED15070_FRAME_H
#define _APP_BOARD_LED15070_FRAME_H

/**
 * best thanks to segatool for offering this reference!
 * :3
*/

/* Frame structure:

   [0] Sync byte (0xE0)
   [1] Destination address
   [2] Source Address
   [3] Length of data/payload
   [4] Data/payload
        For requests (host to board):
            [0] Command
            ... Payload
        For responses (board to host):
            [0] Status
            [1] Command
            [2] Report
            ... Payload
   [n] Checksum: Sum of all prior bytes (excluding sync byte)

   Byte stuffing:

   0xD0 is an escape byte. Un-escape the subsequent byte by adding 1. */

#include <stdint.h>
#include <compiler/packed.h>

typedef struct {
  uint8_t sync;
  uint8_t dest;
  uint8_t source;
  uint8_t length;
} pack1 led15070_header_t;

typedef struct {
  uint8_t index;
  uint8_t r;
  uint8_t g;
  uint8_t b;
} pack1 led15070_led_color_t;

typedef struct {
  uint8_t start;
  uint8_t end;
  uint8_t skip;
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t speed;
} pack1 led15070_led_batch_t;

typedef union {
  
  struct {
    uint8_t data[sizeof(led15070_header_t) + 255 + 1];
  } buffer;

  struct {
    led15070_header_t header;
    uint8_t cmd;

    union {

      struct {
        uint8_t data[254 + 1];
      } common;

      led15070_led_color_t pkt_ledctl;
      led15070_led_batch_t pkt_batchctl;
    } payload;

  };

} led15070_frame_t;

#endif /* _APP_BOARD_LED15070_FRAME_H */
