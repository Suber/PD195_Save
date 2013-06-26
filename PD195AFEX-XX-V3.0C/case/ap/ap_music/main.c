/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: main module of music
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"
/*********模块的外部变量*********************************************/
extern kval_t kval;
extern uint8 earphoneinflag;

#pragma name(AP_MAIN)
/*常量段定义*/
const uint8 ResFileName[] = "ui30.res";
//const region_t mainscreensize =
//{ 0, 0, 128, 16 };

/*全局数据段定义*/
uint8 SRSModeFlag = 0;
bool g_introCntMode;
bool need_draw;
bool g_FCexitflag;
bool g_lyricflag = FALSE;//有无歌词标志,true表示有
bool g_ScrollFlag = FALSE; //当定时中断到时置这个标志
bool g_2HZFlag = TRUE;//2HZ中断到标志
bool g_PAOpenFlag = FALSE; //标志PA的开关状态

bool ifSelBuffToltalTimeFlag = FALSE;//TRUE：用 buffer 总时间数据
bool g_from_browser = FALSE;
bool CaltimeFlag = FALSE; //true: 不用再算时间

bool ReturnNotInit; //TRUE:表示下一个调用不用初始化,
uint8 Replaytimes_CNT; //
uint16 ABPointTimeSize;

uint8 g_TagInfoBuf[92];//用于放TAG信息的BUF
uint8 g_OldSecTempBackup;//记录当前秒的值,用于观察秒的变化,从而产生变化信号码
uint8 g_introCNT;
int8 MusicTimer_Sav;
int8 Timer_2HZNo_Sav;

uint8 g_DiskStateFlag = 0;//b7 = 1:flash 盘已格式化
uint8 fsel_mode_change_flag = FALSE;
uint8 g_ScrollTimeCNT = 0;//滚屏分频计数器

//uint8 g_LDBFlag = 0;//bit<0> 表示 歌词是否是unicode ，bit<1>，表示ldb格式
uint8 g_MusicType = FSEL_TYPE_MUSIC;
//uint8 PlayMenuactive = 0;//播放菜单的活动项
uint8 StopMenuactive = 0;//停止菜单的活动项
uint16 UpDisplayFlag = 0; //相应的位表示要更新显示的内容。
uint16 g_musicamount = 0;//当前目录歌曲总数
uint16 g_musicsequence = 0;//当前歌曲序号
uint16 g_2HZ_CNT;//2hz 中断计数器(0.5秒加一)
uint16 g_ABPointTimeSize;//记录AB点长度
uint16 g_result;//表示16位变量
Open_param_t g_Openparam;

comval_t g_comval; //系统变量,ap不能更改
music_vars_t g_music_vars;//本AP的公共变量
Replay_status_t RePlayStatus = NormalPlay; //普通播放模式
Play_status_t PlayStatus = StopSta; //停止状态
Music_type_t g_currentMusicType = Music_NON;//标志当前音乐的类型(mp3/wma/asf/wmv)
uint8  g_ReplayTimeCNT; //记录复读次数
uint8  g_ABMinTimeCNT = 0;//复读最小时长计数器
uint8  return_to_play = 0; //为1表示从其他界面返回播放界面
uint8  tag_is_unicode;
TimeRateData_sav_t TimeRateData_sav;
//uint8 temp_buffer[9] =
//{ 0 };

/*文件内调用函数声明*/
void read_var(void);
void openDAC(void);
void openDAC(void)
{
    dac_t dacattr;
    dacattr.dacselect = 0;
    dacattr.rate = FS_44K1;//默认
    EnableDAC(&dacattr);
}

