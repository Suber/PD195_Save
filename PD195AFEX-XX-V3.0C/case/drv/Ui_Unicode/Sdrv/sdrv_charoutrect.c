/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_charoutrect.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "Drv_S7565.h"
#include "display.h"
#include "string.h"


#pragma renamecode(?PR?BBC_DW3)


////*******************************************************************************/
///*
//** FUNCTION:     Sdrv_CharOutRect
//**
//** Description:  在指定的区域上打出一个字符,超出区域部分不显示，没有超出屏幕的部分将正常显示。
//**
//**  input
//**     str: 当前字符指针，以访问后续字符以支持泰语显示
//**     CRregion: 要求输出的区域
//**     colcnt:   要求从汉字的哪一列开始输出
//**     language: 是否Unicode语言
//**  output
//**     char: 当指定列数超出该字的总列数时返回需要后移的字符数，否则返回0
//********************************************************************************/

uint8 Sdrv_CharOutRect(uint8 *str, region_t *CRregion, int8 colcnt, uint8 language)
{
//	uint8 charmode;
//	bool putchar_result;
	uint8 fontwidth;
	uint8 ScanLen;
	uint16 ucode;
//	uint16 ThaiCode;
	uint16 i,k;
	uint8 IsUnicode = (language == UNICODELANGUAGE);
	
	if(IsUnicode)
	{
		ucode = *(uint16 *)str;
		if (ucode == 0xfffe)
		{
			ucode = 0x2000;
		}
		Sdrv_SwapIntByte(&ucode);
		ScanLen = 2;
	}
	else
	{
		//汉字和ASCII的不同处理
		if(*str >= 0x80 &&MBCodeTwoFlag)
		{
			ucode = *(uint16 *)str;
			Sdrv_SwapIntByte(&ucode);
			ScanLen = 2;
		}
		else
		{
			ucode = *str;
			ScanLen =1;
		}
		ucode = BAPI_FontCodeMBToUni(ucode);
	}
	
	//PutCharErrFlag = RESULT_PUTCHAR_OK; //默认为OK
	//Hdrv_SetDrawMode(DrawModeHPic_def);    //把显示模式设置成竖向显示模式（与旧字库兼容）
	
	//Sdrv_GetFontPoint(code, (int)CharPointBuffer);//取点阵
	BAPI_GetFontPoint(ucode,(uint16)CharPointBuffer); //取点阵
	
	//对泰语特殊处理
	if((ucode>=0x0e01)&&(ucode<=0x0e7F))
	{
		ScanLen += BAPI_ThaiOverlap(str +ScanLen, IsUnicode);
	}
	
	if (CRregion->width == NULL)
	{
		return ScanLen;
	}
	
	if((AsciiSelect==0)&&((uint16)ucode<0x0080))
	{
		CharPointBuffer[32]=0x06;
	}
	if(colcnt >= CharPointBuffer[32] )                 //超出点阵宽度返回
	{
		//Hdrv_SetDrawMode(DrawModeH_def);        //把显示模式设置成横向显示模
		return ScanLen;                        //指向下一个符号
	}
	//当字符宽度大于8时，转换16X16的点阵信息
	if(CharPointBuffer[32] > 8)
	{
		fontwidth = UNICODE_WIDTH_DEF;
	}
	//当字符宽度等于小于8时，只转换8X16的点阵信息
	else
	{
		fontwidth = UNICODE_WIDTH_DEF/2;
	}
	
	//确定输出区域
	region.x = CRregion->x;
	region.y = CRregion->y;
	TextPage = CRregion->y;
	region.width = (CRregion->width < (fontwidth-colcnt))? (CRregion->width):(fontwidth-colcnt);
	region.height = UNICODE_HEIGHT_DEF;
	
	if((AsciiSelect==0)&&((uint16)ucode<0x80))
	{
		region.height = UNICODE_HEIGHT_DEF/2;
	}
	for(k=0;k<region.height/8;k++)
	{
		for(i=0;i<region.width;i++)
		{
			// 对阿拉伯和希伯莱进行平移再对换的操作 by cailizhen 2010-06-28
			// 这样可以免去在common中的滚屏进行特殊处理
			uint8 start = CharPointBuffer[32]- 1 - colcnt;
			uint8 temp = start * 2 - region.width + 1;
			if(IsArabicOrHebrew)
				CharPointBuffer[i+k*fontwidth]=
				CharPointBuffer[temp - ((CharPointBuffer[32]- 1) - (colcnt+i)) + k*fontwidth];
			else
				CharPointBuffer[i+k*fontwidth]=CharPointBuffer[colcnt+i+k*fontwidth];
		}
	}
	
	Hdrv_SetWindow(&region);
	//BlackCharToColor(fontwidth, CharPointBuffer);
	CharPointBuffer[32]=region.width;
	if(TextColumn>Display_Length)
	{
		TextColumn=CRregion->x;
	}
	if(TextColumn>=CRregion->x+CRregion->width)
	{
		TextColumn-=region.width;
	}
	else
	{
		TextColumn = CRregion->x;
	}
	//把暂时显存区的数据送到LCM
	//如果是阿拉伯和希伯莱语,会先将坐标进行映射处理,然后输出到buffer完后再映射回来 by cailizhen
	RegionMapping(&region, CharPointBuffer[32]);
	Hdrv_BuffDataTrans(CharPointBuffer, 0);
	RegionMapping(&region, CharPointBuffer[32]);
	
	//Hdrv_SetDrawMode(DrawModeH_def);    //把显示模式设置成横向显示模
	return 0;
}
