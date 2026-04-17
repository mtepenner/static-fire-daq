#pragma once
#include <Arduino.h>

#pragma pack(push, 1)
struct TelemetryPacket {
    uint32_t timestamp_ms;
    float    pressure_psi;
    float    thrust_n;
    uint8_t  valve_state;
};
#pragma pack(pop)

#define TELEMETRY_PACKET_SIZE sizeof(TelemetryPacket)

class SerialStream {
public:
    SerialStream();
    void begin(unsigned long baudRate);
    void send(const TelemetryPacket& packet);
private:
    void cobsEncode(const uint8_t* data, size_t length, uint8_t* output, size_t* outputLength);
};
