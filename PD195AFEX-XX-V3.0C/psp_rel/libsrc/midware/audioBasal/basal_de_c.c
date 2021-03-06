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

#pragma name(MW_BS_AD_CMD2)

extern HANDLE m_deFileHandle;
extern Open_param_t*m_deOpenparam;//参数表地址

extern uint32 deApointerSave;
extern uint32 deBpointerSave;
extern uint32 deBreakPointSave;
extern decBreakPointInfor_t m_breakPointInfo;
extern BYTE m_backupmode;

#pragma renamecode(MW_BS_AD_CMD2)
/*
 void reserve_function(void)
 {

 }
 */
bool BackupCommand(void)
{
    m_deOpenparam->BreakPTSave->ApointSave = deApointerSave;
    m_deOpenparam->BreakPTSave->BpointSave = deBpointerSave;
    m_deOpenparam->BreakPTSave->breakPT = deBreakPointSave;
    if (m_backupmode == 0)//断点续播备份
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
