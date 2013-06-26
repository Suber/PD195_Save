/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: play menu module
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
 
 //播放和停止界面都使用此文件
#include "ap_music.h"
typedef uint16 (*show_func)(void);
#pragma name(AP_UI_MNU)
extern uint8 PlayMenuactive;
extern  uint16 ui_EQ_menu(void);
uint16 ui_relpay_set_menu(void);
uint16 ui_loop_menu(void);
uint16 ui_shuffle_menu(void);

#if(MACHINE_MODEL==2)

const uint16 play_string_pedo[] =   //pedometer
{ MPLAY1,/*VLOOPM6,*/ /*SRS3D,MPLAY4,*//* MPLAY6, MDIRNAVI, MPLAY7 ,CHANGEVO*/MPLAY2,MRSTOP1,PEDO,SWIMFIT};
const uint16 stop_string_pedo[] =//停止界面多了目录浏览
{ MPLAY1,/*VLOOPM6,*/ /*SRS3D,MPLAY4,*//* MPLAY6, MDIRNAVI, MPLAY7 ,CHANGEVO*/MPLAY2,MRSTOP1,PEDO,SWIMFIT};
const uint16 play_string[] =
{ MPLAY1,/*VLOOPM6,*/ /*SRS3D,MPLAY4,*//* MPLAY6, MDIRNAVI, MPLAY7 ,CHANGEVO*/MPLAY2,MRSTOP1};
const uint16 stop_string[] =//停止界面多了目录浏览
{ MPLAY1,/*VLOOPM6,*/ /*SRS3D,MPLAY4,*//* MPLAY6, MDIRNAVI, MPLAY7 ,CHANGEVO*/MPLAY2,MRSTOP1};
const uint16 repeat_string[] =
{ VLOOPM1, VLOOPM2, VLOOPM3, VLOOPM4, VLOOPM5, VLOOPM6, VLOOPM7};
const uint8 repeat_value[] = 
{FSEL_ALL_SEQUENCE,FSEL_ALL_REPEATONE,FSEL_DIR_SEQUENCE,FSEL_DIR_REPEATALL,FSEL_ALL_REPEATALL,FSEL_DIR_RANDOM,FSEL_DIR_INTRO};
const uint16 Fmt_sle[]={SLECTNO,SLECTYES};

#else

const uint16 play_string_pedo[] =   //pedometer
{ MPLAY1, MPLAY2,/*SRS3D,MPLAY4,*//* MPLAY6, MDIRNAVI, MPLAY7 ,CHANGEVO*/MRSTOP1,PEDO,SWIMFIT};
const uint16 stop_string_pedo[] =//停止界面多了目录浏览
{ MPLAY1, MPLAY2,/*SRS3D,MPLAY4,*//* MPLAY6, MDIRNAVI, MPLAY7 ,CHANGEVO*/MRSTOP1,PEDO,SWIMFIT};
const uint16 play_string[] =
{ MPLAY1, MPLAY2,/*SRS3D,MPLAY4,*//* MPLAY6, MDIRNAVI, MPLAY7 ,CHANGEVO*/MRSTOP1};
const uint16 stop_string[] =//停止界面多了目录浏览
{ MPLAY1, MPLAY2,/*SRS3D,MPLAY4,*//* MPLAY6, MDIRNAVI, MPLAY7 ,CHANGEVO*/MRSTOP1};
const uint16 repeat_string[] =
{ VLOOPM1, VLOOPM2, VLOOPM3, VLOOPM4, VLOOPM5, VLOOPM6/*, VLOOPM7 */};
const uint8 repeat_value[] = 
{FSEL_ALL_SEQUENCE,FSEL_ALL_REPEATONE,FSEL_DIR_SEQUENCE,FSEL_DIR_REPEATALL,FSEL_ALL_REPEATALL,FSEL_DIR_RANDOM,FSEL_DIR_INTRO};

#endif

