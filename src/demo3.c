#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

void RCC_Configuration_Demo3()
{
	// RCC Configuration
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
}

void GPIO_Configuration_Demo3()
{
	// GPIO Configuration
	// LED3 (Green): GPIO_Pin_13
	GPIO_InitTypeDef GPIO_InitStructure_O;
	GPIO_InitStructure_O.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure_O.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure_O.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure_O.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure_O.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure_O);

        GPIO_InitTypeDef GPIO_InitStructure_I;
        GPIO_InitStructure_I.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure_I.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure_I.GPIO_PuPd = GPIO_PuPd_NOPULL;	// Floating
        GPIO_Init(GPIOG, &GPIO_InitStructure_I);
}

void demo3()
{
	RCC_Configuration_Demo3();
	GPIO_Configuration_Demo3();

	// Demo Code: flashing LED3 by polling GPIO_Pin_9
	while(1)
	{
		// GPIO read IDR and assign to ODR
		GPIOG->ODR = GPIOA->IDR << 13;
	}

}
