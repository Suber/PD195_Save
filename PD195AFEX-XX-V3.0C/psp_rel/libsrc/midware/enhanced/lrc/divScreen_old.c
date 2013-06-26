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
#pragma name(MW_EH_LRC_SUB6)

extern LrcTime_t * pTimeLabel;
extern uint8 TimeRecordCounter;//一共有TimeRecordCounter+1条标签 表示结束的那一条不加
extern lyricSaveFormat_T lyricSave;

void LyricDivScreen(void);
void DivScreenBetween(uchar start, uchar end, bool endFlag);
timeFormat_t getDivScreenTime(uchar start, uchar end, bool endFlag);
void CheckOneScreeTime(timeFormat_t *pDivScreenTime);
void dealBetweenTime(uchar start, uchar end, timeFormat_t divScreenTime);

#pragma renamecode(MW_EH_LRC_SUB6)

void dealBetweenTime(uchar start, uchar end, timeFormat_t divScreenTime)
{
    uchar i;
    for (i = start; i < end; i++)
    {
        //ClearWatchDog();
        pTimeLabel[i].p1s += divScreenTime.p1s;
        if (pTimeLabel[i].p1s >= 10)
        {
            pTimeLabel[i].p1s -= 10;
            pTimeLabel[i].sec += 1;
        }

        pTimeLabel[i].sec += divScreenTime.sec;
        if (pTimeLabel[i].sec >= 60)
        {
            pTimeLabel[i].sec -= 60;
            pTimeLabel[i].min += 1;
        }

        pTimeLabel[i].min += divScreenTime.min;
    }

    return;
}

void CheckOneScreeTime(timeFormat_t *pDivScreenTime)
{
    //最大5.5秒
    if ((pDivScreenTime->min > MAX_ONE_SCREEN_TIME_MIN) || ((pDivScreenTime->min == MAX_ONE_SCREEN_TIME_MIN)
            && (pDivScreenTime->sec >= MAX_ONE_SCREEN_TIME_SEC)) || ((pDivScreenTime->min == MAX_ONE_SCREEN_TIME_MIN)
            && (pDivScreenTime->sec == MAX_ONE_SCREEN_TIME_SEC) && (pDivScreenTime->p1s >= MAX_ONE_SCREEN_TIME_100MS)))
    {
        pDivScreenTime->min = MAX_ONE_SCREEN_TIME_MIN;
        pDivScreenTime->sec = MAX_ONE_SCREEN_TIME_SEC;
        pDivScreenTime->p1s = MAX_ONE_SCREEN_TIME_100MS;
    }
    return;
}

timeFormat_t getDivScreenTime(uchar start, uchar end, bool endFlag)
{
    timeFormat_t divScreenTime;
    bool p1sFlag = FALSE;
    WORD timeStart, timeEnd, betweenTime;
    uchar timeTemp;
    uchar DivScreenNum;

    //ClearWatchDog();
    DivScreenNum = end - start + 1;
    if (FALSE != endFlag)
    {
        //间隔3秒
        divScreenTime.min = 0;
        divScreenTime.sec = LAST_TIME_BETWEEN / DivScreenNum;
        divScreenTime.p1s = ((LAST_TIME_BETWEEN % DivScreenNum) * 10) / DivScreenNum;

    }
    else
    {
        timeStart = pTimeLabel[start - 1].min * 60 + pTimeLabel[start - 1].sec;
        timeEnd = pTimeLabel[end].min * 60 + pTimeLabel[end].sec;
        betweenTime = timeEnd - timeStart;
        //防止30.2 - 20.4 分两屏时，如果先从秒位处理，则毫秒会有负数
        if (pTimeLabel[end].p1s < pTimeLabel[start - 1].p1s)
        {
            p1sFlag = TRUE;
            betweenTime -= 1;
        }
        timeTemp = (uchar) (betweenTime % DivScreenNum);
        betweenTime /= DivScreenNum;

        divScreenTime.min = (uchar) (betweenTime / 60);
        divScreenTime.sec = (uchar) (betweenTime % 60);

        if (FALSE != p1sFlag)
        {
            timeTemp = timeTemp * 10 + 10 + pTimeLabel[end].p1s - pTimeLabel[start - 1].p1s;
        }
        else
        {
            timeTemp = timeTemp * 10 + pTimeLabel[end].p1s - pTimeLabel[start - 1].p1s;
        }

        divScreenTime.p1s = timeTemp / DivScreenNum;

    }

    return divScreenTime;

}

//根据start 和end来分，似乎跟
void DivScreenBetween(uchar start, uchar end, bool endFlag)
{
    timeFormat_t divScreenTime = getDivScreenTime(start, end, endFlag);
    CheckOneScreeTime(&divScreenTime);
    dealBetweenTime(start, end, divScreenTime);
}

void LyricDivScreen(void)
{
    uchar i = 1;
    bool startFlag;
    bool endFlag;
    uchar DivScreenCounter;
    uchar start, end;

    pTimeLabel = (LrcTime_t *) lyricSave.LyricTimeAddrBuffer;
    while (1)
    {
        ClearWatchDog();
        startFlag = FALSE;
        while (i < TimeRecordCounter)
        {
            DivScreenCounter = (uint8)(pTimeLabel[i].p1s & 0xf0);
            if (DivScreenCounter == 0)
            {
                i++;
            }
            else
            {
                startFlag = TRUE;
                break;
            }
        }

        if (FALSE != startFlag)
        {
            start = i;
            pTimeLabel[i].p1s &= 0x0f;
            i++;
        }
        else
        {
            return;
        }

        endFlag = FALSE;
        while (i < TimeRecordCounter)
        {
            DivScreenCounter = (uint8)(pTimeLabel[i].p1s & 0xf0);
            if (DivScreenCounter != 0)
            {
                pTimeLabel[i].p1s &= 0x0f;
                i++;
            }
            else
            {
                endFlag = TRUE;
                break;
            }
        }

        if (FALSE != endFlag)
        {
            end = i;
            i++;
            //deal
            DivScreenBetween(start, end, FALSE);
        }
        else // (i == TimeRecordCounter)
        {
            //deal
            DivScreenBetween(start, end, TRUE);
            return;
        }

    }

}