int16 main(int16 param)
{
    int16 main_result;
    SD_FILE* g_Resfp_Sav;
    g_DiskStateFlag = 0;
    g_FCexitflag = FALSE;
    /*param大于1进入时为Voice模式*/
    if (param > 1)
    {
        param -= 2;
        g_MusicType = FSEL_TYPE_VOICE;
    }
    if (param != 0)
    {
        g_from_browser = TRUE;
    }
    /* 读入公共变量 */
    read_var();
    /*打开资源文件,在读入公共变量之后，根据language id显示正确的字符串资源*/
    g_Resfp_Sav = ResOpen(ResFileName);
    ui_auto_update = TRUE;
    /*显示请稍后*/
    show_waiting();
    //初始化全局变量
    
    //====================================
	{
//	 char  buf1[5];
	 //ClearScreen(NULL);
 /*    itoa4(g_music_vars.location.ClusterNo,buf1);
     SetTextPos(0,0);
	 PutS(buf1, -1);
	 itoa4(g_music_vars.location.DirEntry,buf1);
     SetTextPos(0,32);
	 PutS(buf1, -1);*/
	 //ResShowPic(IM_WAITING,0,0);
     //UpdateScreen(NULL);
/*	 ap_sleep(2);
	 ap_sleep(2);
	 ap_sleep(2);
	 ap_sleep(2);
	 ap_sleep(2)*/;
	}
	//====================================
	
    g_music_vars.location.ID3buffer = g_TagInfoBuf;
    g_Openparam.filename = g_music_vars.location.filename;//文件名地址
    g_Openparam.BreakPTSave = &g_music_vars.BreakPTSave;//备份参数地址
    g_Openparam.typeerror = 0;//默认为正常格式文件
    g_Openparam.SoftVolumeMax = 0;
    g_Openparam.FadeInTime = 0x03;
    memset(&TimeRateData_sav, 0, sizeof(TimeRateData_sav_t));
    g_music_vars.fselmod = FSEL_TYPE_COMMONDIR;//每次进入都是设为目录选择器

#ifdef OPT_Sup_LRC_fun
    lyricInit(LYRIC_SCREEN_128_32, g_comval.langid);//初始化歌词解释器
#endif
    Timer_2HZNo_Sav = (int8) TM_SetClock((int) Music_2HZint, 1);//打开 2HZ 定时
    MusicTimer_Sav = (int8) TM_SetTimer((int) Music_timeint, 2);//打开 20ms 定时,用于刷新ID3滚屏信息
    openDAC();
    OpenPA();
    if ((g_comval.ClassDFlag!=0) && (g_comval.KeyTone > 0)) //硬件支持classD
    {
#ifndef PC
        earphoneinflag = 1; //允许直驱与CLD的自动切换
#endif
    }
    SetLanguage((uint8) g_comval.langid);
    if (InitFileSelector() != 0)
    {   
        main_result = (int16)ui_stop();// =============> 进入界面
    }
    else
    {
        main_result = RESULT_NULL;
    }
    /*当按键音打开时不关闭PA*/
    if (!g_comval.KeyTone) //key tone add by mzh 2007.3.14
    {
        if (g_comval.EarphoneFlag == 0)
        {
            DisablePA();
        }
        else
        {
            DisablePADDV();
        }

        if ((g_comval.ClassDFlag!=0) && (g_comval.KeyTone == 0)) //硬件支持classD
        {
            DisableCLD();
#ifndef PC
            earphoneinflag = 0;
#endif
        }
        DisableDAC();
    }
    TM_KillTimer(MusicTimer_Sav);//关闭 20ms 定时
    TM_KillClock(Timer_2HZNo_Sav);//关闭 2hz 定时
    ResClose(g_Resfp_Sav);
    g_music_vars.location.fselTotal = 0;
    if (g_MusicType == FSEL_TYPE_MUSIC)
    {
        VMWrite(&g_music_vars, VM_AP_MUSIC);
    }
    else
    {
        VMWrite(&g_music_vars, VM_AP_VOICE);
    }
    #if(MACHINE_MODEL == 2)
    VMWrite(&g_comval, VM_SYSTEM);
    #endif
    return main_result;
}

/*
 *****************************************************************************
 ** 名字:
 ** 接口:
 ** 描述: 在 play 状态下碰到　错的报告
 ** 输入参数:
 ** 输出参数:
 ** 使用说明:
 *******************************************************************************
 */
void ErrReport(Play_status_t status)
{
    ap_sleep(2);
    if ((g_music_vars.location.disk == 'H') && (DRV_DetectUD(1) != 0x20))
    {
        DisplayErrMsg(CARDPULL);//显示卡拔出
    }
    else
    {
        DisplayErrMsg(MFMTERR);//显示格式错
    }
    if ((status != StopSta) && (status != PauseSta))//播放状态下停止dsp
    {
        musicpSendCommand(MC_STOP, NULL);
    }
    musicpSendCommand(MC_CLOSE, NULL);
}

