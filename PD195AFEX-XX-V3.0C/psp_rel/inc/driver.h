#ifndef _DRIVER_H
#define _DRIVER_H

#include "sysdef.h"
#include "actapi.h"
#include "TypeExt.h"


//****************************
//  驱动程序类型
#define DRV_VM          0x00    //虚拟内存驱动
#define DRV_STG         0x01    //用户存储器驱动
#define DRV_KY          0x02    //键盘驱动
#define DRV_UI          0x03    //显示驱动
#define DRV_FS          0x04    //文件系统驱动
#define DRV_SDFF        0x05    //SD卡快速驱动
#define DRV_I2C         0x06    //I2C驱动
#define DRV_SDRAM       0x07    //USB Host驱动
#define DRV_MFS			0x08    //MFS for data exchange
//****************************
// 用户存储器类型
#define STG_NAF         0x00            //nand type flash
#define STG_SMC         0x10            //smc
#define STG_MMC         0x20            //mmc
#define STG_UHOST       0x50            //外接U盘
#define STG_CFC         0x30            //cfc
#define STG_HD          0x40            //hd
#define STG_NAF_SB      0x00 | STG_NAF  //small block nand type flash
#define STG_NAF_MB      0x01 | STG_NAF  //small block nand type flash
#define STG_NAF_LB      0x02 | STG_NAF  //small block nand type flash    644
#define STG_NAF_LB1     0x03 | STG_NAF  //large block 1 nand type flash  648
#define STG_NAF_LB2     0x04 | STG_NAF  //large block 2 nand type flash  645  Toshiba MLC Flash(284)
#define STG_NAF_LB3     0x05 | STG_NAF  //large block 3 nand type flash  024
#define STG_NAF_LB4     0x06 | STG_NAF  //large block 4 nand type flash  285  Samsung MLC Flash(284)
#define STG_NAF_LB5     0x07 | STG_NAF  //large block 5 nand type flash  286  Hinix   MLC Flash(284)
#define STG_NAF_LB6     0x08 | STG_NAF  //large block 6 nand type flash  287  Micron  MLC Flash(284)
#define STG_NAF_LB7     0x09 | STG_NAF  //large block 7 nand type flash  288  ST      MLC Flash(284)
#define STG_NAF_LB8     0x0A | STG_NAF  //large block 7 nand type flash  289  Samsung MLC 2RB(284)
#define STG_NAF_LB9     0x0B | STG_NAF  //large block 7 nand type flash  28A  Hinix   MLC 2RB(284)
#define STG_NAF_LBA     0x0c | STG_NAF  //LBA nand type flash            28B  Toshiba MLC 2RB(284)

#ifdef __C251__

//驱动程序头信息

typedef struct
{
        int8    filetype;               //文件类型 'D'
        uint8    drvtype;                //驱动类型
        uint16    RcodeStartAddr;         //常驻内存代码段起始地址
        uint16    RcodeLength;            //常驻内存代码段长度
        uint16    Rcode2StartAddr;         //常驻内存代码段起始地址
        uint16    Rcode2Length;            //常驻内存代码段长度
         uint16    RdataStartAddr;         //常驻内存代码段起始地址
        uint16    RdataLength;            //常驻内存代码段长度
        uint16    InitAPI;                //驱动初始化程序api
        uint16    ExitAPI;                //驱动关闭程序api
        uint32   BnkAOffset;             //驱动程序banka代码在文件中的地址
        uint32   BnkBOffset;             //驱动程序bankb代码在文件中的地址
} DRV_HEADINFO; 
 

//安装驱动程序
//para: 驱动文件名，驱动输入参数
//ret: 驱动类型
 int8 DRV_Install(const int8 *filename, uint8 param);

//卸载驱动程序
//para: 驱动类型(见前面定义)
//ret:  true/false
 bool DRV_UnInstall(uint8 drivertype);

//探测物理存储设备是否存在
//para: 物理驱动器号
//ret:  驱动类型(见前面定义) 0xff means no ud detected，0x20 if(UDNo==1) means has card, 0xfe if(UDNo==1) means capacity err
 uint8 DRV_DetectUD(uint8 UDNo);

//激活物理存储设备是否存在
//para: 物理驱动器号（0-f），工作模式（通常=0）
//ret:  true/false
 bool DRV_ActiveUD(uint8 UDNo,uint8 workmod);


//探测文件系统分区是否存在，若存在，返回分区状态（文件系统类型，是否格式化）
//para: 逻辑驱动器号
//  'A','B'  --------------------------保留
//  'C','D','E','F'--------------------on board flash
//  'H','I','J','K'--------------------移动存储设备
//  其他保留
//ret:  bit7: 0/1 是否存在，
//      bit6: 0/1 格式化，
//      bit5-bit0 fs类型 0: fat12 1: fat16,2 fat32
 int8 DRV_DetectDrive(int8 letter);


//激活文件系统分区是否存在，若存在，返回分区状态（文件系统类型，是否格式化）
//para: 逻辑驱动器号，工作模式（通常=0）
//ret:  true/false
 bool DRV_ActiveDrive(int8 letter,uint8 workmod);


