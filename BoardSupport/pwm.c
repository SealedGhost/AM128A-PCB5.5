#include "lpc177x_8x_pwm.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_adc.h"
#include "lpc177x_8x_pinsel.h"
#include "pwm.h"

#define _BACK_LIGHT_BASE_CLK (1000/4)
	uint32_t pclk;
 
 
void lpc1788_PWM_Init(void)//PWM��ʼ������
{
//	uint32_t pclk;
  PWM_TIMERCFG_Type PWMCfgDat;
  PWM_MATCHCFG_Type PWMMatchCfgDat;
	
  PWMCfgDat.PrescaleOption = PWM_TIMER_PRESCALE_TICKVAL;/* ֱ��ʹ�÷�Ƶֵ(���⻹����ʹ��΢��ֵ) */
  PWMCfgDat.PrescaleValue = 1;  //Ƶ��=fpclk
  PWM_Init(1, PWM_MODE_TIMER, (void *) &PWMCfgDat);/* PWM1��ʹ�ö�ʱģʽ�����⻹����ʹ�ü���ģʽ�� */		
	PINSEL_ConfigPin(2,1,1);/*����PWM���� P2.1: PWM1_2*/
	PWM_ChannelConfig(1, 2, PWM_CHANNEL_SINGLE_EDGE);	/* ����PWM1ͨ��2����������ؿ���ģʽ */

  pclk = CLKPWR_GetCLK(CLKPWR_CLKTYPE_PER);	/* ����PWM1ͨ��0���������� */
  PWM_MatchUpdate(1, 0, pclk/_BACK_LIGHT_BASE_CLK, PWM_MATCH_UPDATE_NOW);
  PWMMatchCfgDat.IntOnMatch = DISABLE;
  PWMMatchCfgDat.MatchChannel = 0;
  PWMMatchCfgDat.ResetOnMatch = ENABLE;
  PWMMatchCfgDat.StopOnMatch = DISABLE;
  PWM_ConfigMatch(1, &PWMMatchCfgDat);

  PWM_MatchUpdate(1, 2, (pclk/_BACK_LIGHT_BASE_CLK)/2, PWM_MATCH_UPDATE_NOW);	/* ����PWM1ͨ��2������ռ�ձ�, ������Ϊ50% */
  PWMMatchCfgDat.IntOnMatch = DISABLE;
  PWMMatchCfgDat.MatchChannel = 2;
  PWMMatchCfgDat.ResetOnMatch = DISABLE;
  PWMMatchCfgDat.StopOnMatch = DISABLE;
  PWM_ConfigMatch(1, &PWMMatchCfgDat);

  PWM_ChannelCmd(1, 2, ENABLE);  /* ʹ��PWM1ͨ��2��� */
  PWM_ResetCounter(1);/* ��λPWM1������ */
  PWM_CounterCmd(1, ENABLE);/* PWM1����ʹ�� */
  PWM_Cmd(1, ENABLE);  /* PWMģʽʹ�� */
}
void SetBackLight(uint32_t level)//����PWMռ�ձȣ�����LCD��������
{ 
  PWM_MATCHCFG_Type PWMMatchCfgDat;
  PWM_MatchUpdate(1, 2, level, PWM_MATCH_UPDATE_NOW);	/* ����PWM1ͨ��2������ռ�ձ� */
  PWMMatchCfgDat.IntOnMatch = DISABLE;
  PWMMatchCfgDat.MatchChannel = 2;
  PWMMatchCfgDat.ResetOnMatch = DISABLE;
  PWMMatchCfgDat.StopOnMatch = DISABLE;
  PWM_ConfigMatch(1, &PWMMatchCfgDat);
}

void PWM_SET(uint8_t PWM)//PWM_test
{
	  uint32_t BACK_LIGHT;
	  BACK_LIGHT=pclk/_BACK_LIGHT_BASE_CLK/500;
//    printf("\r\nBACK_LIGHT:%d\n\r",PWM);
		SetBackLight(BACK_LIGHT*PWM);
  switch(PWM)
  {
     case 1:
          SetBackLight(BACK_LIGHT*1);
          break;
     case 2:
          SetBackLight(BACK_LIGHT*5);
          break;
     case 3:
          SetBackLight(BACK_LIGHT*30);
          break;
     case 4:
          SetBackLight(BACK_LIGHT*100);
          break;
     case 5:
          SetBackLight(BACK_LIGHT*150);
          break;
     case 6:
          SetBackLight(BACK_LIGHT*300);
          break;
  }
}



/************************************* End *************************************/
