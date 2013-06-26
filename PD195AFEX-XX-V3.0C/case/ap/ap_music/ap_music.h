/*
******************************************************************************* *                               AS211A
*                              AS211A
*                              Module: music ap
*                 Copyright(c) 2002-2010 Actions Semiconductor,
*                            All Rights Reserved. 
*
* Descripton: the head file of music
*
* History:         
*      <author>    <time>           <version >             <desc>
*       xusong    2010-07-14 11:00     1.0             build this file 
 *******************************************************************************
 */
#ifndef _AP_MUSIC_H
#define _AP_MUSIC_H
#include "ap_common.h"//控件头文件
#include "mmcmd.h"//模块层命令名定义
#include "ap_option.h"//AP功能开关选择
#include "basal.h"//basal module 头文件
#include <rand.h>

#define SOFTVOL_MAX    40
#define CARD_KERNEL 1

#define RESULT_CURR_UI NULL//在当前界面
#define RESULT_LAST_UI RESULT_USER1+1//回上一界面
#define RESULT_UI_STOP RESULT_USER1+2//回 STOP 界面
//第二种方式回到 STOP 界面,这种方式是先停止播放
#define RESULT_UI_STOP2 RESULT_USER1+3
#define RESULT_UI_PLAY  RESULT_USER1+4//回 PLAY 界面
#define RESULT_LIST         RESULT_USER1+5  //列表播放消息
#define RESULT_LIST_ERR     RESULT_USER1+6  //列表不存在或者类表错误
#define RESULT_PLAYLIST_PLAY     RESULT_USER1+7  //播放列表中的文件消息
#define RESULT_USERLIST     RESULT_USER1+8

/*切换上下曲的五种方式*/
#define Music_Reset_Head 0
#define Music_Reset_End 1
#define Music_Cur   2//选当前
#define Music_Next  3//选下一首
#define Music_Prev  4//选上一首
#define POSITION_LRCBG     (0)//歌词显示背景图坐标
#define POSITIONY_LRCBG     (0)

#define POSITION_LRCTOP     (6)//歌词显示文本坐标
#define POSITIONY_LRCTOP     108//(93)
#define POSITION_LRCMID     (6)//歌词显示文本坐标
#define POSITIONY_LRCMID     70//(57)
#define POSITION_LRCBOT     (6)//歌词显示文本坐标
#define POSITIONY_LRCBOT    23// (21)
#define SOFTVOLUMEMAX     40  //数字音量最大级数
#define MENU_GROUP         5  //一屏显示的菜单项

//--------- [UpDisplayFlag] ---要求刷新标志
#define up_musictypeflag    0x8000
#define up_lyricflag        0x4000
#define up_musicbitrate     0x2000
#define up_musicloopmode    0x1000
#define up_musicsequence    0x0800
#define up_MPRLmode         up_musicsequence
#define up_musiceqmode      0x0400
#define up_musiccurrenttime 0x0200
#define up_musictotaltime   0x0100
#define up_musicbattery     0x0080
#define up_musicinformation 0x0040
#define up_musicBackGDPic   0x0001    //刷新背景
//-----------------------------清除刷新标志
#define clr_musictypeflag    0x7fff
#define clr_lyricflag        0xbfff
#define clr_musicbitrate     0xdfff
#define clr_musicloopmode    0xefff
#define clr_musicsequence    0xf7ff
#define clr_MPRLmode         clr_musicsequence
#define clr_musiceqmode      0xfbff
#define clr_musiccurrenttime 0xfdff
#define clr_musictotaltime   0xfeff
#define clr_musicbattery     0xff7f
#define clr_musicinformation 0xffbf
#define clr_musicBackGDPic   0xfffe    //清除背景

//defines for ui lyric
#define LyricIcon_Pos_X 96//116
#define LyricIcon_Pos_Y 40//48

//=========设置显示背景=============
#define SetDisplayBackGDPic()       \
{                                   \
    UpDisplayFlag = UpDisplayFlag |     \
                    up_musicBackGDPic;  \
}

//=========设置STOP界面参数==========
#define SetStopInterface()          \
{                                   \
    ui_auto_update=FALSE;               \
    UpDisplayFlag=UpDisplayFlag |       \
                  up_musictypeflag |    \
                  up_lyricflag |        \
                  up_musicbitrate |     \
                  up_musicloopmode |    \
                  up_musicsequence |    \
                  up_musiceqmode |      \
                  up_musiccurrenttime | \
                  up_musictotaltime |   \
                  up_musicbattery;      \
}

//=========设置刷新全界面参数=====
#define SetFullInterface()          \
{                                   \
    ClearScreen(NULL);              \
    ui_auto_update=FALSE;               \
    UpDisplayFlag=UpDisplayFlag |       \
                  up_musictypeflag |    \
                  up_lyricflag |        \
                  up_musicbitrate |     \
                  up_musicloopmode |    \
                  up_musicsequence |    \
                  up_musiceqmode |      \
                  up_musiccurrenttime | \
                  up_musictotaltime |   \
                  up_musicbattery |     \
                  up_musicBackGDPic |    \
                  up_musicinformation;  \
}

