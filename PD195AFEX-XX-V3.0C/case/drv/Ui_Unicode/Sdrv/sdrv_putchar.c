/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_putchar.c
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

/*
*******************************************************************************
*
** FUNCTION:     CheckDisplaySpace
**
** Description:  检测屏幕空间是否满足显示要求，如超出范围则报错
**
**  input
**     codemod: 点阵的模式，字符点阵和图片数据
**
**  output
**     True: 屏幕空间能满足显示要求
**     False: 屏幕空间不能满足显示要求
********************************************************************************
*/
bool CheckDisplaySpace(uint8 codemod,uint8 fontwidth)
{
	uint8 justwidth;
	
	switch(codemod)
	{
	case UnicodeMode:
		justwidth = fontwidth;
		if( (StrEndX ) && ( region.x+justwidth > StrEndX))
		{
//			DUMP(RESULT_PUTCHAR_X_ERR);
			//PutCharErrFlag = RESULT_PUTCHAR_X_ERR;//X轴超出
			return FALSE;
		}
		goto NextStep;
	case PicDisMode:
		justwidth = region.width;
NextStep:
		//判断是否X轴和y轴超出
		if (region.x+justwidth > region_win.x+region_win.width)
		{
//			DUMP(RESULT_PUTCHAR_X_ERR);
			//PutCharErrFlag = RESULT_PUTCHAR_X_ERR;//X轴超出
			return FALSE;
		}
		if (region.y+region.height > region_win.y+region_win.height)
		{
//			DUMP(RESULT_PUTCHAR_Y_ERR);
			//PutCharErrFlag = RESULT_PUTCHAR_Y_ERR;//Y轴超出
			return FALSE;
		}
		break;
	default:
//		DUMP(RESULT_PUTCHAR_OTHER_ERR);
		//PutCharErrFlag = RESULT_PUTCHAR_OTHER_ERR;//不知类型的也不显示
		return FALSE;
	}
	
	return TRUE;
}

//non_banked char CheckCharMode(int code);
//bool CheckDisplaySpace(char codemod);
////*******************************************************************************/
///*
//** FUNCTION:     PutChar
//**
//** Description:  在屏幕上打出一个字符,并打完后当前指针自动跳到下一个汉字的左上角
//**
//**  input
//**     code: 字符的码值
//**     str: 传递str是为了能够访问后续的字符，以支持泰语的特殊处理
//**     IsUnicode: 表示str的类型，是否Unicode类型
//**  output
//**     signed char:-1表示越界，0表示正常，大于0表示泰语重叠了后面的字符数
//********************************************************************************/

uint8 Sdrv_PutChar(uint8 *str, uint8 IsUnicode, uint16 ucode)
{
	uint8 charmode,fontwidth;
	bool putchar_result;
	uint8 ScanLen=0;
	int8 CharWidth;
	//PutCharErrFlag = RESULT_PUTCHAR_OK; //默认为OK
	Sdrv_GetFontPoint(ucode, (uint16)CharPointBuffer);//取点阵
	
	//泰语特殊处理
	if((ucode>=0x0e01)&&(ucode<=0x0e7F))
	{
		ScanLen = BAPI_ThaiOverlap(str, IsUnicode);
	}
	
	//Hdrv_SetDrawMode(DrawModeHPic_def);    //把显示模式设置成字体显示模式
	charmode = UnicodeMode;
	//检测输出区域是否超过显示屏分辩率
	if((AsciiSelect==0)&&((uint16)ucode<0x0080))
	{
		CharPointBuffer[32]=0x06;
	}
	if(CharPointBuffer[32] > 8)
	{
		fontwidth= UNICODE_WIDTH_DEF;
	}
	else
	{
		fontwidth = UNICODE_WIDTH_DEF/2;
	}
	//region.height = UNICODE_HEIGHT_DEF;
	region.height = UNICODE_HEIGHT_DEF;
	if((AsciiSelect==0)&&((WORD)ucode<0x0080))
	{
		region.height = UNICODE_HEIGHT_DEF/2;
	}
	CharWidth =  CharPointBuffer[32];
	putchar_result = CheckDisplaySpace(charmode,CharWidth);
	if (!putchar_result)
	{
		//Hdrv_SetDrawMode(DrawModeH_def);    //把显示模式设置成横向显示模式
		return -1;//如显示范围不对，则不做显示动作而直接退出
	}
	
	//设置字模输出的大小
	if(CharWidth > 8)
	{
		region.width = UNICODE_WIDTH_DEF;
	}
	else
	{
		region.width = UNICODE_WIDTH_DEF/2;
	}
	Hdrv_SetWindow(&region);
	//如果是阿拉伯和希伯莱语,会先将坐标进行映射处理,然后输出到buffer完后再映射回来 by cailizhen
	RegionMapping(&region, CharWidth);
	//把暂时显存区的数据送到LCM
	Hdrv_BuffDataTrans(CharPointBuffer, 0);
	RegionMapping(&region, CharWidth);
	region.x += CharWidth;//坐标移到下一个字的左上角
	//TextColumn += CharPointBuffer[32];
	//Hdrv_SetDrawMode(DrawModeH_def);    //把显示模式设置成横向显示模
	return (int8)ScanLen;
}


