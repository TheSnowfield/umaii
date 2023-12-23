#include "config.h"

GPIO_InitTypeDef GPIO_SWDEBUG = {
  .Pin = GPIO_PIN_13 | GPIO_PIN_14,
  .Mode = GPIO_MODE_AF_PP,
  .Speed = GPIO_SPEED_FREQ_HIGH,
  .Alternate = GPIO_AF0_SWJ
};

GPIO_InitTypeDef GPIO_LED = {
  .Pin = GPIO_PIN_2,
  .Mode = GPIO_MODE_OUTPUT_PP,
  .Speed = GPIO_SPEED_LOW,
  .Pull = GPIO_PULLDOWN
};

GPIO_InitTypeDef GPIO_CH334F_RST = {
  .Pin = GPIO_PIN_7,
  .Mode = GPIO_MODE_OUTPUT_PP,
  .Speed = GPIO_SPEED_LOW,
  .Pull = GPIO_PULLDOWN
};

GPIO_InitTypeDef GPIO_USB = {
  .Pin = GPIO_PIN_11 | GPIO_PIN_12,
  .Mode = GPIO_MODE_AF_PP,
  .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
  .Pull = GPIO_NOPULL,
  .Alternate = GPIO_AF10_OTG_FS
};

GPIO_InitTypeDef GPIO_GAMEBTN = {
  .Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
         GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_15,
  .Mode = GPIO_MODE_IT_FALLING,
  .Pull = GPIO_PULLDOWN,
  .Speed = GPIO_SPEED_FREQ_VERY_HIGH
};

GPIO_InitTypeDef GPIO_WS2812 = {
  .Pin = GPIO_PIN_15,
  .Mode = GPIO_MODE_AF_OD,
  .Pull = GPIO_PULLDOWN,
  .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
  .Alternate = GPIO_AF5_SPI2
};

SPI_HandleTypeDef spi2 = {
  .Instance = SPI2,
  .Init.Mode = SPI_MODE_MASTER,
  .Init.Direction = SPI_DIRECTION_1LINE,
  .Init.DataSize = SPI_DATASIZE_8BIT,
  .Init.CLKPolarity = SPI_POLARITY_LOW,
  .Init.CLKPhase = SPI_PHASE_2EDGE,
  .Init.NSS = SPI_NSS_SOFT,
  .Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16,
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

GPIO_InitTypeDef port_usart1 = {
  .Pin = GPIO_PIN_9 | GPIO_PIN_10,
  .Mode = GPIO_MODE_AF_PP,
  .Pull = GPIO_NOPULL,
  .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
  .Alternate = GPIO_AF7_USART1
};

UART_HandleTypeDef usart1 = {
  .Instance = USART1,
  .Init.BaudRate= SERIAL_BAUDRATE,
  .Init.WordLength = UART_WORDLENGTH_8B,
  .Init.StopBits = UART_STOPBITS_1,
  .Init.Parity = UART_PARITY_NONE,
  .Init.Mode = UART_MODE_TX_RX,
  .Init.HwFlowCtl = UART_HWCONTROL_NONE,
  .Init.OverSampling = UART_OVERSAMPLING_16
};

GPIO_InitTypeDef port_i2c1 = {
  .Pin = I2C1_PIN_SDA | I2C1_PIN_SCL,
  .Mode = GPIO_MODE_AF_OD,
  .Pull = GPIO_PULLUP,
  .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
  .Alternate = GPIO_AF4_I2C1,
};

GPIO_InitTypeDef port_i2c1_irq = {
  .Pin = I2C1_PIN_IRQ,
  .Mode = GPIO_MODE_INPUT,
  .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
};

I2C_HandleTypeDef i2c1 = {
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

RTC_HandleTypeDef rtc = {
  .Instance = RTC,
};
