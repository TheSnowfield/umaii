#ifndef _SYS_HARDWARE_DRIVER_WS2812_H
#define _SYS_HARDWARE_DRIVER_WS2812_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
  uint8_t g[8];
  uint8_t r[8];
  uint8_t b[8];
} ws2812_color_t;

typedef enum {

  // TH + TL = 1.25us + ±600ns
  // 0 code: high voltage 0.35us, low 0.8us
  // 1 code: high voltage 0.7us,  low 0.6us

  // so right here, the low pulse (0) is defined to 0xc0
  //   _
  //  | |______
  //  1100 0000

  ws2812_bit_low = 0b1100000,

  // and the high pulse (1) is defined to 0xf8
  //   ____
  //  |    |___
  //  1111 1000

  ws2812_bit_high = 0b11111000,

  // this is for reset use
  ws2812_bit_zero = 0x00

} ws2812_bit_t;

#define __make_clrbit(_ch, _bit) \
  ((_ch & _bit) ? ws2812_bit_high : ws2812_bit_low)

#define __inflate_ch(_ch) { \
    __make_clrbit(_ch, 0b10000000), \
    __make_clrbit(_ch, 0b01000000), \
    __make_clrbit(_ch, 0b00100000), \
    __make_clrbit(_ch, 0b00010000), \
    __make_clrbit(_ch, 0b00001000), \
    __make_clrbit(_ch, 0b00000100), \
    __make_clrbit(_ch, 0b00000010), \
    __make_clrbit(_ch, 0b00000001) \
  }

#define ws2812_rgb(_r, _g, _b) \
  ((ws2812_color_t) { \
    .b = __inflate_ch(_b), \
    .g = __inflate_ch(_g), \
    .r = __inflate_ch(_r) \
  })

void ws2812_init(uint8_t leds);
void ws2812_clear(ws2812_color_t color);
void ws2812_dma_enable(bool en);
void ws2812_set_pixel(size_t index, ws2812_color_t color);

#endif /* _SYS_HARDWARE_DRIVER_WS2812_H */
