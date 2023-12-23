#ifndef _SYS_HARDWARE_CONFIG_H
#define _SYS_HARDWARE_CONFIG_H

#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_i2c.h>
#include <stm32f4xx_hal_i2c_ex.h>
#include <stm32f4xx_hal_spi.h>
#include <stm32f4xx_hal_gpio.h>
#include <stm32f4xx_hal_dma.h>
#include <stm32f4xx_hal_dma_ex.h>
#include <stm32f4xx_hal_tim.h>
#include <stm32f4xx_hal_tim_ex.h>
#include <stm32f4xx_hal_rtc.h>
#include <stm32f4xx_hal_pwr.h>
#include <stm32f4xx_hal_pcd.h>
#include <stm32f4xx_hal_pcd_ex.h>

#define SERIAL_BAUDRATE 115200

#define I2C1_PORT GPIOB
#define I2C1_PIN_SCL GPIO_PIN_8
#define I2C1_PIN_SDA GPIO_PIN_7
#define I2C1_PIN_IRQ GPIO_PIN_9

#define I2C2_PORT GPIOB
#define I2C2_PIN_SCL GPIO_PIN_10
#define I2C2_PIN_SDA GPIO_PIN_3

#define RTC_BKUPREG_RESET RTC_BKP_DR0

// RTC
////////////////////////////////////////////////////////
extern RTC_HandleTypeDef rtc;

// GPIO Ports
////////////////////////////////////////////////////////

#define GPIO_PORT_SWDEBUG GPIOA
extern GPIO_InitTypeDef GPIO_SWDEBUG;

#define GPIO_PORT_LED GPIOD
extern GPIO_InitTypeDef GPIO_LED;

#define GPIO_PORT_CH334F_RST GPIOC
extern GPIO_InitTypeDef GPIO_CH334F_RST;

// Game Buttons
////////////////////////////////////////////////////////
#define GPIO_PORT_GAMEBTN GPIOA
extern GPIO_InitTypeDef GPIO_GAMEBTN;

// USB Composite Device
////////////////////////////////////////////////////////

#define GPIO_PORT_USB GPIOA
extern GPIO_InitTypeDef GPIO_USB;

// WS2812 LED Strip
#define GPIO_PORT_WS2812_DOUT GPIOB
extern GPIO_InitTypeDef GPIO_WS2812;
extern SPI_HandleTypeDef spi2;
extern DMA_HandleTypeDef dma_spi2;

// I2C1 is used for MPR121
////////////////////////////////////////////////////////
extern GPIO_InitTypeDef port_i2c1;
extern I2C_HandleTypeDef i2c1;

// I2C2 is used for MPR121
////////////////////////////////////////////////////////
// extern GPIO_InitTypeDef port_i2c2_scl;
// extern GPIO_InitTypeDef port_i2c2_sda;
// extern I2C_HandleTypeDef i2c2;

// I2C3 is used for MPR121
////////////////////////////////////////////////////////
extern GPIO_InitTypeDef port_i2c3_scl;
extern GPIO_InitTypeDef port_i2c3_sda;
extern I2C_HandleTypeDef i2c3;

// I2C4 is used for MPR121
////////////////////////////////////////////////////////
// extern GPIO_InitTypeDef port_i2c4_scl;
// extern GPIO_InitTypeDef port_i2c4_sda;
// extern I2C_HandleTypeDef i2c4;

// SPI1 is used for WS2812
////////////////////////////////////////////////////////
extern SPI_HandleTypeDef spi1;
extern GPIO_InitTypeDef port_spi1;
// extern DMA_HandleTypeDef dma_spi1;

// Button keys
////////////////////////////////////////////////////////
extern GPIO_InitTypeDef port_buttons;

extern GPIO_InitTypeDef port_usart1;
extern UART_HandleTypeDef usart1;

#endif /* _SYS_HARDWARE_CONFIG_H */
