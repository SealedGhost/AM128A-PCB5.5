
#include "Check.h"
#include <math.h>
#include "sound.h"
#include "invader.h"
#include <ucos_ii.h>
#include <string.h>
#include "bully.h"


/*----------------- Macro      defines --------------------------*/
#define MYABS(x)   ((x)>0?(x):(-(x)))

/*----------------- external variables ------------------------*/
extern BERTH Berthes[BOAT_NUM_MAX];
extern SIMP_BERTH SimpBerthes[BOAT_NUM_MAX];
extern int N_boat;
extern MNT_BERTH * pMntHeader;
extern MNT_BERTH MNT_Berthes[MNT_NUM_MAX];

extern FunctionalState isMntEnable;

/// Defined in app.c.If key pressed , isKeyTrigged will be TRUE. Your apps must set iskeyTrigged FALSe after using it.
extern int isKeyTrigged;
 
/*----------------- global   variables ------------------------*/
//INVADER Invaders[INVD_NUM_MAX];
//INVADER * pInvdHeader  = NULL;
//INVADER * pInvdTail    = NULL; 
//INVADER InvdMaskCursor = {0};


/*-------------------- Local variables ------------------------*/
//static FunctionalState isCheckEnable  = ENABLE;

MNT_BERTH NULL_Berth  = {0};

/*--------------------- Local functions -----------------------*/
static void MNT_filter(void);
static void CHECK_huntNew(void);
static Bool CHECK_isStillDsp(MNT_BERTH* pMntBerth);
//static Bool CHECK_isDsp(MNT_BERTH * pMntBerth);
static Bool CHECK_isDrg(MNT_BERTH * pMntBerth);
static Bool CHECK_isBgl(MNT_BERTH * pMntBerth);
static void CHECK_checkName(MNT_BERTH* pMntBerth);
void CHECK_check(MNT_BERTH * pMntBerth);


void check()
{
   static int Cnt  = 0;
   static int CheckNum  = 0;   
   
   int i  = 0;

   MNT_BERTH * pMntBerth  = NULL;
   MNT_BERTH * pIterator  = NULL;
    
   /// Delete all MNTState_Delete .
   MNT_filter();
//   CHECK_huntNew();
   if(pMntHeader == NULL)
   {
      return;
   }    
  
   if(isKeyTrigged)
   { 
      isKeyTrigged   = 0;
     
      /// 将所有触发态掩盖
      
      pIterator  = pMntHeader;
      while(pIterator)
      {
         if( (pIterator->trgState & 0x0f) == MNTState_Triggered )
         {
            pIterator->trgState  =  (pIterator->trgState & 0xf0) | MNTState_Masked;
         }
         
         pIterator  = pIterator->pNext;
      } 


       INVD_updataMask();     
    }
  
  
  
/// 找寻本次应该check哪条监控船舶 

   pMntBerth  = pMntHeader;
   
   if(Cnt==0)
   {   
      for(i=CheckNum;i>0;i--)
      {
         if(pMntBerth->pNext)
         {
            pMntBerth  = pMntBerth->pNext;
         }
         else
         {
            pMntBerth  = pMntHeader;
            CheckNum  = 0;
         }
      }
      CheckNum++;
    
//INFO("Check %09ld",pMntBerth->mntBoat.mmsi);     
     CHECK_check(pMntBerth); 
     
     if(pMntBerth->mntBoat.name[0] == 0)
        CHECK_checkName(pMntBerth);
       
     if(pMntBerth->nickName[0] == 0)
        CHECK_checkNickName(pMntBerth);
   
   }
      
   Cnt++;
   Cnt  = Cnt%2;
}



void CHECK_check(MNT_BERTH * pMntBerth)
{
   unsigned char trgState  = 0;
   

//   if(CHECK_isDsp(pMntBerth))
//   {
//      trgState  = 0x01<<7;
//   }
   if(CHECK_isStillDsp(pMntBerth))
   {
      trgState  = 0x01<<7;
   }
   else if(CHECK_isDrg(pMntBerth))
   {
      INVD_clear(pMntBerth - MNT_Berthes);
      trgState  = 0x01<<6;
      if(pMntBerth->pBerth->Boat.latitude == 0  ||  pMntBerth->pBerth->Boat.longitude == 0)
      {
INFO("ERR");        
      }
   }
   else if(CHECK_isBgl(pMntBerth))
   {
      trgState  = 0x01<<5;
      if(pMntBerth->pBerth->Boat.latitude == 0  ||  pMntBerth->pBerth->Boat.longitude == 0)
      {
INFO("ERR");
      }
   }
   
   
   if(trgState == 0)
   {
      pMntBerth->trgState  = MNTState_Monitored;
      return;
   }
   else if( (trgState ^ pMntBerth->trgState) & trgState )
   {
      pMntBerth->trgState  = (trgState & 0xf0) | MNTState_Triggered;
   }  
}


