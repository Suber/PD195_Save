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

#pragma name(MW_EH_LRC_SUB5)

//edata
extern LrcTime_t * pTimeLabel;
extern uint8 TimeRecordCounter;//一共有TimeRecordCounter+1条标签 表示结束的那一条不加
extern lyricSaveFormat_T lyricSave;

extern uint8 offset_p1s;//0.1s值   //应该搞成一个数据结构
extern uint8 offset_sec;//秒值
extern bool positiveflag;//正号标志


void HandleLastLyric(void);
void HandleID3Overlap(void);
void SortLyricData(void);
extern void AddOffsetData(LrcTime_t *timetab, BYTE recordCNT);
extern bool SaveLyricInVRAM(void);
extern void LyricDivScreen(void);
void AddOffsetData(LrcTime_t *timetab, BYTE recordCNT);

#pragma renamecode(MW_EH_LRC_SUB5)

void SortLyricData(void)
{
    uchar i, j;
    LrcTime_t timeData;
    pTimeLabel = (LrcTime_t *) lyricSave.LyricTimeAddrBuffer;
    for (i = 1; i <= TimeRecordCounter; i++)
    {
        ClearWatchDog();
        for (j = i; j > 0; j--)
        {
            //ClearWatchDog();
            if (pTimeLabel[j].min > pTimeLabel[j - 1].min)
            {
                break;//continue;
            }
            else if ((pTimeLabel[j].min == pTimeLabel[j - 1].min) && (pTimeLabel[j].sec > pTimeLabel[j - 1].sec))
            {
                break;//continue;
            }
            else if ((pTimeLabel[j].min == pTimeLabel[j - 1].min) && (pTimeLabel[j].sec == pTimeLabel[j - 1].sec)
                    && ((pTimeLabel[j].p1s & 0x0f) >= (pTimeLabel[j - 1].p1s & 0x0f)))
            {
                break;//continue;
            }
            else//一定要小于才交换，保证歌词顺序
            {
                memcpy(&timeData, pTimeLabel + j, sizeof(LrcTime_t));
                memcpy(pTimeLabel + j, pTimeLabel + j - 1, sizeof(LrcTime_t));
                memcpy(pTimeLabel + j - 1, &timeData, sizeof(LrcTime_t));
            }
        }
    }
    return;
}

//处理最后行歌词分屏间隔太小的问题   最后一行分屏没有参考点，每条加2秒，
void HandleLastLyric(void)
{
    pTimeLabel = (LrcTime_t *) lyricSave.LyricTimeAddrBuffer;
    pTimeLabel += (TimeRecordCounter - 1); //修改的是ff aa 55 最后添加的那一条
    if ((pTimeLabel->p1s & 0xf0) != 0) //TimeRecordCounter 为有效时间条码数，+1 表示加上FF AA 55这一条
    {
        if (pTimeLabel->sec < 58)
        {
            pTimeLabel->sec += 2;//加2秒，最后一行没有参考点
            return;
        }
        else if (pTimeLabel->min < MAX_MINUTE)
        {
            pTimeLabel->sec -= 58;
            pTimeLabel->min += 1;
            return;
        }
        else
        {
        }
    }
    else
    {
        return;//means 只有一条对应
    }
}

void HandleID3Overlap(void)
{
    //id3 最多90   /16  =  6 条记录
    uchar i = 0;
    pTimeLabel = (LrcTime_t *) lyricSave.LyricTimeAddrBuffer;
    pTimeLabel++;////第一条时间标签一定是ID3，从第二条开始搜索
    ////ID3长可能有很多条时间标签，从第二条起毫秒高位代表显示为哪一屏
    while (((pTimeLabel->p1s) & 0xf0) != 0)
    {
        pTimeLabel++;
    }

    //当不为0时表示到了第一条歌词的开始处
    //是0的都设置为1秒，以后会根据codec的时间来取值，所以时间不对也没有关系


    while (i < 10) //最多检测10条
    {
        if (((pTimeLabel->sec) > 0) || ((pTimeLabel->min) > 0))
        {
            return;
        }
        pTimeLabel->sec = 1; //都将秒钟设为1 保证ID3显示至少为1秒 毫秒不需要改了
        pTimeLabel++;
        i++;
    }
    return;
}