/*
 ********************************************************************************
 *             void read_var(void)
 *
 * Description : 读系统变量和ap局部变量
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
void read_var(void)
{
    int16 result;

    result = VMRead(&g_comval, VM_SYSTEM, sizeof(g_comval));
    if (g_comval.magic != MAGIC_COMVAL)
    {
        GetDefaultComval(&g_comval);
    }
    ap_message_init(&g_comval);
    if (g_MusicType == FSEL_TYPE_MUSIC)
    {
        result = VMRead(&g_music_vars, VM_AP_MUSIC, sizeof(g_music_vars));
    }
    else
    {
        result = VMRead(&g_music_vars, VM_AP_VOICE, sizeof(g_music_vars));
    }
#ifdef PC
        g_comval.VolumeMax = 40;
#endif
	//g_comval.VolumeMax = 32;
    if (g_music_vars.magic != MAGIC_MUSIC)
    {
        g_music_vars.magic = MAGIC_MUSIC;
        g_music_vars.vol_display = g_comval.VolumeDef;

        g_music_vars.volume = g_comval.VolumeDef;//(g_music_vars.vol_display * SOFTVOL_MAX) / g_comval.VolumeMax;

        g_music_vars.eq_type = 0;
        g_music_vars.replaytimes = 5;
        g_music_vars.replaygap = 1;
        g_music_vars.repeat_mode = FSEL_ALL_SEQUENCE;
        g_music_vars.play_rate = 0;
        g_music_vars.location.disk = 'C';
        g_music_vars.fselmod = FSEL_TYPE_COMMONDIR;
        g_music_vars.MusicOpenMod = Nor_open;
        g_music_vars.SRSMod.Wow = 8;
        g_music_vars.SRSMod.Bass = 8;
        g_music_vars.SRSMod.Srs = 5;
        g_music_vars.SRSMod.Center = 4;
        g_music_vars.SRSMod.Definition = 6;
        g_music_vars.SRSMod.Speakersize = 1;
        g_music_vars.SRSMod.Limiter = 0;
        g_music_vars.SRSMod.GainSetting = 30;
        g_music_vars.SRSMod.IsNewFlag = 1;
        g_music_vars.SRSModeFlag = 0;
#ifdef OPT_Sup_FselFolder_fun
        g_music_vars.fselget_type = Normal;
#endif
#ifdef OPT_Sup_FFRevSpeed_fun
        g_music_vars.FFRevSpeed = 1;
#endif
#ifdef OPT_Sup_FFRevSound_fun
        g_music_vars.FFRevSound = 0;
#endif
#ifdef OPT_Sup_FadeInOutTime_fun
        g_music_vars.FadeInOutTime = 5;
#endif
        strcpy(g_music_vars.location.path, FS_CD_ROOT);
        ClearTimePara();
    }
    #if(MACHINE_MODEL==2)
    if ((g_from_browser != 0) && (g_comval.MTPFormatType != 17))
    #else
    if (g_from_browser != 0)
    #endif
    {
        browser_vars_t temp_browser_vars;
        VMRead(&temp_browser_vars, VM_AP_BROWSER, sizeof(browser_vars_t));
        memcpy(&g_music_vars.location, &temp_browser_vars.location, sizeof(file_location_t));
        g_music_vars.location.APE_Cur = 0;
        ClearTimePara();
    }
#ifdef OPT_Sup_FadeInOutTime_fun
    if(g_music_vars.FadeInOutTime> 15)
    {
        g_music_vars.FadeInOutTime = 10;
    }
#endif

    if(g_comval.PedoFlag)   //pedometer
    {
        //if(Pedo_Swim_flag)
        {
            sKY_BankGsensorGetStep(&Swim_total_step,&Swim_total_sec,1);
            Swim_work_flag = sKY_BankGsensorGetADC(6,1);
        }
        //else
        {
            sKY_BankGsensorGetStep(&Pedo_total_step,&Pedo_total_sec,0);
            Pedo_work_flag = sKY_BankGsensorGetADC(6,0);
        }
        Pedo_Swim_flag = Swim_work_flag;
    }

}

void ClearTimePara(void)
{
    g_music_vars.MusicOpenMod = Nor_open;
    g_music_vars.BreakPTSave.breakPT = 0; //开始断点为0
    g_music_vars.BreakPTDisTime.hour = 0;
    g_music_vars.BreakPTDisTime.minute = 0;
    g_music_vars.BreakPTDisTime.second = 0;
    g_music_vars.BreakPTSave.ApointSave = 0; //开始断点为0
    g_music_vars.BreakPTSave.BpointSave = 0; //开始断点为0
    StartIntro(TRUE)//因为时间清0了，所以浏览要从头开始
}

void OpenPA(void)
{

    pa_t pa =
    { 0 };

    if (!g_PAOpenFlag)
    {
        g_PAOpenFlag = TRUE;//标志PA已打开

        pa.pa_in.dacin = 1;//默认
        pa.volume = 0;
        if (g_comval.EarphoneFlag == 0)
        {
            EnablePA(&pa);
        }
        else
        {
            EnablePADDV(&pa);
        }

    }
    //openDAC();
}

