#include "stm32f10x.h"                  // Device header
#include"timer2.h"
#include"spi.h"

void GPIO_Init_PC13(void)
{
  RCC->APB2ENR |= (1 << 4);
	GPIOC->CRH &= ~(15 << 20);
	GPIOC->CRH |= (3 << 20);
}

void delay_ms(uint32_t t)
{
  while(t)
	{
	  TIM2->CNT = 0; 
		while(TIM2->CNT < 1000) {}
	  --t;
	}
}

void delay_us(uint32_t t)
{
	  TIM2->CNT = 0; 
	  while (TIM2->CNT < t) {}
}

int main()
{
  GPIO_Init_PC13();
	timer2_Init();
	SPI2_Init();
	
	uint8_t i;
	
	GPIOB->ODR |= (1 << 12);
	delay_us(100);
	
	/* gui du lieu khong dung DMA */

	GPIOB->ODR &= ~(1 << 12);
	SPI_Write(u8Buff, sizeof(u8Buff));
	GPIOB->ODR |= (1 << 12);
	
	delay_us(100);
	
	/* gui du lieu dung DMA */
	
	for(i = 0; i < sizeof(u8Buff); i++)
	{
		GPIOB->ODR &= ~(1 << 12);
		DMA1_Init();
		while(DMA1_Channel5->CNDTR);
	}
	
	GPIOB->ODR |= (1 << 12);
	
	while(1)
	{
	  GPIOC->ODR |= (1 << 13);
		delay_ms(500);
		GPIOC->ODR &= ~(1 << 13);
		delay_ms(500);
	}
}
