/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: sub functions 3 of music
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"

#pragma name(AP_SUB_FUN3)
const uint8 RootDirName[] = "ROOT";
/*
 *****************************************************************************
 ** 名字: 显示输入 id 的资源内容
 ** 接口: void DisplayErrInfor(uint16 id)
 ** 描述:
 ** 输入参数: uint16 id:要显示的资源号
 ** 输出参数: void
 ** 使用说明:
 *******************************************************************************
 */
void make_dirname(void)
{

    //uint16 result;
    //列表播放模式返回
    fselGetLocation(&g_music_vars.location);
/*
    if (g_music_vars.location.path[0] == ':')
    {
        strcpy(dir_name, RootDirName);
    }
    else
    {
        uint8 Backbuf[32];
        FS_Backup(Backbuf, FS_BACKUP_DIRINFO, 0);//目录项备份
        FS_CD("\\");
        memcpy(&dir_name[2], g_music_vars.location.path, 12);
        result = (uint16) FS_GetName(&dir_name[2], 13); //取出长名
        if (result != 0)
        {
            //标记Unicode小端
            dir_name[0] = 0xff;
            dir_name[1] = 0xfe;
        }
        else
        {
            memcpy(dir_name, g_music_vars.location.path, 12);
            dir_name[8] = 0;
            dir_name[9] = 0;
        }
        FS_Restore(Backbuf, FS_RESTORE_DIRINFO, 0);//目录项恢复
    }
*/
}

void DisplayErrInfor(uint16 id)
{
    ClearScreen(NULL);//清屏
    ui_auto_update = TRUE;
    ResShowMultiString(id, 255, 16);//显示磁盘无文件
    UpdateScreen(NULL);//清屏
    while (ap_get_message() != NULL)
        ;//清空消息池,确保DELAY时间准确
    ap_sleep(4);
}

/******************************************************************************
 ** 名字:  文件选择器初始化及报错显示
 ** 接口:  uint16 InitFileSelector(void)
 ** 描述:  把文件选择器初始化到有效的盘上，如找不到，则报错退出
 ** 输入参数: void
 ** 输出参数: 0: 没有有效的盘或盘上无文件，初始化失败，要求退出AP
 1: 已完成初始化文件选择器动作
 ** 使用说明:
 ********************************************************************************/
uint16 InitFileSelector(void)
{
    if (g_music_vars.location.disk == 'H')
    {
        /*查卡检测并初始化查卡盘*/
        if (DRV_DetectUD(1) == 0x20)
        {
            if (FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, g_music_vars.fselmod, 'H') != 0)
            {
                return 1;
            }
            else
            {
                DisplayErrInfor(CREADERR);//显示卡错
            }

        }
        /*初始化插卡盘失败，检测Flash磁盘*/
        g_music_vars.location.disk = 'C';

        if (FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, g_music_vars.fselmod, 'C') != 0)
        {
            return 1;
        }
        else
        {
            DisplayErrInfor(MREADERR);//显示磁盘错
            return 0;
        }
    }
    else /*g_music_vars.location.disk == 'C' 或其他*/
    {
        if (FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, g_music_vars.fselmod, 'C') != 0)
        {
            g_music_vars.location.disk = 'C';
            return 1;
        }
        else
        {
            DisplayErrInfor(MREADERR);//显示卡错
        }
        /*Flash 磁盘初始化失败，初始化插卡盘*/
        if (DRV_DetectUD(1) == 0x20)
        {
            g_music_vars.location.disk = 'H';
            if (FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, g_music_vars.fselmod, 'H') != 0)
            {
                return 1;
            }
            else
            {
                DisplayErrInfor(CREADERR);//显示卡错
                return 0;//非法
            }
        }
        return 0;//没卡退出
    }
}

/*
 *****************************************************************************
 ** 名字:
 ** 接口: uint16 UpdateFileSelector(void)
 ** 描述: 更新文件选择器
 ** 输入参数: void
 ** 输出参数:
 0: 更新不成功，要退出AP
 1: 更新成功，已可用
 ** 使用说明:
 *******************************************************************************
 */
uint16 UpdateFileSelector(void)
{
    uint8 result = FSELSetLocation(&g_music_vars.location);
    if (result == 1)
    {
        /*找到的文件，而不是非目录首*/
        if (g_music_vars.location.filename[0] != '\0')
        {
            return 1;
        }
        /*找到目录的第一个文件*/
        if (StartFileSelector() == 0)
        {
            return 0;
        }
	  
       return 1; 
    }

    /*result == 0*/
    ClearTimePara();//时间断点参数从0开始
    /*找不到对应文件时要回到目录第一个文件*/
    g_music_vars.location.filename[0] = '\0';
    result = FSELSetLocation(&g_music_vars.location);
    if (result != 0)
    {
        /*找当前目录的第一次文件*/
        if (StartFileSelector() == 0)
        {
            return 0;
        }
    }
    else
    {
        /*切换目录和磁盘重新检索文件*/
        if (InitFileSelector() == 0)
        {
            return 0;
        }
        if (StartFileSelector() == 0)
        {
            return 0;
        }
	
    }
    return 1;
}

/******************************************************************************
 ** 名字:
 ** 接口:
 ** 描述:
 ** 输入参数:
 ** 输出参数:
 ** 使用说明: //注，这函数是在初始化完文件选择器或set 完目录后调用的
 ********************************************************************************/
uint16 StartFileSelector(void)
{
    if (g_music_vars.location.disk == 'C')
    {
        if (FSELGetNext(g_music_vars.location.filename) != 0)
        {
            return 1;
        }
        /*从根目录开始重新搜索文件*/
        if (InitFileSelector() == 0)
        {
            return 0;
        }
        if (FSELGetNext(g_music_vars.location.filename) != 0)
        {
            return 1;
        }

        if (g_music_vars.location.disk == 'H')
        {
            DisplayErrInfor(CFILEOUT);//显示卡无文件
            g_music_vars.location.disk = 'C';
            return 0;
        }
        DisplayErrInfor(DFILEOUT);//显示磁盘无文件
        /*检测插卡盘有无文件*/
        g_music_vars.location.disk = 'H';
        if (DRV_DetectUD(1) != 0x20)
        {
            g_music_vars.location.disk = 'C';
            return 0;
        }
        if (FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, g_music_vars.fselmod, 'H') != 0)
        {
            if (FSELGetNext(g_music_vars.location.filename) != 0)
            {
                return 1;//找到文件
            }
            DisplayErrInfor(CFILEOUT);//显示卡无文件
        }
        return 0;
    }
    else //g_music_vars.location.disk == 'H'
    {
        if (FSELGetNext(g_music_vars.location.filename) != 0)
        {
            return 1;//找到文件
        }
        /*再查一次,因为可能这目录前面还有文件(虽然后面已无文件)*/
        if (InitFileSelector() == 0)
        {
            return 0;
        }
        if (FSELGetNext(g_music_vars.location.filename) != 0)
        {
            return 1;//找到文件
        }

        if (g_music_vars.location.disk == 'C')
        {
            DisplayErrInfor(DFILEOUT);//显示磁盘无文件
            return 0; //因为前面已从 'H' 查到 'C' 了，已查完，未找到文件
        }
        DisplayErrInfor(CFILEOUT);//显示卡无文件
        g_music_vars.location.disk = 'C';
        if (FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, g_music_vars.fselmod, 'C') != 0)
        {
            if (FSELGetNext(g_music_vars.location.filename) != 0)
            {
                return 1;//找到文件
            }
            DisplayErrInfor(DFILEOUT);//显示磁盘无文件
        }
        return 0;
    }
}

