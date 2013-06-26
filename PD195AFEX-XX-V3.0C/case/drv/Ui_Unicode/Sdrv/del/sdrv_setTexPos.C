/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_setTexPos.C
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "Drv_S7565.h"
#include "display.h"
#include "string.h"



#pragma renamecode(UI_RCODE)


//********************************************************
/*
** FUNCTION:    SetAsciiFont
**
** DESCRIPTION: 设置文本显示的位置
**
** PARAMETERS:
**  input
**      e=在屏幕第几列。0~127
**      c=在屏幕第几行。0~31
**  output
**      none
*/
//***********************************************************
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