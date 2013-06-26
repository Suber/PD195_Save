/*
 *******************************************************************************
 *                ACTOS AP
 *        ap common lib header  file
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *  $Id: ap_common.h,v 1.1.1.1 2008/01/11 07:28:06 rowena Exp $
 *******************************************************************************
 */

#ifndef AP_COMMON_H
#define AP_COMMON_H

#include "actos.h"
#include "ui_res.h"
#include "enhanced.h"
#include "display.h"
#include "key.h"
#include "common.h"
#include "ap_cfg.h"

#include "stringH.h"
//event, 1~8 系统保留, 9~16 用户使用
#define        AP_EVENT_MAIN        0x0001
#define        AP_EVENT_STANDBY    0x0002
#define        AP_EVENT_RECORD        0x0004
#define        AP_EVENT_UDISK        0x0008
#define        AP_EVENT_REDRAW        0x0010
#define        AP_EVENT_CAMERA     0x0020
// ~8 os reserved
#define        AP_EVENT_9        0x0100
#define        AP_EVENT_10        0x0200
#define        AP_EVENT_11        0x0400
#define        AP_EVENT_12        0x0800
#define        AP_EVENT_13        0x1000
#define        AP_EVENT_14        0x2000
#define        AP_EVENT_15        0x4000
#define        AP_EVENT_16        0x8000

#define        AP_EVENT_ALL        0xffff
#define        AP_EVENT_ANY        0x0000

//main ap startup ap id
#define        RESULT_NULL         0
#define        RESULT_MUSIC        1             //音乐
#define        RESULT_VOICE        2            //放音
#define        RESULT_RADIO        3            //FM收音
#define        RESULT_SYSTEM       4          //系统设置
#define        RESULT_REC_NOSTART  5            //录音
#define        RESULT_VIEW         6            //图片浏览
#define        RESULT_MOVIE        7            //电影
#define        RESULT_READER       8            //电子书
#define        RESULT_GAME         9           //游戏
#define        RESULT_TELBOOK      10            //电话本
#define        RESULT_BROWSER      11         //文件浏览
//12 - 19预留为新增的AP的结果消息
#define        RESULT_RECORD        20      //录音热键消息
#define        RESULT_TESTER        21
#define        RESULT_STANDBY       22
#define        RESULT_UPGRADE       23
#define        RESULT_UDISK         24
#define        RESULT_MAIN          25
#define        RESULT_FIRST_BOOT    26
#define        RESULT_PDDRM         27
#define        RESULT_XML2HDS       28
#define        RESULT_BUILDLIST      29
#define        RESULT_ONLINEDEVICE_SWITCH   30
#define        RESULT_UDISK1                31
#define        RESULT_MYTST            32

#define        RESULT_MUSICEDIT         33
//radio.ap return param.
#define        RESULT_FMREC_START       34
#define        RESULT_FMREC_NOSTART     35
#define        RESULT_FMREC_WAV         36
#define        RESULT_FMREC_ACT         37
#define        RESULT_FMREC_SAVE        38
#define        RESULT_FMREC_AB          39
#define        RESULT_CAMERA            40
#define        RESULT_MUSIC_PLAY        41
#define        RESULT_VOICE_PLAY        42
#define        RESULT_MOVIE_PLAY        43
#define        RESULT_READER_PLAY       44
#define        RESULT_VIEW_PLAY         45

//return message id
#define        RESULT_REDRAW         46    //界面需要重画
#define        RESULT_POWERON        47    //从standby返回
#define        RESULT_IGNORE         48    //控件取消选择
#define        RESULT_SD_IN          49
#define        RESULT_SD_OUT         50
#define        RESULT_SYSTEM_ADFU       51
#define        RESULT_AUDIBLE_PLAY      52   //音乐
#define        RESULT_AUTORECORD        53
#define        RESULT_AUTORECORD_START  54
#define        RESULT_TIMER             55
#define        RESULT_FMRECORD_POWERON  56
#define        RESULT_ALARM_POWERON     57
#define        RESULT_ALARM             58   //闹钟
#define        RESULT_RETURN_TO_MAIN    59
#define        RESULT_AUTO_BACK         60

