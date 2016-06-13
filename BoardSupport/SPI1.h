#ifndef _SPI1_H
#define _SPI1_H

#include <LPC177x_8x.H>

#define SPI1_CS_LOW()       GPIO_ClearValue(0, (1<<6))
#define SPI1_CS_HIGH()      GPIO_SetValue(0,  (1<<6))

void  SPI1_Int(void);
uint8_t SPI1_SendByte(uint8_t dat);
void SPI1_SendData(uint8_t cmd, uint8_t* pData,uint8_t limits);
uint16_t SPI1_ReadData(void);


#endif

