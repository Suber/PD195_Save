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
#pragma name(MW_EH_LRC_SUB4)
//edata
extern HANDLE lyricFileHandle;
extern uint8 LanguageFlag;//LanguageFlag = CharSet是字符集，
//虽然很多种语系，但只是区分是两个字节表示还是一个字节表示一个字符
//统一用Ascii表示一个字节, chinese表示两个字节
extern lyricSaveFormat_T lyricSave;
LrcTime_t * pTimeLabel;
uint8 TimeRecordCounter;//一共有TimeRecordCounter+1条标签 表示结束的那一条不加
languageLrc_t LyricLanguageFlag;//语言标志       { 01h: 英语 || 02h: 汉语 }
BOOL AddLinkCharFlag = FALSE;//分屏时要加连接符标志 00:不加,  0ffh: 加 如果加则应指向前一个字符
uint8 * pLyricStrBufferOffest;
uint8 * pDataBufferOffest;
uint16 PointColCounter;//现用了点阵的列数计数器
uint8 HalfChineseTempSavRam;//半个汉字代码暂存
uint8 LyricLineCounter;//歌词行数计数器,记录当前歌词读到的行数（在读LRC文件处理时用到）
uint8 No1DataNotStaConFlag;//第一个字符是“[”标志，下面有用  //00h: "][" 的情况  0ffh: "]x"的情况
uint8 DivScreenPageCNT;//分屏计数器  03-9-12 16:21
uint8 SameStringCNT;//多个时间标签共用一个字符串计数器(为1时表示有2个时间标签共用一个字符串，类推)

#pragma userclass(near = DATA_BUFFER)
uchar DataBuffer[PAGE_BYTE];////读取歌词，再做时间和歌词内容的分离
#pragma userclass(near = default)

//fardata
bool far No1BitNotZero;//在读歌词文件时读分秒时第一位是否为0  当为0第一位为0 /0ffh:  前面已有不是0的数了
word far SaveTheStartDivAddr;//记录开始分屏的开始时间片指针地址。
//BOOL far LyricFileFlag;//是否存在歌词文件标志  { 01h:有歌词文件 || 00h:无歌词文件 }
//uint8 far TmpSavTimeRecCNT;//临时记录 时间记录 个数倒计数的值

uint8 far lyricFilePageNum;
uint16 far lyricFileLastPageByte;
uint8 far lyricFilePagePos;

uchar *far pTimeLabelChar;//做时间标签的BCD变化有用


BOOL ExplainLyricFile(const char* id3InfoBuffer);
extern returnDealString_t DealSaveString(uchar charData);
extern uchar ReadID3FromBuf(void);

returnReadData_t TimeStartDataRead(void);
returnReadData_t MinuteDataRead(void);
returnReadData_t SecondDataRead(void);
returnReadString_t ReadStringData(void);

void DealTimeData(void);
returnReadString_t SaveStringData(uchar tempData);

#pragma renamecode(MW_EH_LRC_SUB4)

