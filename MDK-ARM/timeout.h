#ifndef TIMEOUT_H
#define TIMEOUT_H

#include "stm32f1xx_hal.h"

typedef struct {
    uint32_t start_tick;
    uint32_t timeout_duration;
} Timeout;

void Timeout_Init(Timeout* t, uint32_t duration_ms) {
    t->start_tick = HAL_GetTick();
    t->timeout_duration = duration_ms;
}

int Timeout_HasElapsed(Timeout* t) {
    return (HAL_GetTick() - t->start_tick) >= t->timeout_duration;
}

void Timeout_Reset(Timeout* t) {
    t->start_tick = HAL_GetTick();
}

#endif // TIMEOUT_H
