#include "lpc177x_8x_gpdma.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_ssp.h"
#include "dma.h"
#include "uart.h"
#include "lpc177x_8x_uart.h"
#include <ucos_ii.h>
#include "GUI.h"
//#include "WM.h"
#include "MainTask.h"  
#include "pwm.h"
#include "Config.h"
#include "lpc_types.h"
//#include "dlg.h"
/*---------------------------- Macro defines --------------------------------------*/
#define DMA_SIZE		50

#define NUM_BYTE     7
#define DST_X_BYTE   5 
#define DST_Y_BYTE   2
#define CMD_BYTE     1

/*---------------------------- External  variables --------------------------------*/
extern volatile const void *GPDMA_LUTPerAddr[];
extern const LPC_GPDMACH_TypeDef *pGPDMACh[8];
extern const uint8_t GPDMA_LUTPerBurst[];
extern const uint8_t GPDMA_LUTPerWid[];

extern OS_EVENT *QSem;//
extern OS_MEM   *PartitionPt;
extern uint8_t  Partition[MSG_QUEUE_TABNUM][100];

/// If key pressed , isKeyTrigged will be TRUE. Your apps must set iskeyTrigged FALSe after using it.
extern int isKeyTrigged;    


extern unsigned char isDstSetChanged;             
/*-------------------------- Local Variables --------------------------------------*/
/*-------------------------- Global Variables -------------------------------------*/
volatile int xlCnt  = 0;
// volatile Bool Doubleclick  = FALSE;
uint8_t DMADest_Buffer[DMA_SIZE]; 
GPDMA_Channel_CFG_Type GPDMACfg_SPI2;
GPDMA_Channel_CFG_Type GPDMACfg_UART2;

volatile int myCnt = 0;
uint8_t UART2_RX[18];//

void DMA_IRQHandler (void)
{
	uint8_t index;	

	if (GPDMA_IntGetStatus(GPDMA_STAT_INT, 0))
	{
    GPDMA_ChannelCmd(0, DISABLE);  
    
    if(GPDMA_IntGetStatus(GPDMA_STAT_INTTC, 0))/* 检查DMA通道1终端计数请求状态，读取DMACIntTCStatus寄存器来判断中断是否因为传输的结束而产生（终端计数） */ 		
    {

       if(DMADest_Buffer[0] == '!'  ||  DMADest_Buffer[0] == '$')
       {
          for(index=0;index<50;index++)	
          {
            Partition[myCnt][index]=DMADest_Buffer[index];
          }

          OSQPost(QSem,(void *)Partition[myCnt]); 
          myCnt++;
          myCnt  = myCnt%(MSG_QUEUE_TABNUM);
          
          LPC_GPDMACH0->CDestAddr = GPDMACfg_SPI2.DstMemAddr;// Assign memory destination address
          LPC_GPDMACH0->CControl= GPDMA_DMACCxControl_TransferSize((uint32_t)GPDMACfg_SPI2.TransferSize) \
                     | GPDMA_DMACCxControl_SBSize((uint32_t)GPDMA_LUTPerBurst[GPDMACfg_SPI2.SrcConn]) \
                     | GPDMA_DMACCxControl_DBSize((uint32_t)GPDMA_LUTPerBurst[GPDMACfg_SPI2.SrcConn]) \
                     | GPDMA_DMACCxControl_SWidth((uint32_t)GPDMA_LUTPerWid[GPDMACfg_SPI2.SrcConn]) \
                     | GPDMA_DMACCxControl_DWidth((uint32_t)GPDMA_LUTPerWid[GPDMACfg_SPI2.SrcConn]) \
                     | GPDMA_DMACCxControl_DI \
                     | GPDMA_DMACCxControl_I;  
          GPDMA_ClearIntPending (GPDMA_STATCLR_INTTC, 0);
          GPDMA_ChannelCmd(0, ENABLE);	                   
       } 
       else
       {

          GPDMA_ClearIntPending (GPDMA_STATCLR_INTERR, 0);
          SPI2_DMA_Init();
          PRINT("spi2 dma error");
       }          
    }
 }
}



void SPI2_DMA_Init(void)
{
	NVIC_DisableIRQ(DMA_IRQn);		/* 关闭GPDMA中断 */
	NVIC_SetPriority(DMA_IRQn, ((0x01<<3)|0x01));/* 设置抢占优先级preemption为1, 子优先级sub-priority为1 */
	//GPDMA_Init();	/* 初始化GPDMA控制器，打开DMA时钟电源，复位各通道寄存器，清除错误标志等 */
	GPDMACfg_SPI2.ChannelNum = 0;	/* 使用DMA通道0 */
	GPDMACfg_SPI2.SrcMemAddr =0;	/* 源地址 */
	GPDMACfg_SPI2.DstMemAddr = (uint32_t)DMADest_Buffer;	/* 目的地址 */
	GPDMACfg_SPI2.TransferSize = sizeof(DMADest_Buffer);/* 传输大小 */
	//GPDMACfg.TransferWidth =GPDMA_WIDTH_WORD;	/* 传输宽度 */
	GPDMACfg_SPI2.TransferType = GPDMA_TRANSFERTYPE_P2M;	/* 设置传输类型为外设到存储器 */
	GPDMACfg_SPI2.SrcConn = GPDMA_CONN_SSP2_Rx;	/* 设置DMA源请求连接*/
	GPDMACfg_SPI2.DstConn = 0;	/* 设置DMA目的请求连接，未使用*/
	GPDMACfg_SPI2.DMALLI = 0;	/* 设置DMA通道链表项，未使用 */

	GPDMA_Setup(&GPDMACfg_SPI2);/* 把上面的参数设置进DMA相关寄存器 */
	//LPC_SSP2->DMACR |=0x11;
	SSP_DMACmd (LPC_SSP2, SSP_DMA_RX, ENABLE);
	NVIC_EnableIRQ(DMA_IRQn);
	GPDMA_ChannelCmd(0, ENABLE);
}

void UART2_DMA_Init()
{

   NVIC_DisableIRQ (DMA_IRQn);
   NVIC_SetPriority(DMA_IRQn, ((0x01<<3)|0x01));
   GPDMACfg_UART2.ChannelNum = 1;
   GPDMACfg_UART2.SrcMemAddr = 0;// Source memory - don't care
   GPDMACfg_UART2.DstMemAddr = (uint32_t) &UART2_RX;// Destination memory
   GPDMACfg_UART2.TransferSize = 18;// Transfer size
  //	GPDMACfg1.TransferWidth = 0;// Transfer width - don't care
   GPDMACfg_UART2.TransferType = GPDMA_TRANSFERTYPE_P2M;// Transfer type
   GPDMACfg_UART2.SrcConn = GPDMA_CONN_UART2_Rx;// Source connection
   GPDMACfg_UART2.DstConn = 0;// Destination connection - don't care
   GPDMACfg_UART2.DMALLI = 0;// Linker List Item - unused
   GPDMA_Setup(&GPDMACfg_UART2);
   NVIC_EnableIRQ (DMA_IRQn);
   GPDMA_ChannelCmd(1, ENABLE);
}

