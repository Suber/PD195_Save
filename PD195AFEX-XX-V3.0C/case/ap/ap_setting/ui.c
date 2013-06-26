/*
*******************************************************************************
*                ACTOS AP
*        system setting ap of sdk 3.0, ui part
*
*        (c) Copyright, Actions Co,Ld.
*             All Right Reserved
*
*  $Id: ui.c,v 1.2 2006/06/26 01:15:39 tangwh Exp $
*******************************************************************************
*/
#include "actos.h"
#include "enhanced.h"
#include "ap_common.h"
#include "ap_setting.h"    
#define LANGUAGETOTAL  24    

#pragma name(AP_UI)

extern comval_t g_comval; 
extern uint8 g_contrast_num; 
extern vm_cfg_t  g_modifyval;   
 
const uint16 lan_text[LANGUAGETOTAL] =
{ 
  SLAN1, SLAN3, SLAN2, SLAN4, SLAN5, SLAN6, SLAN7, SLAN8,SLAN9, SLAN10, 
  SLAN11, SLAN12, SLAN13, SLAN14, SLAN15,SLAN16, SLAN17, SLAN18, SLAN19, 
  SLAN20, SLAN21, SLAN22, SLAN23, SLAN24 

//  SLAN1,SLAN2,SLAN3,SLAN6,SLAN7,SLAN8,SLAN10,SLAN11,SLAN15,SLAN16,
};
//const uint16 Menu_Poweroff[2]={EARTIME,STPWCLOS};
const uint16 Menu_Poweroff[2]={AUTOOFF,ALWAYSON};
const uint16 Poweroff_time[5]= {TIME1M,TIME2M,TIME5M,TIME10M,DISLIMIT};  //关机时间   
const uint8 Powervalue[5]={1,2,5,10,0};
const uint16 Menu_Select[2]= {SLECTNO,SLECTYES};
const uint16 Menu_Display[2]={CONTRAST,BACKLIGHT};
const uint16 Menu_display_text[2]= {SMAIN6,SMAIN2};
const uint16 Menu_lighttime[5]= {TIME10,TIME20,TIME30,TIME1M,DISLIMIT};
const uint8 Light_time[5]={10,20,30,60,0};
const uint16 Autolock_time[5]= {TIME10,TIME20,TIME30,TIME1M,DISLIMIT};  
const uint8 Locktime[5]={10,20,30,60,0};

uint16 far lan_list[LANGUAGETOTAL];//要显示的语言资源ID列表 


void backlight_callback(uint16 value);

/*
 ********************************************************************************
 * Description : 背光时间设置回调函数
 *
 * Arguments   : value, 背光时间,单位秒
 *
 * Returns     :
 *
 * Notes       : =0时关闭背光
 *
 ********************************************************************************
 */
void backlight_callback(uint16 value)
{
      
    g_comval.LightTime = (uint8) value;
    ap_message_init(&g_comval);
}

/*
 ********************************************************************************
 * Description : 背光时间设定界面
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
/*
uint16 show_backlight(void)
{ 
    ClearScreen(NULL);

    //backlight read from vm

    //show little icon
//    ResShowPic(BKLIGHT, 10, 0);
    UpdateScreen(NULL);

    //init slider
    slider.max = 30;
    slider.min = 0;
    slider.step = 3;
    slider.value = g_comval.LightTime;

    //return ui_slider(&slider, backlight_callback);
    return ui_slider(&slider, backlight_callback, 1);

}
*/

//==显示设置
uint16 show_display(void)//包含背光时间和对比度调节
{
    uint8 i;
    uint16 result;
	uint16 active=0;
	 while(1)
	 	{
		     menu.total = 2;        
		     menu.menuhead = STDISP;//显示设置
		     menu.string = Menu_display_text;
		     menu.active = active;
		     menu.string_id = g_comval.langid;
		     //ui_auto_select = TRUE;  
		     result = ui_function_menu(&menu, NULL);
		     //ui_auto_select = FALSE;
			 active=menu.active;
			 if(result!=NULL)
			 {
			   	return  result;
			 }
			 else
		 	 {
                switch(menu.active)
                	{
			      	 case 1:
					  	 menu.total = sizeof(Menu_lighttime)/2;        
					     menu.menuhead = MAIN5;
					     menu.string = Menu_lighttime;
					     for(i=0;i<menu.total;i++)
					     {
                            if(g_comval.StandbyTime == Light_time[i])
                            {
                                menu.active = i;
                                break;
                            }
					     }
					     if(i >= menu.total)
					        menu.active = 0;
						 menu.string_id = g_comval.langid;
					     //ui_auto_select = TRUE;  //show menu
					     result = ui_function_menu(&menu, NULL);
					     //ui_auto_select = FALSE;
						 if(result!=NULL)
			             {
				              if(result!=RESULT_IGNORE)
							   return  result; 
						 }
						 else
						 {
							 g_comval.LightTime=Light_time[menu.active]; 
							 ap_message_init(&g_comval);
						 } 	
			             break;
				      case 0:
					 	 result=show_contrast();
						 if(result!=NULL)
						 return result;
			             break; 
					  default:
					  	break;
                	}
		 	 }
			 		 	
	 	 }	
	 return  0;
}

