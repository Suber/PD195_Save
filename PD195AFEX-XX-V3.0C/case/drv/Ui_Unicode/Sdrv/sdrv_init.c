/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_init.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "Drv_S7565.h"
#include  "HDrv_S7565.h"
#include "display.h"
#include "string.h"


 

#pragma userclass(near=BBD_IE)
SD_FILE * near fp;
#pragma userclass(near=default)

#pragma userclass(hconst=BBS_IE)

const uint8 far FontLibName[] ="UNICODE.BIN";
const uint8 far FontAscLibName[]="ASCII.bin";
const uint8 far MBtoUnicTabName[][13]=
    {"V936GBK.TBL",  /*简体中文codepage table*/
     "V950BIG.TBL",  /*繁体中文codepage table*/
     "V1252.TBL",      /*英文,西欧语系 codepage table*/
     "V932JIS.TBL",   /*日文 codepage table*/
     "V949KOR.TBL", /*韩文 codepage table*/
     "V1257.TBL",      /*波罗地海语系*/
     "V1251.TBL",      /*俄语语系*/
     "V1254.TBL",      /*土耳其语系*/
     "V1255.TBL",      /*希伯莱语*/
     "V874.TBL" ,       /*泰语*/
     "V1250.TBL",       /*北欧，东欧语系*/
     "V1256.TBL",      /*阿拉伯语系*/
     "V1253.TBL"      /*希腊文 codepage table*/
    };
#pragma userclass(hconst=default)
    

#pragma renamecode(?PR?BBC_IE)

bool Sdrv_LanguageSet(uint8 lang_id);
bool Sdrv_Init(uint8 mode)
{
    uint8 *pRSTDisplayAPI; //RSTDisplayAPI入口
    int *pDrvEntrance; //DrvEntrance入口
    
	fp = SD_FOpen(FontLibName,MODE_READ);
	if (fp == NULL)
	{
		 
		return 0;
	}
	FontLibAddr = fp->startaddr;
	SD_FClose(fp);
	
	fp = SD_FOpen(FontAscLibName,MODE_READ);
	if (fp == NULL)
	{
		 
		return 0;
	}
	AsciiFontLibAddr = fp->startaddr;
	SD_FClose(fp);
	
	//BufPage=0x07;           //Buffer默认位于Zram2
	//安装驱动时必须重新初始化中断向量
    //********** 显示中断向量的赋值 ********
	pRSTDisplayAPI = (uint8*) (0xff0000 | RSTDisplayAPI);
    *pRSTDisplayAPI = 0x02; //将"LJMP"放入RSTDisplayAPI入口地址
    pRSTDisplayAPI++;
    pDrvEntrance = (int*) pRSTDisplayAPI;
    *pDrvEntrance = (int) &(Drv_Entrance);
    
	Sdrv_LanguageSet(0);  //默认语言设置
	if(mode==0)
	{
		Hdrv_HardInit();
	}
	return 1;
	
}

void Sdrv_Exit(void)
{
	return;
}

bool Sdrv_LanguageSet(uint8 lang_id)
{
	unsigned char table_index = 0;
	//MBCodeTwoFlag用来标识当前语言类型是否用2个字节来表示一个内码
	//IsArabicOrHebrew用来标识当前语言是否阿拉伯或者希伯莱，这样可以减少在其他地方进行判断
	MBCodeTwoFlag = 0;
	IsArabicOrHebrew = 0;
	
	switch(lang_id)
	{
	case  CHINESE_SIMPLIFIED:
		table_index = 0;
		MBCodeTwoFlag = 1;
		
		break;
		
	case   CHINESE_TRADITIONAL:
		table_index = 1;
		MBCodeTwoFlag = 1;
		break;
		
	case  ENGLISH:
	case  FRENCH:
	case  GERMAN:
	case  ITALIAN:
	case  DUTCH:
	case  PORTUGUESE:
	case  SPANISH:
	case  SWEDISH:
	case  INDONESIAN:
		table_index = 2;
		break;
		
	case   JAPANESE:
		table_index = 3;
		MBCodeTwoFlag = 1;
		break;
		
	case   KOREAN:
		table_index = 4;
		MBCodeTwoFlag = 1;
		break;
		
                case LITHUANIAN:  //white110118#1 for canyon
				case ESTONIAN:    //white110118#1 for canyon
				case LATVIAN:     //white110118#1 for canyon
	case  CZECH:
	case  POLISH:
		table_index = 5;
		break;
		
	case  RUSSIAN:
       	case BULGARIAN:
       	case UKRAINIAN:
		table_index = 6;
		break;
		
	case  TURKISH:
		table_index = 7;
		break;
		
	case  HEBREW:
		table_index = 8;
		IsArabicOrHebrew = 1;
		break;
		
	case  THAI:
		table_index = 9;
		break;
		
        case DANISH:
        case HUNGARIAN:
        case ROMANIAN:
        case  CROATIA:
        case  SLOVAK:
        case SERBIAN:
		table_index = 10;
		break;
		
	case  ARABIC:
		table_index = 11;
		IsArabicOrHebrew = 1;
		break;
	case GREEK:
		table_index = 12;
		break;
		
	default:
		table_index = 0;
		break;
	}
	
	//设置全局变量语言类型
	Language_ID = lang_id;
	
	/*初始化多国语言到Unicode的转换表的句柄*/
	fp = SD_FOpen(MBtoUnicTabName[table_index],MODE_READ);
	if (fp == NULL)
	{
		 
		return FALSE;
	}
	MBToUniTabAddr=fp->startaddr;
	SD_FClose(fp);
	return TRUE;
}
