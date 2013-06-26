/*
*******************************************************************************
*                ACTOS AP
*        system setting ap of AS211A
*
*        (c) Copyright, Actions Co,Ld.
*             All Right Reserved
*
*  $Id: main.c,v 1.2 2010/03/23 10:48:01 Mars Exp $
*******************************************************************************
*/
#include "actos.h"
#include "ap_common.h"
#include "ap_setting.h"

typedef uint16 (*show_func)(void);

#pragma name(AP_MAIN)
uint16 UpDisplayFlag = 0; //对应于更新相关选项
comval_t g_comval;
vm_cfg_t  g_modifyval;  
setting_vars_t g_setting_vars;
slider_t slider;
progress_t progress;
uint8 charge_flag; 
function_menu_t menu;

uint8 PMflag=0;
date_t date ;
time_t time ;

extern unsigned long systemtime;
extern unsigned long gwtime;

const region_t clock_region =
{ 34, 40, 60, 16 };

//days of month
const uint8 month_days[] =
{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

//usb模式 时间设置 省电设置 语言设置 显示设置 录音设置 系统工具 发射 共8个主菜单
//#if (PETOMETER_USE)
#define      MAX_SYSMENUNUM_PEDO   7    //pedometer
const uint16 main_text_pedo[MAX_SYSMENUNUM_PEDO]= 
{
    TIMESET,POWER,LANGUAGE,DISPLAY,AUTOLOCK,/*RECORDSETTING,*/SYSTEMTOOLS,PERSONAL
};
//#else
#define      MAX_SYSMENUNUM   5 
const uint16 main_text[MAX_SYSMENUNUM]= 
{
    POWER,LANGUAGE,DISPLAY,AUTOLOCK,/*RECORDSETTING,*/SYSTEMTOOLS
};
//#endif
const show_func entries[MAX_SYSMENUNUM_PEDO] =
{  
  show_clock,show_powerdown,show_language,show_display,show_lock,/*show_record,*/system_tool,show_gusensou
};

const uint16 record_text[1]= 
{
  SETRQUA/*,SETRVAD,SETRAGC*/  
}; 
const uint16 record_qua[3]= 
{
  SETRLTIM ,SETRNOMA ,SETRHQUA 
}; 
const char FwcfgName[] = "FWCFG.BIN"; 
/*
********************************************************************************
* Description : 读入系统设定变量
*
* Arguments   :
*
* Returns     :
*
* Notes       :
*
********************************************************************************
*/
void read_var(void)
{
    uint16 result;
    result = VMRead(&g_comval, VM_SYSTEM, sizeof(g_comval));
    ap_message_init(&g_comval);    
    result = VMRead(&g_modifyval, VM_AP_FWCFG,sizeof(vm_cfg_t)); 
    if(g_comval.PedoFlag)   //pedometer
    {
        //if(Pedo_Swim_flag)
        {
            sKY_BankGsensorGetStep(&Swim_total_step,&Swim_total_sec,1);
            Swim_work_flag = sKY_BankGsensorGetADC(6,1);
        }
        //else
        {
            sKY_BankGsensorGetStep(&Pedo_total_step,&Pedo_total_sec,0);
            Pedo_work_flag = sKY_BankGsensorGetADC(6,0);
        }
        Pedo_Swim_flag = Swim_work_flag;
    }
}

int16 main(int16 param)
{  
    int16 result;
    int16 menu_active=0;    //main menu's active number,
    BOOL mc_result;

	param = param;
    mc_result= DRV_ActiveUD(0,MODE_SLOW);
    /*先读VM，在打开资源文件，保证语言的索引表正确*/
    read_var();
    //ClearScreen(NULL);
    res_fp = ResOpen("ui30.res");
    while(1)
    {
        if(g_comval.PedoFlag)
        {
            menu.total = MAX_SYSMENUNUM_PEDO;             //"+2":for MP3 recorder by ccm
            menu.string = main_text_pedo;
        }
        else
        {
            menu.total = MAX_SYSMENUNUM;             //"+2":for MP3 recorder by ccm
            menu.string = main_text;
        }
        
        menu.menuhead = MAIN5;
        menu.active = menu_active;
        menu.string_id = g_comval.langid;
        //ui_auto_select = TRUE; 
        result = ui_function_menu(&menu, NULL);
        //ui_auto_select = FALSE;
        if(result == RESULT_IGNORE)
        {     
            result = 0;
            break;
        }
        /*check hotkey*/
        else if(result != 0)
        {    
             break;
        }
        else if(menu.active == menu.total - 1)
		{
		     ;//break;    //user choose exit.
        }
         
        menu_active = menu.active;
        result = entries[menu.active](); 
        if(result == RESULT_IGNORE)
        {
            result = 0;
        }     
        if(result != 0) 
        {
            break;
        }    
    }//while(1)
    ResClose(res_fp);
    mc_result = VMWrite(&g_comval, VM_SYSTEM);
    return result;
}
uint16 show_clock(void)
{
    uint8 bSettime = 0;
    uint8 bTimeAdd = 0xFF;
    date_t date;
    time_t time;
    uint8 buf[11]; //date &  time
    uint8 key;
    uint8 set_what = 0; //0, 非设定状态, 1,year, 2,month...
    BOOL show_what = TRUE; //true, 显示, false, 不显示
    uint8 days; //本月的天数
    BOOL result;
    BOOL need_draw = TRUE;
    BOOL need_draw_backgroud = TRUE;
    BOOL set_ok = FALSE; //设置完成
    BOOL adjusting = FALSE; //正在设置,不要闪烁
    uint8 PMflag=0;
    kval_t kval;
    
    result = TM_GetDate(&date, 0);
    result = TM_GetTime(&time);	
    
	bSettime=0;
    bTimeAdd=0xFF;
    
    while (1)
    {
        //显示背景
        if (need_draw_backgroud)
        {
            ClearScreen(NULL);
            ui_auto_update = FALSE;
            ui_auto_update = TRUE;
            need_draw_backgroud = FALSE;
        }

        //显示时钟
        if (need_draw)
        {
            ClearScreen(&clock_region);
            if (bTimeAdd == 0xff)
            {
                result = TM_GetDate(&date, 0);
                result = TM_GetTime(&time);
                //系统时间没作2098限制，ap层限制
                if (date.year >= 2099) //2000~2098, 不定为2099是为了方便处理从2098跳2099能回到00
                {
                    date.year = 2000;
                    TM_SetDate(&date, 0);
                }
            }
           
            if ((set_what == 1)/* && (!show_what)*/)
            {
                //buf[0] = ' ';
                //buf[1] = ' ';
                //buf[2] = ' ';
                //buf[3] = ' ';
                itoa4(date.year, buf);
            }
            else
            {
                itoa4(date.year, buf);
            }
            buf[4] = '/';
            if ((set_what == 2)/* && (!show_what)*/)
            {
                //buf[5] = ' ';
                //buf[6] = ' ';
                itoa2(date.month, buf + 5);
            }
            else
            {
                itoa2(date.month, buf + 5);
            }
            buf[7] = '/';
            if ((set_what == 3)/* && (!show_what)*/)
            {
                //buf[8] = ' ';
                //buf[9] = ' ';
                //buf[10] = 0;
                itoa2(date.day, buf + 8);
            }
            else
            {
                itoa2(date.day, buf + 8);
			}
			
            SetTextPos(24, 16);
            PutS(buf, 10);
            
            //show 19:17:50
            if ((set_what == 4)/* && (!show_what)*/)
            {
                //buf[0] = ' ';
                //buf[1] = ' ';
                itoa2(time.hour, buf);
            }
            else
            {
                itoa2(time.hour, buf);
            }
            buf[2] = ':';
            if ((set_what == 5)/* && (!show_what)*/)
            {
                //buf[3] = ' ';
                //buf[4] = ' ';
                itoa2(time.minute, buf + 3);
            }
            else
            {
                itoa2(time.minute, buf + 3);
            }
            buf[5] = ':';
            if ((set_what == 6)/* && (!show_what)*/)
            {
                //buf[6] = ' ';
                //buf[7] = ' ';
                //buf[8] = 0;
                itoa2(time.second, buf + 6);
            }
            else
            {
                itoa2(time.second, buf + 6);
            }
            SetTextPos(32, 32);
            PutS(buf, 8);
            UpdateScreen(NULL);

            need_draw = FALSE;
        }

        //handle msg
        key = ap_get_message();
        switch (key)
        {

            
            case AP_MSG_CHARGING:
            case AP_MSG_RTC:
            if(adjusting) show_what = TRUE;
            else show_what = !show_what;
            
            need_draw = TRUE;
            break;
            
            case AP_KEY_MODE | AP_KEY_UP:
            //case AP_KEY_VOLADD | AP_KEY_UP:
            if (set_what > 6)
                set_what = 1;
            else
                set_what++;
            bTimeAdd = 0;

            need_draw = TRUE;
            /*
            case AP_KEY_MODE | AP_KEY_LONG:
            case AP_KEY_NEXT | AP_KEY_LONG:
            case AP_KEY_PREV | AP_KEY_LONG:
            set_what = 0;
            bTimeAdd = 0;
            need_draw = TRUE;
            break;
            */
            /*
            case AP_KEY_PREV | AP_KEY_LONG:
            //case AP_KEY_VOLSUB | AP_KEY_UP:
            if (set_what < 1)
                set_what = 8;
            else
                set_what--;            
            bTimeAdd = 0;
            
            need_draw = TRUE;
            
            break;
            */
            //case AP_KEY_NEXT: //in setting clock
            case AP_KEY_NEXT | AP_KEY_UP:
            adjusting = TRUE;
            bSettime = 0xff;
            switch (set_what)
            {
                case 1: //year
                if (date.year < 2098)
                    date.year++;
                else
                    date.year = 2000;
                break;
                case 2: //month
                if (date.month < 12)
                    date.month++;
                else
                    date.month = 1;
                break;
                case 3: //day
                days = month_days[date.month - 1]; //本月天数
                if ((date.year % 4 == 0) && (date.month == 2))
                    days++; //闰年+1
                    
                if (date.day < days)
                    date.day++;
                else
                    date.day = 1;
                break;
                
                case 4: //hour
                if (time.hour < 23)
                    time.hour++;
                else
                    time.hour = 0;
                break;
                
                case 5: //min
                if (time.minute < 59)
                    time.minute++;
                else
                    time.minute = 0;
                break;
                
                case 6: //sec
                if (time.second < 59)
                    time.second++;
                else
                    time.second = 0;
                break;
                
                default: //no in setting
                bSettime = 0;
                break;
            } //set_what

            //clear the key buffer
            do
            {
                key = ap_get_message();
                if (key == (AP_KEY_NEXT | AP_KEY_UP))
                {
                    adjusting = FALSE;
                    break;
                }
            } while (key != 0);
            need_draw = TRUE;
            break;

            //case AP_KEY_PREV: //in setting clock
            case AP_KEY_PREV | AP_KEY_UP:
            adjusting = TRUE;
            bSettime = 0xff;
            switch (set_what)
            {
                case 1: //year
                if (date.year > 2000)
                    date.year--;
                else
                    date.year = 2098;
                break;
                case 2: //month
                if (date.month > 1)
                    date.month--;
                else
                    date.month = 12;
                break;
                case 3: //day
                days = month_days[date.month - 1]; //本月天数
                if ((date.year % 4 == 0) && (date.month == 2))
                    days++; //闰年+1
                if (date.day > 1)
                    date.day--;
                else
                    date.day = days;
                break;
                case 4: //hour
                if (time.hour > 0)
                    time.hour--;
                else
                    time.hour = 23;
                break;
                case 5: //min
                if (time.minute > 0)
                    time.minute--;
                else
                    time.minute = 59;
                break;
                case 6: //sec
                if (time.second > 0)
                    time.second--;
                else
                    time.second = 59;
                break;
                default: //no in setting
                bSettime = 0;
                break;
            } //set_what

            //clear the key buffer
            do
            {
                key = ap_get_message();
                if (key == (AP_KEY_PREV | AP_KEY_UP))
                {
                    adjusting = FALSE;
                    break;
                }
            } while (key != 0);

            need_draw = TRUE;

            break;

            //case AP_KEY_NEXT | AP_KEY_UP:
            //case AP_KEY_PREV | AP_KEY_UP:
            //adjusting = FALSE;

            break;
            case AP_MSG_WAIT_TIMEOUT: //子菜单界面在8秒后返回系统功能菜单, add by ccm 2004/07/15
            //case AP_KEY_MODE | AP_KEY_UP:
            bSettime = 0;
            
            case AP_KEY_PLAY | AP_KEY_UP:
            //return 0;        //退出

            key = 0;
            set_ok = TRUE;

            break;
            default:
            key = ap_handle_hotkey(key);
            if (key == RESULT_REDRAW)
            {
                need_draw_backgroud = TRUE;
                need_draw = TRUE;
            }
            else if (key != 0)
            {
                set_ok = TRUE;
            }
        } //switch(key)

        //set ok?
        if (set_ok)
            break;
    }//while(1)
    if (bSettime == 0)
    {
        return key;
    }
    else if (key != 0) //不是按mode退出的都不保存设置，如USB线插上等
    {
        return key;
    }

    SetAsciiFont(FONT_TYPE_DEFAULT);

    //re-check the date/time format
    days = month_days[date.month - 1];
    if ((date.year % 4 == 0) && (date.month == 2))
        days++; //润年
    if (date.day > days)
        date.day = days;

    //save date & time
    result = TM_SetDate(&date, 0);
    //DUMP("set date", 0);
//    ASSERT(result);
    result = TM_SetTime(&time);
    //DUMP("set time", 0);
//    ASSERT(result);

    //将systemtime 和 gwtime写回到VRAM中
    VMRead(&kval, VM_KERNEL, sizeof(kval_t));
    kval.systemtime = systemtime; //全局变量
    kval.gwtime = gwtime; //全局变量
    VMWrite(&kval, VM_KERNEL);

    return key;
}

