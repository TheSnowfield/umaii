#include <driver/config.h>

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

void mpr121_init() {
  iic_pattern_init(&i2c1, MPR121_ADDR, _cmd_init);
  // iic_pattern_init(&i2c2, 0x5a, _cmd_init);
  // iic_pattern_init(&i2c2, 0x5a, _cmd_init);
  // iic_pattern_init(&i2c3, 0x5a, _cmd_init);
}

void mpr121_set_irqcb(mpr121_irqcb_t callback) {
  _callback = callback;
}

uint16_t mpr121_get_touch() {

  uint16_t stat = 0;
  iic_read(&i2c1, MPR121_ADDR, MPR121_REG_TOUCH_STATUS_L, (uint8_t *)&stat, 1);

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
