 /*
********************************************************************************
*                       ACTOS
*                  driver demo
*
*                (c) Copyright 2002-2003, Actions Co,Ld.
*                        All Right Reserved
*
* File   : UI_S6B33B0A.A51
* Version: 1> v1.00     first version     date
********************************************************************************
*/

#include "display.h"
#include "Drv_S7565.h"


		NAME DRV_GLOBAL_VARS

		PUBLIC 	DisplayBuffer
		PUBLIC  FontLibAddr
		PUBLIC	MBToUniTabAddr
		PUBLIC	CharPointBuffer
		PUBLIC	Lineleft
		PUBLIC	CEPort_Sav
		PUBLIC	TranMode
		PUBLIC	PicMode
		PUBLIC	LCDDrawMode
		PUBLIC	dc
		PUBLIC	region
		PUBLIC	region_x
		PUBLIC	region_y
		PUBLIC	region_width
		PUBLIC	region_height
		PUBLIC	region_win
		PUBLIC	set_region_addr
		PUBLIC	reg_addr
		PUBLIC	regx
		PUBLIC	regy
		PUBLIC	regw
		PUBLIC	regh
		PUBLIC	ClarityFlag
		PUBLIC	charclarity
		PUBLIC	charinvflag
		PUBLIC	PutCharErrFlag
		PUBLIC	putCharChFlag 
		PUBLIC  StrEndX
		PUBLIC  dma_mode
		PUBLIC  printharpoint
		//PUBLIC  ThaiPointBuf
    PUBLIC  Thai_lan
    PUBLIC  TwoByteLan  
    PUBLIC  PrevAffectsNextJoining
    PUBLIC  backup_endstrx
    PUBLIC  backup_curstrx
    PUBLIC  Language_ID
    public  UniBuffer
		public  ArabicBuf
		public reent_flag
		public  cesel
		public  AsciiSelect
	    public  TextColumn
	    public  TextPage
	    public  IsArabicOrHebrew
	    public  AuxiliaryBuffer
	    public  AsciiFontLibAddr
	    public  MBCodeTwoFlag
	    public  StrHeight
	    public  AscFontSmall
	    public  CharWidth
		
		DISPLAY_BUFF SEGMENT EDATA
        RSEG   DISPLAY_BUFF
DisplayBuffer:           DS    DisplayBufLen


		UI_GLOBAL_DATA SEGMENT EDATA
		RSEG   UI_GLOBAL_DATA

FontLibAddr:             ds 4//DB    0,0,0,0
MBToUniTabAddr:          ds 4//DB    0,0,0,0
CharPointBuffer:         DS 33      //最后一个字节为字体宽度信息 

Lineleft:                 ds 2//DB    0         //USB显示模式下显示buffer位于zram哪个page
CEPort_Sav:              ds 1//DB    0         //保存原来的 CE 值 

//彩屏专用 
//传送数据类型 有NORTransMode_def, DMATransMode_def 两种 
TranMode:                ds 1//DB    NORTransMode_def

PicMode:                 ds 1//DB    0          
LCDDrawMode:             ds 1//DB    DrawModeH_def                   

//为绘图设备开空间 
dc:
                         dsw 1//DW    0	
			             dsw 1//DW    0x0000//PenColor,画笔和字体颜色 
                         dsw 1//DW    0 
          				 dsw 1//DW    0x0000//BackgdColor,背景颜色:黑色

//region_t region;//记录区域坐标 
region:
region_x:                 ds 1//DB    0//列位置(x轴)
region_y:                 ds 1//DB    0//行位置(y轴)
region_width:             ds 1 //DB    0//宽度  
region_height:            ds 1 //DB    0//高度 
						  
//设定的输出窗口区域（初始值为屏幕的大小） 
region_win:                
                         ds 1 //DB    0
                         ds 1 //DB    0
                         ds 1 //DB    Displaylength
                         ds 1 //DB    Displayhight                                                

//------------------------
set_region_addr:
             			ds 1 //DB    0
            			ds 1 //DB    0
            			ds 1 //DB    0
            			ds 1 //DB    0

//-------------------------
reg_addr:
regx:                    ds 1 //DB    0
regy:                    ds 1 //DB    0
regw:                    ds 1 //DB    0
regh:                    ds 1 //DB    0

