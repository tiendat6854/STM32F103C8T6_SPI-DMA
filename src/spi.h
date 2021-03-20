#ifndef _spi_h_
#define _spi_h_

#include "stm32f10x.h" 

static uint8_t u8Buff[] = {0x14, 0x10, 0x00};

void SPI2_Init(void);
void DMA1_Init(void);
void SPI_Write(uint8_t *pu8Data, uint32_t u32Len);

#endif