//user define，预留20个
#define        RESULT_USER1             61

//system  msg  define
#define        USB_PLUGIN_FINISH             80
#define        MSG_USB_STICK_FLAG            81
#define        AP_MSG_USB_PLUGIN_FLAG        82
#define        AP_MSG_SD_OUT                 83
#define        AP_MSG_SD_IN                  84
#define        AP_MSG_ALARM                  85
#define        AP_MSG_FMRECORD               86
#define        AP_MSG_TIMER                  87
#define        AP_MSG_LOCK                   88
#define        AP_MSG_USB_PLUGIN             89
#define        AP_MSG_CHARGING               90
#define        AP_MSG_RTC                    91
#define        AP_MSG_STANDBY                92
#define        AP_MSG_WAIT_TIMEOUT           93
#define        AP_MSG_FORCE_STANDBY          94    //强制关机
#define        DELETE_FILE        0
#define        DELETE_MUSIC        1
#define        DELETE_VOICE        2
#define        DELETE_AMV        3
#define        DELETE_PIC        4
#define        DELETE_TXT        5
//#define        DELETE_ALL        3    //this was not impletemented
//#define        FSEL_TYPE_DIR      7
#define        DIRECTORY_MUSIC        FSEL_TYPE_MUSIC
#define        DIRECTORY_VOICE        FSEL_TYPE_VOICE
//#define        DIRECTORY_ALL        2
//resource type
#define        RES_TYPE_PIC        1
#define        RES_TYPE_STRING        2
#define        RES_TYPE_MSTRING    3
#define        RES_TYPE_LOGO        4

//屏幕大小
#define        SCR_WIDTH         Display_Length
#define        SCR_HEIGHT        Display_Height

//字符, 中文字符的宽度
#define        LARGE_CHAR_WIDTH        8
#define        CHAR_WIDTH        6
#define        TINY_CHAR_WIDTH        4
#define        TCHAR_WIDTH        8

/*磁盘状态标志定义*/
#define SETDISKOKFLAG_DEF 0x80
#define CLRDISKOKFLAG_DEF 0x7f
#define SETDISKNOFILE_DEF  0x40
#define CLRDISKNOFILE_DEF 0xbf
#define SETCARDOKFLAG_DEF 0x08
#define CLRCARDOKFLAG_DEF 0xf7
#define SETCARDNOFILE_DEF  0x04
#define CLRCARDNOFILE_DEF  0xfb

//主界面各个控件的x坐标
//======下面定义图标显示的坐标=========
//主界面各个控件的x坐标
#if 0
#define        POSITION_A        (3)//音乐类型显示
#define        POSITIONY_A       (48+64)

#define        POSITION_B        (28)//循环类型显示
#define        POSITIONY_B       (48+64)

#define        POSITION_C        (43)//EQ显示
#define        POSITIONY_C       (48+64)

#define        POSITION_C2        (43)//EQ2显示
#define        POSITIONY_C2       (56+64)

#define        POSITION_D        (62)//当前时间显示
#define        POSITIONY_D       (56+64)

#define        POSITION_D2       (42)//总时间
#define        POSITIONY_D2      (116)

#define        POSITION_E        (5)//文件夹显示
#define        POSITIONY_E       (51)

#define        POSITION_F        (22)//文件夹名显示
#define        POSITIONY_F       (51)

#define        POSITION_G        (48)//信息显示区
#define        POSITIONY_G       (40)
#define        POSITION_G1        (28)//信息显示区
#define        POSITIONY_G1       (40)

#define        POSITION_H        (3)//歌曲序号显示
#define        POSITIONY_H       (1)

#define        POSITION_BATT     (100)//电池电量显示区
#define        POSITIONY_BATT    (1)

