/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_getstringlengthExt.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "Drv_S7565.h"
#include "display.h"
#include "string.h"


#pragma renamecode(?PR?BBC_GSLExt)


/*
*******************************************************************************
 
** FUNCTION:    GetFontPoint_Ext
**
** PURPOSE:     Get Char Point Info by dma2
**
** PARAMETERS:
**  input
**      code=区位码
**      picbuf=输出数据的存放地址
**  output
**      none
** DESCRIPTION:
****************************************************************************
*/
void Sdrv_GetFontPointExt(uint16 ucode,uint8 * picbuf)
{
	SD_RW sdrwfont;
	uint16  PageNum;
	sdrwfont.faddr=(uint32)( FontLibAddr+(uint32) ucode*33);
	if(((WORD)ucode<0x0080)&&(AsciiSelect==0))
	{
		sdrwfont.faddr=(uint32)( AsciiFontLibAddr+(uint32) ucode*8);
	}
//	if((uint16)picbuf < 0x4000)
//	{
//		;//sdrwfont.srampage=0x08;
//	}
//	else
//	{
//		sdrwfont.srampage= input8(0x05) & 0x07;
//	}
	sdrwfont.srampage = 0;
	sdrwfont.sramaddr = (uint)picbuf;
	
	//同一扇区
	PageNum = sdrwfont.faddr%512;
	if ( PageNum <= 479 )
	{
		SD_Read(&sdrwfont,33);
	}
	else
	{
		SD_Read(&sdrwfont,512 - PageNum );
		sdrwfont.sramaddr = (uint16)picbuf+ 512 - PageNum ;
		sdrwfont.faddr+=  512 - PageNum;
		SD_Read(&sdrwfont,33 - (512 - PageNum ));
	}
	
}


/*
*******************************************************************************
 
** FUNCTION:    Sdrv_GetStringLength
**
** PURPOSE:     Get Char Point Info by dma2
**
** PARAMETERS:
**  input
**      str=字符串首地址
**      str_length=字符串长度
**      str_type,  0, char, 1,unicode
**  output
**      unsigned char = 字符宽度
** DESCRIPTION:
****************************************************************************
*/
bool Sdrv_GetStringLengthExt(uint8 *str,uint16 CharCount,uint8 str_type, uint8 * ResCharCount, uint8 * ResColCount)
{
	uint8  StrCnt ;
	 uint16 code_temp;
	uint8 cur_length;
	uint16  ucode;
	uint8 StrWidth = 0;
///	uint8 ThaiScanLen;
	uint8 WordsCount = 0;
	uint8 Words_bk;
	uint8 Col = 0;
	uint8 Col_bk;
	uint8 *str_bk = str;
	uint8 str_type_bk = str_type;
///	uint8 result;

	if(CharCount > RowWordsCountMax)
		cur_length = RowWordsCountMax;
	else
		cur_length = (int8)CharCount;
	if(Language_ID == ARABIC)
	{
		if(str_type==0)
		{
			//只处理前面26个字符，足够返回一个大于LCM宽度的数值用以表示该字符串长度大于LCM宽度
			memcpy(AuxiliaryBuffer, str, cur_length);
			AuxiliaryBuffer[cur_length] = '\0';
			cur_length = BAPI_TurnArabicToUn(AuxiliaryBuffer, FALSE, (int8)cur_length);
			str = AuxiliaryBuffer;
		}
		cur_length = BAPI_CopyStr(AuxiliaryBuffer,str,cur_length,1);
		str = AuxiliaryBuffer;
		str_type=1;
	}
	
	if(str_type == 0)
	{
		for(StrCnt = 0; StrCnt < cur_length; StrCnt ++)
		{
			if((*str) == '\r' ||  (*str) == '\n' )
			{
				break;
			}

			Col_bk = Col;
			Words_bk = WordsCount;
			if (*str >= 0x80 && MBCodeTwoFlag)
			{
				ucode = *(uint16 *)str;
#ifndef PC
		    Sdrv_SwapIntByte((uint16*) &code_temp);
#endif                
				str+=2;
				Col  += 2;
				WordsCount ++;
			}
			else
			{//ASCII
				ucode = *str;
				str++;
				Col ++;
				WordsCount ++;
			}
			ucode = BAPI_FontCodeMBToUni(ucode);
			StrHeight = UNICODE_HEIGHT_DEF;//把当前行的高度设为汉字的高度
			
			Sdrv_GetFontPointExt(ucode, CharPointBuffer);//取点阵
			if((AsciiSelect==0)&&((uint16)ucode<0x0080))
			{
				CharPointBuffer[32]=0x06;
			}
			else if((ucode==0x0E31)||((ucode<=0x0E3A)&&(ucode>=0x0E34))
					||((ucode<=0x0E4E)&&(ucode>=0x0E47)))
			{
				continue;
			}			
			StrWidth += CharPointBuffer[32];
			if(StrWidth > 128 || WordsCount > RowWordsCountMax)
			{
				Col = Col_bk;
				WordsCount = Words_bk;
				break;
			}
		}   /*end of  for*/
	} /*end of if*/
	else
	{
		for(StrCnt = 0; StrCnt < cur_length; StrCnt++)
		{
			Col_bk = Col;
			Words_bk = WordsCount;
			ucode = *(uint16 *)str;
			if(ucode == 0x000D || ucode == 0x000A )
			{
				break;
			}
			else
			{
				str += 2;
				Col  += 2;
				WordsCount ++;
			}
			Sdrv_GetFontPointExt(ucode, CharPointBuffer);//取点阵
			if((AsciiSelect==0)&&((uint16)ucode<0x0080))
			{
				CharPointBuffer[32]=0x06;
			}
			else if((ucode==0x0E31)||((ucode<=0x0E3A)&&(ucode>=0x0E34))
					||((ucode<=0x0E4E)&&(ucode>=0x0E47)))
			{
				continue;
			}
			StrWidth += CharPointBuffer[32];

			if(StrWidth > 128 || WordsCount > RowWordsCountMax)
			{
				Col = Col_bk;
				WordsCount = Words_bk;
				break;
			}
		} /*end of for*/
	}   /*end of else*/

	// 返回结果，即使缺页也返回，因为如果是到达文件结束，将输出文件末尾部分
	* ResCharCount = WordsCount;
	* ResColCount = Col;

	//阿拉伯特殊处理
	if(Language_ID == ARABIC)
	{
		//用于判断阿拉伯语言是否缺页
		if(CharCount <= RowWordsCountMax && StrCnt >= cur_length)
			return TRUE;
		else if(StrCnt >= cur_length)
			return FALSE;

		//切掉阿拉伯字符串的不完整尾部
		cur_length = WordsCount;
		if(str_type_bk==0)
		{
			//只处理前面26个字符，足够返回一个大于LCM宽度的数值用以表示该字符串长度大于LCM宽度
			memcpy(AuxiliaryBuffer, str_bk, cur_length);
			AuxiliaryBuffer[cur_length] = '\0';
			cur_length = BAPI_TurnArabicToUn(AuxiliaryBuffer, FALSE, (int8)cur_length);
			str_bk = AuxiliaryBuffer;
		}
		*ResCharCount = WordsCount = BAPI_CopyStr(AuxiliaryBuffer,str_bk,cur_length,1);
		if(str_type_bk == 0)
		{
			*ResColCount = WordsCount;
		}
		else
		{
			*ResColCount = WordsCount + WordsCount;
		}

		return FALSE;
	}

	if((int)StrCnt >= CharCount)
		return TRUE;
	else
		return  FALSE;
}

