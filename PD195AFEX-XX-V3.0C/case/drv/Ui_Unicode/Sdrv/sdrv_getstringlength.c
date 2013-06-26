/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_getstringlength.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "Drv_S7565.h"
#include "display.h"
#include "string.h"


#pragma renamecode(?PR?BBC_GSL)

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
void Sdrv_GetFontPoint_Len(uint16 ucode,uint8 * picbuf)
{
	SD_RW sdrwfont;
	uint16  PageNum;
	sdrwfont.faddr=(uint32)( FontLibAddr+(uint32) ucode*33);
	if(((uint16)ucode<0x0080)&&(AsciiSelect==0))
	{
		sdrwfont.faddr=(uint32)( AsciiFontLibAddr+(uint32) ucode*8);
	}
//	if((uint16)picbuf < 0x4000)
//	{
//		sdrwfont.srampage=0x08;
//	}
//	else
//	{
//		sdrwfont.srampage= input8(0x05) & 0x07;
//	}
	sdrwfont.srampage = 0;
	sdrwfont.sramaddr = (uint16)picbuf;
	
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
uint8 Sdrv_GetStringLength(uint8 *str,uint16 CharCount,uint8 str_type)
{
	uint16  StrCnt ;
	uint16  ucode;
	uint16 cur_length;
	uint8 StrWidth = 0;
	uint8 ThaiScanLen;


	cur_length =  CharCount;
	
	if(Language_ID == ARABIC)
	{
		if(cur_length > RowWordsCountMax)
			cur_length = RowWordsCountMax;
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
		for(StrCnt = 0; StrCnt < cur_length; StrCnt++)
		{
			if((*str) == '\0' ||  (*str) == '\n' )
			{
				break;
			}
			
			if (*str >= 0x80 && MBCodeTwoFlag)
			{
				ucode = *(uint16 *)str;
				str+=2;
			}
			else
			{//ASCII
				ucode = *str;
				str++;
			}
			ucode = BAPI_FontCodeMBToUni(ucode);
			StrHeight = UNICODE_HEIGHT_DEF;//把当前行的高度设为汉字的高度
			
			Sdrv_GetFontPoint_Len(ucode, CharPointBuffer);//取点阵
			if((AsciiSelect==0)&&((uint16)ucode<0x0080))
			{
				CharPointBuffer[32]=0x06;
			}
			else if((ucode>=0x0E01)&&(ucode<=0x0E7F))
			{
				ThaiScanLen = BAPI_ThaiOverlap(str, FALSE);
				str += ThaiScanLen;
				StrCnt += ThaiScanLen;
			}
			StrWidth += (uint16)CharPointBuffer[32];
			if (StrWidth > Display_Length)
            {
                StrWidth = Display_Length;
                break;
            }
			
		}   /*end of  for*/
		
	} /*end of if*/
	else
	{
		for(StrCnt = 0; StrCnt < cur_length; StrCnt++)
		{
			ucode = *(uint16 *)str;
			Sdrv_SwapIntByte((uint16*) &ucode);
			if(ucode == 0x0000 || ucode == 0x000A )
			{
				break;
			}
			else
			{
				str += 2;
			}
			Sdrv_GetFontPoint_Len(ucode, CharPointBuffer);//取点阵
			if((AsciiSelect==0)&&((uint16)ucode<0x0080))
			{
				CharPointBuffer[32]=0x06;
			}
			else if((ucode>=0x0E01)&&(ucode<=0x0E7F))
			{
				ThaiScanLen = BAPI_ThaiOverlap(str, TRUE);
				str += ThaiScanLen;
				StrCnt += ThaiScanLen/2;
			}
			StrWidth += (uint16)CharPointBuffer[32];
			if (StrWidth > Display_Length)
            {
                StrWidth = Display_Length;
                break;
            }
		} /*end of for*/
	}   /*end of else*/
	
	 
	
	return  StrWidth;
}

