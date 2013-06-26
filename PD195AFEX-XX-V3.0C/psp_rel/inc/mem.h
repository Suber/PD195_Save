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
//ϵͳ������
//ϵͳ������
#define         VM_KERNEL       0x0000    //��ap_kercfg����ֵ
#define         VM_SYSTEM       0x0200   //��ap_main����ֵ
#define	        VM_AP_FWCFG     0x0400  //for modify information,��ap_main����ֵ    

//��Ӧ�ó����������ÿ������1KB
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
#define         VM_AP_GAME      0x3800      //Ԥ��6��ap��VM����


#define         VM_AP_TEL_HEAD  0x4800   //for telbook,25K
#define         VM_AP_TEL_DATA  0x4A00

#define         VM_LYRIC        0xB000

//ר�Ÿ��ļ�ϵͳʹ��
#define         VM_FSYS         0xf000

#define         VM_AP_TESTER    0x14000


#ifdef __C251__



/*  ���뱣֤BufAlloc��BufFreeƥ�䣬����һһ��Ӧ����ͬջ��ʹ��
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


//��ȡָ�����ȵ����ݡ�
//pbuf: �������ݵ�Buffer ָ�롣
//Address: vram ��ַ
//Len[1~512]: ��ȡ�ĳ��ȡ�
//�ܴ�zram��uram�ж���Ҳ�ܴ�ipm/idm�ж�����port05����
 bool VMRead(void *pbuf, uint16 address, uint16 len);             //len: �ֽ�Ϊ��λ
 bool VMExpRead(void *pbuf, uint32 address, uint16 len);         //len: �ֽ�Ϊ��λ

//��pbuf �е�����д��VRam, �̶�����Ϊ512Byte��
//pbuf: �������ݵ�Buffer ָ�롣
//Address: vram ��ַ
//����VMWrite�ǳ��������벻ҪƵ���ĵ��ã�һ������ap�˳�����ٶ�Ҫ�󲻸ߵĵط�ʹ�á�
//ֻ�ܴ�zram��uram��д�����ܴ�ipm/idm��д
 bool VMWrite(void *pbuf, uint16 address);
 bool VMExpWrite( void *pbuf, uint32 address, uint16 len, int8 page );      //address��λΪBytes��len: ����Ϊ��λ

#endif /* __C251__ */

#ifdef __A251__

#define API_VMRead      0x0001+API_VM_BaseNoA
#define API_VMWrite     0x0002+API_VM_BaseNoA
#define API_VMExpRead   0x0003+API_VM_BaseNoA
#define API_VMExpWrite  0x0004+API_VM_BaseNoA



//��������õĺ�
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


