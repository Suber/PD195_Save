/*
*******************************************************************************
*                ACTOS AP
*        system setting ap of sdk 3.0, ui part
*
*        (c) Copyright, Actions Co,Ld.
*             All Right Reserved
*
*  $Id: ui_1.c,v 1.3 2006/07/14 06:48:01 tangwh Exp $
*******************************************************************************
*/
#include "actos.h"
#include "enhanced.h"
#include "ap_common.h"
#include "ap_setting.h" 
#include "sysdef.h"

extern vm_cfg_t  g_modifyval;   
extern uint8 earphoneinflag;
#define HIGH4(x) ((x) >> 4)
#define LOW4(x) ((x) & 0x0f)

#pragma name(AP_UI_1)
const uint8 mychar[]="0123456789";
const uint16 systool_text[]={STSYDELE,STFORMAT,STRRESET,SYSINF};
const uint16 Fmt_sle[]={SLECTNO,SLECTYES};
const uint16 online_text[4] =
{ 
    SONLINE1, SONLINE2, SONLINE3, SONLINE4 
};
const char CaseDate[11] =
{ "2011/01/10" };
const char CaseNum[15] =
{ "ATJ3315_AS331B" };

#if 0//EARPROTECT
const uint16 earprotect_mtext[2] =
{ EARPROCT, EARPARAM };//,EARATTEN,EARTIME};
const uint16 earprotect_onoff_text[2] =
{ EAROFF, EARON };
const uint16 earprotect_level_text[3] =
{ EARHIGH, EARMID, EARLOW };
const uint16 earprotect_mode_text[2] =
{ ATTENON, ATTENOFF };
#if 0
uint16 ear_protect_onoff(void)
{
    uint16 result;

    menu.total = 2;
    menu.active = g_comval.EarProtect_flag;
    //menu.pic_inactive = earprotect_onoff;
    //menu.pic_active = earprotect_onoff_act;
    menu.menuhead = MAIN5;
    menu.string = earprotect_onoff_text;
    menu.string_id = g_comval.langid;
    //    menu.headid=EARPROCT;
    ui_auto_select = FALSE;
    result = ui_function_menu(&menu, NULL);
    ui_auto_select = TRUE;
    if (result == RESULT_IGNORE)
        return 0;
    if (result == RESULT_RECORD)
        return result;

    g_comval.EarProtect_flag = menu.active;

    return result;
}

uint16 ear_protect_param_adjust(void)
{
    uint16 result;
    uint8 key;
    uint8 mode = 0; //0, power mode, 1, sleep mode
    bool need_draw = TRUE;

    switch (g_comval.EarProtectThreshold)
    {
        case 22:
        mode = 0;
        break;
        case 25:
        mode = 1;
        break;
        default:
        mode = 2;
        break;
    }

    menu.total = 3;
    menu.active = mode;
    //menu.pic_inactive = earprotect_level;
    //menu.pic_active = earprotect_level_act;
    menu.menuhead = MAIN5;
    menu.string = earprotect_level_text;
    menu.string_id = g_comval.langid;
    //    menu.headid=EARPARAM;
    ui_auto_select = FALSE;
    result = ui_function_menu(&menu, NULL);
    ui_auto_select = TRUE;
    if (result == RESULT_IGNORE)
        return 0;
    else if (result != 0)
        return result;
    else
    {
        mode = menu.active;
        if (mode == 0)
        {
            g_comval.EarProtectThreshold = 22;
        }
        else if (mode == 1)
        {
            g_comval.EarProtectThreshold = 25;
        }
        else
        {
            g_comval.EarProtectThreshold = 28;
        }
        //重新设置关机时间
        ap_message_init(&g_comval);
        return 0;
    }
}

uint16 ear_protect_mode(void)
{
  
    uint16 result;

    menu.total = 2;
    menu.active = g_comval.attenuation_flag;
    //menu.pic_inactive = earprotect_sub;
    //menu.pic_active = earprotect_sub_act;
    menu.menuhead = MAIN5;
    menu.string = earprotect_mode_text;
    menu.string_id = g_comval.langid;
    //    menu.headid=EARATTEN;
    ui_auto_select = FALSE;
    result = ui_function_menu(&menu, NULL);
    ui_auto_select = TRUE;
    if (result == RESULT_IGNORE)
        return 0;
    if (result == RESULT_RECORD)
        return result;

    g_comval.attenuation_flag = menu.active;

    return result;

	return 0;
}

