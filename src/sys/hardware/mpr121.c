#include <hardware/common.h>
#include <sys/fault.h>
#include <sys/debug.h>
#include "common/iic.h"
#include "mpr121.h"

#define MPR121_ADDR (0x5a << 1)

static mpr121_irqcb_t _callback;
static const iic_cmddef_t _cmd_init[] = {

  // chip reset
  { MPR121_REG_SOFT_RESET, 50, 1, bytes { 0x63 }},


  // { MPR121_REG_AUTO_CONFIG_CONTROL_1, 0, 2, bytes { 0x00, 0x00 }},
  // { MPR121_REG_AUTO_CONFIG_USL, 0, 2, bytes { 0x00, 0x00 }},
  // { MPR121_REG_AUTO_CONFIG_LSL, 0, 2, bytes { 0x00, 0x00 }},
  // { MPR121_REG_AUTO_CONFIG_TARGET_LEVEL, 0, 2, bytes { 0x00, 0x00 }},

  // set auto configuration
  // { MPR121_REG_AUTO_CONFIG_CONTROL_0, 0, 1, bytes { 0x0b }},

  // set baseline filtering

  // set touch and release threshold
  // { MPR121_REG_ELE0_TOUCH_THRESHOLD, 0, 2, bytes { 0x0f, 0x0a }},
  // { MPR121_REG_ELE0_RELEASE_THRESHOLD, 0, 2, bytes { 0x0a, 0x05 }},
  // { MPR121_REG_ELE1_TOUCH_THRESHOLD, 0, 2, bytes { 0x0f, 0x0a }},
  // { MPR121_REG_ELE1_RELEASE_THRESHOLD, 0, 2, bytes { 0x0a, 0x05 }},
  // { MPR121_REG_ELE2_TOUCH_THRESHOLD, 0, 2, bytes { 0x0f, 0x0a }},
  // { MPR121_REG_ELE2_RELEASE_THRESHOLD, 0, 2, bytes { 0x0a, 0x05 }},
  // { MPR121_REG_ELE3_TOUCH_THRESHOLD, 0, 2, bytes { 0x0f, 0x0a }},
  // { MPR121_REG_ELE3_RELEASE_THRESHOLD, 0, 2, bytes { 0x0a, 0x05 }},
  // { MPR121_REG_ELE4_TOUCH_THRESHOLD, 0, 2, bytes { 0x0f, 0x0a }},
  // { MPR121_REG_ELE4_RELEASE_THRESHOLD, 0, 2, bytes { 0x0a, 0x05 }},
  // { MPR121_REG_ELE5_TOUCH_THRESHOLD, 0, 2, bytes { 0x0f, 0x0a }},
  // { MPR121_REG_ELE5_RELEASE_THRESHOLD, 0, 2, bytes { 0x0a, 0x05 }},
  // { MPR121_REG_ELE6_TOUCH_THRESHOLD, 0, 2, bytes { 0x0f, 0x0a }},
  // { MPR121_REG_ELE6_RELEASE_THRESHOLD, 0, 2, bytes { 0x0a, 0x05 }},
  // { MPR121_REG_ELE7_TOUCH_THRESHOLD, 0, 2, bytes { 0x0f, 0x0a }},
  // { MPR121_REG_ELE7_RELEASE_THRESHOLD, 0, 2, bytes { 0x0a, 0x05 }},
  // { MPR121_REG_ELE8_TOUCH_THRESHOLD, 0, 2, bytes { 0x0f, 0x0a }},
  // { MPR121_REG_ELE8_RELEASE_THRESHOLD, 0, 2, bytes { 0x0a, 0x05 }},
  // { MPR121_REG_ELE9_TOUCH_THRESHOLD, 0, 2, bytes { 0x0f, 0x0a }},
  // { MPR121_REG_ELE9_RELEASE_THRESHOLD, 0, 2, bytes { 0x0a, 0x05 }},
  // { MPR121_REG_ELE10_TOUCH_THRESHOLD, 0, 2, bytes { 0x0f, 0x0a }},
  // { MPR121_REG_ELE10_RELEASE_THRESHOLD, 0, 2, bytes { 0x0a, 0x05 }},
  // { MPR121_REG_ELE11_TOUCH_THRESHOLD, 0, 2, bytes { 0x0f, 0x0a }},
  // { MPR121_REG_ELE11_RELEASE_THRESHOLD, 0, 2, bytes { 0x0a, 0x05 }},
  // { MPR121_REG_ELEPROX_TOUCH_THRESHOLD, 0, 2, bytes { 0x0f, 0x0a }},
  // { MPR121_REG_ELEPROX_RELEASE_THRESHOLD, 0, 2, bytes { 0x0a, 0x05 }},

  // set electrode configuration
  // { MPR121_REG_ELECTRODE_CONFIGURATION, 0, 2, bytes { 0x0f, 0x00 }},
  // { MPR121_REG_ELE0_ELE1_CHARGE_TIME, 0, 2, bytes { 0x00, 0x00 }},
  // { MPR121_REG_ELE2_ELE3_CHARGE_TIME, 0, 2, bytes { 0x00, 0x00 }},
  // { MPR121_REG_ELE4_ELE5_CHARGE_TIME, 0, 2, bytes { 0x00, 0x00 }},
  // { MPR121_REG_ELE6_ELE7_CHARGE_TIME, 0, 2, bytes { 0x00, 0x00 }},
  // { MPR121_REG_ELE8_ELE9_CHARGE_TIME, 0, 2, bytes { 0x00, 0x00 }},
  // { MPR121_REG_ELE10_ELE11_CHARGE_TIME, 0, 2, bytes { 0x00, 0x00 }},
  // { MPR121_REG_ELEPROX_CHARGE_TIME, 0, 2, bytes { 0x00, 0x00 }},

  // set electrode current
  // { MPR121_REG_ELE0_CURRENT, 0, 1, bytes { 0x0f }},
  // { MPR121_REG_ELE1_CURRENT, 0, 1, bytes { 0x0f }},
  // { MPR121_REG_ELE2_CURRENT, 0, 1, bytes { 0x0f }},
  // { MPR121_REG_ELE3_CURRENT, 0, 1, bytes { 0x0f }},
  // { MPR121_REG_ELE4_CURRENT, 0, 1, bytes { 0x0f }},
  // { MPR121_REG_ELE5_CURRENT, 0, 1, bytes { 0x0f }},
  // { MPR121_REG_ELE6_CURRENT, 0, 1, bytes { 0x0f }},
  // { MPR121_REG_ELE7_CURRENT, 0, 1, bytes { 0x0f }},
  // { MPR121_REG_ELE8_CURRENT, 0, 1, bytes { 0x0f }},
  // { MPR121_REG_ELE9_CURRENT, 0, 1, bytes { 0x0f }},
  // { MPR121_REG_ELE10_CURRENT, 0, 1, bytes { 0x0f }},
  // { MPR121_REG_ELE11_CURRENT, 0, 1, bytes { 0x0f }},
  // { MPR121_REG_ELEPROX_CURRENT, 0, 1, bytes { 0x0f }},


  { MPR121_REG_MHD_RISING, 0, 1, bytes { 0x01 }},
  { MPR121_REG_NHD_AMOUNT_RISING, 0, 1, bytes { 0x01 }},
  { MPR121_REG_NCL_RISING, 0, 1, bytes { 0x0e }},
  { MPR121_REG_FDL_RISING, 0, 1, bytes { 0x00 }},

  { MPR121_REG_MHD_FALLING, 0, 1, bytes { 0x01 }},
  { MPR121_REG_NHD_AMOUNT_FALLING, 0, 1, bytes { 0x05 }},
  { MPR121_REG_NCL_FALLING, 0, 1, bytes { 0x01 }},
  { MPR121_REG_FDL_FALLING, 0, 1, bytes { 0x00 }},

  { MPR121_REG_NHD_AMOUNT_TOUCHED, 0, 1, bytes { 0x00 }},
  { MPR121_REG_NCL_TOUCHED, 0, 1, bytes { 0x00 }},
  { MPR121_REG_FDL_TOUCHED, 0, 1, bytes { 0x00 }},

  // { MPR121_REG_AUTO_CONFIG_CONTROL_0, 0, 1, bytes { 0x0b }},
  // { MPR121_REG_AUTO_CONFIG_USL, 0, 1, bytes {200}},
  // { MPR121_REG_AUTO_CONFIG_LSL, 0, 1, bytes {130}},
  // { MPR121_REG_AUTO_CONFIG_TARGET_LEVEL, 0, 1, bytes {180}},

  // enable electrodes
  { MPR121_REG_ELECTRODE_CONFIGURATION, 0 , 1 , bytes { 0b11110011 }},

  END_OF_CMD
};

