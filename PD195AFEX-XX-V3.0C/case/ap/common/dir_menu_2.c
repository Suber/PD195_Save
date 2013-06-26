/*
 *******************************************************************************
 *                ACTOS AP
 *        ap common lib, directory control file
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 * File: dir_menu_2.c
 * By  : stevenluo
 * Version: v1.00     first version     2010-04-09 12:00
 *******************************************************************************
 */

#include "ap_common.h"

#pragma name(DIR_MENU_2)
/*######################################################################*/
/*#########################BANK内的常数段#############################*/
/*######################################################################*/
static const uint8 Ui_IgnoreExt[4] =
{ "*  " };//文件格式识别数组
static const uint8 ExtMusic[][4] =
{ "MP3", "WMA", "WAV", "MP1", "MP2" };//音乐文件格式
static const uint8 ExtPicture[][4] =
{ "JPG", "BMP", "GIF" };//图片文件格式
static const uint8 ExtTXT[][4] =
{ "TXT" };//电子书文件格式
static const uint8 ExtVoice[][4] =
{ "WAV" };//录音文件格式
static const uint8 ExtVideo[][4] =
{ "AMV", "AVI" };//视频文件格式
static const uint8 dir_ExtAll[][4] =
{ "*  " };//所有所有文件格式
static const uint8 Extstr[][4] = //文件格式识别数组
        { "MP3", "WMA", "MP1", "MP2", "WAV", "TXT","AMV", "AVI","JPG", "BMP", "GIF" };
static const uint8 ExtResult[] =
{ FSEL_TYPE_MUSIC, FSEL_TYPE_VOICE, FSEL_TYPE_TXT, FSEL_TYPE_AMV, FSEL_TYPE_DSC, DIRECTORY_ALL };
static const uint8 ExtSize[] =
{ sizeof(ExtMusic) / 4, sizeof(ExtVoice) / 4, sizeof(ExtTXT) / 4, sizeof(ExtVideo) / 4, sizeof(ExtPicture) / 4, 0 };
static const uint32 ExtPoint[] =
{ (uint32*) ExtMusic, (uint32*) ExtVoice, (uint32*) ExtTXT, (uint32*) ExtVideo, (uint32*) ExtPicture,
        (uint32*) dir_ExtAll 
};
static const uint8 ExtNum[] =
{ 3,4, 5, 7, 10 };
static const uint8 ReturnResult[] =
{ RESULT_MUSIC_PLAY,RESULT_VOICE_PLAY,RESULT_READER_PLAY, RESULT_MOVIE_PLAY, RESULT_VIEW_PLAY  };

/*######################################################################*/
/*#########################代码段#####################################*/
/*######################################################################*/

/********************************************************************************
 * Description :根据盘符显示出错信息
 *
 ** Arguments  :
 *			type：当前的盘符0-FLASH，1-卡
 * Returns     :
 *			无
 * Notes       :
 *
 ********************************************************************************/
void show_info(uint8 type)
{
    if (type == 0)
    {
     //   ui_err_msg(DFILEOUT);
    }
    else
    {
    //    ui_err_msg(DFILEOUT); //卡无文件
    }
}

/********************************************************************************
 * Description :根据当前文件短各获取长名存储下来
 *
 *
 ** Arguments  :
 *			Namebuf：文件短名
 *			index：当前列表表项内容所对应的选项
 * Returns     :
 *			无
 * Notes       :
 *
 ********************************************************************************/
void GetLongNameAndSave(uint8 *Namebuf, uint8 index, uint8 firstindex)
{
    uint8 NameNum;
    uint8 i = 0, spcount = 0;
    uint8 file_longname[50];
    uint8 sfr_bak, mcsr9_bak;
    memset(file_longname, 0, 50);
    file_longname[0] = 0x20;
    if (firstindex != 0)
    {
        NameNum = 0;
    }
    else
    {
        NameNum = FS_GetName(file_longname, 24);
    }
    if (NameNum != 0)
    {
        DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
        memcpy(dirbuf_filename[index], file_longname, 50); //get one usful record
        dirbuf_longname_flag[index] = 1;
        DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak)

    }
    else
    {
        //对短名的文件显示进行处理,如果当前的名字小于八个字节
        //则把字符后面的空格去掉,然后在字符后面加上"."再把后缀加上
        if (dir_temp_record.filetype == TYPE_FILE)
        {
            for (i = 7; i > 0; i--)
            {
                if (Namebuf[i] == 0x20) //计算空格数
                {
                    spcount++;
                }
                else
                {
                    break;
                }

            }
            memcpy(file_longname, Namebuf, 8 - spcount);
            file_longname[8 - spcount] = 0x2e;
            memcpy(&file_longname[9 - spcount], &Namebuf[8], 4);
        }
        else
        {
            memcpy(file_longname, Namebuf, 12); //get one usful record
        }
        DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
        dirbuf_longname_flag[index] = 0;
        memcpy(dirbuf_filename[index], file_longname, 12);
        DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak)
    }
}
/********************************************************************************
 * Description :侦破获取到的一屏数据
 *
 *
 ** Arguments  :
 *			file：当前表项的文件短名
 *			index：当前列表表项内容所对应的选项
 *			file_type：当前选项所对应文件类型
 * Returns     :
 *			无
 * Notes       :
 *
 ********************************************************************************/