//==Auto Lock设置
uint16 show_lock(void)
{
                         uint8 i;
                         uint16 result;
                         
	                     menu.total = sizeof(Autolock_time)/2;        
					     menu.menuhead = EARTIME;
					     menu.string = Autolock_time;
					     for(i=0;i<menu.total;i++)
					     {
                            if(g_comval.StandbyTime == Locktime[i])
                            {
                                menu.active = i;
                                break;
                            }
					     }
					     if(i >= menu.total)
					        menu.active = 0;
						 menu.string_id = g_comval.langid;
					     //ui_auto_select = TRUE;  //show menu
					     result = ui_function_menu(&menu, NULL);
					     //ui_auto_select = FALSE;
						 if(result!=NULL)
			             {
				              if(result!=RESULT_IGNORE)
							   return  result; 
						 }
						 else
						 {
							 g_comval.LockTime=Locktime[menu.active]; 
							 ap_message_init(&g_comval);
						 } 	
                         return  0;
}
//== 
/*
********************************************************************************
* Description : 语言设定界面
*
* Arguments   :
*
* Returns     :
*
* Notes       : 注意切换字体后打开lcd
*
********************************************************************************
*/
     
uint16 show_language(void)
{
    uint16 result;
    uint8 old_id;
    uint8 total_language;
    uint8 language_num;
    uint8 i, j;
    uint16 lan_list[LANGUAGETOTAL];

    ClearScreen(NULL);

    if (g_modifyval.LanInfor.Total > LANGUAGETOTAL)
    {
        total_language = LANGUAGETOTAL;
    }
    else
    {
        total_language = g_modifyval.LanInfor.Total;
    }

    language_num = 0;
    for (i = 0; i < total_language; i++)
    {
        j = g_modifyval.LanInfor.Table[i];
        lan_list[i] = lan_text[j];

        if (g_comval.langid == g_modifyval.LanInfor.Table[i])
        {
            language_num = i;
        }
    }

    //init
    menu.total = total_language;
    menu.string = lan_list;
    menu.active = language_num; //g_comval.langid;
    menu.string_id = g_comval.langid;
    old_id = g_comval.langid;
    //ui_auto_select = TRUE;
    result = ui_function_menu(&menu, NULL);
    //ui_auto_select = FALSE;
    if (result == RESULT_IGNORE)
        return 0;

    language_num =   menu.active;
    g_comval.langid = g_modifyval.LanInfor.Table[language_num];

    //check if need install new driver
    if (g_comval.langid != old_id)
    {

 
        
		SetLanguage(g_comval.langid);
        VMWrite(&g_comval, VM_SYSTEM);
        /*更新语言驱动后需重新打开资源文件*/
        ResClose(res_fp);
        res_fp = ResOpen("ui30.res");
        StandbyScreen(TRUE);
 
    }

    SetContrast(g_comval.DisplayContrast);
    return result;
}

    
    
    
    
    
/*
********************************************************************************
* Description : 关机设定界面
*
* Arguments   :
*
* Returns     :
*
* Notes       : 非标准功能菜单控件
*
********************************************************************************
*/
/*
 ********************************************************************************
 * Description : 关机设定界面
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       : 非标准功能菜单控件
 *
 ********************************************************************************
 */
 #if 0