static I2C_HandleTypeDef _i2c_ch0 = {
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

static I2C_HandleTypeDef _i2c_ch1 = {
  .Instance = I2C2,
  .Init.ClockSpeed = 400000, // 400Khz Fast Mode
  .Init.DutyCycle = I2C_DUTYCYCLE_2,
  .Init.OwnAddress1 = 0x00,
  .Init.OwnAddress2 = 0x00,
  .Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,
  .Init.DualAddressMode = I2C_DUALADDRESS_DISABLE,
  .Init.GeneralCallMode = I2C_DUALADDRESS_DISABLE,
  .Init.NoStretchMode = I2C_NOSTRETCH_DISABLE
};

static I2C_HandleTypeDef _i2c_ch2 = {
  .Instance = I2C3,
  .Init.ClockSpeed = 400000, // 400Khz Fast Mode
  .Init.DutyCycle = I2C_DUTYCYCLE_2,
  .Init.OwnAddress1 = 0x00,
  .Init.OwnAddress2 = 0x00,
  .Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,
  .Init.DualAddressMode = I2C_DUALADDRESS_DISABLE,
  .Init.GeneralCallMode = I2C_DUALADDRESS_DISABLE,
  .Init.NoStretchMode = I2C_NOSTRETCH_DISABLE
};

static FMPI2C_HandleTypeDef _i2c_ch3 = {
  .Instance = FMPI2C1,
  .Init.Timing = 0x009034b6,
  .Init.OwnAddress1 = 0x00,
  .Init.OwnAddress2 = 0x00,
  .Init.AddressingMode = FMPI2C_ADDRESSINGMODE_7BIT,
  .Init.DualAddressMode = FMPI2C_DUALADDRESS_DISABLE,
  .Init.GeneralCallMode = FMPI2C_GENERALCALL_DISABLE,
  .Init.NoStretchMode = FMPI2C_NOSTRETCH_DISABLE,
  .Init.OwnAddress2Masks = FMPI2C_OA2_NOMASK
};

void mpr121_init() {

  // init mpr121 ch0
  __HAL_RCC_I2C1_CLK_ENABLE(); {

    // i2c1 scl & sda
    HAL_GPIO_Init(GPIOB, &(GPIO_InitTypeDef) {
      .Pin = GPIO_PIN_6 | GPIO_PIN_7,
      .Mode = GPIO_MODE_AF_OD,
      .Pull = GPIO_PULLUP,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
      .Alternate = GPIO_AF4_I2C1,
    });

    // i2c1 irq
    HAL_GPIO_Init(GPIOC, &(GPIO_InitTypeDef) {
      .Pin = GPIO_PIN_8,
      .Mode = GPIO_MODE_INPUT,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
    });

    if((HAL_I2C_Init(&_i2c_ch0) != HAL_OK)) {
      log_fatal(TAG, "init mpr121 ch0 peripheral failed");
      return;
    }
  }

  // init mpr121 ch1
  __HAL_RCC_I2C2_CLK_ENABLE(); {

    // i2c2 sda
    HAL_GPIO_Init(GPIOB, &(GPIO_InitTypeDef) {
      .Pin = GPIO_PIN_10,
      .Mode = GPIO_MODE_AF_OD,
      .Pull = GPIO_PULLUP,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
      .Alternate = GPIO_AF4_I2C2,
    });

    // i2c2 scl
    HAL_GPIO_Init(GPIOB, &(GPIO_InitTypeDef) {
      .Pin = GPIO_PIN_3,
      .Mode = GPIO_MODE_AF_OD,
      .Pull = GPIO_PULLUP,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
      .Alternate = GPIO_AF9_I2C2,
    });

    // i2c2 irq
    HAL_GPIO_Init(GPIOC, &(GPIO_InitTypeDef) {
      .Pin = GPIO_PIN_1,
      .Mode = GPIO_MODE_INPUT,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
    });

    if((HAL_I2C_Init(&_i2c_ch1) != HAL_OK)) {
      log_fatal(TAG, "init mpr121 ch1 peripheral failed");
      return;
    }
  }

  // init mpr121 ch2
  __HAL_RCC_I2C3_CLK_ENABLE(); {

    // i2c3 sda
    HAL_GPIO_Init(GPIOC, &(GPIO_InitTypeDef) {
      .Pin = GPIO_PIN_9,
      .Mode = GPIO_MODE_AF_OD,
      .Pull = GPIO_PULLUP,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
      .Alternate = GPIO_AF4_I2C3,
    });

    // i2c3 scl
    HAL_GPIO_Init(GPIOA, &(GPIO_InitTypeDef) {
      .Pin = GPIO_PIN_8,
      .Mode = GPIO_MODE_AF_OD,
      .Pull = GPIO_PULLUP,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
      .Alternate = GPIO_AF4_I2C3,
    });

    // i2c3 irq
    HAL_GPIO_Init(GPIOC, &(GPIO_InitTypeDef) {
      .Pin = GPIO_PIN_2,
      .Mode = GPIO_MODE_INPUT,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
    });

    if((HAL_I2C_Init(&_i2c_ch2) != HAL_OK)) {
      log_fatal(TAG, "init mpr121 ch2 peripheral failed");
      return;
    }

  }

  // init mpr121 ch3
  __HAL_RCC_FMPI2C1_CLK_ENABLE(); {



    // sda
    HAL_GPIO_Init(GPIOB, &(GPIO_InitTypeDef) {
      .Pin = GPIO_PIN_14,
      .Mode = GPIO_MODE_AF_OD,
      .Pull = GPIO_PULLUP,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
      .Alternate = GPIO_AF4_FMPI2C1,
    });

    // scl
    HAL_GPIO_Init(GPIOC, &(GPIO_InitTypeDef) {
      .Pin = GPIO_PIN_6,
      .Mode = GPIO_MODE_AF_OD,
      .Pull = GPIO_PULLUP,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
      .Alternate = GPIO_AF4_FMPI2C1,
    });

    // irq
    HAL_GPIO_Init(GPIOC, &(GPIO_InitTypeDef) {
      .Pin = GPIO_PIN_3,
      .Mode = GPIO_MODE_INPUT,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
    });

    if((HAL_FMPI2C_Init(&_i2c_ch3) != HAL_OK)) {
      log_fatal(TAG, "init mpr121 ch3 peripheral failed");
      return;
    }

    HAL_FMPI2CEx_ConfigAnalogFilter(&_i2c_ch3, FMPI2C_ANALOGFILTER_ENABLE);

  }

  iic_pattern_init(&_i2c_ch0, MPR121_ADDR, _cmd_init, false);
  iic_pattern_init(&_i2c_ch1, MPR121_ADDR, _cmd_init, false);
  iic_pattern_init(&_i2c_ch2, MPR121_ADDR, _cmd_init, false);
  // iic_pattern_init(&_i2c_ch3, MPR121_ADDR, _cmd_init, true);
}

void mpr121_set_irqcb(mpr121_irqcb_t callback) {
  _callback = callback;
}

uint16_t mpr121_get_touch() {

  uint16_t stat[4] = {0};
  iic_read(&_i2c_ch0, MPR121_ADDR, MPR121_REG_TOUCH_STATUS_L, (uint8_t *)&stat[0], 1);
  iic_read(&_i2c_ch1, MPR121_ADDR, MPR121_REG_TOUCH_STATUS_L, (uint8_t *)&stat[1], 1);
  iic_read(&_i2c_ch2, MPR121_ADDR, MPR121_REG_TOUCH_STATUS_L, (uint8_t *)&stat[2], 1);
  // iic_read_ex(&_i2c_ch3, MPR121_ADDR, MPR121_REG_TOUCH_STATUS_L, (uint8_t *)&stat[3], 1, &HAL_FMPI2C_Mem_Read);
  log_info("tp", "%d %d %d %d", stat[0], stat[1], stat[2], stat[3]);

  return stat;
}

void mpr121_enable_irq(bool enable) {

  if(enable) {
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  }

  else {
    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
  }

}
