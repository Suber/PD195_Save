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

#pragma name(AP_UI_FOL)

#define DEF_RECLOWBATT 5//定义最低能录音的电压
//判断是低电的次数，当电压值低于DEF_RecLowBatt值次数超过这个值时确认为低电
#define DEF_RECLOWBATTIME 5

void DealForExitRec(void); //退出录音时的处理
void Show_time_FC(void);
void OpenMusic(void);
/*
 ********************************************************************************
 *             uint16 ui_followRec( void )
 *
 * Description : 跟读--------录音子状态
 *
 * Arguments   : void
 *
 * Returns     : int
 *               RESULT_UI_STOP： 表示要进入停止状态，此时所有module已关闭
 *               key: 热健消息
 *               NULL: 表示正常退出
 *
 * Notes       : 在此状态下，按play到stop状态；按mode到play;按NEXT到对比状态；
 *
 ********************************************************************************
 */
uint8 IsLowPower(uint8 power)
{
    /*锂电池低电判断标准*/
    #if(MACHINE_MODEL==0)
    if((GetUsbCableStatus() & 0xc0)!= 0)
    #elif((MACHINE_MODEL==1)||(MACHINE_MODEL==2))
    if(check_5v())
    #endif
	{
	    return 0;
	}
    if (power > LITHIUM_LOW )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

uint16 ui_followRec(void)
{
    uint8 key;
    uint8 LowBatt_CNT = 0;//低电计数器
    uint16 result;
    uint32 DiskSpace;

    if (++Replaytimes_CNT > g_music_vars.replaytimes)
    {
        FS_FRemove(FollowReadTempFileName); //如有这个文件就删除
        OpenMusic();
        RePlayStatus = ABLoopPlay;
        UpDisplayFlag = UpDisplayFlag | up_MPRLmode;
        need_draw = TRUE;
        return NULL;
    }

    FS_FRemove(FollowReadTempFileName); //如有这个文件就删除
    DiskSpace = FS_GetSpace(1);

    //如果磁盘空间少于100k不能跟读
    if (DiskSpace <= 200)
    {
        DisplayErrMsg(MWRIERR);
        OpenMusic();
        RePlayStatus = ABLoopPlay;
        SetFullInterface();
        need_draw = TRUE;
        return NULL;
    }
    // 如电量不足够，就退出。
    if (IsLowPower(GetBattery()) != 0)
    {
        DisplayErrMsg(MPWROUT);
        OpenMusic();
        RePlayStatus = NormalPlay;
        UpDisplayFlag = UpDisplayFlag | up_MPRLmode;
        need_draw = TRUE;
        return NULL;
    }

    //使控件不能出来
    ui_run_realtime = TRUE;
    wavrSendCommand(MC_OPEN, NULL);
    if (!wavrSendCommand(MC_LOAD, (void*) FollowReadTempFileName))
    {
        switch (FS_GetInfo())
        {
            case FS_INFO_SPACE_OUT: //6      表示文件操作或目录操作没有磁盘空间，不能写数据或者扩展子目录
            DisplayErrMsg(MWRIERR);
            break;
            case FS_INFO_DIR_OUT: //7      文件操作根目录目录项满
            DisplayErrMsg(MDIROUT);
            break;

            default:
            break;
        }
        OpenMusic();
        RePlayStatus = ABLoopPlay;
        UpDisplayFlag = UpDisplayFlag | up_MPRLmode;
        need_draw = TRUE;
        return NULL;
    }

    //在开始前把清按键,下面才开始响应按键
    while (ap_get_message() != 0)
    {
        ;//do nothing
    }
    if (!wavrSendCommand(MC_RECORD, NULL))
    {
        DealForExitRec();
        RePlayStatus = FollowAB;
        //need_draw = TRUE;
        return NULL;
    }
    UpDisplayFlag = UpDisplayFlag | up_MPRLmode;
    need_draw = TRUE;
    g_2HZ_CNT = 0;

    while (1)
    {
        result = wavrProceed();

        //if( wavrProceed() == 1 )
        if (result == 1)
        {
            if ((DiskSpace--) <= 200)
            {
                DealForExitRec();
                DisplayErrMsg(MWRIERR);
                RePlayStatus = FollowAB;
                //  SetFullInterface();
                return_to_play = 1;
                return NULL;
            }
        }
        else if (result == -1)
        {
            DealForExitRec();
            DisplayErrMsg(MREADERR);
            OpenMusic();
            RePlayStatus = NormalPlay;
            UpDisplayFlag = UpDisplayFlag | up_MPRLmode;
            need_draw = TRUE;
            return NULL;
        }
        else
        {

        }
        if (need_draw != 0)
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

            case AP_KEY_PLAY | AP_KEY_UP: //进入stop状态
            DealForExitRec();//关闭录音模块
            RePlayStatus = NormalPlay;
            g_FCexitflag = TRUE;
            OpenMusic();
            SetFullInterface();
            need_draw = TRUE;
            return RESULT_UI_STOP2; //选关播放后回到停止界面

            case AP_KEY_AB | AP_KEY_UP:
            case AP_KEY_NEXT | AP_KEY_UP: //进入对比状态
            DealForExitRec();//关闭录音模块
            RePlayStatus = CompareAB;
            return NULL;

            case AP_KEY_MODE | AP_KEY_UP: //进入正常播放状态
            DealForExitRec();//关闭录音模块
            OpenMusic();
            RePlayStatus = NormalPlay;
            SetFullInterface();
            need_draw = TRUE;
            return NULL;

            case AP_KEY_PREV | AP_KEY_UP: //进入ab播放状态
            DealForExitRec();//关闭录音模块
            OpenMusic();
            RePlayStatus = ABLoopPlay;
            SetFullInterface();
            need_draw = TRUE;
            return NULL;

            case AP_MSG_RTC:
            g_result = GetBattery();
            if (IsLowPower(g_result) != 0)
            {
                if ((++LowBatt_CNT) > DEF_RECLOWBATTIME)
                {
                    DealForExitRec();
                    DisplayErrMsg(MPWROUT);
                    OpenMusic();
                    //RePlayStatus = ABLoopPlay;
                    RePlayStatus = NormalPlay;
                    UpDisplayFlag = UpDisplayFlag | up_MPRLmode;
                    need_draw = TRUE;
                    return NULL;
                }
            }
            else
            {
                LowBatt_CNT = 0;
            }

            UpDisplayFlag = UpDisplayFlag | up_musiccurrenttime;
            Show_time_FC();
            ui_show_batt( GetBattery());
            break;

            default:
            key = ap_handle_hotkey(key);
            if (key == RESULT_REDRAW)
            {
                SetFullInterface();
                return_to_play = 1;
                need_draw = TRUE;
            }
            else if (key != 0)
            {
                DealForExitRec();
                RePlayStatus = NormalPlay;
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
        } //key
        if (g_2HZ_CNT > ABPointTimeSize) //check if time out

        {
            DealForExitRec();
            RePlayStatus = FollowAB;
            return NULL;
        }
    } //while(1)
    return NULL;
} //ui_followRec

/*
 ********************************************************************************
 *             uint16 ui_followAB( void )
 *
 * Description : 跟读--------A-B播放子状态
 *
 * Arguments   : void
 *
 * Returns     : int
 *               RESULT_UI_STOP： 表示要进入停止状态，此时所有module已关闭
 *               key: 热健消息
 *               NULL: 表示正常退出
 *
 * Notes       : 在此状态下，按play到stop状态；按mode到play;按NEXT到对比状态；
 *
 ******************************************************** ************************
 */

uint16 ui_followAB(void)
{
    uint16 key;
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
        if (need_draw != 0)
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
            ReturnNotInit = TRUE; //要求进入比较AB时不要初始化
            RePlayStatus = CompareAB;
            return NULL;

            case AP_KEY_MODE | AP_KEY_UP:
            RePlayStatus = NormalPlay;
            return NULL;

            case AP_KEY_PREV | AP_KEY_UP:
            RePlayStatus = ABLoopPlay;
            return NULL;

            case AP_KEY_VOLSUB:
            case AP_KEY_VOLADD:
            case AP_KEY_VOL:
            need_draw = TRUE;
            break;

            case AP_MSG_RTC:
            UpDisplayFlag = UpDisplayFlag | up_musiccurrenttime;
            Show_time_FC(); //显示比特率
            ShowNowTime();
            ui_show_batt( GetBattery());//need_draw = TRUE;
            break;

            default:
            key = ap_handle_hotkey(key);
            if (key == RESULT_REDRAW)
            {
                SetFullInterface();
                return_to_play = 1;
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
        }

        musicpSendCommand(MC_GETSTATUS, (void*) &status_buf);
        if (status_buf.signal == SIGNAL_REACH_B)
        {
            Show_time_FC(); //显示比特率
            RefreshMainScreen();
            need_draw = FALSE;
            musicpSendCommand(MC_STOP, NULL);
            musicpSendCommand(MC_CLOSE, NULL);
            RePlayStatus = FollowRec;
            return NULL;
        }
    }
    return NULL;
}
