/*
*******************************************************************************
*                ACTOS AP
*        system setting ap of AS211A
*
*        (c) Copyright, Actions Co,Ld.
*             All Right Reserved
*
*  $Id: main.c,v 1.2 2010/03/29 10:48:01 Mars Exp $
*******************************************************************************
*/
#include "cfg.h"
#pragma name(CFG_MAIN)

/************************头信息定义**************************************/

//const char   CfgMagic[4]={"CFG"};
//const uint8  TotallItem = 5;
//const char   ReservHead1[11];
#pragma userclass(nconst = CFG_HEAD)
const  file_cfg_head_t FileHead = 
{
   "CFG",9,{0xfe,0xff,0x00}    
};

const cfg_item_t CfgItem[6] = 
{
   {(uint32)(&ParaHead),"Parameters","参数设置"},
   {(uint32)(&KeyCfg),"Key-press","按键"},
   {(uint32)(&APCfg),"Application","应用程序"},
   {(uint32)(&LanguageCfg),"language","语言"},
   {(uint32)(&FileCfg),"File-exchange","替换文件"},
   {(uint32)(&FuncCfg),"Functions","功能配置"}        
};
const char ReservHead2[336];

#pragma userclass(nconst = CFG_PARA)

/******************参数设置********************************************/
const paraset_head_t ParaHead = 
{(uint32)ValueSet,16,(uint32)(&StringSet),2,""};

const value_t  ValueSet[16] = 
{
    {10,60,0,1,"","Auto Lock","自动锁时间(秒)"},
    {12,15,0,1,"","contrast","对比度设置"},
    {10,60,0,1,"","backlight time","背光时间(秒)"},
    {1,60,0,1,"","Power off time","关机时间(分钟)"},
    {1,1,0,1,"","AllwaysOnPlaying","播放中不关机"}, //g_modifyval.SleepTime

    #if(Volume_Ctl==2)
	{27,40,0,1,"","volume set","音量等级"},
    {13,40,0,1,"","vol default","默认音量"},
    #elif(Volume_Ctl==1)
    {32,40,0,1,"","volume set","音量等级"},
    {16,40,0,1,"","vol default","默认音量"},
    #else
    {40,40,0,1,"","volume set","音量等级"},
    {24,40,0,1,"","vol default","默认音量"},
    #endif    
    
	{6,7,0,1,"","Record gain","录音增益"},
	
    {50,200,20,1,"","GS Strid","步长设置"},
    {0,1,0,1,"","PedoUnit","公英制转换"},
    {50,200,20,1,"","GS Weight","体重设置"},
    {50,200,20,1,"","ArmLen  ","臂长设置"},
    {50,200,5,1,"","PoolLen ","池长设置"}, 
    {18,200,1,1,"","Age","年龄设置"},
    {1,1,0,1,"","Gender","性别设置"},
    {170,255,20,1,"","Height","身高设置"},
};

const string_t StringSet[2]=
{
   {"AS211A_V1000","Device Version","设备版本"}, 
   #if(LCD_TYPE==1)
   {"05.01.01","Firmware Version","固件版本"} 
   #elif(LCD_TYPE==2)
   {"05.01.02","Firmware Version","固件版本"} 
   #elif(LCD_TYPE==3)
   {"05.01.03","Firmware Version","固件版本"} 
   #elif(LCD_TYPE==4)
   {"05.01.14","Firmware Version","固件版本"} 
   #endif
};

#pragma userclass(nconst = CFG_KEY)


/********************按键设置**********************************************/
//const uint8 KeyTotal = 5;
//const uint8 TotalItem = 11;
//const char  KeyReserv[14];
//const uint8 keyValue[16]

const key_cfg_t KeyCfg = 
{
    9,11,"",
    {
    #if(MACHINE_MODEL==1)
        Msg_KeyNull,Msg_KeyPlayPause,Msg_KeyREC,Msg_KeyMenu,Msg_KeyLast,Msg_KeyNext,Msg_KeyLoop,Msg_KeyVolAdd,Msg_KeyVolSub,Msg_KeyHold,Msg_KeyEQ
    #elif(MACHINE_MODEL==2)
        Msg_KeyNull,Msg_KeyPlayPause,Msg_KeyVolSub,Msg_KeyVolAdd,Msg_KeyLast,Msg_KeyNext,Msg_KeyMenu,Msg_KeyREC,Msg_KeyLoop,Msg_KeyHold,Msg_KeyEQ
    #endif
    } 
};


