#include "SPI1.h"
#include "lpc177x_8x_ssp.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_pinsel.h"
#include "DMA.h"


SSP_CFG_Type SSP_ConfigStruct;// SSP Configuration structure variable

void  SPI1_Int(void)
{
  PINSEL_ConfigPin(0, 7, 2);//p0.7：SSP0_SCK
  PINSEL_ConfigPin(0, 6, 0);//p0.6: CS //SSP0_SSEL
  PINSEL_ConfigPin(0, 8, 2);//p0.8: SSP0_MISO
  PINSEL_ConfigPin(0, 9, 2);//p0.9: SSP0_MOSI
	
	GPIO_SetDir(0, (1<<6), 1); 	/* 设置触摸片选管脚p1.8为输出 */
  GPIO_SetValue(0, (1<<6));
	
  SSP_ConfigStruct.ClockRate =1000000; 	/* 配置SPI参数最高20M */
  SSP_ConfigStruct.CPHA = SSP_CPHA_SECOND;
  SSP_ConfigStruct.CPOL = SSP_CPOL_LO; 
	SSP_ConfigStruct.Databit = SSP_DATABIT_8;
	SSP_ConfigStruct.Mode = SSP_MASTER_MODE;
	SSP_ConfigStruct.FrameFormat = SSP_FRAME_SPI;
	
	SSP_Init(LPC_SSP1, &SSP_ConfigStruct);// Initialize SSP peripheral with parameter given in structure above
	
	LPC_SSP1->CR1 |= SSP_CR1_SSP_EN;//SSP_Cmd(0, ENABLE);// Enable SSP peripheral	
// printf("\n\rSPI_INIT\n\r");
}
uint16_t SPI1_ReadData(void)
{
   uint16_t  result   = 0;
   
   SPI1_CS_HIGH();
   SPI1_CS_LOW();
   
   result  = SPI1_SendByte(0x40);
   result  = result << 8;
   
   result  |= SPI1_SendByte(0x00);
   
   SPI1_CS_HIGH();
   
   return result;
}



uint8_t  SPI1_SendByte(uint8_t dat)
{
   while( (LPC_SSP1->SR & (SSP_SR_TNF|SSP_SR_BSY))  !=  SSP_SR_TNF);
   LPC_SSP1->DR  = dat;
   while( (LPC_SSP1->SR & (SSP_SR_BSY|SSP_SR_RNE))  !=  SSP_SR_RNE);
   return (LPC_SSP1->DR);
}



void SPI1_SendData(uint8_t cmd, uint8_t* pData,uint8_t limits)
{
   SPI1_CS_HIGH();
   SPI1_CS_LOW();
   
   SPI1_SendByte(cmd);
   
   if(limits > 0  &&  limits < 3){
      int i  = 0;
      for(i=0; i<limits; i++){
         SPI1_SendByte(pData[i]);
      }
   }
   SPI1_CS_HIGH();
}























































































































