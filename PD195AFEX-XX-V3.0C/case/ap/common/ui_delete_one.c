/*
 *******************************************************************************
 *                ACTOS AP
 *        ap common lib, delete one file
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 * File: ui_delete.c
 * By  : stevenluo
 * Version: v1.00     first version     2010-04-09 12:00
 *******************************************************************************
 */

#include "ap_common.h"
#pragma name(COM_DELETE_ONE)
#if 0
/*######################################################################*/
/*#########################BANK内的常数段#############################*/
/*######################################################################*/
static const uint8 music_ext[][4] =
{ "MP3", "WMA", "WAV", "MP1", "MP2" };
static const uint8 amv_ext[][4] =
{ "AMV", "AVI" };
static const uint8 voice_ext[][4] =
{ "WAV" };
static const uint8 pic_ext[][4] =
{ "JPG", "BMP", "GIF" };
static const uint8 txt_ext[][4] =
{ "TXT" };
static const uint8 all_ext[][4] =
{ "*  " };
static const uint8 ExtResult[] =
{ FSEL_TYPE_MUSIC,FSEL_TYPE_VOICE, FSEL_TYPE_TXT, FSEL_TYPE_AMV, FSEL_TYPE_DSC, DIRECTORY_ALL };
static const uint8 ExtSize[] =
{ sizeof(music_ext) / 4,sizeof(voice_ext) / 4, sizeof(txt_ext) / 4, sizeof(amv_ext) / 4,
        sizeof(pic_ext) / 4, 0 
};
static const uint32 ExtPoint[] =
{ (uint32*) music_ext, (uint32*) voice_ext, (uint32*) txt_ext, (uint32*) amv_ext,
        (uint32*) pic_ext, (uint32*) all_ext 
};

static const uint16 delete_text[2] =
{ STEM_1, STEM_2 };
static const uint16 delete_text[2] =
{ STEM_1, STEM_2 };
#endif
uint16 delete_text[2] =
{ SLECTNO, SLECTYES };
uint16 del_play[2] =
{ JPEGSET2, VSTOPM3 };
/*######################################################################*/
/*#########################代码段#####################################*/
/*######################################################################*/

uint16 get_HeadID(uint8 file_type)
{
file_type = file_type;
return 0;
#if 0
    uint16 result;
    if (file_type == FSEL_TYPE_MUSIC)
    {
        result = T_MUSIC;
    }
    else if (file_type == FSEL_TYPE_TXT)
    {
        result = T_EBOOK;
    }
    else if (file_type == FSEL_TYPE_AMV)
    {
        result = T_MOVIE;
    }
    else if (file_type == FSEL_TYPE_DSC)
    {
        result = T_PHOTO;
    }
    else
    {
        result = T_BROWSER;
    }
    return result;
#endif
}
uint16 show_del_play(void)
{
     uint16 result;
     function_menu_t menu_music;
     menu_music.total = 2;        
     menu_music.menuhead =STOPMENU;
     menu_music.string = del_play;
     menu_music.active = 0;//
     menu_music.string_id = g_comval.langid;
     //ui_auto_select = TRUE;  //show menu
     result = ui_function_menu(&menu_music, NULL);
     //ui_auto_select = FALSE;
	 if(result == 0)
	 {
         if(menu_music.active == 0)
		 result = RESULT_MUSIC_PLAY;
	 }
     return result;

}
uint16 show_delete_menu(uint8 type)
{
#if 0//文件浏览可进行删除文件
     uint16 result;
     pop_up_list_t list;
     list.active = 0;
     list.total = 2; 
     list.strid = delete_text;
     list.headid = MAIN1;
	 ui_auto_select = FALSE;  
	 result = ui_popup_list(&list, NULL);
	 ui_auto_select = TRUE;
     if ((result == 0)&&(list.active == 0))
     {
        result = RESULT_IGNORE;
     }
    return result;
#endif
#if 1
     uint16 result;
     function_menu_t menu_music;
	 type = type;
     menu_music.total = 2;        
     menu_music.menuhead = DELFILE;
     menu_music.string = delete_text;
     menu_music.active = 0;//
     menu_music.string_id = g_comval.langid;
     //ui_auto_select = TRUE;  //show menu
     result = ui_function_menu(&menu_music, NULL);
     //ui_auto_select = FALSE;
     if ((result == 0)&&(menu_music.active == 0))
     {
        result = RESULT_IGNORE;
     }
     return result;
#endif
}
/********************************************************************************
 * Description : 获取当前文件后缀的总数
 *
 * Arguments  :
 *           	file_type:文件类型
 *#define FSEL_TYPE_MUSIC         0      //音乐文件
 *#define FSEL_TYPE_VOICE         1      //放音文件
 *#define DIRECTORY_ALL           2       //所有文件格式
 *#define FSEL_TYPE_DSC       3		  //图片文件
 *#define	FSEL_TYPE_TXT	    4         //电子书文件
 *#define	FSEL_TYPE_AMV	    5         //视频文件
 *#define	FSEL_TYPE_ALL	    6        //所有文件格式
 *#define	FSEL_TYPE_DIR       7         //文件夹
 *
 * Returns     :
 *            当前文件后缀的总数
 * Notes       :
 *
 ********************************************************************************/
