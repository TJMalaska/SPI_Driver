#include "spi_driver.h"


void spi_init(SPI_TypeDef * SPIx, uint32_t baudrate, bool ){
    if (SPIx == SPI1){
        // clock enable
        RCC->APB2ENR = RCC_APB2ENR_SPI1EN;  
        RCC->AHB2ENR = RCC_AHB2ENR_GPIOEEN;

        //gpio alternate functions
        GPIOE->MODER |= GPIO_MODER_MODE12_1 | GPIO_MODER_MODE13_1 | GPIO_MODER_MODE14_1 | GPIO_MODER_MODE15_1 ;
        GPIOE->AFR[1] = 5 << GPIO_AFRH_AFSEL12_Pos;
        GPIOE->AFR[1] = 5 << GPIO_AFRH_AFSEL13_Pos;
        GPIOE->AFR[1] = 5 << GPIO_AFRH_AFSEL14_Pos;
        GPIOE->AFR[1] = 5 << GPIO_AFRH_AFSEL15_Pos;

        //SPI configuration 
        SPIx->CR1 = SPI_CR1_BR_0; // baud rate ctrl (1mhz i think)
        //we want to enable nsspmode for multiple slaves



    }

}



