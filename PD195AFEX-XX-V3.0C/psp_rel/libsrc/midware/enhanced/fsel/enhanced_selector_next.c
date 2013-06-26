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
#include <rand.h>

#pragma name(MW_EH_FSEL_NEXT)

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

const uint8 ExtMusicNext[][4] =
{ "MP3", "WMA", /*"WAV",*/ "MP1", "MP2" };
const uint8 ExtVideoNext[][4] =
{ "AMV", "AVI" };
const uint8 ExtVoiceNext[][4] =
{ "WAV" };
const uint8 RootNameNext[] =
{ ":" };
const uint8 IgnoreExtNext[4] =
{ "*  " };

const uint8 ExtDscNext[][4] =
{ "BMP", "JPG", "GIF" };
const uint8 ExtTxtNext[][4] =
{ "TXT" };
const uint8 ExtAllNext[][4] =
{ "*  " };

#ifdef _ACTIVE_USERPL
extern WORD User_ItemTotal;
extern WORD User_ItemCur;
#endif

extern uint16 GetFileTotal(uint8 *strDir, bool bCheckValidDir);

bool sfselGetByNO(uint8 *strFile, const uint16 Num);

#pragma renamecode(MW_EH_FSEL_NEXT)
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
void Init(uint8 type, uint8 mode)
{
    uint8 i;

    fselType = type;
    fselDirLayer = 0;
    fselMode = mode;
    for (i = 0; i < TOTALDIRLAYER; i++)
    {
        fselDirNo[i] = 0;
    }
    fselFileNo = 0;
    fselExtNo = 0;
    fselError = 0;
    fselTotal = 0;
    FS_CD(":");
}

/********************************************************************************
 * Description : Get a random number
 *
 * Arguments  : None
 * Returns     :
 Random number
 * Notes       :  None
 *
 ********************************************************************************/
uint16 Random(void)
{
    fselRandCount = rand();
    return fselRandCount;
}

uint8 GetExtSize(void)
{
    if (fselType == FSEL_TYPE_MUSIC)
    {
        return sizeof(ExtMusicNext) / 4;
    }
    else if (fselType == FSEL_TYPE_VOICE)
    {
        return sizeof(ExtVoiceNext) / 4;
    }
    else if (fselType == FSEL_TYPE_TXT)
    {
        return sizeof(ExtTxtNext) / 4;
    }
    else if (fselType == FSEL_TYPE_AMV)
    {
        return sizeof(ExtVideoNext) / 4;
    }
    else if (fselType == FSEL_TYPE_DSC)
    {
        return sizeof(ExtDscNext) / 4;
    }
    else
    {
        return 0;
    }
    return 0;
}

/********************************************************************************
 * Description : Get name buffer of current extend
 *
 * Arguments  :  获得歌曲扩展名
 * Returns     :  the buffer of current file extend name
 * Notes       :     None
 *
 ********************************************************************************/
const uint8 * GetExtBuf(void)
{
    ClearWatchDog();
    if (fselType == FSEL_TYPE_MUSIC)
    {
        return ExtMusicNext[0];
    }
    else if (fselType == FSEL_TYPE_VOICE)
    {
        return ExtVoiceNext[0];
    }
    else if (fselType == FSEL_TYPE_AMV)
    {
        return ExtVideoNext[0];
    }
    else if (fselType == FSEL_TYPE_DSC)
    {
        return ExtDscNext[0];
    }
    else if (fselType == DIRECTORY_ALL)
    {
        return ExtAllNext[0];
    }
    else if (fselType == FSEL_TYPE_TXT)
    {
        return ExtTxtNext[0];
    }
    else
    {
        return 0;
    }
}
/*
 *******************************************************************************
 * Description : select the next file and get the file name
 *
 * Arguments   :
 * Returns     :文件初始化时 fselFileNo = 0；调用SelectNext获得第一个文件后，fselFileNo = 1；因此fselFileNo
 * Notes       : 表示将要读的那个文件序号(从0开始)，每调用一次select，fselFileNo都会加1
 *
 *******************************************************************************
 */
