#pragma once
#include "stm32l4xx.h"
#include <stdbool.h>


void spi_init(SPI_TypeDef * SPIx, bool CPOL, bool CPHA, uint8_t direction, bool LSB);
void spi_tx(SPI_TypeDef * SPIx, uint8_t data);
void spi_transact(SPI_TypeDef * SPIx, uint8_t txbuf[], uint8_t rxbuf[], uint32_t txsize, uint32_t rxsize);
void spi_rx(SPI_TypeDef * SPIx, uint8_t * pdata);

void spi_cs_toggle(SPI_TypeDef * SPIx);

