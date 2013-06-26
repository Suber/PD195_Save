/********************************************************************************
*                              AS212A
*                            Module: USB Host
*                 Copyright(c) 2001-2010 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*       xiaomaky   2009-12-4 13:54     1.0                build this file 
********************************************************************************/



#ifndef _UHDRV_H
#define _UHDRV_H

#include "string.h"
#include "typeext.h"
#include "port.h"
#include "irq.h"
#include "message.h"
#include "uddrv.h"

/* error  meaning */
#define GENERAL_OK                     0x00
#define GENERAL_ERROR                  0x01
#define DEVICE_NOT_IN                  0x02
#define ENUMERATION_FAIL               0x03
#define BULKSTART_FAIL                 0x04
#define ERROR_NOT_SUPPORT_DEVICE       0X05
#define ERROR_SETUP_OVERTIME           0X06
#define ERROR_CONTROL_IN_DATA          0X07
#define ERROR_STATUS_OVERTIME          0X08
#define ERROR_NOT_SUPPORT_PROTOCOL     0X09
#define ERROR_UNIT_READY_OVERTIME      0X0A
#define ERROR_IN_DATA_WRONG            0X0B
#define ERROR_LUN_BLOCK_SIZE           0X0C
#define ERROR_BUS_RESET_OVERTIME       0X0D

#define ERROR_TRANSFER_BUSY            0X80
#define ERROR_IRQ_OCCUR                0X81
#define ERROR_EP_BUFFER_NOT_EMPTY      0X82  /* not busy, but some endpoint sub-buffer is not empty */
#define ERROR_OTG_STATE                0X83
#define ERROR_EP_STALL                 0X84
#define ERROR_NO_EP_HANDSHAKE		   0X85	

#define ERROR_PARAMETER_IN             0XF0
#define ERROR_MIDIUM_NOT_PRESENT	   0XF1
#define ERROR_MIDIA_CHANGED	   		   0XF2


/* function number  */
#define API_UH_SectorRead              API_UD_SectorRead    //0x00
#define API_UH_SectorWrite             API_UD_SectorWrite   //0x01
//#define API_UH_WordRead                0x02
//#define API_UH_DwordRead               0x03
#define API_UH_Update                  API_UD_Update        //0x04
#define API_UH_GetMaxLun               0x05
//#define API_UH_TestLunReady            0x06
//#define API_UH_TestUdiskReady          0x07
#define API_UH_GetCurrentLun           0x08
//#define API_UH_SetCurrentLun           0x09

//BANK A
#define API_UH_EnableCon               0x0000+API_STG_BaseNoA
#define API_UH_DisableCon              0x1000+API_STG_BaseNoA

//BANK B
#define API_UH_Init                    API_UD_Init          //0x0000+API_STG_BaseNoB
#define API_UH_Exit                    API_UD_Exit          //0x0001+API_STG_BaseNoB
#define API_UH_GetCap                  API_UD_GetCap        //0x0002+API_STG_BaseNoB                   
#define API_UH_GetSectorSize           API_UD_GET_SCTR_SIZE //0x1002+API_STG_BaseNoB
#define API_UH_SetLUN                  0x0003+API_STG_BaseNoB
#define API_UH_ClearStall              0x0004+API_STG_BaseNoB               


#ifdef __C251__


/*!
 *  structure of read or write info   \n
 *  \li SectorAddress: logical block address by 512BYTE.
 *  \li SectorNumber : sector number.
 *  \li RAMAddress   : RAM address.
 * 
 */
/***********************************************************
函数名称：  UH_Init
函数功能：  uhost驱动的初始化函数，在DRV_Install的时候调用，DRV_Install
            之后也可以随时调用，用来获取usb连接状态相关的信息
函数输入：  mode:   not used
函数返回：  0x01       success;
            0xff    没有插入miniA线
            0xfe    插入了miniA线，但是没有插入u盘，或者u盘没有连接
            0xfd    设备不支持
            //0xfc    数据传输错误，说明电气性能不好----------暂时不实现
            其他    resverd
*************************************************************/
//non_banked byte UH_Init(unsigned char mode);
uint8 UH_Init(char mode);

/***********************************************************
函数名称：  UH_Exit
函数功能：  uhost驱动的退出函数，在DRV_UnInstall的时候调用
函数输入：
函数返回：
*************************************************************/
void UH_Exit(void);

/***********************************************************
函数名称：  UH_SectorRead
函数功能：  扇区读函数
函数输入：  shro:   参见struct UD_RW
函数返回：  0       success;
            0xff    地址输入错误，如不存在或越界的地址
            0xfe    U盘上报了读失败
            其他    resverd
*************************************************************/
uint8 UH_SectorRead(UD_RW *shro);

