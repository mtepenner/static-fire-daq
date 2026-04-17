#pragma once
#include <Arduino.h>

#define SOL_IGNITION_PIN 2
#define SOL_PURGE_PIN    3
#define SOL_MOV_PIN      4

enum class ValveState : uint8_t {
    CLOSED = 0,
    OPEN   = 1
};

class Solenoids {
public:
    Solenoids();
    void begin();
    void setIgnition(ValveState state);
    void setPurge(ValveState state);
    void setMOV(ValveState state);
    void abortAll();
    uint8_t getValveStateByte() const;
private:
    ValveState ignition_;
    ValveState purge_;
    ValveState mov_;
};
