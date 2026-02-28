# STM32 Motor Control Testbench

An embedded testbench built on STM32 to characterize 
brushed DC motor behaviour under variable load conditions.

## Features
- PWM-based speed control via H-bridge (L298N)
- ADC current sensing using ACS712 sensor
- Encoder-based RPM measurement via timer input capture
- Real-time UART data logging (duty cycle, current, RPM)
- Signal integrity verified using oscilloscope

## Hardware
| Component | Details |
|-----------|---------|
| MCU | STM32F103C8T6 (Blue Pill) |
| Motor Driver | L298N H-Bridge |
| Current Sensor | ACS712 (5A) |
| Encoder | 600 PPR Rotary Encoder |

## Pin Mapping
| Signal | STM32 Pin |
|--------|-----------|
| PWM Output | PA8 (TIM1_CH1) |
| Motor IN1 | PB0 |
| Motor IN2 | PB1 |
| Current Sense | PA0 (ADC1_CH0) |
| Encoder A | PA0 (TIM2_CH1) |
| Encoder B | PA1 (TIM2_CH2) |
| UART TX | PA9 |

## Results
- Achieved stable PWM control across 0–100% duty cycle
- Logged RPM vs current data across variable load conditions
- Verified PWM signal integrity using oscilloscope screenshots
