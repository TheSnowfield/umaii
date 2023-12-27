#include <sys/debug.h>
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

#define WS2812_PORT GPIOB
#define WS2812_PIN GPIO_PIN_15

SPI_HandleTypeDef spi2 = {
  .Instance = SPI2,
  .Init.Mode = SPI_MODE_MASTER,
  .Init.Direction = SPI_DIRECTION_1LINE,
  .Init.DataSize = SPI_DATASIZE_8BIT,
  .Init.CLKPolarity = SPI_POLARITY_LOW,
  .Init.CLKPhase = SPI_PHASE_2EDGE,
  .Init.NSS = SPI_NSS_SOFT,
  .Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8,
  .Init.FirstBit = SPI_FIRSTBIT_MSB,
  .Init.TIMode = SPI_TIMODE_DISABLE,
  .Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE,
  .Init.CRCPolynomial = 7
};

DMA_HandleTypeDef dma_spi2 = {
  .Instance = DMA1_Stream4,
  .Init.Channel = DMA_CHANNEL_0,
  .Init.Direction = DMA_MEMORY_TO_PERIPH,
  .Init.PeriphInc = DMA_PINC_DISABLE,
  .Init.MemInc = DMA_MINC_ENABLE,
  .Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
  .Init.MemDataAlignment = DMA_MDATAALIGN_BYTE,
  .Init.Mode = DMA_CIRCULAR,
  .Init.Priority = DMA_PRIORITY_LOW,
  .Init.FIFOMode = DMA_FIFOMODE_DISABLE
};

static ws2812_framebuf_t _ws2812_frame;

void ws2812_init(uint8_t leds) {

  // enable peripheral clk
  __HAL_RCC_SPI2_CLK_ENABLE(); {

    // init ws2812 gpio
    HAL_GPIO_Init(WS2812_PORT, &(GPIO_InitTypeDef) {
      .Pin = WS2812_PIN,
      .Mode = GPIO_MODE_AF_OD,
      .Pull = GPIO_PULLDOWN,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
      .Alternate = GPIO_AF5_SPI2
    });

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
