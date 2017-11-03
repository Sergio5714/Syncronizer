#include "stm32f4xx.h"
#include "stm32f407xx.h"

void GP_TIM_Setup(TIM_TypeDef* TIMx, int ARR, int CCR1);
void GP_TIM_Enable_Disable(TIM_TypeDef* TIMx, FunctionalState state);
void GP_TIM_Set_ARR(TIM_TypeDef* TIMx, int ARR);
void GP_TIM_Set_Clear_OPM(TIM_TypeDef* TIMx, FlagStatus mode);
void GP_TIM_Get_CNT(TIM_TypeDef* TIMx, int* CNT);
void GP_TIM_Clear_SR(TIM_TypeDef* TIMx);
void GP_TIM_Clear_CNT(TIM_TypeDef* TIMx);
