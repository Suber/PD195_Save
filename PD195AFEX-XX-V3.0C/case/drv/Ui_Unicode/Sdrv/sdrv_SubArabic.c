/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_SubArabic.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "Drv_S7565.h"
#include "display.h"
#include "string.h"


#pragma renamecode(?PR?BBC_SubArabic)

void ConvertEnglish(int8 *str,int8 CharCount)
{
	uint16 Glyph;
	uint16 prevGlyph = 0;
	uint16 nextGlyph = 0;
	uint16 store;
	int8 start;
	int8 end;
	int8 length;
	int8 i =0 ;
	bool noArabicLetter=TRUE;
	//将字符数转换为字节数
	length = (BYTE)CharCount + (BYTE)CharCount;

	// 2010-04-16 change by cailizhen from fiona at as211a unicode driver
	// 更正在阿拉伯语下其他语言的unicode显示问题
	while((str[i] != 0 || str[i+1] !=0)&& i < length)
	{
		bool flag = FALSE;
		Glyph = *(int *)(str+i);
		if(i < length - 2)
		{
			nextGlyph = *(int *)(str+i+2);
		}
		start = i;
		#if 0
		if(noArabicLetter&&((Glyph>=0x600&&Glyph<=0x6ff)||(Glyph>=0xFB50&&Glyph<=0xFDFF)||(Glyph>=0xFE70&&Glyph<=0xFEFF)||(Glyph>=0x590&&Glyph<=0x5ff)))
			noArabicLetter=FALSE;
		while(Glyph <= 0x80 && Glyph !=0x0000 && Glyph  != 0x000D&&Glyph  != 0x000A&&(noArabicLetter || Glyph!=0x2E))//0X2E='.'
		{
			i+=2;
			Glyph = *(int *)(str+i);
		}
		
		while(( Glyph == 0x000D||Glyph  == 0x000A||Glyph ==0||Glyph == ' '||Glyph ==0x2E )&&i>start)
		{
			i-=2;
			Glyph = *(int *)(str+i);
		}
		#endif
		while(i < length && Glyph < 0x80 
		&& ( (Glyph >= 0x41&&Glyph <= 0x5a)
			||(Glyph >= 0x61&&Glyph <= 0x7a)
			||(Glyph >= 0x30&&Glyph <= 0x39)
			||(Glyph == 0x2e&&prevGlyph >= 0x30&&prevGlyph <= 0x39&&nextGlyph >= 0x30&&nextGlyph <= 0x39) ) )
		{
			prevGlyph = Glyph;
			Glyph = nextGlyph;
			i+=2;
			if(i < length - 2)
			{
				nextGlyph = *(int *)(str+i+2);
			}
			else
			{
				nextGlyph = 0;
			}
			flag = TRUE;
		}        
		if(flag)
		{
			i-=2;
		}    

		end =i;
		if(start == end )//||start == (end-2) )
			i+=2;
		else //if(start > (end+2) )
		{
			while(end > start)
			{
				store=*(int *)(str+end);
				*(int *)(str+end) = *(int *)(str+start);
				*(int *)(str+start) = store;
				end -=2;
				start +=2;
			}
			i+=2;
		}
	}
}

////*********************************************************************************/
///*
//** FUNCTION:     CopyStr
//**
//** Description:  将阿拉伯和希伯莱字符串转换为连写和倒置的字符串
//**  input
//**     src: 原字符串首址
//**     DstBufLength: 目标字符串的最大长度
//**     Dst: 用于保存转换后的结果
//**
//**  output
//**     char 返回转换后的结果长度
//*************************************************************************************/
int8 CopyStr(int8 *Dst,int8 *src,int8 CharCount,uint8 IsSwap)
{
	uint16 ucode;
	int8 * cp = Dst;
	int8 NowCharCount=0;

	//if((int8)CharCount > RowWordsCountMax)
		CharCount = RowWordsCountMax;

	//如果Dst不等于src，那么将src拷贝到Dst中
	while(1)
	{
		ucode = *(uint16*)src;
		if(IsSwap == 0)
		{
		    Sdrv_SwapIntByte(&ucode);
		}
		if(ucode == 0 || (NowCharCount  >= (int8)CharCount))
			break;
		*(uint16*)cp = ucode;
		cp+=2;
		src+=2;
		NowCharCount ++;
	}
	*(uint16*)cp = 0;

	//进行阿拉伯和希伯莱的数字和英文字母倒序
	ConvertEnglish( Dst,(int8)NowCharCount);
	//对阿拉伯进行连写处理
	if(Language_ID == ARABIC)
	{
		// 返回长度可能和传进去的长度不一致
		NowCharCount = BAPI_UnicodeTOArabic( Dst,(int8)NowCharCount);
	}
	
	return NowCharCount;
}