#define        POSITION_VOL      (74)//音量显示区的初始位置
#define        POSITIONY_VOL     (143)

#define        POSITION_TVOL     (50)//音量值显示区的数字位置
#define        POSITIONY_TVOL    (1+64)

#define        POSITION_J     (68)    //歌词标志
#define        POSITIONY_J     (1+64)
#define        POSITION_EF       (68)   //总时间
#define        POSITIONY_EF       (108)

#define      POS_BG                            (0)
#define      POSY_BG                           (0)
#define      POS_BITRATE                       (14)
#define      POSY_BITRATE                      (11)
#define      POS_TYPE                          (4)
#define      POSY_TYPE                         (19)
#define      POS_EQ                            (25)
#define      POSY_EQ                           (143)
#define      POS_SONGTIME                      (4)
#define      POSY_SONGTIME                     (112)
#define      POS_GUAGEBG                    (24)
#define      POS_GUAGE                    (26)
#define      POSY_GUAGE                   (37)
#define      POS_SCROLLSTRAT                 (36)   //(30)
#define      POSY_SCROLL                     (61)  //(47)
#define      POS_SCROLLEND                   (124)
#define      POS_DIRNAME                     (41)
#define      POSY_DIRNAME                    (80)
#define      POS_FILENAME                    (30)
#define      POSY_FILENAME                   (47)
#define      POS_SONGNUM                     (85)
#define      POSY_SONGNUM                    (108)
#define      POS_SYSTIME                     (25)
#define      POSY_SYSTIME                    (118)
#define      POS_BATTERY                     (106)
#define      POSY_BATTERY                    (144)
#define      POS_PERIOD                      (7)     //AM
#define      POSY_PERIOD                     (118)
#define      POS_VOL                         (92)
#define      POSY_VOL                        (147)
#define      POS_PALYMODE                    (45)
#define      POSY_PALYMODE                   (143)
#else
#define        POSITION_A           (0)
#define        POSITION_B           (22)
#define        POSITION_C           (15)
#define        POSITION_D           (0)
#define        POSITION_E           (0)
#define        POSITION_F           (34)
#define        POSITION_BATT        (105)
#define        POSITION_G           (0)
#define        POSITION_T           (24)
#endif


//功能菜单图形位置x

#define        START_LINE         0//2
#define        MENU_GAP              16
#define        POSITION_MENU1        (START_LINE)
#define        POSITION_MENU2        (START_LINE+MENU_GAP)
#define        POSITION_MENU3        (START_LINE+MENU_GAP*2)
#define        POSITION_MENU4        (START_LINE+MENU_GAP*3)
#define        POSITION_MENU5        (START_LINE+MENU_GAP*4)
#define        POSITION_MENU6        (START_LINE+MENU_GAP*5)

#define        POSITION_MENU7        (START_LINE+MENU_GAP*6)
#define        POSITION_MENU8        (START_LINE+MENU_GAP*7)
#define        POSITION_MENU9        (START_LINE+MENU_GAP*8)
#define        POSITION_MENU10       (START_LINE+MENU_GAP*9)


//slider图形位置x
#define        SLIDER_BAR_WIDTH    64
#define        SLIDER_BG_WIDTH        4
#define        POSITION_SLIDERBAR    (60)


//progress图形位置x
#define        PROGRESS_BAR_WIDTH    96
#define        PROGRESS_BG_WIDTH    2
#define        POSITION_PROGRESSBAR    (13)

//定义菜单滚屏的宽度
#define        MENU_SCROLL_WIDTH      96

/******************************************************************
 ************************资源头的数据结构***************************
 资源结构:
 资源头        res_head
 ------------------------------------------
 资源目录项	res_head_Item1(picture Item head)
 res_head_Item2(string Item head)
 res_head_Item3(multi-string Item head)
 ------------------------------------------
 资源索引表	res_entry1(picture 1)
 ................
 res_entryN(picture N)
 res_entry1(lang 1)
 ................
 res_entryN(lang N)
 --------------------------------------------
 资源数据      picture data 1
 ................
 picture data N
 Lang ID 1 entry
 ................
 Lang ID N entry
 Lang 1 string data
 ................
 Lang N string data
 ******************************************************************/
