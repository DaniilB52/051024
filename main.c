#include "main.h"
#include "stm32f4xx_hal.h"
#include "math.h"
#include "ADC_function.h"
#include "ds18b20.h"
#include "Timer_function.h"
#include "ThermocoupleTypeK.h"
#include "DefineInc.h"
#include "TSP_DTS054-100P.h"


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Init_GPIO(void);
void Init_SPI(void);
void SPI_Transmit_8BIT(SPI_TypeDef* SPI, uint8_t* data, uint16_t Size_data);
void SPI_Receive_8BIT(SPI_TypeDef* SPI, uint8_t* data, uint16_t Size_data);
void ConvertingNumberIntoADigitalArray(double DigitalValue, uint8_t ArrayOfNumbers[]);
void InitOfFigureLedDigitDisplay (uint8_t figure);
void InitOfLedScopeBoard(uint8_t ArrayOfNumber[], int SizeOfLedDigitDisplay);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);

double ResArray[100];
double PolinomArray500[10], PolinomArray1372[7];
uint16_t a1, a2;
uint8_t massiv[4];
int Zero=0; //ЧИСЛО ДЛЯ ЦИКЛА ЗАГОРАНИЯ СВЕТОДИОДНОГО ДИСПЛЕЯ
double TemperArray[9];



double tt, TempOfDisplay;
int main(void)
{
	  PolinomArray500[0] = 0.0;
	  PolinomArray500[1] = 2.508355 * 10.0;
	  PolinomArray500[2] = 7.860106 * 0.01; // pow(10,-2);
	  PolinomArray500[3] = -2.503131 * 0.1; // pow(10,-1);
	  PolinomArray500[4] = 8.315270 * 0.01; // pow(10,-2);
	  PolinomArray500[5] = -1.228034 * 0.01; // pow(10,-2);
	  PolinomArray500[6] = 9.804036 * 0.0001; // pow(10,-4);
	  PolinomArray500[7] = -4.413030 * 0.00001; // pow(10,-5);
	  PolinomArray500[8] = 1.057734 * 0.000001; // pow(10,-6);
	  PolinomArray500[9] = -1.052755 * 0.00000001; // pow(10,-8);
	  PolinomArray1372[0] = -1.318058 * 100; // pow(10,2);
	  PolinomArray1372[1] = 4.830222 * 10; // pow(10,1);
	  PolinomArray1372[2] = -1.646031;
	  PolinomArray1372[3] = 5.464731 * 0.01; // pow(10,-2);
	  PolinomArray1372[4] = -9.650715 * 0.0001; //pow(10,-4);
	  PolinomArray1372[5] = 8.802193 * 0.000001; //pow(10,-6);
	  PolinomArray1372[6] = -3.110810 * 0.00000001; // pow(10,-8);

	  HAL_Init();
	  SystemClock_Config();
	  MX_GPIO_Init();
	  Init_GPIO();
	  Init_SPI();
	  Init_ADC1();
	  Init_Timer2();
	  Init_Timer3();
	  Timer_Start(TIM2);
	  Timer_Start(TIM3);

	  NVIC_SetPriority(TIM2_IRQn, 0);
	  NVIC_SetPriority(TIM3_IRQn, 1);



  while (1)
  {

  }

}

