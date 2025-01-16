#ifndef PWM_H
#define PWM_H

#include "stm32f1xx_hal.h"

typedef struct {
    TIM_HandleTypeDef *htim;   // Con tro den handler cua timer
    uint32_t Channel;          // Kênh cua PWM (TIM_CHANNEL_1, TIM_CHANNEL_2, etc.)
    uint32_t Pulse;            // Giá tri de r?ng xung (duty cycle)
    uint32_t Period;           // Chu ky cua PWM
} PWM_HandleTypeDef;

void PWM_Init(PWM_HandleTypeDef *hpwm, TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t Period, uint32_t Pulse) {
    hpwm->htim = htim;
    hpwm->Channel = Channel;
    hpwm->Period = Period;
    hpwm->Pulse = Pulse;

    TIM_OC_InitTypeDef sConfigOC = {0};
    htim->Init.Period = Period;
    HAL_TIM_PWM_Init(htim);

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = Pulse;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, Channel);
    HAL_TIM_PWM_Start(htim, Channel);
}

// Cap nhat giá tri de r?ng xung (duty cycle)
void PWM_SetDutyCycle(PWM_HandleTypeDef *hpwm, uint32_t Pulse) {
    hpwm->Pulse = Pulse;
    __HAL_TIM_SET_COMPARE(hpwm->htim, hpwm->Channel, Pulse);
}

void PWM_Stop(PWM_HandleTypeDef *hpwm) {
    HAL_TIM_PWM_Stop(hpwm->htim, hpwm->Channel);
}

void PWM_Start(PWM_HandleTypeDef *hpwm) {
    HAL_TIM_PWM_Start(hpwm->htim, hpwm->Channel);
}

#endif // PWM_H

