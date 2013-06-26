/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_bidi_isnsm.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "Drv_S7565.h"
#include "display.h"
#include "string.h"

// 优化 by cailizhen
//算法说明:本函数功能很简单，就是检测一个字符是否是NSM字符，总共有4种情况:
// 1，如果该字符在不属于NSM的字符集合，那么将其判断为"非NSM字符"
// 2，如果不是情况1，那么如果该字符在属于NSM的字符集合，那么将其判断为"NSM字符"
// 3，如果不是情况1和2，那么如果该字符在属于NSM的区间集合，那么将其判断为"NSM字符"
// 4，如果不属于以上3种情况，那么该字符判断为"非NSM字符"

typedef struct
{
	uint16 min;
	uint16 max;
}BetteanStruct;


#pragma userclass(hconst=IS_NSM_CONST)
const uint16 far EqualNSM[85]=
    {
        0x05BF, 0x05C1, 0x05C2, 0x05C4, 0x05C5, 0x05C7, 0x0670, 0x0711,
        0x093C, 0x094D, 0x0951, 0x0952, 0x0953, 0x0954, 0x0962, 0x0963,
        0x0981, 0x09BC, 0x09CD, 0x09E2, 0x09E3, 0x0A01, 0x0A02, 0x0A3C,
        0x0A81, 0x0A82, 0x0ABC, 0x0ACD, 0x0AE2, 0x0AE3, 0x0B01, 0x0B3C,
        0x0B3F, 0x0B4D, 0x0B56, 0x0B82, 0x0BC0, 0x0BCD, 0x0CBC, 0x0CCC,
        0x0CCD, 0x0D41, 0x0D42, 0x0D43, 0x0D4D, 0x0DCA, 0x0DD6, 0x0E31,
        0x0EB1, 0x0F18, 0x0F19, 0x0F35, 0x0F37, 0x0F39, 0x0FC6, 0x1058,
        0x1059, 0x135F, 0x1712, 0x1713, 0x1714, 0x1752, 0x1753, 0x1772,
        0x1773, 0x17C6, 0x17DD, 0x180B, 0x180C, 0x180D, 0x18A9, 0x1932,
        0x1939, 0x193A, 0x193B, 0x1A17, 0x1A18, 0x3099, 0x309A, 0xA802,
        0xA806, 0xA80B, 0xA825, 0xA826, 0xFB1E
    };
    
const uint16 far NotEqualNSM[9]=
    {
        0x06DD, 0x06E5, 0x06E6, 0x06E9, 0x07B1, 0x0F7F, 0x0F85,0x1031, 0x1038
    };
    
const BetteanStruct far BetweenStructArray[33]=
    {
        {0x0300, 0x0370},
        {0x0483, 0x048A},
        {0x0591, 0x05BE},
        {0x0610, 0x061B},
        {0x064B, 0x0660},
        {0x06D6, 0x06EE},
        {0x0730, 0x074D},
        {0x07A6, 0x0903},
        {0x0941, 0x0949},
        {0x09C1, 0x09C5},
        {0x0A41, 0x0A4E},
        {0x0A70, 0x0A72},
        {0x0AC1, 0x0AC9},
        {0x0B41, 0x0B44},
        {0x0C3E, 0x0C41},
        {0x0C46, 0x0C57},
        {0x0DD2, 0x0DD5},
        {0x0E34, 0x0E3B},
        {0x0E47, 0x0E4F},
        {0x0EB4, 0x0EBD},
        {0x0EC8, 0x0ECF},
        {0x0F90, 0x0FBE},
        {0x102D, 0x1040},
        {0x1732, 0x1735},
        {0x17B7, 0x17BE},
        {0x17C9, 0x17D4},
        {0x1920, 0x1923},
        {0x1927, 0x192C},
        {0x1DC0, 0x1DC4},
        {0x20D0, 0x20EC},
        {0x302A, 0x3030},
        {0xFE00, 0xFE10},
        {0xFE20, 0xFE30}
    };
#pragma userclass(hconst=default)
    
 

#pragma renamecode(?PR?IS_NSM)
bool GUI_BIDI_IsNSM(uint16 c)
{
	uint8 i;
	
	//返回FALSE的点
	for(i=0;i<9;i++)
	{
		if(c==NotEqualNSM[i])
			return FALSE;
	}
	
	//返回TRUE的点
	for(i=0;i<85;i++)
	{
		if(c==EqualNSM[i])
			return TRUE;
	}
	
	//返回TRUE的区间
	for(i=0;i<33;i++)
	{
		if(c>=BetweenStructArray[i].min&&c<BetweenStructArray[i].max)
			return TRUE;
	}
	return FALSE;
}


