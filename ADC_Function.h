#include "stm32f4xx_hal.h"

#ifndef ADC_FUNCTION_H_
#define ADC_FUNCTION_H_

void Init_ADC1(void);
uint16_t ADC1_Read(void);
void ADC1_Start(void);

#endif /* ADC_FUNCTION_H_ */
