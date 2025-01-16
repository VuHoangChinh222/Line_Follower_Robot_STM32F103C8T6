#ifndef BUTTON_H
#define BUTTON_H

#include "stm32f1xx_hal.h"

typedef enum {
    BUTTON_EVENT_NONE = 0,
    BUTTON_EVENT_PRESS,
    BUTTON_EVENT_DOUBLE_PRESS,
    BUTTON_EVENT_LONG_PRESS
} ButtonEvent;

typedef struct {
    GPIO_TypeDef* GPIOx;  // C?ng GPIO c?a nút nh?n
    uint16_t GPIO_Pin;    // Pin GPIO c?a nút nh?n
    uint32_t lastDebounceTime; // Th?i gian debounce g?n nh?t
    uint32_t lastPressTime;    // Th?i gian nh?n g?n nh?t
    uint8_t buttonState;       // Tr?ng thái hi?n t?i c?a nút nh?n
    uint8_t lastButtonState;   // Tr?ng thái nút nh?n tru?c dó
    uint8_t pressCount;        // Ð?m s? l?n nh?n
} Button_HandleTypeDef;

// C?u hình debounce, long press và double press
#define DEBOUNCE_DELAY 50        // Th?i gian debounce (ms)
#define LONG_PRESS_DELAY 1000    // Th?i gian nh?n gi? (ms)
#define DOUBLE_PRESS_DELAY 300   // Th?i gian d? nh?n di?n nh?n 2 l?n (ms)


void Button_Init(Button_HandleTypeDef *btn, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    btn->GPIOx = GPIOx;
    btn->GPIO_Pin = GPIO_Pin;
    btn->lastDebounceTime = 0;
    btn->lastPressTime = 0;
    btn->buttonState = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
    btn->lastButtonState = btn->buttonState;
    btn->pressCount = 0;

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    if (GPIOx == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (GPIOx == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (GPIOx == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (GPIOx == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    } else if (GPIOx == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    }
    
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP; // S? d?ng Pull-Up ho?c Pull-Down tùy thu?c vào ph?n c?ng
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

ButtonEvent Button_Update(Button_HandleTypeDef *btn) {
    uint8_t currentState = HAL_GPIO_ReadPin(btn->GPIOx, btn->GPIO_Pin);
    ButtonEvent event = BUTTON_EVENT_NONE;
    uint32_t currentTime = HAL_GetTick();

    if (currentState != btn->lastButtonState) {
        btn->lastDebounceTime = currentTime;
    }

    if ((currentTime - btn->lastDebounceTime) > DEBOUNCE_DELAY) {
        if (currentState != btn->buttonState) {
            btn->buttonState = currentState;

            if (btn->buttonState == GPIO_PIN_RESET) {
                btn->pressCount++;
                btn->lastPressTime = currentTime;
            }
        } else {
            if (btn->buttonState == GPIO_PIN_RESET && (currentTime - btn->lastPressTime) > LONG_PRESS_DELAY) {
                event = BUTTON_EVENT_LONG_PRESS;
                btn->pressCount = 0;
            }
        }
    }

    if (btn->pressCount == 1 && (currentTime - btn->lastPressTime) > DOUBLE_PRESS_DELAY) {
        event = BUTTON_EVENT_PRESS;
        btn->pressCount = 0;
    } else if (btn->pressCount == 2 && (currentTime - btn->lastPressTime) <= DOUBLE_PRESS_DELAY) {
        event = BUTTON_EVENT_DOUBLE_PRESS;
        btn->pressCount = 0;
    }

    btn->lastButtonState = currentState;
    return event;
}

#endif // BUTTON_H
