/*
 *******************************************************************************
 *                ACTOS AP
 *        ap common lib file, udisk use this file too
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *  $Id: ap_common.c,v 1.1.1.1 2006/05/22 03:23:59 selina Exp $
 *******************************************************************************
 */

#include "ap_common.h"

//========== global field ===============
#pragma name(COM_COM)
uint8 g_keylight_flag = TRUE;//按键灯开关标志。//maya090412#1 for keylight
uint8 g_keylight_time = 0;//关按键背光时间，0.5秒为单位

uint8 g_light_flag = TRUE;//背景灯开关标志
uint8 g_light_time = 0; //关背光时间,0.5秒为单位
uint8 g_light_mode = 0; //关亮度模式，0：变暗，1：变黑
uint8 g_lock_time = 0; //Auto lock时间,0.5秒为单位
uint8 g_contrast_num = 0; //对比度
//uint8 usbplugflag = 0;    //USB插入检测
uint8 g_charge_counter = 0; //检测充电状态的计数器，插上USB就开始检测第一次
uint16 g_sleep_time = 0; //睡眠时间,0.5秒为单位
uint16 g_standby_time = 0; //自动关机时间,0.5秒为单位
uint16 g_rtc_counter = 0; //rtc 消息计数
uint16 key_count = 0; //当前按键消息发生的次数
uint8 key_value = Msg_KeyNull; //当前按键的值
uint16 lowPower_counter = 0;
/*#ifdef EAR_PROTECT
uint8 g_decrease_time = 0; //声音递减计时时间
uint8 g_EarProtect_flag; //护耳模式是否开启的标志
uint8 EarProtectThreshold;
uint16 g_half_hour_counter = 0;
uint16 g_autoswitch_counter = 0;
uint16 g_autoswitch_time;
#endif*/

uint8 file_name[12]; //存放文件短名数字
//uint8 dir_name[30];
uint32 pic_entry_offset;
uint32 string_entry_offset;
SD_FILE *res_fp;
res_entry_t res_entry;
region_t res_region;

#ifndef USB_DISK_USING
//滚屏相关参数
uint8 *str_pt;//字符串当前指针
uint8 *strhead_pt;//字串首址记录
uint8 scroll_cnt;
uint8 ScrollFlag;
uint8 ScrollFlag2;//菜单滚屏参数
uint8 scroll_buf[104];
region_t region_scroll; //滚屏
//fiona add for special language
//uint8 charbuf[33];
uint8 scrollhead_width;
uint8* scrollhead_pt;//滚屏字串首址记录

uint8 g_menu_active = FALSE;

//请不要把这个变量定义在非IDATA0 段,否则不能初始化
uint8 ui_auto_update = TRUE; //是否自动刷新屏幕
uint8 ui_auto_select = TRUE; //是否自动确认
uint8 ui_run_realtime = FALSE; //控件跑实时模式
#endif
uint8 batvalue[5] =
{ 0x5b, 0x5f, 0x65, 0x6b, 0x75 };//{ 0x58, 0x5d, 0x5f, 0x65, 0x76 };
uint8 cur_value = 0; //当前的电池电量值，主要用于充电显示
uint8 last_value = 10;
bool blnInPlayLong = FALSE; //在play long处理中
bool g_lockflag = FALSE;
bool g_lockflag2 = FALSE;
//uint8 InvertFlag;

Pedo_Record_t Pedo_record;

bool Pedo_ui_draw = FALSE;   //pedometer
bool Pedo_work_flag = FALSE;
uint8 Pedo_time_cnt = 0;
uint32 Pedo_total_sec = 0;
uint32 Pedo_total_step = 0;
//uint16 test_up = 0;
//uint16 test_down = 0;
uint8  Pedo_ui_num = 0;
//uint8 gsensor_posflag = 0;	//20110525	0:旧状态为大值	1:旧状态为小值
//uint8 g_debounce = 0;

bool Pedo_Swim_flag = FALSE;   //0:pedometer    1:Swim
bool Swim_work_flag = FALSE;
uint32 Swim_total_sec = 0;
uint32 Swim_total_step = 0;

/*
 ********************************************************************************
 * Description : 打开资源文件
 *
 * Arguments   : filename, 资源文件名字
 *
 * Returns     : 成功,文件指针
 失败, NULL
 *
 * Notes       :
 *
 ********************************************************************************
 */