uint16 show_powerdown(void)
{
    uint16 result;
    uint8 key;
    uint8 mode = 0; //0, power mode, 1, sleep mode
    bool need_draw = TRUE;
    while (1)
    {
        //show ui
        if (need_draw)
        {
            ClearScreen(NULL);

            if (mode == 0) 
            {
                ResShowMultiString(SPOWER1,  255, 40);
            }
            else 
            {
                ResShowMultiString(SPOWER2,  255, 40);
            }
            need_draw = FALSE;
        }
        key = ap_get_message();
        switch (key)
        {
            case AP_KEY_PREV:
            case AP_KEY_NEXT:
            case AP_KEY_PREV | AP_KEY_HOLD:
            case AP_KEY_NEXT | AP_KEY_HOLD:
            if (mode == 0)
                mode = 1;
            else
                mode = 0;
            need_draw = TRUE;
            break;

            case AP_MSG_WAIT_TIMEOUT: //子菜单界面在8秒后返回系统功能菜单, add by ccm 2004/07/15
            case AP_KEY_PLAY | AP_KEY_UP:
             
            return 0;

            case AP_KEY_MODE | AP_KEY_UP:
            ClearScreen(NULL);

            //show background
////            ResShowPic(POWER, 10, 0);
            UpdateScreen(NULL);

            if (mode == 0)
            {
                slider.min = 0;
                slider.max = 60;
                slider.step = 10;
                slider.value = g_comval.StandbyTime;
                result = ui_slider(&slider, NULL, 1);
                if (result != 0)
                    return result;
                g_comval.StandbyTime = (BYTE) slider.value;
            }
            else
            {
                slider.min = 0;
                slider.max = 120;
                slider.step = 10;
                slider.value = g_comval.SleepTime;
                result = ui_slider(&slider, NULL, 1);
                if (result != 0)
                    return result;
                g_comval.SleepTime = (BYTE) slider.value;
            }

            //重新设置关机时间
            ap_message_init(&g_comval);

            return 0; //不回到上一个界面吗??
            //break;
            default:
            key = ap_handle_hotkey(key);
            if (key == RESULT_REDRAW)
                need_draw = TRUE;
            else if (key != 0)
                return key;
        }//key

    } //while(1)
	return 0;
}
 #endif
 uint16 show_powerdown(void)//关机设定(相当于样机的省电模式)
 {
     
                         uint8 i;
                         uint16 result;
                         
	                     menu.total = sizeof(Poweroff_time)/2;        
					     menu.menuhead = EARTIME;
					     menu.string = Poweroff_time;
					     for(i=0;i<menu.total;i++)
					     {
                            if(g_comval.StandbyTime == Powervalue[i])
                            {
                                menu.active = i;
                                break;
                            }
					     }
					     if(i >= menu.total)
					        menu.active = 0;
						 menu.string_id = g_comval.langid;
					     //ui_auto_select = TRUE;  //show menu
					     result = ui_function_menu(&menu, NULL);
					     //ui_auto_select = FALSE;
						 if(result!=NULL)
			             {
				              if(result!=RESULT_IGNORE)
							   return  result; 
						 }
						 else
						 {
							 g_comval.StandbyTime=Powervalue[menu.active]; 
							 ap_message_init(&g_comval);
						 } 	
                         return  0;
	/*	  
	 uint16 result;
	 uint16 active=0;
	 while(1)
	 	{
		     menu.total = 2;        
		     menu.menuhead = SPOWER1;
		     menu.string = Menu_Poweroff;
		     menu.active = active;//
		     menu.string_id = g_comval.langid;
		     ui_auto_select = FALSE;  //show menu
		     result = ui_function_menu(&menu, NULL);
		     ui_auto_select = TRUE;
			 active=menu.active;
			 if(result!=NULL)
			 {
			   	return  result;
			 }
			 else
		 	 {
                switch(menu.active)
                	{
			      	 case 0:
					  	 menu.total = 6;        
					     menu.menuhead = EARTIME;
					     menu.string = Poweroff_time;
						 if(g_comval.StandbyTime==0)
					     menu.active =5;
						 else
						 menu.active=g_comval.StandbyTime/30;
					     menu.string_id = g_comval.langid;
					     ui_auto_select = FALSE;  //show menu
					     result = ui_function_menu(&menu, NULL);
					     ui_auto_select = TRUE;
						 if(result!=NULL)
			             {
				              if(result!=RESULT_IGNORE)
							   return  result; 
						 }
						 else
                         g_comval.StandbyTime=Powervalue[menu.active]; 
			             break;
				     case 1:
					 	 menu.total = 2;        
					     menu.menuhead = STPWCLOS;
					     menu.string = Menu_Select;
						 menu.active=(uint16)g_comval.playpowerofflag;
					     menu.string_id = g_comval.langid;
					     ui_auto_select = FALSE;  
					     result = ui_function_menu(&menu, NULL);
					     ui_auto_select = TRUE;
						 if(result!=NULL)
			             {
				              if(result!=RESULT_IGNORE)
							   return  result; 
						 }
						 else
                         g_comval.playpowerofflag=menu.active;
			             break; 
					  default:
					  	break;
                	}
		 	 }
			 		 	
	 	 }	
	 return  0;
     */ 
 }

 #if 0
 uint16 show_display(void)//masda20110525
 {
     uint16 result;
	 uint16 active=0;
	 while(1)
	 	{
		     menu.total = 2;        
		     menu.menuhead = SPOWER1;
		     menu.string = Menu_Display;
		     menu.active = active;//
		     menu.string_id = g_comval.langid;
		     ui_auto_select = FALSE;  //show menu
		     result = ui_function_menu(&menu, NULL);
		     ui_auto_select = TRUE;
			 active=menu.active;
			 if(result!=NULL)
			 {
			   	return  result;
			 }
			 else
		 	 {
                switch(menu.active)
                	{
			      	 case 0:
					 	show_contrast();
						/*
					  	 menu.total = 6;        
					     menu.menuhead = EARTIME;
					     menu.string = Poweroff_time;
						 if(g_comval.SleepTime==0)
					     menu.active =5;
						 else
						 menu.active=g_comval.SleepTime/30;
					     menu.string_id = g_comval.langid;
					     ui_auto_select = FALSE;  //show menu
					     result = ui_function_menu(&menu, NULL);
					     ui_auto_select = TRUE;
						 if(result!=NULL)
			             {
				              if(result!=RESULT_IGNORE)
							   return  result; 
						 }
						 else
                         g_comval.SleepTime=Powervalue[menu.active]; 
						 */
			             break;
				     case 1:
					 	show_backlight();
					 	/*
					 	 menu.total = 2;        
					     menu.menuhead = STPWCLOS;
					     menu.string = Menu_Select;
						 menu.active=(uint16)g_comval.playpowerofflag;
					     menu.string_id = g_comval.langid;
					     ui_auto_select = FALSE;  
					     result = ui_function_menu(&menu, NULL);
					     ui_auto_select = TRUE;
						 if(result!=NULL)
			             {
				              if(result!=RESULT_IGNORE)
							   return  result; 
						 }
						 else
                         g_comval.playpowerofflag=menu.active;
						 */
			             break; 
					  default:
					  	break;
                	}
		 	 }
			 		 	
	 	 }	
	 return  0;
      
 }
 #endif 