#define   RESHEADITEM    16 		   //各个entry长度，统一为16uint8s
#define   GROUPDEFINE    3// 2//4
#define   ROW_COUNT_DEF  3// 2//4
#define   TYPE_DIR        0
#define   TYPE_FILE       1

typedef struct
{
    uint8 magic[4]; //'R', 'U', '2', 0x19
    uint16 counts; //资源的个数
} res_head_t; //6 uint8s


/*资源类型索引表的数据结构*/
typedef struct
{
    uint32 dwOffset; //资源内容索引表的偏移
    uint16 wCount; //资源类型总数
    uint8 bItemType; //'P'--PIC Table,'S'--String Table,'X' -- XML File
    uint8 type;
} res_entry_t;

/*资源内容信息索引的数据结构*/
typedef struct
{
    uint32 dwOffset; //图片数据区在文件内偏移,4 uint8s
    uint32 dwLength; //资源长度, 最大 4G，4 uint8s
    uint8 bType; //资源类型,0x01--language string ,0x02--PIC
    uint16 wWidth; //若是图片，则代表图片宽，若是字符串，则代表ID总数
    uint16 wHeight; //若是图片，则代表图片长，若是字符串，则代表该语言的ID.
} res_infor_t; //13 uint8s


/*多国语言资源ID索引表的数据结构*/
typedef struct
{
    uint32 dwOffset; // 字符ID号对应字符串编码在文件内的偏移
    uint16 dwLength; //  字符串长度.即unicode编码字符串的字节数
} res_langid_entry_t; // 6 uint8s

typedef struct
{
    uint8 filetype; //文件类型 0-- 目录  1  文件
    char name[12];
    int32 DirEntry;
} file_record_m;


typedef struct
{
    uint16 total; //总共有几个item
    uint16 active; //活动状态的编号
    uint16 menuhead; // 菜单头显示字符串          
    const uint16 *string; //字符串id 数组
    uint8 string_id; // multi string 的 string_id, 多语言显示时用
} function_menu_t;

typedef struct
{
    uint16 magic; // +0
    //uint8  autoback_flag;
    //char   File_Type;
    //char Cur_SelNUM;
    file_location_t location; //+ 9
} browser_vars_t;

typedef struct
{
    uint16 magic; 
    date_t Date;
    time_t Time;
    uint8 Stride;
    uint8 PedoUnit;
    uint16 Weight;
    uint8 ArmLen;
    uint8 PoolLen;
    uint32 TotalSec;
    uint32 TotalStep;
} Pedo_Record_t;

typedef struct
{
    uint16 Magic;
    uint16 No;
    uint8 Cate;
    date_t Date;
    time_t Time;
    uint8 Gender;
    uint8 Age;
    uint8 Height;
    uint16 Weight;
    uint8 PoolLen;
    uint32 TotalSec;
    uint32 TotalStep;
    uint32 CRC;
    uint8 Resv[34];
} pedo_save_vars_t;

typedef struct
{
    uint16 Magic;
    uint16 Version;
    uint8 Mode[32];
    uint16 Total;
    uint8 Cate;
    date_t Date;
    time_t Time;
    uint32 CRC;
    uint8 Resv[462];
} pedo_save_vars_head_t;

extern SD_FILE *res_fp;
extern BOOL ui_auto_update; //是否自动刷新屏幕
extern BOOL ui_auto_select; //是否自动确认
extern BOOL ui_run_realtime; //控件跑实时模式
extern region_t res_region;
extern res_entry_t res_entry;
extern uint32 pic_entry_offset;
extern uint32 string_entry_offset;

