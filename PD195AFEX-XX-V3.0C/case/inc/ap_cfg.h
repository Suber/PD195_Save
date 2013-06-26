/*
*******************************************************************************
*                ACTOS AP
*        ap config header  file
*
*        (c) Copyright, Actions Co,Ld.
*             All Right Reserved
*
*  $Id: ap_cfg.h,v 1.7 2009/07/30 14:54:29 mikeyang Exp $
*******************************************************************************
*/
#ifndef AP_CFG_H
#define AP_CFG_H

#ifdef MAIN_AP
#include "cfg_MainAP.h"   
#endif

#ifdef UDISK_AP
#include "CFGUDISK_AP.h"
#endif

#ifdef MUSIC_AP
#include "cfgmusic_ap.h"
#endif

#ifdef RADIO_AP
#include "cfgRADIO_AP.h"
#endif


#define     MACHINE_MODEL           2   //'2' PD195
										//'1' SC903: usb耳機共用注意: 5v檢測，耳機檢測无，耳機控制c4
										
#define     LCD_TYPE                4   //'1' SSD1303 RiT
                                        //'2' SSD1306 UG
                                        //'3' SSD1306 UG New
                                        //'4' SSD1306 UG w/ DC-DC
                                      
#define     Volume_Ctl              1   //'0' 40=600mv
										//'1' 32=250mv
										//'2' 27=140mv
										//'2' SSD1305 UG
										
#define     EARPHONE_ON             0   //" " = on level
										
#define     PETOMETER_USE           1
#define     PETOMETER_METHOD        2   //'1' no HPF; '2" with HPF
#define     SWIM_USE                1   //SWIM FITNESS
/*#define     G_TYPE                1   //'0' no use, no pedometer
                                        //'1' MXC62025M
										//'2' MMA8452Q
										//'3' LIS3DH    */

#define     USB_EMI                     //USB_EMI

#define     WEIGHT_MAX              500

										
//define this flag when release      
//#define SupportCard              // 支持插卡 
//#define SupportWMAREC          //支持WMA格式录音 
#define Msg_CTC2HZ             //使用CTC定时的2HZ消息
#define UNICODE_SUPPORT       //支持unicode多国语言驱动
#define NEWFORMATRES          //支持Excel输入Unicode编码新格式Res文件
//#define ALARM_CLOCK             //支持闹钟功能
//#define AUTO_FMREC              //支持FM定时自动录音功能

#ifdef __C251__
/*固件配置相关数据结构定义*/
typedef struct
{
     uint8  Total; //支持的语言总数,最大25种语言
     uint8  Active;//当前默认语言的ID号
     uint8  Table[62]; //语言ID号数组，每个数组成员表示不同语言的ID，取值范围为为0-29，0xff表示到结尾
}lan_set_t;

typedef struct
{
     uint8  Total;      //支持的按键定义总数，最多支持16个按键
     uint8  Table[16];  //按键消息值定义数组，每个数组成员表示不同的按键消息
}key_set_t;

typedef struct
{
     uint8  Total;     //支持的AP的总数
     uint8  Active;    //进入主菜单默认ap的编号
     uint8 Table[16]; //AP对应的数组，每个数组成员两个字节，第一个表示AP编号,第二个表示AP状态
}ap_set_t;

typedef struct
{
    uint8  DefValue;  //默认值
    uint8  Max;      //最大值
    uint8  Min;      //最小值 
    uint8  Step;     //步长  
}value_set_t;

typedef struct
{
    uint16        Magic;     
    lan_set_t     LanInfor;
    key_set_t     KeyInfor;
    ap_set_t      ApInfor;
    value_set_t   Autolock;
    value_set_t   ContrInfor;
    value_set_t   BacklightTime;
    value_set_t   PowerOfftime;
    value_set_t   SleepTime;
    value_set_t   Volume;
    value_set_t   VolDefault;
    value_set_t   RecordGain; 
    value_set_t   Stride;   //pedometer
    value_set_t   PedoUnit;
    value_set_t   Weight;
    value_set_t   ArmLen;
    value_set_t   PoolLen;  
    value_set_t   Gender;
    value_set_t   Age;
    value_set_t   Height;
    uint8  DevVesion[32];
	uint8  FirmwareDate[32]; 
	uint8  CardFlag;      //插卡支持，0，不支持插卡；1，支持插卡
	uint8  EarphoneFlag;  //耳机直驱，0，不支持，1，支持	
	uint8  ClassDFlag;	  //D类功放，外挂喇叭时使用，0，不支持，1，支持	
    uint8  KeyToneFlag;
    uint8  PedoFlag;   //pedometer
}vm_cfg_t;


/*配置项数据结构定义*/
typedef struct
{
    char   CfgMagic[4];
    uint8  TotalItem;
    char   Reserv[11];
}file_cfg_head_t;

typedef struct
{
    uint32 Offset;
    char   EName[14];
    char   CName[14];
}cfg_item_t;  // 32 bytes


/*参数类配置项头信息数据结构定义*/
typedef struct
{
    uint32 ValueOffset;
    uint8  ValueTotal;
    uint32 StringOffset;
    uint8  StringTotal;
    char   Reserv[6];
}paraset_head_t;  //16 bytes

/*数值类配置项数据结构定义*/
typedef struct
{
    uint8  DefValue;
    uint8  Max;
    uint8  Min;
    uint8  Step;
    uint8  Reserv[12];
    char   EName[16];
    char   CName[16];
}value_t;  //48 bytes

/*字符串类配置项数据结构定义*/
typedef struct
{
    char   StringCfg[32];
    char   EName[16];
    char   CName[16];
}string_t; //64 bytes

/*按键类配置项数据结构定义*/
typedef struct
{
    uint8  TotalKey;
    uint8  TotalItem;
    char   Reserv[14];
    char   KeyValue[16];
}key_cfg_t; //32 bytes

typedef struct
{
    uint8  Value;
    uint8  Status; //0 表示可配置的按键，1表示不可配置
    char   Description[14];
}key_infor_t; //32 byt

/*AP配置项头信息数据结构定义*/
typedef struct
{
    uint8  Total;
    uint8  Active;
    char   Reserv[14];
}ap_cfg_head_t;  //16 bytes

/*ap 配置信息数据结构*/
typedef struct
{
    uint8  Index;
    uint8  Status;
    char   EName[15];
    char   CName[15];
}ap_infor_t;  //32 bytes

typedef struct
{
    uint8  Total; //支持的语言总数,最大25种语言
    uint8  Active;//当前默认语言的ID号
    uint8  Table[62]; //语言ID号数组，每个数组成员表示不同语言的ID，取值范围为为0-29，0xff表示到结尾
}lang_infor_t; //64 bytes

typedef struct
{
    uint8  Total; //当前可替换的固件文件
    uint8  Reserv[15]; //保留
}file_ex_t; //16 bytes

typedef struct
{
    uint8  Total; //当前可配置项的总数
    uint8  Reserv[15]; //保留
}func_cfg_t; //16 bytes

typedef struct
{
    uint8  State; //功能支持项的状态，0，不支持，1，支持。
    uint8  Reserv[15]; //保留
    uint8  EName[16];  //英文名
    uint8  CName[16];  //中文名
}func_item_t; //48 bytes

#endif      /*end of #ifdef __C251__*/

#endif

