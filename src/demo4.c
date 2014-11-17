#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_syscfg.h"

void RCC_Configuration_Demo4()
{
	// RCC Configuration
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
}

void GPIO_Configuration_Demo4()
{
	// GPIO Configuration
	// LED3 (Green): GPIO_Pin_13
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

        GPIO_InitTypeDef GPIO_InitStructure_I;
        GPIO_InitStructure_I.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure_I.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure_I.GPIO_PuPd = GPIO_PuPd_NOPULL;      // Floating
        GPIO_Init(GPIOG, &GPIO_InitStructure_I); 
}

void Interrupts_Configuration_Demo4(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Connect EXTI Line 0 to the button GPIO Pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, GPIO_PinSource9);
	
	/* Configure the EXTI line to generate an interrupt when the button is
	 * pressed. The button pin is high when pressed, so it needs to trigger
	 * when rising from low to high. */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable and set Button EXTI Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line1) != RESET)
	{
		// GPIO read IDR and assign to ODR
		GPIOG->ODR = GPIOA->IDR << 13;
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

void deme4()
{
	RCC_Configuration_Demo4();
	GPIO_Configuration_Demo4();
	Interrupts_Configuration_Demo4();

	// Demo Code: flashing LED3 by polling GPIO_Pin_9
	while(1)
	{
	}

}
