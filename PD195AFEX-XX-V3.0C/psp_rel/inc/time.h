/*
********************************************************************************
*                       ACTOS
*                       ACTOS
*                  time define head file
*
*                (c) Copyright 2002-2003, Actions Co,Ld.
*                        All Right Reserved
*
* File   : time.h
* By     : Gongee
* Version: 1> v1.00     first version     2003-11-20 9:14
********************************************************************************
********************************************************************************
*/
#ifndef _TIME_H
#define _TIME_H

#include "actapi.h"
#include "typeext.h"

#define DOWNPOWERADDR   0x07
#define WORLD_DATE    0
#define CHINA_DATE    1

#ifdef __C251__



/* changed by Weng*/
typedef struct
{
    uint16 year;
    uint8 month;
    uint8 day;
}date_t;

typedef struct
{
        uint8 hour;
        uint8 minute;
        uint8 second;
}time_t;
 

//timer的周期为1ms
// timersub是timer响应函数入口地址，此处定义为int是为了方便c的非bank调用
 uint8 TM_SetTimer(int16 timersub, int8 period);
// 杀掉timer必须提供timer号
 bool TM_KillTimer(int8 timerno);

//clock的周期为0.5S,
// clocksub是clock响应函数入口地址，此处定义为int是为了方便c的非bank调用
 uint8 TM_SetClock(int16 clocksub, int8 period);
// 杀掉clock必须提供clock号
 bool TM_KillClock(int8 clockno);

//设置Alarm 中断的时间
 void TM_SetAlarmTime(time_t *time);

//时间为24小时制
 bool TM_SetTime(time_t *time);
 bool TM_GetTime(time_t *time);

//type must equ. 0 means 公历格式
 bool TM_SetDate(date_t *date, const int8 type);
 bool TM_GetDate(date_t *date, const int8 type);

//add by gongee@200408261711
//设置格林威治时间，格式:seconds pass 1970/01/01 00:00:00
 bool TM_GetGWTime(time_t *time);
 bool TM_SetGWTime(time_t *time);

//type must equ. 0 means 公历格式
 bool TM_GetGWDate(date_t *date, const int8 type);
 bool TM_SetGWDate(date_t *date, const int8 type);

//----------------------------------------------------------------------
 void TM_DelayMS(uint8 mscnt);
 uint32 TM_ChangetoStdSec(time_t *time,date_t *date);
 bool TM_ChangetoCalendar(uint32 cc_seconds,time_t *time,date_t *date);
 #define RTCFlush() {if(RTCREGUPDATE != 0xA5) RTCREGUPDATE = 0xA5;while(RTCREGUPDATE != 0x5A);} 
#endif /* __C251__ */

#ifdef __A251__
//*****************************************************
//此处宏的调用入口参数参考c原型

//定义api号
#define API_TM_SetTimer         0x0000+API_TM_BaseNoA
#define API_TM_KillTimer        0x1000+API_TM_BaseNoA
#define API_TM_SetClock         0x2000+API_TM_BaseNoA
#define API_TM_KillClock        0x3000+API_TM_BaseNoA
#define API_TM_SetAlarmTime     0x4000+API_TM_BaseNoA


#define API_TM_GetTime          0x0001+API_TM_BaseNoA
#define API_TM_GetDate          0x1001+API_TM_BaseNoA
#define API_TM_GetDateAndTime   0x2001+API_TM_BaseNoA
//#define API_StdSectoTime        0x0301+API_TM_BaseNoA

#define API_TM_SetTime          0x0002+API_TM_BaseNoA
#define API_TM_SetDate          0x1002+API_TM_BaseNoA

#define API_TM_GetGWTime        0x0003+API_TM_BaseNoA
#define API_TM_GetGWDate        0x1003+API_TM_BaseNoA
#define API_TM_GetGWDateAndTime 0x2003+API_TM_BaseNoA

#define API_TM_SetGWTime        0x0004+API_TM_BaseNoA
#define API_TM_SetGWDate        0x1004+API_TM_BaseNoA

#define API_TM_ChangetoStdSec   0x0005+API_TM_BaseNoA

#define API_TM_ChangetoCalendar 0x0006+API_TM_BaseNoA
#define API_StdSectoDate        0x1006+API_TM_BaseNoA


//#ifdef     USE_EXTERNAL_RTC
    #define API_TM_GetGWTime_RTCIC  0x000c + API_UI_BaseNoA     //放在显示驱动
    #define API_TM_GetGWDate_RTCIC  0x100c + API_UI_BaseNoA     //放在显示驱动
    #define API_TM_getcurtime       0x200c + API_UI_BaseNoA     //放在显示驱动
    
    #define API_TM_SetGWTime_RTCIC  0x000d + API_UI_BaseNoA     //放在显示驱动
    #define API_TM_SetGWDate_RTCIC  0x100d + API_UI_BaseNoA     //放在显示驱动
    
    #define API_Ertc_HT1380_AllGet  0x000e + API_UI_BaseNoA     //放在显示驱动
    #define API_Ertc_HT1380_AllSet  0x100e + API_UI_BaseNoA     //放在显示驱动
//#endif


 

//定义宏
mTM_SetTimer       macro
       MOV 				 WR8,#API_TM_SetTimer
       LCALL  		RSTBankAPI
       endm       
mTM_KillTimer   macro
        MOV     WR8,#API_TM_KillTimer
        LCALL     RSTBankAPI
        endm
mTM_SetClock    macro
        MOV     WR8,#API_TM_SetClock
        LCALL     RSTBankAPI
        endm
mTM_KillClock   macro
        MOV     WR8,#API_TM_KillClock
        LCALL     RSTBankAPI
        endm

mTM_SetAlarmTime   macro
        MOV     WR8,#API_TM_SetAlarmTime
        LCALL     RSTBankAPI
        endm

mTM_SetTime     macro
        MOV     WR8,#API_TM_SetTime
        LCALL     RSTBankAPI
        endm
mTM_GetTime     macro
        MOV     WR8,#API_TM_GetTime
        LCALL     RSTBankAPI
        endm
mTM_SetDate     macro
        MOV     WR8,#API_TM_SetDate
        LCALL     RSTBankAPI
        endm
mTM_GetDate     macro
        MOV     WR8,#API_TM_GetDate
        LCALL     RSTBankAPI
        endm


mTM_SetGWTime     macro
        MOV     WR8,#API_TM_SetGWTime
        LCALL     RSTBankAPI
        endm
mTM_GetGWTime     macro
        MOV     WR8,#API_TM_GetGWTime
        LCALL     RSTBankAPI
        endm
mTM_SetGWDate     macro
        MOV     WR8,#API_TM_SetGWDate
        LCALL     RSTBankAPI
        endm
mTM_GetGWDate     macro
        MOV     WR8,#API_TM_GetGWDate
        LCALL     RSTBankAPI
        endm


mTM_ChangetoStdSec     macro
        MOV     WR8,#API_TM_ChangetoStdSec
        LCALL     RSTBankAPI
        endm
mTM_ChangetoCalendar     macro
        MOV     WR8,#API_TM_ChangetoCalendar
        LCALL     RSTBankAPI
        endm
mTM_DelayMS macro
        LCALL     DELAYFunAPI
        endm	
#endif /* __A251__ */
#endif/*_TIME_H*/
