/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : Sdrv_putsdimage.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "Drv_S7565.h"
#include "display.h"
#include "string.h"




#pragma userclass(near=BAD_PSDI)
SD_RW sdrw;
//extern  unsigned char BufPage;
#pragma userclass(near=default)

#pragma renamecode(?PR?BAC_PSDI)

void Sdrv_PutSDImage(region_t *pic,uint32 imgddr)
{
	uint8 i,j,k;
	int16 cnt;
	//坐标出了屏幕，则退出
	//ybfu
    if(pic->x>Display_Length-1 || pic->y >Display_Height-1 ) return;

    //显示出屏幕的处理
    j = pic->height + pic->y;
    if(j>Display_Height) j=Display_Height;

    j=j>>3;
    k = pic->width;
    if(k > Display_Length-pic->x) k=Display_Length-pic->x;
		
	cnt = (uint32)pic->width * (uint32)pic->height /8;
	sdrw.faddr=imgddr;
	//sdrw.srampage=(input8(0x05)&0x0f);
	sdrw.srampage = 0;
	//如果是阿拉伯和希伯莱语,会先将坐标进行映射处理,然后输出到buffer完后再映射回来 by cailizhen
	RegionMapping(pic, -1);
	for(i=pic->y>>3; i< j ; i++)
	{
		sdrw.sramaddr=(uint16)DisplayBuffer+ i*Display_Length + pic->x;
		SD_Read(&sdrw,k);
		sdrw.faddr += pic->width;
	}
	RegionMapping(pic, -1);
	
	//PutImage_setPos(pic);
	//Hdrv_SetWindow(pic);    //设定要填入 的 图片的 行、列起始地址，就从这里开始填数据
	//Hdrv_BuffDataTrans(DisplayBuffer, cnt);//把显存中的数据搬到LCM中
}

