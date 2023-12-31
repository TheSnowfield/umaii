#include <stddef.h>
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

typedef enum {
  recv_state_header,
  recv_state_body,
} recv_state_t;

typedef struct {
  recv_state_t state;

  led15093_frame_t* current;

  struct {
    uint8_t index;
    led15093_frame_t swap[2];
  } framebuf;

} recv_context_t;

static led15093_passive_cb_t _callback = NULL;
static recv_context_t _recv_ctx = {0};

#define SWAPSIZE ((sizeof(_recv_ctx.framebuf.swap) / sizeof(led15093_frame_t)))

/**
 * @brief exti nvic handler. see stm32f4xx_it.c
 */
void __exti_usart1_handler() {
  HAL_UART_IRQHandler(&_uart1);
}

#define __packet_total_length(p) (4 + (p)->length + 1)

/**
 * @brief swap the frame buffer
 * calling this function when start to recving a new packet
 */
static led15093_frame_t* __packet_swap_frame() {
  
  // ring buffer
  if(++_recv_ctx.framebuf.index >= SWAPSIZE) {
    _recv_ctx.framebuf.index = 0;
  }

  // apply the new frame
  led15093_frame_t* _old = _recv_ctx.current;
  _recv_ctx.current = &_recv_ctx.framebuf.swap[_recv_ctx.framebuf.index];

  return _old;
}

/**
 * @brief receive packet part in interrupt mode
 *
 * @param state the new state
 */
static void __packet_recv_it(recv_state_t state) {

  // packet example
  // E0 11 01 08 | 32 00 20 00 00 00 00 00 | 6C
  //          ^                              ^
  //          data length                    checksum
  switch(_recv_ctx.state = state) {

    // header need 4 bytes
    case recv_state_header:
      HAL_UART_Receive_IT(&_uart1, (uint8_t *)_recv_ctx.current, 4);
    break;

    // receive body and checksum
    case recv_state_body:
      HAL_UART_Receive_IT(&_uart1, _recv_ctx.current->payload, _recv_ctx.current->length + 1);
    break;

  }
}

/**
 * @brief calculate checksum
 *
 * @param frame the packet frame
 * @return uint8_t checksum
 */
static uint8_t __packet_calc_checksum(led15093_frame_t* frame) {

  uint32_t _sum = 0;
  uint8_t* _srtcalc = (uint8_t *)frame + 1;
  uint8_t* _endcalc = _srtcalc + 3 + frame->length;

  // calclate checksum
  for(uint8_t* i = _srtcalc; i < _endcalc; ++i) {
    _sum += *i;
  }

  // teardown low 1 byte
  return _sum & 0xff;
}

/**
 * @brief packet handler
 * do not call this function in the interrupt
 *
 * @param frame the packet frame
 */
static void __packet_handler(led15093_frame_t* frame) {

  // no need to process the packet if no callback is set
  if(_callback == NULL) return;

  switch(frame->payload[0]) {

    // reset the board
    case led15093_cmd_reset:
      _callback(led15093_event_connected);
    break;



  }

}

/**
 * @brief weak function. uart rx complete callback
 *
 * @param huart the uart handle
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

  // start to read the body and checksum
  if(_recv_ctx.state == recv_state_header) {
    __packet_recv_it(recv_state_body);
    return;
  }

  // okay. we have received one packet
  else if (_recv_ctx.state == recv_state_body) {

    // get checksum
    uint8_t _chksum = 0; {
      uint8_t* _ptr = (uint8_t *)_recv_ctx.current;
      _ptr += 4 + _recv_ctx.current->length;
      _chksum = *_ptr;
    }

    // validate the checksum
    // if correct, swap the frame and start parse
    if(__packet_calc_checksum(_recv_ctx.current) == _chksum) {
      run_async(__packet_handler, 1, __packet_swap_frame());
    }

    // restart the recv
    __packet_recv_it(recv_state_header);

    return;
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

  log_info(TAG, "led15093 board initialized");

  // start receive
  __packet_swap_frame();
  __packet_recv_it(recv_state_header);
  log_info(TAG, "listening on the serial data...");

}

void led15093_set_callback(led15093_passive_cb_t cb) {
  _callback = cb;
}

void led15093_mux_uart_tx(uint8_t* data, size_t length) {
  HAL_UART_Transmit(&_uart1, data, length, HAL_MAX_DELAY);
}