bool SelectNext(uint8* strFile)
{
    BYTE select_type;
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

    if (FALSE != FS_Dir(strFile, GetExtBuf(), select_type, GetExtSize()))
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

/*
 *******************************************************************************
 * Description : Select the next directory of current select directory
 * 选择上下文件夹的时候 fselDirno[fselDirlayer] != 0	,其他情况是等于0的
 * Arguments  : 进入下一个目录，并修改fselDirNo[fselDirLayer]
 *                 strDir: Dos 8.3 directory name of the next.
 *
 * Returns     :
 *    TRUE:  success.
 *    FALSE: fail, detail info store in fselError.
 * Notes       :  None
 *这里的实现是查找下一个目录时根据目录数，每次都从第一个开始dir，dir到所需的目录序号，太慢了
 *******************************************************************************
 */
bool fselNextDir(uint8 *strDir)
{
    uint8 subDirNum;
    subDirNum = fselDirNo[fselDirLayer]; //已经退回平级目录了
    subDirNum++;

    if (subDirNum > FSEL_MAX_DIRNUM)
    {
        fselError = FSEL_ERR_OUTOF;
        return FALSE;
    }
    memset(strDir, 0x00, 12);

    if (!FS_Dir(strDir, IgnoreExtNext, FS_DIR_DIR_CONT, 1)) //因为之前调用parentNext,所以不需再实现selDiryByNo调用
    {
        fselError = FSEL_ERR_OUTOF;
        return FALSE;
    }
    fselDirNo[fselDirLayer] = subDirNum;
    return TRUE;
}

/********************************************************************************
 * Description : 获得当前目录的某种类型文件的总数
 * Arguments  :
 *            strDir: Dos 8.3 directory name
 *            bCheckValidDir: if check valid directory only
 * Returns     :
 *            The file total of current directory when bCheckValidDir = FALSE
 or Vaild BOOL flag when bCheckValidDir = TRUE
 * Notes       : None
 *
 ********************************************************************************/
uint16 GetFileTotalCur(void)
{
    WORD total = 0;
    BYTE dir_type = FS_DIR_FILE;
    char m_szTemp[12];
    long lEntry = FS_GetCurDirEntry();
    do
    {
        ClearWatchDog();
        if (FALSE != FS_Dir(m_szTemp, GetExtBuf(), dir_type, GetExtSize()))
        {
            total++;
        }
        else
        {
            break;
        }
        dir_type = FS_DIR_FILE_CONT;
    } while (1);//(total < FSEL_MAX_FILENUM);
    FS_SetCurDirEntry(lEntry);
    return total;
}

/********************************************************************************
 * Description : Seek to the next file
 *
 * Arguments  :
 strFile:output file name
 * Returns     :
 TRUE: Successful
 FALSE: fail.
 * Notes       :  目录树查找,先检查父目录再检查子目录(子目录1 -- 子目录99)
 *
 ********************************************************************************/
bool SelectNext_8Layer(uint8 *strFile)
{
    //在当前目录向下找文件
    char m_szTemp[12];
    fselError = 0;
    if (fselTotal == 0)
    {
        fselTotal = GetFileTotalCur();
    }

    if (!SelectNext(strFile))
    {
        CurrentDiragain: ClearWatchDog();

        //寻找该目录的第一个子文件夹
        if (FALSE != FS_Dir(m_szTemp, IgnoreExtNext, FS_DIR_DIR, 1))
        {
            if (FALSE != FS_CD(FS_CD_SON))
            {
                //进入子文件夹，获得该子文件夹的该种文件类型数
                fselTotal = GetFileTotalCur();
                fselDirNo[fselDirLayer] = 1;
                fselDirLayer++;

                if (fselDirLayer > TOTALDIRLAYER)// (fselDirLayer > 7)
                {
                    fselDirLayer--; //problem	   bug
                    fselDirNo[fselDirLayer] = 0;
                    FS_CD(FS_CD_PARENT);
                    goto ParentDiragainNext;
                }

                fselFileNo = 0;
                fselExtNo = 0;

                // 检查当前目录是否有合适的文件.
                if (FALSE != SelectNext(strFile))
                {
                    fselError = 0;
                    return TRUE;
                }
                else //子目录下的子目录循环查找
                {
                    goto CurrentDiragain;
                }
            }
            else //出错了
            {
                //fselInit(fselType, fselMode, 0, fselDiskLetter);
                Init(fselType, fselMode);
                fselError = FSEL_ERR_OUTOF;
                return FALSE;
            }
        }
        else //如果没有子文件夹，则向上跳一层，寻找当前目录的下一个目录（平级寻找）
        {
            ParentDiragainNext:

            ClearWatchDog();
            if (FALSE != FS_DirParNext())//回到父目录的下一个
            {

                fselDirLayer--;
                if (FALSE != fselNextDir(m_szTemp)) //这个函数维护 fselDirNo[fselDirLayer]为下一个子目录
                {

                    FS_CD(FS_CD_SON);
                    fselDirLayer++;

                    fselTotal = GetFileTotalCur();
                    fselFileNo = 0;
                    fselExtNo = 0;

                    if (FALSE != SelectNext(strFile))
                    {
                        fselError = 0;
                        return TRUE;
                    }
                    // 检查子目录.
                    goto CurrentDiragain;
                }
                else //平级没有下一级目录，到上级目录
                {

                    fselDirNo[fselDirLayer] = 0;

                    goto ParentDiragainNext;
                }
            }
            else
            {
                //fselInit(fselType, fselMode,0,fselDiskLetter);
                Init(fselType, fselMode);
                fselError = FSEL_ERR_OUTOF;
                return FALSE;
            }
        }
    }
    return TRUE;
}
/********************************************************************************
 * Description : Select the next file according to type & mode, then get the name of file.
 *
 * Arguments  :
 *            strFile:  msdos 8.3  file name of next file
 * Returns     :true表示成功，false表示失败
 *            TRUE: Successful
 *            FALSE: Fail, detail information in fselError
 * Notes       :  None
 *
 ********************************************************************************/

bool fselGetNext(uint8 *strFile)
{
    uint16 num;

    ClearWatchDog();
    switch (fselMode)
    {
        //重复当前一个文件
        case FSEL_ALL_REPEATONE:
        {
            if ((0 == fselFileNo) && (0 != fselTotal))
            {
                fselFileNo = 1;
                return sfselGetByNO(strFile, fselFileNo);
            }
            else if (fselTotal == 0)
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

        //当前目录下的随机播放
        case FSEL_DIR_RANDOM:
        {
            num = (Random() % fselTotal) + 1;
            return sfselGetByNO(strFile, num);
        }

        //当前目录下的下一首，都目录尾就结束
        case FSEL_DIR_SEQUENCE:
        case FSEL_DIR_INTRO:
        {
            return SelectNext(strFile);
        }

        //所有文件顺序读取，不重复
        case FSEL_ALL_SEQUENCE:
        {
            return SelectNext_8Layer(strFile);
        }

        //所有文件顺序读取，重复
        case FSEL_ALL_REPEATALL:
        {
            if (FALSE != SelectNext_8Layer(strFile))
            {
                return TRUE;
            }
            else
            {
                //fselInit(fselType, fselMode, FSEL_TYPE_COMMONDIR, fselDiskLetter);
                Init(fselType, fselMode);
                return SelectNext_8Layer(strFile);
            }
        }

        //当前目录重复播放
        case FSEL_DIR_REPEATALL:
        {
            if (FALSE != SelectNext(strFile))
            {
                return TRUE;
            }
            fselFileNo = 0;
            fselExtNo = 0;
            return SelectNext(strFile);
        }

        default:
        return FALSE;
    }
}

/********************************************************************************
 * Description : Select  the file name according to number in current directory
 *
 * Arguments  :
 *            strFile:  file name
 *            Num  : File number in currect directory
 * Returns     :
 *            TRUE: Successful
 *            FALSE: Fail, detail information in fselError.
 * Notes       :  这个函数一定是可以改进的。每次都是从目录首开始dir，太慢了。可以从当前位置开始啊
 *
 ********************************************************************************/

#ifndef PC
bool sfselGetByNO(uint8 *strFile, const uint16 Num)
#else
bool fselGetByNO(uint8 *strFile, const uint16 Num)
#endif
{
    uint16 dir_num;
    uint8 dir_type;

    ClearWatchDog();
    if ((Num > fselTotal) || (Num == 0))
    {
        fselError = FSEL_ERR_OUTOF;
        return FALSE;
    }

    if (fselFileNo == 0)
    {
        dir_type = FS_DIR_FILE;
    }
    else
    {
        dir_type = FS_DIR_FILE_CONT;
    }
    if (fselFileNo < Num)//大于当前文件号，向后Dir
    {
        dir_num = Num - fselFileNo;
        fselFileNo = 0;
        while (fselFileNo < dir_num)
        {
            ClearWatchDog();
            //if( FS_DirFile(strFile,GetExtBuf,dir_type,GetExtSize))
            if (FALSE != FS_Dir(strFile, GetExtBuf(), dir_type, GetExtSize()))
            {
                fselFileNo++;
            }
            else
            {
                return FALSE;
            }
            dir_type = FS_DIR_FILE_CONT;
        }
    }
    else if (fselFileNo > Num) //小于当前文件号，向前Dir
    {
        dir_type = FS_DIR_FILE_CONT;
        dir_num = fselFileNo - Num;
        fselFileNo = 0;
        while (fselFileNo < dir_num)
        {
            ClearWatchDog();
            if (FALSE != FS_DirPrev(strFile, GetExtBuf(), dir_type, GetExtSize()))
            {
                fselFileNo++;
            }
            else
            {
                return FALSE;
            }
        }
    }
    else //和当前的文件号相等
    {
#ifndef PC
        if (fselFileNo == 1) //第一个文件
        {
            dir_type = FS_DIR_FILE;
            if (!FS_Dir(strFile, GetExtBuf(), dir_type, GetExtSize()))
            {
                return FALSE;
            }
        }
        else
        {
            if (!FS_GetName(strFile, 0))
            {
                return FALSE;
            }
        }
#else
        if(fselFileNo == 1) //第一个文件
        {
            dir_type = FS_DIR_FILE;
            if(!FS_Dir(strFile,GetExtBuf(),dir_type,GetExtSize()))
            {
                return FALSE;
            }
        }
#endif
    }

    fselFileNo = Num;
    return TRUE;
}


/*
#ifndef PC
bool sfselGetByNO(uint8 *strFile, const uint16 Num)
#else
bool fselGetByNO(uint8 *strFile, const uint16 Num)
#endif
{
    uint8 dir_type;

    ClearWatchDog();
    if ((Num > fselTotal) || (Num == 0))
    {
        fselError = FSEL_ERR_OUTOF;
        return FALSE;
    }
    fselFileNo = 0;
    dir_type	=	FS_DIR_FILE;
    if(FALSE	==	FS_Dir(strFile,	GetExtBuf(),	dir_type,	GetExtSize()))
    {
        return FALSE;
    }
    fselFileNo	=	1;

    dir_type	=	FS_DIR_FILE_CONT;
    while	(fselFileNo	<	Num)
    {
        ClearWatchDog();
        if	(FALSE	!=	FS_Dir(strFile,	GetExtBuf(),	dir_type,	GetExtSize()))
        {
            fselFileNo++;
        }
        else
        {
            return	FALSE;
        }
    }
    return TRUE;
}
*/