#include <stdint.h>

#define RCC_APB2ENR   (*(volatile uint32_t *)0x40021018)

#define GPIOA_CRL     (*(volatile uint32_t *)0x40010800)
#define GPIOA_CRH     (*(volatile uint32_t *)0x40010804)

#define USART1_SR     (*(volatile uint32_t *)0x40013800)
#define USART1_DR     (*(volatile uint32_t *)0x40013804)
#define USART1_BRR    (*(volatile uint32_t *)0x40013808)
#define USART1_CR1    (*(volatile uint32_t *)0x4001380C)

#define USART1_TXE    (1 << 7)
#define USART1_RXNE   (1 << 5)
#define USART1_UE     (1 << 13)
#define USART1_TE     (1 << 3)
#define USART1_RE     (1 << 2)

void USART1_Init(void)
{
    /* Clock GPIOA + USART1 */
    RCC_APB2ENR |= (1 << 2);
    RCC_APB2ENR |= (1 << 14);

    /* PA9 = Alternate Function Push-Pull, 50 MHz
       PA10 = Input Floating */

    GPIOA_CRH &= ~0x00000FF0;
    GPIOA_CRH |=  0x000004B0;

    /* 72 MHz / 115200 */
    USART1_BRR = 0x0271;

    /* Enable USART, TX, RX */
    USART1_CR1 = USART1_UE | USART1_TE | USART1_RE;
}

void USART1_SendChar(char c)
{
    while (!(USART1_SR & USART1_TXE));

    USART1_DR = c;
}

char USART1_ReceiveChar(void)
{
    while (!(USART1_SR & USART1_RXNE));

    return (char)(USART1_DR & 0xFF);
}

void USART1_SendString(const char *str)
{
    while (*str)
    {
        USART1_SendChar(*str++);
    }
}

int main(void)
{
    char buffer[100];
    uint32_t index = 0;

    USART1_Init();

    USART1_SendString("STM32F103 USART1 READY\r\n");

    while (1)
    {
        char c = USART1_ReceiveChar();

        if (c == '!')
        {
            buffer[index] = '\0';

            USART1_SendString("\r\nReceived: ");
            USART1_SendString(buffer);
            USART1_SendString("\r\n");

            index = 0;
        }
        else
        {
            if (index < 99)
            {
                buffer[index++] = c;
            }

            USART1_SendChar(c);
        }
    }
}
