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
mUD_GetFlashType  macro                 //返回值  非MLC flash:0    MLC flash:1
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
函数名称：  UD_WordRead
函数功能：  读出指定扇区地址的扇
函数输入：  UD_RW *RWStruct
函数返回：  1 success; 0 fail
数据结构：
        typedef struct
        {
            uint32   lba;        //word所在扇区内的地址
            int8    reserv;     //未用
            void    byteaddr;   //word在扇区内的字节偏移
            uint8    reserv;     //未用
        }UD_RW;
*/
 int  UD_WordRead(UD_RW *rwaddr);



//**UD_DwordRead**//
/*
函数名称：  UD_DwordRead
函数功能：  读出指定扇区地址的扇
函数输入：  UD_RW *RWStruct
函数返回：  1 success; 0 fail
数据结构：
        typedef struct
        {
            uint32   lba;        //dword所在扇区内的地址
            int8    reserv;     //未用
            void    byteaddr;   //dword在扇区内的字节偏移
            uint8    reserv;     //未用
        }UD_RW;
*/
 long int UD_DwordRead(UD_RW *rwaddr);



//**UD_SectorRead**//
/*
函数名称：  UD_SectorRead
函数功能：  读512 byte数据到指定内存位置
函数输入：  UD_RW *RWStruct
函数返回：  0   success;
            1   地址输入错误，如不存在或越界的地址
            3   其他错误，需要重新激活卡
数据结构：
        typedef struct
        {
            uint32   lba;        //LBA读写地址，以512 BYTE为单位
            int8    endflag;    //控制参数，输入5A用于结束连续读写，正常为0
            void    *sramaddr;  //读写到内存中的地址
            uint8    srampage;   //内存页 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/
 int8 UD_SectorRead(UD_RW *rwaddr);

//**UD_SectorWrite**//
/*
函数名称：  UD_SectorWrite
函数功能：  写512 byte数据到指定卡的指定扇区
函数输入：  UD_RW *RWStruct
函数返回：  0   success;
            1   地址输入错误，如不存在或越界的地址
            2   写保护错误
            3   其他错误，需要重新激活卡
数据结构：
        typedef struct
        {
            uint32   lba;        //LBA读写地址，以512 BYTE为单位
            int8    endflag;    //控制参数，输入5A用于结束连续读写，正常为0
            void    *sramaddr;  //读写到内存中的地址
            uint8    srampage;   //内存页 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/
 int8 UD_SectorWrite(UD_RW *rwaddr);

//**UD_WPDetect**//
/*
函数名称：  UD_WPDetect
函数描述：  检测FLASH是否写保护
函数输入：
函数返回：  0 没有写保护
            1 写保护
数据结构：  无
*/
 int8 UD_WPDetect(void);

//**UD_GetCap**//
/*
函数名称：  UD_GetCap
函数描述：  检测FLASH容量
函数输入：
函数返回：  HL FLASH容量，以128K为单位
数据结构：  无
*/
 uint16 UD_GetCap(void);
/*
*********************************************************************************************************
*                                           UD_GetSctrSize
*
* Description: 获取当前所激活盘符一个扇区的容量(插入的U盘一个扇区可能为512byte的整数倍)
*
* Arguments  : None.
*
* Returns    : a: sector size, 2表示为1024byte, so on.
*
* Note(s)    : None.
*********************************************************************************************************
*/
int8 UD_GetSctrSize(void);
//**UD_Update**//
/*
函数名称：  UD_Update
函数描述：  写入最后一笔数据到FLASH
函数输入：
函数返回：  无
数据结构：  无
*/
 void UD_Update(void);


//**UD_GetFlashType**//
/*
函数名称：  UD_GetFlashType
函数描述：  取得flash 类型是否MLC
函数输入：
函数返回：  返回值  非MLC flash:0    MLC flash:1
数据结构：  无
*/
 char UD_GetFlashType(void);  //返回值  非MLC flash:0    MLC flash:1

//**Get_Flash_Page_Size**//
/*
函数名称：  Get_Flash_Page_Size
函数描述：  取得flash page size大小，以512字节为单位，并返回是否为1K读写
函数输入：
函数返回：  返回值   高字节：0：512字节读写 1：1024字节读写 低字节：flash物理page内的扇区（512字节）数
数据结构：  无
*/
 uint16 Get_Flash_Page_Size(void); 


