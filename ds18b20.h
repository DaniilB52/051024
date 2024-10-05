#include "stm32f4xx_hal.h"

#ifndef DS18B20_H_
#define DS18B20_H_

#define SKIP_ROM 0
#define NO_SKIP_ROM 1
//--------------------------------------------------
#define RESOLUTION_9BIT 0x1F
#define RESOLUTION_10BIT 0x3F
#define RESOLUTION_11BIT 0x5F
#define RESOLUTION_12BIT 0x7F
//--------------------------------------------------

uint8_t ds18b20_Init(uint8_t mode);
void ds18b20_MeasureTemperCmd(uint8_t mode, uint8_t DevNum);
void ds18b20_ReadStratcpad(uint8_t mode, uint8_t *Data, uint8_t DevNum);
uint8_t ds18b20_GetSign(uint16_t dt);
float ds18b20_Convert(uint16_t dt);

/*
void ds18b20_WriteByte(uint8_t dt);
void ds18b20_WriteBit(uint8_t bit);
uint8_t ds18b20_ReadByte(void);
uint8_t ds18b20_ReadBit(void);
uint8_t ds18b20_Reset(void);
void ds18b20_Init(void);
void ds18b20_ReadTemp(uint8_t Data[]);
*/

#endif /* DS18B20_H_ */
