/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: follow compare and record play module
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"

#pragma name(AP_UI_FC)
//比特率的小图标(由于比标准ASCII小，所以要另出一套图标)
const uint16 NUM_PIC1[] =
{SYSFONT_SMALL_0,SYSFONT_SMALL_1,SYSFONT_SMALL_2,SYSFONT_SMALL_3,SYSFONT_SMALL_4,
 SYSFONT_SMALL_5,SYSFONT_SMALL_6,SYSFONT_SMALL_7,SYSFONT_SMALL_8,SYSFONT_SMALL_9};

void DealForExitRec(void); //退出录音时的处理
void Show_time_FC(void);
void OpenMusic(void);
/*
 ********************************************************************************
 *             uint16 ui_follow_compare( void )
 *
 * Description : 跟读对比主循环
 *
 * Arguments   : void
 *
 * Returns     : int
 *               RESULT_UI_STOP： 表示要进入停止状态，此时所有module已关闭
 *               key: 热健消息
 *               NULL: 表示正常退出
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 ui_follow_compare(void)
{
    uint16 result;

    {
        mp3_play_status_t status_buf;
        uint16 i;
        i = g_ABPointTimeSize / 10; //长 10%
        if (i < 2)
        {
            i = 2;
        }
        ABPointTimeSize = g_ABPointTimeSize + 2 + i;//2为经验补尝值

        musicpSendCommand(MC_GETSTATUS, (void*) (&status_buf));
        //保存用于跟读对比显示
        TimeRateData_sav.TotalTime_sav[0] = status_buf.total_time.hour;
        TimeRateData_sav.TotalTime_sav[1] = status_buf.total_time.minute;
        TimeRateData_sav.TotalTime_sav[2] = status_buf.total_time.second;
        ifSelBuffToltalTimeFlag = TRUE;
    }

    ReturnNotInit = FALSE; //下次需要初始化
    Replaytimes_CNT = 0;

    musicpSendCommand(MC_STOP, 1);//1表示MP3 退出时备份的是A点时间
    musicpSendCommand(MC_BACKUP, (void*) &g_music_vars.BreakPTSave);
    musicpSendCommand(MC_CLOSE, NULL);

    RePlayStatus = FollowRec;

    //UpDisplayFlag = UpDisplayFlag | up_MPRLmode;
    while (1)
    {
        switch (RePlayStatus)
        //状态处理
        {
            case WaitSetAPointForAB://待设A点
            case NormalPlay://普通播放
            musicpSendCommand(MC_SETA_B, ClrABPoint);//clear_a b
            return NULL;

            case FollowRec://跟读录音
            result = ui_followRec();
            if (result != 0)
            {
                return result;
            }
            break;

            case FollowAB://跟读AB
            result = ui_followAB();
            if (result != 0)
            {
                return result;
            }
            break;

            case ComparePlay://对比播放
            result = ui_comparePlay();
            if (result != 0)
            {
                return result;
            }
            break;

            case CompareAB: //对比AB
            result = ui_compareAB();
            if (result != 0)
            {
                return result;
            }
            break;

            case ABLoopPlay: //复读
            return NULL;

            default:
            break;
        }
    }
    return NULL;
}

/*
 ********************************************************************************
 *             void DealForExitRec( void )
 *
 * Description :
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
void DealForExitRec(void)
{
    wav_rec_status_t pStatus;

    wavrSendCommand(MC_STOP, NULL);

    do
    {
        wavrProceed();
        wavrSendCommand(MC_GETSTATUS, (void*) (&pStatus));
        if (RECORDING_DISK_ERROR == pStatus.status)
        {
            DisplayErrInfor(MREADERR);//显示磁盘错
            break;
        }
    } while (pStatus.status != RECORDING_STOP);

    wavrSendCommand(MC_CLOSE, NULL);

    //使控件可以出来
    ui_run_realtime = FALSE;
}
/*
 ********************************************************************************
 *             void Show_time_FC( void )
 *
 * Description : 刷新当前时间，刷新总时间，刷新比特率
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
void Show_time_FC(void)
{
    uint8 temp_buffer[9];
    // 刷新当前时间
    if ((UpDisplayFlag & up_musiccurrenttime) != 0)
    {
        time_t time;
        if (RePlayStatus == FollowRec)//跟读时显示时间
        {
            wavrSendCommand(MC_GETTIME, (void*) &time);
             
            
            itoa2(time.hour, temp_buffer);
            if (g_currentMusicType != Music_Wav)
                temp_buffer[0] = ' ';
            temp_buffer[2] = ':';
            itoa2(time.minute, temp_buffer + 3);
            temp_buffer[5] = ':';
            itoa2(time.second, &temp_buffer[6]);
            temp_buffer[8] = 0x00;
//            if (g_currentMusicType != Music_Wav)
                ui_show_f(temp_buffer);
//            else
//               ui_show_ef(temp_buffer, 0);
        }
        else if (RePlayStatus == ComparePlay)//对比放时显示时间
        {
            Full_time_t time_buf;
            if (wavpSendCommand(MC_GETTIME, (void*) &time_buf) != 0)
            {
                 
                
                itoa2(time_buf.TimeHour, temp_buffer);
                if (g_currentMusicType != Music_Wav)
                    temp_buffer[0] = ' ';
                temp_buffer[2] = ':';
                itoa2(time_buf.TimeMinute, temp_buffer + 3);
                temp_buffer[5] = ':';
                itoa2(time_buf.TimeSecond, &temp_buffer[6]);
                temp_buffer[8] = 0x00;
//                if (g_currentMusicType != Music_Wav)
                    ui_show_f(temp_buffer);
//                else
//                    ui_show_ef(temp_buffer, 0);
            }
        }
        else//其他状态显示时间(都是MUSIC播放)
        {
            Full_time_t time_buf;
            if (musicpSendCommand(MC_GETTIME, (void*) &time_buf) != 0)
            {
                g_music_vars.BreakPTDisTime.hour = time_buf.TimeHour;
                g_music_vars.BreakPTDisTime.minute = time_buf.TimeMinute;
                g_music_vars.BreakPTDisTime.second = time_buf.TimeSecond;
            }
            else
            {
                //如未能正确取得时间，则先不刷新显示
                UpDisplayFlag &= clr_musiccurrenttime;
            }

        }
        // UpDisplayFlag ^= up_musiccurrenttime;//清除刷新标志

    }

    //刷新比特率
    if ((UpDisplayFlag & up_musicbitrate) != 0)
    {
        if ((RePlayStatus == FollowRec) || (RePlayStatus == ComparePlay))
        {
            //            Show_MusicType();//先显示音乐类型图标

            UpDisplayFlag = UpDisplayFlag & clr_musicbitrate;//清除刷新标志
        }
    }

    //刷新总时间
    if (UpDisplayFlag & up_musictotaltime)
    {
        if ((RePlayStatus == FollowRec) || (RePlayStatus == ComparePlay))
        {

            itoa2(TimeRateData_sav.TotalTime_sav[0], temp_buffer);
            if (g_currentMusicType != Music_Wav)
                temp_buffer[0] = ' ';
            temp_buffer[2] = ':';
            itoa2(TimeRateData_sav.TotalTime_sav[1], temp_buffer + 3);
            temp_buffer[5] = ':';
            itoa2(TimeRateData_sav.TotalTime_sav[2], &temp_buffer[6]);
            temp_buffer[8] = 0x00;
//            if (g_currentMusicType != Music_Wav)
                ui_show_f(temp_buffer);
//            else
//                ui_show_ef(temp_buffer, 1);
        }
        else
        {
             

            itoa2(TimeRateData_sav.TotalTime_sav[0], temp_buffer);
            if (g_currentMusicType != Music_Wav)
                temp_buffer[0] = ' ';
            temp_buffer[2] = ':';
            itoa2(TimeRateData_sav.TotalTime_sav[1], temp_buffer + 3);
            temp_buffer[5] = ':';
            itoa2(TimeRateData_sav.TotalTime_sav[2], &temp_buffer[6]);
            temp_buffer[8] = 0x00;
//            if (g_currentMusicType != Music_Wav)
                ui_show_f(temp_buffer);
//            else
//                ui_show_ef(temp_buffer);
        }
        //RePlayStatus ^= up_musictotaltime;

        //UpDisplayFlag ^= up_musictotaltime;//清除刷新标志
        UpDisplayFlag &= clr_musictotaltime;//清除刷新标志

    }

    //刷新比特率
    if (UpDisplayFlag & up_musicbitrate)
    {
        //audio_format_t pFormat;
        //region_t region_b = {POSITION_B, 0, 21, 16};

        if ((RePlayStatus == FollowRec) || (RePlayStatus == ComparePlay))
        {
            Show_MusicType();//先显示音乐类型图标

            ResShowPic(NUM_PIC1[TimeRateData_sav.Bitrate_sav[0] - 0x30], POSITION_B + 1 + 2, 48);
            ResShowPic(NUM_PIC1[TimeRateData_sav.Bitrate_sav[1] - 0x30], POSITION_B + 6 + 3, 48);
            ResShowPic(NUM_PIC1[TimeRateData_sav.Bitrate_sav[2] - 0x30], POSITION_B + 12 + 3, 48);
            UpDisplayFlag = UpDisplayFlag & clr_musicbitrate;//清除刷新标志
        }
    }

    return;
}
/*
 ********************************************************************************
 *             void OpenMusic( void )
 *
 * Description :
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
void OpenMusic(void)
{
    g_music_vars.MusicOpenMod = ABPlay_open;
    musicpSendCommand(MC_OPEN, (void*) &g_Openparam);
    musicpSendCommand(MC_LOAD, NULL);
    musicpSendCommand(MC_SETPOWERCOUNT, 5); //set power out level count=2^4=16
    musicpSendCommand(MC_PLAY, (void*) g_music_vars.MusicOpenMod);
}