static Bool CHECK_isStillDsp(MNT_BERTH* pMntBerth)
{
   if(pMntBerth->pBerth  &&  pMntBerth->pBerth->Boat.user_id != pMntBerth->mntBoat.mmsi)
   {
INFO("Berth err:%09ld--%09ld", pMntBerth->mntBoat.mmsi,pMntBerth->pBerth->Boat.user_id);     
      pMntBerth->pBerth  = NULL;
      INVD_clear(pMntBerth-MNT_Berthes);
   }
   
   if(pMntBerth->pBerth == NULL )
   {
      int i;
      for(i=0; i<N_boat; i++)
      {
         if(SimpBerthes[i].pBerth->Boat.user_id == pMntBerth->mntBoat.mmsi)
         {
            pMntBerth->pBerth  = SimpBerthes[i].pBerth;
            SimpBerthes[i].pBerth->mntState  = MNTState_Monitored;
            if(SimpBerthes[i].pBerth->isInvader)
            {
               INVD_deleteByAddr(SimpBerthes[i].pBerth);
            }

//printf("find %09ld at %d--%09ld\n",pMntBerth->mntBoat.mmsi ,SimpBerthes[i].pBerth-Berthes, SimpBerthes[i].pBerth->Boat.user_id);            
            break;
         }
      }
   }
   
   
   if(pMntBerth->pBerth != NULL)
   {
//      if(pMntBerth->pBerth->Boat.dist >= 99999)
//      if(pMntBerth->pBerth->Boat.user_id != pMntBerth->mntBoat.mmsi)
//INFO("GG");
      
      if(pMntBerth->pBerth->Boat.latitude == 0  ||  pMntBerth->pBerth->Boat.longitude == 0)
         pMntBerth->cfgState  = MNTState_Pending;
      else 
      {
         if(pMntBerth->cfgState != MNTState_Monitored)
         {
            pMntBerth->mntBoat.lg  = pMntBerth->pBerth->Boat.longitude;
            pMntBerth->mntBoat.lt  = pMntBerth->pBerth->Boat.latitude;
         }
         pMntBerth->cfgState  = MNTState_Monitored;
         
      }
   }
   else if(pMntBerth->cfgState != MNTState_Init)
   {
      return TRUE;
   }
   
   return FALSE;
}



static Bool CHECK_isDrg(MNT_BERTH * pMntBerth)
{
   long diff_lon;
   long diff_lat;
   float r  = 0.0;
  
   if(pMntBerth->cfgState != MNTState_Monitored  ||  pMntBerth->trgState == MNTState_Pending  ||  pMntBerth->mntBoat.mntSetting.DRG_Setting.isEnable == FALSE)
   {
      return FALSE;
   }  

   diff_lon  = pMntBerth->pBerth->Boat.longitude - pMntBerth->mntBoat.lg;
   diff_lat  = pMntBerth->pBerth->Boat.latitude  - pMntBerth->mntBoat.lt;
   
   diff_lon  = MYABS(diff_lon);
   diff_lat  = MYABS(diff_lat);
   
   ///  先判断是否在走锚报警区域的内切正方形内，若是就不必勾股了，不是的话再老老实实sqrt吧。
   if(  diff_lon + 2*diff_lon/5 > pMntBerth->mntBoat.mntSetting.DRG_Setting.Dist
      ||diff_lat + 2*diff_lat/5 > pMntBerth->mntBoat.mntSetting.DRG_Setting.Dist)
   {
      r  = sqrt(diff_lon*diff_lon + diff_lat*diff_lat);
   }
   
   if(r > pMntBerth->mntBoat.mntSetting.DRG_Setting.Dist)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}



static Bool CHECK_isBgl(MNT_BERTH * pMntBerth)
{
   int i ; 

   if(pMntBerth->cfgState != MNTState_Monitored  ||  pMntBerth->trgState == MNTState_Pending  ||  pMntBerth->mntBoat.mntSetting.BGL_Setting.isEnable == DISABLE)
   { 
      return FALSE;
   } 

   for(i=N_boat-1; i>=0; i--)
   {
      if(SimpBerthes[i].pBerth->mntState == MNTState_None)
      {
         /// Is invader
         if(  (MYABS(SimpBerthes[i].longitude - pMntBerth->pBerth->Boat.longitude) <= pMntBerth->mntBoat.mntSetting.BGL_Setting.Dist)
            &&(MYABS(SimpBerthes[i].latitude  - pMntBerth->pBerth->Boat.latitude)  <= pMntBerth->mntBoat.mntSetting.BGL_Setting.Dist))
         {       
            if(INVD_add(pMntBerth-MNT_Berthes, SimpBerthes[i].pBerth))
               SimpBerthes[i].pBerth->isInvader  = pMntBerth->mntBoat.mmsi;        
         }
         
         /// Is not invader but has been.So remove it from black list.
         else 
         {   
//printf("black->white\n");         
            INVD_delete(pMntBerth-MNT_Berthes, SimpBerthes[i].pBerth);  
//            if(SimpBerthes[i].pBerth->isInvader == pMntBerth->mntBoat.mmsi)
//               SimpBerthes[i].pBerth->isInvader  = 0;            
         }
      }
   }

//   if(INVD_isAllMasked(pMntBerth-MNT_Berthes) == FALSE)
   if(INVD_isAlone(pMntBerth-MNT_Berthes) == FALSE)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}