void stored_filename(uint8 *file, uint8 index, uint8 file_type, uint8 firstIndex)
{
    uint8 sfr_bak, mcsr9_bak;
    dir_temp_record.filetype = file_type;
    dir_temp_record.DirEntry = FS_GetCurDirEntry();
    memcpy(dir_temp_record.name, file, 12);
    GetLongNameAndSave(file, index, firstIndex); //保存长,短名字到 dir_temp_record
    DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
    memcpy(&OnePageRecord[index], &dir_temp_record, sizeof(file_record_m));
    DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak)
}

/********************************************************************************
 * Description :根据当前的文件类型获取同一类型文件格式的数量
 *
 *
 ** Arguments  :
 * Returns     :
 *			同一类型文件格式的数量
 * Notes       :
 *
 ********************************************************************************/
uint8 dir_GetExtSize_2(void)
{
    uint8 i;
    ClearWatchDog();
    for (i = 0; i < sizeof(ExtResult); i++)
    {
        if (dir_fselType == ExtResult[i])
        {
            return ExtSize[i];
        }
    }
    return 0;
}
/********************************************************************************
 * Description :根据当前的文件类型获取同一类型的文件格式
 *
 *
 ** Arguments  :
 * Returns     :
 *			同一类型的文件格式
 * Notes       :
 *
 ********************************************************************************/
const uint8 * dir_GetAllExtBuf_2(void)
{
    uint8 i;
    ClearWatchDog();
    for (i = 0; i < sizeof(ExtResult); i++)
    {
        if (dir_fselType == ExtResult[i])
        {
            return (const uint8*) ExtPoint[i];
        }
    }
    return (const uint8*) ExtPoint[4];
}

/********************************************************************************
 * Description :获取列表第一项的符号
 *
 *
 ** Arguments  :
 * Returns     :
 *			无
 * Notes       :
 *
 ********************************************************************************/
void get_firstindex_name(uint8 *filename)
{
    if (dir_fselDirLayer == 0)
    {
        memcpy(filename, "ROOT", 4);
        filename[4] = 0;
    }
    else
    {
        filename[0] = '\\';
        filename[1] = 0;
    }
}
/********************************************************************************
 * Description :获取当前一屏数据中需要显示的文件
 *
 *
 ** Arguments  :
 * Returns     :
 *			TRUE OR FALSE
 * Notes       :
 *
 ********************************************************************************/

