/*
 *******************************************************************************
 *                File selector
 *            common library
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 * File: common_selector_api.c
 * By  : stevenluo
 * Version: v1.00     first version     2010-04-09 12:00
 *******************************************************************************
 */

#include "ap_common.h"

#pragma name(DIR_SEL_API)

/*######################################################################*/
/*#########################常驻数据段################################*/
/*######################################################################*/
uint8 dir_fselPLOrCommonDir;
uint8 dir_fselType; //Type of select,  Music or voice.
uint8 dir_fselMode; //Mode of select, Sequence, repeat one ...
uint8 dir_fselDirNo[TOTALDIRLAYER]; //Number of current valid directory
uint8 dir_fselDirLayer; //当前目录层次.与dir_fselDirNo[]共同记录当前目录在目录树中的位置.
uint8 dir_fselDiskLetter;
uint8 dir_IgnoreExt4[4] =
{ "*  " };
uint8 dir_m_szTemp[12];
uint16 dir_fselFileNo; //Number of current file,
uint16 dir_fselTotal; //Total of current select file


/*######################################################################*/
/*#########################代码段#####################################*/
/*######################################################################*/

/********************************************************************************
 * Description : 获取本地目录信息入口函数
 *
 * Arguments  :
 *            location: 本地目录信息结构体
 * Returns     :
 *            TRUE OR FALSE
 * Notes       :
 *
 ********************************************************************************/
uint8 dir_fselGetLocation(file_location_t *location)
{
    if (dir_fselPLOrCommonDir == FSEL_TYPE_COMMONDIR) //comond file system
    {
        return dir_fselGetLocationC(location);
    }
    else
    {
        return FALSE;
    }
}

/********************************************************************************
 * Description : 根据目录层次号判别该文件夹是否存在
 *
 * Arguments  :
 *            strDir: 存储文件短名的指针
 *		 Num：目录层次所对应的索引号
 * Returns     :
 *            TRUE OR FALSE
 * Notes       :
 *
 ********************************************************************************/
uint8 dir_ToDirEntry(uint8 *strDir, const uint16 Num)
{
    uint16 temp;
    if ((Num > FSEL_MAX_DIRNUM) || (Num < 1))
    {
        return FALSE;
    }
    memset(strDir, 0x00, 12);
    if (!FS_Dir(strDir, dir_IgnoreExt4, FS_DIR_DIR, 0))
    {
        return FALSE;
    }
    if (Num == 1)
    {
        return TRUE;
    }

    temp = 2;
    while (temp <= Num)
    {

        if (!FS_Dir(strDir, dir_IgnoreExt4, FS_DIR_DIR_CONT, 0))
        {
            return FALSE;
        }
        ClearWatchDog();
        temp++;
    }
    return TRUE;
}
/********************************************************************************
 * Description : 获取location信息
 *
 * Arguments  :
 *            location: 目录信息结构体
 *
 * Returns     :
 *            TRUE OR FALSE
 * Notes       :
 *
 ********************************************************************************/
uint8 dir_fselGetLocationC(file_location_t *location)
{
    int32 lEntry;
    location->disk =(int8) dir_fselDiskLetter;
    memcpy(location->DirNo, dir_fselDirNo, TOTALDIRLAYER);
    location->ClusterNo = FS_GetCurDir();
    location->dirLayer =(int8) dir_fselDirLayer;
    location->fselTotal = dir_fselTotal;
    location->fselFileNo = dir_fselFileNo;
    lEntry = FS_GetCurDirEntry();
    // 只有在dir_fselDirNo[0] == 0时,当前目录是根目录.
    if ((dir_fselDirLayer > TOTALDIRLAYER) || (dir_fselDirNo[0] == 0))
    {
        dir_m_szTemp[0] = ':';
        dir_fselDirNo[0] = 0;
        dir_fselDirLayer = 0;
    }
    else
    {
        FS_CD("\\");
        dir_ToDirEntry(dir_m_szTemp, dir_fselDirNo[dir_fselDirLayer - 1]);
    }
    memcpy(location->path, dir_m_szTemp, 11);
    FS_CD(dir_m_szTemp);
    FS_SetCurDirEntry(lEntry);
    return TRUE;
}

/********************************************************************************
 * Description : 初始化一些全局公共变量，变索引到根目录中
 *
 * Arguments  :
 *            type: 文件类型
 *		 mode：文件选择模式
 * Returns     :
 *           无
 * Notes       :
 *
 ********************************************************************************/
