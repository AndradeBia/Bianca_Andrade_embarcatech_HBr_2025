#include "temp_sensor.h"

const float ADC_VOLTAGE_REF = 3.3f;
const float ADC_MAX_VALUE = 4095.0f;
const float TEMP_SENSOR_VOLTAGE_AT_27C = 0.706f;
const float TEMP_SENSOR_SLOPE = 0.001721f;

float adc_to_celsius(uint16_t adc_val) {
    float voltage = ((float)adc_val * ADC_VOLTAGE_REF) / ADC_MAX_VALUE;

    float temperature = 27.0f - (voltage - TEMP_SENSOR_VOLTAGE_AT_27C) / TEMP_SENSOR_SLOPE;

    return temperature;
}