#include <stddef.h>
#include <stdbool.h>
#include <config.h>
#include <sys/debug.h>
#include <hardware/common.h>
#include <app/led.h>
#include <frost/api.h>
#include <frost/tricks.h>

#include "led15093.h"
#include "led15093_cmd.h"
#include "led15093_frame.h"

static UART_HandleTypeDef _uart1 = {
  .Instance = USART1,
  .Init.BaudRate= UMAIIDX_LIGHT_BAUDRATE,
  .Init.WordLength = UART_WORDLENGTH_8B,
  .Init.StopBits = UART_STOPBITS_1,
  .Init.Parity = UART_PARITY_NONE,
  .Init.Mode = UART_MODE_TX_RX,
  .Init.HwFlowCtl = UART_HWCONTROL_NONE,
  .Init.OverSampling = UART_OVERSAMPLING_16,
};

static DMA_HandleTypeDef _dma_uart1 = {
  .Instance = DMA2_Stream2,
  .Init.Channel = DMA_CHANNEL_4,
  .Init.Direction = DMA_PERIPH_TO_MEMORY,
  .Init.PeriphInc = DMA_PINC_DISABLE,
  .Init.MemInc = DMA_MINC_ENABLE,
  .Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
  .Init.MemDataAlignment = DMA_MDATAALIGN_BYTE,
  .Init.Mode = DMA_CIRCULAR,
  .Init.Priority = DMA_PRIORITY_LOW,
  .Init.FIFOMode = DMA_FIFOMODE_DISABLE,
};

#define DMASIZE 512
#define SWAPSIZE 64

typedef struct {
  bool init;

  struct {
    size_t len;
    size_t checksum;
    uint8_t escape;
    led15093_frame_t* ref;
  } state;

  struct {
    uint8_t index;
    led15093_frame_t pool[SWAPSIZE];
  } swap;

  struct {
    size_t recv;
    uint8_t buffer[DMASIZE];
  } dma;

} recv_context_t;

static led15093_passive_cb_t _callback = NULL;
static recv_context_t _recv_ctx = {0};

/**
 * @brief exti nvic handler. see stm32f4xx_it.c
 */
void __exti_usart1_handler() {
  HAL_UART_IRQHandler(&_uart1);
}

void DMA2_Stream2_IRQHandler() {
  HAL_DMA_IRQHandler(&_dma_uart1);
}

#define __packet_total_length(p) (4 + (p)->length + 1)

/**
 * @brief swap the frame buffer
 * calling this function when start to recving a new packet
 */
static led15093_frame_t* __packet_swap_frame() {
  
  // ring buffer
  if(++_recv_ctx.swap.index >= SWAPSIZE) {
    _recv_ctx.swap.index = 0;
  }

  // apply the new frame
  led15093_frame_t* _old = _recv_ctx.state.ref;
  _recv_ctx.state.ref = &_recv_ctx.swap.pool[_recv_ctx.swap.index];

  return _old;
}

/**
 * @brief receive packet part in interrupt mode
 */
static void __packet_recv_dma() {
  HAL_UARTEx_ReceiveToIdle_DMA(&_uart1, _recv_ctx.dma.buffer, DMASIZE);
}

// /**
//  * @brief calculate checksum
//  *
//  * @param frame the packet frame
//  * @return uint8_t checksum
//  */
// static uint8_t __packet_calc_checksum(led15093_frame_t* frame) {

//   uint32_t _sum = 0;
//   uint8_t* _srtcalc = (uint8_t *)frame + 1;
//   uint8_t* _endcalc = _srtcalc + 3 + frame->length;

//   // calclate checksum
//   for(uint8_t* i = _srtcalc; i < _endcalc; ++i) {
//     _sum += *i;
//   }

//   // teardown low 1 byte
//   return _sum & 0xff;
// }


/**
 * @brief packet handler
 * do not call this function in the interrupt
 *
 * @param frame the packet frame
 */
static void __packet_handler(led15093_frame_t* frame) {

  // no need to process the packet if no callback is set
  if(_callback == NULL) return;

  switch(frame->cmd) {

    // reset the board
    case led15093_cmd_reset:
      if(!_recv_ctx.init) {
        _callback(led15093_event_connected);
        _recv_ctx.init = true;
      }
    break;

    // turn board into slient mode
    case led15093_cmd_set_disable_response:
      led_toggle();
    break;

  }

}

