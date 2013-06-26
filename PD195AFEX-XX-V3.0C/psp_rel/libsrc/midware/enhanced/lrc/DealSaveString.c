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
#include	<lang_id.h>

#pragma name(MW_EH_LRC_SUB3)

extern LrcTime_t * pTimeLabel;
extern uint8 TimeRecordCounter;//一共有TimeRecordCounter+1条标签 表示结束的那一条不加
extern lyricSaveFormat_T lyricSave;
extern BOOL AddLinkCharFlag;//分屏时要加连接符标志 00:不加,  0ffh: 加 如果加则应指向前一个字符
extern uint8 * pLyricStrBufferOffest;
extern uint16 PointColCounter;//现用了点阵的列数计数器
extern uint8 HalfChineseTempSavRam;//半个汉字代码暂存
extern uint8 LyricLineCounter;//歌词行数计数器,记录当前歌词读到的行数（在读LRC文件处理时用到）
extern uint8 No1DataNotStaConFlag;//第一个字符是“[”标志，下面有用  //00h: "][" 的情况  0ffh: "]x"的情况
extern languageLrc_t LyricLanguageFlag;//语言标志       { 01h: 英语 || 02h: 汉语 }
extern uint8 LanguageFlag;//LanguageFlag = CharSet是字符集，
extern uint8 SameStringCNT;//多个时间标签共用一个字符串计数器(为1时表示有2个时间标签共用一个字符串，类推)
extern uint8 DivScreenPageCNT;//分屏计数器  03-9-12 16:21
extern uint8 * pDataBufferOffest;
extern uchar DataBuffer[];
extern uint16 g_LCD_COL_TIMES;//歌词每行最长列数 cailizhen 2011-3-9 10:05

returnDealString_t DealSaveString(uchar charData);
returnDealString_t dealChineseCharLow(uchar charData);
returnDealString_t dealAsciiChar(uchar charData);
returnDealString_t dealChineseCharHigh(uchar charData);
uchar GetAsciiCol(void);
void AddLinkChar(uchar charData);
bool isEnglishChar(uchar cData);
void DoSomeThingForDivScrren(bool isChineseChar);

#pragma renamecode(MW_EH_LRC_SUB3)

void DoSomeThingForDivScrren(bool isChineseChar)
{
    uchar i;

    //复制时间标签
    memcpy(pTimeLabel, pTimeLabel - (SameStringCNT + 1), (SameStringCNT + 1) * sizeof(LrcTime_t));

    //换屏时复制时间标签后要接着处理指向的LRCbuffer的地址
    DivScreenPageCNT++;//DivScreenPageCNT = 0 表示第一屏 1 第二屏
    for (i = 0; i < (SameStringCNT + 1); i++)
    {
        ClearWatchDog();
        pTimeLabel->p1s &= 0x0f;
        pTimeLabel->p1s |= DivScreenPageCNT << 4;//将分配数加到毫秒的高四位，第四位表示0到900毫秒

        if (FALSE != isChineseChar)
        {
            *(WORD *) (&(pTimeLabel->buffaddr_High)) = (WORD) (pLyricStrBufferOffest - 2);
        }
        else
        {
            if (FALSE != AddLinkCharFlag)
            {
                //pTimeLabel->buffaddr_High = (uchar)(((WORD)(pLyricStrBufferOffest-2)& 0xff00)>>8);
                //pTimeLabel->buffaddr_Low= (uchar)((WORD)(pLyricStrBufferOffest-2)& 0x00ff);
                *(WORD *) (&(pTimeLabel->buffaddr_High)) = (WORD) (pLyricStrBufferOffest - 2);

            }
            else
            {
                //pTimeLabel->buffaddr_High = (uchar)(((WORD)(pLyricStrBufferOffest-1)& 0xff00)>>8);
                //pTimeLabel->buffaddr_Low= (uchar)((WORD)(pLyricStrBufferOffest-1)& 0x00ff);
                *(WORD *) (&(pTimeLabel->buffaddr_High)) = (WORD) (pLyricStrBufferOffest - 1);

            }
        }

        pTimeLabel++;
        TimeRecordCounter++;
    }

    AddLinkCharFlag = 0;
    LyricLineCounter = 0;

}