void AddOffsetData(LrcTime_t *timetab, BYTE recordCNT)
{
    BYTE i;//进位记录
    BYTE j;

    if ((offset_p1s == 0) && (offset_sec == 0))
    {
        //如要加入的时间为0，则不要加
        return;
    }

    if (FALSE == positiveflag)
    {
        //为正数
        for (; recordCNT > 0; recordCNT--)
        {
            //ClearWatchDog();
            if (((timetab->sec != 0) || (timetab->min != 0)) || (timetab->p1s != 0))
            {
                //100ms(0.1s)数位加
                j = offset_p1s + timetab->p1s;
                i = 0;
                if (j >= 10)
                {
                    j = j - 10;
                    i = 1;
                }
                timetab->p1s = j;

                //秒数位加
                j = offset_sec + i + timetab->sec;
                i = 0;
                if (j >= 60)
                {
                    j = j - 60;
                    i = 1;
                }
                timetab->sec = j;

                //分位数加
                timetab->min += i;
            }
            timetab++; //指针加一
        }
    }
    else
    { //为负数

        BYTE tmpbuf1[4];
        BYTE tmpbuf2[4];

        //offset_sec有可能大于60，可能超过1分钟，只是小于100
        //时间标签的秒钟已经调整了，其超过60会向分钟进位，所以
        //当偏移是80秒时，原来时间为1分钟 10秒 100毫秒时，
        //比较仍然不成立，但却是错误的
        //tmpbuf1[0] = offset_p1s;
        //tmpbuf1[1] = offset_sec;
        //tmpbuf1[2] = 0;
        //tmpbuf1[3] = 0;
        //并且需要改成大端的   wuyueqian-2009-10-22
        if (offset_sec >= 60)
        {
            tmpbuf1[0] = 1;
            tmpbuf1[1] = offset_sec - 60;
        }
        else
        {
            tmpbuf1[0] = 0;
            tmpbuf1[1] = offset_sec;
        }
        tmpbuf1[2] = offset_p1s;
        tmpbuf1[3] = 0;
        #ifdef PC
		SIM_ChgLongPC(tmpbuf1);
        #endif

        for (; recordCNT > 0; recordCNT--)
        {
            //ClearWatchDog();
            if ((timetab->min != 0) || (timetab->sec != 0) || (timetab->p1s != 0))
            {
                tmpbuf2[0] = timetab->min;
                tmpbuf2[1] = timetab->sec;
                tmpbuf2[2] = timetab->p1s;
                tmpbuf2[3] = 0;
                #ifdef PC
				SIM_ChgLongPC(tmpbuf2);
                #endif
                if (*(long *) tmpbuf1 > *(long *) tmpbuf2)
                {
                    //处理时间条比OFFSET值小的情况
                    //memcpy(tmpbuf1, tmpbuf2, sizeof(tmpbuf1));
                    //offset_p1s = tmpbuf1[0];
                    //offset_sec = tmpbuf1[1];
                    timetab->min = 0;
                    timetab->sec = 0;
                    timetab->p1s = 0;
                    timetab++; //指针加一
                    continue;
                }

                //100ms(0.1s)数位减
                if (timetab->p1s >= offset_p1s)
                {
                    j = timetab->p1s;
                    i = 0;
                }
                else
                {
                    j = timetab->p1s + 10;
                    i = 1;//借了1
                }
                timetab->p1s = j - offset_p1s;

                //秒数位减
                if (timetab->sec >= (tmpbuf1[1] + i))
                {
                    j = timetab->sec - i;
                    i = 0;
                }
                else
                {
                    j = timetab->sec + 60 - i;
                    i = 1;
                }
                timetab->sec = j - tmpbuf1[1];

                //分位数减
                timetab->min -= (i + tmpbuf1[0]);
            }
            timetab++; //指针加一
        }
    }
    return;
}

bool Sort_Div_Save(void)
{
    //保证ID3有足够1秒的显示，歌词标签应从1秒开始
    HandleID3Overlap();
    HandleLastLyric();
    ClearWatchDog();
    SortLyricData();
    ClearWatchDog();
    LyricDivScreen();
    AddOffsetData((LrcTime_t *) lyricSave.LyricTimeAddrBuffer, TimeRecordCounter);
    ClearWatchDog();
    return SaveLyricInVRAM();
}