uint16 ear_protect(void)
{

    uint16 result;
    uint8 mode = 0;

    menu.total = 2;
    menu.active = g_comval.EarProtect_flag;
    //menu.pic_inactive = earprotect_main;
    //menu.pic_active = earprotect_main_act;
    menu.menuhead = MAIN5;
    menu.string = earprotect_mtext;
    menu.string_id = g_comval.langid;
    //    menu.headid=EARPROCT;
    ui_auto_select = FALSE;
    result = ui_function_menu(&menu, NULL);
    ui_auto_select = TRUE;
    if (result == RESULT_IGNORE)
        return 0;
    else if (result != 0)
        return result;
    else
    {
        mode = menu.active;
        if (mode == 0)
        {
            result = ear_protect_onoff();
        }
        else if (mode == 1)
        {
            result = ear_protect_param_adjust();
        }
        else if (mode == 2)
        {
            result = ear_protect_mode();
        }
        else
        {
            ClearScreen(NULL);
            UpdateScreen(NULL);
            slider.min = 0;
            slider.max = 240;
            slider.step = 10;
            slider.value = g_comval.auto_switchoff_time;
            result = ui_slider(&slider, NULL, 1);
            if (result != 0)
                return result;
            else if (result == RESULT_IGNORE)
                return 0;
            g_comval.auto_switchoff_time = slider.value;
        }
        //重新设置关机时间
        ap_message_init(&g_comval);
        if (result == RESULT_IGNORE)
            return 0;
        if (result != 0)
            return result;
    }

    return result;
	#endif
	return 0;
}
#endif
/*
 ********************************************************************************
 * Description : Key tone setting
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
#if 0 
uint16 show_keytone(void)
{
    ClearScreen(NULL);
    UpdateScreen(NULL);
    slider.min = 0;
    slider.max = 40;  //CHIP_VOLUME_MAX;
    slider.step = 1;
    slider.value = g_comval.KeyTone;
    return ui_slider(&slider, keytone_callback);//, 1);    //masda20110526

}
#endif
/*
 ********************************************************************************
 * Description : 把actos的版本格式转成字符串格式
 *
 * Arguments   : fwversion, actos格式的版本信息
 buf, 输出字符串缓冲区
 *
 * Returns     :
 *
 * Notes       :
 0x31,0x23,0x45,0x67  => 3.1.23.4567
 *
 ********************************************************************************
 */
void fmt_version(const fwversion_t *fwversion, char *buf)
{
    uint8 *p;
    uint8 ch1, ch2, ch3, ch4;

    ch1 = fwversion->version & 0xff;
    ch2 = (fwversion->version >> 8) & 0xff;
    ch3 = (fwversion->version >> 16) & 0xff;
    ch4 = (fwversion->version >> 24) & 0xff;

    p = buf;

    *p++ = mychar[HIGH4(ch1)];

    *p++ = '.';

    *p++ = mychar[LOW4(ch1)];

    *p++ = '.';

    *p++ = mychar[HIGH4(ch2)];
    *p++ = mychar[LOW4(ch2)];

    *p++ = '.';

    *p++ = mychar[HIGH4(ch3)];
    *p++ = mychar[LOW4(ch3)];
    *p++ = mychar[HIGH4(ch4)];
    *p++ = mychar[LOW4(ch4)];

    *p = 0;
}

/*
 ********************************************************************************
 * Description : 把actos的版本日期格式转成字符串格式
 *
 * Arguments   : fwversion, actos格式的版本日期信息
 buf, 输出字符串缓冲区
 *
 * Returns     :
 *
 * Notes       :
 0x20,0x04,0x03,0x11  => 2004/03/11
 *
 ********************************************************************************
 */