void TIM3_IRQHandler(void)
{
	if(TIM3->SR & TIM_SR_UIF)
	{
		TIM3->SR &= ~TIM_SR_UIF;
	}

	if(a==0)
	{
		TempOfDisplay = ReadTemperatureFromThermocouple(100, PolinomArray500, PolinomArray1372, 0.77); //Если в последний аргумент написать 0, то коэффициент термопары установится по умолчанию
		a=1;
	}
	else
	{
		TempOfDisplay = RTDTemperatureMeasurement(ConvectorThemperatureVoltage()); //АРГУМЕНТОМ ФУНКЦИИ ЯВЛЯЕТСЯ ПОКАЗАНИЯ АЦП В ВОЛЬТАХ
		if(TempOfDisplay>28)
		{
			a=0;
		}
		a=0;
	}

}
void TIM2_IRQHandler(void)
{
	if(TIM2->SR & TIM_SR_UIF)
	{
		TIM2->SR &= ~TIM_SR_UIF;
	}
	ConvertingNumberIntoADigitalArray(TempOfDisplay, massiv); //КОНВЕРТАЦИЯ ВЕЩЕСТВЕННОГО ЧИСЛА В МАССИВ ЦЕЛЫХ ЧИСЕЛ ДЛЯ ЦИФРОВОГО ДИСПЛЕЯ
	InitOfLedScopeBoard(massiv, 5); //ВЫЗОВ ФУНКЦИИ ОТОБРАЖЕНИЯ ЦИФР ИЗ МАССИВА НА LED-ДИСПЛЕЕ
}
void InitOfLedScopeBoard(uint8_t ArrayOfNumber[], int SizeOfLedDigitDisplay)
{
	//ПООЧЕРЕДНОЕ ОТОБРАЖЕНИЕ ЦИФР МАССИВА. ТАК КАК ОТОБРАЖЕНИЕ ИДЕТ ПО ТАЙМЕРУ В 2мс, ТО НА КАЖДОЕ ПРЕРЫВАНИЕ ТАЙМЕРА ФУНКЦИЯ СЧИТАЕТ СЛЕДУЮЩУЮ ЦИФРУ
	//ПО ДОСТИЖЕНИИ КОНЕЧНОЙ - СЧЕТЧИК ОБНУЛЯЕТСЯ
	InitOfFigureLedDigitDisplay(ArrayOfNumber[Zero]);
	switch (Zero)
	{
	case 0: //ЕСЛИ ПЕРВАЯ ЦИФРА МАССИВА РАВНА НУЛЮ, ТО ОНО НЕ ОТОБРАЖАЕТСЯ, АНАЛОГИЧНО ДЛЯ ВТОРОЙ ЦИФРЫ
		if(ArrayOfNumber[Zero] == 0)
		{
			RESET_D1;
		}
		else
		{
			SET_D1;
		}
		RESET_D2; RESET_D3; RESET_D4; RESET_D5; RESET_DP;
		break;
	case 1:
		if(ArrayOfNumber[Zero] == 0)
		{
			RESET_D2;
		}
		else
		{
			SET_D2;
		}
		RESET_D1; RESET_D3; RESET_D4; RESET_D5; RESET_DP;
		break;
	case 2:
		RESET_D1; RESET_D2; SET_D3; RESET_D4; RESET_D5; SET_DP;
		break;
	case 3:
		RESET_D1; RESET_D2; RESET_D3; SET_D4; RESET_D5;	RESET_DP;
		break;
	case 4:
		RESET_D1; RESET_D2; RESET_D3; RESET_D4; RESET_B; RESET_C; RESET_F; RESET_G;	RESET_DP; SET_A; SET_F;	SET_E; SET_D; SET_D5;
		break;
	}
	Zero++;
	if(Zero==SizeOfLedDigitDisplay) //ОБНУЛЕНИЕ СЧЕТЧИКА
	{
		Zero=0;
	}

}
void InitOfFigureLedDigitDisplay (uint8_t figure)
{
		switch (figure)
		{
		case 0: //ЦИФРА -- 0
			SET_A; SET_B; SET_C; SET_D; SET_E; SET_F; RESET_G;
			break;
		case 1: //ЦИФРА -- 1
			SET_B; SET_C; RESET_D; RESET_E; RESET_F; RESET_G; RESET_A;
			break;
		case 2: //ЦИФРА -- 2
			SET_A; SET_B; SET_G; SET_E; SET_D; RESET_F; RESET_C;
			break;
		case 3: //ЦИФРА -- 3
			SET_A; SET_B; SET_C; SET_D; SET_G; RESET_E; RESET_F;
			break;
		case 4: //ЦИФРА -- 4
			RESET_A; SET_B; SET_C; RESET_D; SET_G; RESET_E; SET_F;
			break;
		case 5: //ЦИФРА -- 5
			SET_A; RESET_B; SET_C; SET_D; SET_G; RESET_E; SET_F;
			break;
		case 6: //ЦИФРА -- 6
			SET_A; RESET_B; SET_C; SET_D; SET_G; SET_E; SET_F;
			break;
		case 7: //ЦИФРА -- 7
			SET_A; SET_B; SET_C; RESET_D; RESET_G; RESET_E; RESET_F;
			break;
		case 8: //ЦИФРА -- 8
			SET_A; SET_B; SET_C; SET_D; SET_G; RESET_E; RESET_F;
			break;
		case 9: //ЦИФРА -- 9
			SET_A; SET_B; SET_C; SET_D; SET_G; RESET_E; SET_F;
			break;
		}
}
void ConvertingNumberIntoADigitalArray(double DigitalValue, uint8_t ArrayOfNumbers[])
{
	ArrayOfNumbers[0] = ((int)DigitalValue)/100; //сотни
	ArrayOfNumbers[1] = ((int)DigitalValue)%100/10; //десятки
	ArrayOfNumbers[2] = ((int)DigitalValue)%10; //единицы
	ArrayOfNumbers[3] = ((int)(DigitalValue*100.0))%100/10; //десятые
}