BOOL ExplainLyricFile(const char* id3InfoBuffer)
{
    returnReadData_t result_data;
    returnReadString_t result_string;
    No1BitNotZero = 0;

    //ClearWatchDog();
    //LyricLanguageFlag = English_LRC;
    //记得在databank确定后优化这部分，因为获取offest的时候已经处理前面
    //一部分内容了，不必重新处理
    //之前是复制歌词内容到offestBuffer，这里是复制到dataBuffer，是dababank的原因
    lyricFilePageNum = (uint8) FS_GetFileLen(lyricFileHandle);
    lyricFileLastPageByte = (uint16) FS_GetUnalignedLen(lyricFileHandle);

    repeat_parse_file_again: lyricFilePagePos = 0;
    if (!FS_FSeek(0, 0, lyricFileHandle))
    {
        return 1;
    }
    memset((char *) &lyricSave.LyricTimeAddrBuffer, 0, sizeof(lyricSaveFormat_T) - LYRIC_FILENAME_SIZE
            - LYRIC_PAGEMODE_SIZE);
    memset(DataBuffer, 0, PAGE_BYTE);//第一次应该清除以前的脏数据
    memcpy(DataBuffer, id3InfoBuffer, 90); //会再设定，原来是90的
    //DataBuffer[90] = 0x0;

    repeat_parse_id3_again: TimeRecordCounter = 0;//清读记录计数器
    HalfChineseTempSavRam = 0;//清半个汉字记录器
    LyricLineCounter = 0;//清歌词行数计数器
    PointColCounter = 0; //清歌词列数计数器
    SameStringCNT = 0;//清一行歌词对应的时间标签数，默认为0，表示1条


    //读取ID3信息
    if (0 != ReadID3FromBuf())
    {
        goto repeat_parse_id3_again;
    }

    //读取歌词信息文件系统读文件时应该考虑正常读，读完和读出错三种情况
    pDataBufferOffest = DataBuffer + PAGE_BYTE;//for timeStartRead File

    while (1)
    {
        ClearWatchDog();
        repeat_searchTimeStart: result_data = TimeStartDataRead();
        if ((READ_DATA_FILE_END == result_data) || (READ_DATA_ZERO == result_data))
        {
            goto rightExplain;
        }
        else if (READ_DATA_FILE_ERROR == result_data)
        {
            goto errorExplain;
        }
        else
        {
            //right
        }

        repeat_minuteDataRead: //sldfkjsdlf[]
        result_data = MinuteDataRead();
        if ((READ_DATA_FILE_END == result_data) || (READ_DATA_ZERO == result_data))
        {
            goto rightExplain;
        }
        else if (READ_DATA_FILE_ERROR == result_data)
        {
            goto errorExplain;
        }
        else if (READ_DATA_NOT_USE_DATA == result_data)
        {
            goto repeat_searchTimeStart;
        }
        else
        {
            //right
        }

        result_data = SecondDataRead();
        if ((READ_DATA_FILE_END == result_data) || (READ_DATA_ZERO == result_data))
        {
            goto rightExplain;
        }
        else if (READ_DATA_FILE_ERROR == result_data)
        {
            goto errorExplain;
        }
        else if (READ_DATA_NOT_USE_DATA == result_data)
        {
            goto repeat_searchTimeStart;
        }
        else
        {
            //right
        }

        //record string
        result_string = ReadStringData();
        if ((READ_STRING_FILE_END == result_string) || (READ_STRING_ZERO == result_string))
        {
            goto rightExplain;
        }
        else if (READ_STRING_FILE_ERROR == result_string)
        {
            goto errorExplain;
        }
        else if (READ_STRING_DONE == result_string) //歌词一行结束
        {
            goto repeat_searchTimeStart;
        }
        else if (READ_STRING_TIME_START == result_string)//sdkfjsdlkf[]
        {
            goto repeat_minuteDataRead;
        }
        else if (READ_STRING_LRC_OVERFLOW == result_string)//sdkfjsdlkf[]
        {
            goto rightExplain;
        }
        else //英文模式改为中文模式，全部重读 //READ_STRING_REREAD_FILE
        {
            goto repeat_parse_file_again;
        }

    }

    //ReadLyricFileRet
    //加上最后一条标签
    errorExplain: return ERROR_EXPLAIN;
    rightExplain:
    //add the final timeLabel,
    pTimeLabel->min = 0xff;
    pTimeLabel->sec = 0x55;
    pTimeLabel->p1s = 0xaa;
    //pTimeLabel->buffaddr_High = (uchar)(((WORD)pLyricStrBufferOffest & 0xff00)>>8);//代表歌词结束符号的下一个位置
    //pTimeLabel->buffaddr_Low= (uchar)((WORD)pLyricStrBufferOffest & 0x00ff);
    *(WORD *) (&(pTimeLabel->buffaddr_High)) = (WORD) (pLyricStrBufferOffest);
    //pTimeLabel++;//结束了，不再增加
    return RIGHT_EXPLAIN;

}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//以下程序在LRC文件中读入 1PAGE 数据到 DATABUFFER
//input :
//output:(LRCFileEndFlag)=04h,文件结束
//output:a=0 : 读文件没错 ;  a=0ffh : 读文件错 ; a=04h : 读文件结束
//文件读取时，读到最后一页返回的仍然是512byte，但后面会有补零，只能通过FS_GetUnalignedLen来获取最后一个扇区的实际字节
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
returnReadFile_t ReadLRCFileDataToBuffer(void)
{
    bool result;

    result = FS_FRead(DataBuffer, 1, lyricFileHandle);

    if (FALSE == result)
    {
        if (FS_INFO_OVEFLOW == FS_GetInfo())
        {
            return READ_FILE_END;
        }
        else
        {
            return READ_FILE_ERROR;
        }
    }
    else
    {
        lyricFilePagePos++;
        if (lyricFilePagePos == lyricFilePageNum)
        {
			if(lyricFileLastPageByte != 0x00)
			{
				*(DataBuffer + lyricFileLastPageByte) = 0x00;
			}             
        }

        return READ_FILE_NORMAL;
    }
}

