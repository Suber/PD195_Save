/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: sub functions 1 of music
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"

extern void DisInFFEnd(uint8 DisMod);
extern void openDAC(void);
#pragma name(AP_SUB_FUN1)
//===========[放到文件尾时的处理]===============
uint16 DealMusicFileEnd(uint8 DisMod)
{
    need_draw = TRUE;
    if (PlayStatus == FBPlaySta)
    {
        DisInFBHead(DisMod);//到头显示
    }
    else if (PlayStatus == FFPlaySta)
    {
        if (g_music_vars.repeat_mode != Intro)
        {
            DisInFFEnd(DisMod);//到尾显示
        }
        while (ap_get_message() != (AP_KEY_NEXT | AP_KEY_UP))
        {

            ;//do nothing
        }
        g_result = (uint16) ToNextMusic(Play_play);
        if (g_result == RESULT_UI_STOP)
        {
            return g_result;
        }
    }
    else
    {
        if ((g_music_vars.repeat_mode != Intro) && (return_to_play == 0))
        {
            DisInFFEnd(DisMod);//到尾显示
        }
        g_result = (uint16) ToNextMusic(Play_play);
        if (g_result == RESULT_UI_STOP)
        {
            return g_result;
        }
    }
    return NULL;
}

/*处理选择播放下一曲歌曲*/
uint16 ToNextMusic(Music_sel_mod_t mod)
{
    uint8 key;
    musicpSendCommand_bank(MC_STOP, NULL);
    musicpSendCommand_bank(MC_CLOSE, NULL);
    /* 格式错误跳到下一曲继续播放*/
    findfileagain: while ((key = ap_get_message()) != NULL)
    {
        switch (key)
        {
            case AP_KEY_PLAY | AP_KEY_UP:
            return RESULT_UI_STOP;
            break;
            default:
            break;
        }
    }
    ClearWatchDog();
    g_result = ChangeMusic(Music_Next, mod);
    if (!g_result)
    {
        return RESULT_UI_STOP;//回到停止界面
    }
    GetInformation(); //取文件信息
    ClearWatchDog();
    if (g_Openparam.typeerror == 1)
    {
		//sKY_SetGsensor(0,0,0);       //pedometer
 		Pedo_ui_draw = FALSE;
 		
        DisplayErrMsg(MFMTERR);//显示格式错
        SetFullInterface();
        RefreshMainScreen();
        goto findfileagain;

    }
    musicpSendCommand_bank(MC_OPEN, (void*) &g_Openparam);
    g_result = musicpSendCommand_bank(MC_LOAD, NULL);
    if (g_result == 0)
    {
        ErrReport(PlayStatus);
        SetFullInterface();
        RefreshMainScreen();
        goto findfileagain;

    }

    SetFullInterface();
    if (return_to_play == 0)
    {
       // UpDisplayFlag = UpDisplayFlag & (~up_musicbattery); //by zxs for not refresh the bg
       // RefreshMainScreen();
    }
    musicpSendCommand_bank(MC_PLAY, NULL);

    return NULL;
}

/*
 *******************************************************************************
 * Description : 换曲时要做的选曲，初始化动作
 * Arguments  :
 * Returns     :真时表示可继续放，假时表示要停止放(播放状态)
 * Notes       :
 *******************************************************************************
 */
bool ChangeMusic(uint8 Direct, Music_sel_mod_t mod)
{
    bool result;
	//fselSetLocation(&g_music_vars.location);	//matt120217#1
    RePlayStatus = NormalPlay;
    ClearTimePara();
    UpDisplayFlag |= up_musiccurrenttime;
    if (fsel_mode_change_flag != 0) //退出playlist 模式时要重新初始化文件选择器
    {
        fsel_mode_change_flag = FALSE;
        //初始化文件选择器
        FSELInit(g_MusicType, g_music_vars.repeat_mode, g_music_vars.fselmod, (uint8) g_music_vars.location.disk);
    }
    if (mod == Play_key)/*播放时手动切换按键的情况*/
    {
        if (g_music_vars.repeat_mode == FSEL_ALL_REPEATONE)
        {
            FSELSetMode(FSEL_ALL_REPEATALL);//PLAY 时按键
        }
        else
        {
            FSELSetMode(g_music_vars.repeat_mode);//
        }
    }
    else if (mod == Stop_key) /*停止界面手动切换歌曲*/
    {
        FSELSetMode(FSEL_ALL_REPEATALL);//STOP时按键
    }
    else/*正常播放到文件尾自动切换的情况*/
    {
        FSELSetMode(g_music_vars.repeat_mode);
    }

    switch (Direct)
    {
        case Music_Next:
        ifSelBuffToltalTimeFlag = FALSE;
        result = FSELGetNext(g_music_vars.location.filename);
        break;

        case Music_Prev:
        ifSelBuffToltalTimeFlag = FALSE;
        result = FSELGetPrev(g_music_vars.location.filename);
        break;

        default:
        return FALSE;
    }
    if (!result)
    {
        fselSetLocation(&g_music_vars.location);
        return result;
    }

    /*取当前歌曲数状态*/
    g_musicamount = (uint16) FSELGetTotal();
    g_musicsequence = (uint16) FSELGetNo();
    FSELSetMode(g_music_vars.repeat_mode);
    make_dirname(); /*取目录名*/
    return TRUE;
}

