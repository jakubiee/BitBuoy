#include "stlm75.h"

void STLM75_Init(STLM75_HandleTypeDef *stlm75, I2C_HandleTypeDef *hi2c) {
    stlm75->hi2c = hi2c;
}

HAL_StatusTypeDef STLM75_ReadTemperature(STLM75_HandleTypeDef *stlm75, float *temperature) {
    uint8_t temp_data[2] = {0};

    HAL_StatusTypeDef result = HAL_I2C_Mem_Read(stlm75->hi2c, STLM75_I2C_ADDR, TEMP_REG, I2C_MEMADD_SIZE_8BIT, temp_data, 2, HAL_MAX_DELAY);

    if (result == HAL_OK) {
        int16_t raw_temp = (temp_data[0] << 8) | temp_data[1];
        raw_temp >>= 7;
        *temperature = raw_temp * 0.5;
    }

    return result;
}
