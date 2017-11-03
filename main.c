/*#if !defined (STM32F10X_MD)
#define STM32F10X_MD
#endif
*/
#include  "stm32f4xx.h"
#include "stm32f407xx.h"
#include "GP_TIM.h"

// Frequency of signal in frames per seconds
float signalFrequency = 30;
// Impulse width (sec)
float impulseRatio= 0.5;

// Register values
int ARR = 0;
int CCR1 = 0;

// shift value
int shift = 0;

// Only for start
int startFlag = 1;

// Number of frames
int currentFrame = 0;

// Flag for changing setup (timer need correction)
int setupFlag = 0;

int indicator1 = 0;

void TIM2_IRQHandler()
{
	if (READ_BIT(TIM2->SR, TIM_SR_CC1IF))
		{
		if (shift)
			{
				GP_TIM_Set_ARR(TIM2, ARR + shift);
				setupFlag = 1;
				shift = 0;
				
				// We were hurrying up, so it is our first frame 
				currentFrame = 1;
			}
		// Turn off D12
		SET_BIT(GPIOD->BSRR, GPIO_BSRR_BR12);
		// Turn off A2
		SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR2);
		}
	if (READ_BIT(TIM2->SR, TIM_SR_UIF))
		{
		// Turn on D12
		SET_BIT(GPIOD->BSRR, GPIO_BSRR_BS12);
		// Turn on A2
		SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS2);
		if (setupFlag)
			{
				GP_TIM_Set_ARR(TIM2, ARR);
				setupFlag = 0;
			}
		if (currentFrame == signalFrequency + 2)
			currentFrame = 2;
		else
			currentFrame++;
		}
	// Clearing Status register
	GP_TIM_Clear_SR(TIM2);
}

void EXTI9_5_IRQHandler ()
{
	//if EXTI5 source
	if(EXTI->PR & (1 << 5)) 
		{   
      SET_BIT(EXTI->PR, EXTI_PR_PR5);
			if (startFlag)
				{
					// Turn on D12
					SET_BIT(GPIOD->BSRR, GPIO_BSRR_BS12);
					// Turn on A2
					SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS2);
					// Timer enable
					GP_TIM_Enable_Disable(TIM2, ENABLE);
					startFlag = 0;
					currentFrame = 1;
				}
			if (currentFrame == signalFrequency + 1)
				{
					shift = (TIM2->CNT);
				}
			else if (currentFrame == signalFrequency)
				{
					//Timer restart
					
					GP_TIM_Enable_Disable(TIM2, DISABLE);
					GP_TIM_Clear_CNT(TIM2);
					// Turn on D12
					SET_BIT(GPIOD->BSRR, GPIO_BSRR_BS12);
					// Turn on A2
					SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS2);
					GP_TIM_Enable_Disable(TIM2, ENABLE);
					
					//first frame started
					currentFrame = 1;
				}
    }
}
void EXTISetup()
{
	/* Enable SWO output */
	DBGMCU->CR = 0x00000020;
	
	// For PC5 external interrupt pin
	// Enable clocking for GPIOC
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);
	
	// Input mode for C5
	CLEAR_BIT(GPIOC->MODER, GPIO_MODER_MODER5_0);
	
	// Enable clocking for SYSCFG
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
	
	// Select external interrupt on PC5
	WRITE_REG(SYSCFG->EXTICR[1], SYSCFG_EXTICR2_EXTI5_PC );
	
	// Rising trigger selection for PC5
	SET_BIT(EXTI->RTSR, EXTI_RTSR_TR5);
	
	// Turn on interrupt
	SET_BIT(EXTI->IMR, EXTI_IMR_IM5);
}
void Setup()
{
	SystemCoreClockUpdate();
	// GPIO
	// Enable clocking for GPIO D
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);
	// Enable clocking for GPIO A
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
	
	// Output Push pull for D12
	SET_BIT(GPIOD->MODER, GPIO_MODER_MODE12_0);
	// Output Push pull for A2
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODE2_0);
	
	// Turn off D12
	SET_BIT(GPIOD->BSRR, GPIO_BSRR_BR12);
	// Turn off A2
	SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR2);
	
	//External interrupt on pin C5 setup
	EXTISetup();
	
	//Calculation of ARR register
	ARR = (SystemCoreClock/signalFrequency);
	CCR1 = impulseRatio*ARR;
	
	// Setup of timer
	GP_TIM_Setup(TIM2, ARR, CCR1);
	
	// Clearing Status register
	GP_TIM_Clear_SR(TIM2);
}
int main()
{
		Setup();
	__enable_irq();
	__NVIC_EnableIRQ(TIM2_IRQn);
	__NVIC_EnableIRQ(EXTI9_5_IRQn);
	while(1)
	{
		SystemCoreClockUpdate();
	}
}
