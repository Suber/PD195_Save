/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: stop menu module
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"
extern Open_param_t* m_deOpenparam;
extern uint8 StopMenuactive; //保持选中菜单的序号
extern uint16 PlaylistCurClass; //1 ~ ClassItemTotal

#pragma name(APC_UI_MNU_STOP)

/*定义菜单项的字符ID号数组*/
const uint16 dir_string_card[5] =
{ MSTOP1, MSTOP2, MSTOP3, MSTOP4, MSTOP5 };

const uint16 dir_string[4] =
{ MSTOP1, MSTOP3, MSTOP4, MSTOP5 };

uint16 ui_stop_menu(void)
{
    uint16 result;
    function_menu_t menu;
    bool tmpflag = FALSE;
    bool card_exist = FALSE;  
    if (DRV_DetectUD(1) == 0x20)
    {
        //有卡时的显示
        menu.menuhead = MAIN1;
        menu.total =sizeof(dir_string_card) / 2;
        menu.string = dir_string_card;
        card_exist = TRUE;
    }
    else
    {   
        menu.menuhead = MAIN1;
        menu.total = sizeof(dir_string) / 2;
        menu.string = dir_string;
        if (StopMenuactive >= menu.total)
        {
            StopMenuactive--;
        }
    }
     
    menu.active = (uint16)StopMenuactive;
    //ui_auto_select = FALSE;
    result = ui_function_menu(&menu, NULL);
    //ui_auto_select = TRUE;
    StopMenuactive = menu.active;
    if (result != 0)
    {
        return result;
    }
    {

        //后面是以前的目录及删除选择
        if (DRV_DetectUD(1) == 0x20)
        {
            //有卡的情况
            switch (menu.active)
            {

                case 0: //主目录
                if ((g_music_vars.location.disk == 'H') || (g_music_vars.fselmod == FSEL_TYPE_USERPL))
                {
                    g_music_vars.fselmod = FSEL_TYPE_COMMONDIR;
                    g_music_vars.location.disk = 'C';
                    g_music_vars.location.fselTotal = 0;
                    FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, FSEL_TYPE_COMMONDIR, (uint8) g_music_vars.location.disk);
                    fselGetLocation(&g_music_vars.location);
                }
                result = ui_directory(&g_music_vars.location, g_MusicType);
                FSELSetMode(FSEL_ALL_SEQUENCE);
                if (result == RESULT_IGNORE)
                {
                    fselSetLocation(&g_music_vars.location);
                }
                break;
                case 1: //插卡目录
                if ((g_music_vars.location.disk == 'C') || (g_music_vars.fselmod == FSEL_TYPE_USERPL))
                {
                    g_music_vars.location.disk = 'H';
                    strcpy(g_music_vars.location.path, FS_CD_ROOT);
                    g_music_vars.fselmod = FSEL_TYPE_COMMONDIR;
                    g_music_vars.location.fselTotal = 0;
                    result = FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, FSEL_TYPE_COMMONDIR,
                            (uint8) g_music_vars.location.disk);
                    if (!result)
                    {
                        g_DiskStateFlag = g_DiskStateFlag & CLRCARDOKFLAG_DEF;
                        g_music_vars.location.disk = 'C';
                        DisplayErrInfor(CREADERR);//显示卡错
                        break;
                    }
                    g_DiskStateFlag = g_DiskStateFlag | SETCARDOKFLAG_DEF;
                    fselGetLocation(&g_music_vars.location);
                    tmpflag = TRUE;

                }
                result = ui_directory(&g_music_vars.location, g_MusicType);
                FSELSetMode(FSEL_ALL_SEQUENCE);
                if (result == 0)
                {
                    if (tmpflag != 0)
                    {
                        tmpflag = FALSE;
                    }
                    g_music_vars.location.APE_Cur = 0;
                }

                if (result == RESULT_SD_OUT)
                {
                    //DisplayErrMsg(CARDPULL);//显示卡拔出


                    g_music_vars.location.disk = 'C';
                    result = RESULT_IGNORE;

                }
                break;

                case 2: //删除文件
                result = ui_delete(&g_music_vars.location, g_MusicType);
                if (RESULT_IGNORE == result)
                {
                    fselSetLocation(&g_music_vars.location);

                }
                else
                {
                    FSELGetNext(g_music_vars.location.filename);
                }
                break;
                case 3: //删除全部
                g_music_vars.fselmod = FSEL_TYPE_COMMONDIR;
                result = ui_delete_all(&g_music_vars.location, g_MusicType);
                g_music_vars.location.fselTotal = 0;
                break;
                default://退出
                result = RESULT_IGNORE;
                break;
            }
        }
        else
        {
            //无卡的情况
            switch (menu.active)
            {
                case 0: //主目录
                if (g_music_vars.location.disk == 'H')
                {
                    g_music_vars.location.disk = 'C';
                    FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, FSEL_TYPE_COMMONDIR, (uint8) g_music_vars.location.disk);
                    g_music_vars.fselmod = FSEL_TYPE_COMMONDIR;
                }
                result = ui_directory(&g_music_vars.location, g_MusicType);
                FSELSetMode(FSEL_ALL_SEQUENCE);
                if (result == 0)
                {
                    g_music_vars.location.APE_Cur = 0;
                }
                break;

                case 1: //删除文件
                result = ui_delete(&g_music_vars.location, g_MusicType);
                if (RESULT_IGNORE == result)
                {
                    fselSetLocation(&g_music_vars.location);

                }
                else
                {
                    FSELGetNext(g_music_vars.location.filename);
                }
                break;
                case 2: //删除全部
                FSELSetMode(FSEL_ALL_REPEATALL);
                g_music_vars.fselmod = FSEL_TYPE_COMMONDIR;
                result = ui_delete_all(&g_music_vars.location, g_MusicType);
                FSELSetMode(g_music_vars.repeat_mode);
                break;

                default://退出
                result = RESULT_IGNORE;
                break;
            }
        }
        return result;
    }
}

