/*
********************************************************************************
*                       ACTOS
*               System Disk file system head file
*
*                (c) Copyright 2002-2003, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdfs.h
* By     : Gongee
* Version: 1> v1.00     first version     2003-11-20 9:14
********************************************************************************
*/
#ifndef _SDFS_H
#define _SDFS_H

#include "actapi.h"
#include "typeext.h"



#define MODE_READ   0

#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2
#define SDFILE_CLOSED           0xFF
#define  MFSLOCATEPAGE  0xf0        //IPM第0页


#ifdef __C251__
typedef struct
{
    int8    mode;
    uint32   startaddr;
    uint32   endaddr;
    uint32   rwpointer;
} SD_FILE;

typedef struct
{
    int8    fname[11];
    int8    fattr;
    int8    reserve0[2];
    uint16    version;
    uint32   offset;
    uint32   size;
    uint32   reserve1;
    uint32   checksum;
} SD_DIR;

//同时可以打开4个文件
 SD_FILE *SD_FOpen (int8 *filename, uint8 mode);
 int8 SD_FClose (SD_FILE *fp);
 int8 SD_FSeek (SD_FILE *fp, uint8 nFrom, uint32 offset);
 int32 SD_FTell (SD_FILE *fp);
 int16 SD_FRead (SD_FILE *fp, void *buffer, uint16 Length);
 bool sMfsRcodeRead(int8 *filename, int8 * mfs_ap_head);
 int8 Check_SDFile_Handle(SD_FILE *fp);
#endif /* __C251__ */
#ifdef __A251__
#define API_SD_FOpen            0x0000+API_SDFS_BaseNoA

mSD_FOpen        macro
        MOV 	  WR8,#API_SD_FOpen
        LCALL     RSTBankAPI
        endm
mSD_FClose        macro
       MOV 		  R10 ,#API_SD_FClose
       LCALL   	  RSTFastAPI
        endm
mSD_FSeek         macro
       MOV 		  R10,#API_SD_FSeek
       LCALL  	  RSTFastAPI
        endm
mSD_FTell         macro
       MOV 		  R10,#API_SD_FTell
       LCALL  	  RSTFastAPI
        endm
mSD_FRead         macro
        MOV       R10,#API_SD_FRead
        LCALL     RSTFastAPI
        endm
#endif /* __A251__ */

#endif /*_SDFS_H */