void fmt_date(const fwversion_t *fwversion, char *buf)//固件日期。现改为手动设置
{
    uint8 *p;
    uint8 ch1, ch2, ch3, ch4;

    ch1 = fwversion->date & 0xff;
    ch2 = (fwversion->date >> 8) & 0xff;
    ch3 = (fwversion->date >> 16) & 0xff;
    ch4 = (fwversion->date >> 24) & 0xff;

    p = buf;

    *p++ = mychar[HIGH4(ch1)];
    *p++ = mychar[LOW4(ch1)];
    //*p++ = mychar[HIGH4(ch2)];
    //*p++ = mychar[LOW4(ch2)];

   *p++ = '.';

    *p++ = mychar[HIGH4(ch3)];
    *p++ = mychar[LOW4(ch3)];

    *p++ = '.';

    *p++ = mychar[HIGH4(ch4)];
    *p++ = mychar[LOW4(ch4)];

    *p = 0;
}

/*
 ********************************************************************************
 * Description : 固件版本界面
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void show_data_mb(ulong data_mb,uint8 x,uint8 y)
{
  uint8 count=0;
  uint8 buf[16];
	data_mb/=2048;
			 if(data_mb>=1000)
			 {
			 	itoa4(data_mb,buf); 
				count=4;
			 }
			 else if(data_mb>=100)
			 {
			 	itoa3(data_mb,buf); 
				count=3;
			 }
			 else if(data_mb>=10)
			 {
			 	itoa2(data_mb,buf); 
				count=2;
			 }
			 else 
			 {
			 	itoa1(data_mb,buf); 
				count=1;
			 }
			    buf[count]='M';
			    buf[++count]='B';
			    buf[++count]='\0';
			    SetTextPos(x, y);
			    //PutS(buf, -1);
			     PutS(buf, count);
				
}

uint16 show_firmware(void)//固件版本
{  
   #if 1
    uint8 key;
	uint8 mem_ok;
	uint16 scrollcount=0;
    bool need_draw = TRUE;
	ulong total1 = 0L, free1 = 0L; //插卡
	
	//=====
    mem_ok = DRV_ActiveDrive('C', 0);
    if (mem_ok)
    {
     total1 = FS_GetSpace(FS_GET_SPACE_TOTAL);
     free1 = FS_GetSpace(FS_GET_SPACE_FREE);
    }
	//=====
	
    while (1)
    {
        //show ui
        if (need_draw)
        {
            ClearScreen(NULL);
            ui_show_batt(GetBattery());
            //show date & version
            SetAsciiFont(FONT_TYPE_LARGE);
   		    ResShowMultiString(MEMVER, 0, 16);  //
			/*
            GetFwVersion(&fwversion);
            //date 2004/01/01
            fmt_date(&fwversion, buf);
            SetTextPos(20, 8);
            PutS(buf, -1);
            */
            SetTextPos(24, 16);
            PutS(g_modifyval.FirmwareDate, -1);
            
     		ResShowMultiString(MEMFREE, 0, 32);
			//data_mb=free1/2048;
			show_data_mb(free1,40,32);
			ResShowMultiString(MEMTOTAL, 0, 48); 
			//data_mb=total1/2048;	
			show_data_mb(total1,40,48);

			SetAsciiFont(FONT_TYPE_SMALL);
            UpdateScreen(NULL);
            need_draw = FALSE;
        }//need_draw
         if(ScrollFlag)
   	     {
           if(scrollcount++>0x1000)
           {
             scrollcount = 0;
		     SetAsciiFont(FONT_TYPE_SMALL);
             ScrollString(scroll_buf, FALSE,UNICODELANGUAGE,&region_scroll);
             UpdateScreen(NULL);
			 SetAsciiFont(FONT_TYPE_DEFAULT);
           }
   	     } 

        //handle msg
        key = ap_get_message();
        switch (key)
        {
            //case AP_MSG_WAIT_TIMEOUT: //子菜单界面在8秒后返回系统功能菜单
            case AP_KEY_MODE | AP_KEY_UP:
            case AP_KEY_PLAY | AP_KEY_UP:
            case AP_KEY_PREV | AP_KEY_UP:
            case AP_KEY_NEXT | AP_KEY_UP:
            case AP_MSG_WAIT_TIMEOUT:
                return 0;
			
            default:
                key = ap_handle_hotkey(key);
                if (key == RESULT_REDRAW)
                    need_draw = TRUE;
                else if (key != 0)
                    return key;
                break;
        }
    }//while(1)
   #endif	
	return 0;
}
//==========
int16 show_format(uint8 disk)//格式化磁盘
{
    uint8  format_type = 1;
	uint8  iscard =0;
    int16  result; 
    uint32 disk_cap;   
    menu.total = 2;
    menu.active = 0;
    menu.menuhead = STFORMAT;
    menu.string = Fmt_sle;
    menu.string_id = g_comval.langid;
    /*选择主盘时在控件中拔卡不提示卡拔出消息*/
    if(disk != 'H')
    {
        DRV_ActiveUD(0,MODE_SLOW);
    }    
    //ui_auto_select = TRUE;
    result = ui_function_menu(&menu, NULL);
    //ui_auto_select = FALSE;
    if(result == RESULT_IGNORE || result == RESULT_SD_OUT)
    {
        return 0;
    }
    else if(result == 0 && menu.active == 1)
    {
        if(disk == 'C')
        {
			DRV_ActiveUD(0,MODE_SLOW);
			disk_cap = ((uint32)UD_GetCap())<<11;//UD_GetCap 是以M bytes为单位
			disk_cap = disk_cap - GetSDCap()-GetVMCap()-GetMICap()-GetUICap()-GetHideDiskCap()-GetAutoRunDiskCap();
			iscard = 0;
        }
        else if(DRV_DetectUD(1) == 0x20)
        {
            DRV_ActiveUD(1,MODE_SLOW);
		    disk_cap = ((uint32)UD_GetCap())<<11;//UD_GetCap 是以M bytes为单位
			iscard = 1;
        }
        else
        {
            return 0;
        }       
        if(disk_cap > 0x400000)
        {
            format_type = 2;
        }
        ClearScreen(NULL);
        ResShowMultiString(RINIT, 255, 16); 
        if(DRV_FormatDrive(disk_cap, format_type,iscard))   
        {
            ClearScreen(NULL);
            ResShowMultiString(DISKFMT, 255, 16); 
        } 
        else
        {
            ClearScreen(NULL);
            ResShowMultiString(MFMTERR, 255, 16); 
        }
		UpdateScreen(NULL);
        while(ap_get_message() != 0);    //清除RTC消息,确保delay准确 
        ap_sleep(2);      
    } 
    return result;   

}

