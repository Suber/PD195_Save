/*
 *******************************************************************************
 *                ACTOS AP
 *        enhanced module impletementation
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *******************************************************************************
 */

#include <actos.h>
#include <enhanced.h>
#include "ReadLyricFile.h"
#pragma name(MW_EH_LRC_SUB2)

const char offsetkeyword[] =
{ 8, '[', 'O', 'F', 'F', 'S', 'E', 'T', ':' };//8是buffer的长度


//Fdata
extern uchar DataBuffer[PAGE_BYTE];////读取歌词，再做时间和歌词内容的分离
int16 far g_keyWordPos;//在当前BUF中的位置
static bool far result;

//Edata
extern HANDLE lyricFileHandle;
uint8 offset_p1s;//0.1s值
uint8 offset_sec;//秒值
bool positiveflag;//正号标志


bool SearchMem(BYTE *sBuff, const char *memBuff);
CharType_e checkTypeChar(uchar cdata);
bool GetOffsetData(void);
bool ReadPageData(void);

#pragma renamecode(MW_EH_LRC_SUB2)
/********************************************************************************
 * Function:  bool ReadOffset(HANDLE filehandle)
 * Description : 读出歌词文件中的 offset 标签的值
 * input:    HANDLE filehandle: 歌词文件的句柄
 * output:   bool return: 返回是否有OFFSET 标签
 *             如有，则放在 公共变量 offset_sec,offset_ms中
 ********************************************************************************/
bool ReadOffset(void)
{
    offset_sec = 0;
    offset_p1s = 0;
    g_keyWordPos = 512;//要求读数据(因大等于512就要读数据)

    while (1)
    {
        ClearWatchDog();
        //全部是一个字符一个字符地检测，所以要常读
        result = ReadPageData();//读一个PAGE数据
        if (!result)
        {
            FS_FSeek(0, 0, lyricFileHandle);//回到文件的开始位置
            return FALSE;
        }

        //找关键字 [OFFSET:
        result = SearchMem(DataBuffer, offsetkeyword);//找字串
        if (!result)
        {
            FS_FSeek(0, 0, lyricFileHandle);//回到文件的开始位置
            return FALSE;
        }

        //找到关键字，读取偏移时间
        result = GetOffsetData();
        if (FALSE != result)
        {
            //成功找到Offset的值，退出
            FS_FSeek(0, 0, lyricFileHandle);//回到文件的开始位置
            return TRUE;
        }
        //循环的意义是，读偏移时间是若发现[OFFEST:XXX]这种情况会出错退出
        //而这个时候文件还未结束，可以继续搜索
    }
    return 0;//cancel the warmning
}

/********************************************************************************
 函数原型： bool GetOffsetData(void)
 input:
 void :
 g_keyWordPos: 当前数据指针
 offset_p1s是char型，最多小于100秒的偏移秒数，如果要设置大于100，应该改类型，并在程序中添加处理程序

 output:
 bool return:  T：成功； F：失败
 成功时读到的值放在如下两变量中:
 char offset_ms;
 char offset_sec;


 功能：读出OFFSET数据
 ********************************************************************************/
