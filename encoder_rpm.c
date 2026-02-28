#include "encoder_rpm.h"

#define ENCODER_PPR     600    // Pulses per revolution
#define SAMPLE_TIME_MS  100    // RPM sample window

static int32_t last_count = 0;
static uint32_t last_tick = 0;

/**
 * Returns RPM based on encoder count delta
 */
int32_t Encoder_GetRPM(TIM_HandleTypeDef *htim)
{
    uint32_t now = HAL_GetTick();
    int32_t current_count = (int32_t)__HAL_TIM_GET_COUNTER(htim);

    if((now - last_tick) >= SAMPLE_TIME_MS)
    {
        int32_t delta = current_count - last_count;
        float time_sec = (now - last_tick) / 1000.0f;

        /* RPM = (pulses / PPR) / time * 60 */
        int32_t rpm = (int32_t)((delta / (float)ENCODER_PPR) 
                                / time_sec * 60.0f);

        last_count = current_count;
        last_tick = now;
        return rpm;
    }
    return 0;
}

void MX_TIM2_Init(void)
{
    TIM_Encoder_InitTypeDef sConfig = {0};

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 0;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 65535;
    HAL_TIM_Encoder_Init(&htim2, &sConfig);

    sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
    sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC1Filter = 0x0;
    sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC2Filter = 0x0;
    HAL_TIM_Encoder_Init(&htim2, &sConfig);
}