uint16 ui_play_menu(uint8 Playsta)
{
    uint16 result = 0;
	uint16 SelecFunc=0;
	uint8 fselTotalbak,fselfileNobak; 
    function_menu_t menu;
//	file_location_t tmp_location;
    //music_play_status_t status_buf;
    
	while(1){
	menu.menuhead = PLAYMENU;
	if(g_comval.PedoFlag)
	{
     menu.total = sizeof(play_string_pedo) / 2;
     menu.string = play_string_pedo;
	}
	else
	{
	 menu.total = sizeof(play_string) / 2;
     menu.string = play_string;
	}
    menu.active=SelecFunc;
    //ui_auto_select = FALSE;
    g_result = ui_function_menu(&menu, NULL);
    //ui_auto_select = TRUE;
    SelecFunc=menu.active;
    if (g_result == RESULT_IGNORE)
        return 0;
    if (g_result != 0)
        return g_result; //是热键
    if(Playsta)  
	menu.active++;
    switch (menu.active)
    {   
        //#if(MACHINE_MODEL==2)
        
        case 0: //repeat
        g_result = ui_loop_menu();
		if(g_result!=0)
        return g_result;
        break;
        
        //#if(MACHINE_MODEL==2)
        /*
        case 1: //shuffle
        if (g_currentMusicType != Music_Wav)
            g_result = ui_shuffle_menu();
        else
            g_result = 0;
		if(g_result!=0)
        return g_result;
        break;	
        */
        case 1: //eq
        if (g_currentMusicType != Music_Wav)
            g_result = ui_EQ_menu();
        else
            g_result = 0;
		if(g_result!=0)
        return g_result;
        break;	
        
		case 2://主盘目录
	    if (g_music_vars.location.disk == 'H')
        {
            g_music_vars.location.disk = 'C';
            FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, FSEL_TYPE_COMMONDIR, (uint8) g_music_vars.location.disk);
            g_music_vars.fselmod = FSEL_TYPE_COMMONDIR;
        }
		fselTotalbak = g_music_vars.location.fselTotal;
		fselfileNobak = g_music_vars.location.fselFileNo;
		g_music_vars.location.fselTotal = 0;
		//ui_auto_select = TRUE;
        result = ui_directory(&g_music_vars.location, g_MusicType);
		//ui_auto_select = FALSE;
		if(result == RESULT_MUSIC_PLAY)                
		{                		
			g_from_browser = TRUE;                		
			//fselGetLocation(&g_music_vars.location);               
		}
        //  FSELSetMode(FSEL_ALL_SEQUENCE);//为什么会引起死机?可能是BANK切换。
      
		if (result == 0)
        {
            g_music_vars.location.APE_Cur = 0;
			break;
        }
		if(result!=RESULT_IGNORE)
		{  
		    return result;
		}
        break;
        
		case 3:
//			sKY_SetGsensor(2,0,0);		
/*			if(Pedo_work_flag)
			{
				sKY_SetGsensor(0,0,0);
			}
			else
			{			
				sKY_SetGsensor(1,0,0);
			}*/
			Pedo_ui_draw = TRUE;
			Pedo_Swim_flag = FALSE;
			if(Swim_work_flag)  Swim_work_flag = FALSE;
			return 0;  

		case 4:
//			sKY_SetGsensor(2,0,0);		
/*			if(Pedo_work_flag)
			{
				sKY_SetGsensor(0,0,0);
			}
			else
			{			
				sKY_SetGsensor(1,0,0);
			}*/
			Pedo_ui_draw = TRUE;	  
			Pedo_Swim_flag = TRUE;
			if(Pedo_work_flag)  Pedo_work_flag = FALSE;
			return 0;  	
		break;
		

        /*#else
		
        case 1: //eq
        if (g_currentMusicType != Music_Wav)
            g_result = ui_EQ_menu();
        else
            g_result = 0;
		if(g_result!=0)
        return g_result;
        break;*/	
        
//#ifdef SUPPORT_SRS
        /*case 3: //SRS WOW
        if( g_currentMusicType==Music_MP3 ||g_currentMusicType==Music_WMA)
        {
            g_result = ui_SRS_menu();
            return g_result;
        }
        else
        break;*/
		//#endif
        /*
        case 2: // set A_B
        musicpSendCommand(MC_GETSTATUS, (void*) (&status_buf));
        if (status_buf.status == PLAYING_REACH_END)
        {
            break;
        }
        if (musicpSendCommand(MC_SETA_B, ClrABPoint))
        {
            RePlayStatus = WaitSetAPointForAB;
        }
		return 0;
        break;
        */
        
	    /*case 5: //vol
            g_result=ChangeVolume();
		if(g_result!=0)
        return g_result;
        break;*/
        /*
		case 2://主盘目录
	    if (g_music_vars.location.disk == 'H')
        {
            g_music_vars.location.disk = 'C';
            FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, FSEL_TYPE_COMMONDIR, (uint8) g_music_vars.location.disk);
            g_music_vars.fselmod = FSEL_TYPE_COMMONDIR;
        }
		fselTotalbak = g_music_vars.location.fselTotal;
		fselfileNobak = g_music_vars.location.fselFileNo;
		g_music_vars.location.fselTotal = 0;
		//ui_auto_select = TRUE;
        result = ui_directory(&g_music_vars.location, g_MusicType);
		//ui_auto_select = FALSE;
		if(result == RESULT_MUSIC_PLAY)                
		{                		
			g_from_browser = TRUE;                		
			//fselGetLocation(&g_music_vars.location);               
		}*/
        //  FSELSetMode(FSEL_ALL_SEQUENCE);//为什么会引起死机?可能是BANK切换。
      
		/*
		//=============test
		if (result == RESULT_IGNORE)
		{  
		   	{//test
		    char buf1[5];
			ClearScreen(NULL);
			SetTextPos(0,0);
			itoa4(g_music_vars.location.fselFileNo,buf1);
			PutS(buf1,-1);
			itoa4(g_music_vars.location.fselTotal,buf1);
			SetTextPos(0,16);
			PutS(buf1,-1);
			SetTextPos(0,32);
			PutS(g_music_vars.location.filename,-1);
			UpdateScreen(NULL);
			ap_sleep(2);
			ap_sleep(2);
			ap_sleep(2);
			ap_sleep(2);
			}
        }
        */
		//==============
	
        
/*		case 3:
//			sKY_SetGsensor(2,0,0);		
/*			if(Pedo_work_flag)
			{
				sKY_SetGsensor(0,0,0);
			}
			else
			{			
				sKY_SetGsensor(1,0,0);
			}*/
			
/*		case 4:
//			sKY_SetGsensor(2,0,0);		
/*			if(Pedo_work_flag)
			{
				sKY_SetGsensor(0,0,0);
			}
			else
			{			
				sKY_SetGsensor(1,0,0);
			}*/

		//#endif
		
		
#if 0

        case 3: //replay times
        slider.max = 10;
        slider.min = 1;
        slider.step = 1;
        CheckReplaytimesLimit();
        slider.value = g_music_vars.replaytimes;
        ClearScreen(NULL);
        //UpdateScreen(NULL);
        ui_auto_update = FALSE;
//        ResShowPic(BKMENU, 22, 16);
        SetTextPos(26, 24);
        PutS("Replay times", 12);
        g_result = ui_slider(&slider, NULL, 1);
        ui_auto_update = TRUE;
        g_music_vars.replaytimes = slider.value;
        if (g_result != 0)
            return g_result;
        break;

        case 4: //replay pause
        slider.max = 10;
        slider.min = 1;
        slider.step = 1;
        CheckReplaygapLimit();
        slider.value = g_music_vars.replaygap;
        ClearScreen(NULL);
        //UpdateScreen(NULL);
        //ui_auto_update=FALSE;
        ui_auto_update = FALSE;
//        ResShowPic(BKMENU, 22, 16);
        SetTextPos(30, 24);
        PutS("Replay gap", 12);
        g_result = ui_slider(&slider, NULL, 1);
        ui_auto_update = TRUE;
        g_music_vars.replaygap = slider.value;
        if (g_result != 0)
            return g_result;
        break;
 
#ifdef SUPPORT_FAV
        case 8: //收藏夹功能
        g_music_vars.file_list_type = 0xff;
        g_result =UI_Favourite(g_comval.langid);
        if(g_result == RESULT_IGNORE)g_result = 0;
        return g_result;
        break;
#endif
        case 5: //Navi
     // g_music_vars.file_list_type = 0xff;
        g_music_vars.fselmod = FSEL_TYPE_COMMONDIR;
#ifdef   BROWSER_DISK
        memcpy(&g_browser_vars,&g_music_vars.location,sizeof(file_location_t));
        g_result = browser_main(&g_browser_vars,0,g_comval.langid);
        if(g_result == RESULT_MUSIC_PLAY)
        {
            memcpy(&g_music_vars.location, &g_browser_vars, sizeof(file_location_t));
        }
#else
       // g_result = Deal_play_Navi();
#endif
        if (g_result == RESULT_IGNORE)
            g_result = 0;
        return g_result;
        break;

        //        case 13: //exit
        //            break;
#endif

        default:
        break;
    } //switch(menu.active)
   }
    return result;
}

