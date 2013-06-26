/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: sub_callback module of music
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"

#pragma name(AP_SUB_CBK)

#define EQCOUNT_DEF 8

/*Speaker 模式下，设置特殊EQ模式，减小瞬态电流*/
//#define SPEAKEREQ      
extern uint8 SRSModeFlag;
const uint16 eq_string[7] =
{ MEQ1, MEQ2, MEQ3, MEQ4, MEQ5, MEQ6, MEQ7 };
const uint8 EQtemp[8][10] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //none
    5, 3, 0, -1, 0, 4, 5, 6, 0, 0, //rock
    3, 0, 0, 0, 0, -1, -2, 6, 0, 0, //Pop
    0, 0, 0, -1, -1, -2, -4, 6, 0, 0, //Classic
    0, 0, 1, 2, 0, 1, 1, 6, 0, 0, //Soft
    3, 0, 0, -1, 0, 2, 4, 6, 0, 0, //Jazz
    5, 2, 0, 0, 0, 0, 0, 6, 0, 0, //Dbb
    -9, 0, 0, 0, 0, 0, 0, 6, 0, 0 //Speaker
};

//const uint16 eq_string[] =
//{ NATURAL, ROCK, POP, CLASSIC, SOFT, JAZZ, DBB };

//const uint16 srsuser_string[] =
//{ SRS3D, SRSTBAS, SRSFOCUS, EXIT };
//const uint16 srs_string[] =
//{ SRSWOW, SRSUSER, DISABSRS };
void srs_default_set2(void);
/*音量回调函数*/

/*停止界面显示音量调节 界面*/
void show_volume_menu(void)
{
#if 0
    slider.min = 0;
    slider.max = g_comval.VolumeMax;
    slider.value = g_music_vars.vol_display;
    slider.step = 1;
    slider.title = VOLCHANGE;
    ui_auto_update = FALSE;
    g_result = (uint16) ui_slider(&slider, NULL);
    g_music_vars.vol_display = (uint8) slider.value;
    g_music_vars.volume = (uint8) (slider.value * SOFTVOL_MAX) / g_comval.VolumeMax;//退出时再改变音量的值
    ui_auto_update = TRUE;
#endif
}

void srs_default_set(void)
{
    uint8 temp[12];
    g_music_vars.eq_type = SRS;

    temp[0] = 8;
    temp[1] = 8;
    temp[2] = 5;
    temp[3] = 4;
    temp[4] = 6;
    temp[5] = 1;
    temp[6] = 0;
    temp[7] = 30;
    temp[8] = 1;
    temp[9] = 0;
    temp[10] = 0;
    temp[11] = 0;

    musicpSendCommand(MC_SETUSEREQ, (void*) temp);
    musicpSendCommand(MC_SETEQ, 0x4f);
}
void EQMode_callback(uint8 value)//解决回调时菜单无法滚动的问题。
{
    uint8 temp[12];
	BYTE music_type;
	
    memset(temp, 0, 12);
    g_music_vars.eq_type = (uint8) value;
	switch (g_currentMusicType)
    {
        case Music_MP3: //mp3
		music_type=TYPE_MP3;
        break;
        case Music_WMA: //wma
        case Music_WMV: //wmv
        music_type=TYPE_WMA;
        break;
        case Music_Wav:
		music_type=TYPE_PCM;
        default:
        break;
    }
    if (value == 0)
    {
       // musicpSendCommand(MC_SETEQ, 0);
	   apSendCommand(MC_SETEQ,music_type, (void*) 0);
    }
    else
    {
        #ifdef 	SPEAKEREQ
	    /*无耳机插入，并且支持Class D，设置Speaker的音效，减小瞬态电流*/
		if((sKY_GetEarState() == 0) && (g_comval.ClassDFlag != 0))
	    {
			memcpy(temp, (void*) EQtemp[7], 10);
	    }
		else
		{
            memcpy(temp, (void*) EQtemp[value], 10);
		}
		#else
	    memcpy(temp, (void*) EQtemp[value], 10);
		#endif
      //  musicpSendCommand(MC_SETUSEREQ, (void*) temp);
		 apSendCommand(MC_SETUSEREQ,music_type, (void*) temp);
     //   musicpSendCommand(MC_SETEQ, 0x07);
	     apSendCommand(MC_SETEQ,music_type, (void*) 0x07);
    }
}



   
  
