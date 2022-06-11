
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* AHB1 Base Addresses ******************************************************/
#define STM32_RCC_BASE    0x40023800               /* 0x40023800-0x40023bff: Reset
                                                  and Clock control RCC */

/* AHB2 Base Addresses ******************************************************/
#define STM32_GPIOC_BASE     0x48000800U           /* 0x48000800-0x48000bff: GPIO3
                                                    Port C */           
#define STM32_GPIOA_BASE   0x40020000              /* 0x40020000-0x400203FF: GPIO PORT A */                                                   

/* Register Offsets *********************************************************/

#define STM32_RCC_AHB1ENR_OFFSET  0x0030           /* AHB1 Peripheral Clock enableregister */

#define STM32_GPIO_MODER_OFFSET   0x0000           /* GPIO port mode register */

#define STM32_GPIO_OTYPER_OFFSET  0x0004           /* GPIO port output type register */

#define STM32_GPIO_PUPDR_OFFSET   0x000c           /* GPIO port pull-up/pull-downregister */

#define STM32_GPIO_BSRR_OFFSET    0x0018            /* GPIO port bit set/reset register */

#define STM32_GPIO_ODR_OFFSET     0x0014            /* GPIO port output data register */

/* Register Addresses *******************************************************/

#define STM32_RCC_AHB1ENR          (STM32_RCC_BASE+STM32_RCC_AHB1ENR_OFFSET)

#define STM32_GPIOC_MODER          (STM32_GPIOC_BASE+STM32_GPIO_MODER_OFFSET)
#define STM32_GPIOC_OTYPER         (STM32_GPIOC_BASE+STM32_GPIO_OTYPER_OFFSET)
#define STM32_GPIOC_PUPDR          (STM32_GPIOC_BASE+STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIOC_BSRR           (STM32_GPIOC_BASE + STM32_GPIO_BSRR_OFFSET)

#define STM32_GPIOA_MODER          (STM32_GPIOA_BASE+STM32_GPIO_MODER_OFFSET)
#define STM32_GPIOA_OTYPER         (STM32_GPIOA_BASE+STM32_GPIO_OTYPER_OFFSET)
#define STM32_GPIOA_PUPDR          (STM32_GPIOA_BASE+STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIOA_ODR            (STM32_GPIOA_BASE+STM32_GPIO_ODR_OFFSET)  
#define STM32_GPIOA_BSRR           (STM32_GPIOA_BASE + STM32_GPIO_BSRR_OFFSET)

/* AHB1 Peripheral Clock enable register */

#define RCC_AHB1ENR_GPIOCEN         (1 << 2)   /* Bit 2:  IO port C clockenable */     
#define RCC_AHB1ENR_GPIOAEN         (1 << 0)   /* Bit 0:  IO port A clockenable */

/* GPIO port mode register */

#define GPIO_MODER_INPUT            (0)                 /* Input */
#define GPIO_MODER_OUTPUT           (1)                 /* General purpose output mode */
#define GPIO_MODER_ALT              (2)                 /* Alternate mode */
#define GPIO_MODER_ANALOG           (3)                 /* Analog mode */

#define GPIO_MODER13_SHIFT          (26)            
#define GPIO_MODER13_MASK           (3 << GPIO_MODER13_SHIFT)

/* GPIO port output type register */

#define GPIO_OTYPER_PP               (0)                /* 0=Output push-pull */
#define GPIO_OTYPER_OD               (1)                /* 1=Output open-drain */

#define GPIO_OT13_SHIFT              (13) 
#define GPIO_OT13_MASK               (1 << GPIO_OT13_SHIFT)

/* GPIO port pull-up/pull-down register */

#define GPIO_PUPDR_NONE               (0)               /* No pull-up, pull-down */
#define GPIO_PUPDR_PULLUP             (1)               /* Pull-up */
#define GPIO_PUPDR_PULLDOWN           (2)               /* Pull-down */

#define GPIO_PUPDR13_SHIFT            (26)
#define GPIO_PUPDR13_MASK             (3 << GPIO_PUPDR13_SHIFT)

#define GPIO_ODR_MASK(n) (1 << n)

/* GPIO port bit set/reset register */
#define GPIO_BSRR_SET(n)    (1 << (n))
#define GPIO_BSRR_RST(n)    (1 << (n + 16))

/* Configuration ************************************************************/
#define LOW_DELAY   10000
#define HIGH_DELAY  50000

static const char fw_version[] = {'V','1','.','0'};
static uint32_t led_status;

int main(int argc,char *argv[])
{
    uint32_t reg;
    
    /* Ponteiros para registradores */

    uint32_t *pRCC_AHB1ENR  = (uint32_t *)STM32_RCC_AHB1ENR;
    uint32_t *pGPIOC_MODER  = (uint32_t *)STM32_GPIOC_MODER;
    uint32_t *pGPIOC_OTYPER = (uint32_t *)STM32_GPIOC_OTYPER;
    uint32_t *pGPIOC_PUPDR  = (uint32_t *)STM32_GPIOC_PUPDR;
    uint32_t *pGPIOC_BSRR   = (uint32_t *)STM32_GPIOC_BSRR;

    uint32_t *pGPIOA_MODER  = (uint32_t *)STM32_GPIOA_MODER;
    uint32_t *pGPIOA_OTYPER = (uint32_t *)STM32_GPIOA_OTYPER;
    uint32_t *pGPIOA_PUPDR  = (uint32_t *)STM32_GPIOA_PUPDR;
    uint32_t *pGPIOA_ODR    = (uint32_t *)STM32_GPIOA_ODR;
    
    /* Habilita clock GPIOC */

    reg  = *pRCC_AHB1ENR;
    reg |= RCC_AHB1ENR_GPIOCEN;
    *pRCC_AHB1ENR = reg;
    
    /* Configura PC13 como saida pull-up off e pull-down off */

    
    reg = *pGPIOC_MODER;
    reg &= ~(GPIO_MODER13_MASK);
    reg |= (GPIO_MODER_OUTPUT << GPIO_MODER13_SHIFT);
    *pGPIOC_MODER = reg;  

    reg = *pGPIOC_OTYPER;
    reg &= ~(GPIO_OT13_MASK);
    reg |= (GPIO_OTYPER_PP << GPIO_OT13_SHIFT);
    *pGPIOC_OTYPER = reg;

    reg = *pGPIOC_PUPDR;
    reg &= ~(GPIO_PUPDR13_MASK);
    reg |= (GPIO_PUPDR_NONE << GPIO_PUPDR13_SHIFT);
    *pGPIOC_PUPDR = reg;

    while(1)
    {
        reg = *pGPIOA_ODR;
        reg &= ~GPIO_ODR_MASK(0);
        *pGPIOC_BSRR = GPIO_BSRR_RST(13);
        if( (bool)reg )
        {
             for (uint32_t i = 0; i < LOW_DELAY; i++);
        }else
        {
            for (uint32_t i = 0; i < HIGH_DELAY; i++);
        }       
        *pGPIOC_BSRR = GPIO_BSRR_SET(13);

    }
    /* Nao deveria chegar aqui */

    return EXIT_SUCCESS;
}