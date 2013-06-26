/*
********************************************************************************
*                       ACTOS
*                       ACTOS
*                  time define head file
*
*                (c) Copyright 2002-2003, Actions Co,Ld.
*                        All Right Reserved
*
* File   : mem.h
* By     : Gongee
* Version: 1> v1.00     first version     12/4/2003 11:13PM
********************************************************************************
********************************************************************************
*/
#ifndef _MEM_H
#define _MEM_H

#include "actapi.h"

//vm address
//系统参数区
//系统参数区
#define         VM_KERNEL       0x0000    //由ap_kercfg赋初值
#define         VM_SYSTEM       0x0200   //由ap_main赋初值
#define	        VM_AP_FWCFG     0x0400  //for modify information,由ap_main赋初值    

//各应用程序参数区，每个程序1KB
#define         VM_AP_MUSIC     0x0800
#define		    VM_AP_AMV	    0x0c00
#define         VM_AP_RECORD    0x1000
#define         VM_AP_VOICE     0x1400
#define         VM_AP_RADIO     0x1800
#define         VM_AP_View      0x1c00         // for picture view fuction  by jacklee
#define         VM_AP_SETTING   0x2000
#define	        VM_AP_READER    0x2400
#define         VM_AP_READER1   0x2800
#define	        VM_AP_BROWSER	0x2C00		//for browser file
#define         VM_AP_PEDO      0x3000      
#define         VM_AP_SWIM      0x3400 
#define         VM_AP_GAME      0x3800      //预留6个ap的VM区间


#define         VM_AP_TEL_HEAD  0x4800   //for telbook,25K
#define         VM_AP_TEL_DATA  0x4A00

#define         VM_LYRIC        0xB000

//专门给文件系统使用
#define         VM_FSYS         0xf000

#define         VM_AP_TESTER    0x14000


#ifdef __C251__



/*  必须保证BufAlloc于BufFree匹配，而且一一对应，如同栈的使用
//such as:
  right use:
       int    bufpoint0,bufpoint1;
       bufpoint0=BufAlloc(20);  //------
              ...               //     |
       bufpoint1=BufAlloc(30);  //--   |
              ...               // |   |
       BufFree(bufpoint1);      //--   |
              ...               //     |
       BufFree(bufpoint0);      //------

  wrong use:
       int    bufpoint0,bufpoint1;
       bufpoint0=BufAlloc(20);  //---
              ...               //  |
       bufpoint1=BufAlloc(30);  //--|----
              ...               //  |   |
       BufFree(bufpoint0);      //---   |
              ...               //      |
       BufFree(bufpoint1);      //------
*/
// void *BufAlloc(uint8  size);
 //void BufFree(void *buffer,uint8 size);


//读取指定长度的数据。
//pbuf: 接受数据的Buffer 指针。
//Address: vram 地址
//Len[1~512]: 读取的长度。
//能从zram或uram中读，也能从ipm/idm中读，由port05决定
 bool VMRead(void *pbuf, uint16 address, uint16 len);             //len: 字节为单位
 bool VMExpRead(void *pbuf, uint32 address, uint16 len);         //len: 字节为单位

//将pbuf 中的数据写入VRam, 固定长度为512Byte。
//pbuf: 接受数据的Buffer 指针。
//Address: vram 地址
//由于VMWrite非常的慢，请不要频繁的调用，一般是在ap退出或对速度要求不高的地方使用。
//只能从zram或uram中写，不能从ipm/idm中写
 bool VMWrite(void *pbuf, uint16 address);
 bool VMExpWrite( void *pbuf, uint32 address, uint16 len, int8 page );      //address单位为Bytes，len: 扇区为单位

#endif /* __C251__ */

#ifdef __A251__

#define API_VMRead      0x0001+API_VM_BaseNoA
#define API_VMWrite     0x0002+API_VM_BaseNoA
#define API_VMExpRead   0x0003+API_VM_BaseNoA
#define API_VMExpWrite  0x0004+API_VM_BaseNoA



//定义汇编调用的宏
mVMRead			macro
  		MOV WR8,#API_VMRead
        LCALL     RSTBankAPI
        endm

mVMWrite		macro
  		MOV WR8,#API_VMWrite
        LCALL     RSTBankAPI
        endm

mVMExpRead      macro
        MOV WR8,#API_VMExpRead
        LCALL     RSTBankAPI
        endm

mVMExpWrite     macro
        MOV WR8,#API_VMExpWrite
        LCALL     RSTBankAPI
        endm



#endif /* __A251__ */
#endif/*_MEM_H*/


