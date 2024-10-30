#ifndef STLM75_H
#define STLM75_H

#include "stm32l1xx_hal.h"
#include <stdint.h>

#define STLM75_I2C_ADDR 0x48 << 1
#define TEMP_REG 0x00

typedef struct {
    I2C_HandleTypeDef *hi2c;
} STLM75_HandleTypeDef;

void STLM75_Init(STLM75_HandleTypeDef *stlm75, I2C_HandleTypeDef *hi2c);

HAL_StatusTypeDef STLM75_ReadTemperature(STLM75_HandleTypeDef *stlm75, float *temperature);

#endif