uint8 Get_CurExtSize1(uint8 file_type)
{
#if 0
    uint8 i;
    ClearWatchDog();
    for (i = 0; i < sizeof(ExtResult); i++)
    {
        if (file_type == ExtResult[i])
        {
            return ExtSize[i];
        }
    }
#endif
file_type = file_type;
    return 0;
}
/********************************************************************************
 * Description : 获取当前文件的总后缀名
 *
 * Arguments  :
 *           	file_type:文件类型
 *#define FSEL_TYPE_MUSIC         0      //音乐文件
 *#define FSEL_TYPE_VOICE         1      //放音文件
 *#define DIRECTORY_ALL           2       //所有文件格式
 *#define FSEL_TYPE_DSC       3		  //图片文件
 *#define	FSEL_TYPE_TXT	    4         //电子书文件
 *#define	FSEL_TYPE_AMV	    5         //视频文件
 *#define	FSEL_TYPE_ALL	    6        //所有文件格式
 *#define	FSEL_TYPE_DIR       7         //文件夹
 *
 * Returns     :
 *            当前文件的总后缀名
 * Notes       :
 *
 ********************************************************************************/
uint8 * Get_CurExtStr1(uint8 file_type)
{
    file_type =file_type;
    return 0;

#if 0
    uint8 i;
    ClearWatchDog();
    for (i = 0; i < sizeof(ExtResult); i++)
    {
        if (file_type == ExtResult[i])
        {
            return (const uint8*) ExtPoint[i];
        }
    }
    return (const uint8*) ExtPoint[4];
#endif
}
uint16 Get_totalAndSelFile(uint8 *filename, uint8 file_type)
{
    filename = filename;
    file_type = file_type;
    return 0;

#if 0
    uint16 total;
    uint8 dir_type = FS_DIR_FILE;
    total = 0;
    do
    {
        if (FS_Dir(file_name, (const uint8*) Get_CurExtStr1(file_type), dir_type, Get_CurExtSize1(file_type)) == TRUE)
        {
            total++;
            ClearWatchDog();
            if (memcmp(file_name, filename, 11) == 0)
            {
                dir_fselFileNo = total;
            }
        }
        else
        {
            break;
        }
        dir_type = FS_DIR_FILE_CONT;
    } while (1);
    return total;
#endif
}
/********************************************************************************
 * Description : 根据索引号删除单个文件
 *
 * Arguments  :
 *		    selnum：索引号
 file_type：删除的文件类型
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************/
uint8 del_sel_file(uint8 file_type, uint16 selnum)
{
#if 1    
    uint8 temp_num = 0;
    int32 dirEntry;
    uint8 sfr_bak, mcsr9_bak;
    if (file_type != 0)
    {
    }
    temp_num = (uint8) (selnum - top - 1);
    DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
    dirEntry = OnePageRecord[temp_num].DirEntry;
    DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak)
    FS_SetCurDirEntry(dirEntry);
    if (!FS_FRemove(0))
    {
        return FALSE;
    }
#endif    
    return TRUE;
}