//===恢复系统设置
uint16 show_reset(void)
{   
    uint16 Result;
    uint8 VRamBuffer[32];
    menu.total = 2;
    menu.active = 0;
    menu.menuhead = STRRESET;
    menu.string = Fmt_sle;
    menu.string_id = g_comval.langid;
    //ui_auto_select = TRUE;
    Result = ui_function_menu(&menu, NULL);
    //ui_auto_select = FALSE;
    if(Result==NULL&&menu.active==1)
    {  
        ClearScreen(NULL);
        ResShowMultiString(RINIT,255, 16);
        UpdateScreen(NULL);  
	
        memset(VRamBuffer,0xFF,sizeof(VRamBuffer));
        VMWrite(VRamBuffer, VM_AP_MUSIC);
        VMWrite(VRamBuffer, VM_AP_RECORD);
        VMWrite(VRamBuffer, VM_AP_RADIO);
        VMWrite(VRamBuffer, VM_AP_VOICE);
/*   
        g_comval.langid=2;   
        g_comval.Lightmode=1;
        g_comval.LightTime=15;
        g_comval.StandbyTime=15;
        g_comval.SleepTime=0;
		g_comval.DisplayContrast=7;
        g_comval.VolumeMax=32; 
		g_comval.rec_bitrate=0;
        VMWrite(&g_comval, VM_SYSTEM);
        g_modifyval.Magic=0xffff;
        VMWrite(&g_modifyval, VM_AP_FWCFG);
        SetLanguage(g_comval.langid);
        ResClose(res_fp);
        res_fp = ResOpen("ui30.res");
        ap_message_init(&g_comval);  
        SetContrast(g_comval.DisplayContrast);
*/
        g_comval.magic=0xffff;
        VMWrite(&g_comval, VM_SYSTEM);
                
       return RESULT_SYSTEM;//
    }
	else
	{
      if(Result==RESULT_IGNORE)
       Result=NULL;  
	}
	return Result;
}
/*
 ********************************************************************************
 * Description : 录音设置
 *
 * Arguments   :
 *
 * Returns     : 结果消息
 RESULT_UPGRADE        //进入adfu
 *
 * Notes       :
 *
 ********************************************************************************
 */

