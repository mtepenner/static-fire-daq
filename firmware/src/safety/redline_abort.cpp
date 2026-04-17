#include "redline_abort.h"

RedlineAbort::RedlineAbort(const RedlineConfig& config)
    : config_(config), aborted_(false), abortCallback_(nullptr) {}

void RedlineAbort::setAbortCallback(void (*callback)()) {
    abortCallback_ = callback;
}

bool RedlineAbort::check(float pressurePsi, float thrustN) {
    if (!aborted_ && (pressurePsi > config_.maxPressurePsi || thrustN > config_.maxThrustN)) {
        aborted_ = true;
        if (abortCallback_) {
            abortCallback_();
        }
        return true;
    }
    return false;
}

bool RedlineAbort::isAborted() const {
    return aborted_;
}
