#ifndef TDS_SENSOR_H
#define TDS_SENSOR_H

#include "stm32l1xx_hal.h"
#include <stdint.h>

void TDS_Sensor_Init(ADC_HandleTypeDef *hadc);

float TDS_Sensor_Read(void);

#endif