void AddLinkChar(uchar charData)
{
    //加上换行符后pLyricStrBufferOffest已经加1了
    bool isEnglishFlag;
    uchar tempData;

    AddLinkCharFlag = FALSE;
    isEnglishFlag = isEnglishChar(charData);
    if (FALSE != isEnglishFlag)
    {
        if (FALSE != isEnglishChar(*(pLyricStrBufferOffest - 2)))
        {
            tempData = *(pLyricStrBufferOffest - 2);
            if (FALSE != isEnglishChar(*(pLyricStrBufferOffest - 3)))
            {
                //AB||C---->A-||BC
                *(pLyricStrBufferOffest - 2) = '-';

            }
            else
            {
                //xA||C---->' '||AC
                *(pLyricStrBufferOffest - 2) = ' ';
            }
            *(pLyricStrBufferOffest++) = tempData;
            PointColCounter += GetAsciiCol();
            AddLinkCharFlag = TRUE;
            return;
        }
        else
        {
            //A3||B 无需加连接符
            return;
        }
    }
    else
    {
        //AB||3 无需加连接符
        return;
    }
}

bool isEnglishChar(uchar cData)
{
    if (((cData >= 'A') && (cData <= 'Z')) || ((cData >= 'a') && (cData <= 'z')))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

uchar GetAsciiCol(void)
{
    //return (LyricLanguageFlag == English_LRC ? ASCII_COL : CHINESE_COL/2);
    return ASCII_COL;
}

returnDealString_t dealChineseCharHigh(uchar charData)
{
    HalfChineseTempSavRam = charData;
    return READ_BUFFER_AGAIN;
}

returnDealString_t dealChineseCharLow(uchar charData)
{
    if (English_LRC == LyricLanguageFlag)
    {
        LyricLanguageFlag = Chinese_LRC;
        return READ_FILE_AGAIN;
    }
    else
    {
        PointColCounter += CHINESE_COL;
        if (PointColCounter >= (g_LCD_COL_TIMES + 1))
        {
            //超过一行了
            if (LyricLineCounter == (LCD_DIS_CHINESE_LINE - 1))
            {
                //满屏
                *(pLyricStrBufferOffest++) = NEXT_SCREEN;
                *(pLyricStrBufferOffest++) = HalfChineseTempSavRam;
                *(pLyricStrBufferOffest++) = charData;
                DoSomeThingForDivScrren(TRUE);
                PointColCounter = CHINESE_COL;
                LyricLineCounter = 0;
            }
            else
            {
                //未满屏，需要换行,中文处理无需考虑连接符
                *(pLyricStrBufferOffest++) = NEXT_ROW;
                *(pLyricStrBufferOffest++) = HalfChineseTempSavRam;
                *(pLyricStrBufferOffest++) = charData;
                LyricLineCounter++;
                PointColCounter = CHINESE_COL + ICON_WIDTH_DOT;
            }

        }
        else
        {
            //没有超过一行
            *(pLyricStrBufferOffest++) = HalfChineseTempSavRam;
            *(pLyricStrBufferOffest++) = charData;
        }
    }

    HalfChineseTempSavRam = 0;
    if (pLyricStrBufferOffest < (uchar*) (lyricSave.LyricStrBuffer + CONTENT_TABLE_SIZE - 1))
    {
        return READ_BUFFER_AGAIN;
    }
    else
    {
        return STRING_BUFFER_OVERFLOW;
    }

}

returnDealString_t dealAsciiChar(uchar charData)
{
    uchar asciiColumn;
    uchar LineMax;

    asciiColumn = GetAsciiCol();
    HalfChineseTempSavRam = 0;
    PointColCounter += asciiColumn;

    if (PointColCounter >= (g_LCD_COL_TIMES + 1))
    {
        //超过一行了
        if (English_LRC == LyricLanguageFlag)
        {
            LineMax = LCD_DIS_ENGLISH_LINE; //英文模式下处理单字节字符
        }
        else
        {
            LineMax = LCD_DIS_CHINESE_LINE;
        }

        if (LyricLineCounter == (LineMax - 1))
        {

            //满屏
            *(pLyricStrBufferOffest++) = NEXT_SCREEN;
            PointColCounter = GetAsciiCol();
            AddLinkChar(charData);
            *(pLyricStrBufferOffest++) = charData;
            DoSomeThingForDivScrren(FALSE);
            //AddLinkCharFlag在上面的函数已经清零了
            LyricLineCounter = 0;
        }
        else
        {
            //未满屏，换行
            if (LyricLineCounter == (LineMax - 2))
            {
                //到最后一行了
                PointColCounter = ICON_WIDTH_DOT + asciiColumn;
            }
            else
            {
                PointColCounter = asciiColumn;
            }

            *(pLyricStrBufferOffest++) = NEXT_ROW;
            //连接符处理
            AddLinkChar(charData);
            AddLinkCharFlag = 0;//已经加了
            *(pLyricStrBufferOffest++) = charData;
            LyricLineCounter++;
        }

    }
    else //未满一行
    {
        *(pLyricStrBufferOffest++) = charData;
    }

    if (pLyricStrBufferOffest < (uchar*) (lyricSave.LyricStrBuffer + CONTENT_TABLE_SIZE - 1))//最后一个字符要加上分屏
    {
        return READ_BUFFER_AGAIN;
    }
    else
    {
        return STRING_BUFFER_OVERFLOW;
    }

}

//==========处理要保存的字符串格式的子函数===========
//说明：在buffer中读出的每个BYTE数据都要经过如下处理，把每个
//数据按这个格式重新组成新的数据格式，这种数据格式是状态机中
//用来作方便的显示。
//------------------------------------------------
//input: a: 读Buffer的字符值
//input:
//output:
//      a=1:  x
//      a=2:  再读BUFFER
//      a=3:  重新读文件
//      a=4:  空间超出而要退出
//      a=5:  x
//-----------------------------------------------

returnDealString_t DealSaveString(uchar charData)
{
    No1DataNotStaConFlag = 0xff; //第一个字符不是 [
    if (HalfChineseTempSavRam != 0)
    {
        //上一个字节是中文的高位，现处理低位
        return dealChineseCharLow(charData);
    }
    else
    {
        if ((LanguageFlag == ENGLISH) || (LanguageFlag >= FRENCH))
        {
            //isAsciiChar
            return dealAsciiChar(charData);
        }
        else
        {
            if (charData < 0x81)
            {
                //one byte encode, for exam, english
                return dealAsciiChar(charData);
            }
            else
            {
                //two bytes encode, for exam, chinese
                return dealChineseCharHigh(charData);
            }
        }
        //        else if (LanguageFlag == JAPANESE || LanguageFlag == KOREAN || LanguageFlag == CHINESE_TRADITIONAL )
        //        {
        //            //汉字 Big5 香港 台湾 繁体
        //            //
        //            if (charData < 0x81)
        //            {
        //                //ascii
        //                return dealAsciiChar(charData);
        //            }
        //            else
        //            {
        //                //chinese
        //                return dealChineseCharHigh(charData);
        //            }
        //        }
        //        else    //汉字GB2312 CHINESE_SIMPLIFIED
        //        {
        //            if (charData < 0xa0)
        //            {
        //                //ascii
        //                return dealAsciiChar(charData);
        //            }
        //            else
        //            {
        //                //chinese
        //                return dealChineseCharHigh(charData);
        //            }
        //        }

    }
}

/********************************************************************************
 * Description : ReadID3FromBufAgain
 *
 * Arguments  :

 * Returns     : 获得ID3的信息，用时间标签记录
 返回 0 表示成功读取了ID3，返回非零表示需要重新读取ID3，如英文切换为中文


 * Notes       :  Only support LRC file now.
 *
 ********************************************************************************/

uchar ReadID3FromBuf(void)
{
    //记录第一条时间标签，第一条格式为[0 0 0 歌词首地址高位  歌词首地址低位]
    uchar charData;
    returnDealString_t result;
    pTimeLabel = (LrcTime_t *) lyricSave.LyricTimeAddrBuffer;
    pLyricStrBufferOffest = lyricSave.LyricStrBuffer;
    pDataBufferOffest = DataBuffer;

    pTimeLabel->min = 0;
    pTimeLabel->sec = 0;
    pTimeLabel->p1s = 0;
    //pTimeLabel->buffaddr = (WORD)pLyricStrBufferOffest;
    *(WORD *) (&(pTimeLabel->buffaddr_High)) = (WORD) (pLyricStrBufferOffest);

    pTimeLabel++;
    TimeRecordCounter++;

    //将ID3的信息存入LyricStrBuffer，并设置时间标签
    HalfChineseTempSavRam = 0;
    charData = *(pDataBufferOffest++);
    //ID3标签已经经常处理，各信息段之间用空格 最后才用结束符号
    while (charData != NULL)
    {
        ClearWatchDog();
        result = DealSaveString(charData);
        if (result == READ_BUFFER_AGAIN)
        {
            charData = *(pDataBufferOffest++);
        }
        else if (result == READ_FILE_AGAIN)
        {
            return READ_FILE_AGAIN;
        }
        else //不会overflow
        {
            //empty
        }
    }
    DivScreenPageCNT = 0;
    *(pLyricStrBufferOffest++) = NEXT_SCREEN;
    LyricLineCounter = 0;
    return 0;
}
