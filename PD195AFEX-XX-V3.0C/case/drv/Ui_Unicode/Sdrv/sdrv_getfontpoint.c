/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_getfontpoint.c
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
 
** FUNCTION:    GetCharPoint
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
void Sdrv_GetFontPoint(uint16 ucode,uint16 picbuf)
{
	SD_RW sdrwfont;
	uint16  PageNum;
	sdrwfont.faddr=(uint32)( FontLibAddr+(uint32) ucode*33);
	if(((uint16)ucode<0x0080)&&(AsciiSelect==0))
	{
		sdrwfont.faddr=(uint32)( AsciiFontLibAddr+(uint32) ucode*8);
	}
	//if(picbuf < 0x4000)
	//{
	//	sdrwfont.srampage= 0;   //0x08;
	//}
	//else
	//{
		sdrwfont.srampage= 0;  //input8(0x05) & 0x07;
	//}
	sdrwfont.sramaddr = picbuf;
	
	//同一扇区
	PageNum = sdrwfont.faddr%512;
	if ( PageNum <= 479 )
	{
		SD_Read(&sdrwfont,33);
	}
	else
	{
		SD_Read(&sdrwfont,512 - PageNum );
		sdrwfont.sramaddr = picbuf+ 512 - PageNum ;
		sdrwfont.faddr+=  512 - PageNum;
		SD_Read(&sdrwfont,33 - (512 - PageNum ));
	}
	
}
