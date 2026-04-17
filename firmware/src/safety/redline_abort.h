#pragma once
#include <Arduino.h>

struct RedlineConfig {
    float maxPressurePsi;
    float maxThrustN;
};

class RedlineAbort {
public:
    RedlineAbort(const RedlineConfig& config);
    void setAbortCallback(void (*callback)());
    bool check(float pressurePsi, float thrustN);
    bool isAborted() const;
private:
    RedlineConfig config_;
    bool aborted_;
    void (*abortCallback_)();
};
