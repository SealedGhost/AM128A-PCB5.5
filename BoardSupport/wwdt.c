#include "wwdt.h"
//#include "uart.h"
#include "lpc177x_8x_wwdt.h"


void LPC1788_WWDT_Init(int WDT_TimeOut)//WDT_TimeOut ��λus
{
  WWDT_Init(WDT_TimeOut);//4000000us=4s
	 WWDT_Enable(ENABLE);//ʹ�ܿ��Ź�
	 WWDT_SetMode(WWDT_RESET_MODE,ENABLE);//���Ź���ʱ����оƬ��λ
 	WWDT_Start(WDT_TimeOut);//ιһ�ι���ʹ������
	
}











