/*
*******************************************************************************
*                        ACTOS
*                        API HEAD FILE
*
*                (c) Copyright 2002, Actions Co,Ld.
*                        All Right Reserved
*
* File: API.h
* By  : Gongee
*******************************************************************************
*/
#ifndef _ACTAPI_H_
#define _ACTAPI_H_
#include "ak211x.h"
//BankA/BankB Define
//**************************************************************************
//
#define 	BANKBHEAD_SIZE			    19

//Software Interrupt entrance 
//**************************************************************************
//api entry
#define RSTBASE				    0xC0
#define FARRSTBASE				0xff00c0

#define RSTFastAPI              RSTBASE + 0x08    //快速api
#define FFSDCardAPI             RSTBASE + 0x0c    //快速SD卡API
#define RSTBankAPI              RSTBASE + 0x10    //bank api(相对慢速)
//#define I2CDrvAPI               RSTBASE + 0x14    //I2C Driver 入口
#define RSTMIDAPI              	RSTBASE + 0x14    //MID 入口
#define RSTStgAPI               RSTBASE + 0x18    //存储类驱动专用api入口
//#define SDRAMAPI                RSTBASE + 0x1c    //SDRAM Driver 入口 
#define RSTFARAPI				FARRSTBASE	+ 0x1C	   //Huge模式调用API
#define RSTDisplayAPI           RSTBASE + 0x20    //显示类驱动专用api入口
#define RSTFS2API               RSTBASE + 0x24    //文件系统驱动专用api入口
#define RSTFSAPI                RSTBASE + 0x28    //文件系统驱动专用api入口
#define UHOSTAPI				RSTBASE +0x2C	  //UHOST API
#define RSTSysErrAPI            RSTBASE + 0x30    //系统出错处理专用api入口
#define RSTFFReadAPI            RSTBASE + 0x38    //SD_Read快速接口
#define DELAYFunAPI 			0x90
//**************************************************************************
#define API_SDDR_FastNo         0x00
#define API_MSG_FastNo          0x10
#define API_APM_FastNo          0x20
#define API_MEM_FastNo          0x30
//bank a api resource
//-------------------------------------
//系统类函数的api bank a 资源分配
//-------------------------------------
#define API_IRQ_BaseNoA         0x0000        //irq manager in bank a
#define API_SYSERR              0x0001        //system error 
 
#define API_ADFU_BaseNoA        0x0002        //adfu

#define API_TM_BaseNoA          0x0008        //time manager in bank a
#define API_SDFS_BaseNoA        0x0010        //sd file system in bank a
#define API_MSG_BaseNoA         0x0018        //message system in bank a  
#define API_DEVICE_BaseNoA      0x0020        //device in bank a
//-------------------------------------
//驱动类函数的api bank a 资源分配
//-------------------------------------
//存储类驱动固定要分配16 bank a 资源 
#define API_STG_BaseNoA         0x0080
//虚拟内存类驱动固定要分配16 bank a 资源 
#define API_VM_BaseNoA          0x0180
//显示驱动固定要分配16 bank a 资源      
#define API_UI_BaseNoA          0x0280
#define API_UI_BaseNoA_G        0x02
//键盘驱动固定要分配16 bank a 资源 
#define API_KY_BaseNoA          0x0380  
//文件系统驱动固定要分配16 bank a 资源
#define API_FS_BaseNoA          0x0480
//快速SD卡驱动固定分配16 个bank a资源
#define API_SDCardFF_BaseNoA    0x0580
//I2C驱动固定分配16个Banka资源
#define API_I2C_BaseNoA         0x0680
#define API_SDRAM_BaseNoA       0x0780  
//**************************************************************************
//bank b api resource
//-------------------------------------
//系统类函数的api bank b 资源分配
//-------------------------------------
#define API_DRV_BaseNoB         0x8000  //驱动管理        
#define API_APM_BaseNoB         0x8008  //ap管理  
#define API_DEVICE_BaseNoB      0x8010  //device 管理  
//-------------------------------------
//驱动类函数的api bank b 资源分配
//-------------------------------------
//文件系统固定要分配32 bank b 资源 
#define API_FS_BaseNoB          0x8080  
//显示驱动固定要分配16 bank b 资源 
#define API_UI_BaseNoB          0x8280 
#define API_UI_BaseNoB_G        0x02  
//存储器驱动固定要分配16 bank b 资源
#define API_STG_BaseNoB         0x8380
//存储器驱动固定要分配16 bank b 资源
#define API_SDCardFF_BaseNoB    0x8480      
#define API_SDRAM_BaseNoB       0x8780                                              
#define API_CardPowerOn     0x05000 + API_KY_BaseNoA        //05的取值根据ky_hard_bnkif.msa中的排列                                             
//**************************************************************************

//快速API号，统一定义，便于修改
#define API_SD_Read             0x00
#define API_SD_FRead            0x01  
#define API_SD_FSeek            0x02 
#define API_SD_FTell            0x03
#define API_SD_FClose           0x04
#define API_MSG_PutSysMsg       0x05
#define API_MSG_GetSysMsg       0x06
#define API_ExecAP      		0x07
#define API_BankSwitch      	0x08
#define API_PutTSR 				0x09
#define API_KillTSR   			0x0a
#define API_RunTSR  			0x0b
//#define API_BufAlloc    		0x0c	
//#define API_BufFree     		0x0d
//***********************************************************************
#endif /*_ACTAPI_H */