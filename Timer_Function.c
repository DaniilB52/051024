#include "Timer_Function.h"



void Init_Timer2(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;//ВКЛЮЧЕНИЕ ТАКТИРОВАНИЯ TIM2


	TIM2->CR1 &= ~TIM_CR1_DIR; //СЧЕТЧИК ИСПОЛЬЗУЕТСЯ КАК ПОВЫШАЮЩИЙ
	TIM2->CR1 |= 0b00<<TIM_CR1_CMS_Pos; //ВЫРАВНИВАНИЕ ПО КРАЯМ
	TIM2->CR1 |= 0b00<<TIM_CR1_CKD_Pos;

	NVIC_EnableIRQ(TIM2_IRQn); //ВКЛЮЧАЕМ ГЛОБАЛЬНЫЕ ПРЕРЫВАНИЯ ДЛЯ TIM2;

	TIM2->PSC = 45000; //ПЕРИОД ТАКТИРОВАНИЯ МИЛИСЕКУНДЫ
	TIM2->ARR = 2; //СЧЕТЧИК ДО Xx, ПОЛУЧАЕТСЯ ПЕРИОД СЧЕТА ТАЙМЕРА Xx МИЛИСЕКУНД

	TIM2->DIER |= TIM_DIER_UIE; //ВКЛЮЧЕНИЕ ПРЕРЫВАНИЙ ОТ TIM2;

}

void Init_Timer3(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;//ВКЛЮЧЕНИЕ ТАКТИРОВАНИЯ TIM2


	TIM3->CR1 &= ~TIM_CR1_DIR; //СЧЕТЧИК ИСПОЛЬЗУЕТСЯ КАК ПОВЫШАЮЩИЙ
	TIM3->CR1 |= 0b00<<TIM_CR1_CMS_Pos; //ВЫРАВНИВАНИЕ ПО КРАЯМ
	TIM3->CR1 |= 0b00<<TIM_CR1_CKD_Pos;

	NVIC_EnableIRQ(TIM3_IRQn); //ВКЛЮЧАЕМ ГЛОБАЛЬНЫЕ ПРЕРЫВАНИЯ ДЛЯ TIM2;

	TIM3->PSC = 45000; //ПЕРИОД ТАКТИРОВАНИЯ МИЛИСЕКУНДЫ
	TIM3->ARR = 5000; //СЧЕТЧИК ДО Xx, ПОЛУЧАЕТСЯ ПЕРИОД СЧЕТА ТАЙМЕРА Xx МИЛИСЕКУНД

	TIM3->DIER |= TIM_DIER_UIE; //ВКЛЮЧЕНИЕ ПРЕРЫВАНИЙ ОТ TIM2;

}
