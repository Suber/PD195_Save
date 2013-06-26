/*
*********************************************************************
*File Name :udsik.h
*author    : Mars
*Date      : 2009-11-13 16:39
*Version   : 1.00
*******************************************************************
*/
#ifndef _UDISK_H_
#define _UDISK_H_
#include "actos.h"
#include "ui_res.h"
#include "ap_common.h"
#include "usbdisk.h"

//#define  BACKLIGHTCTL
#define BATTPICBUFFER 0xd000
#define BATTPICLENGTH 0x200

/***************func of ap*********************/
 bool UDiskInitAll(int para);
 int UdiskExitAll(void);
 void RamToMcu(void);
 void MsgManager(uint8 dispIndi);
 void Display(uint8 dispIndi);

 void UsbStandbyScreen(bool bStandby,char contrast);
 void FlashLed(void);
 int Adfumain(int para);

 bool ADFUInitAll(void);
 void MsgManager1(void);
 void Display1(char dispIndi);
 void ShowFwVersion(void);
 void ShowLogoUPG_DONE(void);
/***************************************************/

#endif
//#endif /*__IAR_SYSTEMS_ICC*/