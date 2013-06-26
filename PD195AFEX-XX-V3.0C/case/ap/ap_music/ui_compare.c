/*
******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
*
* Descripton: deal the compare and record status of music playing
*
* History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
******************************************************************************
*/

#include "ap_music.h"

#pragma name(AP_UI_CMP)

void Show_time_FC(void);
void OpenMusic(void);

/*
 ********************************************************************************
 *             uint16 ui_comparePlay( void )
 *
 * Description : 对比--------对比录音播放子状态
 *
 * Arguments   : void
 *
 * Returns     : int
 *
 * Notes       : 在此状态下，按play到stop状态；按mode到play;按PREV到跟读状态；
 *
 ********************************************************************************
 */

uint16 ui_comparePlay(void)
{
    uint8 key;
    wma_play_status_t status_buf;
    if (wavpSendCommand(MC_OPEN, NULL) != 0)
    {
        if (wavpSendCommand(MC_LOAD, (void*) FollowReadTempFileName) != 0)
        {
            if (!wavpSendCommand(MC_PLAY, NULL))
            {
                goto compareplayend;
            }
        }
        else
        {
            goto compareplayend;
        }
    }
    else
    {
        goto compareplayend1;
    }

    UpDisplayFlag = UpDisplayFlag | up_MPRLmode;
    need_draw = TRUE;

    while (1)
    {
        if (need_draw != 0)
        {
            RefreshMainScreen();
            Show_time_FC();
            need_draw = FALSE;
        }
        key = ap_get_message();

        switch (key)
        {
            case AP_MSG_STANDBY:
            case AP_MSG_LOCK: //不响应锁键
            break;

            case AP_KEY_PLAY | AP_KEY_UP:
            wavpSendCommand(MC_STOP, NULL);
            wavpSendCommand(MC_CLOSE, NULL);
            RePlayStatus = NormalPlay;
            g_FCexitflag = TRUE;
            OpenMusic();
            SetFullInterface();
            need_draw = TRUE;
            return RESULT_UI_STOP2; //选关播放后回到停止界面

            case AP_KEY_AB | AP_KEY_UP:
            case AP_KEY_NEXT | AP_KEY_UP:
            wavpSendCommand(MC_STOP, NULL);
            wavpSendCommand(MC_CLOSE, NULL);
            OpenMusic();
            RePlayStatus = WaitSetAPointForAB;
            SetFullInterface();
            return NULL;

            case AP_KEY_MODE | AP_KEY_UP:
            wavpSendCommand(MC_STOP, NULL);
            wavpSendCommand(MC_CLOSE, NULL);
            OpenMusic();
            RePlayStatus = NormalPlay;
            SetFullInterface();
            return NULL;

            case AP_KEY_PREV | AP_KEY_UP:
            wavpSendCommand(MC_STOP, NULL);
            wavpSendCommand(MC_CLOSE, NULL);
            Replaytimes_CNT = 0; //重新开始计复读次数
            RePlayStatus = FollowAB;
            return NULL;

            case AP_MSG_RTC:
            UpDisplayFlag = UpDisplayFlag | up_musiccurrenttime;
            Show_time_FC();
            ui_show_batt(GetBattery());
            break;

            default:
            key = ap_handle_hotkey(key);
            if (key == RESULT_REDRAW)
            {
                SetFullInterface();
                need_draw = TRUE;
            }
            else if (key != 0)
            {
                wavpSendCommand(MC_STOP, NULL);
                wavpSendCommand(MC_CLOSE, NULL);
                g_FCexitflag = TRUE;
                OpenMusic();
                SetFullInterface();
                need_draw = TRUE;
                return key;
            }
            else
            {

            }
            break;
        } //switch(key)

        wavpSendCommand(MC_GETSTATUS, (void*) &status_buf);

        if (status_buf.status == PLAYING_REACH_END)
        {
            wavpSendCommand(MC_STOP, NULL);
            compareplayend:

            wavpSendCommand(MC_CLOSE, NULL);
            compareplayend1: RePlayStatus = CompareAB;
            return NULL;
        }
    } //while(1)
    return NULL;
} //ui_comparePlay(int)

/*
 ********************************************************************************
 *             uint16 ui_compareAB( void )
 *
 * Description : 对比--------对比音乐播放子状态
 *
 * Arguments   : void
 *
 * Returns     : int
 *
 * Notes       : 在此状态下，按play到stop状态；按mode到play;按PREV到跟读状态；
 *
 ********************************************************************************
 */
uint16 ui_compareAB(void)
{
    uint8 key;
    wma_play_status_t status_buf;

    if (!ReturnNotInit)
    {
        OpenMusic();
    }
    ReturnNotInit = FALSE;
    UpDisplayFlag = UpDisplayFlag | up_MPRLmode;
    need_draw = TRUE;

    while (1)
    {
        if (need_draw!=0)
        {
            Show_time_FC(); //显示比特率
            RefreshMainScreen();
            need_draw = FALSE;
        }

        key = ap_get_message();

        switch (key)
        {
            case AP_MSG_STANDBY:
            break;

            case AP_KEY_PLAY | AP_KEY_UP:
            g_result = DealPlayKey();
            if (g_result != 0)
            {
                return g_result;
            }
            return RESULT_UI_STOP;

            case AP_KEY_AB | AP_KEY_UP:
            case AP_KEY_NEXT | AP_KEY_UP:
            RePlayStatus = WaitSetAPointForAB;
            return NULL;

            case AP_KEY_MODE | AP_KEY_UP:
            RePlayStatus = NormalPlay;
            return NULL;

            case AP_KEY_PREV | AP_KEY_UP:
            ReturnNotInit = TRUE; //要求退回跟读AB时不要初始化
            Replaytimes_CNT = 0; //重新开始计复读次数
            RePlayStatus = FollowAB;
            return NULL;

            case AP_KEY_VOLSUB:
            case AP_KEY_VOLADD:
            case AP_KEY_VOL:

            need_draw = TRUE;
            break;

            case AP_MSG_RTC:
            ShowNowTime();
            ui_show_batt(GetBattery());
            break;

            default:
            key = ap_handle_hotkey(key);
            if (key == RESULT_REDRAW)
            {
                SetFullInterface();
                need_draw = TRUE;
            }
            else if (key != 0)
            {
                return key;
            }
            else
            {

            }
            break;
        } //switch(key)
        musicpSendCommand(MC_GETSTATUS, (void*) &status_buf);
        if (status_buf.signal == SIGNAL_REACH_B)
        {
            Show_time_FC(); //显示比特率
            RefreshMainScreen();
            need_draw = FALSE;
            musicpSendCommand(MC_STOP, NULL);
            musicpSendCommand(MC_CLOSE, NULL);
            RePlayStatus = ComparePlay;
            return NULL;
        }
    } //while(1)
    return NULL;
} //ui_compareAB