/*
 ********************************************************************************
 * Description : 系统工具
 *
 * Arguments   :
 *
 * Returns     : 结果消息
 RESULT_UPGRADE        //进入adfu
 *
 * Notes       :
 *
 ********************************************************************************
 */
 //===删除 格式化 恢复设定 系统资料(版本信息)
uint16 system_tool(void)
{
    uint16 result;
    uint16 g_menu_act=0;
	uint8 disk_letter='C';
	browser_vars_t g_browser_tmp;
	while(1)
	{
	    menu.total = 4;
	    menu.active = g_menu_act;
	    menu.menuhead = STSYTOOL;
	    menu.string = systool_text;
	    menu.string_id = g_comval.langid;
	    //ui_auto_select = TRUE;
	    result = ui_function_menu(&menu, NULL);
		if(result==RESULT_IGNORE)
			return 0;
		g_menu_act=menu.active;
	    //ui_auto_select = FALSE;
		if(result!=0)
		{
	       if(result==RESULT_IGNORE)
		   	result=0;
	       else
		   	return result;
		}
		else
		{
           switch(g_menu_act)
           	{
			   	case 0: //主盘目录内嵌删除功能
			      	DRV_ActiveDrive('C', 1);//需要进行初始化
			   	    g_browser_tmp.location.disk = 'C';
			        g_browser_tmp.location.fselTotal = 0;
			        strcpy(g_browser_tmp.location.path, FS_CD_ROOT);
			        g_browser_tmp.location.ClusterNo = 0x0;
			        result = ui_directory(&g_browser_tmp.location, DIRECTORY_ALL);
					if(result!=RESULT_IGNORE)
					return 	result;
			   	break;
				
				case 1: //格式化
				result = show_format(disk_letter); 
				if(result!=0)
				return result;
				break;
				
				case 2: //恢复设置
				result=show_reset();
				if(result!=0)
				return result;
				break;
				
			    case 3:	//系统资料	
			    result=show_firmware();
				if(result!=0)
				return result;
				
			    break;
	            default:
				break;
           	}
		}
	
	}
  return 0;	
}



