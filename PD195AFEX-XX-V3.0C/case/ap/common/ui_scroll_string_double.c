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
#pragma name(COM_UI_SCROLL2)


//===============
uint8 *str_pt2;//字符串当前指针
uint8 *strhead_pt2;//字串首址记录
uint8 scroll_cnt2;
//uint8 ScrollFlag2;
uint8 scroll_buf2[104];
region_t region_scroll2; //滚屏
uint8 InvertFlag2;
//================


void ScrollStringInit2(uint8 *str,uint8 language,region_t *scroll);
void HoriScrollString2(int8 step, uint8 language, region_t *scroll);

/* (字符串首址，是否初始化true为要初始化) */
void ScrollString2( uint8 * scrollstr, bool mode ,uint8 language,region_t *scroll)
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
				if(scroll_buf2[i] == '\0')
				{
					break;
				}
			}
			/*插入空格符*/
			if(i>=50)
				i=50;
			scroll_buf2[i] = ' ';
			scroll_buf2[i+1] = ' ';
			scroll_buf2[i+2] = '\0';
		}
		ClearScreen(scroll);
		ScrollStringInit2(scrollstr,language,scroll);
	}
	else
	{
		//阿拉伯和希伯莱已经在之前被加工处理为Unicode的了 by cailizhen
		if(g_comval.langid == ARABIC||g_comval.langid == HEBREW)
		{
			language = UNICODELANGUAGE;
		}
		HoriScrollString2(1,language,scroll);
	}
	 
}

void ScrollStringInit2(uint8 *str,uint8 language,region_t *scroll)
{
//	uint16 count;
	uint8 result;
	region_t kregion;//余下部分区域
	scroll_cnt2 = 0;
	strhead_pt2 = str;
	SetTextPos(scroll->x, scroll->y);
	//SetStrEndPosition(scroll->x + scroll->width);
	//对阿拉伯和西伯来语进行转换，然后输出，返回结束字符的指针 by cailizhen
	str_pt2 =PutStringExt(str, -1, language==UNICODELANGUAGE, 0x04);
	SetStrEndPosition(0);
	if(*str_pt2!=0)
	{
		GetTextPos(&kregion.x, &kregion.y);
		kregion.width = scroll->width+scroll->x - kregion.x;
		kregion.height = scroll->height;
		
		//输出最后一个字，传递str_pt，以便对泰语后面的字符叠加到当前字符上 by cailizhen
		result = CharOutRect(str_pt2, &kregion, scroll_cnt2, language);
		if (result)
		{
			return;
		}
		scroll_cnt2 = kregion.width;
	}
}

//==========================================================
/*
滚动屏幕
*/
void HoriScrollString2(int8 step,uint8 language,region_t *scroll)
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
		if( *(int *)str_pt2 ==0x0000 )
		{
			str_pt2 = strhead_pt2;
		}
	}
	else  if (*str_pt2 == '\0')
	{
		str_pt2 = strhead_pt2;
	}
	//传递str_pt进去，以便对泰语进行特殊处理 by cailizhen
	Result = CharOutRect(str_pt2, &kregion, scroll_cnt2, language);
	if (Result)	//更换最后一个符号
	{
		str_pt2 +=Result;
		scroll_cnt2 = 0;
		goto test;
	}
	
	/*if(InvertFlag2)
	{
		InvertRegion(&kregion);
	}*/
	scroll_cnt2 += step;//未支持负数
	
}
/*
 ********************************************************************************
 * Description : 在指定位置滚动显示多语言字符串
 *
 * Arguments   : id, 资源代号
 x, y, 屏幕坐标,x=255 表示x方向居中
 *       scroll_width, 滚屏区域的宽度
 * Returns     : 成功, 1
 失败, 0
 *
 * Notes       :
 *
 ********************************************************************************
 */
 
uint16 ResShowMultiString_Scroll2(uint16 str_id, uint8 x, uint8 y, uint8 scroll_width)
{
#if 1
    uint8 str_len;
    uint8 strWidth;
    uint16 str_size;
    res_langid_entry_t res_langid_entry;
    /*读取字符串ID索引信息 */
    memset(scroll_buf2, 0, sizeof(scroll_buf2));
	str_len=scroll_width ;
    str_size = sizeof(res_langid_entry_t);
    SD_FSeek(res_fp, SEEK_SET, string_entry_offset + (uint32) (str_id * str_size)); // 指针指向字符在文件中的位置
    SD_FRead(res_fp, &res_langid_entry, str_size); //res_fp->&res_entry
    SIM_ChgLongPC((BYTE *) &res_langid_entry.dwOffset);
    SIM_ChgWordPC((BYTE *) &res_langid_entry.dwLength);

    /*读取字符串内容，unicode编码 */
    //    str_len = (uint8) (res_langid_entry.dwLength < 100) ? (res_langid_entry.dwLength) : (100);
    if (res_langid_entry.dwLength < 100)
    {
        str_len = (uint8) res_langid_entry.dwLength;
    }
    else
    {
        str_len = 100;
    }
    SD_FSeek(res_fp, SEEK_SET, res_langid_entry.dwOffset);
    SD_FRead(res_fp, scroll_buf2, str_len);

    /*是否居中显示字符串 */
    strWidth = GetStringLength(scroll_buf2, (uint16) str_len / 2, 1);
    if (strWidth > 96)
    {
        /*init scroll parameters*/
        region_scroll2.width = 96;
        region_scroll2.height = SCROLL_REGION_HEIGHT;
        region_scroll2.x = x;
        region_scroll2.y = y;
        scroll_buf2[str_len - 2] = ' ';
        scroll_buf2[str_len - 1] = 0x00;
        scroll_buf2[str_len] = ' ';
        scroll_buf2[str_len + 1] = 0x00;
        scroll_buf2[str_len + 2] = 0x00;
        scroll_buf2[str_len + 3] = 0x00;
        ScrollFlag2 = TRUE;
        ScrollString2(scroll_buf2, TRUE,UNICODELANGUAGE , &region_scroll2);// ARABICHASJOIN
    }
    else
    {
        ScrollFlag2 = FALSE;
        SetTextPos(x, y);
        PutUnS(scroll_buf2, -1);//ARABICHASJOIN
    }
#endif
    return 1;
    
}
void ScrollString2_re(void)
{
  ScrollString2(scroll_buf2, FALSE,UNICODELANGUAGE,&region_scroll2);
}