/**@brief MNT_filter
 *
 * @dscp Delete mntBoat whose state is MNTState_Delete;
 *       Find all disappear mntBoat.
 *
 */
static void MNT_filter()
{
   MNT_BERTH * pIterator  = NULL;
   MNT_BERTH * pBC        = NULL;

   
   while(pMntHeader && pMntHeader->chsState == MNTState_Cancel)
   {   
      pBC  = pMntHeader;
      pMntHeader  = pMntHeader->pNext;
      
      INVD_clear(pBC-MNT_Berthes);     
    
      EEPROM_Write(0 , MNT_PAGE_ID+pBC-MNT_Berthes,
                  &(NULL_Berth.mntBoat), MODE_8_BIT, sizeof(MNT_BOAT));
      if(pBC->pBerth && pBC->pBerth->Boat.user_id == pBC->mntBoat.mmsi)
      {
         pBC->pBerth->mntState  = MNTState_None;           
      }
      memset((void*)pBC, 0, sizeof(MNT_BERTH));    
   }
  
   
   if(pMntHeader == NULL)
   {
      return;
   }

   pBC        = pMntHeader;
   pIterator  = pMntHeader->pNext; 
   while(pIterator)
   {
      if(pIterator->chsState == MNTState_Cancel)
      {    
         pBC->pNext  = pIterator->pNext;
//printf("mnt delete claer\n");
         INVD_clear(pIterator-MNT_Berthes); 
        
         EEPROM_Write(0 , MNT_PAGE_ID+pIterator-MNT_Berthes,
                      &(NULL_Berth.mntBoat), MODE_8_BIT, sizeof(MNT_BOAT));     
         if(pIterator->pBerth && pIterator->pBerth->Boat.user_id == pIterator->mntBoat.mmsi)
            pIterator->pBerth->mntState  = MNTState_None;      
         memset((void*)pIterator, 0, sizeof(MNT_BERTH));
      }
      else
      {              
         pBC  = pIterator;        
      }
      
      pIterator  = pBC->pNext;
   } 
  
}


static void CHECK_huntNew()
{
   int i  = 0;
   for(i=0; i<N_boat; i++)
   {
      if(SimpBerthes[i].pBerth->mntState == MNTState_Add)
      {
      
         if(MNT_add(SimpBerthes[i].pBerth))
         {
            SimpBerthes[i].pBerth->mntState  = MNTState_Monitored;
         }
         else
         {
            SimpBerthes[i].pBerth->mntState  = MNTState_None;
         }
      }
   }
}


static void CHECK_checkName(MNT_BERTH* pMntBerth)
{
   if(pMntBerth && pMntBerth->mntBoat.name[0] == 0)
   {
      if(pMntBerth->pBerth  &&  pMntBerth->pBerth->Boat.name[0] != 0)
      {
         int i  = 0;
         for(; i<19; i++)
         {
            pMntBerth->mntBoat.name[i]  = pMntBerth->pBerth->Boat.name[i];
            if(pMntBerth->mntBoat.name[i] == '\0')
               return ;
         }
         pMntBerth->mntBoat.name[19]  = 0;
         MNT_storeBoatInfo(pMntBerth);
      }
   }
}


void CHECK_checkNickName(MNT_BERTH * pMntBerth)
{
   int i  = 0;
   
   if(pMntBerth && pMntBerth->mntBoat.name[0])
   {
      for(; i<20; i++)
      {
         if(pMntBerth->mntBoat.name[i] == '\0')
         {
            break;
         }
      }
      
      i--;
      if(   (pMntBerth->mntBoat.name[i] >= '0' && pMntBerth->mntBoat.name[i] <= '9')
         || (pMntBerth->mntBoat.name[i] >= 'A' && pMntBerth->mntBoat.name[i] <= 'Z')  )
      {
         pMntBerth->nickName[1]  = pMntBerth->mntBoat.name[i] ;
         if(i)
         {
            i--;
            if(   (pMntBerth->mntBoat.name[i] >= '0' && pMntBerth->mntBoat.name[i] <= '9')
               || (pMntBerth->mntBoat.name[i] >= 'A' && pMntBerth->mntBoat.name[i] <= 'Z')  )
            {
               pMntBerth->nickName[0]  = pMntBerth->mntBoat.name[i];   
            }
            else
            {
               pMntBerth->nickName[0]  = ' ';
            }
         }
         else
         {
            pMntBerth->nickName[0]  = ' ';
         }
      }
      else
      {
         pMntBerth->nickName[1]  = ' ';
      }
   }
}
