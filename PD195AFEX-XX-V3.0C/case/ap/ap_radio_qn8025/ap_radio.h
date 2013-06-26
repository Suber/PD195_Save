/*
 ********************************************************************************
 *                       ACTOS
 *                  ap_radio head file
 *
 *                (c) Copyright 2002-2003, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File   : ap_radio.h
 * By     : ybfu
 * Version: 1> v1.00     first version     2004-1-12 13:45
 ********************************************************************************
 ********************************************************************************
 */
#ifndef _APRADIO_H
#define _APRADIO_H
#include "actos.h"
#include "ap_common.h"
#include "filesys.h"
#include "basal.h"
#include "fmradio.h"
#include "stringH.h"
//#ifdef FranceVoice
//#define MAXVOLUME    16
//#else
#define MAXVOLUME    32
//#endif
//#define  HARDWARE_SEEK 				   //硬件SEEK 编译开关
#define I2C_DEV0_ADDRESS 0x20 // 00100000B, LSB will be set to "1" (address = 0x21) when do read accessing
#define I2C_TIMEOUT_TIME    10
#define I2C_TIMEOUT_COUNT    8
#define MS_DELAY_CONST    40

#define	FM_BAND_US_BOTTOM		875	
#define	FM_BAND_US_TOP			1080
#define	FM_BAND_JP_BOTTOM		760	
#define	FM_BAND_JP_TOP			900
#define	FM_DEFAULT_FREQ			898

#define QND_BAND_NUM     6
#define RSSINTHRESHOLD   4
#define QND_MODE_RX                     0x8000
#define QND_MODE_TX                     0x4000
#define QND_MODE_FM                     0x0000
#define COUNTRY_CHINA                   0
#define COUNTRY_USA                     1
#define COUNTRY_JAPAN                   2

#define QND_MODE_SLEEP                  0     
#define QND_MODE_WAKEUP                 1
#define QND_MODE_DEFAULT    						2
#define CCA_CH_DIS      				0x01
//#define CHSC            			0x02
#define RDSEN           				0x08
#define CH_CH		    						0x03
#define CH_CH_START     				0x0c
#define CH_CH_STOP      				0x30
//#define STNBY           				0x20
#define RX_MODE         				0x10
//#define IMR             			0x40
#define RDS_RXUPD       				0x80
#define ST_MO_RX                0x01
//#define R_TXRX_MASK             0x30
#define RX_MONO_MASK    				0x04
#define PA_CTL_PORTNUM   				0x05
extern uint16 WFreq;
#pragma ROM(large)
extern void Search_TimerInt(void);
#pragma ROM(huge)

extern void ui_show_FMdata(uint8 *buf, uint8 x, uint8 y);
extern void ShowStationCount(uint8 count);
extern void ui_show_volfm(uint8 volume);
extern void QN8025_RST(uint8 i);
extern void QNF_SetMute(BYTE On);
extern uint8 fmSendCommand(uint8 cmd, uint16 param);
extern void QND_SetSysMode(uint16 mode);
extern void CleanStationCount(void);
//extern uint8 FM_Standby(void);
#define     SEARCHLEVEL    10
#define Read_SDA     (GPIOBDAT & 0x10)

//#define     F_COMVAL                            g_comval
#ifdef __C251__

typedef struct
{
    uint16 *param1; //电台表地址指针，20个（US模式）/20个（Japan模式）
    uint16 *param2; //当前电台信息的地址指针
}FMRADIO_param_t;
extern uint16 g_CNT;
extern uint8 AutoTabNum;
extern bool new_tab;
#ifdef EAR_PROTECT
extern int16 g_autoswitch_counter;
#endif
#endif /* __C251__ */
#endif/*_APRADIO_H*/