//R B'  RF2  R' B   RF2   R2 
// RU' RU RU RU'   R'U' R2
/*
 ********************************************************************************
 * Description : 联机模式界面
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
  #if 0
uint16 show_online(void)
{
    uint16 result;

    result = DRV_DetectDrive('H');
    if ((result & 0x80) != 0)
        menu.total = 4;
    else
        menu.total = 3;
    if(g_comval.OnlineMode >= menu.total)
    {
        menu.active = 0;
    }
    else
    {
        menu.active = g_comval.OnlineMode;
    }
   
    menu.menuhead = MAIN5;
    menu.string = online_text;
    menu.string_id = g_comval.langid;
    //menu.headid=SMAIN65;
    ui_auto_select = FALSE;
    result = ui_function_menu(&menu, NULL);
    ui_auto_select = TRUE;
    if (result == RESULT_IGNORE)
        return 0;
    //record the value
    g_comval.OnlineMode = menu.active;
 
  return result;
}
 #endif 
#if 0
int16 show_format(uint8 disk)
{
    uint8  format_type = 1;
	uint8  iscard =0;
    int16  result; 
    uint32 disk_cap;   
    list.strid = select_text;
    list.total = sizeof(select_text)/2;  
    list.active = 1;;
    list.headid = T_SETUP;
    /*选择主盘时在控件中拔卡不提示卡拔出消息*/
    if(disk != 'H')
    {
        DRV_ActiveUD(0,MODE_SLOW);
    }    
    result = ui_popup_list(&list, NULL);
    if(result == RESULT_IGNORE || result == RESULT_SD_OUT)
    {
        return 0;
    }
    else if(result == 0 && list.active == 0)
    {
        if(disk == 'C')
        {
			DRV_ActiveUD(0,MODE_SLOW);
			disk_cap = ((uint32)UD_GetCap())<<11;//UD_GetCap 是以M bytes为单位
			disk_cap = disk_cap - GetSDCap()-GetVMCap()-GetMICap()-GetUICap()-GetHideDiskCap()-GetAutoRunDiskCap();
			iscard = 0;
        }
        else if(DRV_DetectUD(1) == 0x20)
        {
            DRV_ActiveUD(1,MODE_SLOW);
		    disk_cap = ((uint32)UD_GetCap())<<11;//UD_GetCap 是以M bytes为单位
			iscard = 1;
        }
        else
        {
            return 0;
        }       
        if(disk_cap > 0x400000)
        {
            format_type = 2;
        }
        ClearScreen(NULL);
        ResShowMultiString(FORMATTING, 255, 80); 
        if(DRV_FormatDrive(disk_cap, format_type,iscard))   
        {
            ClearScreen(NULL);
            ResShowMultiString(FORMATOK, 255, 80); 
        } 
        else
        {
            ClearScreen(NULL);
            ResShowMultiString(FORMATFAIL, 255, 80); 
        }
        while(ap_get_message() != 0);    //清除RTC消息,确保delay准确 
        ap_sleep(2);      
    } 
    return result;   

}




uint16 do_format(void)
{
    uint8 disk_letter; 
    uint16 result;
    list.strid = format_text;
    list.total = sizeof(format_text)/2;
    if (DRV_DetectUD(1) != 0x20)
    {
      list.total--;   
    } 
    else
    {
        /*装载卡驱动，控件中能及时收到拔卡消息*/
	    DRV_ActiveUD(1,MODE_SLOW); 
    }       
    list.active = 0;;
    list.headid = T_SETUP;
   
    result = ui_popup_list(&list, NULL);
    
    if(result == RESULT_IGNORE || result == RESULT_SD_OUT )
    {
        return 0;
    }
    else if(result == 0)
    {
       if(list.active == 0)
       {
          disk_letter = 'C';
       }
       else
       {
          disk_letter = 'H';
       }
       result = show_format(disk_letter); 
       if(result == RESULT_IGNORE)
       {
           return 0;
       }   
       else
       {
          return result;      
       }            
    }  
    else
    {
        return result;
    }      
}
#endif

void OpenPA(uint8 volume)
{      
    pa_t pa ={ 0 };
    pa.pa_in.dacin = 1;//默认
    pa.volume = volume;
    if (g_comval.EarphoneFlag == 0)
    {
       EnablePA(&pa);
    }
    else
    {
       EnablePADDV(&pa);
    }
	if (g_comval.ClassDFlag && (g_comval.KeyTone > 0))  	//硬件支持classD
	{
        //EnableCLD(); 
		#ifndef PC
	    earphoneinflag = 1;		//允许直驱与CLD的自动切换 
        #endif
	}
}

  #if 0
void keytone_callback(uint16 value)
{  
 
    uint8 temp_tone;
	temp_tone = g_comval.KeyTone;
    g_comval.KeyTone = (uint8)value;   
    if(g_comval.KeyTone == 1)
    {	     
	   dac_t dacattr;
	   dacattr.dacselect = 0;
       dacattr.rate = 22;	
       EnableDAC(&dacattr);	
       OpenPA(g_comval.KeyTone);
    }                                
	else if(g_comval.KeyTone == 0)
    {  	
        if (g_comval.EarphoneFlag == 0)
        {
            DisablePA();
        }
        else
        {
            DisablePADDV();
        }
        if(g_comval.ClassDFlag)
        {    
		    DisableCLD();
		}
		DisableDAC();
        #ifndef PC
        earphoneinflag = 0;
        #endif 
    } 
	
}
#endif

	






