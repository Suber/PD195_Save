/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2003, Actions Co,Ld.
*                        All Right Reserved
*
* File   : UI_ShowPic.msa
* Version: 1> v1.00     first version     10/17/2003 10:46PM
********************************************************************************
*/

$include (actos.inc)
#include "display.h"
#include "Drv_S7565.h"

NAME  Drv_Entrance

PUBLIC Drv_Entrance{RPrefix}

EXTRN CODE(Hdrv_UpdateScreen{RPrefix})                                     
//EXTRN CODE(Sdrv_SetTextOutMode{RPrefix})                   
EXTRN CODE(Sdrv_SetAsciiFont{RPrefix})                 
EXTRN CODE(Hdrv_SetWindow{RPrefix})   
EXTRN CODE(Hdrv_BuffDataTrans{RPrefix})                  
EXTRN CODE(Sdrv_SetStrEndPosition{RPrefix})  
EXTRN CODE(Hdrv_LightOnOff{RPrefix})
EXTRN CODE(Hdrv_SetContrast{RPrefix})
EXTRN CODE(Hdrv_StandbyScreen{RPrefix})


//-----------API 号与函数的对应表-------------
#define FUNAPI_0    Drv_Entrance_ret   
#define FUNAPI_1    Hdrv_UpdateScreen{RPrefix}    
//#define FUNAPI_2    Sdrv_SetTextOutMode{RPrefix}      
#define FUNAPI_3    Sdrv_SetAsciiFont{RPrefix}
#define FUNAPI_4    Sdrv_SetStrEndPosition{RPrefix}     
#define FUNAPI_5    Hdrv_BuffDataTrans{RPrefix}
#define FUNAPI_6    Hdrv_SetWindow{RPrefix}
#define FUNAPI_7    Hdrv_LightOnOff{RPrefix} 
#define FUNAPI_8    Hdrv_SetContrast{RPrefix} 
#define FUNAPI_9    Hdrv_StandbyScreen{RPrefix} 




//---------------------------------------------

UI_RCODE_1 SEGMENT CODE
rseg    UI_RCODE_1

;采用循环方式取址,WR8传入API_NUM,R12做比较寄存器
;并乘2以保证TABLE字节地址偏移 
Drv_Entrance{RPrefix}:
        PUSH    R11
        PUSH    R0
        MOV     R11, R9              
        MOV     R0,  #0x00
		MOV     WR18, #UI_API_TABLE
loop:
		CMP     R0, R11
		JNC     API_Ent
		INC     R0, #0x01
		LJMP    loop    
API_Ent:
        RL      A
		MOV     R10, #0x00
		ADD     WR18, WR10
		MOV     WR16, #0xff
		MOV     WR20, @DR16
		
		POP     R0
		POP     R11                 
		LJMP    @WR20
Drv_Entrance_ret:
		RET
	

UI_API_TB SEGMENT CODE
rseg    UI_API_TB

UI_API_TABLE:
        DW  WORD0(FUNAPI_0)
		DW  WORD0(FUNAPI_1)
		DW  WORD0(FUNAPI_3)     		
		DW  WORD0(FUNAPI_4)
		DW  WORD0(FUNAPI_5)
		DW  WORD0(FUNAPI_6)
		DW  WORD0(FUNAPI_7)
		DW  WORD0(FUNAPI_8)
		DW  WORD0(FUNAPI_9)
	
END