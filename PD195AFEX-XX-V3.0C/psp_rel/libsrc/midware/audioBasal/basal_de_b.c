/********************************************************************************
 *                              USDK 5102
 *                            Module: POWER MANAGER
 *                 Copyright(c) 2001-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      wuyueqian     2009-9-07 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * file
 * brief
 * Author   wuyueqian
 * par
 *
 *      Copyright(c) 2001-2007 Actions Semiconductor, All Rights Reserved.
 *
 * version 1.0
 * date  2008/6/12
 *******************************************************************************/
#include "basal.h"
#include "decC.h"

#ifdef PC
extern void decAudioIntSub(void);
extern void decDMAIntSub(void);
extern void decTimerIntSub(void);
#endif

#pragma name(MW_BS_AD_CMD1)

#define DECODE_BUFFER_ADDRESS_NORMAL 0x0600//0x1800
#define WRONG_DECODE_FILE_TYPE 0xf0

extern HANDLE m_deFileHandle;
extern Open_param_t*m_deOpenparam;//参数表地址

extern uint32 deBpointerSave;
extern uint32 deBreakPointSave;
extern decBreakPointInfor_t m_breakPointInfo;
extern uint32 deApointerSave;
extern uint32 deFilepages;
extern BYTE m_backupmode;
extern play_status_t play_status;
extern bool m_deABok_sav;
extern BYTE * m_deDataBuffer;
extern uint16 m_deOldDspIntAddrSav;
extern uint16 m_deOldDmaIntAddrSav;
extern time_t m_Totaltimelength;
extern uint8 m_decTimer_Sav;

bool LoadCommand(BYTE music_type, void* param);
bool StopCommand(void* param);
bool PlayCommand(void* param);
bool CaltimeCommand(void* param);
bool RestoreCommand(void);
bool BackupCommand(void);
bool APointPlayCommand(void);

#ifndef PC
#pragma ROM(large)
extern uint8 m_deRead_Data(uchar readFileDirection, DWORD position);
#pragma ROM(huge)
#else
extern uint8 m_deRead_Data(uchar readFileDirection, DWORD position);
#endif

#pragma renamecode(MW_BS_AD_CMD1)

/*************************************************************************
 * LoadCommand 初始化codec用的全局变量g_decInitPara，
 * 包括codec的buffer、dma中断的hook函数、歌曲文件的第一帧数据地址等等
 ****************************************************************************/
bool LoadCommand(BYTE music_type, void* param)
{
    BankSwitch(BYTE2_OF(m_deRead_Data));
    BankSwitch(BYTE2_OF(decAudioIntSub));

    m_deDataBuffer = (BYTE*) DECODE_BUFFER_ADDRESS_NORMAL;

    g_decReadFilePara.ReadFileHandleHook = (DWORD) m_deRead_Data;
    g_decReadFilePara.ReadFileBuffAddr = (WORD) (m_deDataBuffer);
    g_decControlInfor.SongType = music_type;

    if (m_deOpenparam != NULL)
    {
        g_decSongInfor.FileHDRLen = m_deOpenparam->musicframeoffset;
        memcpy(&m_Totaltimelength, &(m_deOpenparam->musictotaltime), sizeof(time_t));
    }
    else
    {
        g_decSongInfor.FileHDRLen = 512;
    }

    //play_status.speed=0x00;
    m_deABok_sav = FALSE;

    //g_decControlInfor.SoftVolumeMax = 41;	 //由上层去设定，否则每次调用会修改之前设置过的值
    //g_decControlInfor.Volume = 23;

    g_decControlInfor.ChannelSelect = 0;//双通道

    g_decControlInfor.TimeLimit = 50; //auto ab play
    g_decControlInfor.EnergyLimit = 8; //auto ab play

    play_status.status = PLAYING_STOP;

    m_backupmode = 0;

    if (m_deOpenparam != NULL)
    {
        m_deFileHandle = FS_FOpen(m_deOpenparam->filename, FS_OPEN_NORMAL);
    }
    else
    {
        m_deFileHandle = FS_FOpen((uint8*) param, FS_OPEN_NORMAL);
    }

    if (m_deFileHandle == NULL)
    {
        play_status.status = PLAYING_ERROR;
        return FALSE;
    }

    {
        uint32 EndPageLen;//最后一个PAGE的字节数
        EndPageLen = (uint32)FS_GetUnalignedLen(m_deFileHandle);//取最后一个PAGE的BYTE数
        deFilepages = (uint32)FS_GetFileLen(m_deFileHandle);
        g_decReadFilePara.FileLen = (((uint32)deFilepages)<<9); //* 512;

        //处理文件尾不是整PAGE的数据,一页一页传送
        if (EndPageLen != 0)
        {
            g_decReadFilePara.FileLen = g_decReadFilePara.FileLen - 512 + EndPageLen;
        }
    }

    if (g_decReadFilePara.FileLen == 0)
    {
        play_status.status = PLAYING_ERROR;
        return FALSE;
    }

    return TRUE;

}

/*************************************************************************
 * CaltimeCommand 通过AUDIO IP来计算歌曲的时间
 *现在WMA WAV MP3均可以通过软件来计算总时间
 ****************************************************************************/

