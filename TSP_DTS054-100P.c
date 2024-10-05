#include "TSP_DTS054-100P.h"



double RTDTemperatureMeasurement(double Voltage)
{
	double TemperatureRTD = 0, TemperatureRTDfiltr = 0;;
	double Amperage = 0.0195, Resistors0 = 100.0, ResistorsTSP100;
//	double k2 = 0.00282;
	double k2 = 0.0036;
	int a=0; //СЧЕТЧИК ДЛЯ WHILE
	ResistorsTSP100 = Voltage/Amperage; //ВЫЧИСЛЕНИЕ СОПРОТИВЛЕНИЯ RTD ПРИ ПОСТОЯННОЙ СИЛЕ ТОКА

	if(Voltage > 0.0 && Voltage < 3.3)
	{
		while(a<10)
		{
			TemperatureRTD = (ResistorsTSP100 / Resistors0 - 1.0) / k2; //ВЫЧИСЛЕНИЕ ТЕМПЕРАТУРЫ ПО ФОРМУЛЕ ГОСТ 6651-2009
 			TemperatureRTDfiltr = TemperatureRTD * TemperatureRTD + TemperatureRTDfiltr;
			a++;
		}
		return sqrt(TemperatureRTDfiltr / 10.0);
	}
	else
	{
		return 0;
	}
}
double ConvectorThemperatureVoltage()
{
	uint16_t ValueChannel2;
	double VoltageChannel2;
	int NumberOfReads = 10;
	int ValueAverage = 0;

	for(int AccountNumber=0; AccountNumber<NumberOfReads; AccountNumber++)
	{
		ADC1->SQR3 |= 0b00001<<ADC_SQR3_SQ2_Pos; //ВЫБОР КАНАЛА АЦП
		ADC1_Start(); //ЗАПУСК АЦП
		ValueChannel2 = ADC1_Read(); //ЧТЕНИЕ ДАННЫХ С АЦП
		ADC1->SQR3 = 0; //ОБНУЛЕНИЕ РЕГИСТРА ВЫБОРА КАНАЛА АЦП
		ValueAverage = ValueChannel2 * ValueChannel2 + ValueAverage; //СУММА ЗНАЧЕНИЙ ПРИНЯТЫХ АЦП
	}
	ValueChannel2 = sqrt(ValueAverage/NumberOfReads); //СРЕДНЕЕ квадратичное ЗНАЧЕНИЕ ПОКАЗАНИЙ АЦП

	VoltageChannel2 = (double)3.3 * (double)ValueChannel2 / (double)4096.0; //ПЕРЕСЧЕТ напряжения на входе АЦП

	return VoltageChannel2;
}
