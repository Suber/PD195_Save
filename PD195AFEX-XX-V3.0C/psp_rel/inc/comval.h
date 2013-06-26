/*
********************************************************************************
*                       ACTOS
*                  common value define head file
*
*                (c) Copyright 2002-2003, Actions Co,Ld.
*                        All Right Reserved
*
* File   : comval.h
* By     : Gongee
* Version: 1> v1.00     first version     2003-12-2 18:37
********************************************************************************
********************************************************************************
*/
#ifndef _COMVAL_H
#define _COMVAL_H

#include "sysdef.h"
#include "TypeExt.h"

 


//ÒÔÏÂmagicÓÃÓÚÅĞ¶ÏvmÀïµÄÊı¾İÊÇ·ñºÏ·¨,uint16 ÀàĞÍ
#define MAGIC_KVAL              0xdeed
#define MAGIC_COMVAL            0xdead
#define MAGIC_MUSIC             0xbeef
#define MAGIC_VOICE             0xfee0
#define MAGIC_RECORD            0x3d3d
#define MAGIC_FMRADIO           0xad01
#define MAGIC_SETTING           0xbaba
#define MAGIC_UDISK             0xee77
#define MAGIC_TESTER            0x9801
#define MAGIC_STANDBY           0x3935
#define MAGIC_UPGRADE           0x3951
#define MAGIC_MRECORD           0xcc00       //for M-Record by ccm
#define MAGIC_VIEWPIC		0x4a50		//for picture view
#define	MAGIC_READER		0x7478		//for reader "tx"
#define	MAGIC_AMV		0x616d		//for amv file "am"
#define    MAGIC_CFG        0x55aa        //for modify information
//language id
#ifdef PC
#define LAN_ID_SCHINESE        0           //ÖĞÎÄ¼òÌå
#define LAN_ID_TCHINESE        2           //ÖĞÎÄ·±Ìå
#endif
//#define LAN_ID_ENGLISH         1           //Ó¢ÎÄ
//#define LAN_ID_TCHINESE        2           //ÖĞÎÄ·±Ìå
//#define LAN_ID_JAPANESE        3           //ÈÕÎÄ
//#define LAN_ID_KOREAN         4            //º«ÎÄ
//#define LAN_ID_RUSSIAN         5            //¶íÂŞË¹Óï
//#define LAN_ID_RABBINIC        6           //Î÷²®À³ÎÄ
//#define LAN_ID_THAI                7            //Ì©¹úÓï
//#define LAN_ID_FRENCH             8           //·¨Óï
//#define LAN_ID_GERMANY           9          //µÂ¹úÓï
//#define LAN_ID_ITALIAN             10          //Òâ´óÀûÓï
//#define LAN_ID_DUTCH              11          //ºÉÀ¼Óï
//#define LAN_ID_PORTUGUESE        12          //ÆÏÌÑÑÀÓï
//#define LAN_ID_SPANISH             13         //Î÷°àÑÀÓï 
//#define LAN_ID_SWEDISH            14         //ÈğµäÓï
//#define LAN_ID_CZECHISH           15        //½İ¿ËÓï
//#define LAN_ID_POLAND             16         //²¨À¼Óï
//#define LAN_ID_FINNISH             17        //·ÒÀ¼Óï
//#define LAN_ID_DENISH              18         //µ¤ÂóÓï
//#define LAN_ID_HUNGARIAN         19         //ĞÙÑÀÀûÓï
//#define LAN_ID_SLOVAKIAN         20         //Ë¹Âå·¥¿ËÓï
//batt type
#define BATT_TYPE_ALKALINE    0
#define BATT_TYPE_NIH        1
#define BATT_TYPE_LITHIUM    2

#ifdef __C251__

//************************************
//ÏµÍ³¹«¹²±äÁ¿¶¨Òå
//************************************
typedef struct
{
        //magic
        uint16 magic;    //"KVAL"
        //ÏµÍ³ÆğÊ¼Ê±¼ä(°ëÃëÎªµ¥Î»)
        //ÏµÍ³µ±Ç°µÄÊ±¼äµÈÓÚÏµÍ³ÆğÊ¼Ê±¼ä¼ÓÏµÍ³time counterµÄÊ±¼ä
        uint32 systemtime;
        uint32 gwtime;
        int8  rtcresetflag;//Ä¬ÈÏÎªÕı³££¬rtcresetflag=0x00£¬Èç¹ûrtcµôµçrtcresetflag=0x55
}kval_t;

