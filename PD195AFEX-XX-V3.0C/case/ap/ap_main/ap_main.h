/*******************************************************************************
 *                              AS211A
 *                              Module:ap_main
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
* Descripton: the head file of main ap
*
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-06-29 15:00     1.0             build this file 
*******************************************************************************/
#ifndef AP_MAIN_H
#define AP_MAIN_H
#include "ap_common.h"
#define MAIN_ITEMCOUNT    16//支持ap的总数

extern int8 *usbpluginflag;
extern uint8 low_battery_value; //低电设置，os根据这个判断是否发低电消息
extern bool blnInPlayLong; 

extern void   _nop_ (void);

#endif