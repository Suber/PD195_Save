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

#pragma name(MW_EH_FSEL_PREV)
#ifdef PC
#define sfselGetByNO fselGetByNO
#endif
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

const int8 ExtMusicPrev[][4] =
{ "MP3", "WMA", /*"WAV", */"MP1", "MP2" };
const int8 ExtVideoPrev[][4] =
{ "AMV", "AVI" };
const int8 ExtVoicePrev[][4] =
{ "WAV" };
const int8 IgnoreExtPrev[4] =
{ "*  " };
const int8 ExtAll1[][4] =
{ "*  " };
const int8 ExtDscPrev[][4] =
{ "BMP", "JPG", "GIF" };
const int8 ExtTxtPrev[][4] =
{ "TXT" };

extern uint16 Random(void);
extern uint16 GetFileTotalCur(void);
extern uint16 GetFileTotal(uint8 *strDir, BOOL bCheckValidDir);

bool SelLastDir(uint8 *strDir);
extern bool sfselGetByNO(uint8 *strFile, const uint16 Num);
extern bool fselGetNext(uint8 *strFile);
extern uint8 fselSetLocationC(file_location_t *location);
extern bool fselGetLocationC(file_location_t *location);
bool SelectPrevFile(uint8* strFile);

#pragma renamecode(MW_EH_FSEL_PREV)

/********************************************************************************
 * Description : Initialize the the file selector parament.
 ********************************************************************************/
void Init_Prev(uint8 type, uint8 mode)
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
 * Description : PREV操作获取后缀名BUFFER的地址
 *
 * Arguments  :  None
 * Returns     :  the buffer of current file extend name
 * Notes       :     None
 *
 ********************************************************************************/
const uint8 * GetExtBuf_Prev(void)
{
    ClearWatchDog();

    if (fselType == FSEL_TYPE_MUSIC)
    {
        return ExtMusicPrev[fselExtNo];
    }
    else if (fselType == FSEL_TYPE_VOICE)
    {
        return ExtVoicePrev[fselExtNo];
    }
    else if (fselType == FSEL_TYPE_AMV)
    {
        return ExtVideoPrev[fselExtNo];
    }
    else if (fselType == FSEL_TYPE_TXT)
    {
        return ExtTxtPrev[fselExtNo];
    }
    else if (fselType == FSEL_TYPE_DSC)
    {
        return ExtDscPrev[fselExtNo];
    }
    else
    {
        return ExtAll1[fselExtNo];
    }
}

/********************************************************************************
 * Description : PREV操作获取后缀名的个数，例如MUSIC支持WMA MP3 WAV一起DIR
 *
 * Arguments  :  None
 * Returns     :  the buffer of current file extend name
 * Notes       :     None
 *
 ********************************************************************************/

uint8 GetExtSize_Prev(void)
{
    if (fselType == FSEL_TYPE_MUSIC)
    {
        return sizeof(ExtMusicPrev) / 4;
    }
    else if (fselType == FSEL_TYPE_VOICE)
    {
        return sizeof(ExtVoicePrev) / 4;
    }
    else if (fselType == FSEL_TYPE_TXT)
    {
        return sizeof(ExtTxtPrev) / 4;
    }
    else if (fselType == FSEL_TYPE_AMV)
    {
        return sizeof(ExtVideoPrev) / 4;
    }
    else if (fselType == FSEL_TYPE_DSC)
    {
        return sizeof(ExtDscPrev) / 4;
    }
    else
    {
        return 0;
    }
    return 0;
}

/********************************************************************************
 * Description : select the next file and get the file name
 *
 * Arguments  :
 * Returns     :
 * Notes       :
 *
 ********************************************************************************/
bool SelectNextFile_Location(uint8* strFile)
{
    uint8 select_type;

    ClearWatchDog();
    if (fselFileNo == fselTotal)
    {
        fselError = FSEL_ERR_OUTOF;
        return FALSE;
    }

    if (fselFileNo == 0)
    {
        select_type = FS_DIR_FILE;
    }
    else
    {
        select_type = FS_DIR_FILE_CONT;
    }

    if (FALSE != FS_Dir(strFile, GetExtBuf_Prev(), select_type, GetExtSize_Prev()))
    {
        fselFileNo++;
        return TRUE;
    }
    else
    {
        fselError = FSEL_ERR_FAIL;
    }
    return FALSE;
}