//Mtp中激活文件系统分区是否存在，若存在，返回分区状态（文件系统类型，是否格式化）
//para: 逻辑驱动器号，工作模式（通常=0）
//ret:  true/false
 bool DRV_MActiveDrive(int8 letter,uint8 workmod);


//本机驱动器格式化，支持FAT16和FAT32格式化，支持单盘格式化（格式化的物理区域从物理磁盘首部开始），
//不生成MBR，只有DBR，不支持分区
//DiskACapacity: 磁盘的容量，用扇区表示
//type:格式化的文件系统类型，1表示FAT16，2表示FAT32
//ret:  true/false
 bool DRV_FormatDrive(uint32 DiskACapacity,int8 type,int8 cardornot);

//ret: high uint8=WorkMode:工作模式（0=MODE_FAST,1=MODE_SLOW）  low uint8=UDNo:物理驱动器号（0-f）
 uint16 DRV_GetActiveUD(void);


 bool DRV_FFInstall(const int8 *filename, uint8 param);



/*
*********************************************************************************************************
*Function    ：active drive and initialize FS        2008-9-19 14:45
*
*Description：used specially for data copying
*
*Arguments    : letter   ---- drive letter, such as C--Flash, H--Card, L--HideDisk.
*             workmode ---- high uint8 is slow mode or fast mode,for data copying, default fast mode, MODE_FAST，that is 0x00.
*                           low uint8 is source disk or destination disk.
*
*Rerurns    ：0            failed
*              1            successful
*
*Other        :
*********************************************************************************************************
*/
 bool DRV_SActiveDrive(int8 letter, uint16 workmode);


#endif /*  __C251__ */

#ifdef __A251__

#define API_DRV_Install         0x0000 + API_DRV_BaseNoB
#define API_DRV_UnInstall       0x1000 + API_DRV_BaseNoB
#define API_DRV_DetectUD        0x2000 + API_DRV_BaseNoB
#define API_DRV_GetActiveUD     0x4000 + API_DRV_BaseNoB
#define API_DRV_ActiveUD        0x3000 + API_DRV_BaseNoB

#define API_DRV_DetectDrive     0x0001 + API_DRV_BaseNoB
#define API_DRV_ActiveDrive     0x1001 + API_DRV_BaseNoB
#define API_DRV_MActiveDrive    0x2001 + API_DRV_BaseNoB    //for mtp use
#define API_DRV_SActiveDrive    0x3001 + API_DRV_BaseNoB
#define API_DRV_FFInstall       0x4001 + API_DRV_BaseNoB

#define API_DRV_CheckCard       0x0002 + API_DRV_BaseNoB
#define API_DRV_CheckContain    0x1002 + API_DRV_BaseNoB

#define API_DRV_FormatDrive     0x0003 + API_DRV_BaseNoB
#define API_DRV_FormatFat16     0x1003 + API_DRV_BaseNoB
#define API_DRV_FormatFat32     0x0004 + API_DRV_BaseNoB

mDRV_Install    macro
        MOV         WR8,#API_DRV_Install
		LCALL	 RSTBankAPI
        endm
mDRV_UnInstall  macro
        MOV         WR8,#API_DRV_UnInstall
		LCALL	 RSTBankAPI
        endm
mDRV_DetectUD  macro
        MOV         WR8,#API_DRV_DetectUD
		LCALL	 RSTBankAPI
        endm                  
mDRV_GetActiveUD  macro
        MOV         WR8,#API_DRV_GetActiveUD
		LCALL	 RSTBankAPI
        endm    
mDRV_ActiveUD  macro
        MOV         WR8,#API_DRV_ActiveUD
		LCALL	 RSTBankAPI
        endm           
mDRV_FFInstall  macro
        MOV         WR8,#API_DRV_FFInstall
		LCALL	 RSTBankAPI
        endm  
        
mDRV_DetectDrive  macro
        MOV         WR8,#API_DRV_DetectDrive
		LCALL	 RSTBankAPI
        endm           
mDRV_DRV_ActiveDrive  macro
        MOV         WR8,#API_DRV_ActiveDrive
		LCALL	 RSTBankAPI
        endm                     
        
mDRV_CheckCard      macro
        MOV         WR8,#API_DRV_CheckCard
		LCALL	 RSTBankAPI
        endm
mDRV_CheckContain      macro
        MOV         WR8,#API_DRV_CheckContain
		LCALL	 RSTBankAPI
        endm  
        
mDRV_FormatDrive      macro
        MOV         WR8,#API_DRV_FormatDrive
		LCALL	 RSTBankAPI
        endm      
mDRV_FormatFat16      macro
        MOV         WR8,#API_DRV_FormatFat16
		LCALL	 RSTBankAPI
        endm
mDRV_FormatFat32      macro
        MOV         WR8,#API_DRV_FormatFat32
		LCALL	 RSTBankAPI
        endm                         
        
#endif /* __A251__ */

#endif /*_DRIVER_H */