const key_infor_t KeyInfor[11]=
{
    {Msg_KeyNull,0,"NULL"},
    {Msg_KeyPlayPause,1,"PLAY/PAUSE"}, //PLAY键不可配置
    {Msg_KeyLoop,0,"LOOP"},
    {Msg_KeyVolSub,0,"VOL-"},
    {Msg_KeyVolAdd,0,"VOL+"},
    {Msg_KeyLast,0,"PREV"}, 
    {Msg_KeyNext,0,"NEXT"},
    {Msg_KeyMenu,0,"MENU"}, 
    {Msg_KeyREC,0,"REC/AB"},
    {Msg_KeyEQ,0,"EQ"}, 
    {Msg_KeyVol,0,"VOL"}
};

#pragma userclass(nconst = CFG_AP)

/********************AP 配置*****************************/
//const uint8 TotalAP = 10;
//const uint8 AvtiveAP = 1;
//const char  Reserv[14];

const ap_cfg_head_t APCfg = 
{
    11,1,""    
};

const ap_infor_t APTable[11] =
{
    {RESULT_MUSIC,1,"Music","音乐"},
    {RESULT_MOVIE,0,"Movie","电影"},
    {RESULT_REC_NOSTART,0,"Record","录音"},
    #if(MACHINE_MODEL==2)
    {RESULT_VOICE,0,"Voice","放音"},
    #else
    {RESULT_VOICE,1,"Voice","放音"},
    #endif
    {RESULT_RADIO,1,"Radio","收音机"},
    {RESULT_VIEW,0,"Picture","图片浏览"},
    {RESULT_SYSTEM,1,"Setting","系统设置"},    
    {RESULT_READER,0,"Reader","电子书"},    
    {RESULT_GAME,0,"Game","游戏"},
	{RESULT_TELBOOK,0,"Tel-Book","电话本"},
	{RESULT_BROWSER,0,"Browser","文件导航"}
};

#pragma userclass(nconst = CFG_LAN)
/******************语言设置*****************************************/
const lang_infor_t LanguageCfg = 
{   
    //10,2,
    //{
    //    CHINESE_SIMPLIFIED,CHINESE_TRADITIONAL,ENGLISH,/*JAPANESE,KOREAN,*/
    //    FRENCH,GERMAN,ITALIAN/*,DUTCH*/,PORTUGUESE,SPANISH,/*SWEDISH,CZECH,
    //    DANISH,*/POLISH,RUSSIAN/*,TURKISH,HEBREW,THAI,HUNGARIAN,ROMANIAN,
    //    ARABIC,INDONESIAN,GREEK*/ 
    //}
    
    22,2,
    {
        CHINESE_SIMPLIFIED,CHINESE_TRADITIONAL,ENGLISH,JAPANESE,KOREAN,
        FRENCH,GERMAN,ITALIAN,DUTCH,PORTUGUESE,
        SPANISH,SWEDISH,CZECH,DANISH,POLISH,
        RUSSIAN,TURKISH,THAI,HUNGARIAN,ROMANIAN,
        INDONESIAN,GREEK,/*BULGARIAN,SLOVAK,UKRAINIAN,
        CROATIA,LITHUANIAN,ESTONIAN,LATVIAN,SERBIAN*/
    }
};

/*************文件替换******************/
#pragma userclass(nconst = CFG_FILE_EX)
//const uint8 FileTotal = 10;
//const char  FileExReserv[15];
const file_ex_t FileCfg = 
{
  17,""  
};

const char  FileName[][16]=
{
    "music.ap",
    "amv.ap",
    "record.ap",
    "voice.ap",
    "fmradio.ap",
    "viewpic.ap",
    "setting.ap",
    "reader.ap",
    "TelBook.ap",
    "game.ap",
    "main.ap",
    "udisk.ap",
    "standby.ap",
    "ky_hard.drv",
    "unicode.drv",
    "welcome.bin",
    "unicode.fon",
    "browser.ap"
};

/*功能配置项*/
#pragma userclass(nconst = CFG_FUNC)
const func_cfg_t FuncCfg = 
{
 5,""   
};

const func_item_t FuncItem[5] = 
{
    {0,"","Support Card","支持插卡"},
    #if(MACHINE_MODEL==1)
    {1,"","Earphone","耳机直通"},
    #elif(MACHINE_MODEL==2)
    {0,"","Earphone","耳机直通"},
    #endif
    {0,"","Class D","D类功放"},
    {0,"","KeyTone","按键音"},  
    #if (PETOMETER_USE)
    {1,"","Pedometer","计步器"},
    #else
    {0,"","Pedometer","计步器"},
    #endif
};
#pragma userclass(nconst = default)