bool IfDataBufferEnd(void)
{
    ClearWatchDog();
    if (((uint32) pDataBufferOffest) < ((uint32) (DataBuffer + PAGE_BYTE)))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

returnReadData_t SecondDataRead(void)
{
    uchar tempData;
    uchar secondNum = 0;
    returnReadFile_t result;
    bool lyricSecReadOkFlag = FALSE;
    bool secondReadDone = FALSE;
    bool SecondPointFlag = FALSE;
    No1BitNotZero = FALSE;

    while (TRUE)
    {
        //ClearWatchDog();
        while (!IfDataBufferEnd())
        {
            //ClearWatchDog();
            tempData = (*pDataBufferOffest++);
            if (tempData == 0)
            {
                return READ_DATA_ZERO;
            }
            else if (tempData == ']')
            {
                DealTimeData();
                TimeRecordCounter++;
                PointColCounter = 0;
                return READ_DATA_DONE;
            }
            else if (tempData == '.')
            {
                if (!SecondPointFlag)
                {
                    SecondPointFlag = TRUE;
                }
                else
                {
                    return READ_DATA_NOT_USE_DATA;
                }
            }
            else if ((tempData > '9') || (tempData < '0'))
            {
                return READ_DATA_NOT_USE_DATA;
            }
            else
            {
                if ((tempData == '0') && (No1BitNotZero == FALSE))//[00001:]----->[1:]
                {
                    continue;
                }
                else
                {
                    No1BitNotZero = TRUE;
                    if (!lyricSecReadOkFlag)
                    {
                        if (FALSE != SecondPointFlag)
                        {
                            //record point section
                            *(pTimeLabelChar + 9) = tempData - '0';
                            lyricSecReadOkFlag = TRUE;
                        }
                        else
                        {
                            // recond second
                            if (!secondReadDone)
                            {
                                if (secondNum < SECOND_VALID) //保存两位分数位
                                {
                                    *(pTimeLabelChar + 5) = *(pTimeLabelChar + 6);
                                    *(pTimeLabelChar + 6) = *(pTimeLabelChar + 7);
                                    *(pTimeLabelChar + 7) = *(pTimeLabelChar + 8);
                                    *(pTimeLabelChar + 8) = tempData - '0';
                                }
                                else
                                {
                                    *(pTimeLabelChar + 5) = 6;
                                    //为了节省代码空间, cailizhen 2010-11-15 11:13
                                    *(WORD*)(pTimeLabelChar + 6) = 0;
                                    //*(pTimeLabelChar + 6) = 0;
                                    //*(pTimeLabelChar + 7) = 0;
                                    *(WORD*)(pTimeLabelChar + 8) = 0;
                                    //*(pTimeLabelChar + 8) = 0;
                                    //*(pTimeLabelChar + 9) = 0;
                                    secondReadDone = TRUE;
                                }
                                secondNum++;
                            }
                        }
                    }

                }
            }

        }

        result = ReadLRCFileDataToBuffer();
        if (READ_FILE_NORMAL == result)
        {
            pDataBufferOffest = DataBuffer;
        }
        else //应该再判断是否出错的情况
        {
            if (READ_FILE_ERROR == result)
            {
                return READ_DATA_FILE_ERROR;
            }
            else//end
            {
                return READ_DATA_FILE_END;
            }
        }

    }

    return 0;//cancel the warmning

}

// 0 means read '\0'; 1 means 'dataBuffer over';2 means found need;

returnReadData_t MinuteDataRead(void)
{
    uchar tempData;
    uchar minuteNum = 0;
    returnReadFile_t result;
    bool minuteReadDone = FALSE;
    pTimeLabelChar = (uchar *) pTimeLabel;
    memset(pTimeLabelChar, 0, sizeof(LrcTime_t) * 2);

    No1BitNotZero = FALSE;

    while (1)
    {
        //ClearWatchDog();
        while (!IfDataBufferEnd())
        {
            //ClearWatchDog();
            tempData = (*pDataBufferOffest++);
            if (tempData == 0)
            {
                return READ_DATA_ZERO;
            }
            else if (tempData == ':')
            {
                return READ_DATA_DONE;
            }
            else if ((tempData > '9') || (tempData < '0'))
            {
                return READ_DATA_NOT_USE_DATA;
            }
            else
            {
                if ((tempData == '0') && (No1BitNotZero == FALSE))//[00001:]----->[1:]
                {
                    continue;
                }
                else
                {
                    No1BitNotZero = TRUE;
                    if (!minuteReadDone)
                    {
                        if (minuteNum < MINUTE_VALID) //保存两位分数位
                        {
                            *pTimeLabelChar = *(pTimeLabelChar + 1);
                            *(pTimeLabelChar + 1) = tempData - '0';
                        }
                        else
                        {
                            *pTimeLabelChar = 9;
                            *(pTimeLabelChar + 1) = 9;
                            minuteReadDone = TRUE;
                        }
                        minuteNum++;
                    }

                }
            }

        }

        result = ReadLRCFileDataToBuffer();
        if (READ_FILE_NORMAL == result)
        {
            pDataBufferOffest = DataBuffer;
        }
        else //应该再判断是否出错的情况
        {
            if (READ_FILE_ERROR == result)
            {
                return READ_DATA_FILE_ERROR;
            }
            else//end
            {
                return READ_DATA_FILE_END;
            }
        }

    }
    return 0;//cancel the warmning

}

returnReadData_t TimeStartDataRead(void)
{
    uchar tempData;
    returnReadFile_t result;

    while (1)
    {
        //ClearWatchDog();
        while (!IfDataBufferEnd())
        {
            //ClearWatchDog();
            tempData = (*pDataBufferOffest++);
            if (tempData == 0)
            {
                return READ_DATA_ZERO;
            }
            else
            {
                if (tempData != '[')
                {
                    continue;
                }
                else
                {
                    return READ_DATA_DONE;
                }
            }

        }

        result = ReadLRCFileDataToBuffer();
        if (READ_FILE_NORMAL == result)
        {
            pDataBufferOffest = DataBuffer;
        }
        else //应该再判断是否出错的情况
        {
            if (READ_FILE_ERROR == result)
            {
                return READ_DATA_FILE_ERROR;
            }
            else//end
            {
                return READ_DATA_FILE_END;
            }
        }

    }
    return 0;//cancel the warmning


}

void DealTimeData(void)
{
    WORD second = 0;
    uchar minute = 0;
    uchar temp;
    uchar i;

    //ClearWatchDog();
    for (i = 0; i < MINUTE_VALID; i++)
    {
        minute *= 10;
        minute += *(pTimeLabelChar + i);
    }
    for (i = 0; i < SECOND_VALID; i++)
    {
        second *= 10;
        second += *(pTimeLabelChar + 5 + i);
    }
    temp = (uchar)(second / 60);

    if ((temp + minute) > MAX_MINUTE)
    {
        pTimeLabel->min = MAX_MINUTE;
        pTimeLabel->sec = 59;
        pTimeLabel->p1s = 9;
    }
    else
    {
        minute += temp;
        second %= 60;
        pTimeLabel->min = minute;
        pTimeLabel->sec = (uchar) second;
        pTimeLabel->p1s = *(pTimeLabelChar + 9);
    }
    //pTimeLabel->buffaddr_High = (uchar)((pLyricStrBufferOffest & 0xff00)>>8);
    //pTimeLabel->buffaddr_Low= (uchar)(pLyricStrBufferOffest & 0x00ff);
    *(WORD *) (&(pTimeLabel->buffaddr_High)) = (WORD) pLyricStrBufferOffest;
    pTimeLabel++;

}

returnReadString_t ReadStringData(void)
{
    uchar tempData;
    returnReadString_t result_dealString;
    returnReadFile_t result_readFile;
    No1DataNotStaConFlag = 0;

    while (1)
    {
        //ClearWatchDog();
        while (!IfDataBufferEnd())//data buffer over
        {
            //ClearWatchDog();
            tempData = (*pDataBufferOffest++);
            if (FALSE != HalfChineseTempSavRam)
            {
                result_dealString = SaveStringData(tempData);
                if (result_dealString != READ_STRING_DONE)
                {
                    return result_dealString;
                }
            }
            else
            {
                if (tempData == 0)
                {
                    *pLyricStrBufferOffest = 0x0d;
                    return READ_STRING_ZERO;
                }
                else if ((tempData == 0x0d) || (tempData == 0x0a))
                {
                    //表示歌词一行结束了，应重新找[
                    *(pLyricStrBufferOffest++) = NEXT_SCREEN;
                    LyricLineCounter = 0;
                    PointColCounter = 0;
                    DivScreenPageCNT = 0;
                    SameStringCNT = 0;
                    return READ_STRING_DONE;
                }
                else if (tempData != '[')
                {
                    result_dealString = SaveStringData(tempData);
                    if (result_dealString != READ_STRING_DONE)
                    {
                        return result_dealString;
                    }
                }
                else//[
                {
                    if (FALSE != No1DataNotStaConFlag)
                    {
                        // sdfjsdlf []
                        *(pLyricStrBufferOffest++) = NEXT_SCREEN;
                        DivScreenPageCNT = 0;
                        SameStringCNT = 0;
                    }
                    else //[][]uujkkkkl
                    {
                        SameStringCNT++;
                    }
                    LyricLineCounter = 0;
                    PointColCounter = 0;
                    return READ_STRING_TIME_START;
                }

            }

        }

        result_readFile = ReadLRCFileDataToBuffer();
        if (READ_FILE_NORMAL == result_readFile)
        {
            pDataBufferOffest = DataBuffer;
        }
        else //应该再判断是否出错的情况
        {
            if (READ_FILE_ERROR == result_readFile)
            {
                return READ_STRING_FILE_ERROR;
            }
            else//end
            {
                *pLyricStrBufferOffest = 0x0d;
                return READ_STRING_FILE_END;
            }
        }

    }
    return 0;//cancel the warmning
}

returnReadString_t SaveStringData(uchar tempData)
{
    returnDealString_t result_dealString = DealSaveString(tempData);
    if (result_dealString == READ_BUFFER_AGAIN)
    {
        return READ_STRING_DONE;
    }
    else if (result_dealString == READ_FILE_AGAIN)
    {
        return READ_STRING_REREAD_FILE;
    }
    else//lrcbuffer over flow
    {
        //empty
        *pLyricStrBufferOffest = 0x0d;
        return READ_STRING_LRC_OVERFLOW;
    }
}