/*滚屏相关参数的初始化 */
extern uint8 ScrollFlag; //滚动标志位
extern uint8 ScrollFlag2; //滚动标志位
extern uint8 scroll_buf[]; //滚屏buffer区
extern region_t region_scroll; //滚屏区域
extern uint8 *str_pt;//字符串当前指针
extern uint8 *strhead_pt;//字串首址记录
extern uint8 scroll_cnt;
extern uint8 ScrollFlag;
extern uint8 scroll_buf[104];

//===========菜单滚屏
/*滚屏相关参数的初始化 */

extern uint8 ScrollFlag2; //滚动标志位
extern uint8 scroll_buf2[104]; //滚屏buffer区
extern region_t region_scroll2; //滚屏区域
extern uint8 *str_pt2;//字符串当前指针
extern uint8 *strhead_pt2;//字串首址记录
extern uint8 scroll_cnt2;

//===========

//fiona add for	special language
extern uint8 charbuf[33];
extern uint8 scrollhead_width;
extern uint8* scrollhead_pt;//滚屏字串首址记录
extern bool g_keylight_flag;//按键灯开关标志。
extern uint8 g_keylight_time;

extern bool g_light_flag; //背景灯开关标志
extern uint8 g_light_time; //关背光时间,0.5秒为单位
extern uint8 g_light_mode; //关亮度模式，0：变暗，1：变黑
extern uint8 g_lock_time;
extern uint8 g_contrast_num; //对比度
extern uint8 g_charge_counter; //检测充电状态的计数器，插上USB就开始检测第一次
extern uint16 g_sleep_time; //睡眠时间,0.5秒为单位
extern uint16 g_standby_time; //自动关机时间,0.5秒为单位
extern uint16 g_rtc_counter; //rtc 消息计数
extern uint8 usbplugflag; //USB插入检测
extern uint16 key_count; //当前按键消息发生的次数
extern uint8 key_value; //当前按键的值
extern uint16 lowPower_counter;//提示低电消息的次数
//资源操作, 禁止重入

extern uint8 file_name[12]; //存放文件短名数字
extern uint8 dir_name[30];
extern SD_FILE *res_fp;

#ifndef USB_DISK_USING
extern uint8 g_menu_active;
extern uint8 ui_auto_update; //是否自动刷新屏幕
extern uint8 ui_auto_select; //是否自动确认
extern uint8 ui_run_realtime; //控件跑实时模式
#endif

extern uint8 cur_value; //当前的电池电量值，主要用于充电显示
extern uint8 last_value;

extern int8 alarmtimerflag;
extern comval_t g_comval;
extern DLL_IMP uint8 CardDetectState;//0-无卡，非0-有卡
extern DLL_IMP uint8 bGlobal_Critical;// 1-表示当前为Nand盘，2-表示为卡盘,3-为Host盘 
extern DLL_IMP uint8 SuppCardFlag;
extern uint8 batvalue[5];
//文件浏览变量

extern uint8 dir_fselPLOrCommonDir; //playlist or common operation
extern uint8 dir_fselType; //Type of select,  Music or voice.
extern uint8 dir_fselDirLayer; //dir  layer
extern uint8 dir_fselDirNo[TOTALDIRLAYER]; //Number of current valid directory
extern uint8 dir_fselMode; //opened file selector mode
extern uint8 dir_fselDiskLetter;
extern uint8 dir_IgnoreExt4[4];
extern uint8 dir_m_szTemp[12];
extern uint16 dir_fselFileNo; //file no
extern uint16 dir_fselTotal; //Total of current select file

extern file_record_m OnePageRecord[ROW_COUNT_DEF];
extern file_record_m dir_temp_record;
//extern uint8 file_mode;
extern uint8 longname_flag;