bool GetOffsetData(void)
{
    uchar tmpbuff[5] =
    { 0, 0, 0, 0, 0 };
    uchar i = 0;

    positiveflag = TRUE;//默认为正号

    if (DataBuffer[g_keyWordPos] == '-')
    {
        positiveflag = FALSE;
        g_keyWordPos++;
        result = ReadPageData();
        if (!result)
        {
            return FALSE;
        }
    }
    else if (DataBuffer[g_keyWordPos] == '+')
    {
        g_keyWordPos++;
        result = ReadPageData();
        if (!result)
        {
            return FALSE;
        }
    }
    else
    {
    }

    while (1)
    {
        ClearWatchDog();
        if (DataBuffer[g_keyWordPos] == ']')
        {
            //已读到数据，作转换，并返回真 毫秒是单位
            //例如[OFFEST:234],偏移234毫秒，读为[2][3][4][][] 则应移成[0][0][2][3][4]
            //数字在LRC文件里面也是ASCII字符
            for (; i < 5; i++)
            {
                //右移一位,左边补0
                tmpbuff[4] = tmpbuff[3];
                tmpbuff[3] = tmpbuff[2];
                tmpbuff[2] = tmpbuff[1];
                tmpbuff[1] = tmpbuff[0];
                tmpbuff[0] = 0;
            }

            offset_sec = tmpbuff[1] + (uint8)(tmpbuff[0] * 10);//求出秒数
            offset_p1s = tmpbuff[2];
            return TRUE;
        }

        //防止  [OFFEST:2XXX]这种情况
        if (checkTypeChar(DataBuffer[g_keyWordPos]) != CharType_NUM)
        {
            //碰到不是数字的就出
            return FALSE;
        }

        if (i < 5)
        {
            //BUFF 只有五个byte,超过了就不再补，这里可能会造成误差
            tmpbuff[i] = DataBuffer[g_keyWordPos] - '0';
            i++;
        }

        g_keyWordPos++;
        result = ReadPageData();
        if (!result)
        {
            return FALSE; //
        }
    }
    return 0;//cancel the warmning
}

/********************************************************************************
 函数原型： bool ReadPageData()
 input:
 g_keyWordPos: 当前在BUFF中的位置

 output:
 bool return:  T：成功； F：读文件失败
 g_keyWordPos: BYTE 指针

 功能：判断BUFFER数据是否用完，如用完就读出一个新的PAGE数据
 ********************************************************************************/
bool ReadPageData(void)
{
    if (g_keyWordPos >= 512)
    {
        //如数据不够就读一个PAGE数据
        result = FS_FRead(DataBuffer, 1, lyricFileHandle); //读一PAGE数据
        if (!result)
        {
            return FALSE;
        }
        g_keyWordPos = 0;
    }
    return TRUE;
}

/********************************************************************************
 函数原型： bool SearchMem(BYTE *TempBuf , const char *strbuf)
 input:
 BYTE *TempBuf: 放数据的BUFF（为一个PAGE）
 const char *strbuf: 要查的BUFF的首址（第一个字节为BUFF的长度）
 g_keyWordPos: 当前在BUFF中的位置

 output:
 bool return: 找到字符串时为真
 g_keyWordPos: 完全相同时输出的是比较串的最后一个数据的下一位置

 ********************************************************************************/
bool SearchMem(BYTE *sBuff, const char *memBuff)
{
    int j;
    uchar k;
    bool result_SearchMem;
    j = 0;

    while (1)
    {
        ClearWatchDog();
        result_SearchMem = ReadPageData();//如数据不够就读一个PAGE数据
        if (!result_SearchMem)
        {
            return result_SearchMem;//读失败就退出
        }

        k = sBuff[g_keyWordPos];

        if (checkTypeChar(k) == CharType_DNER)
        {
            k = k - 0x20;//变为大写来处理
        }

        if (k != memBuff[j + 1])//因为第一个BUFF是数组的长度
        {
            j = 0;
        }
        else
        {
            j++;
        }
        g_keyWordPos++;

        if (j >= memBuff[0])
        {
            //是否全部查找完
            return TRUE;
        }
    }
    return 0;//cancel the warmning

}

CharType_e checkTypeChar(uchar cdata)
{
    if ((cdata >= '0') && (cdata <= '9'))
    {
        return CharType_NUM;
    }

    if ((cdata >= 'A') && (cdata <= 'Z'))
    {
        return CharType_UPER;
    }

    if ((cdata >= 'a') && (cdata <= 'z'))
    {
        return CharType_DNER;
    }

    if (cdata == ' ')
    {
        return CharType_SPACE;
    }
    return CharType_OTHER;
}