//=========设置刷新全界面参数=====
#define SetStopToPlayInterface() \
{                                \
    ui_auto_update=FALSE;            \
    UpDisplayFlag=UpDisplayFlag |    \
                  up_musictypeflag | \
                  up_lyricflag |     \
                  up_musicbitrate |  \
                  up_musicloopmode | \
                  up_musicsequence | \
                  up_musiceqmode |   \
                  up_musicbattery;   \
}

#define StartIntro(mode)                  \
{                                         \
    g_introCntMode = mode;                    \
    g_OldSecTempBackup                        \
        = g_music_vars.BreakPTDisTime.second; \
    g_introCNT = 0;                           \
}

/*music 应用结构体定义*/
typedef enum
{
    NormalPlay, //普通模式的显示(0)
    WaitSetAPointForAB, //AB放的待设A点(1)
    WaitSetBPointForAB, //AB放的待设B点(2)
    ABLoopPlay, //AB循环播放(3)
    FollowRec,//跟读录音(4)
    FollowAB,//跟读放AB(5)
    ComparePlay,//对比播放(6)
    CompareAB  //对比放AB(7)
} Replay_status_t;

typedef enum
{
    StopSta, //停止(0)
    PauseSta, //暂停(1)
    PlaySta, //普通模式的显示(2)
    FFPlaySta, //快进状态(3)
    FBPlaySta  //快退状态(4)
} Play_status_t;

typedef enum
{
    LoopNor, //普通循环
    LoopOne, //循环放当前首
    LoopOneDir, //循环放当前目录
    LoopDir, //循环放目录
    LoopAll, //循环所有
    Random, //随机播放
    Intro, //浏览播放
    FolderShift,
    MaxRepeatMod
} Repeat_Mode_t;

typedef enum
{
    Music_NON = 0, Music_MP1, Music_MP2, Music_MP3, Music_WMA,
    //  Music_ASF,
    Music_WMV,
    Music_Wav,
    Music_TypeMax
} Music_type_t;

typedef enum
{
    Stop_key, Play_play, Play_key, list_key
} Music_sel_mod_t;

//AP_MUSIC使用的系统变量
typedef struct
{
    uint8 Wow;
    uint8 Bass;
    uint8 Srs;
    uint8 Center; //环绕
    uint8 Definition; //频段突出
    uint8 Speakersize;
    uint8 Limiter;
    uint8 GainSetting;
    uint8 IsNewFlag;
} SRSMod_t; //SRS WOW 音效

//AP_MUSIC使用的系统变量
typedef struct
{
    uint16 magic; // +0
    uint8 volume; //
    Repeat_Mode_t repeat_mode; //repeat 菜单的序号  //+1
    eq_t eq_type; //+2
    int8 play_rate; //播放速度 //+3
    uint8 replaytimes; //复读次数// +4
    uint8 replaygap; //复读间隔//+5
    Open_mod_t MusicOpenMod; //打开方式 +6
    file_location_t location; //+ 9
    breakPT_t BreakPTSave; //断点参数 +36
    time_t BreakPTDisTime; //断点时间备份 + //
    uint8 fselmod; //文件选择器的模式
    SRSMod_t SRSMod;
    uint8 SRSModeFlag;
    uint8  vol_display; //音量值的显示
} music_vars_t;

typedef struct
{ //作用是在跟读录音和对比放音都显示原音的
    uint8 Bitrate_sav[3]; //比特率
    uint8 TotalTime_sav[3]; //总时间
} TimeRateData_sav_t;

typedef struct
{
    uint16 HeadIcon;//头部图标的ID值
    uint16 HeadStr;//头部信息的ID值
    uint8 *string;//信息显示区的显示的字串首址
} dialog_t; //对话框

//===========================================================
//下面声名外部变量
//===========================================================
extern music_vars_t g_music_vars;
extern Replay_status_t RePlayStatus;
extern Play_status_t PlayStatus;
extern uint16 UpDisplayFlag;
extern uint16 g_result; //常用的16位临时变量
//extern slider_t slider;
extern Music_type_t g_currentMusicType;
extern uint16 g_musicamount;
extern uint16 g_musicsequence;
extern uint8 g_replaytimes;
extern uint8 g_replaypause;
extern uint8 g_ScrollFlag;
extern bool g_lyricflag;
extern uint16 g_2HZ_CNT;
extern bool g_2HZFlag;
extern uint16 g_ABPointTimeSize;
extern uint8 g_TagInfoBuf[92];
extern uint8 g_TagInfoBuf1[];
extern region_t scrollscreensize;
extern bool g_ifAVflag;
extern BOOL need_draw;
extern uint8 g_volume;
extern bool g_introCntMode;
extern uint8 g_OldSecTempBackup;
extern uint8 g_introCNT;
extern comval_t g_comval;
extern uint8  g_ReplayTimeCNT;
extern TimeRateData_sav_t TimeRateData_sav;
extern uint8 g_FadeInStatusFlag;
extern uint8  g_ABMinTimeCNT;
extern Open_param_t g_Openparam;
extern bool g_CardInFlag;
extern uint8 g_DiskStateFlag;
extern uint8 MotivePosion0;
extern uint8 MotivePosion1;
extern bool ifSelBuffToltalTimeFlag;
extern bool CaltimeFlag;
extern uint8 WaitForCaltimeCNT;
extern uint8 savenum;//时间进度条save时间
extern bool g_from_browser;
extern uint8  return_to_play; //是否从菜单界面返回play界面的函数
extern bool g_FCexitflag;
extern uint8 fsel_mode_change_flag;
extern uint8 g_ScrollTimeCNT;
extern uint8  g_MusicType;

