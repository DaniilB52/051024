#include "ds18b20.h"
#include "stm32f4xx_hal.h"
#include "math.h"
#include "ADC_function.h"
#include "main.h"


#ifndef THERMOCOUPLETYPEK_H_
#define THERMOCOUPLETYPEK_H_

double ReadTemperatureFromThermocouple(int NumberOfReads, double PolinomArrayLow[], double PolinomArrayHigh[], double GainThermocouple);


uint16_t Value;
uint16_t ValueAverage;
uint16_t AverageVelue;
double ConversionToVoltage;
double VoltageOnThermocouple;
double ThermocoupleTemperature;
long double Temp, Res;
uint16_t frt, temperature, a;
double readTemp, kt;
uint8_t StatusDs18b20;
uint8_t dt[8];


/*Полиномы для термопары К
	ArrayLow[0] = 0.0;
	ArrayLow[1] = 2.508355 * 10.0;
	ArrayLow[2] = 7.860106 * pow(10,-2);
	ArrayLow[3] = -2.503131 * pow(10,-1);
	ArrayLow[4] = 8.315270 * pow(10,-2);
	ArrayLow[5] = -1.228034 * pow(10,-2);
	ArrayLow[6] = 9.804036 * pow(10,-4);
	ArrayLow[7] = -4.413030 * pow(10,-5);
	ArrayLow[8] = 1.057734 * pow(10,-6);
	ArrayLow[9] = -1.052755 * pow(10,-8);

	ArrayHigh[0] = -1.318058 * pow(10,2);
	ArrayHigh[1] = 4.830222 * pow(10,1);
	ArrayHigh[2] = -1.646031;
	ArrayHigh[3] = 5.464731 * pow(10,-2);
	ArrayHigh[4] = -9.650715 * pow(10,-4);
	ArrayHigh[5] = 8.802193 * pow(10,-6);
	ArrayHigh[6] = -3.110810 * pow(10,-8);
*/
#endif /* THERMOCOUPLETYPEK_H_ */
