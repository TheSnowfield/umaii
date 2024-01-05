#include <stddef.h>
#include <stdbool.h>
#include <config.h>
#include <sys/debug.h>
#include <hardware/common.h>
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
    led15093_frame_t pool[UMAIIDX_LED15093_QSIZE];
  } swap;

  struct {
    size_t recv;
    uint8_t buffer[UMAIIDX_LED15093_DMABUFFER];
  } dma;

} recv_context_t;

static led15093_passive_cb_t _callback = NULL;
static recv_context_t _recv_ctx = {0};

void __exti_usart1_handler() {
  HAL_UART_IRQHandler(&_uart1);
}

void __exti_dma2s2_handler() {
  HAL_DMA_IRQHandler(&_dma_uart1);
}

/**
 * @brief swap the frame buffer
 * calling this function when starts to receiving a new packet
 */
static led15093_frame_t* __packet_swap_frame() {
  
  // ring buffer
  if(++_recv_ctx.swap.index >= UMAIIDX_LED15093_QSIZE) {
    _recv_ctx.swap.index = 0;
  }

  // apply the new frame
  led15093_frame_t* _old = _recv_ctx.state.ref;
  _recv_ctx.state.ref = &_recv_ctx.swap.pool[_recv_ctx.swap.index];

  return _old;
}

/**
 * @brief receive data stream to idle
 */
static void __packet_recv_dma() {
  HAL_UARTEx_ReceiveToIdle_DMA(&_uart1, _recv_ctx.dma.buffer, UMAIIDX_LED15093_DMABUFFER);
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

  // I decided the checksum was useless here due to the protocol
  // has its sync byte as 0xE0.

  // When the checksum is not correct we have to drop it,
  // because we can't recover the data of the packet or request host to re-transmit.

  // Thus even if the packet is broken, we completely can wait for the
  // sync byte in the next, to reset the context and then continue receiving (drop it).

  for(; i < _end; ++i) switch(*i) {

    // the sync byte
    case '\xe0':
      __state_reset(_recv_ctx);
      __state_escape_unset(_recv_ctx);
      __state_write(_recv_ctx, *i, false);
    break;

    // unescape byte by add 1
    case '\xd0':
      if(__state_is_escape(_recv_ctx)) {
        __state_write(_recv_ctx, (*i + __state_is_escape(_recv_ctx)), true);
        __state_escape_unset(_recv_ctx);
      }
      else __state_escape_set(_recv_ctx);
    break;

    // default. write a byte to buffer
    default:
    
      // write data(or unescape) to the buffer
      __state_write(_recv_ctx, (*i + __state_is_escape(_recv_ctx)), true);
      __state_escape_unset(_recv_ctx);

      // compare packet is recv complete
      if(__state_is_complete(_recv_ctx) && _callback) {
        run_async(_callback, 1, __packet_swap_frame());
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

  // init uart rx dma stream
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
