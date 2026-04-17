#include <Arduino.h>
#include "sensors/adc_reader.h"
#include "sensors/kalman_filter.h"
#include "safety/redline_abort.h"
#include "actuators/solenoids.h"
#include "comms/serial_stream.h"

// instances
ADCReader adc;
KalmanFilter pressureFilter(0.1f, 10.0f, 1.0f, 0.0f);
KalmanFilter thrustFilter(0.1f, 10.0f, 1.0f, 0.0f);
Solenoids solenoids;
SerialStream stream;

RedlineConfig config = { 750.0f, 4500.0f };
RedlineAbort redline(config);

void onAbort() {
    solenoids.abortAll();
}

void setup() {
    stream.begin(2000000);
    adc.begin();
    solenoids.begin();
    redline.setAbortCallback(onAbort);
}

void loop() {
    float rawPressure = adc.readPressure();
    float rawThrust   = adc.readThrust();
    float pressure    = pressureFilter.update(rawPressure);
    float thrust      = thrustFilter.update(rawThrust);

    redline.check(pressure, thrust);

    TelemetryPacket pkt;
    pkt.timestamp_ms = millis();
    pkt.pressure_psi = pressure;
    pkt.thrust_n     = thrust;
    pkt.valve_state  = solenoids.getValveStateByte();

    stream.send(pkt);
}
