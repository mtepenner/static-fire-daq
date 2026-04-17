#pragma once

class KalmanFilter {
public:
    KalmanFilter(float processNoise, float measurementNoise, float estimationError, float initialValue);
    float update(float measurement);
    float getEstimate() const;
private:
    float q_; // process noise covariance
    float r_; // measurement noise covariance
    float p_; // estimation error covariance
    float x_; // current estimate
    float k_; // Kalman gain
};