/*
 ********************************************************************************
 * Description : 对比度设定回调函数
 *
 * Arguments   : value, 1~16, 对应硬件 6~21
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void contrast_callback(int16 value)
{
    uchar ValideContrast;
    //true value 6~21
    g_comval.DisplayContrast = (uint8) value; //modified by ccm(5->2)

    //设置到硬件
    ValideContrast = g_comval.DisplayContrast ; //显示驱动中定义最大为15级
    SetContrast(ValideContrast);

} 
/*
 ********************************************************************************
 * Description : 对比度设定界面
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       : 注意背景先画
 *
 ********************************************************************************
 */
uint16 show_contrast(void)
{   
    uint16  result;
	uint8  DisplayContrast_bak=g_comval.DisplayContrast;
    ClearScreen(NULL);
	//ResShowMultiString(SMAIN6,255, 0);
    UpdateScreen(NULL);
    slider.min = 0;
    slider.max = g_comval.BackLightMax;
    slider.step = 1;
    if (g_comval.DisplayContrast > g_comval.BackLightMax)
    {
        g_comval.DisplayContrast = g_comval.BackLightMax;
    }

    slider.value = g_comval.DisplayContrast; //modified by ccm(5->2)
   // slider.title = 0x88;    
    result=ui_slider(&slider, contrast_callback);//, 1);   //masda20110526
   // slider.title = 0x00;
    
	if(result!=NULL)
	{

	   if(result==RESULT_IGNORE)
	   	{
		   	contrast_callback(DisplayContrast_bak);
			result=NULL;
	   	}
	}
	return result;
}
 /*
 ********************************************************************************
 * Description : 格式化储存容量的显示字符串
 *
 * Arguments   : buf, 输出字符串buffer
 mega，总容量
 percent，使用百分比
 *
 * Returns     : 输出字符串的长度
 *
 * Notes       : 1024M<100%>
 *
 ********************************************************************************
 */
uint16 fmt_memory(uint8 *buf, uint16 mega, uint8 percent)
{
    uint8 *p = buf;

    if (mega >= 1000)
    {
        itoa4(mega, p);
        p += 4;
    }
    else if (mega >= 100)
    {
        itoa3(mega, p);
        p += 3;
    }
    else
    {
        itoa2(mega, p);
        p += 2;
    }
    *p++ = 'M';
    *p++ = '<';
    if (percent == 100)
    {
        itoa3(percent, p);
        p += 3;
    }
    else
    {
        itoa2(percent, p);
        p += 2;
    }
    *p++ = '%';
    *p++ = '>';
    *p++ = 0;

    return (p - buf);
}

