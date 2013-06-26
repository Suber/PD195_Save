/*
 *******************************************************************************
 *                ACTOS AP
 *        ap common lib, delete all file
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 * File: ui_delete_all.c
 * By  : stevenluo
 * Version: v1.00     first version     2010-04-09 12:00
 *******************************************************************************
 */

#include "ap_common.h"

#pragma name(COM_DELETE_ALL)
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
{ FSEL_TYPE_MUSIC, FSEL_TYPE_VOICE, FSEL_TYPE_TXT, FSEL_TYPE_AMV, FSEL_TYPE_DSC, DIRECTORY_ALL };
static const uint8 ExtSize[] =
{ sizeof(music_ext) / 4, sizeof(voice_ext) / 4, sizeof(txt_ext) / 4, sizeof(amv_ext) / 4, sizeof(pic_ext) / 4, 0 };
static const uint32 ExtPoint[] =
{ (uint32*) music_ext, (uint32*) voice_ext, (uint32*) txt_ext, (uint32*) amv_ext, (uint32*) pic_ext,
        (uint32*) all_ext 
};
/*######################################################################*/
/*#########################代码段#####################################*/
/*######################################################################*/
#endif
/********************************************************************************
 * Description : 显示删除出错信息
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void show_del_fail(void)
{
#if 0    
    ResShowMultiString(DELFAIL, 16, 32); //删除失败界面显示
    ap_sleep(4);
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
uint8 Get_CurExtSize(uint8 file_type)
{
file_type = file_type;
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
uint8 * Get_CurExtStr(uint8 file_type)
{
file_type = file_type;
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
/********************************************************************************
 * Description : 定位到根目录
 *
 * Arguments  :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************/
uint8 SetToRoot(void)
{
    memset(dir_fselDirNo, 0x0, 8);
    if (!FS_CD(":"))
    {
        return FALSE;
    }
    dir_fselDirLayer = 0;
    return TRUE;
}
/********************************************************************************
 * Description : 删除同一类型的所有文件
 *
 * Arguments  :
 *		    location：目录结构体
 file_type：删除的文件类型
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************/
uint8 del_AllExt_files(file_location_t *location, uint8 file_type)
{
			   location->filename[0] = location->filename[0];
			   file_type =file_type;
			   return 0;
#if 0    
    //	uint8 DelStrname[12];
    //	uint16 i;
    uint8 dir_type;
    uint8 result = TRUE;
    long CurDirEntry = 0;
    while (1)
    {
        ClearWatchDog(); //清WATCH DOG
        // dir_fselGetNext(location->filename);
        if (!dir_fselGetNext(location->filename))
        {
            result = TRUE;
            break;
        }
        else
        {
            dir_type = FS_DIR_FILE;
            while (1)
            {
                if (FS_Dir(file_name, (const uint8*) Get_CurExtStr(file_type), dir_type, Get_CurExtSize(file_type))
                        == TRUE)
                {
                    if (!FS_FRemove(0))
                    {
                        return FALSE;
                    }
                    dir_type = FS_DIR_FILE_CONT;
                    ClearWatchDog();//清WATCH DOG
                }
                else
                {
                    break;
                }
            }
            dir_fselFileNo = 0;// dir_fselTotal;
            dir_fselTotal = 0;
        }
    }
    
    return result;
#endif
}
/********************************************************************************
 * Description : 删除同一类型的所有文件的界面显示及消息处理
 *
 * Arguments  :
 *		    location：目录结构体
 file_type：删除的文件类型
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************/
uint16 ui_delete_all(file_location_t *location, uint8 type)
{
			   location->filename[0] = location->filename[0];
			   type =type;
#if 0    
    uint8 need_draw = TRUE;
    uint8 delflag;
    uint8 key;
    file_location_t temp_location;
    memcpy(&temp_location, location, sizeof(file_location_t));
    dir_fselInit(type, FSEL_ALL_REPEATALL, 0); //初始化当前磁盘的文件系统
    //   location->path[0] = ':';
    //   dir_fselSetLocation(location);
    need_draw = TRUE;
    delflag = 0;
    ap_message_init(NULL);
    while (1)
    {
        if (need_draw == TRUE)
        {
            ResShowPic(MENUTWO, 0, 0);
            SetPenColor(Color_BULE2_def);
            SetBackgdColor(0x0000effe);
            ResShowMultiString(DELALL, 255, 100);
            ResShowPic(DELNO, 50, 60);
            need_draw = FALSE;
        }

        //handle msg
        key = ap_get_message(); //获取消息
        switch (key)
        {
            case AP_KEY_NEXT: //YES和NO之间切换
            case AP_KEY_PREV:
            case AP_KEY_NEXT | AP_KEY_HOLD:
            case AP_KEY_PREV | AP_KEY_HOLD:
            if (delflag == 0)
            {
                delflag = 1;
                ResShowPic(DELYES, 50, 60);
            }
            else
            {
                delflag = 0;
                ResShowPic(DELNO, 50, 60);
            }
            break;

            case AP_KEY_MODE | AP_KEY_UP:
            if (delflag == 1) //YES时按下MODE键的处理
            {

                ClearScreen(NULL);//清屏
                SetTextOutMode(0); //设字体不透明
                SetPenColor(Color_BULE2_def);
                ResShowMultiString(DELING, 255, 80);
                if (!del_AllExt_files(location, type))
                {
                    if ((location->disk == 'H') && (CardDetectState == 0))
                    {
                        ui_err_msg(CARDPULL);
                        return RESULT_SD_OUT;
                    }
                    else
                    {
                        ui_err_msg(DELFAIL);
                        return RESULT_IGNORE;
                    }
                }
                else
                {
                    dir_fselFileNo = 0;
                    SetToRoot();
                    strcpy(location->path, FS_CD_ROOT);
                    location->filename[0] = '\0';
                    location->ClusterNo = FS_GetCurDir();
                    location->dirLayer = 0;
                    memset(location->DirNo, 0, TOTALDIRLAYER);
                    ui_err_msg(FILEOUT);
                    //return RESULT_NULL;
                }
                return RESULT_IGNORE;
            }
            else
            {
                memcpy(location, &temp_location, sizeof(file_location_t));
                dir_fselSetLocation(location);
                return RESULT_IGNORE;//NO时按下MODE键的处理
            }

            case AP_MSG_WAIT_TIMEOUT: //8秒退出
            break;

            case AP_KEY_PLAY | AP_KEY_UP:
            memcpy(location, &temp_location, sizeof(file_location_t));
            dir_fselSetLocation(location);
            return RESULT_IGNORE;
            case AP_MSG_SD_IN:
            break;

            default:
            key = ap_handle_hotkey(key); //热键处理
            if (key == RESULT_REDRAW)
            {
                need_draw = TRUE;
            }
            else if (key != 0)
            {
                return key;
            }
            else
            {

            }
        }//switch(key)
    }//while(1)
#endif
    return 1;
}
