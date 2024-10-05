#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f446xx.h"


#ifndef TIMER_FUNCTION_H_
#define TIMER_FUNCTION_H_

#define Timer_Start(TIMx)	TIMx->CR1 |= TIM_CR1_CEN //ВКЛЮЧЕНИЕ СЧЕТЧИКА
#define Timer_Stop(TIMx)	TIMx->CR1 &= ~TIM_CR1_CEN //ВЫКЛЮЧЕНИЕ СЧЕТЧИКА

void Init_Timer2(void);
void Init_Timer3(void);

#endif /* TIMER_FUNCTION_H_ */
