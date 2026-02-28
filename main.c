#include "main.h"
#include "pwm_control.h"
#include "adc_current.h"
#include "encoder_rpm.h"

/* Private variables */
TIM_HandleTypeDef htim1;  // PWM Timer
TIM_HandleTypeDef htim2;  // Encoder Timer
ADC_HandleTypeDef hadc1;  // Current Sensing ADC
UART_HandleTypeDef huart1; // Data Logging

uint16_t duty_cycle = 0;
float current_ma = 0;
int32_t rpm = 0;
char uart_buf[100];

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    /* Initialize peripherals */
    MX_GPIO_Init();
    MX_TIM1_Init();   // PWM
    MX_TIM2_Init();   // Encoder
    MX_ADC1_Init();   // Current sense
    MX_USART1_UART_Init(); // Logging

    /* Start PWM on TIM1 Channel 1 */
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

    /* Start Encoder on TIM2 */
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

    /* Set initial direction - Forward */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);   // IN1 HIGH
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // IN2 LOW

    while (1)
    {
        /* Ramp up duty cycle 0 to 100% */
        for(duty_cycle = 0; duty_cycle <= 1000; duty_cycle += 10)
        {
            PWM_SetDutyCycle(&htim1, TIM_CHANNEL_1, duty_cycle);

            /* Read current */
            current_ma = ADC_GetCurrentMA(&hadc1);

            /* Read RPM */
            rpm = Encoder_GetRPM(&htim2);

            /* Log over UART */
            snprintf(uart_buf, sizeof(uart_buf),
                "Duty: %d%% | Current: %.2f mA | RPM: %ld\r\n",
                duty_cycle/10, current_ma, rpm);
            HAL_UART_Transmit(&huart1, (uint8_t*)uart_buf,
                strlen(uart_buf), HAL_MAX_DELAY);

            HAL_Delay(200);
        }

        /* Ramp down */
        for(duty_cycle = 1000; duty_cycle > 0; duty_cycle -= 10)
        {
            PWM_SetDutyCycle(&htim1, TIM_CHANNEL_1, duty_cycle);
            current_ma = ADC_GetCurrentMA(&hadc1);
            rpm = Encoder_GetRPM(&htim2);

            snprintf(uart_buf, sizeof(uart_buf),
                "Duty: %d%% | Current: %.2f mA | RPM: %ld\r\n",
                duty_cycle/10, current_ma, rpm);
            HAL_UART_Transmit(&huart1, (uint8_t*)uart_buf,
                strlen(uart_buf), HAL_MAX_DELAY);

            HAL_Delay(200);
        }

        HAL_Delay(1000);
    }
}
