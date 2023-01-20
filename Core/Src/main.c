#include "stm32f0xx.h"

int led_position[18][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
							{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
							{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}};


void ClockConfig()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // Enable clock for GPIOA
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // Enable clock for GPIOC

}

void PinConfig()
{
	GPIOA->MODER |= (1 << 8) | (1 << 10) | (1 << 12) | (1 << 14) | (1 << 16) | (1 << 18) | (1 << 20) | (1 << 22) | (1 << 24); // Set PA4 to PA12 as output

	GPIOC->MODER &= ~GPIO_MODER_MODER13; // Clear mode register for PC13
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13; // Clear pull-up/pull-down register PC13
}

void Delay(int delay)
{
    for (int i = 0; i < delay; i++) {
      __NOP();
    }
}


void MemoryWritePortA(int two_bytes[16])
{

	for (int i = 0; i<16; i++)
	{
		if(two_bytes[i] == 1)
		{
			GPIOA->BSRR = (1 << i);
		}
		else
		{
			GPIOA->BRR = (1 << i);
		}
	}

}

int main(void) {

ClockConfig();
PinConfig();
int position = 0;


  while (1)
  {

  	if (GPIOC->IDR & GPIO_IDR_13)
  	{
  		if (position < 18)
  		{
  			MemoryWritePortA(led_position[position]);
  			Delay(50000);
  			position++;
  		}
  		else
  		{
  			position = 0;
  		}
  	}
  	else
  	{
  		if (position > 0)
  		{
  			MemoryWritePortA(led_position[position]);
  			Delay(50000);
  			position--;
  		}
  		else
  		{
  			position = 17;
  		}
  	}


  }

  return 0;
}
