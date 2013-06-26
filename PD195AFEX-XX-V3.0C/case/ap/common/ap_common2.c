/*
 *******************************************************************************
 *                ACTOS AP
 *        ap common lib file, part 2
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *  $Id: ap_common2.c,v 1.1.1.1 2006/05/22 03:23:59 selina Exp $
 *******************************************************************************
 */

#include "ap_common.h"
#pragma name(COM_COM2)
extern uint8 ap_handle_hotkey_core(uint8 key);
//========== global field ===============
//#define d_Scroll_width  96-1
//extern res_entry_t res_entry;

/*
 ********************************************************************************
 * Description : 获得字符串的显示宽度
 *
 * Arguments   :
 *
 *
 * Returns     :
 *
 * Notes       : 能够识别 \r \n, 中文作为2个TCHAR 识别
 *
 ********************************************************************************
 */

uint16 get_text_width(const uint8 *string)
{
    int w = 0;

    

    while (*string != 0)
    {
        if (*string == '\r' || *string == '\n')
        {
            ;
        } //不计算回车换行
        else if (*string < 0x80)
            w += CHAR_WIDTH;
        else
            w += TCHAR_WIDTH;

        string++;
    }
    return w;
}



/*
 ********************************************************************************
 * Description : 获得字符串的显示宽度
 *
 * Arguments   :
 *
 *
 * Returns     :
 *
 * Notes       : 能够识别 \r \n, 中文作为2个Tuint8 识别
 *
 ********************************************************************************
 */

uint16 get_large_text_width(const uint8 *string)
{
    //注意: 如果 LARGE_uint8_WIDTH != TCHAR_WIDTH, 请使用注释起来的代码

    //ASSERT(LARGE_CHAR_WIDTH == TCHAR_WIDTH);
    return strlen(string) * 8;
}

/*
 ********************************************************************************
 * Description : 在指定位置显示多语言字符串
 *
 * Arguments   : str_id:字符串ID号
 x, y, 屏幕坐标,x=255 表示x方向居中
 *
 * Returns     : 成功, 1
 失败, 0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 ResShowMultiString(uint16 id, uint8 x, uint8 y)
{
    uint8 str_len;
    uint16 str_size;
    res_langid_entry_t res_langid_entry;

    /*读取字符串ID索引信息 */
    memset(scroll_buf, 0, sizeof(scroll_buf));
    str_size = sizeof(res_langid_entry_t);
    SD_FSeek(res_fp, SEEK_SET, string_entry_offset + (uint32) id * str_size); // 指针指向字符在文件中的位置
    SD_FRead(res_fp, &res_langid_entry, str_size); //res_fp->&res_entry
    SIM_ChgLongPC((BYTE *) &res_langid_entry.dwOffset);
    SIM_ChgWordPC((BYTE *) &res_langid_entry.dwLength);

    /*读取字符串内容，unicode编码 */
    //   str_len = (uint8) (res_langid_entry.dwLength < 100) ? (res_langid_entry.dwLength) : 100;
    if (res_langid_entry.dwLength < 100)
    {
        str_len = (uint8) res_langid_entry.dwLength;
    }
    else
    {
        str_len = 100;
    }
    SD_FSeek(res_fp, SEEK_SET, res_langid_entry.dwOffset);
    SD_FRead(res_fp, scroll_buf, str_len);

    /*是否居中显示字符串 */
    if (x == 255)
    {
       // SetTextPos(((uint8) SCR_WIDTH - GetStringLength(scroll_buf, (uint16) str_len / 2, 1)) / 2, y);
        region_scroll.x = ((uint8) SCR_WIDTH - GetStringLength(scroll_buf, (uint16) str_len / 2, 1))/2;
    }
    else
    {
       // SetTextPos(x, y);
        region_scroll.x = x;
    }

    /*超出50个字节,设置结束符 */
    if (str_len == 100)
    {
        scroll_buf[100] = 0x00;
        scroll_buf[101] = 0x00;
    }
#if 1
    	region_scroll.width = SCR_WIDTH-x;//解决最后一个字缺画的问题 ，需检查是否引起其他BUG
    	region_scroll.height = SCROLL_REGION_HEIGHT;
   		//region_scroll.x = x;
    	region_scroll.y = y;
    	scroll_buf[str_len - 2] = ' ';
    	scroll_buf[str_len - 1] = 0x00;
    	scroll_buf[str_len] = ' ';
    	scroll_buf[str_len + 1] = 0x00;
    	scroll_buf[str_len + 2] = 0x00;
    	scroll_buf[str_len + 3] = 0x00;    
		ScrollFlag = FALSE;
    	ScrollString(scroll_buf, TRUE,UNICODELANGUAGE , &region_scroll);// ARABICHASJOIN
    
	
#else
        PutUnS(scroll_buf,  -1); 
        if(ui_auto_update) UpdateScreen(NULL);
#endif
  
    return 1;
}

