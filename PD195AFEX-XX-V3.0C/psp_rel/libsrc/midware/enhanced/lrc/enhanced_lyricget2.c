/*
 *******************************************************************************
 *                lyric sequencer get functions
 *                    enhanced library
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 注意将Z80的小端改为51的大端
 *
 *******************************************************************************
 */
#pragma name(MW_EH_LRC_API3)

#include <actos.h>
#include <enhanced.h>
#include "ReadLyricFile.h"
//#include "debug.h"
//keil 51是大端，与小端是有区别的
//lyricNextTime lyricCurTime这两个变量是需要维护的

//vram lyric buf 数据结构
//时间标签
//record items:
//    *************************************************
//    *** minute|second|100ms|content table address ***
//    *** Byte0  BYTE1  BYTE2   BYTE3    BYTE4      ***
//    *************************************************
//    是大端，所以byte3是高字节地址，byte4是低字节地址
//    minute为0xff时表示时间标签结束

//歌词内容信息
//record items:
//    *************************************************
//    *** Line1 |0x0a|Line2 |0x0a|...|Linen |0x0d| ***
//    *** string|'\a'|string|'\a'|...|string|'\d'| ***
//    *************************************************
//               换行        换行            换屏


extern uint16 lyricPageNo;
extern uint32 lyricCurTime;
extern uint32 lyricNextTime;

#pragma renamecode(MW_EH_LRC_API3)

extern BOOL lyricGetByNo(char * strLyric, uchar len, uint32 *ptime, uint16 No);
/********************************************************************************
 * Description : Get the next lyric page
 *
 * Arguments  :
 strLyric:	lyric page string
 len:		the max length of lyric page
 * Returns     :
 TRUE:   Successful.
 FALSE:  Fail,  next lyric page don't exist
 * Notes       :  exist a problem that don't differ Flash R/W Error with exceed the tail.
 *
 ********************************************************************************/
#ifndef PC
bool slyricGetNext(BYTE * strLyric, uchar len)
#else
bool lyricGetNext(BYTE * strLyric, uchar len)
#endif
{
    uint32 time;

    if (len == 0)
    {
        return FALSE;
    }

    if (!lyricGetByNo(strLyric, len, &time, lyricPageNo + 1))
    {
        return FALSE;
    }

    lyricPageNo++;
    lyricCurTime = time;

    if (!lyricGetByNo(NULL, 1, &time, lyricPageNo + 1))
    {
        lyricNextTime = MAX_TIME;
    }
    else
    {
        lyricNextTime = time;
    }

    return TRUE;

}

/********************************************************************************
 * Description : Get the prev lyric page
 *
 * Arguments  :
 strLyric:	lyric page string
 len:		the max length of lyric page
 * Returns     :
 TRUE:   Successful.
 FALSE:  Fail,  next lyric page don't exist
 * Notes       :  exist a problem that don't differ Flash R/W Error with exceed the head.
 *
 ********************************************************************************/
#ifndef PC
bool slyricGetPrev(BYTE * strLyric, uchar len)
#else
bool lyricGetPrev(BYTE * strLyric, uchar len)
#endif
{
    uint32 time;

    if ((lyricPageNo == 0) || (len == 0))
    {
        return FALSE;
    }

    if (!lyricGetByNo(strLyric, len, &time, lyricPageNo - 1))
    {
        return FALSE;
    }

    lyricPageNo--;
    lyricNextTime = lyricCurTime;
    lyricCurTime = time;
    return TRUE;
}
