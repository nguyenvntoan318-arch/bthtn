#include "stm32f10x.h" // ThÆ° viá»‡n lÃµi cho dÃ²ng STM32F10x

/* CÃ¡c biáº¿n Ä‘áº¿m thá»�i gian cho Systick */
volatile uint32_t count_0_1Hz = 0;
volatile uint32_t count_1Hz = 0;
volatile uint32_t count_10Hz = 0;

/* Khai bÃ¡o nguyÃªn máº«u hÃ m */
void GPIO_Configuration(void);

int main(void)
{
    /* 1. Khá»Ÿi táº¡o GPIO (PA0, PA1, PA2) */
    GPIO_Configuration();

    /* 2. Cáº¥u hÃ¬nh Systick ngáº¯t má»—i 1ms (1/1000 giÃ¢y)
     * SystemCoreClock Ä‘Ã£ Ä‘Æ°á»£c cáº­p nháº­t tá»± Ä‘á»™ng nhá»� file startup (thÆ°á»�ng lÃ  72MHz) 
     */
    if (SysTick_Config(SystemCoreClock / 1000))
    { 
        /* Náº¿u lá»—i cáº¥u hÃ¬nh Systick thÃ¬ treo á»Ÿ Ä‘Ã¢y */
        while (1);
    }

    while (1)
    {
        // VÃ²ng láº·p main Ä‘á»ƒ trá»‘ng, má»�i viá»‡c xá»­ lÃ½ á»Ÿ ngáº¯t Systick
    }
}

/* HÃ m khá»Ÿi táº¡o GPIO */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // Cáº¥p xung nhá»‹p cho PORT A
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // Cáº¥u hÃ¬nh chÃ¢n PA0, PA1, PA2 lÃ  Output Push-Pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // Táº¯t cÃ¡c LED ban Ä‘áº§u
    GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
}

/* 
 * TrÃ¬nh phá»¥c vá»¥ ngáº¯t Systick (SysTick Handler).
 * HÃ m nÃ y Ä‘Æ°á»£c gá»�i tá»± Ä‘á»™ng má»—i 1ms.
 */
void SysTick_Handler(void)
{
    // TÄƒng cÃ¡c biáº¿n Ä‘áº¿m lÃªn 1 (tÆ°Æ¡ng á»©ng 1ms)
    count_0_1Hz++;
    count_1Hz++;
    count_10Hz++;

    // --- 1. NhÃ¡y LED chÃ¢n PA0 táº§n sá»‘ 0.1Hz (Ä�áº£o tráº¡ng thÃ¡i má»—i 5000ms) ---
    if (count_0_1Hz >= 5000)
    {
        GPIOA->ODR ^= GPIO_Pin_0; // Ä�áº£o tráº¡ng thÃ¡i báº±ng cÃ¡ch XOR thanh ghi ODR
        count_0_1Hz = 0;
    }

    // --- 2. NhÃ¡y LED chÃ¢n PA1 táº§n sá»‘ 1Hz (Ä�áº£o tráº¡ng thÃ¡i má»—i 500ms) ---
    if (count_1Hz >= 500)
    {
        GPIOA->ODR ^= GPIO_Pin_1;
        count_1Hz = 0;
    }

    // --- 3. NhÃ¡y LED chÃ¢n PA2 táº§n sá»‘ 10Hz (Ä�áº£o tráº¡ng thÃ¡i má»—i 50ms) ---
    if (count_10Hz >= 50)
    {
        GPIOA->ODR ^= GPIO_Pin_2;
        count_10Hz = 0;
    }
}
