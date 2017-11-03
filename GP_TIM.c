#include "GP_TIM.h"
void GP_TIM_Setup(TIM_TypeDef* TIMx, int ARR, int CCR1)
{
	// Enable cloking for TIMx
	if (TIMx == TIM2)
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);
	else if (TIMx == TIM3)
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);
	else if (TIMx == TIM4)
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM4EN);
	else if (TIMx == TIM5)
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM5EN);
	
	// APB1 prescaler = 1
	SET_BIT(RCC->CFGR, RCC_CFGR_PPRE1_DIV1 );
	
	// Writing to Auto reload register and capture compare register
	WRITE_REG(TIMx->ARR, ARR);
	WRITE_REG(TIMx->CCR1,CCR1);
	
	// ARR register buffered
	SET_BIT(TIMx->CR1, TIM_CR1_ARPE);
	
	
	// Setup interrupts for Capture-compare 1 and update event
	SET_BIT(TIMx->DIER, TIM_DIER_UIE);
	SET_BIT(TIMx->DIER, TIM_DIER_CC1IE);

}
void GP_TIM_Enable_Disable(TIM_TypeDef* TIMx, FunctionalState state)
{
	if (state == ENABLE)
	SET_BIT(TIMx->CR1, TIM_CR1_CEN);
	else if (state == DISABLE)
	CLEAR_BIT(TIMx->CR1, TIM_CR1_CEN);
}
void GP_TIM_Set_ARR(TIM_TypeDef* TIMx, int ARR)
{
	WRITE_REG(TIMx->ARR, ARR);
}
void GP_TIM_Set_Clear_OPM(TIM_TypeDef* TIMx, FlagStatus mode)
{
	// One pulse mode
	if (mode == SET)
	SET_BIT(TIMx->CR1, TIM_CR1_OPM);
	else if (mode == RESET)
	CLEAR_BIT(TIMx->CR1, TIM_CR1_OPM);
}
void GP_TIM_Get_CNT(TIM_TypeDef* TIMx, int* CNT)
{
	*CNT = TIMx->CNT;
}
void GP_TIM_Clear_SR(TIM_TypeDef* TIMx)
{
	// Clearing Status register
	CLEAR_REG(TIMx->SR);
}
void GP_TIM_Clear_CNT(TIM_TypeDef* TIMx)
{
	TIMx->CNT = 0;

}
