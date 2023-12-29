#include <config.h>
#include <sys/debug.h>
#include <hardware/common.h>
#include <hardware/mpr121.h>

#include "touchpanel.h"

static I2C_HandleTypeDef i2c1 = {
  .Instance = I2C1,
  .Init.ClockSpeed = 400000, // 400Khz Fast Mode
  .Init.DutyCycle = I2C_DUTYCYCLE_2,
  .Init.OwnAddress1 = 0x00,
  .Init.OwnAddress2 = 0x00,
  .Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,
  .Init.DualAddressMode = I2C_DUALADDRESS_DISABLE,
  .Init.GeneralCallMode = I2C_DUALADDRESS_DISABLE,
  .Init.NoStretchMode = I2C_NOSTRETCH_DISABLE
};

static UART_HandleTypeDef uart3 = {
  .Instance = USART3,
  .Init.BaudRate = UMAIIDX_TOUCHPANEL_BAUDRATE,
  .Init.WordLength = UART_WORDLENGTH_8B,
  .Init.StopBits = UART_STOPBITS_1,
  .Init.Parity = UART_PARITY_NONE,
  .Init.Mode = UART_MODE_TX_RX,
  .Init.HwFlowCtl = UART_HWCONTROL_NONE,
  .Init.OverSampling = UART_OVERSAMPLING_16
};

#define I2C1_PORT GPIOB
#define I2C1_PIN GPIO_PIN_8 | GPIO_PIN_7

#define I2C1_PORT_IRQ GPIOC
#define I2C1_PIN_IRQ GPIO_PIN_8
//  | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11

void touchpanel_init() {

  __HAL_RCC_USART3_CLK_ENABLE(); {

    // init gpio pinmux
    HAL_GPIO_Init(GPIOC, &(GPIO_InitTypeDef) {
      .Pin = GPIO_PIN_10 | GPIO_PIN_11,
      .Mode = GPIO_MODE_AF_PP,
      .Pull = GPIO_NOPULL,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
      .Alternate = GPIO_AF7_USART3
    });

    // init usart
    HAL_UART_Init(&uart3);
  }

  // init i2c1 and its irq pin
  __HAL_RCC_I2C1_CLK_ENABLE(); {
    HAL_GPIO_Init(I2C1_PORT, &(GPIO_InitTypeDef) {
      .Pin = I2C1_PIN,
      .Mode = GPIO_MODE_AF_OD,
      .Pull = GPIO_PULLUP,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
      .Alternate = GPIO_AF4_I2C1,
    });

    HAL_GPIO_Init(I2C1_PORT_IRQ, &(GPIO_InitTypeDef) {
      .Pin = I2C1_PIN_IRQ,
      .Mode = GPIO_MODE_INPUT,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
    });
  }

  // init mpr121 driver
  mpr121_init();
  // mpr121_set_irqcb(cb_mpr121_touched);
  // mpr121_enable_irq(true);

}


void touchpanel_enable_irq() {

}