void dir_Init(uint8 type, uint8 mode)
{
    dir_fselType     = type;
    dir_fselDirLayer = 0;
    dir_fselMode     = mode;
    memset(dir_fselDirNo, 0x0, TOTALDIRLAYER);
    dir_fselFileNo   = 0;
    dir_fselTotal    = 0;
    FS_CD(":");
}

/********************************************************************************
 * Description : 初始化一些全局公共变量，变索引到根目录中
 *
 * Arguments  :
 *            type: 文件类型
 *		 mode：文件选择模式
 *		 PlayListOrCommonDir:列表模式
 * Returns     :
 *           无
 * Notes       :
 *
 ********************************************************************************/
uint8 dir_fselInit(uint8 type, uint8 mode, uint8 PlayListOrCommonDir)
{
    uint8 result;
    dir_Init(type, mode);
    if (PlayListOrCommonDir == FSEL_TYPE_COMMONDIR)
    {
        dir_fselPLOrCommonDir = FSEL_TYPE_COMMONDIR;
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    return result;
}

/********************************************************************************
 * Description : 进入当前目录
 *
 * Arguments  :
 *            strDir: 目录短名
 *
 *
 * Returns     :
 *           无
 * Notes       :
 *
 ********************************************************************************/
uint8 dir_fselEntCurrentDir(uint8 *strDir)
{
    if (dir_fselDirLayer <= TOTALDIRLAYER)
    {
        dir_fselDirLayer++;
    }
    else
    {
        return FALSE;
    }
    if (!FS_CD(strDir)) //enter the current dir
    {
        dir_fselDirLayer--;
        return FALSE;
    }
    if (dir_fselDirLayer <= TOTALDIRLAYER)
    {
        dir_fselDirNo[dir_fselDirLayer] = 0;
    }
    return TRUE;
}
/********************************************************************************
 * Description : 计算当前目录下的文件夹总数
 *
 * Arguments  :
 *            无
 *
 *
 * Returns     :
 *           文件夹总数
 * Notes       :
 *
 ********************************************************************************/
uint16 count_dir_TotalNum(void)
{
    uint16 tmp = 0;
    uint8 strDir[12];
    uint8 dir_type;
    dir_type = FS_DIR_DIR;
    memset(strDir, 0x00, 12);
    while (1)
    {

        if (!FS_Dir(strDir, dir_IgnoreExt4, dir_type, 0))
        {
            break;
        }
        ClearWatchDog();
        dir_type = FS_DIR_DIR_CONT;
        tmp++;
    }
    return tmp;
}
/********************************************************************************
 * Description : 返回上一级目录
 *
 * Arguments  :
 *            strDir：目录短名
 *
 *
 * Returns     :
 *           TRUE OR FALSE
 * Notes       :
 *
 ********************************************************************************/
uint8 dir_fselRetParentDir(uint8 *strDir)
{

    if (dir_fselDirLayer > 0)
    {
        dir_fselDirLayer--;
    }
    else
    {
        return FALSE;
    }
    if (!FS_CD(FS_CD_PARENT)) //return parent directory
    {
        return FALSE;
    }
    if (!dir_ToDirEntry(strDir, dir_fselDirNo[dir_fselDirLayer])) //get the parent direcory name
    {
        return FALSE;
    }

    return TRUE;
}
/********************************************************************************
 * Description :计算当前目录下的文件总数和文件夹总数
 *
 * Arguments  :
 *            无
 *
 *
 * Returns     :
 *           TRUE OR FALSE
 * Notes       :
 *
 ********************************************************************************/
uint8 init_dir_buf1(uint16 totalnum, uint8 flag)
{
    uint16 f_total_num;
    int32 lEntry;
    lEntry = FS_GetCurDirEntry();
    dir_total_num = count_dir_TotalNum();
    if (dir_fselType == FSEL_TYPE_DIR)
    {
        f_total_num = 0;
    }
    else
    {
        if (!flag)
        {
            f_total_num = dir_GetFileTotalCur();
        }
        else
        {
            f_total_num = totalnum;
        }
    }
    FS_SetCurDirEntry(lEntry);
    file_total_num = f_total_num;
    dir_fselTotal = f_total_num;
    list_total_num = dir_total_num + file_total_num + 1;
    dir_fselTotal = file_total_num;
    return TRUE;
}