/*
 ********************************************************************************
 * Description : 在指定位置滚动显示多语言字符串
 *
 * Arguments   : id, 资源代号
 x, y, 屏幕坐标,x=255 表示x方向居中
 *       scroll_width, 滚屏区域的宽度
 * Returns     : 成功, 1
 失败, 0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 ResShowMultiString_Scroll(uint16 str_id, uint8 x, uint8 y, uint8 scroll_width,uint8 chance)
{
#if 1
    uint8 str_len;
    uint8 strWidth;
    uint16 str_size;
    res_langid_entry_t res_langid_entry;
    /*读取字符串ID索引信息 */
	strWidth=scroll_width;//去除警告
    memset(scroll_buf, 0, sizeof(scroll_buf));
    str_size = sizeof(res_langid_entry_t);
    SD_FSeek(res_fp, SEEK_SET, string_entry_offset + (uint32) (str_id * str_size)); // 指针指向字符在文件中的位置
    SD_FRead(res_fp, &res_langid_entry, str_size); //res_fp->&res_entry
    SIM_ChgLongPC((BYTE *) &res_langid_entry.dwOffset);
    SIM_ChgWordPC((BYTE *) &res_langid_entry.dwLength);

    /*读取字符串内容，unicode编码 */
    //    str_len = (uint8) (res_langid_entry.dwLength < 100) ? (res_langid_entry.dwLength) : (100);
    if (res_langid_entry.dwLength < 100)
    {
        str_len = (uint8) res_langid_entry.dwLength;
    }
    else
    {
        str_len = 100;
    }
    SD_FSeek(res_fp, SEEK_SET, res_langid_entry.dwOffset);
    SD_FRead(res_fp, scroll_buf, str_len);

    /*是否居中显示字符串 */
    strWidth = GetStringLength(scroll_buf, (uint16) str_len / 2, 1);
    if (strWidth >= scroll_width)     //95
    {
        if (x == 255)
        {
            x = 0;
        }
        /*init scroll parameters*/
        region_scroll.width = scroll_width;//95;
        region_scroll.height = SCROLL_REGION_HEIGHT;
        region_scroll.x = x;
        region_scroll.y = y;
        scroll_buf[str_len - 2] = ' ';
        scroll_buf[str_len - 1] = 0x00;
        scroll_buf[str_len] = ' ';
        scroll_buf[str_len + 1] = 0x00;
        scroll_buf[str_len + 2] = 0x00;
        scroll_buf[str_len + 3] = 0x00;
        ScrollFlag = TRUE;
        ScrollString(scroll_buf, TRUE,UNICODELANGUAGE , &region_scroll);// ARABICHASJOIN
    }
    else
    {
        ScrollFlag = FALSE;
        if (x == 255)
        {
            x = ((uint8) SCR_WIDTH - strWidth)/2;
        }
        if(chance ==1)
        	SetTextPos(x, y);
        else 	
        	SetTextPos((SCR_WIDTH - strWidth )  / 2, y);
        PutUnS(scroll_buf, -1);//ARABICHASJOIN
    }
#endif
    return 1;
    
}

void ui_show_CardOut(void)
{
#if 0    
    DC oldDC_sav;
    GetDCStatus(&oldDC_sav);

    SetPenColor(Color_WHITE_def);
    SetBackgdColor(Color_BULE_def);
    ClearScreen(NULL);
    SetTextOutMode(1);
    ResShowMultiString(CARDPULL, 255, 80);
    ap_sleep(4);
    SetPenColor(oldDC_sav.PenColor);
    SetBackgdColor(oldDC_sav.BackgdColor);
#endif    
}
/*
 ********************************************************************************
 * Description : 初始化消息处理
 *
 * Arguments   : comval, 系统设定值, =null 表示只初始化内部计数器
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void ap_message_init(const comval_t *comval)
{

    if (comval != NULL)
    {
        g_light_time = comval->LightTime * 2;
        g_lock_time =  comval->LockTime * 2;
        g_light_mode = comval->Lightmode;
        g_sleep_time = comval->SleepTime * 60 * 2;

        g_standby_time = comval->StandbyTime * 2 * 60;
        g_contrast_num = comval->DisplayContrast;
        //        key_g_light_time = g_comval.keybacklight*2;
/*#ifdef EAR_PROTECT
        g_EarProtect_flag = comval->EarProtect_flag;
        EarProtectThreshold = comval->EarProtectThreshold;
        g_autoswitch_time = comval->auto_switchoff_time * 60 * 2;
#endif*/
    }

    g_rtc_counter = 0;
    //    g_light_flag = TRUE;
}
/*
 ********************************************************************************
 * Description : 睡眠指定的时间
 *
 * Arguments   : half_sec, 时间, 0.5秒为单位
 *
 * Returns     : 如果指定的时间内没有其他消息产生, 返回0
 如果指定的时间内有其他消息产生, 返回该消息
 * Notes       : 如果指定的时间内有其他消息产生, 睡眠的时间将< 指定的时间
 *
 ********************************************************************************
 */

uint8 ap_sleep(uint16 half_sec)
{
    uint8 n = 0; //rtc counter
    uint8 key, result;

    while (1)
    {
        key = ap_get_message();
        //sleep 时不管timeout
        if ((key == AP_MSG_RTC) || (key == AP_MSG_CHARGING) || (key == AP_MSG_WAIT_TIMEOUT))
        {
            n++;
            if (n >= (uint8) half_sec)
            {
                result = 0;
                break;
            }
        }//lock 只有up, 没有long/hold
        else if ((key == AP_KEY_NULL) || (key == AP_MSG_LOCK) || (key == (AP_MSG_LOCK | AP_KEY_UP)))
        {
            continue;
        }
        else
        {
            result = ap_handle_hotkey_core(key);
            break;
        }
    }
    return result;
}

uint8 ui_show_lock(uint8 islock)
{
    ClearScreen(NULL);
    if(islock == 1)
        ResShowPic(ICON_KEY_HOLD,0,0);
    else
        ResShowPic(ICON_LIST_GROUND,0,0);
	UpdateScreen(NULL);
    
    //wait 2s
    return ap_sleep(4);
}

extern bool g_lockflag;
void ui_show_lock_1(void)
{
    if(g_lockflag)
        ResShowPic(ICON_KEY_LOCK,88,0);
    else
        ResShowPic(IM_LOCK,88,0);
    //UpdateScreen(NULL);
}
