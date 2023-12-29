#ifndef _SYS_HARDWARE_DRIVER_COMMON_IIC_H
#define _SYS_HARDWARE_DRIVER_COMMON_IIC_H

#include <stdint.h>
#include <stdbool.h>

#include <stm32f4xx_hal_def.h>
#include <stm32f4xx_hal_dma.h>
#include <stm32f4xx_hal_i2c.h>
#include <stm32f4xx_hal_i2c_ex.h>

typedef struct {
  uint16_t reg;
  uint16_t delay;

  uint16_t len;
  uint8_t* args;
} iic_cmddef_t;

#define bytes (uint8_t [])
#define END_OF_CMD { 0, 0, 0, NULL }

/**
 * @brief initialize iic slave device with a command table
 * @param inst hal iic peripheral
 * @param addr slave device address
 * @param pattern the command table
 * @return true
 * @return false
 */
bool iic_pattern_init(I2C_HandleTypeDef* inst, uint16_t addr, const iic_cmddef_t* pattern);

/**
 * @brief read data from slave device
 * @param inst hal iic peripheral
 * @param addr slave device address
 * @param reg slave device register to access
 * @param data the data being read
 * @param length the data length
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef iic_read(I2C_HandleTypeDef* inst, uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length);

/**
 * @brief write data to slave device
 * @param inst hal iic peripheral
 * @param addr slave device address
 * @param reg slave device register to access
 * @param data the data being write
 * @param length the data length
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef iic_write(I2C_HandleTypeDef* inst, uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length);

#endif /* _SYS_HARDWARE_DRIVER_COMMON_IIC_H */
