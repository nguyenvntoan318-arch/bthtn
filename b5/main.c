/* STM32F103C8T6: HSI 8 MHz, TIM2_CH1 PA0, USART1 PA9/PA10.
 * Nhan va truyen UART deu dung ngat. Xu ly lenh trong main.
 */
#include <stdint.h>
#include "app.h"
#include "rx.h"
#define REG(a) (*(volatile uint32_t *)(a))
#define RCC_CR       REG(0x40021000u)
#define RCC_CFGR     REG(0x40021004u)
#define RCC_APB2ENR  REG(0x40021018u)
#define RCC_APB1ENR  REG(0x4002101Cu)
#define GPIOA_CRL    REG(0x40010800u)
#define GPIOA_CRH    REG(0x40010804u)
#define GPIOA_BSRR   REG(0x40010810u)
#define TIM2_CR1     REG(0x40000000u)
#define TIM2_SR      REG(0x40000010u)
#define TIM2_EGR     REG(0x40000014u)
#define TIM2_CCMR1   REG(0x40000018u)
#define TIM2_CCER    REG(0x40000020u)
#define TIM2_PSC     REG(0x40000028u)
#define TIM2_ARR     REG(0x4000002Cu)
#define TIM2_CCR1    REG(0x40000034u)
#define USART1_SR   REG(0x40013800u)
#define USART1_DR   REG(0x40013804u)
#define USART1_BRR  REG(0x40013808u)
#define USART1_CR1  REG(0x4001380Cu)
#define USART1_CR2  REG(0x40013810u)
#define USART1_CR3  REG(0x40013814u)
#define NVIC_ISER1  REG(0xE000E104u)
#define SCB_VTOR    REG(0xE000ED08u)
#define RXNE (1u << 5)
#define TXE  (1u << 7)
#define TX_SIZE 128u
static Receiver receiver;
static volatile char tx_data[TX_SIZE];
static volatile unsigned tx_head, tx_tail;
static void irq_off(void) { __asm volatile("cpsid i" ::: "memory"); }
static void irq_on(void) { __asm volatile("cpsie i" ::: "memory"); }

static void hardware_init(void) {
    SCB_VTOR = 0x08000000u;
    RCC_CR |= 1u;
    while (!(RCC_CR & (1u << 1))) { }
    RCC_CFGR = 0; /* HSI, AHB/APB1/APB2 chia 1. */
    while (RCC_CFGR & (3u << 2)) { }
    RCC_APB2ENR |= (1u << 2) | (1u << 14);
    RCC_APB1ENR |= 1u;
    (void)RCC_APB2ENR; (void)RCC_APB1ENR;
    GPIOA_CRL = (GPIOA_CRL & ~0xFu) | 0xAu; /* PA0 AF push-pull. */
    GPIOA_CRH = (GPIOA_CRH & ~0xFF0u) | 0x8A0u;
    GPIOA_BSRR = 1u << 10; /* PA9 TX AF; PA10 RX pull-up. */
    TIM2_PSC = 7;
    TIM2_ARR = 999; /* 8 MHz / (7+1) / (999+1) = 1 kHz. */
    TIM2_CCR1 = 0;
    TIM2_CCMR1 = (6u << 4) | (1u << 3); /* PWM1, preload. */
    TIM2_CCER = 1;
    TIM2_EGR = 1;
    TIM2_SR = 0;
    TIM2_CR1 = (1u << 7) | 1u;
    USART1_BRR = 833; /* 8 MHz / 9600, lam tron. */
    USART1_CR2 = 0;
    USART1_CR3 = 0;
    USART1_CR1 = (1u << 13) | (1u << 3) | (1u << 2) | RXNE;
    NVIC_ISER1 = 1u << 5; /* USART1 IRQ so 37. */
}

void USART1_IRQHandler(void) {
    uint32_t status = USART1_SR;
    if (status & (RXNE | 0xFu)) {
        char c = (char)USART1_DR; /* Doc SR roi DR xoa RXNE/loi. */
        if (status & 0xFu) { rx_error(&receiver); }
        else { rx_byte(&receiver, c); }
    }
    if ((USART1_CR1 & TXE) && (USART1_SR & TXE)) {
        if (tx_tail != tx_head) {
            USART1_DR = (uint8_t)tx_data[tx_tail];
            tx_tail = (tx_tail + 1u) % TX_SIZE;
        } else { USART1_CR1 &= ~TXE; }
    }
}

/* Chi goi tu main, ngat phai dang bat. TXE interrupt gui tung byte. */
static void uart_send(const char *s) {
    while (*s) {
        unsigned next = (tx_head + 1u) % TX_SIZE;
        while (next == tx_tail) { }
        irq_off();
        tx_data[tx_head] = *s++;
        tx_head = next;
        USART1_CR1 |= TXE;
        irq_on();
    }
}

int main(void) {
    App app;
    app_init(&app);
    irq_off();
    hardware_init();
    irq_on();
    uart_send("READY: ON! OFF! PWM:0%!..PWM:100%! Status!\r\n");
    for (;;) {
        char command[RX_SIZE], reply[48];
        irq_off();
        int got = rx_pop(&receiver, command);
        uint8_t errors = receiver.errors;
        receiver.errors = 0;
        irq_on();
        if (errors & RX_LONG) { uart_send("ERR: command too long/invalid byte\r\n"); }
        if (errors & RX_FULL) { uart_send("ERR: queue full; send slower\r\n"); }
        if (errors & RX_ERROR) { uart_send("ERR: UART; end with ! then retry\r\n"); }
        if (!got) { continue; }
        CommandResult result = app_command(&app, command);
        if (result == CMD_BAD) {
            uart_send("ERR: use ON! OFF! PWM:0%!..PWM:100%! Status!\r\n");
            continue;
        }
        TIM2_CCR1 = (uint32_t)app_output(&app) * 10u;
        if (result == CMD_CHANGED) { uart_send("OK "); }
        app_status(&app, reply);
        uart_send(reply);
    }
}