//字符透明度 
ClarityFlag:             ds 1 //DB    0                                                                     
charclarity:             ds 1 //DB    0//字符的透明度 
charinvflag:             ds 1 //DB    0//字符反相标志 (0时不反相，1时反相)
PutCharErrFlag:          ds 1 //DB    0//
putCharChFlag:           ds 1 //DB    8       
StrEndX:                 ds 1 //DB    0//字符串输出的列的结束位置
dma_mode:                ds 1 //picType,RGB or YCBCR,初始化为1（YCBCR）  
printharpoint:           ds 1//打点方式
//ThaiPointBuf:            ds 33   
Thai_lan:                ds 1    
TwoByteLan:              ds 1 
PrevAffectsNextJoining:  dsw 1   
backup_endstrx:          ds 1
backup_curstrx:          ds 1
Language_ID:             ds 1
UniBuffer:               ds 4
ArabicBuf:               ds 4
reent_flag:              ds 1
cesel:                   ds 1
AsciiSelect:             ds 1
TextColumn:              ds 1
TextPage:                ds 1
IsArabicOrHebrew:        ds 1
AuxiliaryBuffer:		 ds	AuxiliaryBufferLen
AsciiFontLibAddr:        ds 4
MBCodeTwoFlag:            ds 1
StrHeight:                 ds 1
AscFontSmall:              ds 1
CharWidth:                 ds 1
/*********************** edata val init ***********************/

        ?C_INITEDATA	SEGMENT     HCONST
		RSEG    ?C_INITEDATA
				
        DW  	00004H
        DW  	WORD0(FontLibAddr+0)
        DB  	00H, 00H, 00H, 00H

		DW  	00004H
        DW  	WORD0(MBToUniTabAddr+0)
        DB  	00H, 00H, 00H, 00H

		DW  	00002H
        DW  	WORD0(Lineleft+0)
        DB  	00H, 00H

		DW  	00001H
        DW  	WORD0(CEPort_Sav+0)
        DB  	00H

		DW  	00001H
        DW  	WORD0(TranMode+0)
        DB  	NORTransMode_def

		DW  	00001H
        DW  	WORD0(PicMode+0)
        DB  	00H

		DW  	00001H
        DW  	WORD0(LCDDrawMode+0)
        DB  	DrawModeH_def

		DW  	00008H
        DW  	WORD0(dc+0)
        DB  	00H, 00H, 00H, 00H, 00H, 00H, 00H, 00H

		DW  	00004H
        DW  	WORD0(region+0)
        DB  	00H, 00H, 00H, 00H
		
		DW  	00004H
        DW  	WORD0(region_win+0)
        DB  	00H, 00H, Display_Length, Display_Height

		DW  	00004H
        DW  	WORD0(set_region_addr+0)
        DB  	00H, 00H, 00H, 00H	
		
	    DW  	00001H
        DW  	WORD0(ClarityFlag+0)
        DB  	00H
		
		DW  	00001H
        DW  	WORD0(charclarity+0)
        DB  	00H	

		DW  	00001H
        DW  	WORD0(charinvflag+0)
        DB  	00H

		DW  	00001H
        DW  	WORD0(PutCharErrFlag+0)
        DB  	00H

	    DW  	00001H
        DW  	WORD0(putCharChFlag+0)
        DB  	08H

		DW  	00001H
        DW  	WORD0(StrEndX+0)
        DB  	00H

		DW      00001H
		DW      WORD0(dma_mode+0)
		DB      00H
		
		DW      00001H
		DW      WORD0(printharpoint+0)
		DB      00H
		
		DW      00001H
		DW      WORD0(Thai_lan+0)
		DB      00H
		
		DW      00001H
		DW      WORD0(TwoByteLan+0)
		DB      00H
		
		DW      00002H
		DW      WORD0(PrevAffectsNextJoining+0)
		DB      00H,00H
		
	  DW      00001H
		DW      WORD0(backup_endstrx+0)
		DB      00H

	  DW      00001H
		DW      WORD0(backup_curstrx+0)
		DB      00H

	    DW      00004H
		DW      WORD0(UniBuffer+0)
		DB      00H,00H,26H,00H

	    DW      00004H
		DW      WORD0(ArabicBuf+0)
		DB      00H,00H,27H,00H
	    DW      00001H
		DW      WORD0(reent_flag+0)
		DB      00H
 
END