//**UD_Init**//
/*
函数名称：  UD_Init
函数描述：  初始化FLASH，建立数据结构
函数输入：
函数返回：  无
数据结构：  无
*/
 bool UD_Init(int8 mode);

//**UD_Exit**//
/*
函数名称：  UD_Exit
函数描述：  回写数据结构到FLASH
函数输入：
函数返回：  无
数据结构：  无
*/
 void UD_Exit(void);

//********************************for SD Card (快速驱动)**************************************

//**UD_SDCardReadInit**//
/*
函数名称：  UD_SDCardReadInit
函数功能：  初始化连续读的扇区，仅在USB模式下使用
函数输入：  输入连续读的扇区数目
            输入连续读的起始扇区的地址
函数返回：  0   success;
数据结构：
        typedef struct
        {
            uint32   lba;        //连续读的起始扇区的地址
            int8    SecNum;     //连续读的扇区数目
            void    *sramaddr;  //读写到内存中的地址
            uint8    srampage;   //内存页 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/

 int8 UD_SDCardReadInit(UD_RW *rwaddr);


//**UD_SDCardWriteInit**//
/*
函数名称：  UD_SDCardWriteInit
函数描述：  初始化连续写的扇区，初始化操作包括：
            1)擦除从起始地址开始的连续N个BLK
            2)设置连续写技术器
            仅在USB模式下使用
函数输入：  输入连续写的扇区数目
            输入连续写的起始扇区的地址
函数返回：  0   success;
            1   地址输入错误，如不存在或越界的地址
            2   写保护错误
            3   其他错误，需要重新激活卡
数据结构：
        typedef struct
        {
            uint32   lba;        //连续写的起始扇区的地址
            int8    SecNum;     //连续写的扇区数目
            void    *sramaddr;  //读写到内存中的地址
            uint8    srampage;   //内存页 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/
 int8 UD_SDCardWriteInit(UD_RW *rwaddr);

//**UD_SDCardSectorRead**//
/*
函数名称：  UD_SDCardSectorRead
函数功能：  读512 byte数据到指定内存位置
函数输入：  UD_RW *RWStruct
函数返回：  0   success;
            1   地址输入错误，如不存在或越界的地址
            3   其他错误，需要重新激活卡
数据结构：
        typedef struct
        {
            uint32   lba;        //LBA读写地址，以512 BYTE为单位
            int8    endflag;    //控制参数，输入5A用于结束连续读写，正常为0
            void    *sramaddr;  //读写到内存中的地址
            uint8    srampage;   //内存页 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/
 int8 UD_SDCardSectorRead(UD_RW *rwaddr);

//**UD_SDCardSectorWrite**//
/*
函数名称：  UD_SDCardSectorWrite
函数功能：  写512 byte数据到指定卡的指定扇区
函数输入：  UD_RW *RWStruct
函数返回：  0   success;
            1   地址输入错误，如不存在或越界的地址
            2   写保护错误
            3   其他错误，需要重新激活卡
数据结构：
        typedef struct
        {
            uint32   lba;        //LBA读写地址，以512 BYTE为单位
            int8    endflag;    //控制参数，输入5A用于结束连续读写，正常为0
            void    *sramaddr;  //读写到内存中的地址
            uint8    srampage;   //内存页 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/
 int8 UD_SDCardSectorWrite(UD_RW *rwaddr);

//**UD_SDCardWPDetect**//
/*
函数名称：  UD_SDCardWPDetect
函数描述：  检测卡是否写保护
函数输入：
函数返回：  0 没有写保护
            1 写保护
数据结构：  无
*/
 int8 UD_SDCardWPDetect(void);

//**UD_SDCardGetCap**//
/*
函数名称：  UD_SDCardGetCap
函数描述：  检测卡容量
函数输入：
函数返回：  HL 卡容量，以128K为单位
数据结构：  无
*/
 uint32 UD_SDCardGetCap(void);

//**UD_Init**//
/*
函数名称：  UD_Init
函数描述：  初始化FLASH，建立数据结构
函数输入：
函数返回：  无
数据结构：  无
*/
 bool UD_SDCardInit(int8 mode);

//**UD_SDCardExit**//
/*
函数名称：  UD_SDCardExit
函数描述：  关闭卡
函数输入：
函数返回：  无
数据结构：  无
*/
 void UD_SDCardExit(void);

 void UD_SetHighFrequence(void);


 void UD_SetLowFrequence(void);



#endif /* __C251__ */

#endif /*_UDDRV_H */