/********************************************************************************
 * Description : select the next file and get the file name
 *向目录首方向获取上一个文件
 ********************************************************************************/
bool SelectPrevFile(uint8* strFile)
{
    BYTE dir_type = FS_DIR_FILE_CONT;
    if (fselFileNo == 1)
    {
        fselError = FSEL_ERR_OUTOF;
        return FALSE;
    }

    if (fselFileNo == 0)//表示刚进入该目录
    {
        fselFileNo = fselTotal;
        FS_DirLast(); //定位到最后一个目录项
    }
    else
    {
        fselFileNo--;
    }

    if (FALSE != FS_DirPrev(strFile, GetExtBuf_Prev(), dir_type, GetExtSize_Prev()))
    {
        return TRUE;
    }
    fselError = FSEL_ERR_FAIL;
    return FALSE;
}

/********************************************************************************
 * Description : 根据序号选择文件夹，
 *			   在fselPrevDir中调用，一般从第一个文件夹一直DIR到所需的文件夹
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
bool SelDirByNo_Prev(uint8 *strDir, uint16 Num)
{
    WORD temp;

    ClearWatchDog();

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
    if (!FS_Dir(strDir, IgnoreExtPrev, FS_DIR_DIR, 1))
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
        if (!FS_Dir(strDir, IgnoreExtPrev, FS_DIR_DIR_CONT, 1))
        {
            fselError = FSEL_ERR_OUTOF;
            return FALSE;
        }
        temp++;
    }

    return TRUE;
}
/********************************************************************************
 * Description : 选择上一个文件夹
 *
 * Arguments  : none
 *
 *
 * Returns     :
 *    TRUE:  success.
 *    FALSE: fail, detail info store in fselError.
 * Notes       : rewind to the last valid directory when fselDir == 0;
 *
 ********************************************************************************/
bool fselPrevDir(uint8 *strDir)
{
    uint8 curDirNo;

    if (fselDirNo[fselDirLayer] <= 1)
    {
        return FALSE;
    }

    curDirNo = fselDirNo[fselDirLayer];
    curDirNo--;
    if (!SelDirByNo_Prev(strDir, curDirNo))
    {
        return FALSE;
    }
    fselDirNo[fselDirLayer] = curDirNo;
    return TRUE;
}

/********************************************************************************
 * Description : 一般PREV操作进入到一个子目录， 就需要定位到最后的文件夹
 *
 * Arguments  : 	因为事先不知道总共几个目录，所以需要从头开始DIR
 strDir:output Dir name
 * Returns    :
 TRUE: Successful
 FALSE: fail.
 * Notes       :  None
 *
 ********************************************************************************/
bool SelLastDir(uint8 *strDir)
{
    char m_szTemp_ape1[12];
    if (fselDirLayer >= TOTALDIRLAYER)//if (fselDirLayer >= 7)
    {
        return FALSE;
    }
    fselDirNo[fselDirLayer] = 0;//搜索目录状态
    if (!FS_Dir(m_szTemp_ape1, IgnoreExtPrev, FS_DIR_DIR, 1)) // if no dir ,return error
    {
        return FALSE;
    }
    fselDirNo[fselDirLayer] = 1;
    memcpy(strDir, m_szTemp_ape1, 12);
    while (FALSE != FS_Dir(m_szTemp_ape1, IgnoreExtPrev, FS_DIR_DIR_CONT, 1))
    {
        ClearWatchDog();
        fselDirNo[fselDirLayer]++;
        memcpy(strDir, m_szTemp_ape1, 12);
    }
    return TRUE;
}

/********************************************************************************
 *选择上一个文件
 ********************************************************************************/

