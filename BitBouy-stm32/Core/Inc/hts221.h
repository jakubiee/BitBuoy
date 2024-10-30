#ifndef HTS221_H
#define HTS221_H

#include "stm32l1xx_hal.h"

#define HTS221_ADDRESS_WRITE 0xBE
#define HTS221_ADDRESS_READ  0xBF //

#define HTS221_HUMIDITY_OUT_L 0x28
#define HTS221_HUMIDITY_OUT_H 0x29
#define HTS221_TEMP_OUT_L 0x2A
#define HTS221_TEMP_OUT_H 0x2B
#define HTS221_CTRL_REG1 0x20

typedef struct {
    uint8_t H0_rH_x2;
    uint8_t H1_rH_x2;
    uint16_t T0_degC_x8;
    uint16_t T1_degC_x8;
    int16_t H0_T0_OUT;
    int16_t H1_T0_OUT;
    int16_t T0_OUT;
    int16_t T1_OUT;
} HTS221_Calibration_t;

void HTS221_ReadCalibration(I2C_HandleTypeDef *hi2c);
float HTS221_ReadHumidity(I2C_HandleTypeDef *hi2c);

#endif
