#include "tds_sensor.h"

static ADC_HandleTypeDef *tds_hadc;

void TDS_Sensor_Init(ADC_HandleTypeDef *hadc) {
    tds_hadc = hadc;
}

static float Calculate_TDS(uint32_t adc_value) {
    float voltage = (adc_value / 4095.0) * 5;
    float tds = (133.42 * voltage * voltage * voltage) - (255.86 * voltage * voltage) + (857.39 * voltage) * 0.5;
    return tds;
}

float TDS_Sensor_Read(void) {
    uint32_t adc_value = 0;

    HAL_ADC_Start(tds_hadc);
    if (HAL_ADC_PollForConversion(tds_hadc, HAL_MAX_DELAY) == HAL_OK) {
        adc_value = HAL_ADC_GetValue(tds_hadc);
    }
    HAL_ADC_Stop(tds_hadc);

    return Calculate_TDS(adc_value);
}
