#ifndef         _readlyricfile_H
#define         _readlyricfile_H

//#include        "chip_cfg.h"
#include        <sysdef.h>
#include        <actos.h>

#define MINUTE_INDEX 0
#define SECONDE_INDEX 1
#define MILSECONDE_INDEX 2
#define CONTENT_IDNEX 3
#define MAX_TIME 0xff000000

typedef struct
{
    uchar min;
    uchar sec;
    uchar p1s;
    uchar buffaddr_High;
    uchar buffaddr_Low;
} LrcTime_t;

#ifdef ColorSrcreen

//===================歌词翻屏时间的设置(表示翻屏时间最大不能超过这个值)============注要用标准时间写法
#define         MAX_ONE_SCREEN_TIME_MIN        0      //这里指分钟数   （0-59） 
#define         MAX_ONE_SCREEN_TIME_SEC        5      //这里指的是秒数 （0-59） 
#define         MAX_ONE_SCREEN_TIME_100MS      5      //这里指的是100毫秒数 9 表示 900ms（0-9）
//=======================================================================

//<><><><><><><><><><><><><><><><><><> [ UP26A36_WMA ] <><><><><><><><><><><><><><><><><><><><><><>
#define         LCD_DIS_CHINESE_LINE      2    //要求在LCD上显示汉字为几行 
#define         LCD_DIS_ENGLISH_LINE      2	//LCD_DIS_CHINESE_LINE*2  
#define         CHINESE_COL             16  //12	//16   //汉字方阵的列数
#define         CHINESE_LINE            16  //12	//16  //汉字方阵的行数
#define         ASCII_COL               8   //6 //Ascii 方阵的列数
#define         ASCII_LINE              16  //8 //Ascii 方阵的行数
#define         ICON_WIDTH_DOT           0  //图标区的宽度(up26a33为12)
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


//define every table address in VRAM
#define LYRIC_FILENAME_ADDR VM_LYRIC
#define LYRIC_FILENAME_SIZE 100
#define LYRIC_PAGEMODE_ADDR  LYRIC_FILENAME_ADDR + LYRIC_FILENAME_SIZE
#define LYRIC_PAGEMODE_SIZE 1
#define TIME_TABLE_ADDR     LYRIC_PAGEMODE_ADDR + LYRIC_PAGEMODE_SIZE
#define TIME_TABLE_SIZE     0x800-LYRIC_FILENAME_SIZE-LYRIC_PAGEMODE_SIZE
#define CONTENT_TABLE_ADDR  TIME_TABLE_ADDR+TIME_TABLE_SIZE
#define CONTENT_TABLE_SIZE  0x1000
#define TIME_RECORD_SIZE   5

#ifdef __C251__
//bool readlyricfileMSA(const char *);
HANDLE lyricOpenfile(const BYTE *shortfilename);
#endif

#else    //#ifdef ColorSrcreen
//===================歌词翻屏时间的设置(表示翻屏时间最大不能超过这个值)============注要用标准时间写法
#define         MAX_ONE_SCREEN_TIME_MIN        0                   //这里指分钟数   （0-59）                     //
#define         MAX_ONE_SCREEN_TIME_SEC        5                   //这里指的是秒数 （0-59）                     //表示5.5秒
#define         MAX_ONE_SCREEN_TIME_100MS      5                   //这里指的是100毫秒数 9 表示 900ms（0-9）      //
//=======================================================================
#ifdef         UNICODE_SUPPORT

//<><><><><><><><><><><><><><><><><><> [ UP26A36_WMA ] <><><><><><><><><><><><><><><><><><><><><><>
#define         LCD_DIS_CHINESE_LINE      2                    //要求在LCD上显示汉字为几行(一般ASCII行数为汉字的2倍)(up26a33 为2行)
#define         LCD_DIS_ENGLISH_LINE      2 //LCD_DIS_CHINESE_LINE*2  //要求在LCD上显示英文为几行(一般ASCII行数为汉字的2倍)(up26a33 为4行)
#define         CHINESE_COL             16                   //汉字方阵的列数
#define         CHINESE_LINE            16                   //汉字方阵的行数
#define         ASCII_COL               8                    //Ascii 方阵的列数
#define         ASCII_LINE              16                    //Ascii 方阵的行数
#define         ICON_WIDTH_DOT           12                   //图标区的宽度(up26a33为12)
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

#else
//<><><><><><><><><><><><><><><><><><> [ UP26A36_WMA ] <><><><><><><><><><><><><><><><><><><><><><>
#define         LCD_DIS_CHINESE_LINE      2                    //要求在LCD上显示汉字为几行(一般ASCII行数为汉字的2倍)(up26a33 为2行)
#define         LCD_DIS_ENGLISH_LINE      LCD_DIS_CHINESE_LINE*2  //要求在LCD上显示英文为几行(一般ASCII行数为汉字的2倍)(up26a33 为4行)
#define         CHINESE_COL             16                   //汉字方阵的列数
#define         CHINESE_LINE            16                   //汉字方阵的行数
#define         ASCII_COL               6                    //Ascii 方阵的列数
#define         ASCII_LINE              8                    //Ascii 方阵的行数
#define         ICON_WIDTH_DOT           12                   //图标区的宽度(up26a33为12)
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#endif

//define every table address in VRAM
#define LYRIC_FILENAME_ADDR VM_LYRIC
#define LYRIC_FILENAME_SIZE 100
#define LYRIC_PAGEMODE_ADDR  LYRIC_FILENAME_ADDR + LYRIC_FILENAME_SIZE
#define LYRIC_PAGEMODE_SIZE 1
#define TIME_TABLE_ADDR     LYRIC_PAGEMODE_ADDR + LYRIC_PAGEMODE_SIZE
#define TIME_TABLE_SIZE     (0x800-LYRIC_FILENAME_SIZE-LYRIC_PAGEMODE_SIZE)
#define CONTENT_TABLE_ADDR  TIME_TABLE_ADDR+TIME_TABLE_SIZE
#define CONTENT_TABLE_SIZE  0x1000
#define TIME_RECORD_SIZE   5

#ifdef __C251__
//bool readlyricfileMSA(const char *);
HANDLE lyricOpenfile(const BYTE *shortfilename);
#endif

#endif  //#ifdef ColorSrcreen
typedef struct lyricSaveFormat
{
    UCHAR lyricFileName[LYRIC_FILENAME_SIZE];
    UCHAR lyricPageMode;//[LYRIC_PAGEMODE_SIZE];
    UCHAR LyricTimeAddrBuffer[TIME_TABLE_SIZE];
    UCHAR LyricStrBuffer[CONTENT_TABLE_SIZE];
} lyricSaveFormat_T;

#endif//_readlyricfile_H
