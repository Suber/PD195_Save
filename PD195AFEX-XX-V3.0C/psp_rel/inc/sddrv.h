/*
********************************************************************************
*                       ACTOS
*               System Disk driver head file
*
*                (c) Copyright 2002-2003, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sddrv.h
* By     : Gongee
* Version: 1> v1.00     first version     2003-11-20 9:14
********************************************************************************
********************************************************************************
*/
#ifndef _SDDRV_INCLUDED
#define _SDDRV_INCLUDED

#include "actapi.h"

#ifdef __C251__



typedef struct
{
        uint32   faddr; //字节地址
        uint8    reserve;
        uint8    srampage;
        IntAddr  sramaddr;
}SD_RW;

typedef struct
{
        uint32   faddr; //扇区地址
        uint8    length;//扇区数
        uint8    srampage;
        IntAddr  sramaddr;
}SD_FFRW;



 bool SD_SimpleInit(uint8 mode);
 void SD_SimpleExit(void);
 bool SD_Read(SD_RW *sdrwptr,uint16 length);
 bool SD_FFRead(SD_FFRW *sdrwptr);
 bool SD_SimpleSectorRead(SD_RW *sdrw);

 bool SD_SectorRead(SD_RW *sdrw);
 bool SD_SectorWrite(SD_RW *sdrw);
 bool SD_Bre_SectorRead(SD_RW *sdrw);
 bool SD_Bre_SectorWrite(SD_RW *sdrw);
 bool SD_Update(void);
 int8 SetSWAPAddress(int8 page);  //ret: ¦̡�page


#endif /* ____C251__ */

#ifdef __A251__

 

#define API_SD_SectorRead           0x00
#define API_SD_SectorWrite          0x01
#define API_Bre_SectorRead          0x08
#define API_Bre_SectorWrite         0x09
#define API_CardBre_SectorRead      0x0004+API_SDCardFF_BaseNoA
#define API_CardBre_SectorWrite     0x1004+API_SDCardFF_BaseNoA

#define API_SD_Update           0x04
mSD_Read        macro
        MOV       R10,#API_SD_Read
        LCALL     RSTFastAPI
        endm

mSD_SectorRead  macro
        MOV  	  R10,#API_SD_SectorRead
        LCALL     RSTStgAPI
        endm

mSD_SectorWrite macro
        MOV  R10,#API_SD_SectorWrite
        LCALL     RSTStgAPI
        endm

mBrec_SectorRead macro
        MOV		  WR8,#API_Bre_SectorRead
        LCALL     RSTBankAPI
        endm

mBrec_SectorWrite macro
         MOV	  WR8,API_Bre_SectorWrite
        LCALL     RSTBankAPI
        endm

mSD_Update macro
        MOV       R10,#API_SD_Update
        LCALL     RSTStgAPI
        endm
#endif /* __A251__ */

#endif /*_SDDRV_H */
