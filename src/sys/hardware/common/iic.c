#include "iic.h"
#include <sys/tick.h>

HAL_StatusTypeDef iic_write(I2C_HandleTypeDef* inst,
uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length) {
  return HAL_I2C_Mem_Write(inst, addr, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY);
}

HAL_StatusTypeDef iic_read(I2C_HandleTypeDef* inst,
uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length) {
 return HAL_I2C_Mem_Read(inst, addr, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY);
}

bool iic_pattern_init(I2C_HandleTypeDef* inst,
uint16_t addr, const iic_cmddef_t* pattern) {

  if(inst == NULL || pattern == NULL) {
    return false;
  }

  HAL_StatusTypeDef stat = HAL_OK;
  for(int i = 0; pattern[i].reg != 0; ++i) {

    // transmit data
    stat = iic_write(inst, addr,
      pattern[i].reg, pattern[i].args, pattern[i].len); {
      
      // if failed return false
      if(stat != HAL_OK) {
        return false;
      }
    }

    // delay several seconds
    if(pattern[i].delay != 0)
      tick_delay(pattern[i].delay);
  }

  return true;
}

