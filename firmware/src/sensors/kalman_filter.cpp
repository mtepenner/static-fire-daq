#include "kalman_filter.h"

KalmanFilter::KalmanFilter(float processNoise, float measurementNoise, float estimationError, float initialValue)
    : q_(processNoise), r_(measurementNoise), p_(estimationError), x_(initialValue), k_(0.0f) {}

float KalmanFilter::update(float measurement) {
    // Prediction update
    p_ = p_ + q_;

    // Measurement update
    k_ = p_ / (p_ + r_);
    x_ = x_ + k_ * (measurement - x_);
    p_ = (1.0f - k_) * p_;

    return x_;
}

float KalmanFilter::getEstimate() const {
    return x_;
}
