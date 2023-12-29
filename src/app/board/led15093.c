#include <config.h>
#include <sys/debug.h>
#include <hardware/common.h>

#include "led15093.h"
#include "led15093_cmd.h"

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
  // .Init.FIFOMode = DMA_FIFOMODE_ENABLE,
  // .Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL,
  // .Init.MemBurst = DMA_MBURST_SINGLE,
  // .Init.PeriphBurst = DMA_PBURST_SINGLE,
};

static led15093_passive_cb_t _callback = NULL;
static led15093_frame_t _frame = {0};

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  log_debug(TAG, "dma received");
}

/**
 * @brief exti nvic handler. see stm32f4xx_it.c
 */
void __exti_usart1_handler() {
  HAL_UART_IRQHandler(&_uart1);
}

/**
 * @brief exti nvic handler. see stm32f4xx_it.c
 */
void __exti_dma2_s2_handler() {
  HAL_DMA_IRQHandler(&_dma_uart1);
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

    // init dma
    __HAL_LINKDMA(&_uart1, hdmarx, _dma_uart1);
    __HAL_RCC_DMA2_CLK_ENABLE(); {
      HAL_DMA_Init(&_dma_uart1);
      HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
    }

  }

  // start receive
  HAL_StatusTypeDef _s = HAL_UART_Receive_DMA(&_uart1, (uint8_t *)&_frame, sizeof(_frame));

  log_info(TAG, "led15093 board initialized %d", _s);

}

void led15093_set_callback(led15093_passive_cb_t cb) {
  _callback = cb;
}

void led15093_mux_uart_tx(uint8_t* data, size_t length) {
  HAL_UART_Transmit(&_uart1, data, length, HAL_MAX_DELAY);
}
