#include "hts221.h"

static HTS221_Calibration_t hts221_cal;

void HTS221_ReadCalibration(I2C_HandleTypeDef *hi2c) {
    uint8_t buffer[16];
    uint8_t t0_t1_msb;

    HAL_I2C_Mem_Read(hi2c, HTS221_ADDRESS_READ, 0x30 | 0x80, I2C_MEMADD_SIZE_8BIT, buffer, 16, HAL_MAX_DELAY);

    hts221_cal.H0_rH_x2 = buffer[0];
    hts221_cal.H1_rH_x2 = buffer[1];
    hts221_cal.T0_degC_x8 = buffer[2];
    hts221_cal.T1_degC_x8 = buffer[3];

    HAL_I2C_Mem_Read(hi2c, HTS221_ADDRESS_READ, 0x35, I2C_MEMADD_SIZE_8BIT, &t0_t1_msb, 1, HAL_MAX_DELAY);

    hts221_cal.T0_degC_x8 |= (uint16_t)(t0_t1_msb & 0x03) << 8;
    hts221_cal.T1_degC_x8 |= (uint16_t)((t0_t1_msb & 0x0C) >> 2) << 8;

    HAL_I2C_Mem_Read(hi2c, HTS221_ADDRESS_READ, 0x36 | 0x80, I2C_MEMADD_SIZE_8BIT, buffer, 2, HAL_MAX_DELAY);
    hts221_cal.H0_T0_OUT = (int16_t)(buffer[1] << 8 | buffer[0]);

    HAL_I2C_Mem_Read(hi2c, HTS221_ADDRESS_READ, 0x3A | 0x80, I2C_MEMADD_SIZE_8BIT, buffer, 2, HAL_MAX_DELAY);
    hts221_cal.H1_T0_OUT = (int16_t)(buffer[1] << 8 | buffer[0]);

    HAL_I2C_Mem_Read(hi2c, HTS221_ADDRESS_READ, 0x3C | 0x80, I2C_MEMADD_SIZE_8BIT, buffer, 2, HAL_MAX_DELAY);
    hts221_cal.T0_OUT = (int16_t)(buffer[1] << 8 | buffer[0]);

    HAL_I2C_Mem_Read(hi2c, HTS221_ADDRESS_READ, 0x3E | 0x80, I2C_MEMADD_SIZE_8BIT, buffer, 2, HAL_MAX_DELAY);
    hts221_cal.T1_OUT = (int16_t)(buffer[1] << 8 | buffer[0]);
}

float HTS221_ReadHumidity(I2C_HandleTypeDef *hi2c) {
    uint8_t buffer[2];
    HAL_I2C_Mem_Read(hi2c, HTS221_ADDRESS_READ, HTS221_HUMIDITY_OUT_L | 0x80, I2C_MEMADD_SIZE_8BIT, buffer, 2, HAL_MAX_DELAY);

    int16_t H_OUT = (int16_t)(buffer[1] << 8 | buffer[0]);

    float H0_rH = hts221_cal.H0_rH_x2 / 2.0f;
    float H1_rH = hts221_cal.H1_rH_x2 / 2.0f;

    float humidity = (H_OUT - hts221_cal.H0_T0_OUT) * (H1_rH - H0_rH) / (hts221_cal.H1_T0_OUT - hts221_cal.H0_T0_OUT) + H0_rH;

    if (humidity < 0) humidity = 0;
    if (humidity > 100) humidity = 100;

    return humidity;
}