void loop_callback(uint16 loop)
{
    g_music_vars.repeat_mode = repeat_value[loop];
    UpDisplayFlag = UpDisplayFlag | up_musicloopmode;
}
uint16 ui_loop_menu(void)
{
    uint8 i;
    function_menu_t menu;
	uint16 loop_bak=g_music_vars.repeat_mode;
	
    menu.total = sizeof(repeat_string) / 2;//MaxRepeatMod;  
    menu.menuhead = MPLAY1;
    menu.string = repeat_string;
    #if(MACHINE_MODEL==2)
	if(g_music_vars.repeat_mode == FSEL_DIR_RANDOM)
        menu.active = 0;
    else
    {
        for(i=0;i<menu.total;i++)
		{
            if(g_music_vars.repeat_mode == repeat_value[i])
            {
                menu.active = i;
                break;
            }
		}
		if(i >= menu.total)
		    menu.active = 0;
    }    
	#else
    menu.active = g_music_vars.repeat_mode;
    #endif
    //ui_auto_select = FALSE;
    g_result = ui_function_menu(&menu, loop_callback);
    //ui_auto_select = TRUE;
    StartIntro(FALSE);
    if (g_result != 0)
    {  
        if (g_result == RESULT_IGNORE)
        {
          loop_callback(loop_bak);
		  g_result=0;
        }
        return g_result;
    }
    return 0;
}

