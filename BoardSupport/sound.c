#include "sound.h"
#include "uart.h"


uint8_t VOL[6][5]  = { {0x7e, 0x03, 0x31,  5, 0xef},
                       {0x7e, 0x03, 0x31, 10, 0xef},
                       {0x7e, 0x03, 0x31, 15, 0xef},                              
                       {0x7e, 0x03, 0x31, 20, 0xef},                              
                       {0x7e, 0x03, 0x31, 25, 0xef}, 
                       {0x7e, 0x03, 0x31, 30, 0xef}                   
                      };

void SND_SetVol(uint8_t SndVol)
{
   uint8_t vol  = 0;
   if(SndVol < SND_VOL_MIN)
      vol  = SND_VOL_MIN;
   else if(SndVol > SND_VOL_MAX)
      vol  = SND_VOL_MAX;
   else 
      vol  = SndVol;
      
//   vol  = vol;
//      
   UART_Send(UART_0, VOL[vol], 5, BLOCKING);
   
//   UART_SendByte(UART_0, 0x7e);
//   UART_SendByte(UART_0, 0x03);
//   UART_SendByte(UART_0, 0x31);
//   UART_SendByte(UART_0, vol);
//   UART_SendByte(UART_0, 0x7e);
}


//void SND_SetVol(uint8_t SndVol)
//{

//   uint8_t vol  = 0;
//   if(SndVol < SND_VOL_MIN)
//      vol  = SND_VOL_MIN;
//   else if(SndVol > SND_VOL_MAX)
//      vol  = SND_VOL_MAX;
//   else 
//      vol  = SndVol;
//   vol  = vol *5;    
//#ifdef SND_IC_XT
//   UART_SendByte(UART_0, 0x7e);
//   UART_SendByte(UART_0, 0xff);
//   UART_SendByte(UART_0, 0x06);
//   UART_SendByte(UART_0, 0x06);
//   UART_SendByte(UART_0, 0x00);
//   UART_SendByte(UART_0, 0x00);
//   UART_SendByte(UART_0, vol);
//   UART_SendByte(UART_0, 0xfe);
//   UART_SendByte(UART_0, 0xf5-vol);
//   UART_SendByte(UART_0, 0xef);
//#else
//   UART_SendByte(UART_0, 0x7e);
//   UART_SendByte(UART_0, 0x03);
//   UART_SendByte(UART_0, 0x31);
//   UART_SendByte(UART_0, vol);
//   UART_SendByte(UART_0, 0x7e);
//#endif   
//}


void SND_Stop()
{
   UART_SendByte(UART_0, 0x7e);
   UART_SendByte(UART_0, 0x02);
   UART_SendByte(UART_0, 0x0e);
   UART_SendByte(UART_0, 0xef);
}

//void SND_Play()
//{
//   UART_SendByte(UART_0, 0x7e);
//   UART_SendByte(UART_0, 0x01);
//   UART_SendByte(UART_0, 0x0e);
//   UART_SendByte(UART_0, 0xef);
//}
void SND_Init()
{
;  UART_SendByte(UART_0, 0x7e);
   UART_SendByte(UART_0, 0x03);
   UART_SendByte(UART_0, 0x33);
   UART_SendByte(UART_0, 0x04);
   UART_SendByte(UART_0, 0xef);
}

void SND_Play(uint8_t id)
{
#ifdef SND_IC_XT
   if(id <= SND_ID_MAX)
   {
      if(id == 0)
      {
         id  = SND_ID_ZRO;
      }
      UART_SendByte(UART_0, 0x7e);
      UART_SendByte(UART_0, 0xff);
      UART_SendByte(UART_0, 0x06);
      UART_SendByte(UART_0, 0x03);
      UART_SendByte(UART_0, 0x00);
      UART_SendByte(UART_0, 0x00);
      UART_SendByte(UART_0, id);
      UART_SendByte(UART_0, 0xfe);
      UART_SendByte(UART_0, 0xf8-id);
      UART_SendByte(UART_0, 0xef);
   }

#else
   if(id<=SND_ID_MAX)
   {
      if(id == 0)
      {
         id  = SND_ID_ZRO;
      }
      UART_SendByte(UART_0,0x7e);
      UART_SendByte(UART_0,0x04);
      UART_SendByte(UART_0,0x41);
      UART_SendByte(UART_0,0x00);
      UART_SendByte(UART_0, id);
      UART_SendByte(UART_0,0xef);     
   }
   
#endif   
}


void SND_ParseDist(int dist, uint8_t* pNums)
{
   if(dist < 99999)
   {
      /// Dist >= 20 nm
      if(dist > 19999)
      {
//         if(dist >= 21000) /// Dist >= 21 nm  :  Nx nm (eg. 2)
//         {
//            pNums[0]  = dist/10000;
//            pNums[1]  = SND_ID_TEN;
//            pNums[2]  = dist%10000/1000;            
//         }
//         /// 
//         else
//         {
//            pNums[0]  = 0;
//            pNums[1]  = dist/10000;
//            pNums[2]  = SND_ID_TEN;
//         }
           pNums[0]  = dist /10000;
           pNums[1]  = SND_ID_TEN;
           pNums[2]  = (dist %10000) /1000;
      }
      /// Dist >= 10 nm
      else if(dist > 9999)
      {
//         if(dist >=11000)
//         {
//            pNums[0]  = 0;
//            pNums[1]  = SND_ID_TEN;
//            pNums[2]  = dist%10000/1000;
//         }
//         else
//         {
//            pNums[0]  = 0;
//            pNums[1]  = 0;
//            pNums[2]  = SND_ID_TEN;
//         }
         pNums[0]  = SND_ID_TEN;
         pNums[1]  = (dist %10000) /1000;
         pNums[2]  = 0;
      }
      
      /// 0.1nm <= Dist < 10nm 
      else if(dist > 99)
      {
         /// Dist >= 1nm   :  1.x nm
         if(dist >999)
           pNums[0]  = dist /1000;
         /// Dist < 1nm    :  0.x nm
         else
           pNums[0]  = SND_ID_ZRO;
         pNums[1]  = SND_ID_DOT;
         pNums[2]  = dist%1000/100;
         if(pNums[2] == 0)
            pNums[2]  = SND_ID_ZRO;
      }
      else
      {
         pNums[0]  = 0;
         pNums[1]  = 0;
         pNums[2]  = SND_ID_ZRO;
      } 

//printf(" %d %d %d\n\r", pNums[0], pNums[1], pNums[2]);      
   }
}















