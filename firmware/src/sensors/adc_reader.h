#pragma once
#include <Arduino.h>

#define ADC_PRESSURE_PIN A0
#define ADC_THRUST_PIN   A1
#define ADC_RESOLUTION   12
#define ADC_REF_VOLTAGE  3.3f
#define ADC_MAX_VALUE    4095.0f

class ADCReader {
public:
    ADCReader();
    void begin();
    float readPressure();
    float readThrust();
private:
    float toVoltage(int raw);
};
