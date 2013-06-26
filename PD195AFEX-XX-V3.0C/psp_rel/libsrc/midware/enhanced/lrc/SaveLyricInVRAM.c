/*
 *******************************************************************************
 *                lyric sequencer get functions
 *                    enhanced library
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 注意将Z80的小端改为51的大端
 确保指针是16位
 *
 *******************************************************************************
 */
#include    "ReadLyricFile.h"
#include    <filesys.h>
#include    <stringH.h>
#include	<enhanced.h>

#pragma name(MW_EH_LRC_SUB7)

extern LrcTime_t * pTimeLabel;
extern uint8 TimeRecordCounter;//一共有TimeRecordCounter+1条标签 表示结束的那一条不加
extern lyricSaveFormat_T lyricSave;
extern languageLrc_t LyricLanguageFlag;//语言标志       { 01h: 英语 || 02h: 汉语 }
extern uchar DataBuffer[PAGE_BYTE];////读取歌词，再做时间和歌词内容的分离


bool SaveLyricInVRAM(void);
void adjustContentOffest(void);
bool saveVM(uint16 pDestAddr, uchar * pSourceAddr, uint16 saveNum);
#pragma renamecode(MW_EH_LRC_SUB7)

bool saveVM(uint16 pDestAddr, uchar * pSourceAddr, uint16 saveNum)
{
    bool result;
    //char checkRead[30];
    while (saveNum > PAGE_BYTE)
    {
        //ClearWatchDog();
        memcpy(DataBuffer, pSourceAddr, PAGE_BYTE);

        result = VMWrite(DataBuffer, pDestAddr);
        if (!result)
        {
            return FALSE;
        }
        pSourceAddr += PAGE_BYTE;
        saveNum -= PAGE_BYTE;
        pDestAddr += PAGE_BYTE;

    }

    memcpy(DataBuffer, pSourceAddr, saveNum);
    memset(DataBuffer + saveNum, 0, (PAGE_BYTE - saveNum));

    result = VMWrite(DataBuffer, pDestAddr);
    //	result = VMRead(checkRead, pDestAddr, 30);
    if (!result)
    {
        return FALSE;
    }
    return TRUE;
}

void adjustContentOffest(void)
{
    uchar i;
    uint16 timeBase;
    //ClearWatchDog();
    pTimeLabel = (LrcTime_t *) lyricSave.LyricTimeAddrBuffer;
    timeBase = *(uint16 *) (&(pTimeLabel->buffaddr_High));
    for (i = 0; i < TimeRecordCounter; i++)
    {
        *(uint16 *) (&(pTimeLabel[i].buffaddr_High)) -= timeBase;
    }
}

bool SaveLyricInVRAM(void)
{
    uint16 saveNum, i = 0;
    uint8 * pSourceAddr;
    uint16 pDestAddr;
    bool result;
    TimeRecordCounter += 1; //加上最后一条
    lyricSave.lyricPageMode = (uchar) LyricLanguageFlag;
    adjustContentOffest();
    //save time label
    saveNum = LYRIC_FILENAME_SIZE + LYRIC_PAGEMODE_SIZE + TimeRecordCounter * 5;
    pSourceAddr = lyricSave.lyricFileName;
    pDestAddr = (uint16) LYRIC_FILENAME_ADDR;
    result = saveVM(pDestAddr, pSourceAddr, saveNum);
    if (!result)
    {
        return FALSE;
    }

    //save content
    saveNum = *(uint16 *) (&pTimeLabel[TimeRecordCounter - 1].buffaddr_High);
    pSourceAddr = lyricSave.LyricStrBuffer;
    pDestAddr = (uint16) CONTENT_TABLE_ADDR;
    result = saveVM(pDestAddr, pSourceAddr, saveNum);
    if (!result)
    {
        return FALSE;
    }
    //	VMRead(DataBuffer, 0xa800, 512);
    //	VMRead(DataBuffer, 0xaa00, 512);
    return TRUE;
}
