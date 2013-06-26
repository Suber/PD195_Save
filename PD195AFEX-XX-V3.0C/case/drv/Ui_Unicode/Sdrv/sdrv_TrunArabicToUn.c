/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_TrunArabicToUn.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "Drv_S7565.h"
#include "display.h"
#include "string.h"


 
#pragma userclass(near=BAD_TurnArabicToUn)
uint8 UnicodeBuffer[128];
#pragma userclass(near=default)

 
#pragma renamecode(?PR?BAC_TurnArabicToUn)

/*
*******************************************************************************
** FUNCTION:    TurnArabicToUn
**
** PURPOSE:     将阿拉伯和希伯莱非Unicode字符串转换为Unicode
**
** PARAMETERS:
**  input
**      charbuf: 字符串指针
**      length: 要转换为Unicode的字符数目，为-1表示全部转换
**  output
**      char: 返回转换的字符数
** DESCRIPTION:
****************************************************************************
*/
int8 TurnArabicToUn(int8* charbuf, int8 IsUnicode, int8 CharCount)
{
	int8 bufindex;
	int8 i;
	int8 length;
	uint16 ucode;
	SD_RW sdrwfont;

	if(CharCount == -1)
	{
		if(IsUnicode)	//仅仅用来求取该Unicode字符串的字符数目
		{
			char totallength = 0;
			while(1)
			{
				if(*(uint16 *)charbuf == 0x00)
					break;
				totallength ++;
				charbuf ++;
				charbuf ++;
			}
			
			return totallength;
		}
		length = 254;
	}
	else
		length = (int8)CharCount + (int8)CharCount;

	bufindex = 0;
	for(i = 0; i < length; i++)
	{
		ucode = charbuf[i];
		if(ucode == 0)
		{
			break;
		}
		if(ucode>=0x80)
		{
			/*需进行多国语言的转换*/
			sdrwfont.faddr=MBToUniTabAddr+(((uint32)ucode)<<1);
			sdrwfont.srampage=0x00;
			sdrwfont.sramaddr=(uint16)&UnicodeBuffer[bufindex];
			SD_Read(&sdrwfont,2);
			Sdrv_SwapIntByte(((uint16*)(&UnicodeBuffer[bufindex])));
		}
		else
		{
			*((int16*)(&UnicodeBuffer[bufindex])) = ucode;
		}
		bufindex++;
		bufindex++;
		if(bufindex >=126)
		{
			break;
		}
	}
	*((int16*)(&UnicodeBuffer[bufindex])) = 0;
	bufindex++;
	bufindex++;
	memcpy(charbuf,UnicodeBuffer,bufindex);
	
	return bufindex/2 - 1;
}