bool CaltimeCommand(void* param)
{

    param = param;
    if (play_status.status == PLAYING_ERROR)
    {
        return FALSE;
    }
    g_decControlInfor.PlayMode = PLAY_MODE_CALTIME; //  codec 会根据这个进行配置
    decInit();

    m_deOldDspIntAddrSav = IRQ_Intercept((uint) &(decAudioIntSub), IRQ_DSP);
    m_deOldDmaIntAddrSav = IRQ_Intercept((uint) &(decDMAIntSub), IRQ_DMA012);
    m_decTimer_Sav = TM_SetTimer((int16) decTimerIntSub, 2);

    decstart();
    do
    {
        ClearWatchDog();
        if (g_decCurStatus.Status >= WRONG_DECODE_FILE_TYPE)
        {
            play_status.status = PLAYING_ERROR;
            break;
        }

    } while (g_decCurStatus.GetSongInfo != 0xff);//while(0);

    memcpy(&m_Totaltimelength, &(g_decSongInfor.TotalPlayTime.TimeHour), sizeof(time_t));

    decExit();
    TM_KillTimer(m_decTimer_Sav);
    IRQ_Release(m_deOldDspIntAddrSav, IRQ_DSP);
    IRQ_Release(m_deOldDmaIntAddrSav, IRQ_DMA012);

    return TRUE;

}

/*************************************************************************
 * StopCommand 停止播放
 *1 g_decReadFilePara.ReadFilePages在dma hook中被赋值 = deBreakPointSave
 ****************************************************************************/

bool StopCommand(void* param)
{
    m_backupmode = (uint8) param;
    while (!decExit())
    {
        ; //qac
    }        
    //g_decBreakPointInfor.BreakPointAddr = g_decReadFilePara.ReadFileCurAddr;//保存文件断点信息
    TM_KillTimer(m_decTimer_Sav);
    IRQ_Release(m_deOldDspIntAddrSav, IRQ_DSP);
    IRQ_Release(m_deOldDmaIntAddrSav, IRQ_DMA012);

    play_status.status = PLAYING_STOP;
    return TRUE;
}

/*************************************************************************
 * PlayCommand 播放
 *
 ****************************************************************************/
bool PlayCommand(void* param)
{
    if (play_status.status == PLAYING_ERROR)
    {
        return FALSE;
    }
    if (play_status.status == PLAYING_PAUSE)
    {
        decContinue();
        play_status.status = PLAYING_PLAYING;
        return TRUE;
    }

    play_status.status = PLAYING_PLAYING;//默认为播放状态
    if ((uint8) param == ABPlay_open)
    {
        RestoreCommand();
        //如果deApointerSave = deFilepages表示最后一页，不用再改
        if ((deApointerSave < deFilepages))
        {
            play_status.status = PLAYING_AB;
            g_decControlInfor.ABSetFlag = AB_FLAG_DEAL_AB_PROCESS;//为了进入RD 读文件时就检测该断点是否超过B点了
            memcpy(&g_decBreakPointInfor, &m_breakPointInfo, sizeof(decBreakPointInfor_t));
            g_decControlInfor.PlayMode = PLAY_MODE_BREAK;
        }
        //回到文件头后，codec会根据deApointerSave来seek，并读取数据
        FS_FSeek(0, 0, m_deFileHandle);
    }
    else if ((uint8) param == Nor_Break_open)
    {
        RestoreCommand();
        if (deBreakPointSave < deFilepages)
        {
            g_decControlInfor.PlayMode = PLAY_MODE_BREAK;
            memcpy(&g_decBreakPointInfor, &m_breakPointInfo, sizeof(decBreakPointInfor_t));
        }
        FS_FSeek(0, 0, m_deFileHandle);
    }
    else
    {
        //要从头开始放
        g_decControlInfor.PlayMode = PLAY_MODE_NORMAL;
        FS_FSeek(0, 0, m_deFileHandle);
    }

    if (m_deOpenparam == NULL)
    {
        g_decControlInfor.PlayMode = PLAY_MODE_AB;//若参数表地址为空则表示为跟读对比播放
    }

    decInit();

    m_deOldDspIntAddrSav = IRQ_Intercept((uint) &(decAudioIntSub), IRQ_DSP);
    m_deOldDmaIntAddrSav = IRQ_Intercept((uint) &(decDMAIntSub), IRQ_DMA012);
    m_decTimer_Sav = TM_SetTimer((int16) decTimerIntSub, 2);
    decstart();
    return TRUE;
}

//other bank combine
/*

 bool BackupCommand(void)
 {
 m_deOpenparam->BreakPTSave->ApointSave = deApointerSave;
 m_deOpenparam->BreakPTSave->BpointSave = deBpointerSave;
 m_deOpenparam->BreakPTSave->breakPT = deBreakPointSave;
 if(m_backupmode == 0)//断点续播备份
 {
 memcpy(&(m_deOpenparam->BreakPTSave->breakPointInfor), &g_decBreakPointInfor, sizeof(decBreakPointInfor_t));
 }
 else//AB 复读过程中的断点不保存，只是保存A点，以后断点续播从A点开始
 {
 memcpy(&(m_deOpenparam->BreakPTSave->breakPointInfor), &m_breakPointInfo, sizeof(decBreakPointInfor_t));
 }
 return TRUE;
 }

 bool RestoreCommand(void)
 {
 deApointerSave = m_deOpenparam->BreakPTSave->ApointSave;
 deBpointerSave = m_deOpenparam->BreakPTSave->BpointSave;
 deBreakPointSave = m_deOpenparam->BreakPTSave->breakPT;
 memcpy(&m_breakPointInfo, &(m_deOpenparam->BreakPTSave->breakPointInfor), sizeof(decBreakPointInfor_t));
 return TRUE;
 }

 bool APointPlayCommand(void)
 {
 g_decControlInfor.PlayMode = PLAY_MODE_BREAK;
 memcpy(&g_decBreakPointInfor, &m_breakPointInfo, sizeof(decBreakPointInfor_t));
 FS_FSeek(0, FS_SEEK_FFROMSTART, m_deFileHandle);
 decInit();//计算时间不淡入
 decstart();
 return TRUE;
 }

 */
