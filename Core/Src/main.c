#include "stm32f0xx.h"

// 2D array that contains all the ON and OFF states of the LEDs and will be cycled through to control them
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

// Function to enable clock for GPIOA and GPIOC
void ClockConfig()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // Enable clock for GPIOA
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // Enable clock for GPIOC

}

// Function to configure the IO pins
void PinConfig()
{
	// Set PA4 to PA12 as output
	GPIOA->MODER |= (1 << 8) | (1 << 10) | (1 << 12) | (1 << 14) | (1 << 16) | (1 << 18) | (1 << 20) | (1 << 22) | (1 << 24);

	// Set PC13 as input
	GPIOC->MODER &= ~GPIO_MODER_MODER13; // Clear mode register for PC13
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13; // Clear pull-up/pull-down register PC13
}

// Function to add a delay
void Delay(int delay)
{
    for (int i = 0; i < delay; i++) {
      __NOP();
    }
}

// Function that takes a 16 bit array and sets the GPIOA output pins according to the "0" and "1" states
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


int main(void)
{
	// Configure clock and IO pins
	ClockConfig();
	PinConfig();
	int position = 0; // Stores the current position of "led_position" array

	while (1)
	{
		// If button at PC13 is pressed, cycle in one direction
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
				position = 0; // Reset position to the beginning of the array
			}
		}
		// If button at PC13 is not pressed, cycle in the other direction
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
				position = 17; // Reset position to the end of the array
			}
		}
	}

	return 0;
}
