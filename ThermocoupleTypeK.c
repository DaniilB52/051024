#include <ThermocoupleTypeK.h>


double ReadTemperatureFromThermocouple(int NumberOfReads, double PolinomArrayLow[], double PolinomArrayHigh[], double GainThermocouple)
{
	ValueAverage = 0;
	Value = 0;
	Temp = 0;
	Res = 0;
	temperature = 0;
	ThermocoupleTemperature = 0;


	//ПРОВЕРКА ВВЕДЕННОГО КОЭФФИЦИЕНТА
	if(GainThermocouple!=0)
	{
		kt = GainThermocouple;
	}
	else
	{
		kt = 0.75; //коэффициент термопары 0.75
	}

	StatusDs18b20 = ds18b20_Init(SKIP_ROM);//ИНИЦИАЛИЗАЦИЯ ДАТЧИКА ds18b20
	ds18b20_ReadStratcpad(SKIP_ROM, dt, 0); //ЧТЕНИЕ ПОКАЗАНИЙ ДАТЧИКА ds18b20
	frt = ((uint16_t)dt[1]<<8)|dt[0]; //ЗАПИСИТЬ В 16-БИТНОМ ЗНАЧЕНИИ ДАННЫХ РЕГИСТРОВ ТЕМПЕРАТУРЫ ДАТЧИКА ds18b20
	temperature = ds18b20_Convert(frt); //ПЕРЕСЧЕТ ТЕМПЕРАТУРЫ, КОТРУЮ СЧИТАЕТ ДАТЧИК ds18b20

	//Суммируем NumberOfReads-количество считываний АЦП для нахождения среднего
	for(int AccountNumber=0; AccountNumber<NumberOfReads; AccountNumber++)
	{
		ADC1->SQR3 |= 0b00001<<ADC_SQR3_SQ1_Pos; //ВЫБОР КАНАЛА АЦП
		ADC1_Start(); //ЗАПУСК АЦП
		Value = ADC1_Read(); //ЧТЕНИЕ ДАННЫХ С АЦП
		ADC1->SQR3 = 0; //ОБНУЛЕНИЕ РЕГИСТРА ВЫБОРА КАНАЛА АЦП
		ValueAverage = Value * Value + ValueAverage; //СУММА ЗНАЧЕНИЙ ПРИНЯТЫХ АЦП

	}
	AverageVelue = sqrt(ValueAverage/NumberOfReads); //СРЕДНЕЕ квадратичное ЗНАЧЕНИЕ ПОКАЗАНИЙ АЦП
	ConversionToVoltage = (double) 3.3 / (double) 4096.0 * (double)AverageVelue;//Пересчет 12-битных значений АЦП в напряжение
	VoltageOnThermocouple = ConversionToVoltage / (double) 59.23;//ПЕРЕСЧЕТ ПРИНЯТОГО НАПРЯЖЕНИЕ В НАПРЯЖЕНИЕ ТЕРМОПАРЫ (КОЭФ-Т УСИЛЕНИЯ ОУ 59.23)

	/*НАХОЖДЕНИЕ ТЕМПЕРАТУРЫ ТЕРМОПАРЫ ТИПА К МЕТОДОМ АПРОКСИМАЦИИ*/

	if((VoltageOnThermocouple*1000.0>0.0) && (VoltageOnThermocouple*1000.0<20.644)) //ДЛЯ ДИАПАЗОНА ТЕМПЕРАТУР 0--500 *С
	{
		Res = PolinomArrayLow[9];
		for(int iTemp=1; iTemp<10; iTemp++)//9 -- ЧИСЛО КОЭЭФИЦИЕНТОВ АПРОКСИМАЦИИ
		{
			//РЕШЕНИЕ МНОЖЕСТВА АПРОКСИМАЦИИ МЕТОДОМ ГОРНЕРА
            Temp = (double)Res * (double)VoltageOnThermocouple * 1000 + (double)PolinomArrayLow[9-iTemp];
            Res = (double)Temp;
            /*РЕШЕНИЕ МНОЖЕСТВА АПРОКСИМАЦИИ МЕТОДОМ СУММИРОВАНИЯ, ОПИСАННОМ В ГОСТ Р 8.585-2001*/
//			Res = PolinomArray500[i] * pow(VoltageOnThermocouple * 1000, i);
//			Temp = Temp + Res;
		}
	}
	else if((VoltageOnThermocouple*1000.0 >= 20.644)&&(VoltageOnThermocouple*1000.0 < 54.886)) //ДЛЯ ДИАПАЗОНА ТЕМПЕРАТУР 500--1372 *С
	{
		Res = PolinomArrayHigh[9];
		for(int iTemp=1; iTemp<7; iTemp++)//7 -- ЧИСЛО КОЭЭФИЦИЕНТОВ АПРОКСИМАЦИИ
		{
			//РЕШЕНИЕ МНОЖЕСТВА АПРОКСИМАЦИИ МЕТОДОМ ГОРНЕРА
            Temp = (double)Res * (double)VoltageOnThermocouple * 1000 + (double)PolinomArrayHigh[6-iTemp];
            Res = (double)Temp;
            /*РЕШЕНИЕ МНОЖЕСТВА АПРОКСИМАЦИИ МЕТОДОМ СУММИРОВАНИЯ, ОПИСАННОМ В ГОСТ Р 8.585-2001*/
//			Res = PolinomArray1372[i] * pow(VoltageOnThermocouple * 1000, i);
//			Temp = Temp + Res;
		}
	}


//		ThermocoupleTemperature = (Temp + temperature) * kt;//ТЕМПЕРАТУРА НА ГОРЯЧЕМ СПАЕ ТЕРМОПАРЫ. КОМПЕНСАЦИЯ ХОЛОДНОГО СПАЯ ДАТЧИКОМ ds18b20

		ThermocoupleTemperature = Temp + kt * (temperature - 0.0);

	return ThermocoupleTemperature;
}

