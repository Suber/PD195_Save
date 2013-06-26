/*
********************************************************************************
*                       ACTOS
*                  message api define
*                                                                         
*                (c) Copyright 2002-2003, Actions Co,Ld.                       
*                        All Right Reserved                               
*
* File   : message.h
* By     : Gongee
* Version: 1> v1.00     first version     2003-11-20 11:31
********************************************************************************
********************************************************************************
*/
#ifndef _MESSAGE_H
#define _MESSAGE_H
 
#include "actapi.h"

#define MSG_USB_STICK            0x91
#define MSG_USB_UNSTICK          0x92
#define MSG_RTC2HZ               0x80
#define MSG_BAT_V_CHG            0x81
#define MSG_LOW_POWER            0x82
#define Msg_RtcAlarm             0x83

#define MSG_SD_IN                0xb0   /*²å¿¨×´Ì¬*/
#define MSG_SD_OUT               0xb1   /*²¦¿¨×´Ì¬*/

#define MSG_UH_IN				 0xc0	/*UÅÌ²åÈë*/
#define MSG_UH_OUT				 0xc1	/*UÅÌ°Î³ö*/


#ifdef __C251__
#include <TypeExt.h>
void PutSysMsg(uint8 msg);
uint8 GetSysMsg(void);
#endif /* __C251__ */

#ifdef __A251__

mMSG_PutSysMsg  macro
        mov      R10,#API_MSG_PutSysMsg
 		LCALL     RSTFastAPI
        endm
mMSG_GetSysMsg   macro
          mov    R10,#API_MSG_PutSysMsg
		  MCALL_RSTFastAPI
        endm
#endif /* __A251__ */

#endif/*_MESSAGE_H*/