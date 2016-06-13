#include "DIALOG.h"
#include "Config.h"
#include "HSD_SLIDER.h"
#include "skinColor.h"
#include "SysConf.h"
#include "28.h"
#include "dlg.h"
#include "MainTask.h"


#define ID_WINDOW_0                (GUI_ID_USER + 0x01)

#define ID_SLIDER_CTB              (GUI_ID_USER + 0x10)
#define ID_SLIDER_JPN              (GUI_ID_USER + 0x11)
#define ID_SLIDER_KOR              (GUI_ID_USER + 0x12)
#define ID_SLIDER_PRK              (GUI_ID_USER + 0x13)
#define ID_SLIDER_INA              (GUI_ID_USER + 0x14)
#define ID_SLIDER_VIE              (GUI_ID_USER + 0x15)




#ifdef P_AM128A

static const SysWin_COLOR* pSkin  = &SysWinSkins[0];


static void sldListener(WM_MESSAGE* pMsg);

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]  =
{
   { WINDOW_CreateIndirect,      "window",   ID_WINDOW_0,       SubWin_X,      SubWin_Y,        SubWin_WIDTH,      SubWin_HEIGHT,   0,   0,   0},
   { HSD_SLIDER_CreateIndirect,  "sld0"  ,   ID_SLIDER_CTB,     180, Win_SysSet_txOrg,                                              120, 30, 0, 0, 0},
   { HSD_SLIDER_CreateIndirect,  "sld1"  ,   ID_SLIDER_JPN,     180, Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap),   120, 30, 0, 0, 0},
   { HSD_SLIDER_CreateIndirect,  "sld2"  ,   ID_SLIDER_KOR,     180, Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*2, 120, 30, 0, 0, 0},
   { HSD_SLIDER_CreateIndirect,  "sld3"  ,   ID_SLIDER_PRK,     180, Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*3, 120, 30, 0, 0, 0},
   { HSD_SLIDER_CreateIndirect,  "sld4"  ,   ID_SLIDER_INA,     180, Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*4 ,120, 30, 0, 0, 0},
   { HSD_SLIDER_CreateIndirect,  "sld5"  ,   ID_SLIDER_VIE,     180, Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*5, 120, 30, 0, 0, 0}
};


static WM_HWIN Slideres[6];

static unsigned char  agentNations;

