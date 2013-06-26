/********************************************************************************
 *                              USDK 5102
 *                            Module: POWER MANAGER
 *                 Copyright(c) 2001-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    wuyueqian     2009-9-07 9:42     1.0             build this file
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

#include <basal.h>
#include <decC.h>

#pragma name(MW_BS_AD_CMDR)

HANDLE m_deFileHandle;
Open_param_t *m_deOpenparam;
uint32 deApointerSave;
uint32 deBpointerSave;
uint32 deBreakPointSave; //当前文件断点的地址保存，一个扇区为单位，m_read_date更新
decBreakPointInfor_t m_breakPointInfo;//只保存一次断点信息，例如A点
uint32 deFilepages;//当前播放文件总页数
BYTE m_backupmode;
play_status_t play_status;
BYTE * m_deDataBuffer;//[512];//解码时用的BUFFER
uint16 m_deOldDspIntAddrSav;//保存旧的INT地址
uint16 m_deOldDmaIntAddrSav;
uint8 m_decTimer_Sav;
time_t m_Totaltimelength;
bool m_deABok_sav;

#define READ_SECTOR_BYTE_NUMBER 512
#define MOVE_BACKWARD_ONE_PAGE_FLAG  0x22
#define MOVE_FORWARD_ONE_PAGE_FLAG 0x11
#define RANDOM_CHOOSE_ONE_PAGE_FLAG 0x33

#define ONE_PAGE_BITNUM 9

#ifndef PC
#pragma ROM(large)
void GetFSErrorTYPE(void);
#pragma ROM(huge)
#else
void GetFSErrorTYPE(void);
decBreakPointInfor_t g_decBreakPointInfor;
#endif

#pragma renamecode(MW_BS_AD_CMDR)

#pragma ROM(large)
uint8 m_deRead_Data(uchar readFileDirection, uint32 position)
{
    bool result = TRUE;
    uint32 posPage = 0;
    uint32 posDistance = 0;
    //CLEAR_WATCHDOG_MIDWARE();
    ClearWatchDog();

    //断点记录的是解码过的信息

    if ((play_status.status == PLAYING_REACH_END) || (play_status.status == PLAYING_ERROR) || 
            (play_status.status == PLAYING_REACH_HEAD))
    {
        return 0xff;
    }

    deBreakPointSave = (uint32)(FS_FTell(m_deFileHandle)); //- 1;

    if (deBreakPointSave == deFilepages)
    {
        play_status.status = PLAYING_REACH_END;
        return 0xff;
    }

    if (MOVE_BACKWARD_ONE_PAGE_FLAG == readFileDirection)
    {
        result = FS_FSeek(2, FS_SEEK_BFROMCUR, m_deFileHandle);
    }
    else if (RANDOM_CHOOSE_ONE_PAGE_FLAG == readFileDirection)
    {
        posPage = position >> ONE_PAGE_BITNUM;
        if (posPage > deBreakPointSave)
        {
            result = FS_FSeek((int32)(posPage - deBreakPointSave), FS_SEEK_FFROMCUR, m_deFileHandle);
        }
        else if (posPage < deBreakPointSave)
        {
            posDistance = (deBreakPointSave) - posPage;
            if (posDistance < (deBreakPointSave << 1))
            {
                result = FS_FSeek((int32)(posDistance), FS_SEEK_BFROMCUR, m_deFileHandle);
            }
            else
            {
                result = FS_FSeek((int32)(posPage), FS_SEEK_FFROMSTART, m_deFileHandle);
            }

        }
        else
        {		
        }
    }
    else
    {		
    }

    if (FALSE == result)
    {
        GetFSErrorTYPE();
        return FALSE;
    }

    switch (g_decControlInfor.ABSetFlag)
    {
        case AB_FLAG_DEAL_AB_PROCESS:
        {
            if (play_status.status == PLAYING_WAIT_B) //已经到达B点或者超过，等待stop命令
            {
                break;
            }

            if ((play_status.status == PLAYING_AB) && (deBreakPointSave >= deBpointerSave))//放到B点
            {
                if (FALSE != m_deABok_sav) //不用再播放了
                {//次数到,清B点
                    m_deABok_sav = FALSE;
                    play_status.status = PLAYING_PLAYING;
                    g_decControlInfor.ABSetFlag = AB_CLEAR;
                    g_decControlInfor.PlayMode = PLAY_MODE_NORMAL; //not need
                    //play_status.signal = SIGNAL_CLEAR_B;//通知AB要清B点
                    play_status.signal = 0;//modify by wuyueqian 2010-4-3
                }
                else
                {//次数未到,通知AP回到A点
                    play_status.status = PLAYING_WAIT_B;
                    play_status.signal = SIGNAL_REACH_B;
                }
            }
            break;
        }

        case AB_FLAG_SET_A: //设置完A点后ab flag为0，不进行AB处理，需等B点也设好后才开始AB播放处理
        {
            deApointerSave = deBreakPointSave;
            g_decControlInfor.ABSetFlag = AB_CLEAR;//其实非AB_FLAG_SET_A和非0XFF就可以了
            memcpy(&m_breakPointInfo, &g_decBreakPointInfor, sizeof(decBreakPointInfor_t));
            break;
        }

        case AB_FLAG_SET_B:
        {
            deBpointerSave = deBreakPointSave;
            play_status.signal = SIGNAL_REACH_B;
            play_status.status = PLAYING_WAIT_B; //只在这个m_deRead_Data函数中有效，StopBPlay后出变成PLAYING_WAIT_A
            g_decControlInfor.ABSetFlag = AB_FLAG_DEAL_AB_PROCESS; //以后检测是否到B点
            break;
        }

        default:
        {
            break;
        }

    }

    result = FS_FRead(m_deDataBuffer, 1, m_deFileHandle);

    deBreakPointSave = (uint32)(FS_FTell(m_deFileHandle));
    g_decReadFilePara.ReadFileCurAddr = (deBreakPointSave - 1) * READ_SECTOR_BYTE_NUMBER;

    if (result == 0)
    {
        GetFSErrorTYPE();
        return FALSE;
    }
    return TRUE;
}

/*  FS_GetInfo()
 *描述
 获取文件系统出错信息
 *参数
 *返回
 char status：出错信息
 1   磁盘读写错误
 2   磁盘写保护
 3   磁盘未格式化
 4   文件操作超出文件边界,目录操作超出目录边界
 5   文件操作的目标文件,目录操作的目录项不存在
 6   表示文件操作时没有磁盘空间,不能写数据或者扩展目录
 表示目录操作时没有磁盘空间,不能扩展目录,新建子目录
 7   文件操作时根目录目录项满
 8   删除目录时返回,表示删除的目录非空
 */

void GetFSErrorTYPE(void)
{
    char result;

    result = FS_GetInfo();
    switch (result)
    {
        case FS_INFO_OVEFLOW: //FIX ME
        {
            if (FS_FTell(m_deFileHandle) == 0)
            {
                play_status.status = PLAYING_REACH_HEAD;
            }
            else
            {
                play_status.status = PLAYING_REACH_END;
            }
            break;
        }

        case FS_INFO_RW_ERR:
        {
            play_status.status = PLAYING_ERROR;
            break;
        }

        default:
        {
            break;
        }
    }
}

#ifndef PC
#pragma ROM(huge)
#endif