#if(MACHINE_MODEL==1)
uint16 ui_shuffle_menu(void)
{
    function_menu_t menu;
   	uint16 loop_bak=g_music_vars.repeat_mode;

    menu.total = 2;
    menu.active = 0;
    menu.menuhead = VLOOPM6;
    menu.string = Fmt_sle;
    menu.string_id = g_comval.langid;
    //ui_auto_select = TRUE;
    g_result = ui_function_menu(&menu, NULL);
    //ui_auto_select = FALSE;
    if(g_result==NULL && menu.active==1)
    {
        g_music_vars.repeat_mode = FSEL_DIR_RANDOM;
        UpDisplayFlag = UpDisplayFlag | up_musicloopmode;
    }
    else
    {
        if ((g_result == RESULT_IGNORE) || (g_result==NULL && menu.active==0))
        {
            if(loop_bak == FSEL_DIR_RANDOM) loop_bak = 0;
            loop_callback(loop_bak);
		    g_result=0;
        }
        return g_result;
    }
   
    return 0;
}
#endif

#if 0
uint16 ui_relpay_set_menu(void)
{
    slider_t slider_playmenu;
    pop_up_list_t list_playmenu;
    list_playmenu.strid = replay_string;
    list_playmenu.total = 3;
    list_playmenu.active = 0;
    if(g_MusicType == FSEL_TYPE_VOICE)
    {
        list_playmenu.headid = T_VOICE;//SETREPLAY;
    }
    else
    {
        list_playmenu.headid = T_MUSIC;//SETREPLAY;
    }    
    g_result = ui_popup_list(&list_playmenu, NULL);
    if (g_result == RESULT_IGNORE)
    {
        return 0;
    }
    if (g_result != 0)
    {
        return g_result;
    }
    if (list_playmenu.active == 0)
    {
        if (musicpSendCommand(MC_SETA_B, ClrABPoint) != 0)
        {
            RePlayStatus = WaitSetAPointForAB;
        }
    }
    else
    {
        slider_playmenu.max = 10;
        slider_playmenu.min = 1;
        slider_playmenu.step = 1;
        CheckReplaytimesLimit();
        if (list_playmenu.active == 1)
        {
            slider_playmenu.value = g_music_vars.replaytimes;
        }
        else
        {
            slider_playmenu.value = g_music_vars.replaygap;
        }

        slider_playmenu.title = SETREPLAY;
        g_result = ui_slider(&slider_playmenu, NULL);
        if (list_playmenu.active == 1)
        {
            g_music_vars.replaytimes = slider_playmenu.value;
        }
        else
        {
            g_music_vars.replaygap = slider_playmenu.value;
        }
        if (g_result != 0)
        {
            return g_result;
        }
    }
    return 0;
}
#endif
#if 0
//========================
void eq_callback(uint16 eq)
{
    g_music_vars.eq_type = eq;
    if (eq < 7)
    {
       EQMode_callback(g_music_vars.eq_type);
    }
    UpDisplayFlag = UpDisplayFlag | up_musiceqmode;
}

uint16 ui_EQ_menu(void)
{
    function_menu_t menu;
	uint16 eq_bak=g_music_vars.eq_type;
    if (g_music_vars.eq_type > 8)
    {
        menu.active = 0;
        eq_callback(0);
    }
    else
    {
        menu.active = g_music_vars.eq_type;
    }
    menu.total = 7;
    menu.menuhead = MPLAY2;
    menu.string = eq_string;
    ui_auto_update = FALSE;
    g_result = ui_function_menu(&menu,eq_callback);
    ui_auto_update = TRUE;
    StartIntro(FALSE);
    if (g_result != 0)
      {
    	 if (g_result == RESULT_IGNORE)
    	 	{
    	     eq_callback(eq_bak);
			 g_result=0;
    	 	}
         return g_result;
      }
    return 0;
}
#endif
