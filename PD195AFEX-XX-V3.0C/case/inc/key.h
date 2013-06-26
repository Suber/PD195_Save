/*
********************************************************************************
*                       ACTOS
*                  key board message define
*                                                                         
*                (c) Copyright 2002-2003, Actions Co,Ld.                       
*                        All Right Reserved                               
*
* File   : key.msa
* By     : Gongee
* Version: 1> v1.00     first version     2003-11-20 9:14
********************************************************************************
********************************************************************************
*/
#ifndef _KEY_H			//防止重定义
#define _KEY_H
#include "actapi.h"
#include "port.h"
#include "actapi.h"
#include "driver.h"
#include "sddrv.h"
#include "sdfs.h"
#include "sysdef.h"
#include "ap_cfg.h"

#define lock_key

/*define key message of key driver*/
#define Msg_KeyNull		      0x00
#define Msg_KeyPlayPause      0x01
#define Msg_KeyLoop           0x02
#define Msg_KeyVolSub         0x03
#define Msg_KeyVolAdd         0x04
#define Msg_KeyLast           0x05
#define Msg_KeyNext           0x06
#define Msg_KeyMenu           0x07
#define Msg_KeyREC            0x08
#define Msg_KeyEQ             0x09
#define Msg_KeyVol			  0x0a
	
#define Msg_KeyShortUp        0x30    //短按键抬起 
#define Msg_KeyLongUp         0x31    //长按键抬起 
//#define Msg_KeyStandby          0x21
#define Msg_KeyHold           0x32        //避免和Msg_KeyLoop|AP_KEY_UP冲突，Mars，2008-04-16
#define Msg_KeyUnHold         0x34
#define Msg_HalfSecond        0x35

/*define key message of ap  */
#define AP_KEY_LONG       0x80
#define AP_KEY_HOLD       0x40
#define AP_KEY_UP         0x20
#define AP_KEY_NULL       Msg_KeyNull
#define AP_KEY_MODE       Msg_KeyMenu
#define AP_KEY_PLAY       Msg_KeyPlayPause
#define AP_KEY_NEXT       Msg_KeyNext
#define AP_KEY_PREV       Msg_KeyLast
#define AP_KEY_VOLADD     Msg_KeyVolAdd
#define AP_KEY_VOLSUB     Msg_KeyVolSub
#define AP_KEY_AB         Msg_KeyREC
#define AP_KEY_EQ         Msg_KeyEQ
#define AP_KEY_LOOP       Msg_KeyLoop
#define AP_KEY_VOL 	      Msg_KeyVol

/*采用GPIO的方式检测耳机插入，如果使用线控请注释掉该宏定义*/
//#define GPIO_EAR_DETECT	 

//#endif         //#ifdef Use_RotateKey
#ifdef __C251__
bool sKY_HoldCheck(void);		//TRUE为HOLD，FALSE为UNHOLD
bool sKY_SetBLightColor(uint8 BLightColor);//设置背光颜色。for s2 by ccm
bool sKY_OpenBacklight(void);    //打开背光
bool sKY_CloseBacklight(void);    //关闭背光
bool sKY_ChargeSet(uint8 Setting);    //关闭背光
bool sKY_Beep(uint8 value);                 //key tone
uint8 sKY_ChargeGetandSet(void);      //充电检测 
void sKY_CtcIRQIntercept(void);     
void sKY_CtcIRQRelease(void);     
uint8 *sKY_GetKeyTabAddress(void);
uint8 sKY_GetEarState(void);

void sKY_GsensorCheck();
void sKY_GsensorOn();
void sKY_GsensorOff();
void sKY_ReadGSensor();
bool BAPI_sKY_GsensorOn1();
void BAPI_sKY_GsensorOff1();
void BAPI_sKY_ReadGSensor1();
bool BAPI_sKY_GsensorOn2();
void BAPI_sKY_GsensorOff2();
void BAPI_sKY_ReadGSensor2();
bool BAPI_sKY_GsensorOn3();
void BAPI_sKY_GsensorOff3();
void BAPI_sKY_ReadGSensor3();

void sKY_SetGsensor(uint8 para, uint32 step, uint32 second, uint8 mode);
void sKY_BankGsensorGetStep(uint32 *pStep, uint32 *pSecond, uint8 mode);
uint16 sKY_BankGsensorGetADC(uint8 para, uint8 mode);
#endif /* _C251_ */



#ifdef __A251__       //汇编使用的定义 

////BANKA，函数的地址 
#define API_HoldCheck         0x2002 + API_KY_BaseNoA    //02的取值根据ky_hard_bnkif.msa中的排列 
#define	API_OpenBacklight	  0x3000 + API_KY_BaseNoA
#define	API_CloseBacklight	  0x4000 + API_KY_BaseNoA
//#define API_ChargeSet         0x0600 + API_KY_BaseNoA      
#define API_Beep              0x1002 + API_KY_BaseNoA  
#define API_GetEarState       0x3002 + API_KY_BaseNoA  
       
//#define API_ChargeGetandSet   0x0001 + API_KY_BaseNoA   
//#define API_CardPowerOn       0x0101 + API_KY_BaseNoA    //在"actapi.h"中定义  
#define API_CtcIRQIntercept   0x2001 + API_KY_BaseNoA
#define API_CtcIRQRelease     0x3001 + API_KY_BaseNoA 
 
#define API_GetKeyTabAddress  0x0002 + API_KY_BaseNoA 	 //b2f0

#define API_ChargeSet         0x0001 + API_KY_BaseNoA  	 //b1f0
#define API_ChargeGetandSet   0x1001 + API_KY_BaseNoA		//b1f1

#define API_SetGsensor      	0x4002 + API_KY_BaseNoA         
#define API_BankGsensorGetStep  0x5002 + API_KY_BaseNoA
#define API_BankGsensorGetADC   0x6002 + API_KY_BaseNoA

#define API_GsensorOn1      	0x0003 + API_KY_BaseNoA         
#define API_GsensorOff1         0x1003 + API_KY_BaseNoA
#define API_ReadGSensor1        0x2003 + API_KY_BaseNoA
#define API_GsensorOn2      	0x0004 + API_KY_BaseNoA         
#define API_GsensorOff2         0x1004 + API_KY_BaseNoA
#define API_ReadGSensor2        0x2004 + API_KY_BaseNoA
#define API_GsensorOn3      	0x0005 + API_KY_BaseNoA         
#define API_GsensorOff3         0x1005 + API_KY_BaseNoA
#define API_ReadGSensor3        0x2005 + API_KY_BaseNoA


#endif/**251_ASM***/
#endif/*_KEY_H*/
