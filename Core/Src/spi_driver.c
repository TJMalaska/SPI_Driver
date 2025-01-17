#include "spi_driver.h"


void spi_init(SPI_TypeDef * SPIx, bool CPOL, bool CPHA, uint8_t direction, bool LSB){
    if (SPIx == SPI1){
        // clock enable
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;  
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    }
    //gpio alternate functions
    GPIOA->MODER &= ~(GPIO_MODER_MODE5 | GPIO_MODER_MODE6 | GPIO_MODER_MODE7);
    GPIOA->MODER |= GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1 ;
    //GPIOE->AFR[1] = 5 << GPIO_AFRH_AFSEL12_Pos;
    GPIOA->AFR[0] |= 5 << GPIO_AFRL_AFSEL5_Pos;
    GPIOA->AFR[0] |= 5 << GPIO_AFRL_AFSEL6_Pos;
    GPIOA->AFR[0] |= 5 << GPIO_AFRL_AFSEL7_Pos;

    //SPI configuration 
    SPIx->CR1 |= SPI_CR1_BR; // baud rate ctrl (1mhz i think)
    //we want to enable nsspmode for multiple slaves which means cpha low
    SPIx->CR1 |= CPOL << SPI_CR1_CPOL_Pos; //clock polarity : 0 idle low, 1 idle high
    SPIx->CR1 |= CPHA << SPI_CR1_CPHA_Pos; //clock phase : 0 first edge, 1 second edge

    //duplexity
    switch(direction){
        case 0: //reveive only
            SPIx->CR1 |= SPI_CR1_RXONLY;
            break;
        case 1: // half duplex
            SPIx->CR1 |= SPI_CR1_BIDIMODE;
            SPIx->CR1 |= SPI_CR1_BIDIOE;
            break;
        case 2://full duplex default
            break;
    }
    //msb
    SPIx->CR1 |= LSB << SPI_CR1_LSBFIRST;

    //software chip select
    SPIx->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
    SPIx->CR1 |= SPI_CR1_MSTR; // set to master mode
    SPIx->CR2 |= SPI_CR2_SSOE;  //set ss pin to output
    SPIx->CR2 |= SPI_CR2_FRXTH;

    SPIx->CR1 |= SPI_CR1_SPE;
}
void spi_tx(SPI_TypeDef * SPIx, uint8_t data){
    *(uint8_t *)&SPIx->DR = data;

}

void spi_transact(SPI_TypeDef * SPIx, uint8_t txbuf[], uint8_t rxbuf[], uint32_t txsize, uint32_t rxsize){
    for(int i = 0; i < txsize; i++){
        spi_tx(SPIx, txbuf[i]);
        while(SPIx->SR & SPI_SR_BSY){}
        spi_rx(SPIx, &rxbuf[i]);
    }
    
}

void spi_rx(SPI_TypeDef * SPIx, uint8_t * pdata){
    * pdata = * (volatile uint8_t *)&SPIx->DR;
}



