#include <utils/log.h>
#include "driver/ws2812.h"
#include "driver/config.h"

#include <frost/api.h>
#include <led.h>

#include "ws2812.h"

typedef struct {
  
  uint8_t* buffer;

  struct {

    /**
     * @brief reset signal is ahead of paload data
     * size 32 bytes
     */
    ws2812_color_t* reset;


    /**
     * @brief pixels
     */
    ws2812_color_t* pixels;

  } anchor;

  /**
   * @brief pixel count
   */
  size_t pixel_count;

  /**
   * @brief pixel buffer length
   */
  size_t pixel_buflen;

    /**
   * @brief buff length
   */
  size_t buflen;

} ws2812_framebuf_t;

#define H (ws2812_bit_high)
#define L (ws2812_bit_low)
#define R (ws2812_bit_zero)
#define ALIGN (R,R,R,R,R,R,R,R)
#define RST (ALIGN,ALIGN,ALIGN,ALIGN)

static ws2812_framebuf_t _ws2812_frame;

void ws2812_init(uint8_t leds) {

  // enable peripheral clk
  __HAL_RCC_SPI2_CLK_ENABLE(); {

    // init ws2812 gpio
    HAL_GPIO_Init(GPIO_PORT_WS2812_DOUT, &GPIO_WS2812);

    // init spi peripheral
    if((HAL_SPI_Init(&spi2) != HAL_OK)) {
      log_fatal(TAG, "init ws2812 peripheral failed");
      return;
    }
  }

  // prepare led buffer
  _ws2812_frame.pixel_count = leds;
  _ws2812_frame.buflen = sizeof(ws2812_color_t) * (4 + leds);
  _ws2812_frame.buffer = malloc(_ws2812_frame.buflen);
  _ws2812_frame.anchor.reset = (ws2812_color_t *)_ws2812_frame.buffer;
  _ws2812_frame.anchor.pixels = _ws2812_frame.anchor.reset + 4;
  if(!_ws2812_frame.buffer) {
    log_fatal(TAG, "init ws2812 frame buffer failed");
    return;
  }
  
  // the reset signal
  memset(_ws2812_frame.anchor.reset, ws2812_bit_zero, sizeof(ws2812_color_t) * 4);

  // clear with black(all off)
  ws2812_clear(ws2812_rgb(0, 0, 0));
}

void ws2812_clear(ws2812_color_t color) {
  for(size_t i = 0; i < _ws2812_frame.pixel_count; ++i) {
    _ws2812_frame.anchor.pixels[i] = color;
  }
}

void ws2812_set_pixel(size_t index, ws2812_color_t color) {
  _ws2812_frame.anchor.pixels[index] = color;
}

void ws2812_dma_enable(bool en) {

  if(en) {

    // enable dma
    __HAL_RCC_DMA1_CLK_ENABLE(); {
      HAL_DMA_Init(&dma_spi2);
    }

    // link dma with spi2 tx
    __HAL_LINKDMA(&spi2, hdmatx, dma_spi2);

    // kick start
    HAL_SPI_Transmit_DMA(&spi2, _ws2812_frame.buffer, _ws2812_frame.buflen);

    return;
  }

}

bool ws2812_update() {
  return HAL_SPI_Transmit(&spi2, _ws2812_frame.buffer, _ws2812_frame.buflen, HAL_MAX_DELAY) == HAL_OK;
}
