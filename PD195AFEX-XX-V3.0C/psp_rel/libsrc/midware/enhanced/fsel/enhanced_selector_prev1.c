/*
 *******************************************************************************
 *                File selector
 *            enhanced library
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *******************************************************************************
 */
#include <actos.h>
#include <enhanced.h>
#include <stringH.h>
#include <sysdef.h>

#ifdef PC
#define sfselGetByNO fselGetByNO
#endif
#pragma name(MW_EH_FSEL_PREV1)

extern uint8 fselMode; //Mode of select, Sequence, repeat one ...
extern uint8 fselPLOrCommonDir; //playlist or common operation
extern uint8 fselType; //Type of select,  Music or voice.
extern uint8 fselError; // store error id;
extern uint16 fselFileNo; //file no

extern uint8 fselExtNo; //Number of current file ext name..
extern int8 fselDiskLetter;
extern uint16 fselRandCount;

extern uint16 fselTotal; //Total of current select file
extern int8 fselDirLayer; //dir  layer
extern uint8 fselDirNo[TOTALDIRLAYER]; //Number of current valid directory

extern uint16 GetFileTotalCur(void);
extern uint16 GetFileTotal(uint8 *strDir, bool bCheckValidDir);
//extern const uint8 * GetExtBuf_Prev(void);
extern uint8 GetExtSize_Prev(void);
extern bool SelectNextFile_Location(uint8* strFile);

const int8 dirExtLocation[4] =
{ "*  " };

const int8 ExtMusicPrev1[][4] =
{ "MP3", "WMA",/* "WAV", */"MP1", "MP2" };
const int8 ExtVideoPrev1[][4] =
{ "AMV", "AVI" };
const int8 ExtVoicePrev1[][4] =
{ "WAV" };
const int8 IgnoreExtPrev1[4] =
{ "*  " };
const int8 ExtAll11[][4] =
{ "*  " };
const int8 ExtDscPrev1[][4] =
{ "BMP", "JPG", "GIF" };
const int8 ExtTxtPrev1[][4] =
{ "TXT" };

#pragma renamecode(MW_EH_FSEL_PREV1)

BOOL SelDirByNo_Location(uint8 *strDir, uint16 Num);
extern bool sfselGetByNO(uint8 *strFile, const uint16 Num);

/********************************************************************************
 * Description : Initialize the the file selector parament.
 *
 * Arguments  :
 *            type: music or voice
 mode: 6 kinds mode
 * Returns     :
 *            TRUE:    Successful
 *            FALSE:    Fail, detail information in fselError
 * Notes       :  Should select path in the case of only file name is invalid
 *
 ********************************************************************************/
void Init_Location(uint8 type, uint8 mode)
{
    fselType = type;
    fselDirLayer = 0;
    fselMode = mode;
    memset(fselDirNo, 0, TOTALDIRLAYER);
    fselFileNo = 0;
    fselExtNo = 0;

    fselError = 0;
    fselTotal = 0;
    FS_CD(":");
}

/********************************************************************************
 * Description : seek to  specify Directory Entry
 *
 * Arguments  :
 *        strDir: Dos 8.3 Directory name
 *            num: number of the Directory Entry to seek
 *
 * Returns     :
 *            TRUE: Successful
 *            FALSE: Fail,  detail information in fselError
 * Notes       :
 *
 ********************************************************************************/
bool SelDirByNo_Location(uint8 *strDir, uint16 Num)
{
    WORD temp;
    if (Num > FSEL_MAX_DIRNUM)
    {
        fselError = FSEL_ERR_OUTOF;
        return FALSE;
    }

    if (Num < 1)
    {
        fselError = FSEL_ERR_OUTOF;
        return FALSE;
    }

    memset(strDir, 0x00, 12);
    if (!FS_Dir(strDir, dirExtLocation, FS_DIR_DIR, 1))
    {
        fselError = FSEL_ERR_OUTOF;
        return FALSE;
    }
    if (Num == 1)
    {
        return TRUE;
    }
    temp = 2;
    while (temp <= Num)
    {
        ClearWatchDog();
        if (!FS_Dir(strDir, dirExtLocation, FS_DIR_DIR_CONT, 1))
        {
            fselError = FSEL_ERR_OUTOF;
            return FALSE;
        }
        temp++;
    }

    return TRUE;
}


const uint8 * GetExtBuf_Prev1(void)
{
    ClearWatchDog();

    if (fselType == FSEL_TYPE_MUSIC)
    {
        return ExtMusicPrev1[fselExtNo];
    }
    else if (fselType == FSEL_TYPE_VOICE)
    {
        return ExtVoicePrev1[fselExtNo];
    }
    else if (fselType == FSEL_TYPE_AMV)
    {
        return ExtVideoPrev1[fselExtNo];
    }
    else if (fselType == FSEL_TYPE_TXT)
    {
        return ExtTxtPrev1[fselExtNo];
    }
    else if (fselType == FSEL_TYPE_DSC)
    {
        return ExtDscPrev1[fselExtNo];
    }
    else
    {
        return ExtAll11[fselExtNo];
    }
}


/********************************************************************************
 * Description : Set the selected location which is specified by location.
 *
 * Arguments  :
 *            location:     store drive name, path name, file name.
 * Returns     :
 *            TRUE:    Successful
 *            FALSE:    Fail, detail information in fselError
 * Notes       :  Should select path in the case of only file name is invalid
 *
 ********************************************************************************/
