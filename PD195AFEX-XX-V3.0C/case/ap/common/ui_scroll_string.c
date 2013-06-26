/*
 *******************************************************************************
 *                ACTOS AP
 *        wma player basal module implementation
 *
 *        (c) Copyright Actions Co,Ld.
 *
 *  $Id: ui_scroll.c,v 1.1.1.1 2006/04/21 08:22:43 selina Exp $
 *******************************************************************************
 */
/*
 这个函数的作用是在汇编程序外包一层 C ,这样汇编在C中用BANK方式CALL
 */

#include "actos.h"
#include "ap_common.h"
#pragma name(COM_UI_SCROLL)

void ScrollStringInit(uint8 *str,uint8 language,region_t *scroll);
void HoriScrollString(int8 step, uint8 language, region_t *scroll);
uint16 CountString(uint8 *str, uint8 language, region_t *scroll);
#if 0
void SwapIntByte(uint16 *pword)
{
    uint8 high_byte;
    uint8 low_byte;

    high_byte = (uint8) ((*pword >> 8) & 0xff);
    low_byte = (uint8) (*pword & 0xff);

    *((uint8*) pword + 1) = high_byte;
    *((uint8*) pword) = low_byte;
}
#endif
/* (字符串首址，是否初始化true为要初始化) */
void ScrollString( uint8 * scrollstr, bool mode ,uint8 language,region_t *scroll)
{
	/*设置非透明字体*/
	//SetTextOutMode(0);
	if (mode)
	{
		//初始化滚屏（第一次调用时先显示一串字符）
		if(language !=UNICODELANGUAGE)
		{
			uint8 i;
			for(i=0;i<50;i++)
			{
				if(scroll_buf[i] == '\0')
				{
					break;
				}
			}
			/*插入空格符*/
			if(i>=50)
				i=50;
			scroll_buf[i] = ' ';
			scroll_buf[i+1] = ' ';
			scroll_buf[i+2] = '\0';
		}
		ClearScreen(scroll);
		ScrollStringInit(scrollstr,language,scroll);
	}
	else
	{
		//阿拉伯和希伯莱已经在之前被加工处理为Unicode的了 by cailizhen
		if(g_comval.langid == ARABIC||g_comval.langid == HEBREW)
		{
			language = UNICODELANGUAGE;
		}
		HoriScrollString(1,language,scroll);
	}
	 
}
 void ScrollStringInit(uint8 *str,uint8 language,region_t *scroll)
{
//	uint16 count;
	uint8 result;
	region_t kregion;//余下部分区域
	scroll_cnt = 0;
	strhead_pt = str;//文件头
	SetTextPos(scroll->x, scroll->y);
	//SetStrEndPosition(scroll->x + scroll->width);//如果
	//对阿拉伯和西伯来语进行转换，然后输出，返回结束字符的指针 by cailizhen
	str_pt =PutStringExt(str, -1, language==UNICODELANGUAGE, 0x04);
	SetStrEndPosition(0);
	if(*str_pt!=0)
	{
		GetTextPos(&kregion.x, &kregion.y);
		kregion.width = scroll->width+scroll->x - kregion.x;
		kregion.height = scroll->height;
		
		//输出最后一个字，传递str_pt，以便对泰语后面的字符叠加到当前字符上 by cailizhen
		result = CharOutRect(str_pt, &kregion, scroll_cnt, language);//
		if (result)
		{
			return;
		}
		scroll_cnt = kregion.width;
	}
}

//==========================================================
/*
滚动屏幕
*/
void HoriScrollString(int8 step,uint8 language,region_t *scroll)
{
//	uint16 ucode;
	region_t tregion;
	region_t sregion;
	region_t kregion;//余下部分区域
	uint8 Result;
	
	//余下部分区域
	kregion.x = scroll->x + scroll->width - step;
	kregion.y = scroll->y;
	kregion.width = step;
	kregion.height = scroll->height;
	
	//设定移动的源，目标区域
	sregion.x = scroll->x + step;//暂时未考虑为负数的情况
	tregion.x = scroll->x;
	tregion.y = sregion.y = scroll->y;
	tregion.height = sregion.height = scroll->height;
	tregion.width = sregion.width =scroll->width - step;
	//向左移step列
	RegionCopy(&sregion, &tregion);
	
test:
	if( language == UNICODELANGUAGE )
	{
		if( *(int *)str_pt ==0x0000 )
		{
			str_pt = strhead_pt;
			
		}
	}
	else  if (*str_pt == '\0')
	{
		str_pt = strhead_pt;
		
	}
	//传递str_pt进去，以便对泰语进行特殊处理 by cailizhen
	Result = CharOutRect(str_pt, &kregion, scroll_cnt, language);
	if (Result)	//更换最后一个符号
	{
		str_pt +=Result;
		scroll_cnt = 0;
		goto test;
	}
	
/*	if(InvertFlag)
	{
		InvertRegion(&kregion);
	}*/
	scroll_cnt += step;//未支持负数
	
}
#if 0
uint8 Count_Byte(uint8 *str,uint8 language)   //统计字数 zxs added 2007.03.26
{
	uint8 n=0;
	uint8 m=0;
	while(*str!=0)
	{
		if((*str >= 0x80)&&(language!=LAN_ID_ENGLISH))
		{
			if(m+16 > DISPLAYWIDTH)
				break;
			*str++;
			*str++;
			n++;
			m=m+16;
		}
		else
		{
			if(m+8>DISPLAYWIDTH)
				break;
			*str++;
			n++;
			m=m+8;
		}
	}
	return n;
}
#endif

