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

#pragma name(MW_EH_LRC_API1)

#include <actos.h>
#include <enhanced.h>
#include "ReadLyricFile.h"
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

uint16 lyricPageNo;
uint32 lyricCurTime;
uint32 lyricNextTime;
HANDLE lyricFileHandle;
uint8 LanguageFlag;
uint16 g_LCD_COL_TIMES;//歌词每行最长列数 cailizhen 2011-3-9 10:05

extern uint8 offset_p1s;//0.1s值
extern uint8 offset_sec;//秒值
//bool lyricInitflag = FALSE;//为真表示已初始化

extern bool ReadOffset(void);
bool readlyricfileMSA(const char* id3InfoBuffer);
extern BOOL ExplainLyricFile(const char* id3InfoBuffer);
extern bool Sort_Div_Save(void);
void setTempBufferClk(uint8 * sfrBak, uint8 *membankBak, uint8 *mscrBak);
void storeTempBufferClk(uint8 * psfrBak, uint8 *pmembankBak, uint8 *pmscrBak);
extern languageLrc_t LyricLanguageFlag;
#pragma renamecode(MW_EH_LRC_API1)

/********************************************************************************
 * Description : Initialize lyric Module
 *
 * Arguments  :
 ScreenMode:  the size of screen, only support LYRIC_SCREEN_128_32 now
 CharSet:  Character set  type of lyric, should parse lyric according to it
 * Returns     :
 TRUE: Successful
 FALSE: fail.
 * Notes       :  don't support screen mode and charactor set now.
 *
 ********************************************************************************/
#ifndef PC
bool slyricInit(uchar ScreenMode, uchar CharSet)
#else
bool lyricInit(uchar ScreenMode, uchar CharSet)
#endif
{
	// 设置歌词行列数, 默认为128
	g_LCD_COL_TIMES = 128;
    if(ScreenMode == LYRIC_SCREEN_262_100)
    {
    	g_LCD_COL_TIMES = 262;
    }
    
    LanguageFlag = CharSet;
    //lyricInitflag = TRUE;
    return TRUE;
}

void setTempBufferClk(uint8 * psfrBak, uint8 *pmembankBak, uint8 *pmscrBak)
{
    *psfrBak = (uint8)SFR_BANK;
    *pmembankBak = (uint8)MemBankCtl;
    *pmscrBak = (uint8)MCSR5;

    SFR_BANK = BANK_CMU_RMU;
    MemBankCtl |= 0x08; //PCMRAM IN DATA SEGMENT
    MCSR5 |= 0x04; //pcm in dataSeg
    MCSR5 &= 0xfd;

}

void storeTempBufferClk(uint8 * psfrBak, uint8 *pmembankBak, uint8 *pmscrBak)
{
    SFR_BANK = (char)(*psfrBak);
    MemBankCtl = (char)(*pmembankBak);
    MCSR5 = (char)(*pmscrBak);
}

/********************************************************************************
 * Description : Load a lyric to lyric vram buffer.
 *
 * Arguments  :
 读取歌词内容到VRAM，若成功，返回page mode 是英语模式还是中文模式。英语模式完全是1个字节代表一个字符
 strFile:  Dos 8.3 file name of music audio file.
 strID3:  ID3 information of the music audio file.
 * Returns     :
 page mode of the lyric,  0 means fail.

 * Notes       :  Only support LRC file now.
 *
 ********************************************************************************/
#ifndef PC
char slyricLoad(const BYTE * strFile, const BYTE * strID3)
#else
char lyricLoad(const BYTE * strFile, const BYTE * strID3)
#endif
{
    HANDLE hfile;
    BOOL result;
    uint8 srfBak, membankCtlBak, mscrBak;

    ClearWatchDog();

    setTempBufferClk(&srfBak, &membankCtlBak, &mscrBak);

    //PAGE MODE 会跟歌词内容一起写进VRAM，所以这里不比再取
    hfile = lyricOpenfile(strFile);
    if (hfile == 0)
    {
        storeTempBufferClk(&srfBak, &membankCtlBak, &mscrBak);
        return 0;
    }

    lyricPageNo = 0;
    lyricCurTime = 0;
    lyricNextTime = 0;
    lyricFileHandle = hfile;

    result = ReadOffset(); //读OFFSET值
    if (FALSE == result)
    {
        offset_p1s = 0;
        offset_sec = 0;
        //		if(!FS_FSeek(0, 0, lyricFileHandle))
        //		{
        //			FS_FClose(hfile);
        //			storeTempBufferClk(&srfBak, &membankCtlBak, &mscrBak);
        //			return FALSE;
        //		}
    }

    ClearWatchDog();
    result = readlyricfileMSA(strID3);

    if (!FS_FClose(hfile))
    {
        storeTempBufferClk(&srfBak, &membankCtlBak, &mscrBak);
        return 0;
    }

    storeTempBufferClk(&srfBak, &membankCtlBak, &mscrBak);

    return result;
}

/********************************************************************************
 * Description : Query if exist lyric of specified music audio file.
 *
 * Arguments  :
 strFile:  Dos 8.3 file name of music audio file.
 * Returns     :
 TRUE:  Exist.
 FALSE: Don't

 * Notes       :  Only support LRC file now.
 *
 ********************************************************************************/
bool lyricQueryExist(const BYTE * strFile)
{
    HANDLE hfile;

    hfile = lyricOpenfile(strFile);
    if (hfile == 0)
    {
        return FALSE;
    }
    return FS_FClose(hfile);
}

/********************************************************************************
 * Description : readlyricfileMSA
 *
 * Arguments  :

 * Returns     : 返回0 表示该歌词文件无效， 返回1表示该歌词有效


 * Notes       :  Only support LRC file now.
 *
 ********************************************************************************/

bool readlyricfileMSA(const char* id3InfoBuffer)
{
    bool errorFlag;

    LyricLanguageFlag = English_LRC;
    errorFlag = ExplainLyricFile(id3InfoBuffer);//right 0; error; 1
    //返回0表示
    if (ERROR_EXPLAIN == errorFlag)
    {
        return FALSE;//表示读出错
    }

    //存储到VRAM
    if (!Sort_Div_Save()) //不用切bank
    {
        return FALSE;//表示存储出错
    }
    return TRUE;
}

