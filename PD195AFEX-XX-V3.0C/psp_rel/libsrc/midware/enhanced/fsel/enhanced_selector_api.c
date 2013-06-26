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
#include <irq.h>
#ifdef PC
#define sfselGetByNO fselGetByNO
#endif
#include <rand.h>

#pragma name(MW_EH_FSEL_API)

/*外部变量和函数*/
extern bool sfselGetByNO(uint8 *strFile, const uint16 Num);
extern bool fselNextDir(uint8 *strDir);
extern bool fselGetNext(uint8 *strFile);
extern bool fselGetPrev(uint8 *strFile);
extern bool getClassParam(uint8 *classname, uint16 strlen); //get the current class name and parament
extern uint16 Random(void);
extern bool GetFilebyItem(uint8 *filename, uint16 ItemNum);
extern bool fselUserPLEnt(file_location_t *location);
extern void Init(uint8 type, uint8 mode);
extern uint8 fselSetLocationC(file_location_t *location);
extern bool fselGetLocationC(file_location_t *location);
extern uint16 GetFileTotalCur(void);
uint8 fselMode; //Mode of select, Sequence, repeat one ...
uint8 fselPLOrCommonDir; //playlist or common operation
uint8 fselType; //Type of select,  Music or voice.
uint8 fselError; // store error id;
uint16 fselFileNo; //file no

uint8 fselExtNo; //Number of current file ext name..
int8 fselDiskLetter;
uint16 fselRandCount;

uint16 fselTotal; //Total of current select file
int8 fselDirLayer; //dir  layer
uint8 fselDirNo[TOTALDIRLAYER]; //Number of current valid directory

const uint8 folderType_ext[4] =
{ "*  " };

#pragma renamecode(MW_EH_FSEL_API)

/********************************************************************************
 * Description : Initialize the FSEL module
 *
 * Arguments  :
 type:   Select type,  is FSEL_TYPE_MUSIC or FSEL_TYPE_VOICE
 *            mode: Select mode, is FSEL_ALL_SEQUENCE or  FSEL_ALL_REPEATONE ...
 *            DirLevel: Directory deep level, 0 or 1 now.
 *
 * Returns     :
 *            TRUE: Successful,
 *            FALSE: Fail, detail information in fselError.
 * Notes       :Must call it before call other fsel Module functions
 *
 ********************************************************************************/
