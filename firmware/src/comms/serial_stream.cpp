#include "serial_stream.h"

SerialStream::SerialStream() {}

void SerialStream::begin(unsigned long baudRate) {
    Serial.begin(baudRate);
}

void SerialStream::cobsEncode(const uint8_t* data, size_t length, uint8_t* output, size_t* outputLength) {
    size_t readIdx = 0;
    size_t writeIdx = 1;
    size_t codeIdx = 0;
    uint8_t code = 1;

    while (readIdx < length) {
        if (data[readIdx] == 0x00) {
            output[codeIdx] = code;
            codeIdx = writeIdx++;
            code = 1;
        } else {
            output[writeIdx++] = data[readIdx];
            code++;
            if (code == 0xFF) {
                output[codeIdx] = code;
                codeIdx = writeIdx++;
                code = 1;
            }
        }
        readIdx++;
    }
    output[codeIdx] = code;
    output[writeIdx++] = 0x00;
    *outputLength = writeIdx;
}

void SerialStream::send(const TelemetryPacket& packet) {
    const uint8_t* data = reinterpret_cast<const uint8_t*>(&packet);
    size_t dataLen = TELEMETRY_PACKET_SIZE;
    // COBS encoded output needs at most dataLen + 2 bytes
    uint8_t encoded[TELEMETRY_PACKET_SIZE + 2];
    size_t encodedLen = 0;
    cobsEncode(data, dataLen, encoded, &encodedLen);
    Serial.write(encoded, encodedLen);
}