void SPI_Receive_8BIT(SPI_TypeDef* SPI, uint8_t* data, uint16_t Size_data)
{
	if (!READ_BIT(SPI->SR, SPI_SR_BSY))
	{
		//Проверим занятость шины

		if (READ_BIT(SPI->SR, SPI_SR_OVR) || READ_BIT(SPI->SR, SPI_SR_RXNE))
		{
			//Т.к. мы можем принимать данные в любой момент, например после режима "transmit-only mode"
			//то следует проверить статусы OVR и RXNE. Если хотя бы один из них установлен,
			//то сбросим их при помощи чтения регистра DR.
			SPI->DR;
		}

		//Начнем прием данных
		for (uint16_t i = 0; i < Size_data; i++)
		{
			SPI->DR = 0; //Запустим тактирование, чтоб считать 8 бит
			while (!READ_BIT(SPI->SR, SPI_SR_RXNE))
			{
				HAL_Delay(100);
				break;
			}
			*(data + i) = SPI->DR; //Считываем данные
		}

		while (READ_BIT(SPI->SR, SPI_SR_BSY))
		{
			//Затем подождем, пока BSY станет равным 0.
			//Это указывает на то, что прием последних данных завершен.
			HAL_Delay(100);
			break;
		}
	}
}
void SPI_Transmit_8BIT(SPI_TypeDef* SPI, uint8_t* data, uint16_t Size_data)
{
	if (!READ_BIT(SPI->SR, SPI_SR_BSY))
	{
		//Проверим занятость шины
		SPI->DR = *(data); //Запишем первый элемент данных для отправки в регистр SPI_DR
		//(При этом очищается бит TXE)

		for (uint16_t i = 1; i < Size_data; i++)
		{
			HAL_Delay(100);
			while (!READ_BIT(SPI->SR, SPI_SR_TXE))
			{
				HAL_Delay(100);
				break;
			}
			SPI->DR = *(data + i); //Запишем следующий элемент данных.
		}
		while (!READ_BIT(SPI->SR, SPI_SR_TXE))
		{
			//После записи последнего элемента данных в регистр SPI_DR,
			//подождем, пока TXE станет равным 1.
			HAL_Delay(100);
			break;
		}
	}
	while (READ_BIT(SPI->SR, SPI_SR_BSY))
	{
	}
}
void Init_GPIO(void)
{
	RCC->AHB1ENR |= 255; //ВКЛЮЧЕНИЕ ТАКТИРОВАНИЯ ВСЕХ ПОРТОВ
	RCC->AHB1LPENR |= RCC_AHB1LPENR_GPIOBLPEN|RCC_AHB1LPENR_GPIOCLPEN|RCC_AHB1LPENR_GPIOALPEN;

	/*НАСТРОЙКА ПОРТОВ ВХОДА/ВЫХОДА*/

	/*НАСТРОЙКА ПОРТОВ ДЛЯ СВЕТОДИОДНОГО ЦИФРОВОГО ТАБЛО*/
	GPIOA->PUPDR = 0;
	GPIOA->OSPEEDR = 0;
	GPIOA->OTYPER = 0;

	GPIOC->MODER |= 0b01<<GPIO_MODER_MODE7_Pos; //PC7
	GPIOC->MODER |= 0b01<<GPIO_MODER_MODE9_Pos; //PC9
	GPIOA->MODER |= 0b01<<GPIO_MODER_MODE9_Pos; //PA9
	GPIOB->MODER |= 0b01<<GPIO_MODER_MODE6_Pos; //PB6
	GPIOA->MODER |= 0b01<<GPIO_MODER_MODE10_Pos; //PA10
	GPIOB->MODER |= 0b01<<GPIO_MODER_MODE3_Pos; //PB3
	GPIOB->MODER |= 0b01<<GPIO_MODER_MODE8_Pos; //PB8
	GPIOB->MODER |= 0b01<<GPIO_MODER_MODE9_Pos; //PB9
	GPIOA->MODER |= 0b01<<GPIO_MODER_MODE6_Pos; //PA6
	GPIOA->MODER |= 0b01<<GPIO_MODER_MODE7_Pos; //PA7

	GPIOB->MODER |= 0b01<<GPIO_MODER_MODE13_Pos; //PB13
	GPIOB->MODER |= 0b01<<GPIO_MODER_MODE14_Pos; //PB14
	GPIOB->MODER |= 0b01<<GPIO_MODER_MODE15_Pos; //PB15
	GPIOB->MODER |= 0b01<<GPIO_MODER_MODE1_Pos; //PB1
	GPIOB->MODER |= 0b01<<GPIO_MODER_MODE2_Pos; //PB2


	/*НАСТРОЙКА ПОРТА PA5 ДЛЯ РАБОТЫ С ДАТЧИКОМ ds18b20*/
	GPIOA->MODER |= 0b01<<GPIO_MODER_MODE5_Pos; //НАСТРОЙКА ПОРТА  НА ВЫХОД
	GPIOA->OTYPER |= GPIO_OTYPER_OT5; //НАСТРОКА ПОРТА НА OPEN-DRAIN
	GPIOA->OSPEEDR |= 0b11<<GPIO_OSPEEDR_OSPEED5_Pos; //НАСТРОЙКА МАКСИМАЛЬНОЙ СКОРОСТИ ПОРТА

	/*НАСТРОЙКА ПОРТОВ АНАЛОГОВОГО ВХОДА ДЛЯ ADC1*/
	GPIOA->MODER |= GPIO_MODER_MODE0; //НАСТРОЙКА ПОРТА А0 НА АНАЛОГОВЫЙ РЕЖИМ
	GPIOA->PUPDR |= 0b00<<GPIO_PUPDR_PUPD0; //A0 НАСТРАИВАЕМ БЕЗ ПОДТЯГИВАНИЙ

	GPIOA->MODER |= GPIO_MODER_MODE1; //НАСТРОЙКА ПОРТА А1 НА АНАЛОГОВЫЙ РЕЖИМ
	GPIOA->PUPDR |= 0b00<<GPIO_PUPDR_PUPD1; //A1 НАСТРАИВАЕМ БЕЗ ПОДТЯГИВАНИЙ

	/*НАСТРОЙКА CS PIN PC3*/
	GPIOB->MODER |= 0b00<<GPIO_MODER_MODER3_Pos; //НАСТРЙОКА НА ВЫВОД
	GPIOB->OSPEEDR |= 0b00<<GPIO_OSPEEDR_OSPEED3_Pos; //LOW SPEED


}
void Init_SPI(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_SPI3EN; //включение тактирвоания для SPI3

	SPI3->CR1 |= (0b111<<SPI_CR1_BR_Pos); //выбор предделителя 16
	SPI3->CR1 |= SPI_CR1_CPHA;//фаза 1
	SPI3->CR1 |= SPI_CR1_CPOL;//полярность 1
	SPI3->CR1 &= ~SPI_CR1_RXONLY; //выбор дуплексного режима
	SPI3->CR1 &= ~SPI_CR1_BIDIMODE; //выбор двухнаправленной передачи
	SPI3->CR1 &= ~SPI_CR1_LSBFIRST; //выбор формата MSB
	SPI3->CR1 |= SPI_CR1_SSM;
	SPI3->CR1 |= SPI_CR1_SSI;
	SPI3->CR1 |= SPI_CR1_MSTR; //установка статуса микроконтроллера в МАСТЕР
	SPI3->CR1 &= ~SPI_CR1_DFF; //выбор формата обмена 8 бит
	SPI3->CR1 |= SPI_CR1_SPE;

	SPI3->CR2 = 0;

	SPI3->I2SCFGR &= ~SPI_I2SCFGR_I2SMOD;

}
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

static void MX_GPIO_Init(void)
{

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

}

void _Error_Handler(char * file, int line)
{

  while(1) 
  {
  }
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif
