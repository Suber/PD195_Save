/*
********************************************************************************
*                       ACTOS
*                  usb disk driver head file
*
*                (c) Copyright 2002-2003, Actions Co,Ld.
*                        All Right Reserved
*
* File   : uddrv.h
* By     : Gongee
* Version: 1> v1.00     first version     2003-11-20 11:34
********************************************************************************
********************************************************************************
*/
#ifndef _UDDRV_H
#define _UDDRV_H

#define  MODE_FAST  0x00
#define  MODE_SLOW  0x01

#include "actapi.h"
#include "TypeExt.h"
#include "sysdef.h"
#include "driver.h"
#include "port.h"
#include "irq.h"


#ifdef ICVer3951
    #define EccSupport
#endif

#ifdef __A251__
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* for assemble
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define SDCardFFEntry   0x7f80

#define API_UD_SectorRead       0x00
#define API_UD_SectorWrite      0x01
#define API_UD_WordRead         0x02
#define API_UD_DwordRead        0x03
#define API_UD_Update           0x04
#define API_UD_ClosePage        0x05
#define API_UD_FlashType        0x06
#define API_UD_Get_Flash_Page_Size      0x07
// 8 and 9 have used for bred read write

#define API_UD_SDCardSectorRead       0x00
#define API_UD_SDCardSectorWrite      0x01
#define API_UD_SDCardWordRead         0x02
#define API_UD_SDCardDwordRead        0x03
#define API_UD_SDCardUpdate           0x04
#define API_UD_SDCardWriteInit        0x05
#define API_UD_SDCardReadInit         0x06
#define API_UD_SDCardGetCap           0x07
#define API_UD_SDCardWPDetect         0x08
#define API_UD_SDCardInit             0x0000 + API_SDCardFF_BaseNoB
#define API_UD_SDCardExit             0x0001 + API_SDCardFF_BaseNoB
#define API_UD_SDCardPhyGetCap        0x0001 + API_SDCardFF_BaseNoA

#define API_UD_Init             0x0000+API_STG_BaseNoB
#define API_UD_Exit             0x1000+API_STG_BaseNoB

#define API_UD_SetHighFreq      0x0001+API_STG_BaseNoB
#define API_UD_SetLowFreq       0x1001+API_STG_BaseNoB

#define API_UD_GetCap           0x0002+API_STG_BaseNoB
#define API_UD_GET_SCTR_SIZE    0x1002+API_STG_BaseNoB

#define API_UD_PhyGetCap        0x0001+API_STG_BaseNoA
//#define API_UD_GetCap           0x0002+API_STG_BaseNoA
#define API_UD_WPDetect         0x0003+API_STG_BaseNoA
//#define API_UD_SetHighFreq      0x0005+API_STG_BaseNoA
//#define API_UD_SetLowFreq       0x1005+API_STG_BaseNoA
#define API_UD_EblSvMd          0x0006+API_STG_BaseNoA
#define API_UD_DisblSvMd        0x1006+API_STG_BaseNoA
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//for card fast driver

mUD_SDCardInit	macro
        MOV		WR8, #API_UD_SDCardInit
        LCALL  RSTBankAPI
        endm

mUD_SDCardExit  macro
        MOV  WR8,#API_UD_SDCardExit
        LCALL  RSTBankAPI
        endm



//for flash and card
mUD_SectorRead  macro
        MOV		R10, #API_UD_SectorRead
        LCALL     RSTStgAPI
        endm
mGD_SectorRead  macro
        MOV		R10, #API_UD_SectorRead
        LCALL     RSTStgAPI
        endm
        
mUD_SDCardSectorRead macro
		MOV		R10,#API_UD_SDCardSectorRead
		LCALL	FFSDCardAPI
		endm
mUD_SDCardSectorWrite macro
		MOV		R10,#API_UD_SDCardSectorWrite
		LCALL	FFSDCardAPI
		endm
		
mUD_SectorWrite macro
        MOV		R10, #API_UD_SectorWrite
        LCALL     RSTStgAPI
        endm
mGD_SectorWrite macro
        MOV		R10, #API_UD_SectorWrite
        LCALL     RSTStgAPI
        endm

mUD_WordRead  macro
        MOV		R10, #API_UD_WordRead
        LCALL     RSTStgAPI
        endm

mUD_DwordRead   macro
        MOV		R10, #API_UD_DwordRead
        LCALL     RSTStgAPI
        endm

mUD_Update      macro
        MOV		R10, #API_UD_Update
        LCALL     RSTStgAPI
        endm
mGD_Update      macro
        MOV		R10, #API_UD_Update
        LCALL     RSTStgAPI
        endm
mUD_ClosePage   macro
        MOV		R10, #API_UD_ClosePage
        LCALL     RSTStgAPI
        endm
mUD_GetFlashType  macro                 //����ֵ  ��MLC flash:0    MLC flash:1
        MOV		R10, #API_UD_FlashType
        LCALL     RSTStgAPI
        endm
        
mGet_Flash_Page_Size  macro
        MOV		R10, #API_UD_Get_Flash_Page_Size
        LCALL     RSTStgAPI
        endm

mUD_GetCap macro
        MOV  WR8,#API_UD_GetCap
        LCALL  RSTBankAPI
        endm
mUD_GetSctrSize macro
        MOV  WR8,#API_UD_GET_SCTR_SIZE
        LCALL  RSTBankAPI
        endm
mUD_WPDetect macro
        MOV  WR8,#API_UD_WPDetect
        LCALL  RSTBankAPI
        endm

mUD_Init        macro
        MOV  WR8,#API_UD_Init
        LCALL  RSTBankAPI
        endm
mUD_Exit        macro
        MOV  WR8,#API_UD_Exit
        LCALL  RSTBankAPI
        endm

mUD_SetHighFrequence        macro
        MOV  WR8,#API_UD_SetHighFreq
        LCALL  RSTBankAPI
        endm

mUD_SetLowFrequence         macro
        MOV  WR8,#API_UD_SetLowFreq
        LCALL  RSTBankAPI
        endm


#endif /* __A251__ */


#ifdef __C251__

typedef int lw;

typedef struct
{
        uint32        lba;
        int8         reserv;
        uint8         srampage;
        uint16       sramaddr;

}UD_RW;

typedef struct
{
        int8    ceinfo;
        int8    type;
}CE_INFO;

/*cap[0~3] is for nand flash
cap[4] is for the SM card, cap[5] is for the sd/mmc card ,cap[6] is for cf card, CAP[7] is for hd card*/

typedef struct
{
        CE_INFO ce_info[4];
        uint16    cap[8];
}STG_INFO;

//added by dongzh

//**UD_WordRead**//
/*
�������ƣ�  UD_WordRead
�������ܣ�  ����ָ��������ַ����
�������룺  UD_RW *RWStruct
�������أ�  1 success; 0 fail
���ݽṹ��
        typedef struct
        {
            uint32   lba;        //word���������ڵĵ�ַ
            int8    reserv;     //δ��
            void    byteaddr;   //word�������ڵ��ֽ�ƫ��
            uint8    reserv;     //δ��
        }UD_RW;
*/
 int  UD_WordRead(UD_RW *rwaddr);



//**UD_DwordRead**//
/*
�������ƣ�  UD_DwordRead
�������ܣ�  ����ָ��������ַ����
�������룺  UD_RW *RWStruct
�������أ�  1 success; 0 fail
���ݽṹ��
        typedef struct
        {
            uint32   lba;        //dword���������ڵĵ�ַ
            int8    reserv;     //δ��
            void    byteaddr;   //dword�������ڵ��ֽ�ƫ��
            uint8    reserv;     //δ��
        }UD_RW;
*/
 long int UD_DwordRead(UD_RW *rwaddr);



//**UD_SectorRead**//
/*
�������ƣ�  UD_SectorRead
�������ܣ�  ��512 byte���ݵ�ָ���ڴ�λ��
�������룺  UD_RW *RWStruct
�������أ�  0   success;
            1   ��ַ��������粻���ڻ�Խ��ĵ�ַ
            3   ����������Ҫ���¼��
���ݽṹ��
        typedef struct
        {
            uint32   lba;        //LBA��д��ַ����512 BYTEΪ��λ
            int8    endflag;    //���Ʋ���������5A���ڽ���������д������Ϊ0
            void    *sramaddr;  //��д���ڴ��еĵ�ַ
            uint8    srampage;   //�ڴ�ҳ 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/
 int8 UD_SectorRead(UD_RW *rwaddr);

//**UD_SectorWrite**//
/*
�������ƣ�  UD_SectorWrite
�������ܣ�  д512 byte���ݵ�ָ������ָ������
�������룺  UD_RW *RWStruct
�������أ�  0   success;
            1   ��ַ��������粻���ڻ�Խ��ĵ�ַ
            2   д��������
            3   ����������Ҫ���¼��
���ݽṹ��
        typedef struct
        {
            uint32   lba;        //LBA��д��ַ����512 BYTEΪ��λ
            int8    endflag;    //���Ʋ���������5A���ڽ���������д������Ϊ0
            void    *sramaddr;  //��д���ڴ��еĵ�ַ
            uint8    srampage;   //�ڴ�ҳ 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/
 int8 UD_SectorWrite(UD_RW *rwaddr);

//**UD_WPDetect**//
/*
�������ƣ�  UD_WPDetect
����������  ���FLASH�Ƿ�д����
�������룺
�������أ�  0 û��д����
            1 д����
���ݽṹ��  ��
*/
 int8 UD_WPDetect(void);

//**UD_GetCap**//
/*
�������ƣ�  UD_GetCap
����������  ���FLASH����
�������룺
�������أ�  HL FLASH��������128KΪ��λ
���ݽṹ��  ��
*/
 uint16 UD_GetCap(void);
/*
*********************************************************************************************************
*                                           UD_GetSctrSize
*
* Description: ��ȡ��ǰ�������̷�һ������������(�����U��һ����������Ϊ512byte��������)
*
* Arguments  : None.
*
* Returns    : a: sector size, 2��ʾΪ1024byte, so on.
*
* Note(s)    : None.
*********************************************************************************************************
*/
int8 UD_GetSctrSize(void);
//**UD_Update**//
/*
�������ƣ�  UD_Update
����������  д�����һ�����ݵ�FLASH
�������룺
�������أ�  ��
���ݽṹ��  ��
*/
 void UD_Update(void);


//**UD_GetFlashType**//
/*
�������ƣ�  UD_GetFlashType
����������  ȡ��flash �����Ƿ�MLC
�������룺
�������أ�  ����ֵ  ��MLC flash:0    MLC flash:1
���ݽṹ��  ��
*/
 char UD_GetFlashType(void);  //����ֵ  ��MLC flash:0    MLC flash:1

//**Get_Flash_Page_Size**//
/*
�������ƣ�  Get_Flash_Page_Size
����������  ȡ��flash page size��С����512�ֽ�Ϊ��λ���������Ƿ�Ϊ1K��д
�������룺
�������أ�  ����ֵ   ���ֽڣ�0��512�ֽڶ�д 1��1024�ֽڶ�д ���ֽڣ�flash����page�ڵ�������512�ֽڣ���
���ݽṹ��  ��
*/
 uint16 Get_Flash_Page_Size(void); 


//**UD_Init**//
/*
�������ƣ�  UD_Init
����������  ��ʼ��FLASH���������ݽṹ
�������룺
�������أ�  ��
���ݽṹ��  ��
*/
 bool UD_Init(int8 mode);

//**UD_Exit**//
/*
�������ƣ�  UD_Exit
����������  ��д���ݽṹ��FLASH
�������룺
�������أ�  ��
���ݽṹ��  ��
*/
 void UD_Exit(void);

//********************************for SD Card (��������)**************************************

//**UD_SDCardReadInit**//
/*
�������ƣ�  UD_SDCardReadInit
�������ܣ�  ��ʼ��������������������USBģʽ��ʹ��
�������룺  ������������������Ŀ
            ��������������ʼ�����ĵ�ַ
�������أ�  0   success;
���ݽṹ��
        typedef struct
        {
            uint32   lba;        //����������ʼ�����ĵ�ַ
            int8    SecNum;     //��������������Ŀ
            void    *sramaddr;  //��д���ڴ��еĵ�ַ
            uint8    srampage;   //�ڴ�ҳ 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/

 int8 UD_SDCardReadInit(UD_RW *rwaddr);


//**UD_SDCardWriteInit**//
/*
�������ƣ�  UD_SDCardWriteInit
����������  ��ʼ������д����������ʼ������������
            1)��������ʼ��ַ��ʼ������N��BLK
            2)��������д������
            ����USBģʽ��ʹ��
�������룺  ��������д��������Ŀ
            ��������д����ʼ�����ĵ�ַ
�������أ�  0   success;
            1   ��ַ��������粻���ڻ�Խ��ĵ�ַ
            2   д��������
            3   ����������Ҫ���¼��
���ݽṹ��
        typedef struct
        {
            uint32   lba;        //����д����ʼ�����ĵ�ַ
            int8    SecNum;     //����д��������Ŀ
            void    *sramaddr;  //��д���ڴ��еĵ�ַ
            uint8    srampage;   //�ڴ�ҳ 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/
 int8 UD_SDCardWriteInit(UD_RW *rwaddr);

//**UD_SDCardSectorRead**//
/*
�������ƣ�  UD_SDCardSectorRead
�������ܣ�  ��512 byte���ݵ�ָ���ڴ�λ��
�������룺  UD_RW *RWStruct
�������أ�  0   success;
            1   ��ַ��������粻���ڻ�Խ��ĵ�ַ
            3   ����������Ҫ���¼��
���ݽṹ��
        typedef struct
        {
            uint32   lba;        //LBA��д��ַ����512 BYTEΪ��λ
            int8    endflag;    //���Ʋ���������5A���ڽ���������д������Ϊ0
            void    *sramaddr;  //��д���ڴ��еĵ�ַ
            uint8    srampage;   //�ڴ�ҳ 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/
 int8 UD_SDCardSectorRead(UD_RW *rwaddr);

//**UD_SDCardSectorWrite**//
/*
�������ƣ�  UD_SDCardSectorWrite
�������ܣ�  д512 byte���ݵ�ָ������ָ������
�������룺  UD_RW *RWStruct
�������أ�  0   success;
            1   ��ַ��������粻���ڻ�Խ��ĵ�ַ
            2   д��������
            3   ����������Ҫ���¼��
���ݽṹ��
        typedef struct
        {
            uint32   lba;        //LBA��д��ַ����512 BYTEΪ��λ
            int8    endflag;    //���Ʋ���������5A���ڽ���������д������Ϊ0
            void    *sramaddr;  //��д���ڴ��еĵ�ַ
            uint8    srampage;   //�ڴ�ҳ 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/
 int8 UD_SDCardSectorWrite(UD_RW *rwaddr);

//**UD_SDCardWPDetect**//
/*
�������ƣ�  UD_SDCardWPDetect
����������  ��⿨�Ƿ�д����
�������룺
�������أ�  0 û��д����
            1 д����
���ݽṹ��  ��
*/
 int8 UD_SDCardWPDetect(void);

//**UD_SDCardGetCap**//
/*
�������ƣ�  UD_SDCardGetCap
����������  ��⿨����
�������룺
�������أ�  HL ����������128KΪ��λ
���ݽṹ��  ��
*/
 uint32 UD_SDCardGetCap(void);

//**UD_Init**//
/*
�������ƣ�  UD_Init
����������  ��ʼ��FLASH���������ݽṹ
�������룺
�������أ�  ��
���ݽṹ��  ��
*/
 bool UD_SDCardInit(int8 mode);

//**UD_SDCardExit**//
/*
�������ƣ�  UD_SDCardExit
����������  �رտ�
�������룺
�������أ�  ��
���ݽṹ��  ��
*/
 void UD_SDCardExit(void);

 void UD_SetHighFrequence(void);


 void UD_SetLowFrequence(void);



#endif /* __C251__ */

#endif /*_UDDRV_H */