//==================================================================
bool musicpSendCommand_bank(uint8 cmd, void* param)
{
    bool result = FALSE;
    bool drm_flag;
    drm_flag = FALSE;
    //根据不同的音乐类型发不同的命令
    switch (cmd)
    {
        case MC_PLAY:
        openDAC();
        if (g_Openparam.typeerror == 0) //正常文件时进行下面操作
        {
#ifdef OPT_Sup_FadeIn_fun
            musicpSendCommand(MC_FADEINOUT, FadeIn_def);
#endif //
        }
        break;

        default:
        break;
    }

    switch (g_currentMusicType)
    {
        case Music_MP3: //mp3
        result = mp3pSendCommand(cmd, param);
        if (cmd == MC_LOAD)
        {
            if (g_music_vars.eq_type == SRS)
            {
                srs_callback(g_music_vars.SRSModeFlag);
            }
            else
            {
                EQMode_callback(g_music_vars.eq_type);
            }
        }
        break;

        case Music_WMA: //wma
        case Music_WMV: //wmv
        result = wmapSendCommand(cmd, param);
        if (cmd == MC_LOAD)
        {
            if (g_music_vars.eq_type == SRS)
            {
                srs_callback(g_music_vars.SRSModeFlag);
            }
            else
            {
                EQMode_callback(g_music_vars.eq_type);
            }
        }
        break;
        case Music_Wav:
        result = wavpSendCommand(cmd, param);
        if (cmd == MC_LOAD)
        {
            EQMode_callback(0);
        }
        default:
        break;
    }

    switch (cmd)
    {
        case MC_SETA_B:
        if (result == 0)
        {
            break;
        }
        UpDisplayFlag = UpDisplayFlag | up_MPRLmode;//刷新复读图标
        need_draw = TRUE;
        switch ((uint8) param)
        {
            case SetAPoint:
            g_2HZ_CNT = 0;//开始计AB点时间长度
            g_ABMinTimeCNT = ABReplayMiniTime_def;//定AB的最小时长为400ms
            RePlayStatus = WaitSetBPointForAB;
            break;

            case SetBPoint:
            g_ABPointTimeSize = g_2HZ_CNT;//记下AB点时间长度
            g_ReplayTimeCNT = 0;//复读次数清0
            RePlayStatus = ABLoopPlay;//自动进入复读模式
            break;
			case ClrABPoint:               //==heaimin想要清除AB点的记录
               RePlayStatus=NormalPlay;
			   break;
            default:
            break;
        }
        break;

        case MC_PLAY:
        if (result != 0)
        {
            PlayStatus = PlaySta;
        }
        break;

        case MC_STOP:
        PlayStatus = StopSta;
        break;

        case MC_CALTIME:

        break;

        default:
        break;
    }
    return result;
}

/*删除ID3信息中无用的空格符*/
void DealTheFirstSpace(uint8 *String, uint8 Size)
{
    uint8 i;
    uint8 j = 0;
    uint8 UnicodeFlag = 0;
    if (*((uint16 *) String) == 0xfffe)
    {
        UnicodeFlag = 1;
    }
    else if(String[0] == 0) /*修改全是空格符号的ID3信息没有处理的问题*/
    {
           return; 
    }
    if (String[0] == 0x20)
    {
        j += 1;
    }
    else
    {
        if (((UnicodeFlag == 1) && ((*((uint16 *) (String + 2)))) == 0x0000))
        {
            *String = 0;
            return;
        }
    }
    for (i = 1; i < Size; i++)
    {
        if (UnicodeFlag != 0)
        {
            i++;
            if (i > (Size - 2))
            {
                break;
            }
            if (*((uint16 *) (String + i)) == 0x2000)  /*修改Unicode空格字符的判断条件*/
            {
                j += 2;
            }
            else
            {
                break;
            }
        }
        else if (String[i] == 0x20)
        {
            j += 1;
        }
        else
        {
            break;
        }
    }
    if (j != 0)
    {
        if (UnicodeFlag != 0)
        {
            memcpy(String + 2, String + j + 2, Size - j - 2);
        }
        else
        {
            memcpy(String, String + j, Size - j);
        }

    }
}

/*转换“8＋3”短名*/
void MakeShortName(uint8 *str)
{
    uint8 i = 0;
    uint8 ext[3];
    memcpy(ext, &str[8], 3);
    for (; i < 8; i++)
    {
        if (str[i] == ' ')
        {
            break;
        }
    }
    str[i++] = '.';
    memcpy(&str[i], ext, 3);
    i += 3;
    str[i] = ' ';
    str[i+1] = 0;
    /*
    for (; i < 20; i++)
    {
        str[i] = ' ';
    }
    str[i] = 0;
    */
}