static void _cbDialog(WM_MESSAGE* pMsg){
   int i  = 0;

   switch(pMsg->MsgId ){
      case USER_MSG_SKIN:
           pSkin  = &(SysWinSkins[pMsg->Data.v]);
           WINDOW_SetBkColor(pMsg->hWin, pSkin->bkColor);
           for(i=0; i<6; i++){
              HSD_SLIDER_SetBkColor(Slideres[i], pSkin->sldBk);
              HSD_SLIDER_SetFocusBkColor(Slideres[i], pSkin->sldBk);
              HSD_SLIDER_SetSlotColor(Slideres[i], pSkin->sldSlot);
              HSD_SLIDER_SetFocusSlotColor(Slideres[i], pSkin->sldSlot);
              HSD_SLIDER_SetSliderColor(Slideres[i], pSkin->sldSlider);
              HSD_SLIDER_SetFocusSliderColor(Slideres[i], pSkin->sldFocusSlider);
           }
           break;
      case USER_MSG_REPLY:
           if(pMsg->Data.v  == REPLY_OK){
              SysConf.nations  = agentNations;
              sysStore();
           }
           else{
              agentNations  = SysConf.nations;
              HSD_SLIDER_SetValue(Slideres[0], agentNations & 0x01);
              HSD_SLIDER_SetValue(Slideres[1], (agentNations>>1) & 0x01);
              HSD_SLIDER_SetValue(Slideres[2], (agentNations>>2) & 0x01);
              HSD_SLIDER_SetValue(Slideres[3], (agentNations>>3) & 0x01);
              HSD_SLIDER_SetValue(Slideres[4], (agentNations>>4) & 0x01);
              HSD_SLIDER_SetValue(Slideres[5], (agentNations>>5) & 0x01);
           }
           
           WM_SetFocus(Slideres[0]);
           WM_SetFocus(menuWin);
           break;
      
      case WM_INIT_DIALOG:
           agentNations  = SysConf.nations;
INFO("nations:0x%x",agentNations);
           pSkin  = &(SysWinSkins[SysConf.Skin]);
           
           Slideres[0]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_CTB);
           WM_SetCallback(Slideres[0], &sldListener);
           HSD_SLIDER_SetRange(Slideres[0], 0, 1);
           HSD_SLIDER_SetValue(Slideres[0], agentNations & 0x01);
           
           Slideres[1]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_JPN);
           WM_SetCallback(Slideres[1], &sldListener);
           HSD_SLIDER_SetRange(Slideres[1], 0, 1);                      
           HSD_SLIDER_SetValue(Slideres[1], (agentNations>>1) & 0x01);
           
           Slideres[2]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_KOR);
           WM_SetCallback(Slideres[2], &sldListener);
           HSD_SLIDER_SetRange(Slideres[2], 0, 1);                      
           HSD_SLIDER_SetValue(Slideres[2], (agentNations>>2) & 0x01);
           
           Slideres[3]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_PRK);
           WM_SetCallback(Slideres[3], &sldListener);
           HSD_SLIDER_SetRange(Slideres[3], 0, 1);                      
           HSD_SLIDER_SetValue(Slideres[3], (agentNations>>3) & 0x01);           
           
           Slideres[4]  = WM_GetDialogItem(pMsg->hWin,  ID_SLIDER_INA);
           WM_SetCallback(Slideres[4], &sldListener);
           HSD_SLIDER_SetRange(Slideres[4], 0, 1);                      
           HSD_SLIDER_SetValue(Slideres[4], (agentNations>>4) & 0x01);           
           
           Slideres[5]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_VIE);
           WM_SetCallback(Slideres[5], &sldListener);
           HSD_SLIDER_SetRange(Slideres[5], 0, 1);                      
           HSD_SLIDER_SetValue(Slideres[5], (agentNations>>5) & 0x01); 
           
           WINDOW_SetBkColor(pMsg->hWin, pSkin->bkColor);  

           for(i=0; i<6; i++)           {
              HSD_SLIDER_SetBkColor(Slideres[i], pSkin->sldBk);
              HSD_SLIDER_SetFocusBkColor(Slideres[i], pSkin->sldBk);
              HSD_SLIDER_SetSlotColor(Slideres[i], pSkin->sldSlot);
              HSD_SLIDER_SetFocusSlotColor(Slideres[i], pSkin->sldSlot);
              HSD_SLIDER_SetSliderColor(Slideres[i], pSkin->sldSlider);
              HSD_SLIDER_SetFocusSliderColor(Slideres[i], pSkin->sldFocusSlider);
           }
           break;
           
        case WM_PAINT:{
                GUI_RECT r;
                
                WM_GetClientRectEx(pMsg->hWin, &r);
                GUI_SetColor(pSkin->bkColor);
                GUI_FillRectEx(&r);
                GUI_SetColor(pSkin->ClientbkColor);
                r.x0  = r.x0 +10;
                r.x1  = r.x1 -10;
                r.y0  = r.y0 +40;
                r.y1  = r.y1 -40;
                GUI_FillRectEx(&r);
               
                GUI_SetFont(&GUI_Font30);
                GUI_SetTextMode(GUI_TEXTMODE_TRANS );
                GUI_SetColor(pSkin->txColor);
                
                GUI_DispStringAt("台湾:", 30,  Win_SysSet_txOrg);
                GUI_DispStringAt("日本:", 30,  Win_SysSet_txOrg +(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap));
                GUI_DispStringAt("韩国:", 30,  Win_SysSet_txOrg +(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*2);
                GUI_DispStringAt("朝鲜:", 30,  Win_SysSet_txOrg +(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*3);
                GUI_DispStringAt("印尼:", 30,  Win_SysSet_txOrg +(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*4);
                GUI_DispStringAt("越南:", 30,  Win_SysSet_txOrg +(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*5);
                
                GUI_DispStringAt("关闭", 120,  Win_SysSet_txOrg);
                GUI_DispStringAt("开启", 310,  Win_SysSet_txOrg);
                GUI_DispStringAt("关闭", 120,  Win_SysSet_txOrg +(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap));
                GUI_DispStringAt("开启", 310,  Win_SysSet_txOrg +(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap));
                GUI_DispStringAt("关闭", 120,  Win_SysSet_txOrg +(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*2);
                GUI_DispStringAt("开启", 310,  Win_SysSet_txOrg +(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*2);
                GUI_DispStringAt("关闭", 120,  Win_SysSet_txOrg +(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*3);
                GUI_DispStringAt("开启", 310,  Win_SysSet_txOrg +(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*3);
                GUI_DispStringAt("关闭", 120,  Win_SysSet_txOrg +(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*4);
                GUI_DispStringAt("开启", 310,  Win_SysSet_txOrg +(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*4);
                GUI_DispStringAt("关闭", 120,  Win_SysSet_txOrg +(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*5);
                GUI_DispStringAt("开启", 310,  Win_SysSet_txOrg +(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*5);
                
                GUI_SetFont(&GUI_Font24);
                GUI_DispStringAt("使用",30, Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*7+10);
                GUI_DispStringAt("选择选项",118, Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*7+10);
                GUI_SetColor(pSkin->sldSlider);
                GUI_DispStringAt("左右", 78, Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*7+10);
             }
             break;
             
         default:
             WM_DefaultProc(pMsg);
             break;
   }
}


WM_HWIN  sub4WinCreate(){
   WM_HWIN hWin;
   
   hWin  = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate ), _cbDialog,  WM_HBKWIN, 0, 0);
   return hWin;
}


static void sldListener(WM_MESSAGE* pMsg){
   const WM_KEY_INFO* pInfo;
   int   id  = 0;
   WM_MESSAGE myMsg;
   
   switch(pMsg->MsgId){
      case WM_KEY:
           pInfo  = (WM_KEY_INFO*)(pMsg->Data.p);
           switch(pInfo->Key){
              case GUI_KEY_BACKSPACE:
                   if(agentNations == SysConf.nations){
INFO("nations:0x%x, sys.nation:0x%0x",agentNations, SysConf.nations);                   
                      WM_SetFocus(Slideres[0]);
                      WM_SetFocus(menuWin);
                   }
                   else{
                      myMsg.hWin  = WM_GetClientWindow(confirmWin);
                      myMsg.hWinSrc  = subWins[4];
                      myMsg.MsgId   = USER_MSG_CHOOSE;
                      myMsg.Data.v  = SYS_SETTING;
                      WM_SendMessage(myMsg.hWin, &myMsg);
                      WM_BringToTop(confirmWin);
                      WM_SetFocus(WM_GetDialogItem(confirmWin, GUI_ID_BUTTON0));
                   }
                   break;
                   
              case GUI_KEY_LEFT:
                   id  = WM_GetId(pMsg->hWin) - ID_SLIDER_CTB;
                   agentNations  = (agentNations & (~(0x01<<id)));
                   SLIDER_Callback(pMsg);
                   break;
                   
              case GUI_KEY_RIGHT:
                   id  = WM_GetId(pMsg->hWin) - ID_SLIDER_CTB;
                   agentNations  = agentNations | (0x01<<id);
                   SLIDER_Callback(pMsg);
                   break;
                   
              default:
                   HSD_SLIDER_Callback(pMsg);
                   break;
           }
           break;
      
      default:
           HSD_SLIDER_Callback(pMsg);
           break;
   }
   
}


#endif






































