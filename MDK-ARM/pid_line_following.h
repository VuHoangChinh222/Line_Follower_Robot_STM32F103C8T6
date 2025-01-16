#ifndef PID_LINE_FOLLOWING_H
#define PID_LINE_FOLLOWING_H

#include "stm32f1xx_hal.h"

typedef struct {
    float Kp;               // H� so P (Proportional) khau ti le (khau giup xe tang toc)
    float Ki;               // He so I (Integral) khau tich phan (khau can bang 2 gia tri Kp va Kd)
    float Kd;               // He se D (Derivative) khau dao ham (khau giup xe giam toc)
    float setpoint;         // Gi� tri d�ch (setpoint) 
    float prev_error;       // Luu gi� tri sai so truoc d�
    float integral;         // T�ch ph�n cua sai so
    float output_min;       // Gi� tri toi thieu cho dau ra
    float output_max;       // Gi� tri toi da cho dau ra
    float sample_time;      // Thoi gian lay mau (s)
} PID_HandleTypeDef;

// Khoi tao PID
//Kp cang lon thi xe cang tien ve phia truoc
//KD cang lon thi xe cang lui ve phia sau
//Ki de can bang 2 tham so tren, Ki cx nen de gt nho
void PID_Init(PID_HandleTypeDef *pid, float Kp, float Ki, float Kd, float output_min, float output_max, float sample_time) {
    pid->Kp = Kp;
    pid->Ki = Ki * sample_time;
    pid->Kd = Kd / sample_time;
    pid->setpoint = 0.0f;
    pid->prev_error = 0.0f;
    pid->integral = 0.0f;
    pid->output_min = output_min;
    pid->output_max = output_max;
    pid->sample_time = sample_time;
}

void PID_Compute(PID_HandleTypeDef *pid, float input, int *pwm_left, int *pwm_right) {
    //float error = pid->setpoint - input;   // Sai so giua gi� tri d?t v� gi� tri thuc te
		float error = input;
    pid->integral += error * pid->sample_time;  // T�ch ph�n cua sai so
    float derivative = (error - pid->prev_error) / pid->sample_time;  // �ao h�m cua sai so

    // T�nh to�n gi� tri dieu chinh dua tr�n PID
    float correction = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;

    // �ieu chinh gi� tri PWM cho dung co tr�i v� phai dua tr�n correction
    int32_t left_output = pid->output_max/2 + correction;   // �ung co tr�i
    int32_t right_output = pid->output_max/2 - correction;  // �ung co phai

    if (left_output > pid->output_max) left_output = pid->output_max;
    if (left_output < pid->output_min) left_output = pid->output_min;
    if (right_output > pid->output_max) right_output = pid->output_max;
    if (right_output < pid->output_min) right_output = pid->output_min;

		if(error==0 && pid->prev_error==0){
			left_output=pid->output_max;
			right_output=pid->output_max;
		}
		else if(error==1000){
			left_output=0;
			right_output=0;
		}
	
    *pwm_left = left_output;
    *pwm_right = right_output;

    // Cap nhat sai so truoc d�
    pid->prev_error = error;
}

// Cap nhat gi� tri setpoint
void PID_SetSetpoint(PID_HandleTypeDef *pid, float setpoint) {
    pid->setpoint = setpoint;
}

// Reset bo dieu khien PID
void PID_Reset(PID_HandleTypeDef *pid) {
    pid->prev_error = 0.0f;
    pid->integral = 0.0f;
}

#endif // PID_LINE_FOLLOWING_H
