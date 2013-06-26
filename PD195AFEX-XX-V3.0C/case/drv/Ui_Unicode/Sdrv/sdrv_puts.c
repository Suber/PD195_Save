/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_puts.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "Drv_S7565.h"
#include "display.h"
#include "string.h"

#pragma renamecode(?PR?BBC_PUTS)

//取屏幕坐标
void Sdrv_GetTextPos(uint8 *x,uint8 *y)
{
	*x = region.x;
	*y = region.y;
}

//设置屏幕坐标
void Sdrv_SetTextPos(uint8 x,uint8 y)
{
	if(x<Display_Length)
	{
		region.x=x;
		TextColumn=x;
	}
	if(y<Display_Height)
	{
		TextPage=y;
		region.y=y;
	}
}

//处理阿拉伯和希伯莱的特殊性
void MakeArabicHebrewLink(uint8 *str, uint8 IsUnicode)
{
	char totallength;
	char cur_length;

	totallength = BAPI_TurnArabicToUn(str, IsUnicode, -1);
	while(totallength>0)
	{
		cur_length = BAPI_CopyStr(AuxiliaryBuffer,str,RowWordsCountMax,1);
		memcpy(str, AuxiliaryBuffer, cur_length*2);
		str += cur_length*2;
		totallength -= cur_length;
	}
	*(int *)str = 0x00;
}

// 对字符串进行显示输出
int8 CommonScan(int8 *str, int8 IsUnicode, int8 strlen,uint8 IsConvert)
{
	int8 ScanLen=0;
	int8 ScanLenbk;
	uint16 ucode;
	int8 result;
	int8 lineNum_sav,rowNum_sav;//行号记录
	uint8 ucode_0xfffe = 0;
	
	lineNum_sav = region.y;
	rowNum_sav = region.x;
	StrHeight = UNICODE_HEIGHT_DEF;
	
	while (1)
	{
		//获取当前字符编码
		ucode = (IsUnicode? (*(uint16 *)(str+ScanLen)): *(str+ScanLen));
		    
		if(IsUnicode)
		{
		    ucode = (*(uint16 *)(str+ScanLen));
		    if (ucode == 0xfffe)
		    {
		        ucode_0xfffe = 1;
		    }
			else
		    {
		        ucode_0xfffe = 0;
		    }
		    if(IsConvert == 0)
		    {
		        
		        Sdrv_SwapIntByte(&ucode);
		    }
		}
		if((ucode ==0x00)||(strlen==0x00)||(ucode =='\r'))
		{
			break;
		}
		//换行处理
		if (ucode == '\n')
		{
			region.x = rowNum_sav;
			region.y += StrHeight;
			TextColumn = region.x;
			TextPage = region.y;
			lineNum_sav = region.y;//保存当前行顶的y轴坐标
			StrHeight = UNICODE_HEIGHT_DEF;
			if(IsUnicode)
			{
				ScanLen++;
				ScanLen++;
			}
			else
			{
				ScanLen++;
			}
			(uint8)strlen--;//不理负号直接减(因为0xff时表示显示全部)
			continue;
		}
		ScanLenbk = ScanLen;	//备份地址以在putchar失败后恢复
		if(IsUnicode)
		{
			ScanLen ++;
			ScanLen ++;
		}
		else
		{
			//内码和ASCII的不同处理
			//需进行多国语言的转换
			//中日韩是用两个字节表示内码,其余都是用一个字节
			if (ucode >= 0x80 && MBCodeTwoFlag)
			{
				ucode = *(uint16 *)(str+ScanLen);
				Sdrv_SwapIntByte((uint16*) &ucode);
				ScanLen++;
				ScanLen++;
			}
			else //ASCII
			{
				ucode = *(str+ScanLen);
				ScanLen++;
			}
			ucode = BAPI_FontCodeMBToUni(ucode);
			StrHeight = UNICODE_HEIGHT_DEF;//把当前行的高度设为汉字的高度
		}
		
		//输出该字符
		if (ucode_0xfffe == 0)
		{
			result = Sdrv_PutChar(str+ScanLen, IsUnicode, ucode);	//str+ScanLen指向后续字符,用于处理泰语的叠加
		}
		else
		{
			result = 0;
		}

		if (result==-1)
		{
			return ScanLenbk;
		}
		ScanLen += result;
		(uint8)strlen--;//不理负号直接减(因为0xff时表示显示全部)
		region.y = lineNum_sav;//恢复当前行顶y的轴坐标
	}
	
	return ScanLen;
}