SD_FILE *ResOpen(const uint8* filename)
{
    res_head_t res_head;
    //open res file
    res_fp = SD_FOpen(filename, 0);
    if (res_fp == NULL)
    {
        return NULL;
    }
    SD_FRead(res_fp, &res_head, sizeof(res_head_t));
    SIM_ChgWordPC(&res_head.counts);
    /*判断文件格式是否合法 */
    if ((res_head.magic[0] != 'R') || (res_head.magic[1] != 'U') || (res_head.magic[2] != '2') || (res_head.magic[3]
            != 0x19))
    {
        SD_FClose(res_fp);
        return NULL;
    }

    /*读取图片 entry 的offset*/
    SD_FSeek(res_fp, SEEK_SET, RESHEADITEM);
    SD_FRead(res_fp, &pic_entry_offset, sizeof(pic_entry_offset));
    SIM_ChgLongPC(&pic_entry_offset);

    /*读取多国语言索引的entry地址 */
    SD_FSeek(res_fp, SEEK_SET, RESHEADITEM * 2);
    SD_FRead(res_fp, &string_entry_offset, sizeof(string_entry_offset));
    SIM_ChgLongPC(&string_entry_offset);

    /*读取当前语言ID索引的entry地址 */
    //VMRead(&g_comval, VM_SYSTEM, sizeof(g_comval));
    SD_FSeek(res_fp, SEEK_SET, string_entry_offset + (unsigned long) g_comval.langid * RESHEADITEM);
    SD_FRead(res_fp, &string_entry_offset, sizeof(string_entry_offset));
    SIM_ChgLongPC(&string_entry_offset);
    //return ok
    return res_fp;
}

/*
 ********************************************************************************
 * Description : 关闭资源文件
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void ResClose(const SD_FILE *fp)
{
    SD_FClose(fp);
}

/*
 ********************************************************************************
 * Description : 在指定位置显示图形
 *
 * Arguments   : id, 资源代号
 x,y, 屏幕坐标
 *
 * Returns     : 成功, 1
 失败, 0
 *
 * Notes       :	更新“目录索引项”结构体名称，请查看ap_common.h文件
 *
 ********************************************************************************
 */
uint16 ResShowPic(uint16 pic_id, uint8 x, uint8 y)
{
    res_infor_t res_infor;
    //背光关着时，不刷图片
   /* if (!g_light_flag)
    {
        return 0;
    }
    */
    SD_FSeek(res_fp, SEEK_SET, pic_entry_offset + (uint32) (pic_id * RESHEADITEM)); //找到第N张picture的索引目录
    SD_FRead(res_fp, &res_infor, sizeof(res_infor_t));
    SIM_ChgLongPC(&res_infor.dwOffset);
    SIM_ChgLongPC(&res_infor.dwLength);
    SIM_ChgWordPC(&res_infor.wWidth);
    SIM_ChgWordPC(&res_infor.wHeight);

    /*判断res类型的合法性 */
    if (res_infor.bType != 0x02)
    {
        return 0;
    }

    /*设窗参数设置 */
    res_region.x = x;
    res_region.y = y;
    res_region.width = (uint8) res_infor.wWidth;
    res_region.height = (uint8) res_infor.wHeight;

    /*读取图片数据并刷屏 */
    PutSDImage(&res_region, res_fp->startaddr + res_infor.dwOffset);
    if(ui_auto_update) UpdateScreen(&res_region);
    return 1;
}
//对短名的文件显示进行处理,如果当前的名字小于八个字节
//则把字符后面的空格去掉,然后在字符后面加上"."再把后缀加上
void Deal_ShortName(uint8 *shorName)
{
    uint8 i = 7;
    uint8 temp_name[12];
    memset(temp_name, 0x0, 12);
    while (1)
    {
        if (shorName[i] == 0x20) //计算空格数
        {
            i--;
        }
        else
        {
            break;
        }
    }
    memcpy(temp_name, shorName, i + 1);
    temp_name[i + 1] = 0x2e;
    memcpy(&temp_name[i + 2], &shorName[8], 4);
    memcpy(shorName, temp_name, 12);
}


#ifdef InvertFlag
uint16 ResInvertShowPic(uint16 pic_id, uint8 x, uint8 y)
{
    return 1;
}
#endif