/********************************************************************************
 * Description : 初始化一些公共变量
 *
 * Arguments  :
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void init_commonPara(void)
{ 
  
    old_list_active = dir_list_active;
    top = (dir_list_active / GROUPDEFINE) * GROUPDEFINE;
    bottom = top + GROUPDEFINE - 1;
    if (bottom > (file_total_num - 1))
    {
        bottom = file_total_num - 1;
    }
    
}
/********************************************************************************
 * Description : 选择下一项时重新初始化选项相关变量
 *
 * Arguments  :
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void sel_nextActiveNum(void)
{
    if (file_total_num <= 1)
    {
        return;
    }
    if (dir_list_active < (file_total_num - 1))
    {
        dir_list_active++;
        if ((dir_list_active / GROUPDEFINE) == cur_group)
        {
            old_list_active = dir_list_active - 1;
            return;
        }
        else
        {
            if (selmode == 3)
            {
                selmode_changeflag = 1;
            }
            selmode = 2;
            cur_group = cur_group + 1;
            init_commonPara();
        }
    }
    else
    {
        selmode = 0;
        dir_list_active = 0;
        cur_group = 0;
        init_commonPara();
    }
}
/********************************************************************************
 * Description : 选择上一项时重新初始化选项相关变量
 *
 * Arguments  :
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void sel_prevActiveNum(void)
{
    if (file_total_num <= 1)
    {
        return;
    }
    if (dir_list_active > 0)
    {
        dir_list_active--;
        if ((dir_list_active / GROUPDEFINE) == cur_group)
        {
            old_list_active = dir_list_active + 1;
            return;
        }
        else
        {
            if ((selmode == 2) || (selmode == 0))
            {
                selmode_changeflag = 1;
            }
            selmode = 3;
            cur_group = cur_group - 1;
            init_commonPara();
        }
    }
    else
    {
        selmode = 1;
        dir_list_active = file_total_num - 1;
        cur_group = (file_total_num - 1) / GROUPDEFINE;
        init_commonPara();
    }
}
void GetLocationByName(file_location_t *location, uint8 *nameStr, uint16 tempNum, uint8 file_type)
{
    uint8 dir_type;
    uint16 temp_fileNo;
    dir_type = FS_DIR_FILE;
    temp_fileNo = 0;
    while (temp_fileNo < dir_fselTotal)
    {

        if (FS_Dir(file_name, Get_CurExtStr1(file_type), dir_type, Get_CurExtSize1(file_type)) == TRUE)
        {
            temp_fileNo++;
            dir_type = FS_DIR_FILE_CONT;
            ClearWatchDog();
            if (temp_fileNo == tempNum)
            {
                dir_fselFileNo = temp_fileNo;
                dir_temp_record.DirEntry = FS_GetCurDirEntry();
                memcpy(dir_temp_record.name, file_name, 12);
            }
            if (memcmp(file_name, nameStr, 11) == 0)
            {
                dir_fselFileNo = temp_fileNo;
                dir_temp_record.DirEntry = FS_GetCurDirEntry();
                memcpy(dir_temp_record.name, file_name, 12);
                break;
            }
        }
        else
        {
            if (temp_fileNo < tempNum)
            {
                dir_fselFileNo = temp_fileNo;
                dir_temp_record.DirEntry = FS_GetCurDirEntry();
                memcpy(dir_temp_record.name, file_name, 12);
            }
            break;
        }
    }
    memcpy(location->filename, dir_temp_record.name, 12);
    location->fselFileNo = dir_fselFileNo;
    location->fselTotal = dir_fselTotal;
    location->DirEntry = dir_temp_record.DirEntry;
}
/********************************************************************************
 * Description : 删除同一类型的单个文件的界面显示及消息处理
 *
 * Arguments  :
 *		    location：目录结构体
 file_type：删除的文件类型
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************/
uint16 ui_delete(file_location_t *location, uint8 type)
{
#if 1    
    uint8 need_draw = TRUE, need_decord = TRUE;
    uint8 key;
    uint16 result;
    uint8 temp_filename[12];
    uint16 temp_num;
    //  uint32 direntry;
    dir_fselInit(type, FSEL_ALL_REPEATALL, 0); //初始化当前磁盘的文件系统
    dir_fselSetLocation(location);
    //  dir_fselTotal = Get_totalAndSelFile(location->filename, type);
    dir_fselTotal = location->fselTotal;
    dir_fselFileNo = location->fselFileNo;
    temp_num = location->fselFileNo;
    file_total_num = dir_fselTotal;

    if (file_total_num == 0)
    {
//        ui_err_msg(DIRNOFILE);
        return RESULT_IGNORE;
    }
    memcpy(temp_filename, location->filename, 12);
    dir_fselType = type;
    if (dir_fselFileNo == 0)
    {
        dir_list_active = 0;
    }
    else
    {
        dir_list_active = dir_fselFileNo - 1;
    }
    cur_group = dir_list_active / ROW_COUNT_DEF;
    if (dir_fselFileNo > 6)
    {
        dir_temp_record.DirEntry = location->DirEntry;
        selmode = 4;
    }
    else
    {
        selmode = 0;
    }
    init_commonPara();
    while (ap_get_message() != 0)
    {
    
    } //丢掉多余按键
    ap_message_init(NULL);

    while (1)
    {
        //handle msg
        if (need_draw == TRUE)
        {
            if ((dir_list_active == old_list_active) && (need_decord == TRUE))
            {
                dir_total_num = 0;
                dir_DecodeOnePage(top + 1, bottom + 1);
                selmode_changeflag = 0;
            }
            dir_ShowDirectory(top, bottom, old_list_active, dir_list_active, file_total_num);
            need_draw = FALSE;
            need_decord = TRUE;
        }
        key = ap_get_message();
        switch (key)
        {
            case AP_KEY_NEXT:
            case AP_KEY_NEXT | AP_KEY_HOLD:
            sel_nextActiveNum();
            need_draw = TRUE;
            while (ap_get_message() != 0)
            {

            } //丢掉多余按键
            break;

            case AP_KEY_PREV:
            case AP_KEY_PREV | AP_KEY_HOLD:
            sel_prevActiveNum();
            need_draw = TRUE;
            while (ap_get_message() != 0)
            {

            } //丢掉多余按键
            break;

            case AP_MSG_WAIT_TIMEOUT:
            case AP_KEY_PLAY | AP_KEY_UP:
            GetLocationByName(location, temp_filename, temp_num, type);
            //  dir_fselGetLocation(location);
            return RESULT_IGNORE;
            case AP_KEY_MODE | AP_KEY_UP:
            result = show_delete_menu(type);
            if (result == 0)
            {
                result = del_sel_file(type, dir_list_active + 1);
                if (result == TRUE)
                {
                    dir_fselTotal--;
                    if (dir_fselTotal == 0)
                    {
//                        ui_err_msg(FILEOUT);
                        return RESULT_IGNORE;
                    }
                    file_total_num = dir_fselTotal;
                    if ((file_total_num - dir_fselFileNo) < ROW_COUNT_DEF)
                    {
                        selmode = 1;
                    }
                    else
                    {
                        selmode = 0;
                    }
                    if (dir_list_active >= file_total_num)
                    {
                        dir_list_active = file_total_num - 1;

                    }
                    cur_group = dir_list_active / ROW_COUNT_DEF;
                    dir_fselFileNo = dir_list_active + 1;
                    init_commonPara();
                    // dir_fselGetLocation(location);
                    need_draw = TRUE;
                    break;

                }
                else
                {
                   //  ui_err_msg(DELFAIL);
                    return RESULT_IGNORE;
                }
            }
            else if (result == RESULT_IGNORE)
            {
                cur_group = dir_list_active / ROW_COUNT_DEF;
                dir_fselFileNo = dir_list_active + 1;
                init_commonPara();
                need_draw = TRUE;
                need_decord = FALSE;
                break;
            }
            else
            {
                GetLocationByName(location, temp_filename, temp_num, type);
                return result;
            }

            case AP_MSG_SD_IN:
            break;

            default:
            key = ap_handle_hotkey(key);

            if (key == RESULT_REDRAW)
            {
                need_draw = TRUE;
            }
            else if (key != 0)
            {
                //    dir_fselGetLocation(location);
                GetLocationByName(location, temp_filename, temp_num, type);
                return key;
            }
            else
            {

            }
        } //switch()

        if (ScrollFlag == TRUE)
        {
            ScrollString(scroll_buf, FALSE, UNICODELANGUAGE, &region_scroll);
        }

    }//  while(1)
#endif
    return 1;
}