#define __state_escape_set(x) ((x).state.escape = 1)
#define __state_escape_unset(x) ((x).state.escape = 0)
#define __state_is_escape(x) ((x).state.escape == 1)
#define __state_write(x, y, z) { \
  *((x).state.ref->buffer.data + (x).state.len) = (y); \
  (x).state.len++; \
  (x).state.checksum += (z) ? (y) : 0; \
}

#define __state_reset(x) { \
  (x).state.len = 0; \
  (x).state.checksum = 0; \
}

#define __state_is_complete(x) \
  (((x).state.len > 4) && (((x).state.len) - 5) == ((x).state.ref->header.length))

static size_t __packet_recv_parse(uint8_t* data, size_t length) {

  uint8_t* i = data;
  uint8_t* _end = data + length;
  
  // packet example 1
  // E0 11 01 08 | 32 00 20 00 00 00 00 00 | 6C
  //          ^                              ^
  //          data length                    checksum

  // packet example 2
  // E0 11 01 04 | 39 | D0 CF | D0 CF | FF EE
  //          ^         ^       ^          ^
  //          length    escape  escape     checksum

  for(; i < _end; ++i) switch(*i) {

    // the sync byte
    case '\xe0':
      __state_reset(_recv_ctx);
      __state_escape_unset(_recv_ctx);
      __state_write(_recv_ctx, *i, false);
    break;

    // unescape byte by add 1
    case '\x0d':
      if(__state_is_escape(_recv_ctx)) {
        __state_write(_recv_ctx, (*i + __state_is_escape(_recv_ctx)), true);
        __state_escape_unset(_recv_ctx);
      }
      else __state_escape_set(_recv_ctx);
    break;

    // default. write a byte to buffer
    default:
    
      // write data to the buffer
      __state_write(_recv_ctx, (*i + __state_is_escape(_recv_ctx)), true);
      __state_escape_unset(_recv_ctx);

      // compare packet is recv complete
      if(__state_is_complete(_recv_ctx)) {
        run_async(__packet_handler, 1, __packet_swap_frame());
        __state_reset(_recv_ctx);
      }

    break;
  }

  // log_debug(TAG, "recv: %d", length);
  return length;
}

#define __dma_recv(x) ((x).dma.recv)
#define __dma_recv_clr(x) ((x).dma.recv = 0)
#define __dma_recv_inc(x, y) ((x).dma.recv += (y))
#define __dma_recv_buf(x) ((x).dma.buffer + (x).dma.recv)

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size) {

  size_t _recvlen = __dma_recv(_recv_ctx);
  if(size == _recvlen) return;

  // reset recv counter
  if(size < _recvlen) {
    __dma_recv_clr(_recv_ctx);
  }

  // when received to idle, start process the bytes
  else if(_recvlen < size) {
    size_t _read = __packet_recv_parse(__dma_recv_buf(_recv_ctx), size - _recvlen);
    __dma_recv_inc(_recv_ctx, _read);
  }
}

void led15093_init() {

  // init uart
  __HAL_RCC_USART1_CLK_ENABLE(); {

    // init gpio pinmux
    HAL_GPIO_Init(GPIOA, &(GPIO_InitTypeDef) {
      .Pin = GPIO_PIN_9 | GPIO_PIN_10,
      .Mode = GPIO_MODE_AF_PP,
      .Pull = GPIO_NOPULL,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
      .Alternate = GPIO_AF7_USART1
    });

    // init usart
    HAL_UART_Init(&_uart1);
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }

  __HAL_RCC_DMA2_CLK_ENABLE(); {
    HAL_DMA_Init(&_dma_uart1);
    __HAL_LINKDMA(&_uart1, hdmarx, _dma_uart1);
    HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  }

  log_info(TAG, "led15093 board initialized");

  // start receive
  __packet_swap_frame();
  __packet_recv_dma();
  log_info(TAG, "listening on the serial data...");

}

void led15093_set_callback(led15093_passive_cb_t cb) {
  _callback = cb;
}

void led15093_mux_uart_tx(uint8_t* data, size_t length) {
  HAL_UART_Transmit(&_uart1, data, length, HAL_MAX_DELAY);
}
