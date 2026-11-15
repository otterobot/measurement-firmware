#include "measurements.h"
#include <stdio.h>
#include "usart.h"
#include "tim.h"

#define GEAR_RATIO 20.0f

void measurements_main(void)
{
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    uint32_t prev_count = __HAL_TIM_GET_COUNTER(&htim2);
    uint32_t prev_time = HAL_GetTick();

    char msg[100];
    while (1)
    {
            HAL_Delay(100);

    uint32_t now = HAL_GetTick();
    uint32_t count = __HAL_TIM_GET_COUNTER(&htim2);

    int32_t delta = (int32_t)(count - prev_count);
    float dt = (float)(now - prev_time) / 1000.0f;

    prev_count = count;
    prev_time = now;

    float counts_per_rev = 7.0f * 4.0f * GEAR_RATIO;
    float rpm = 0.0f;

    if (dt > 0.0f) {
        rpm = (float)delta * 60.0f / (counts_per_rev * dt);
    }

    int len = snprintf(msg, sizeof(msg),
                       "%lu,%ld,%.2f\r\n",
                       (unsigned long)now,
                       (long)delta,
                       rpm);

    if (len > 0 && len < sizeof(msg)) {
        HAL_UART_Transmit(&hlpuart1,
                          (uint8_t *)msg,
                          (uint16_t)len,
                          100);
    }
    }
}