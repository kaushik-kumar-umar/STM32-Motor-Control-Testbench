#include "adc_current.h"

#define VREF_MV         3300.0f   // 3.3V reference
#define ADC_RESOLUTION  4096.0f   // 12-bit ADC
#define ACS712_OFFSET   1650.0f   // 0A = VCC/2 = 1650mV
#define ACS712_SENS     185.0f    // 185mV/A for 5A variant

/**
 * Reads current in milliamps from ACS712
 */
float ADC_GetCurrentMA(ADC_HandleTypeDef *hadc)
{
    HAL_ADC_Start(hadc);
    HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
    uint32_t raw = HAL_ADC_GetValue(hadc);
    HAL_ADC_Stop(hadc);

    /* Convert raw to millivolts */
    float voltage_mv = (raw / ADC_RESOLUTION) * VREF_MV;

    /* Convert voltage to current */
    float current_ma = ((voltage_mv - ACS712_OFFSET) 
                        / ACS712_SENS) * 1000.0f;

    return current_ma;
}

void MX_ADC1_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    HAL_ADC_Init(&hadc1);

    sConfig.Channel = ADC_CHANNEL_0;  // PA0
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}
