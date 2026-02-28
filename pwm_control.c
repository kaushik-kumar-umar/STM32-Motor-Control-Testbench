#include "pwm_control.h"

/**
 * Sets PWM duty cycle
 * @param htim: Timer handle
 * @param channel: TIM_CHANNEL_x
 * @param duty: 0-1000 (0% to 100%)
 */
void PWM_SetDutyCycle(TIM_HandleTypeDef *htim,
                       uint32_t channel,
                       uint16_t duty)
{
    /* Clamp duty cycle */
    if(duty > 1000) duty = 1000;

    __HAL_TIM_SET_COMPARE(htim, channel, duty);
}

/**
 * Timer 1 Init - PWM at 20kHz
 * ARR = 1000, PSC = 3 (for 72MHz clock)
 */
void MX_TIM1_Init(void)
{
    TIM_OC_InitTypeDef sConfigOC = {0};

    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 3;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = 1000;  // ARR
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    HAL_TIM_PWM_Init(&htim1);

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;  // Start at 0% duty
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);
}