extern bool ReturnNotInit; //TRUE:表示下一个调用不用初始化,
extern uint8 Replaytimes_CNT; //
extern uint16 ABPointTimeSize;

extern bool g_LDBFlag;
extern bool g_file_error_flag;
extern uint8  tag_is_unicode;
//extern const region_t mainscreensize;
extern uint8 temp_buffer[9];
//extern function_menu_t menu;
//*********************************************
extern void make_dirname(void);
extern void show_volume_menu();
extern void OutPutVolume(uint8 vol);
extern void DisInFBHead(uint8 DisMod);//快退到头显示
extern void Deal_Error_Playing(void); //播放时错误处理
extern void SetStopToPlayInterfaceForPlay(void);//停止界面到播放界面转换
extern void EQMode_callback(uint8 value); //EQ设置的回调函数


//下面声名函数原型
//--------class isr----------------
#pragma ROM(large)
void Music_timeint(void);
void Music_2HZint(void);
#pragma ROM(huge)

void volume_callback(uint16 Vol);
void speed_callback(uint16);
void eq_callback(uint16);
void srs_callback(uint16 srs);
void bass_user_callback(uint16 bass_user);
void srs_user_callback(uint16 srs_user);
void wow_user_callback(uint16 wow_user);
void center_user_callback(uint16 center_user);
void definition_user_callback(uint16 definition);
void speakersize_user_callback(uint16 speakersize);
void limiter_user_callback(uint16 limiter);
void send_user_srs();
void loop_callback(uint16);
void listloop_callback(uint16 loop);
void fselget_callback(uint16 fselget);
//--------------------------------
//-------class ui-------
uint16 ui_stop(void);
uint16 ui_play(void);
uint16 ui_lyric(void);
uint16 ui_play_menu(uint8 Playsta);//Playsta 区分是哪个界面
uint16 ui_stop_menu(void);
uint16 ui_EQ_menu(void);
uint16 ui_SRS_menu(void);
uint16 ui_SRSUSER_menu(void);
uint16 ui_loop_menu(void);
uint16 ui_listloop_menu(void);
uint16 ui_follow_compare(void);
uint16 ui_followAB(void); //跟读AB
uint16 ui_followRec(void); //跟读录音
uint16 ui_compareAB(void); //比较AB
uint16 ui_comparePlay(void); //比较播放
uint16 Deal_music_menu(void);
uint16 UI_Favourite(void);
uint16 music_list_menu(void);
uint16 Deal_in_menu(void);
//---------------------------

//-------class display-------
uint16 RealDisplay(void);
void ShowNowTime(void);
void RefreshMainScreen(void);
void Show_Bitrate(void); //显示比特率
void DisplayErrMsg(uint16 ErrMsg);
void Show_MusicType(void);//显示音乐类型图标ssssss
//---------------------------

//-------class functions-----
Music_type_t CheckMusicType(uint8 * filename);
uint16 play_key_next(uint16 key);
uint16 play_key_prev(void);
uint16 ToNextMusic(Music_sel_mod_t mod);
void GetInformation(void);
bool ChangeMusic(uint8 Direct, Music_sel_mod_t mod);
void SelectCurMusic(void);//选择当前歌曲
bool musicpSendCommand(uint8, void*);
bool musicpSendCommand_bank(uint8 cmd, void* param);
//void StartIntro( bool mode );
bool DealErrCase(void);
uint16 ChangeVolume(void);
uint16 DealMusicFileEnd(uint8 DisMod);
uint16 DealPlayKey(void);
void CheckVolumeLimit(void); //检测音量是否超出范围
void CheckPlayrateLimit(void); //检测播放速度是否超出范围
void CheckReplaygapLimit(void); //检测复读间隔是否超出范围
void CheckReplaytimesLimit(void); //检测复读次数是否超出范围
void BackupCurrentTime(void);
void FadeInOutFun(void);
void FadeOut(bool DisMod);
uint16 PrevMusicInPlay(void);
void ClearTimePara(void);//清0显示时间值及断点清0
void DealForABGap(music_play_status_t *status);
void ClosePA(void);
void openPA(void);
void DisplayErrInfor(uint16 id);
uint16 InitFileSelector(void);
uint16 StartFileSelector(void);
uint16 UpdateFileSelector(void);
void ErrReport(Play_status_t status);
void OpenPA(void);
void FlashMotived(uint8 value);

uint16 DealKeyResult(uint8 key);
uint16  DealKeyForPlayStatus(uint8 key);

#endif     //_ap_music_H