////*********************************************************************************/
///*
//** FUNCTION:     Sdrv_PutS
//**
//** Description:  在屏幕上输出一个ANSI字符串，如串的长度超过屏幕，则多出部分不显示
//**                   指针停在最后一个能显示的字的下一个字符左上角,碰到回车或
//**                   串尾时退出，碰到换行时从下一行开始显示
//**  input
//**     str: 字符串首址 ;
//**     strlen: 字符串的长度
//**
//**  output
//**     none
//*************************************************************************************/
int8 * Sdrv_PutS(int8 *str, int8 strlen)
{
	int8 *str_bk=str;
	int8 IsUnicode = FALSE;
	int8 len = (char)strlen;
	int8 ScanLen;
	uint8 IsSwap = 0 ;
	// 增加对阿拉伯和希伯莱语的处理
	if(IsArabicOrHebrew)
	{
		// 限于空间，阿拉伯一次最多能处理26个字符
		if(len > RowWordsCountMax)
			len = RowWordsCountMax;
		memcpy(AuxiliaryBuffer, str, len);
		AuxiliaryBuffer[len] = '\0';
		len = BAPI_TurnArabicToUn(AuxiliaryBuffer, FALSE, (int8)len);
		str = AuxiliaryBuffer;
		IsSwap = 1;
		len = BAPI_CopyStr(AuxiliaryBuffer,str,len,IsSwap);
		IsUnicode = TRUE;
		
	}
	ScanLen = CommonScan(str, IsUnicode, (int8)len,IsSwap);
	//如果是阿拉伯和希伯来则返回与辅助字符串空间之对应的源字符串的位置
	return str_bk + (IsUnicode? ScanLen/2: ScanLen);
}
////*********************************************************************************/
///*
//** FUNCTION:     Sdrv_PutUnS
//**
//** Description:  在屏幕上输出一个unicode字符串，如串的长度超过屏幕，则多出部分不显示
//**                   指针停在最后一个能显示的字的下一个字符左上角,碰到回车或
//**                   串尾时退出，碰到换行时从下一行开始显示
//**  input
//**     str: 字符串首址 ;
//**     strlen: 字符串的长度
//**
//**  output
//**     none
//*************************************************************************************/
int8 * Sdrv_PutUnS(int8 *str, int8 strlen)
{
	int8 *str_bk=str;
	int8 len = (int8)strlen;
	uint8 IsSwap = 0 ;
	
	// 增加对阿拉伯和希伯莱语的处理
	if(IsArabicOrHebrew)
	{
	    
		len = BAPI_CopyStr(AuxiliaryBuffer,str,(int8)len,IsSwap);
		str = AuxiliaryBuffer;
		IsSwap = 1 ;
	}
	//如果是阿拉伯和希伯来则返回与辅助字符串空间之对应的源字符串的位置
	return str_bk + CommonScan(str, TRUE, strlen,IsSwap);
	
	//
}

////*********************************************************************************/
///*
//** FUNCTION:     Sdrv_PutStringExt
//**
//** Description:  在屏幕上输出一个ANSI字符串，如串的长度超过屏幕，则多出部分不显示
//**                   指针停在最后一个能显示的字的下一个字符左上角,碰到回车或
//**                   串尾时退出，碰到换行时从下一行开始显示
//**  input
//**     str: 字符串首址 ;
//**     strlen: 字符串的长度
//**	    IsUnicode:是否Unicode字符串
//**     mode:处理模式
//**          0--等同于Puts 1--对阿拉伯和希伯莱语言进行转换
//**          2--对阿拉伯和希伯莱进行输出 4--对阿拉伯和希伯莱进行转换并且输出
//**  output
//**     char * 返回处理完后的当前字符指针
//*************************************************************************************/
uint8 * Sdrv_PutStringExt(int8 *str, int8 strlen, int8 IsUnicode, int8 mode)
{
	switch(mode)
	{
		#if 0
	case 0x00://等同于puts或者putuns
		if(IsUnicode)
		{
			return Sdrv_PutUnS(str, strlen);
		}
		else
		{
			return Sdrv_PutS(str, strlen);
		}
		#endif
	case 0x01://仅仅对阿拉伯和希伯莱字符串进行加工处理
		if(IsArabicOrHebrew)
		{
			MakeArabicHebrewLink(str, IsUnicode);
		}
		return str;
	case 0x02://仅仅输出字符串,非阿拉伯和希伯莱情况下等同于0x00模式,
		//在阿拉伯和希伯莱情况下如果没有先经过0x01处理,等于非连写模式
		return str + CommonScan(str, IsUnicode, strlen,0);
	case 0x04://等于0x01模式加上0x02模式
		if(IsArabicOrHebrew)
		{
			MakeArabicHebrewLink(str, IsUnicode);		
			IsUnicode = TRUE;
		}
		return str + CommonScan(str, IsUnicode, strlen,0);
	default:
		return str;
	}
}