uint16 ui_SRS_menu(void)
{
#if 0
    pop_up_list_t list_playmenu;
    list_playmenu.strid = srs_string;
    list_playmenu.total = 3;
    list_playmenu.active = SRSModeFlag;
    if(g_MusicType == FSEL_TYPE_VOICE)
    {
        list_playmenu.headid = T_VOICE;//play_string[list.active]; 
    }
    else
    {      
        list_playmenu.headid = T_MUSIC;//play_string[list.active];
    }
    srs_callback(SRSModeFlag);
    g_result = ui_popup_list(&list_playmenu, srs_callback);
    if (g_result == 0)
    {
        if (list_playmenu.active == 0)
        {
            srs_default_set2();
            SRSModeFlag = 0;
            return NULL;
        }
        else if (list_playmenu.active == 1)
        {
            g_result = (uint16) ui_SRSUSER_menu();
            SRSModeFlag = 1;
        }
        else// if (list_playmenu.active == 2)
        {
            eq_callback(0);
            return NULL;
        }
    }
    if (g_result == RESULT_IGNORE)
    {
        return 0;
    }
    if (g_result != 0)
    {
        return g_result;
    }
#endif
#if 0
    function_menu_t menu;
    menu.active = g_music_vars.SRSModeFlag;
    menu.total = 2;
    menu.menuhead = SRS3D;
    menu.string = eq_string;
    // menu.headid=MPLAY2;
    srs_callback(SRSModeFlag);
    ui_auto_update = FALSE;
    g_result = ui_function_menu(&menu,srs_callback);
    ui_auto_update = TRUE;
#endif
    return 0;
}
uint16 ui_SRSUSER_menu(void)
{
#if 0
    uint8 save_active = 0;
    slider_t slider_playmenu;
    pop_up_list_t list_playmenu;

    while (1)
    {
        list_playmenu.strid = srsuser_string;
        list_playmenu.total = 4;
        list_playmenu.active = save_active;
       
        if(g_MusicType == FSEL_TYPE_VOICE)
        {
            list_playmenu.headid = T_VOICE;//SRSUSER;              //play_string[menu.active];
        }
        else
        {    
            list_playmenu.headid = T_MUSIC;//SRSUSER;              //play_string[menu.active];
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
        switch (list_playmenu.active)
        {
            case 0:
            slider_playmenu.max = 10;
            slider_playmenu.min = 0;
            slider_playmenu.step = 1;
            slider_playmenu.value = g_music_vars.SRSMod.Wow;

            slider_playmenu.title = srsuser_string[list_playmenu.active];
            g_result = ui_slider(&slider_playmenu, wow_user_callback);
            save_active = 0;
            break;

            case 1:
            {
                slider_playmenu.max = 10;
                slider_playmenu.min = 0;
                slider_playmenu.step = 1;
                slider_playmenu.value = g_music_vars.SRSMod.Bass;

                slider_playmenu.title = srsuser_string[list_playmenu.active];
                g_result = ui_slider(&slider_playmenu, bass_user_callback);
                save_active = 1;
            }
            break;
            case 2:
            {
                slider_playmenu.max = 10;
                slider_playmenu.min = 0;
                slider_playmenu.step = 1;
                slider_playmenu.value = g_music_vars.SRSMod.Srs;

                slider_playmenu.title = srsuser_string[list_playmenu.active];
                g_result = ui_slider(&slider_playmenu, srs_user_callback);
                save_active = 2;
            }
            break;

            case 3:
            g_result = RESULT_IGNORE;
            return g_result;
            default:
            break;
        }

        if ((g_result == 0) || (g_result == RESULT_IGNORE))
        {

        }
        else
        {
            return g_result;
        }
    }
#endif
    return 1;
}

 #if 1
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
    //ui_auto_select = FALSE;
    g_result = ui_function_menu(&menu,eq_callback);
    //ui_auto_select = TRUE;
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
//==================
#endif
void srs_callback(uint16 srs)
{
    //实时改变EQ的值
    uint8 temp1[12];
    memset(temp1, 0x00, 12);
    if (srs == 0)
    {
        srs_default_set2();
        g_music_vars.SRSModeFlag = 0;
    }
    else if (srs == 1)
    {
        g_music_vars.eq_type = SRS;//user srs
        memcpy(temp1, &g_music_vars.SRSMod, sizeof(SRSMod_t));
        musicpSendCommand(MC_SETUSEREQ, (void*) temp1);
        g_music_vars.SRSModeFlag = 1;
        musicpSendCommand(MC_SETEQ, 0x4f);
    }
    else
    {
        musicpSendCommand(MC_SETEQ, 0);
        musicpSendCommand(MC_SETUSEREQ, (void*) temp1);
        ap_sleep(2);
        g_music_vars.eq_type = 0;
    }
    UpDisplayFlag = UpDisplayFlag | up_musiceqmode;
}

void srs_user_callback(uint16 srs_user)
{
    uint8 temp[10];
    g_music_vars.eq_type = SRS;
    g_music_vars.SRSMod.Srs = (uint8) srs_user;

    memcpy(temp, &g_music_vars.SRSMod, sizeof(SRSMod_t));
    musicpSendCommand(MC_SETUSEREQ, (void*) temp);
    musicpSendCommand(MC_SETEQ, 0x4f);

}

void wow_user_callback(uint16 wow_user)
{
    uint8 temp[10];
    g_music_vars.eq_type = SRS;
    g_music_vars.SRSMod.Wow = (uint8) wow_user;

    memcpy(temp, &g_music_vars.SRSMod, sizeof(SRSMod_t));
    musicpSendCommand(MC_SETUSEREQ, (void*) temp);
    musicpSendCommand(MC_SETEQ, 0x4f);
}

void bass_user_callback(uint16 bass_user)
{
    uint8 temp[10];
    g_music_vars.eq_type = SRS;
    g_music_vars.SRSMod.Bass = (uint8) bass_user;

    memcpy(temp, &g_music_vars.SRSMod, sizeof(SRSMod_t));
    musicpSendCommand(MC_SETUSEREQ, (void*) temp);
    musicpSendCommand(MC_SETEQ, 0x4f);
}
void center_user_callback(uint16 center_user)
{
    uint8 temp[10];
    g_music_vars.eq_type = SRS;
    g_music_vars.SRSMod.Center = (uint8) center_user;

    memcpy(temp, &g_music_vars.SRSMod, sizeof(SRSMod_t));
    musicpSendCommand(MC_SETUSEREQ, (void*) temp);
    musicpSendCommand(MC_SETEQ, 0x4f);
}

void definition_user_callback(uint16 definition)
{
    uint8 temp[10];
    g_music_vars.eq_type = SRS;
    g_music_vars.SRSMod.Definition = (uint8) definition;

    memcpy(temp, &g_music_vars.SRSMod, sizeof(SRSMod_t));
    musicpSendCommand(MC_SETUSEREQ, (void*) temp);
    musicpSendCommand(MC_SETEQ, 0x4f);
}

void speakersize_user_callback(uint16 speakersize)
{
    uint8 temp[10];
    g_music_vars.eq_type = SRS;
    g_music_vars.SRSMod.Speakersize = (uint8) speakersize;

    memcpy(temp, &g_music_vars.SRSMod, sizeof(SRSMod_t));
    musicpSendCommand(MC_SETUSEREQ, (void*) temp);
    musicpSendCommand(MC_SETEQ, 0x4f);
}

void limiter_user_callback(uint16 limiter)
{
    uint8 temp[10];
    g_music_vars.eq_type = SRS;
    g_music_vars.SRSMod.Limiter = (uint8) limiter;

    memcpy(temp, &g_music_vars.SRSMod, sizeof(SRSMod_t));
    musicpSendCommand(MC_SETUSEREQ, (void*) temp);
    musicpSendCommand(MC_SETEQ, 0x4f);
}

void srs_default_set2(void)
{
    uint8 temp[12];
    g_music_vars.eq_type = SRS;

    temp[0] = 8;
    temp[1] = 8;
    temp[2] = 4;
    temp[3] = 5;
    temp[4] = 0;
    temp[5] = 1;
    temp[6] = 0;
    temp[7] = 30;
    temp[8] = 1;
    temp[9] = 0;
    temp[10] = 0;
    temp[11] = 0;
    musicpSendCommand(MC_SETUSEREQ, (void*) temp);
    musicpSendCommand(MC_SETEQ, 0x4f);
}

#ifdef OPT_Sup_FselFolder_fun
void fselget_callback( uint16 fselget )
{
    g_music_vars.fselget_type = fselget;
}
#endif

/*===========================================
 functions: OutPutVolume(uint8 vol)
 input:    uint8 vol: 要输出的音量
 output:   void
 功能： 把音量值送 到IO口，当在EQ变化时作出补尝
 ============================================*/
void OutPutVolume(uint8 vol)
{
    vol = vol;
    volume_callback(g_music_vars.vol_display);
}