uint8 dir_GetFile(uint16 dir_top, uint16 dir_bottom, uint16 i, uint8 mode)
{
    uint16 m, m1, j;
    uint16 temp_top = 1;
    uint8 numIndex;
    uint8 dir_type = FS_DIR_FILE;
    int32 dirEntry;
    uint8 sfr_bak, mcsr9_bak;
    if (dir_top <= (dir_total_num + 1))
    {
        mode = 0;
    }
    if (mode > 0)
    {
        temp_top = i;
        dir_type = FS_DIR_FILE_CONT;
        //从当前目录的第一个文件切换到最后一个时，
        //直接调用FS_DirLast，定位到当前目录尾。
        if (mode == 1)
        {
            FS_DirLast();
        }
        //如果在浏览文件时，切换方向改变时，
        //则DIR 的文件需要+或-（row_count_def-1）项
        if (selmode_changeflag == TRUE)
        {
            DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
            if (mode == 2)
            {
                dirEntry = OnePageRecord[ROW_COUNT_DEF - 1].DirEntry;
            }
            else
            {
                dirEntry = OnePageRecord[0].DirEntry;
            }
            DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak)
            FS_SetCurDirEntry(dirEntry);
        }
        if (mode == 4)
        {
            dirEntry = dir_temp_record.DirEntry;
            FS_SetCurDirEntry(dirEntry);
            for (j = temp_top; j <= (dir_list_active + 1); j++)
            {
                if (!FS_DirPrev(file_name, (const char*) dir_GetAllExtBuf_2(), dir_type, dir_GetExtSize_2()))
                {
                    return 0;
                }
            }
            mode = 2;
        }
    }
    for (m = temp_top; m <= dir_bottom; m++)
    {
        //往前浏览当前目录下的文件
        if ((mode == 1) || (mode == 3))
        {
            if (!FS_DirPrev(file_name, (const char*) dir_GetAllExtBuf_2(), dir_type, dir_GetExtSize_2()))
            {
                return 0;
            }
        }
        else //往后浏览当前目录下的文件
        {
            if (!FS_Dir(file_name, (const char*) dir_GetAllExtBuf_2(), dir_type, dir_GetExtSize_2()))
            {
                return 0;
            }
        }
        ClearWatchDog();
        if (mode == 0)
        {
            dir_type = FS_DIR_FILE_CONT;
        }
        if (dir_top < dir_total_num)
        {
            m1 = 1;
        }
        else
        {
            m1 = dir_top - dir_total_num;
        }

        if (m >= m1)
        {
            *(file_name + 11) = '\0';
            //往前浏览当前目录下的文件时，
            //存储一屏数据的顺序与往后浏览时相反的。
            if ((mode == 1) || (mode == 3))
            {
                numIndex = (uint8) ((dir_bottom - dir_top) - (i - temp_top));
            }
            else
            {
                numIndex = (uint8) (i - dir_top);
            }
            stored_filename(file_name, numIndex, TYPE_FILE, 0); //将dir_temp_record的记录数据保存到idm中
            i++;
            if (i > dir_bottom)
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

/********************************************************************************
 * Description :获取当前一屏数据中需要显示的目录
 *
 *
 ** Arguments  :
 * Returns     :
 *			TRUE OR FALSE
 * Notes       :
 *
 ********************************************************************************/
uint16 dir_GetDir(uint16 dir_top, uint16 dir_bottom, uint16 i)
{
    uint16 m, tmp;
    uint8 dir_type;
    dir_type = FS_DIR_DIR;
    tmp = i;
    for (m = 1; m <= dir_total_num; m++)
    {

        if (!FS_Dir(file_name, Ui_IgnoreExt, dir_type, 0))
        {
            return FALSE;
        }
        ClearWatchDog();
        dir_type = FS_DIR_DIR_CONT;
        if ((m == tmp) && (i <= dir_total_num))
        {
            stored_filename(file_name, (uint8) (i - dir_top), TYPE_DIR, 0); //将dir_temp_record的记录数据保存到idm中
            i++;

            if ((i > dir_total_num) || (i > dir_bottom))
            {
                return i;
            }
            tmp = i;
        }
    }
    return i;
}
/********************************************************************************
 * Description :解码获取一屏的数据
 *
 *
 ** Arguments  :
 * Returns     :
 *			TRUE OR FALSE
 * Notes       :
 *
 ********************************************************************************/
uint8 dir_DecodeOnePage(uint16 dir_top, uint16 dir_bottom)
{
    uint16 i;
    uint16 tmp;
    uint8 sfr_bak, mcsr9_bak;
    uint8 result;
    DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
    memset(dirbuf_filename, 0, 50 * ROW_COUNT_DEF);
    DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak)
    i = dir_top;
    while (i <= dir_bottom)
    {
        if (i > dir_total_num) //文件
        {
            result = dir_GetFile(dir_top, dir_bottom, i, selmode);
            break;
        }
        else //目录
        {
            tmp = i;
            if (tmp > 0)
            {
                tmp = dir_GetDir(dir_top, dir_bottom, i);
                if (tmp == FALSE)
                {
                    result = FALSE;
                    break;
                }
                else
                {
                    i = tmp;
                }
            }
            else
            {
                get_firstindex_name(file_name);
                stored_filename(file_name, (uint8) (i - dir_top), TYPE_DIR, 1); //将dir_temp_record的记录数据保存到idm中
                i++;
            }
        }
    }
    return result;
}
/********************************************************************************
 * Description :根据不同的文件类型返回相应的消息处理值
 *
 *
 ** Arguments  :
 * Returns     :
 *			相应的消息处理值
 * Notes       :
 *
 ********************************************************************************/
uint8 dir_File_Type(uint8 * filename)
{
    uint8 ext[4];
    uint8 i, j;
    memcpy(ext, filename + 8, 3);

    ext[0] = ((ext[0] >= 'a') && (ext[0] <= 'z')) ? (ext[0] - 'a' + 'A') : ext[0];
    ext[1] = ((ext[1] >= 'a') && (ext[1] <= 'z')) ? (ext[1] - 'a' + 'A') : ext[1];
    ext[2] = ((ext[2] >= 'a') && (ext[2] <= 'z')) ? (ext[2] - 'a' + 'A') : ext[2];
    ext[3] = 0;
    for (i = 0; i < (sizeof(Extstr) / 4); i++)
    {
        if (strcmp(ext, Extstr[i]) == 0)
        {
            for (j = 0; j < 5; j++)
            {
                if (i <= ExtNum[j])
                {
                    if (dir_fselType == DIRECTORY_ALL)
                    {
                        dir_fselType = ExtResult[j];
                    }
                    return ReturnResult[j];
                }
            }
        }
    }
    return NULL;
}