extern uint16 file_total_num;
extern uint16 list_total_num;
extern uint16 dir_list_active;
extern uint16 dir_total_num;
extern uint16 old_list_active;
extern uint16 cur_group;
extern uint16 top;
extern uint16 bottom;
extern uint8 temp_fselDirNo;
extern uint8 DiskFlag;
extern uint8 dirbuf_longname_flag[ROW_COUNT_DEF];
extern uint8 dirbuf_filename[ROW_COUNT_DEF][50];
extern uint8 selmode;
extern uint8 selmode_changeflag;
extern uint8 need_decode;
#ifdef PC
extern DLL_IMP uint8 earphoneinflag;
#else
extern uint8 earphoneinflag;
#endif
extern uint8 InvertFlag;
//extern uint16 ShuffNum;
//extern uint8 record_num[512];
//extern uint8 first_shuff;


extern Pedo_Record_t Pedo_record;
extern bool Pedo_ui_draw;   //pedometer
extern bool Pedo_work_flag;
extern uint8 Pedo_time_cnt;
extern uint32 Pedo_total_sec;
extern uint32 Pedo_total_step;
//extern uint16 test_up;
//extern uint16 test_down;
extern uint8  Pedo_ui_num;
//extern uint8 gsensor_posflag;
//extern uint8 g_debounce;
extern bool Pedo_Swim_flag;   //0:pedometer    1:Swim
extern bool Swim_work_flag;
extern uint32 Swim_total_sec;
extern uint32 Swim_total_step;
void Pedo_RefreshMainScreen(uint8 mode);
void Pedo_RefreshDataScreen(uint8 mode);
uint16 Pedo_menu(uint8 mode);


/*Res相关操作 */
extern SD_FILE *ResOpen(const uint8 * filename);
extern void ResClose(const SD_FILE *fp_res);
extern uint16 ResShowPic(uint16 pic_id, uint8 x, uint8 y);
extern uint16 ResShowMultiString(uint16 id, uint8 x, uint8 y);
extern uint16 main_ResShowPic(uint16 id, uint8 x, uint8 y);
extern void show_waiting(void);
extern uint16 ResShowMultiString_Scroll(uint16 id, uint8 x, uint8 y, uint8 scroll_width,uint8 chance);
uint16 get_text_width(const uint8 *string);
uint16 get_large_text_width(const uint8 *string);
void Deal_ShortName(uint8 *shorName);

//=====
extern uint16 ResShowMultiString_Scroll2(uint16 id, uint8 x, uint8 y, uint8 scroll_width);
void ScrollString2( uint8 * scrollstr, bool mode ,uint8 language,region_t *scroll);
void ScrollString2_re(void);

/*控件相关 */
void ScrollString( uint8 * scrollstr, bool mode ,uint8 language,region_t *scroll);
extern void Scroll(uint8 * scrollstr, bool mode, int8 language);
extern uint16 ui_slider(slider_t *slider, void (*callback)(uint16 res_id));
extern uint8 Count_uint8(uint8 *str, uint8 language);
extern uint8 ui_show_lock(uint8 islock);
extern void ui_show_lock_1(void);

/***Message相关******/
extern uint8 check_5v(void);
extern uint8 earphone_in(void);
extern void earphone_on(bool on);

extern uint8 ap_get_message(void);
extern void ap_message_init(const comval_t *comval);
extern uint8 ap_handle_hotkey(uint8 key);
extern uint8 ap_sleep(uint16 half_sec);
extern uint16 DealPlayKey(void);
void itoa(uint16 src_int, uint8* pbuf);
/*数字转换为10进制字符*/
extern uint8 *itoa1(uint8 i, uint8 *buf);
extern uint8 *itoa2(uint8 i, uint8 *buf);
extern uint8 *itoa3(uint16 i, uint8 *buf);
extern uint8 *itoa4(uint16 i, uint8 *buf);
/*
 #define itoa_1 itoa1
 #define itoa_2 itoa2
 #define itoa_3 itoa3
 #define itoa_4 itoa4*/