#ifndef PC
bool sfselInit(uint8 type, uint8 mode, uint8 PlayListOrCommonDir, uint8 CardOrFlash)
#else
bool fselInit(uint8 type, uint8 mode, uint8 PlayListOrCommonDir, uint8 CardOrFlash)
#endif
{
    fselDiskLetter = (int8)CardOrFlash;

    if (!DRV_ActiveDrive(fselDiskLetter, 1))
    {
        fselError = FSEL_ERR_DISK;
        return FALSE;
    }

    Init(type, mode);
#ifndef PC
    //srand(5)
#endif

    if (PlayListOrCommonDir == FSEL_TYPE_COMMONDIR)
    {
        fselPLOrCommonDir = FSEL_TYPE_COMMONDIR;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

/********************************************************************************
 * Description : Set the select mode.
 *
 * Arguments  :
 mode:  the new mode
 * Returns     :  Successful or Fail
 * Notes       : None
 *
 ********************************************************************************/
#ifndef PC
bool sfselSetMode(uint8 mode)
#else
bool fselSetMode(uint8 mode)
#endif
{
    if (mode >= FSEL_DIR_END)
    {
        return FALSE;
    }
    fselMode = mode;
    return TRUE;
}

/********************************************************************************
 * Description : get the next file in playlist or common file system
 *
 * Arguments  :
 *             filename : output the next filename
 * Returns     :
 *            TRUE: Successful,
 *            FALSE: Fail, detail information in fselError.
 * Notes       :in common type, it call fselGetNext
 *
 ********************************************************************************/
#ifndef PC
bool sfselGetNextFile(uint8 *filename)
#else
bool fselGetNextFile(uint8 *filename)
#endif
{
    if (fselPLOrCommonDir == FSEL_TYPE_COMMONDIR) //comon file system
    {
        return fselGetNext(filename);
    }
    else
    {
        return FALSE;
    }
}

/********************************************************************************
 * Description : get the prev file in playlist or common file system
 *
 * Arguments  :
 *             filename : output the prev filename
 * Returns     :
 *            TRUE: Successful,
 *            FALSE: Fail, detail information in fselError.
 * Notes       :in common type, it call fselGetPrev
 *
 ********************************************************************************/
#ifndef PC
bool sfselGetPrevFile(uint8 *filename)
#else
bool fselGetPrevFile(uint8 *filename)
#endif
{

    if (fselPLOrCommonDir == FSEL_TYPE_COMMONDIR) //comon file system
    {
        return fselGetPrev(filename);
    }
    else
    {
        return FALSE;
    }
}
/********************************************************************************
 * Description : throgh the keeped values set the file selector parament
 *
 * Arguments  :location :the keeped vlaues
 *
 * Returns     :
 *            TRUE: Successful,
 *            FALSE: Fail, detail information in fselError.
 * Notes       :
 *
 ********************************************************************************/
#ifndef PC
uint8 sfselSetLocation(file_location_t *location)
#else
uint8 fselSetLocation(file_location_t *location)
#endif
{
    if (fselPLOrCommonDir == FSEL_TYPE_COMMONDIR) //common file system
    {
        return fselSetLocationC(location);
    }
    else
    {
        return FALSE;
    }
}

/********************************************************************************
 * Description : keep values of the file selector parament
 *
 * Arguments  :location :the keeped vlaues
 *
 * Returns     :
 *            TRUE: Successful,
 *            FALSE: Fail, detail information in fselError.
 * Notes       :
 *
 ********************************************************************************/
#ifndef PC
bool sfselGetLocation(file_location_t *location)
#else
bool fselGetLocation(file_location_t *location)
#endif
{
    if (fselPLOrCommonDir == FSEL_TYPE_COMMONDIR) //comond file system
    {
        return fselGetLocationC(location);
    }
    else
    {
        return FALSE;
    }
}

/********************************************************************************
 * Description : Get the total of valid files according to select type and mode
 *
 * Arguments  : None
 * Returns     : Total of valid files
 * Notes       : None
 *
 ********************************************************************************/
#ifndef PC
uint16 sfselGetTotal(void)
#else
uint16 fselGetTotal(void)
#endif
{
    if (fselPLOrCommonDir == FSEL_TYPE_COMMONDIR)
    {
        return fselTotal;
    }
    else
    {
        return 0;
    }
}

/********************************************************************************
 * Description : Get the No. of current selected file.
 *
 * Arguments  : None
 * Returns     : Number of current selected file.
 * Notes       : None
 *
 ********************************************************************************/
#ifndef PC
uint16 sfselGetNo(void)
#else
uint16 fselGetNo(void)
#endif
{
    if (fselPLOrCommonDir == FSEL_TYPE_COMMONDIR)
    {
        return fselFileNo;
    }
    else
    {
        return 0;
    }
}

/********************************************************************************
 * Description : Get the Error id.
 *
 * Arguments  : None
 * Returns     : Error id
 * Notes       :  None
 *
 ********************************************************************************/
#ifndef PC
int8 sfselGetErrID(void)
#else
int8 fselGetErrID(void)
#endif
{
    return fselError;
}

/********************************************************************************
 * Description : get the long file name in playlist or common file system
 *
 * Arguments  :
 *             longfilename : output the  current filename(long name)
 iLen :the length of get group 获得文件名的长名
 * Returns     :
 *            TRUE: Successful,
 *            FALSE: Fail, detail information in fselError.
 * Notes       :
 *
 ********************************************************************************/
#ifndef PC
bool sfselGetCurFNameL(uint8 *longfilename, uint8 iLen)
#else
bool fselGetCurFNameL(uint8 *longfilename, uint8 iLen)
#endif
{
    char name[16];
    if (fselPLOrCommonDir == FSEL_TYPE_COMMONDIR) // common file system
    {
        if (FALSE != sfselGetByNO(name, fselFileNo)) //through file no get the name
        {
            memcpy(longfilename, name, 12);
            if (FALSE != FS_GetName(longfilename, iLen))
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        return FALSE;
    }
    else
    {
        return FALSE;
    }

}
#ifndef PC
uint16 sFS_CD_DIR_PARENT(void)
#else
uint16 FS_CD_DIR_PARENT(void)
#endif
{
    if ((fselDirLayer == 0) && (fselDirNo[0] == 0))
    {
        //root, after the this call, app should call the fselInit
        fselError = FSEL_ERR_OUTOF; //已经返回到根目录，重新初始化
        return FALSE;
    }
    FS_CD("\\");
    fselDirLayer--;
    fselDirNo[fselDirLayer] = 0;
    fselTotal = GetFileTotalCur();
    fselFileNo = 0;
    fselExtNo = 0;
    return TRUE;
}

#ifndef PC
uint16 sFS_CD_DIR_SON(const uint8 *subFolderName)
#else
uint16 FS_CD_DIR_SON(const uint8 *subFolderName)
#endif
{
    uint8 j;
    uint8 file[12];

    if (fselDirLayer >= TOTALDIRLAYER)//if (fselDirLayer > 7)  //
    {
        return FALSE;
    }

    j = 1;
    if (!(FS_Dir(file, folderType_ext, FS_DIR_DIR, 1)))
    {
        return FALSE;
    }

    do
    {
        ClearWatchDog();
        if (0 == memcmp(file, subFolderName, 8))
        {
            break;
        }
        j++;
        if(j > FSEL_MAX_DIRNUM)
        {
            return FALSE;
        }
        if (0 == FS_Dir(file, folderType_ext, FS_DIR_DIR_CONT, 1))
        {
            return FALSE;
        }
    } while (1);

    fselDirNo[fselDirLayer] = j;
    FS_CD("/");
    fselDirLayer++;
    fselTotal = GetFileTotalCur();
    fselFileNo = 0;
    fselExtNo = 0;
    return TRUE;
}
