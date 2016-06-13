#include "SPI2.h"
#include "lpc177x_8x_ssp.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_pinsel.h"
#include "DMA.h"
#define SSP_TX_SRC_DMA_CONN		(GPDMA_CONN_SSP2_Tx)
#define SSP_RX_SRC_DMA_CONN		(GPDMA_CONN_SSP2_Rx)


void SPI2_Int(void)
{
	 SSP_CFG_Type SSP_ConfigStruct;
  PINSEL_ConfigPin(1, 0, 4);//p1.0��SSP0_SCK
  PINSEL_ConfigPin(1, 8, 4);//p1.8: CS //SSP0_SSEL
  PINSEL_ConfigPin(1, 4, 4);//p1.4: SSP0_MISO
  PINSEL_ConfigPin(1, 1, 4);//p1.1: SSP0_MOSI


	SSP_ConfigStruct.CPHA = SSP_CPHA_FIRST;//SSP���ƼĴ�����֡����ĵ�һ��ʱ�������ز���������
	SSP_ConfigStruct.CPOL = SSP_CPOL_HI;//SSP������ʹ����ʱ����ÿ֡���ݴ���֮�䱣�ָߵ�ƽ
	SSP_ConfigStruct.ClockRate = 1000000;//
	SSP_ConfigStruct.Databit = SSP_DATABIT_8;//ÿ֡8λ����
	SSP_ConfigStruct.Mode = SSP_SLAVE_MODE;//ѡ��SSPΪ�ӻ�
	SSP_ConfigStruct.FrameFormat = SSP_FRAME_SPI;//SPI ģʽ
	
	SSP_Init(LPC_SSP2, &SSP_ConfigStruct);//��ʼ��SSP2

	LPC_SSP2->CR1 |= SSP_CR1_SSP_EN;//
	//SSP_Cmd(LPC_SSP2, ENABLE);	/* ʹ��SPI ���� 1 */
}



