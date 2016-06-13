#include "xt_isd.h"
#include "Config.h"
#include "SPI1.h"

static uint8_t ISD_buf[3];


/** @brief  ISD 上电并等待唤醒
 *
 *
 *
 */
void ISD_Wait_PWRUp(void)
{
   SPI1_SendData(0x10,ISD_buf, 0);
   while( (SPI1_ReadData() & 0x6000) == 0);
}


/** @brief ISD 掉电
 *
 *
 *
 */
void ISD_PWRDn(void)
{
   SPI1_SendData(0x12, ISD_buf, 0);
}




void ISD_Init(void)
{

   uint32_t byte  = 0;

   /// Wake up ISD  
   ISD_Wait_PWRUp();
   
   SPI1_CS_HIGH();
   SPI1_CS_LOW();
   
   byte  = SPI1_SendByte(0x48);
   byte  = byte << 8;
   
   byte  |= SPI1_SendByte(0x00);
   byte  = byte << 8;
   
   byte  |= SPI1_SendByte(0x00);
   byte  = byte << 8;
   
   byte  |= SPI1_SendByte(0x00);
   byte  = byte << 8;
   
   byte  |= SPI1_SendByte(0x00);
   byte  = byte << 8;
   
   SPI1_CS_HIGH();
   
//INFO("ISD id:%x", byte);  
//INFO("ISD init done");

}


void ISD_SetVolumn(uint8_t vol)
{ 
    ISD_buf[0]  = 0x03;
    if(vol == 6){
       ISD_buf[1]  = 0x00;
    }
    else if(vol < 6){
       ISD_buf[1]  = (6-vol) *0x10 -1;
    }
    
//INFO("set vol:%d--send is:0x%x", vol, ISD_buf[1]);      
    SPI1_SendData(0xb8, ISD_buf, 2);
}



void ISD_Play(uint8_t addr)
{
   ISD_buf[0]  = 0x00;
   ISD_buf[1]  = addr + 2 ;
   SPI1_SendData(0xa6, ISD_buf, 2);  
}



Bool ISD_IsBusy(void)
{
   if(SPI1_ReadData() & 0x0200)  
      return TRUE;
   else 
      return FALSE;
}

