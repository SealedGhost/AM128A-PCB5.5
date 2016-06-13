#include "sysConf.h"
#include "Config.h"
#include "Setting.h"
#include "string.h"
#include "lpc177x_8x_eeprom.h"
#include "pwm.h"
#include "sound.h"
#include "Check.h"


/*--------------------- External variables ------------------------*/
extern MNT_BERTH MNT_Berthes[MNT_NUM_MAX];


unsigned long SYS_Date;
unsigned long SYS_Time;
///                                        



CONF_SYS SysConf;

const int SYSCONF_SIZE = sizeof(SysConf);


void printSysConf(CONF_SYS * p)
{
   PRINT(" ");
   PRINT("Skin:           %s-%d\n",p->Skin?"Night":"Day",p->Skin);
   PRINT("Snd -- Vol      %d",p->Vol);
   PRINT("Brightness      %d",p->Brt);
   PRINT("Unit            %s-%d",p->Unit?"nm":"km",p->Unit);
   PRINT("Shape           %s-%d",p->Shape?"Fish":"Boat",p->Shape);
#ifdef P_AM128A   
   PRINT("Nations         0x%x\n\r", p->nations);
#endif   
}


static Bool checkSysConf()
{
   Bool flag  = TRUE;
   if(SysConf.Skin != SKIN_Day  &&  SysConf.Skin != SKIN_Night) 
   {
      flag  = FALSE;
      PRINT("Skin load error! load %d as skin",SysConf.Skin);
      SysConf.Skin  = DEFAULT_SKIN;
   }
   if(SysConf.Brt < 1  ||  SysConf.Brt > 6)                     
   {
      flag  = FALSE;   
      PRINT("Brt  load error! load %d as brg",SysConf.Brt);
      SysConf.Brt  = DEFAULT_BRT;
   }
   if( SysConf.Vol > 6)            
   {
      flag  = FALSE;   
      PRINT("Vol  load error! load %d as vol",SysConf.Vol);
      SysConf.Vol  = DEFAULT_VOL;
   }

   if(SysConf.Unit != UNIT_km  &&  SysConf.Unit != UNIT_nm)     
   {
      flag  = FALSE;   
      PRINT("Unit load error! load %d as unit",SysConf.Unit);
      SysConf.Unit  = DEFAULT_UNIT;
   }
   if(SysConf.Shape != SHAPE_Boat  &&  SysConf.Shape != SHAPE_Fish) 
   {
      flag  = FALSE;   
      PRINT("Shape load error! load %d as shape",SysConf.Shape);
      SysConf.Shape  = DEFAULT_SHAPE;
   }
   return flag;
}



Bool sysLoad()
{  
   uint16_t i  = 0; 
   uint16_t j  = 0;
   uint16_t cnt  = 0;   
   Bool flag  = TRUE;
   
   
   PRINT("\n\rLoading..."); 
   
   EEPROM_Read(SYSCONF_ADDR%EEPROM_PAGE_SIZE, SYSCONF_ADDR/EEPROM_PAGE_SIZE,
               &SysConf, MODE_8_BIT, sizeof(CONF_SYS));
   /// Check if system config is right or not.
   printSysConf(&SysConf);
   if(!checkSysConf())
   {
      sysStore();
PRINT("after fix:\n\r");   
      printSysConf(&SysConf);
      flag  = FALSE;
   }
         

   for(i=0; i<MNT_NUM_MAX; i++)
   {
      EEPROM_Read(0, MNT_PAGE_ID+i, &MNT_Berthes[i], MODE_8_BIT, sizeof(MNT_BOAT));
      if(MNT_Berthes[i].mntBoat.mmsi)
      {
         if(MNT_Berthes[i].mntBoat.name[0] != 0)
            CHECK_checkNickName(&MNT_Berthes[i]);
            
         MNT_Berthes[i].mntBoat.lg  = 0;
         MNT_Berthes[i].mntBoat.lt  = 0;
         MNT_Berthes[i].chsState  = MNTState_Monitored;
         MNT_Berthes[i].trgState  = MNTState_Monitored;
         MNT_Berthes[i].cfgState  = MNTState_Init;
PRINT("%d--MMSI:%09ld\n\r",i,MNT_Berthes[i].mntBoat.mmsi);      
         cnt++;
      }
   }
   
   if(cnt)
   {
      i  = 0;
      while(MNT_Berthes[i].mntBoat.mmsi == 0) i++;
      
      pMntHeader  = &MNT_Berthes[i];
      
      for(j=i+1; j<MNT_NUM_MAX;)
      {
         if(MNT_Berthes[j].mntBoat.mmsi)
         {
            MNT_Berthes[i].pNext  = &MNT_Berthes[j];
            i  = j;
            j  = i+1;
         }
         else
         {
            j++;
         }
      }
   }
   
   return flag;
}


void sysStore()
{
   EEPROM_Write(SYSCONF_ADDR%EEPROM_PAGE_SIZE, SYSCONF_ADDR/EEPROM_PAGE_SIZE,
               &SysConf, MODE_8_BIT, sizeof(CONF_SYS));        
   printSysConf(&SysConf);               
}


  void sysInit()
{        
   if(sysLoad())
   {
INFO("System load successfully!");   
   }
   else
   {
INFO("Error happened when system load.System will be configed with default value");   
   }

   PWM_SET(SysConf.Brt); 
//   SND_Init();
}


void sysRevive(void)
{
   int i  = 0;
   
   SysConf.Skin  = DEFAULT_SKIN;
   SysConf.Brt   = DEFAULT_BRT;
   SysConf.Vol   = DEFAULT_VOL;
   SysConf.Unit  = DEFAULT_UNIT;
   SysConf.Shape = DEFAULT_SHAPE;
#ifdef P_AM128A   
   SysConf.nations = 0;
#endif   
   
   sysStore();
   for(i=0; i<MNT_NUM_MAX; i++)
   {
      EEPROM_Erase(MNT_PAGE_ID+i);
   }
INFO("after revive:");   
   printSysConf(&SysConf);
}


