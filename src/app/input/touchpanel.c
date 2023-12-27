#include <sys/debug.h>
#include <driver/config.h>
#include <driver/mpr121.h>

#include "touchpanel.h"

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

#define I2C1_PORT GPIOB
#define I2C1_PIN GPIO_PIN_8 | GPIO_PIN_7

#define I2C1_PORT_IRQ GPIOC
#define I2C1_PIN_IRQ GPIO_PIN_8
//  | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11

void touchpanel_init() {

  // init i2c1
  HAL_GPIO_Init(I2C1_PORT, &(GPIO_InitTypeDef) {
    .Pin = I2C1_PIN,
    .Mode = GPIO_MODE_AF_OD,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
    .Alternate = GPIO_AF4_I2C1,
  });

  // init i2c1 irq
  HAL_GPIO_Init(I2C1_PORT_IRQ, &(GPIO_InitTypeDef) {
    .Pin = I2C1_PIN_IRQ,
    .Mode = GPIO_MODE_INPUT,
    .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
  });

  mpr121_init();
  // mpr121_set_irqcb(cb_mpr121_touched);
  // mpr121_enable_irq(true);


}


void touchpanel_enable_irq() {

}
