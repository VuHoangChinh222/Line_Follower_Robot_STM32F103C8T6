#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

typedef struct {
    float Q; // Qua trinh noise covariance(hieu phuong sai)
    float R; // Ðo luong noise covariance
    float x; // Giá tri do hien tai
    float P; // Sai so cua estimation(uoc tinh)
    float K; // Kalman gain
} KalmanFilter;

void KalmanFilter_Init(KalmanFilter* kf, float Q, float R, float initial_value) {
    kf->Q = Q;
    kf->R = R;
    kf->x = initial_value;
    kf->P = 1.0f; // Gia su sai so ban dau lon
}

float KalmanFilter_Update(KalmanFilter* kf, float measurement) {
    // Predict
    kf->P += kf->Q;

    // Update
    kf->K = kf->P / (kf->P + kf->R);
    kf->x += kf->K * (measurement - kf->x);
    kf->P *= (1 - kf->K);

    return kf->x;
}

#endif // KALMAN_FILTER_H
