#include "wwdt.h"
//#include "uart.h"
#include "lpc177x_8x_wwdt.h"


void LPC1788_WWDT_Init(int WDT_TimeOut)//WDT_TimeOut 单位us
{
  WWDT_Init(WDT_TimeOut);//4000000us=4s
	 WWDT_Enable(ENABLE);//使能看门狗
	 WWDT_SetMode(WWDT_RESET_MODE,ENABLE);//看门狗超时导致芯片复位
 	WWDT_Start(WDT_TimeOut);//喂一次狗促使狗工作
	
}











