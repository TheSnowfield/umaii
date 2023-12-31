#ifndef _APP_BOARD_LED15093_FRAME_H
#define _APP_BOARD_LED15093_FRAME_H

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

typedef struct {
  uint8_t sync;
  uint8_t dest;
  uint8_t source;
  uint8_t length;
  uint8_t payload[255 + 1];
} led15093_frame_t;

#endif /* _APP_BOARD_LED15093_FRAME_H */