/***********************************************************
函数名称：  UH_SectorWrite
函数功能：  扇区写函数
函数输入：  shro:   参见struct UD_RW
函数返回：  0       success;
            0xff    地址输入错误，如不存在或越界的地址
            0xfe    U盘上报了写失败
            其他    resverd
*************************************************************/
uint8 UH_SectorWrite(UD_RW *shro);

/***********************************************************
函数名称：  UH_GetMaxLun
函数功能：  获取分区数目
函数输入：
函数返回：  0～15   分区数目，根据协议规定取值范围是0～15。
                    注：返回3，代表0～3，共四个盘
            其他    resverd
*************************************************************/
uint8 UH_GetMaxLun(void);

/***********************************************************
函数名称：  UH_TestLunReady
函数功能：  测试对应分区有没有加载磁盘
函数输入：  lun     待测试的分区号
函数返回：  0       该分区加载了磁盘
            0xfa       该分区没有加载磁盘
            0xfc       错误的lun参数等
            其他    resverd
*************************************************************/
//uint8 UH_TestLunReady(unsigned char lun);

/***********************************************************
函数名称：  UH_GetCap
函数功能：  获得对应磁盘的大小
函数输入：  lun     对应磁盘的分区号
函数返回：  分区的大小，以512 byte为单位，一个block大于512的u盘，uhost驱动会将其转换成以512 byte为单位
            0       error
*************************************************************/
uint32 UH_GetCap(void);

/***********************************************************
函数名称：  UH_GetSectorSize
函数功能：  获得u盘一个Sector的大小
函数输入：  lun     对应磁盘的分区号
函数返回：  u盘一个Sector的大小，如果返回n，则一个Sector的大小是n*512 byte
            0       error
*************************************************************/
uint8 UH_GetSectorSize(void);

/***********************************************************
函数名称：  UH_TestUdiskReady
函数功能：  测试当前连接状态
函数输入：
函数返回：  0       成功。miniA插入，u盘也插入，并连接。此时可以进行读，得到容量等操作
            0xff    没有插入miniA线
            0xfe    插入了miniA线，但是没有插入u盘，或者u盘没有连接
            0xfd    设备不支持
            其他    resverd
注：会将这个函数做成UH_Init的简化版，主要通过查询一些软件标志位来返回对应的值
*************************************************************/
//uint8 UH_TestUdiskReady(void);

/***********************************************************
函数名称：  UH_GetCurrentLun
函数功能：  获取当前激活的磁盘分区号
函数输入：
函数返回：  0～15   当前激活的磁盘分区号
            其他    resverd
*************************************************************/
uint8 UH_GetCurrentLun(void);

/***********************************************************
函数名称：  UH_SetCurrentLun
函数功能：  设定当前激活的磁盘分区号，如果不设定的话，默认为0
函数输入：  lun     对应磁盘的分区号
函数返回：  0       success
            0xfc       错误的lun参数等
            0xfd    设备不支持
            其他    resverd
*************************************************************/
uint8 UH_SetCurrentLun(unsigned char lun);

/***********************************************************
函数名称：  UH_WordRead
函数功能：  读取某一个lba，某个偏移量的双字节数据
函数输入：  UD_RW
            typedef struct
            {
                dword   lba;        //word所在扇区内的地址
                char    reserv;     //未用
                void    byteaddr;   //word在扇区内的字节偏移
                byte    reserv;     //未用
            }UD_RW;
函数返回：  0       success
            1       0xff    地址输入错误，如不存在或越界的地址
            其他    resverd
*************************************************************/
//int16 UH_WordRead(UD_RW *rwaddr);

/***********************************************************
函数名称：  UH_DwordRead
函数功能：  读取某一个lba，某个偏移量的四字节数据
函数输入：  UD_RW
            typedef struct
            {
                dword   lba;        //word所在扇区内的地址
                char    reserv;     //未用
                void    byteaddr;   //word在扇区内的字节偏移
                byte    reserv;     //未用
            }UD_RW;
函数返回：  0       success
            1       0xff    地址输入错误，如不存在或越界的地址
            其他    resverd
*************************************************************/
//int16 UH_DwordRead(UD_RW *rwaddr);


/***********************************************************
函数名称：  UH_Update
函数功能：  空函数，FS_FClose()关闭一个文件时被调用
函数输入：
函数返回：
*************************************************************/
void UH_Update(void);


/*
message.h：
#define MSG_UH_IN               0xc0   //U盘插入
#define MSG_UH_OUT              0xc1   //U盘拔出
#define MSG_UH_UDISK_OUT        0xc2   //卡槽中的卡拔出
*/


#endif /* __C251__ */

#endif /*_UHDRV_H */











