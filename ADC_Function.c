#include "ADC_function.h"


void Init_ADC1(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //ВКЛЮЧЕНИЕ ТАКТИРОВАНИЯ АЦП

	ADC1->CR1 = 0;
	ADC1->CR2 = 0;

//	ADC1->CR1 |= 0b0000<<ADC_CR1_AWDCH_Pos | 0b00001<<ADC_CR1_AWDCH_Pos; //ВЫБОР СТОРОЖЕВОГО КАНАЛА (КАНАЛЫ 0 И 1)
	ADC1->CR1 |= 0b001<<ADC_CR1_DISCNUM_Pos;
	ADC1->CR1 |= ADC_CR1_SCAN;

	ADC1->SMPR2 |= 0b000<<ADC_SMPR2_SMP0_Pos | 0b000<<ADC_SMPR2_SMP1_Pos; //ВЫБОР 3 ЦИКЛОВ НУЛЕВОГО И ПЕРВОГО КАНАЛОВ
	ADC1->SQR1 |= 0b0001<<ADC_SQR1_L_Pos; //НАСТРОЙКА ОДНОГО ПРЕОБРАЗОВАНИЯ ДВУХ КАНАЛОВ
	ADC1->SQR2 |= 0;
//	ADC1->SQR3 |= 0b00001<<ADC_SQR3_SQ1_Pos | 0b00010<<ADC_SQR3_SQ2_Pos;

	ADC->CCR |= 0b01<<ADC_CCR_ADCPRE_Pos; //ВЫБОР ПРЕДДЕЛИТЕЛЯ ДЛЯ АЦП (ПРЕДДЕЛИТЕЛЬ 4)

	ADC1->CR2 |= ADC_CR2_ADON; //РАЗРЕШИТЬ АЦП
}
uint16_t ADC1_Read(void)
{
	uint16_t Result;
	while(!(ADC1->SR & ADC_SR_EOC))//ПРОВЕРКА ФЛАГА ПРЕОБРАЗОВАНИЯ ОБЫЧНОГО КАНАЛА
	{
	}
	Result = ADC1->DR; //СЧИТЫВАНИЯ ИЗ ПАМЯТИ ДАННЫХ

	return Result;
}
void ADC1_Start(void)
{
	if(ADC1->CR2 & ADC_CR2_ADON)//ПРОВЕРКА УСТАНОВКИ БИТА РАЗРЕШЕНИЯ РАБОТЫ АЦП
	{
		ADC1->CR2 |= ADC_CR2_SWSTART; //ЗАПУСК АЦП
	}
}
