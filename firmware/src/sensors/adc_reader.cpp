#include "adc_reader.h"

ADCReader::ADCReader() {}

void ADCReader::begin() {
    analogReadResolution(ADC_RESOLUTION);
}

float ADCReader::toVoltage(int raw) {
    return (raw / ADC_MAX_VALUE) * ADC_REF_VOLTAGE;
}

float ADCReader::readPressure() {
    int raw = analogRead(ADC_PRESSURE_PIN);
    float voltage = toVoltage(raw);
    // 0-3.3V maps to 0-1000 PSI
    return (voltage / ADC_REF_VOLTAGE) * 1000.0f;
}

float ADCReader::readThrust() {
    int raw = analogRead(ADC_THRUST_PIN);
    float voltage = toVoltage(raw);
    // 0-3.3V maps to 0-5000 N
    return (voltage / ADC_REF_VOLTAGE) * 5000.0f;
}