uint8 fselSetLocationC(file_location_t *location)
{
    BYTE i, iTag;
    char m_szTemp1[12];

    ClearWatchDog();

    iTag = 0;
    /*加入驱动初始化代码，解决插卡拔出时调用接口死机的问题*/
    if (!DRV_ActiveDrive(location->disk, 1))
    {
        fselError = FSEL_ERR_DISK;
        return FALSE;
    }

    Init_Location(fselType, fselMode);
    fselDiskLetter = location->disk;

    for (i = 0; i < TOTALDIRLAYER; i++)
    {
        fselDirNo[i] = location->DirNo[i];
    }

    //已经设置好文件夹的路径了
    if (!FS_SetCurDir(location->ClusterNo))
    {
        fselError = FSEL_ERR_SETDIR;
        return FALSE;
    }

    fselDirLayer = location->dirLayer;

    if (fselDirNo[0] != 0)
    {
        FS_CD("\\");
        SelDirByNo_Location(m_szTemp1, fselDirNo[fselDirLayer - 1]);
        FS_CD(m_szTemp1);

        if (memcmp(m_szTemp1, location->path, 11) != 0) //path is invalid
        {
            fselError = FSEL_ERR_SETDIR;
            return FALSE;
        }
    }
    else
    {

        if (location->path[0] != ':')
        {
            fselError = FSEL_ERR_SETDIR;
            return FALSE;
        }
        FS_CD(":");
    }

    if (FALSE == FS_SetCurDirEntry(location->DirEntry))
    {
        fselError = FSEL_ERR_SETDIR;
        return FALSE;
    }
    fselTotal = GetFileTotalCur();
    fselExtNo = 0;

    if ('\0' == location->filename[0])
    {
        return TRUE;
    }

    if (0 == fselTotal)
    {
        fselError = FSEL_ERR_SETFILE;
        return FALSE;
    }

    if (!FS_GetName(m_szTemp1, 0))
    {
        fselError = FSEL_ERR_SETFILE;
        return FALSE;
    }
    else
    {
        if (memcmp(m_szTemp1, location->filename, 11) != 0)
        {
            fselError = FSEL_ERR_SETFILE;
            fselFileNo = 0;

            fselExtNo = 0;
            return FALSE;
        }
    }

    /*
     do
     {
     ClearWatchDog();
     if (!SelectNextFile_Location(m_szTemp1))
     {
     fselError = FSEL_ERR_SETFILE;
     fselFileNo = 0;

     fselExtNo = 0;
     return FALSE;
     }
     //iTag = 1;
     //要找到原来打开指定的文件
     if (memcmp(m_szTemp1, location->filename, 11) == 0)
     {
     iTag = 1;
     break;
     }

     } while(1);
     */
    fselFileNo = 1;
    while(FS_DirPrev(m_szTemp1, GetExtBuf_Prev1(), FS_DIR_FILE_CONT, GetExtSize_Prev()))
    {
        ClearWatchDog();
        fselFileNo++;   //计算断点文件的序号
    }
    FS_SetCurDirEntry(location->DirEntry);  //回到刚才的文件
    return TRUE;
}

/********************************************************************************
 * Description : Get the selected location
 *
 * Arguments  :
 *            location:    the buffer to store location
 * Returns     :
 *            TRUE: Successful
 *            FALSE: Fail, detail information in fselError.
 * Notes       :  None
 *
 ********************************************************************************/

bool fselGetLocationC(file_location_t *location)
{
    BYTE i;
    char m_szTemp1[12];
    long lEntry;

    ClearWatchDog();

    location->disk = fselDiskLetter;

    for (i = 0; i < TOTALDIRLAYER; i++)
    {
        location->DirNo[i] = fselDirNo[i];
    }
    location->ClusterNo = FS_GetCurDir();//当前目录项在文件系统中的偏移
    location->dirLayer = fselDirLayer;

    lEntry = FS_GetCurDirEntry();//文件项在当前目录项中的偏移

    location->DirEntry = lEntry;

    if (fselFileNo != 0)
    {
        if (!sfselGetByNO(location->filename, fselFileNo))
        {
            location->filename[0] = 0x00;
            location->path[0] = ':';
            location->dirLayer = 0;
            return FALSE;
        }
    }
    else
    {
        location->filename[0] = 0x00;
    }

    // 只有在fselDirNo[0] == 0时,当前目录是根目录.
    if ((fselDirLayer > TOTALDIRLAYER) || (fselDirNo[0] == 0))//if ((fselDirLayer > 7) || (fselDirNo[0] == 0))
    {
        m_szTemp1[0] = ':';
        fselDirNo[0] = 0;
        fselDirLayer = 0;
    }
    else
    {
        FS_CD("\\");
        SelDirByNo_Location(m_szTemp1, fselDirNo[fselDirLayer - 1]);
    }

    ClearWatchDog();
    memcpy(location->path, m_szTemp1, 11);
    FS_CD(m_szTemp1);//之前退出到上一级来获得该文件夹的名字，现在再次进入。

    location->fselTotal = fselTotal;
    location->fselFileNo = fselFileNo;  //用序号作为断点信息会导致问题

    FS_SetCurDirEntry(lEntry);
    return TRUE;
}

