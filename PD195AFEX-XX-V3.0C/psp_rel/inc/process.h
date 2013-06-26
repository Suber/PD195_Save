#ifndef _PROCESS_H
#define _PROCESS_H

#include "sdfs.h"
#define TWOBANK_INGROUP 1
#define API_BankFLSUH 			0x0000+API_APM_BaseNoB
#ifdef __C251__ 
//ap 头结构
typedef struct
{
    uint8    file_type;          //'P'
    uint8    ap_type;            //0:ap_system, 1:ap_user
    int8    magic[4];            //'W', 'G', 0x19, 0x97
    uint8    major_version;      //3, for ACTOS 3.0
    uint8    minor_version;      //0
    uint32    text_offset;		
    uint16    text_length;
    uint16    text_addr;          //in ram 代码段
    uint32    data_offset;		
    uint16    data_length;
    uint16    data_addr;          //有初始化的全局变量
    uint16	midstart;
    uint16    tsr_length;
    uint16    entry;              //ap 入口
    uint8    entry_bank;         //ap 初始化 bank 号码
    uint8    banks;              //共几个bank, MAX 252
}ap_head_t;

 


typedef struct
{
    uint32    bank_offset;      //在文件内的偏移
    uint16    bank_length;       //MAX 64 KB
    uint16    bank_addr;         //zram 地址
}ap_bank_head_t;

/* ap 文件头结构, ap由 APBuild Tool 从 hex 文件生成
ap_head_t        ap_head;            //32 bytes
ap_bank_head_t    ap_bank_head[0];    //8 bytes
ap_bank_head_t    ap_bank_head[1];    //8 bytes
...
ap_bank_head_t    ap_bank_head[251];    //8 bytes

//空的bank_head 填充0xFF, 共占用2 Kbytes
*/

//tcb 结构
typedef struct
{
    SD_FILE			ap_file;			   //保存当前ap文件在SD区的开始位置
#ifdef	TWOBANK_INGROUP
    uint8   ap_bank[8];            //保存当前ap bank 号
#else
    uint8   ap_bank[4];            //保存当前ap bank 号
#endif
	uint16    midstart;

}tcb_t;

//切换到 BankNo, 返回旧的 bank 号码
int16 BankSwitch(uint8 BankNO);
//执行 strName ap, 返回ap的返回值
int16 ExecAP(int8 *strName, int16 Param);
void PutTSR(void* pfunc);
void RunTSR(uint8 param);
void KillTSR(void);
void BankFlush(uint8 gindex);
#endif/*__C251__*/

#ifdef __A251__
 
mAPM_ExecAP     macro
        mov      R10,#API_ExecAP
		LCALL	 RSTFastAPI
        endm
mAPM_BankSwitch macro
        mov      R10,#API_BankSwitch
		LCALL	 RSTFastAPI
        endm
mAPM_MIDBankSwitch macro
        mov      R10,#API_MID_BankSwitch
		LCALL	 RSTFastAPI
        endm
#endif /*__A251__*/

#endif