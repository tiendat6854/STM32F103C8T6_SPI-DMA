#include"spi.h"

void SPI2_Init(void)
{
	RCC->APB2ENR |= (1 << 3);  // clock GPIOB
	
  /* PB12 - NSS : Output push-pull */
	GPIOB->CRH &= ~(15 << 16);
	GPIOB->CRH |= (3 << 16);
	/* PB13 - SCK : Alternate function push-pull */
	GPIOB->CRH &= ~(15 << 20);
	GPIOB->CRH |= (11 << 20);
	/* PB14 - MISO : Input floating */
	GPIOB->CRH &= ~(15 << 24);
	GPIOB->CRH |= (4 << 24);
	/* PB15 - MOSI : Alternate function push-pull */
	GPIOB->CRH &= ~(15 << 28);
	GPIOB->CRH |= (11 << 28);
	
	RCC->APB1ENR |= (1 << 14); // clock SPI2
	SPI2->CR1 |= (2 << 3);   // 010:  fPCLK/8 = 36 / 8 (Mhz)
	SPI2->CR1 &= ~(1 << 1);  // CPOL = 0 : CK to 0 when idle
	SPI2->CR1 &= ~(1 << 0);  // CPHA = 0 : The first clock transition is the first data capture edge
	SPI2->CR1 |= (1 << 15) | (1 << 14); // 1-line bidirectional data mode selected
	SPI2->CR1 &= ~(1 << 11); // 8-bit data frame format is selected for transmission/reception
	SPI2->CR1 &= ~(1 << 10); // Full duplex (Transmit and receive)
	SPI2->CR1 &= ~(1 << 7);  // MSB transmitted first
	SPI2->CR1 |= (1 << 2);   // Master configuration
	SPI2->CR1 |= (1 << 9);   // Software slave management enabled
	SPI2->CR2 |= (1 << 2);   // SS output is enabled in master mode and when the cell is enabled
  SPI2->CR1 |= (1 << 6);   // SPI enable
}

void DMA1_Init(void)
{
  /*  DMA1 Channel 5 - SPI2_TX   */
	
  RCC->AHBENR |= (1 << 0);
	
	DMA1_Channel5->CCR &= ~(1 << 10);  // Memory size 8 bit
	
	DMA1_Channel5->CCR &= ~(1 << 8); //  Peripheral size 8 bit
	
	//Channel priority level very high, Memory increment mode enabled, Circular mode disabled, Data transfer direction: Read from memory
	DMA1_Channel5->CCR |= (3 << 12) | (1 << 7) | (0 << 5) | (1 << 4); 
	
	DMA1_Channel5->CMAR = (uint32_t)u8Buff;
	
	DMA1_Channel5->CPAR = (uint32_t)(&(SPI2->DR));
	
	DMA1_Channel5->CNDTR = sizeof(u8Buff);

	DMA1_Channel5->CCR |= (1 << 0);  // Channel enable
	
	SPI2->CR2|= (1 << 1);  // Tx buffer DMA enabled
} 

void SPI_Write(uint8_t *pu8Data, uint32_t u32Len)
{
  uint32_t i;
	for( i = 0; i< u32Len; ++i)
	{
	  SPI2->DR = pu8Data[i];
		while(SPI2->SR & 0x80); // SPI is busy
	}
}	
