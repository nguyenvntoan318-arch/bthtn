#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>

// Địa chỉ các thanh ghi cơ bản STM32F103
#define RCC_BASE        0x40021000
#define GPIOA_BASE      0x40010800
#define ADC1_BASE       0x40012400
#define USART1_BASE     0x40013800

#define RCC_APB2ENR     *(volatile uint32_t *)(RCC_BASE + 0x18)
#define GPIOA_CRL       *(volatile uint32_t *)(GPIOA_BASE + 0x00)
#define GPIOA_CRH       *(volatile uint32_t *)(GPIOA_BASE + 0x04)

#define ADC1_SR         *(volatile uint32_t *)(ADC1_BASE + 0x00)
#define ADC1_CR2        *(volatile uint32_t *)(ADC1_BASE + 0x08)
#define ADC1_SQR3       *(volatile uint32_t *)(ADC1_BASE + 0x1C)
#define ADC1_DR         *(volatile uint32_t *)(ADC1_BASE + 0x4C)

#define USART1_SR       *(volatile uint32_t *)(USART1_BASE + 0x00)
#define USART1_DR       *(volatile uint32_t *)(USART1_BASE + 0x04)
#define USART1_BRR      *(volatile uint32_t *)(USART1_BASE + 0x08)
#define USART1_CR1      *(volatile uint32_t *)(USART1_BASE + 0x0C)

#endif