bool SelectPrev_8Layer(uint8 *strFile)
{
    char strDir[12];
    fselError = 0;
    if (fselTotal == 0)
    {
        fselTotal = GetFileTotalCur();
    }
    if (!SelectPrevFile(strFile))
    {
        //检查父目录.
        ParentDiragainNext: ClearWatchDog();
        if (FALSE != FS_CD("\\"))
        {
            fselDirLayer--;

            if (FALSE != fselPrevDir(strDir)) //平级目录的前一个目录
            {
                goto CurrentDiragainNext;
                //平级过去保证LAYER不会大于7
            }
            else
            { //当前目录下有效的文件
                fselDirNo[fselDirLayer] = 0;
                fselTotal = GetFileTotalCur();
                fselFileNo = 0;
                fselExtNo = 0;

                if (FALSE != SelectPrevFile(strFile))
                {
                    fselError = 0;
                    return TRUE;
                }
                //  当前目录分支没有合适的文件,退回上级目录
                goto ParentDiragainNext;
            }
        }
        else
        {
            fselError = FSEL_ERR_OUTOF;
            return FALSE;
        }
        //寻找当前目录下的所有目录
        CurrentDiragainNext: ClearWatchDog();
        FS_CD(strDir);
        fselDirLayer++;
        //        if (fselDirLayer > TOTALDIRLAYER ) //SelLastDir有判断是否等于7，所以不会跑到这个分支
        //        {
        //            FS_CD("\\");
        //            fselDirLayer--;
        //            goto ParentDiragainNext;
        //        }

        if (FALSE != SelLastDir(strDir)) //SelLastDir有判断是否等于7
        {
            goto CurrentDiragainNext;
            //一直找到尽头
        }
        else
        {
            fselTotal = GetFileTotalCur();
            fselFileNo = 0;
            fselExtNo = 0;

            if (FALSE != SelectPrevFile(strFile))
            {
                fselError = 0;
                return TRUE;
            }
            //  当前目录分支没有合适的文件,退回上级目录
            goto ParentDiragainNext;
        }
    }
    return TRUE;
}
/********************************************************************************
 * Description : Select the Prev file according to type & mode, then get the name of file.
 *
 * Arguments  :
 strFile: Dos 8.3 file name of selected file.
 * Returns     :
 TRUE: Successful.
 FALSE: Fail, detail information in fselError
 * Notes       :  None
 *
 ********************************************************************************/
bool fselGetPrev(uint8 *strFile)
{
    char temp[12];
    uint16 num;
    ClearWatchDog();
    switch (fselMode)
    {
        case FSEL_ALL_REPEATONE:
        {
            if ((0 == fselFileNo) && (0 != fselTotal))
            {
                fselFileNo = 1;
                return sfselGetByNO(strFile, fselFileNo);
            }
            else if (0 == fselTotal)
            {
                return FALSE;
            }
            else
            {
                if (!FS_GetName(strFile, 0))
                {
                    return FALSE;
                }
            }
            return TRUE;
        }

        case FSEL_DIR_RANDOM:
        {
            num = (uint16) ((Random() % fselTotal) + 1);
            return sfselGetByNO(strFile, num);
        }
        //return fselGetNext(strFile);

        case FSEL_DIR_SEQUENCE:
        case FSEL_DIR_INTRO:
        return SelectPrevFile(strFile);

        case FSEL_ALL_SEQUENCE:
        if ((0 == fselFileNo) && (fselDirLayer == 0) && (fselDirNo[0] == 0))
        {
            Init_Prev(fselType, fselMode);
            while (FALSE != SelLastDir(temp))
            {
                FS_CD(temp);
                fselDirLayer++;
                ClearWatchDog();
            }
        }
        return SelectPrev_8Layer(strFile);

        case FSEL_ALL_REPEATALL:
        if ((0 == fselFileNo) && (fselDirLayer == 0) && (fselDirNo[0] == 0))
        {
            Init_Prev(fselType, fselMode);
            while (FALSE != SelLastDir(temp))
            {
                FS_CD(temp);
                fselDirLayer++;
                ClearWatchDog();
            }
        }
        if (FALSE != SelectPrev_8Layer(strFile))
        {
            return TRUE;
        }
        else
        {
            Init_Prev(fselType, fselMode);
            while (FALSE != SelLastDir(temp))
            {
                FS_CD(temp);
                fselDirLayer++;
                ClearWatchDog();
            }
            return SelectPrev_8Layer(strFile);
        }

        case FSEL_DIR_REPEATALL:
        if (FALSE != SelectPrevFile(strFile))
        {
            return TRUE;
        }
        fselFileNo = 0;
        fselExtNo = 0;
        return SelectPrevFile(strFile);

        default:
        return FALSE;
    }

}
