#ifndef _SOUND_H
#define _SOUND_H
#include "stdint.h"
#include "lpc177x_8x_uart.h"
#include "Config.h"

#ifdef P_AM128A
#define SND_ID_MAX 29
#else 
#define SND_ID_MAX 20 
#endif 
 
#define SND_ID_MIN  1

#define SND_VOL_MAX   6
#define SND_VOL_MIN   1



#define SND_ID_ZRO   10
#define SND_ID_DOT   11
#define SND_ID_TEN   12
#define SND_ID_WLCM  13
#define SND_ID_DEV   14
#define SND_ID_DSP   15
#define SND_ID_DRG   16
#define SND_ID_BGL   17
#define SND_ID_DST   18
#define SND_ID_NM    19
#define SND_ID_TEST  20

#ifdef P_AM128A
#define SND_ID_HSB   21 /// High speed boat
#define SND_ID_SIS   22 /// Speed is

#define SND_ID_CTB   23
#define SND_ID_JPN   24
#define SND_ID_KOR   25
#define SND_ID_PRK   26
#define SND_ID_INA   27
#define SND_ID_VIE   28

#define SND_ID_KT    29
#endif


#define SND_ID_NUM_BASE 0x05

//void SND_SelectID(uint8_t SndID);
void SND_Init(void);
void SND_SetVol(uint8_t SndVol);
void SND_Stop(void);
void SND_Play(uint8_t id);
void SND_ParseDist(int dist, uint8_t* pNums);
#endif