#define itoa_1(i,buf){itoa1(i,buf);*((buf)+1)=0;}
#define itoa_2(i,buf){itoa2(i,buf);*((buf)+2)=0;}
#define itoa_3(i,buf){itoa3(i,buf);*((buf)+3)=0;}
#define itoa_4(i,buf){itoa4(i,buf);*((buf)+4)=0;}

void ui_show_recordTime(const uint8 *string/*, uint8 part*/);

void ui_show_recordbit(const uint8 *string);

void ui_show_recordName(const uint8 *string);
//controls, 所有关于资源的操作都对于同一个资源文件: ui30.res
void ui_show_a(uint16 id);
void ui_show_type(uint16 id);
void ui_show_c(uint16 id);
void ui_show_d(const uint8 *string);
void ui_show_d2(const uint8 *string); // for record
void ui_show_cd(const uint8 *string, uint8 part);
void ui_show_d_pic(uint16 id/*, uint8 type*/ ); //在D位置显示图形
void ui_show_loop(uint16 id);
void ui_show_f(const uint8 *string/*, uint8 part*/);
void ui_show_ef(const uint8 *string/*, uint8 part*/);
void ui_show_batt(uint8 value); //0~15, -1 表示正在充电
void ui_show_g(const uint8 *string);
void ui_show_h(const uint8 *string);

void ui_show_d_rec(const uint8 *string);
void ui_show_f_rec(const uint8 *string);
void ui_show_g_rec(const uint8 *string);

uint16 ui_delete(file_location_t *location, uint8 type);
uint16 ui_delete_all(file_location_t *location, uint8 type);
uint8 del_sel_file(uint8 file_type, uint16 selnum);
void show_del_fail(void);
uint16 ui_function_menu(function_menu_t *menu, void(*callback)(uint16 res_id));

uint16 ui_slider(slider_t *slider, void (*callback)(uint16 res_id));
uint8 ui_show_lock(uint8 islock);
void ui_show_lock_1(void);
void ui_show_vol(uint8 volume);
uint8 Count_uint8(uint8 *str, uint8 language);
//文件浏览接口函数
uint16 ui_shuff(file_location_t *g_browser_vars, uint8 type);
uint16 ui_directory(file_location_t *location, uint8 type);
void dir_ShowDirectory(uint16 top, uint16 bottom, uint16 Old_active, uint16 active, uint16 total_num);
uint8 dir_DecodeOnePage(uint16 top, uint16 bottom);
uint16 show_delete_menu(uint8 type);

uint16 show_del_play(void);

uint8 init_dir_buf1(uint16 totalnum,uint8 flag);
void dir_TransOneRecord(file_record_m *target, uint16 i);
void save_location(file_location_t *location);
uint16 get_directory_top(void);
void show_info(uint8 type);
uint16 dir_GetSelFileTotal(void);
uint8 dir_ToDirEntry(uint8 *strDir, const uint16 Num);
uint16 dir_GetFileTotalCur(void);
uint8 check_Type(uint8 * filename);
uint16 count_dir_TotalNum(void);
uint8 dir_fselSetLocation(file_location_t *location);
uint8 dir_fselGetLocation(file_location_t *location);
uint8 dir_fselGetLocationC(file_location_t *location);
uint8 dir_fselSetLocationC(file_location_t *location);
uint8 dir_fselInit(uint8 type, uint8 mode, uint8 PlayListOrCommonDir);
uint16 dir_ui_directory(file_location_t *location, uint8 type);
uint8 dir_File_Type(uint8 * filename);
void record_curlacation(void);
uint8 dir_fselRetParentDir(uint8 *strDir);
uint8 dir_fselEntCurrentDir(uint8 *strDir);
//void GetLongNameAndSave(uint8 *Namebuf, uint8 index);
uint8 dir_fselGetByNO(uint8 *strFile, const uint16 Num);
uint8 dir_fselGetNext(uint8 *strFile);

void Count_Step(uint8 mode);

#endif  /* AP_COMMON_H*/