/*
 ********************************************************************************
 * Description : 储存状况界面
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */

uint16 show_memory(void)
{
    bool mem_ok = FALSE; //主内存是否可用
    bool card_ok = FALSE; //卡是否可用
    uint32 total1 = 0L, free1 = 0L; //内存
    uint8 percent1 = 0;
    ulong total2 = 0L, free2 = 0L; //插卡
    uint8 percent2 = 0L;
    uint8 toshow = 0; //要显示那个, 0:main, 1:card
    uint8 key;
    bool need_draw = TRUE;
    uint8 buf[12]; // 1024M<100%>
    uint32 char_count; //buf文字长度
    uint8 time_num = 0;

    //check card
    card_ok = DRV_ActiveDrive('H', 0);
    if (card_ok)
    {
       // FS_ScanFatSec();
        total2 = FS_GetSpace(FS_GET_SPACE_TOTAL);
        free2 = FS_GetSpace(FS_GET_SPACE_FREE);

        //处理<1%
        if (total2 == free2)
            percent2 = 0;
        else
        {
            percent2 = (total2 - free2) * 100 / total2;
            if (percent2 == 0)
                percent2 = 1;
        }
        toshow = 1;
    }

    //check main memory
    mem_ok = DRV_ActiveDrive('C', 0);
    if (mem_ok)
    {

//        FS_ScanFatSec();
        total1 = FS_GetSpace(FS_GET_SPACE_TOTAL);
        free1 = FS_GetSpace(FS_GET_SPACE_FREE);

        //处理<1%
        if (total1 == free1)
            percent1 = 0;
        else
        {
            percent1 = (total1 - free1) * 100 / total1;
            if (percent1 == 0)
                percent1 = 1;
        }
        toshow = 0;
    }

    //control
    progress.min = 0;
    progress.max = 100;

    while (1)
    {
        if (need_draw)
        {
            ClearScreen(NULL);

            //show progress bar & text
            if (toshow == 0)
            {
                //percentage
                progress.value = percent1;

                //显示主内存图标
//                ResShowPic(MEMMAIN, 10, 0);

                //显示箭头
 //               if (card_ok)
//                    ResShowPic(CARROWR, 115, 0);

                //显示文字
                char_count = fmt_memory(buf, total1 / 2048, percent1);
//                ASSERT(char_count <= sizeof(buf));
                SetTextPos((SCR_WIDTH - (BYTE) get_text_width(buf)) / 2, 16);
                PutS(buf, sizeof(buf));
                UpdateScreen(NULL);

                //progress bar
                ui_show_progress(&progress);
            }
            else if (toshow == 1)
            {
                //percentage
                progress.value = percent2;

                //显示卡图标
//                ResShowPic(MEMCARD, 10, 0);

                //显示箭头
  //              if (mem_ok)
//                    ResShowPic(CARROWL, 115, 0);

                //显示文字
                char_count = fmt_memory(buf, total2 / 2048, percent2);
//                ASSERT(char_count <= sizeof(buf));
                SetTextPos((SCR_WIDTH - (BYTE) get_text_width(buf)) / 2, 8);
                PutS(buf, sizeof(buf));
                UpdateScreen(NULL);

                //progress bar
                ui_show_progress(&progress);
            }

            need_draw = FALSE;
        }//need_draw

        key = ap_get_message();

        switch (key)
        {
            //case AP_MSG_WAIT_TIMEOUT:           //子菜单界面在8秒后返回系统功能菜单, add by ccm 2004/07/15
            case AP_KEY_MODE | AP_KEY_UP:
            case AP_KEY_PLAY | AP_KEY_UP:
            return 0; //退出
            //break;

            case AP_KEY_NEXT:
            case AP_KEY_PREV:
            if (toshow == 0 && card_ok)
                toshow = 1;
            else if (toshow == 1 && mem_ok)
                toshow = 0;

            need_draw = TRUE;
            break;
            //case AP_MSG_WAIT_TIMEOUT:
            //ap_sleep(4);
            case AP_MSG_RTC:
            time_num++;
            if (time_num > 30)
                return 0;
            break;
            default:
            key = ap_handle_hotkey(key);
            if (key == RESULT_REDRAW)
                need_draw = TRUE;
            else if (key != 0)
                return key;
        }//switch(key)

    }//while(1)
   return 0;
}
