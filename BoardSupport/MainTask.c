#include "GUI.h"
#include "DIALOG.h"
#include "MainTask.h"
#include "exfuns.h"
#include "sysConf.h"
#include "Setting.h"
#include "logo.h"
#include "120.h"
#include "28.h"
#include "dlg.h"
#include "sound.h"
#include "Config.h"
#include "xt_isd.h"

extern unsigned char isSub0Inited;
extern unsigned char isSub2Inited;
extern unsigned char isChecked;



GUI_MEMDEV_Handle hMute;

void MainTask(void)
{
   GUI_MEMDEV_Handle hMem0;
  


   GUI_Init();	
   WM_SetCreateFlags(WM_CF_MEMDEV);
   //开机界面
   hMem0 = GUI_MEMDEV_Create(0,0,800,480);

   GUI_MEMDEV_Select(hMem0);
   GUI_SetBkColor (GUI_WHITE);
   GUI_Clear();
   
   /// Welcome logo 
   GUI_SetColor (0xb58400);
   GUI_SetFont (&GUI_Font120);
#ifdef P_AM128A
   GUI_DispStringAt("AM-128A", 195,100); 
PRINT("AM-128A");   
#else
   GUI_DispStringAt("AM-128B",195,100);
PRINT("AM-128B");   
PRINT()   
#endif   
   GUI_PNG_Draw (&_accover_colo_1,sizeof(_accover_colo_1),300,LCD_GetYSize()/2);  
   
   
   GUI_MEMDEV_Select(0);
   GUI_MEMDEV_CopyToLCD(hMem0);  
   GUI_MEMDEV_Delete(hMem0);
   
   
   GUI_Delay(500);

   GUI_Clear();
   //创建字体
   GUI_UC_SetEncodeUTF8();	
   //字体设置	//GUI_SetDefaultFont (&SIF_Font);
   GUI_SetDefaultFont (&GUI_Font30);	
   TEXT_SetDefaultFont(&GUI_Font30);
   BUTTON_SetDefaultFont (&GUI_Font30);
   HEADER_SetDefaultFont(&GUI_Font30);
   HEADER_SetDefaultBkColor(GUI_BLUE);
   //EDIT_SetDefaultFont(&GUI_Font30);
   WIDGET_SetDefaultEffect(&WIDGET_Effect_None);
   //创建窗口 
//   sldWinCreate();
//     _sldWinCreate();
//     WM_SetFocus(_sldWinCreate());
   confirmWin  = confirmWinCreate();
	 WM_ShowWindow (confirmWin);
   mntSettingWin  = mntSettingWinCreate(); 
//  _mntSettingWin  = _mntSettingWinCreate();
   subWins[0]  = sub0WinCreate(); 
   subWins[1]  = sub1WinCreate();
   subWins[2]  = sub2WinCreate();
   subWins[3]  = sub3WinCreate();
#ifdef P_AM128A
   subWins[4]  = sub4WinCreate();
#endif   
   menuWin  = menuWinCreate();
   mapWin = mapWinCreate();


   while(1)
   {
      GUI_Delay(200);
      if(isChecked && isSub0Inited && isSub2Inited)
      {
         WM_SendMessageNoPara(subWins[2],USER_MSG_LV_UPDATE);
         WM_SendMessageNoPara(subWins[0],USER_MSG_LV_UPDATE);
         isChecked  = 0;
      }
   }
}

/*************************** End of file ****************************/
