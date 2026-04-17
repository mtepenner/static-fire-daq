#include "solenoids.h"

Solenoids::Solenoids()
    : ignition_(ValveState::CLOSED), purge_(ValveState::CLOSED), mov_(ValveState::CLOSED) {}

void Solenoids::begin() {
    pinMode(SOL_IGNITION_PIN, OUTPUT);
    pinMode(SOL_PURGE_PIN, OUTPUT);
    pinMode(SOL_MOV_PIN, OUTPUT);
    abortAll();
}

void Solenoids::setIgnition(ValveState state) {
    ignition_ = state;
    digitalWrite(SOL_IGNITION_PIN, static_cast<uint8_t>(state));
}

void Solenoids::setPurge(ValveState state) {
    purge_ = state;
    digitalWrite(SOL_PURGE_PIN, static_cast<uint8_t>(state));
}

void Solenoids::setMOV(ValveState state) {
    mov_ = state;
    digitalWrite(SOL_MOV_PIN, static_cast<uint8_t>(state));
}

void Solenoids::abortAll() {
    setIgnition(ValveState::CLOSED);
    setPurge(ValveState::CLOSED);
    setMOV(ValveState::CLOSED);
}

uint8_t Solenoids::getValveStateByte() const {
    uint8_t state = 0;
    state |= (static_cast<uint8_t>(ignition_) & 0x01) << 0;
    state |= (static_cast<uint8_t>(purge_)    & 0x01) << 1;
    state |= (static_cast<uint8_t>(mov_)      & 0x01) << 2;
    return state;
}