typedef struct
{
        //magic
        uint16 magic;

        //ÏµÍ³ÆğÊ¼Ê±¼ä(°ëÃëÎªµ¥Î»)
        //ÏµÍ³µ±Ç°µÄÊ±¼äµÈÓÚÏµÍ³ÆğÊ¼Ê±¼ä¼ÓÏµÍ³time counterµÄÊ±¼ä
        uint32 systemtime;
        //ÏµÍ³Ê±¼ä±ÈÂÊ
        uint16  reserve1;
        //int16 RTCRate;

        //¶Ô±È¶ÈÑ¡Ôñ6~21
        uint8 DisplayContrast;

        //±³¾°µÆÁÁÊ±¼ä,Ä¬ÈÏÖµÎª0ah=5Ãë
        uint8 LightTime;

        //Auto lockÊ±¼äë
        uint8 LockTime;

        //standby¶¨Ê±
        uint8 StandbyTime;

        //sleep¶¨Ê±
        uint8 SleepTime;

        //½çÃæÓïÑÔ, ¼òÌå:0, Ó¢ÎÄ:1, ·±Ìå:2
        int8 langid;

        //¸´¶ÁÄ£Ê½
        uint8 ReplayMode;

        //Áª»úÄ£Ê½
        uint8 OnlineMode;

        uint8 BatteryType;                       //modify by gongee 2004-05-24 09:21
        uint8 FMBuildInFlag;                     //add battery select and fm select flag

        //Â¼ÒôÀàĞÍ,0:ÓïÑÔÂ¼Òô(V-Record),1:ÒôÀÖÂ¼Òô(M-Record). add by ccm 2004-06-08
        uint8 RecordType;

        //±³¹âµÆÑÕÉ«  7Color BackLight. For s2 by ccm
        uint8 BLightColor;
        
        //bit0~bit1: UdiskĞòÁĞºÅÖ§³Ö:0:no sn; 1:unified sn; 2:random sn;
        //bit4: UdiskÈÏÖ¤Ä£Ê½Ö§³Ö: 1:½øÈëUdiskÈÏÖ¤Ä£Ê½,Æô¶¯Ê±¼ä¼Ó¿ì,Ö§³ÖsuspendºÍresumeµÈUSBÈÏÖ¤¹¦ÄÜ
        int8 udisk_setting;
        
  	    uint8 Lightmode;	//0: ±ä°µÄ£Ê½ 1: ±äºÚÄ£Ê½	
        uint8 SuppCard;      //Ö§³Ö¿¨Ñ¡Ôñ  0:²»Ö§³Ö  1:Ö§³Ö
        
        uint8 MTPFormatType; //ÔÚMTPÏÂ¸ñÊ½»¯´ÅÅÌÀàĞÍÑ¡Ôñ  0:FAT16  1:FAT32


        uint8 FMTag;
        
//        int8 EarProtect_flag;
//        int8 EarProtectThreshold;
//        int8 attenuation_flag;
//        int16  auto_switchoff_time;

		uint8 KeyTone;				//add by mzh 2007.3.14 0: no key tone 1:have key tone
        uint8 BackLightMax;  //±³¹â×î´óÁÁ¶Èµ÷½Ú
        uint8 BackLightDef;  //Ä¬ÈÏ±³¹âÁÁ¶È

        uint8 VolumeMax;    //ÒôÁ¿×î´ó¼¶Êıµ÷½Ú
        uint8 VolumeDef;    //Ä¬ÈÏÒôÁ¿Öµ
        
 
		
        uint8 ClassDFlag;   //DÀà¹¦·ÅÖ§³Ö
        uint8 EarphoneFlag; //¶ú»úÖ±ÇıÖ§³Ö 
        uint8 RecordGain;
        
        
				uint8 Boxit;  //ÍÆÏä×Ó
				uint8 Tetris;  //¶íÂŞË¹·½¿é
				uint8 Snaks;   //Ì°³ÔÉß
				uint8 Riddle;   //»ªÈİµÀ
				uint8 playpowerofflag;
		
		uint8 rec_bitrate;//Â¼ÒôµÄÖÊÁ¿
		
		uint8 Stride;   //pedometer
        uint8 PedoUnit;   
        uint16 Weight; 
        uint8 ArmLen; 
        uint8 PoolLen; 
        uint8 PedoFlag;
        uint8 Age;
        uint8 Gender;
        uint8 Height;
//        int8 reserve[32-30];
}comval_t;

//³õÊ¼»¯ comval  µÄºê
#define COMVAL_INIT(val) \
do{ \
        (val).DisplayContrast = 11; \
        (val).langid = LAN_ID_SCHINESE; \
        (val).LightTime = 10; \
        (val).LockTime = 10; \
        (val).magic = MAGIC_COMVAL; \
        (val).ReplayMode = 0; \
        (val).SleepTime = 0; \
        (val).StandbyTime = 30; \
        (val).OnlineMode = 0; \
        (val).systemtime = 0L; \
        (val).BatteryType = 3; \
        (val).FMBuildInFlag = 0; \
        (val).RecordType = 0; \
        (val).BLightColor = 4; \
        (val).Onlinedev = 0; \
        (val).Stride = 0;\
        (val).PedoUnit = 0;\
        (val).Weight = 0;\
        (val).ArmLen = 0;\
        (val).PoolLen = 0;\
        (val).Gender = 0;\
        (val).Age = 0;\
        (val).Height = 0;\
//        (val).EarProtect_flag = 0; \
//        (val).EarProtectThreshold = 22; \
//        (val).attenuation_flag = 0; \
//        (val).auto_switchoff_time = 0; \
}while(0)

#endif/*__C251__*/


#endif/*_COMVAL_H*/
