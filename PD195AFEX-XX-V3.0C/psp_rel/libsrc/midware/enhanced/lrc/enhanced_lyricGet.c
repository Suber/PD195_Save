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
#pragma name(MW_EH_LRC_API2)

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

const int16 m_mstable[10] =
{ 0, 100, 200, 300, 400, 500, 600, 700, 800, 900 };

extern uint16 lyricPageNo;
extern uint32 lyricCurTime;
extern uint32 lyricNextTime;

BOOL lyricGetByNo(char * strLyric, uchar len, uint32 *ptime, uint16 No);
bool VMReadU(char *pbuf, uint16 address, uint16 len);

#pragma renamecode(MW_EH_LRC_API2)
/********************************************************************************
 * Description : Get a lyric page by lyric page No
 *
 * Arguments  : 获取指定一页歌词到strLyric中
 strLyric:   lyric page string.
 len:         max length of lyric page string.
 ptime:	the time of lyric page.
 No:		lyric page No
 * Returns     :
 TRUE: Successful
 FALSE: fail.
 获取的时间 分钟（BYTE） 时钟(BYTE) 毫秒 WORD
 * Notes       :  None.    strLyric为空时是获取时间
 *
 ********************************************************************************/
BOOL lyricGetByNo(char * strLyric, uchar len, uint32 *ptime, uint16 No)
{

    uint8 TimeRec[TIME_RECORD_SIZE];
    uint16 pReadPos;

    ClearWatchDog();
    if ((len == 0) || (No == 0))
    {
        return FALSE;
    }

    if (!VMReadU(TimeRec, (TIME_TABLE_ADDR) + (No - 1) * (TIME_RECORD_SIZE), TIME_RECORD_SIZE))
    {
        return FALSE;
    }

    //minute == 0xff means the last record
    if (0xff == TimeRec[MINUTE_INDEX])
    {
        return FALSE;
    }

    //long是4个字节,这里Z80是小端
    *((int16 *) ptime) = *((int16 *) &TimeRec[MINUTE_INDEX]); //已经取了两个自己了
    *(((int16 *) ptime) + 1) = m_mstable[(TimeRec[MILSECONDE_INDEX]) & 0x0f];
    #ifdef PC
	SIM_ChgLongPC((uint8 *)ptime);
    #endif
    //如果strLyric是空，则表明该函数只是想获取时间，无需拷贝该页歌词
    if (strLyric == NULL)
    {
        return TRUE;
    }

    // read content
    pReadPos = *((uint16 *) (&TimeRec[CONTENT_IDNEX]));
    if (!VMReadU(strLyric, (CONTENT_TABLE_ADDR) + pReadPos, len))
    {
        return FALSE;
    }

    return TRUE;
}

/********************************************************************************
 * Description : Seek for specifying lyric page by time
 *
 * Arguments  :  根据时间来获取歌词，并将该页号更新在该时间点
 歌词时间与显示页号的关系
 如果 第一页 时间是 01 02 3
 第二页 时间是 02 03 4
 第三页 时间是 03 05 6
 现在获得的时间是 02 5 3 那应该是显示第二页的内容
 strLyric:   lyric page string.
 len:         max length of lyric page string.
 lTime:	the time of specifying lyric page.
 * Returns     :
 TRUE: Successful
 FALSE: fail.
 * Notes       :  None.
 *
 ********************************************************************************/
#ifndef PC
bool slyricSeekFor(char *strLyric, const uchar len, const uint32 lTime)
#else
bool lyricSeekFor( char *strLyric, const uchar len, const uint32 lTime)
#endif
{
    uint16 n;
    uint32 time;

    ClearWatchDog();
    if ((len == 0) || (lTime >= MAX_TIME))
    {
        return FALSE;
    }

    if (lTime == 0)
    {
        lyricPageNo = 0;
        return lyricGetNext(strLyric, len);
    }

    //表示已经读到的页数，从1开始。假如是是2，表示已经读了第二页了

    n = lyricPageNo;
    if (lTime >= lyricNextTime)
    {
        do
        {
            n++;
        } while ((lyricGetByNo(NULL, 1, &time, n)) 
                && (lTime >= time) 
                && (n < ((TIME_TABLE_SIZE) / (TIME_RECORD_SIZE))));

        //应该是显示上一页
        if (!lyricGetByNo(strLyric, len, &lyricCurTime, n - 1))
        {
            return FALSE;
        }

        lyricPageNo = n - 1;
        lyricNextTime = time;
    }
    else if (lTime < lyricCurTime) //快退播放时会有这样的需求
    {
        if (n <= 1)//比第一页显示的时间小的时间点是无法显示的
        {
            return FALSE;
        }

        do
        {
            n--;
            if (!lyricGetByNo(strLyric, len, &time, n))
            {
                return FALSE;	
            }                
        } while ((n > 1) && (lTime < time));

        lyricCurTime = time;
        lyricPageNo = n;

        if (!lyricGetByNo(NULL, 1, &time, lyricPageNo + 1))
        {
            lyricNextTime = MAX_TIME;
        }
        else
        {
            lyricNextTime = time;
        }
    }
    else
    {
        return lyricGetByNo(strLyric, len, &time, lyricPageNo);
    }
    return TRUE;

}

/********************************************************************************
 * Description : Query if a time is out of the duration of current lyric page
 *
 * Arguments  :  查询需要显示的时间是否超出现在所显示的范围内
 lTime:	the time to queryed
 * Returns     :
 TRUE:  the time is out of.
 FALSE: don't
 * Notes       :  the function would be called frequent, should fast.
 *
 ********************************************************************************/
#ifndef PC
BOOL slyricQueryOutof(uint32 lTime)
#else
BOOL lyricQueryOutof(uint32 lTime)
#endif
{
    return ((lTime >= lyricNextTime) || (lTime < lyricCurTime));
}

/*
 void setTempBufferClk1(uint8 * psfrBak, uint8 *pmembankBak, uint8 *pmscrBak)
 {
 *psfrBak = SFR_BANK;
 *pmembankBak = MemBankCtl;
 *pmscrBak = MCSR5;

 SFR_BANK = BANK_CMU_RMU;
 MemBankCtl |= 0x08; //PCMRAM IN DATA SEGMENT
 MCSR5 |=0x04; //pcm in dataSeg
 MCSR5 &=0xfd;

 }

 void storeTempBufferClk1(uint8 * psfrBak, uint8 *pmembankBak, uint8 *pmscrBak)
 {
 SFR_BANK = *psfrBak;
 MemBankCtl =*pmembankBak;
 MCSR5 = *pmscrBak;
 }
 */

/********************************************************************************
 * Description : VM Reader fixed the sector bound limits
 *
 * Arguments  :
 pbuf:  the buf to store reading data
 address: the address of reading data
 len:		the length of reading data
 * Returns     :
 TRUE:  successful
 FALSE: fail
 * Notes       :  None
 *
 ********************************************************************************/
bool VMReadU(char *pbuf, uint16 address, uint16 len)
{
    uint16 t;

    t = 512 - (address & 0x1ff);
    if (len > t)//跨越两页
    {
        if (!VMRead(pbuf, address, t))
        {
            return FALSE;
        }
        len -= t;
        pbuf += t;
        address += t;
    }

    return VMRead(pbuf, address, len);
}
