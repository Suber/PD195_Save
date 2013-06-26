/*
 ********************************************************************************
 *                    AK211X MIDWARE WMA FORMAT PARSE
 *                (c) Copyright 2009-2010, Actions Co,Ld.
 *                        All Right Reserved
 *
 * FileName: enhance_wma_check_new.c Author:  wuyueqian Date:2010-2-7
 * Description:   get the total time / content / stream information
 * Others:
 * History:
 *      <author>    <time>       <version >    <desc>
 *      wuyueqian  2010/02/07       1.0      build this file
 ********************************************************************************
 */
#pragma name(MW_EH_TPCHK_WMA)

#pragma renamecode(MW_EH_TPCHK_WMA)

#include <actos.h>
#include <enhanced.h>
#include <stringH.h>

#define TWO_MS 2
//#define BUFFER_ADDRESS (0x0600)
//--------------extern variable--------------
extern BYTE *databuf;
extern int EndPageLen;
extern long m_Filetotalpages;
extern HANDLE FileHandle;
//--------------extern variable end----------

//--------------static variable--------------
static uint8 far tempstr[OBJECT_ID_LENGHT];
static uint8 far tempSizeBuffer[OBJECT_SIZE_LENGHT];
static int8 far m_checkTimer;
static uint8 far wma_time_count;
static uint16 far bufindex;
static int32 far curPage;
static int32 far tempCurPage;
typedef void (*dealFuc_t)(void *param);
typedef struct __ASF_check_object
{
    bool isExist;
    bool isOptional;
    uint32 object_buffer;
    uint32 dealFunc;
    void * param;
} ASF_check_object_t;

const char ASF_Header_Object[] =
{ 0x30, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11, 0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C };
//--------------static variable end---------

//--------------static function-------------------
static BOOL m_readstring(char* readbuf, uint16* bufoffset, uint16 str_len);
uint32 littleEndian_charToInt(uint8 *charValue, uint8 len);
static int16 m_getpage(int32 readSectorPos);
#pragma ROM(large)
void wma_count_fun(void);
#pragma ROM(huge)
//--------------static function end-------------------


//--------------config the task macro-----------------

#define  CHECK_FILEPROPERTIESOBJECT
#define  CHECK_STREAMPROPERTIESOBJECT
#define  CHECK_CONTENTDESCRIPTIONOBJECT
#define  CHECK_EXTENDEDCONTENTDESCRIPTIONOBJECT

#ifdef  CHECK_FILEPROPERTIESOBJECT
void dealFilePropertiesObject(void *param);
const char ASF_File_Properties_Object[] =
{ 0xA1, 0xDC, 0xAB, 0x8C, 0x47, 0xA9, 0xCF, 0x11, 0x8E, 0xE4, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65 };
#endif

#ifdef  CHECK_STREAMPROPERTIESOBJECT
void dealStreamPropertiesObject(void *param);
const char ASF_Stream_Properties_Object[] =
{ 0x91, 0x07, 0xDC, 0xB7, 0xB7, 0xA9, 0xCF, 0x11, 0x8E, 0xE6, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65 };
#endif

#ifdef  CHECK_CONTENTDESCRIPTIONOBJECT
void dealContentDescriptionObject(void *param);
const char ASF_Content_Description_Object[] =
{ 0x33, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11, 0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C };
#endif

#ifdef CHECK_EXTENDEDCONTENTDESCRIPTIONOBJECT
void dealExtendedContentDescriptionObject(void *param);
const char ASF_Extended_Content_Description_Object[] =
{ 0x40, 0xA4, 0xD0, 0xD2, 0x07, 0xE3, 0xD2, 0x11, 0x97, 0xF0, 0x00, 0xA0, 0xC9, 0x5E, 0xA8, 0x50 };
const char AlbumTitle[] = {0x57, 0x00, 0x4D, 0x00, 0x2F, 0x00, 0x41, 0x00, 0x6C, 0x00, 0x62, 0x00, 0x75, 0x00,
                           0x6D, 0x00, 0x54, 0x00, 0x69, 0x00, 0x74, 0x00, 0x6C, 0x00, 0x65, 0x00, 0x00, 0x00};
static uint8 far tempNameBuffer[30];
#endif 


//--------------config the task macro end-----------------


//读以后，指针也会跟着变
static BOOL m_readstring(char* readbuf, uint16* bufoffset, uint16 str_len)
{
    bool result;

    ClearWatchDog();
    if ((*bufoffset + str_len) < BYTES_OF_ONE_PAGE)
    {
        memcpy(readbuf, databuf + *bufoffset, str_len);
    }
    else
    {
        memcpy(readbuf, databuf + *bufoffset, BYTES_OF_ONE_PAGE - *bufoffset);

        result = FS_FRead(databuf, 1, FileHandle);
        if (FALSE == result)
        {
            return FALSE;
        }
        curPage++;
        memcpy(readbuf + BYTES_OF_ONE_PAGE - *bufoffset, databuf, str_len + *bufoffset - BYTES_OF_ONE_PAGE);
    }
    *bufoffset = (str_len + *bufoffset) & 0x1ff;

    return TRUE;
}

static int16 m_getpage(int32 readSectorPos)
{
    int32 curfilepos;
    BOOL result;

    ClearWatchDog();

    curfilepos = FS_FTell(FileHandle);
    if (readSectorPos > curfilepos)
    {
        result = FS_FSeek(readSectorPos - curfilepos, FS_SEEK_FFROMCUR, FileHandle);
        if (result == 0)
        {
            return 0;
        }
    }
    else if (readSectorPos == (curfilepos - 1))
    {
        return BYTES_OF_ONE_PAGE;//no need to read
    }
    else
    {
    }

    result = FS_FRead(databuf, 1, FileHandle);
    curPage = readSectorPos;//ftell is curPage + 1

    if (result == 0)
    {
        return 0;
    }
    if ((curfilepos + 1) == m_Filetotalpages)
    {
        return EndPageLen;
    }
    return BYTES_OF_ONE_PAGE;
}

#pragma ROM(large)
void wma_count_fun(void)
{
    wma_time_count++;
}
#pragma ROM(huge)
#ifndef PC
BYTE swmaCheckType(const uint8 *filename, uint32 *pWavframeoffset, time_t *pTotaltime, audio_format_t *formatParameter,
        ID3Info_t *ID3Info)
#else
BYTE wmaCheckType(const uint8 *filename, uint32 *pWavframeoffset, time_t *pTotaltime, audio_format_t *formatParameter, ID3Info_t *ID3Info)
#endif
{

    int16 datalen;
    uint8 numberOfHeader = 0;
    uint8 needCheckObject = 0;
    uint8 i = 0;
    uint8 j = 0;
    uint8 doneCheckObject = 0;
    uint32 objectSize = 0;
    uint16 tempBufIndex = 0;
    dealFuc_t funcPtr;

    //------------config your task parameter----------------------
    ASF_check_object_t ASF_check_object[] =
    {

#ifdef  CHECK_FILEPROPERTIESOBJECT
        { FALSE, FALSE, (uint32) ASF_File_Properties_Object, (uint32) dealFilePropertiesObject, 0 },
#endif

#ifdef  CHECK_STREAMPROPERTIESOBJECT
        { FALSE, FALSE, (uint32) ASF_Stream_Properties_Object, (uint32) dealStreamPropertiesObject, 0 },
#endif

#ifdef  CHECK_CONTENTDESCRIPTIONOBJECT
        { FALSE, TRUE, (uint32) ASF_Content_Description_Object, (uint32) dealContentDescriptionObject, 0 },
#endif

#ifdef CHECK_EXTENDEDCONTENTDESCRIPTIONOBJECT
        { FALSE, TRUE, (uint32) ASF_Extended_Content_Description_Object, (uint32) dealExtendedContentDescriptionObject, 0}
#endif

    };

    //the param is variable, so it can be initialized in the ASF_check_object struct
    //you shall ajust the param index of ASF_check_object if some macro is not defined
#ifdef  CHECK_FILEPROPERTIESOBJECT
    ASF_check_object[0].param = (void *) pTotaltime;
#endif

#ifdef 	CHECK_STREAMPROPERTIESOBJECT
    ASF_check_object[1].param = (void *) formatParameter;//index shall be adjust when needed
#endif

#ifdef	CHECK_CONTENTDESCRIPTIONOBJECT
    ASF_check_object[2].param = (void *) ID3Info;
#endif

#ifdef CHECK_EXTENDEDCONTENTDESCRIPTIONOBJECT
    ASF_check_object[3].param = (void *) ID3Info;
#endif
    //------------config your task parameter end ----------
    ClearWatchDog();

    BankFlush(FLUSH_BANK_CODEC);
    #ifdef PC
    databuf = (BYTE*)(GET_REAL_ADDR(BUFFER_ADDRESS));
    #else
    databuf = (BYTE*) BUFFER_ADDRESS;
    #endif
    curPage = -1;
    tempCurPage = -1;
    bufindex = 0;

    FileHandle = NULL;
    FileHandle = FS_FOpen(filename, FS_OPEN_NORMAL);
    if (FileHandle == NULL)
    {
        return CHECK_NOPASS;
    }
    EndPageLen = FS_GetUnalignedLen(FileHandle);//取最后一个PAGE的BYTE数
    m_Filetotalpages = (long) FS_GetFileLen(FileHandle);

    m_checkTimer = (int8)TM_SetTimer((int) wma_count_fun, TWO_MS);
    datalen = m_getpage(0);

    if (datalen <= (BYTES_OF_ONE_PAGE - 1))
    {
        FS_FClose(FileHandle);
        TM_KillTimer(m_checkTimer);
        return CHECK_NOPASS;
    }

    //NO DEED TO CHECK RETURN
    m_readstring(tempstr, &bufindex, OBJECT_ID_LENGHT);
    if (memcmp(tempstr, ASF_Header_Object, OBJECT_ID_LENGHT) != 0)
    {
        FS_FClose(FileHandle);
        TM_KillTimer(m_checkTimer);
        return CHECK_NOPASS;
    }

    m_readstring(tempstr, &bufindex, OBJECT_SIZE_LENGHT);
    *pWavframeoffset = DATA_OBJECT_HEADER_LENGHT + littleEndian_charToInt(tempstr, OBJECT_SIZE_LENGHT);

    m_readstring(tempstr, &bufindex, NUMBER_OF_HEADER_LENGHT);
    numberOfHeader = (uint8) littleEndian_charToInt(tempstr, NUMBER_OF_HEADER_LENGHT);

    needCheckObject = sizeof(ASF_check_object) / sizeof(ASF_check_object_t);

    bufindex = FIRST_HEADER_OBJECT_POS;
    while ((i < numberOfHeader) && (wma_time_count < 150))
    {
        ClearWatchDog();

        tempCurPage = curPage;
        tempBufIndex = bufindex;

        m_readstring(tempstr, &bufindex, OBJECT_ID_LENGHT);
        m_readstring(tempSizeBuffer, &bufindex, OBJECT_SIZE_LENGHT);
        objectSize = littleEndian_charToInt(tempSizeBuffer, OBJECT_SIZE_LENGHT);

        for (j = 0; j < needCheckObject; j++)
        {
            ClearWatchDog();
            if (ASF_check_object[j].isExist == FALSE)
            {
                if (memcmp(tempstr, (uint8 *) (ASF_check_object[j].object_buffer), OBJECT_ID_LENGHT) == 0)
                {
                    ASF_check_object[j].isExist = TRUE;
                    funcPtr = (dealFuc_t) (ASF_check_object[j].dealFunc);
                    funcPtr((void *) (ASF_check_object[j].param));

                    doneCheckObject++;
                    if (doneCheckObject == needCheckObject)
                    {
                        FS_FClose(FileHandle);
                        TM_KillTimer(m_checkTimer);
                        return CHECK_PASS;
                    }
                    break;
                }
            }
        }

        i++;
        objectSize += tempBufIndex;
        bufindex = objectSize % BYTES_OF_ONE_PAGE;
        if (objectSize > BYTES_OF_ONE_PAGE)
        {
            datalen = m_getpage(((int32)(objectSize >> 9)) + tempCurPage);
        }
        
        if (((uint32)((curPage<<9) + (int32)bufindex)) > (*pWavframeoffset))
        {
            break;
        }
    }
    FS_FClose(FileHandle);
    TM_KillTimer(m_checkTimer);

    for (j = 0; j < needCheckObject; j++)
    {
        if (!(ASF_check_object[j].isExist | ASF_check_object[j].isOptional))
        {
            return CHECK_NOPASS;
        }
    }

    return CHECK_PASS;//有没有检测到的项目，但其实可选的，非强制的，如content
}

uint32 littleEndian_charToInt(uint8 *charValue, uint8 len)
{
    uint32 intValue = 0;
    uint8 i;

    ClearWatchDog();
    for (i = len; i > 0; i--)
    {
        intValue = intValue << 8;
        intValue += (uint32) charValue[i - 1];
    }
    return intValue;
}

#ifdef  CHECK_FILEPROPERTIESOBJECT
void dealFilePropertiesObject(void *param)
{
    //now the file ptr is after the size of object
    uint32 playDuration;
    uint32 preroll;

    time_t *pTime = (time_t *) param;

    ClearWatchDog();

    bufindex += 320 / 8;
    if (bufindex > BYTES_OF_ONE_PAGE)
    {
        m_getpage(((int32)(bufindex >> 9)) + tempCurPage);
        bufindex = bufindex % BYTES_OF_ONE_PAGE;
    }

    m_readstring(tempstr, &bufindex, PLAY_DURATION_LENGHT);
    playDuration = littleEndian_charToInt(tempstr, PLAY_DURATION_LENGHT);

    bufindex += 8;
    if (bufindex > BYTES_OF_ONE_PAGE)
    {
        m_getpage(((int32)(bufindex >> 9)) + tempCurPage);
        bufindex = bufindex % BYTES_OF_ONE_PAGE;
    }

    m_readstring(tempstr, &bufindex, PREROLL_LENGHT);
    preroll = littleEndian_charToInt(tempstr, PREROLL_LENGHT);

    playDuration = playDuration - preroll;

    pTime->hour = 0;
    pTime->minute = (uchar) (playDuration / 600000000);
    playDuration %= 600000000;
    pTime->second = (uchar) (playDuration / 10000000);

}
#endif

#ifdef  CHECK_STREAMPROPERTIESOBJECT
void dealStreamPropertiesObject(void *param)
{
    //now the file ptr is after the size of object
    audio_format_t *formatPara = (audio_format_t *) param;
    uint32 bitRate, byteRate;

    ClearWatchDog();

    bufindex += 62;
    if (bufindex > BYTES_OF_ONE_PAGE)
    {
        m_getpage(((int32)(bufindex >> 9)) + tempCurPage);
        bufindex = bufindex % BYTES_OF_ONE_PAGE;
    }

    m_readstring(tempstr, &bufindex, 4);
    byteRate = littleEndian_charToInt(tempstr, 4);

    bitRate = ((uint32) byteRate) << 3;
    bitRate /= 1000;

    formatPara->bit_rate[0] = ((uint8)(bitRate / 100)) + '0';
    bitRate %= 100;
    formatPara->bit_rate[1] = ((uint8)(bitRate / 10)) + '0';
    bitRate %= 10;
    formatPara->bit_rate[2] = ((uint8)(bitRate)) + '0';
    formatPara->bit_rate[3] = 0;
}
#endif

#ifdef  CHECK_CONTENTDESCRIPTIONOBJECT
void dealContentDescriptionObject(void *param)
{
    //now the file ptr is after the size of object
    ID3Info_t *ID3Info = (ID3Info_t *) param;
    uint16 titleLen = 0;
    uint16 authorLen = 0;

    ClearWatchDog();

    m_readstring(tempstr, &bufindex, 2);
    titleLen = littleEndian_charToInt(tempstr, 2);

    m_readstring(tempstr, &bufindex, 2);
    authorLen = littleEndian_charToInt(tempstr, 2);

    bufindex += 6;
    if (bufindex > BYTES_OF_ONE_PAGE)
    {
        m_getpage(((int32)(bufindex >> 9)) + tempCurPage);
        bufindex = bufindex % BYTES_OF_ONE_PAGE;
    }

    if (ID3Info->TIT2_length < titleLen)
    {
        m_readstring(ID3Info->TIT2_buffer, &bufindex, ID3Info->TIT2_length);
        ID3Info->TIT2_buffer[ID3Info->TIT2_length-2] = 0;
        ID3Info->TIT2_buffer[ID3Info->TIT2_length-1] = 0;
		bufindex += titleLen - ID3Info->TIT2_length;
        if (bufindex > BYTES_OF_ONE_PAGE)
        {
            m_getpage(((int32)(bufindex >> 9)) + tempCurPage);
            bufindex = bufindex % BYTES_OF_ONE_PAGE;
        }
    }
    else
    {
        m_readstring(ID3Info->TIT2_buffer, &bufindex, titleLen);
        ID3Info->TIT2_buffer[titleLen-2] = 0;
        ID3Info->TIT2_buffer[titleLen-1] = 0;
    }
    

    if (ID3Info->TPE1_length < authorLen)
    {
        authorLen = ID3Info->TPE1_length;	//no need jmp for next object
    }
    m_readstring(ID3Info->TPE1_buffer, &bufindex, authorLen);
    ID3Info->TPE1_buffer[authorLen-2] = 0;
    ID3Info->TPE1_buffer[authorLen-1] = 0;
}
#endif

#ifdef CHECK_EXTENDEDCONTENTDESCRIPTIONOBJECT
void dealExtendedContentDescriptionObject(void *param)
{
    //now the file ptr is after the size of object AlbumTitle
    ID3Info_t *ID3Info = (ID3Info_t *) param;
    uint16 Content_Descriptors_Count = 0;
    uint16 Descriptor_Name_Length;
    uint16 Descriptor_Value_Length;
	uint i;

    ClearWatchDog();

    m_readstring(tempstr, &bufindex, 2);
    Content_Descriptors_Count = littleEndian_charToInt(tempstr, 2);

    for (i = 0; i < Content_Descriptors_Count; i++)
    {
        m_readstring(tempstr, &bufindex, 2);
        Descriptor_Name_Length = littleEndian_charToInt(tempstr, 2);
        if (Descriptor_Name_Length != sizeof(AlbumTitle))
        {
            bufindex += Descriptor_Name_Length + 2;//Descriptor Value Data Type
            if (bufindex > BYTES_OF_ONE_PAGE)
            {
                m_getpage(((int32)(bufindex >> 9)) + tempCurPage);
                bufindex = bufindex % BYTES_OF_ONE_PAGE;
            }        	
        }
        else
        {
            m_readstring(tempNameBuffer, &bufindex, Descriptor_Name_Length);
            if(0 == memcmp(tempNameBuffer, AlbumTitle, Descriptor_Name_Length))
            {
                m_readstring(tempstr, &bufindex, 2);//Descriptor_Value_Data_Type
                m_readstring(tempstr, &bufindex, 2);
                Descriptor_Value_Length = littleEndian_charToInt(tempstr, 2);
                if (ID3Info->TALB_length < Descriptor_Value_Length)
                {
                    Descriptor_Value_Length = ID3Info->TALB_length;
                }
                m_readstring(ID3Info->TALB_buffer, &bufindex, Descriptor_Value_Length);
		        ID3Info->TALB_buffer[Descriptor_Value_Length-2] = 0;
		        ID3Info->TALB_buffer[Descriptor_Value_Length-1] = 0;
                return;	
            }
            m_readstring(tempstr, &bufindex, 2);
            //Descriptor_Value_Data_Type = littleEndian_charToInt(tempstr, 2);
        }
        m_readstring(tempstr, &bufindex, 2);
        Descriptor_Value_Length = littleEndian_charToInt(tempstr, 2);
        bufindex += Descriptor_Value_Length;
        if (bufindex > BYTES_OF_ONE_PAGE)
        {
            m_getpage(((int32)(bufindex >> 9)) + tempCurPage);
            bufindex = bufindex % BYTES_OF_ONE_PAGE;
        }
    } 

}
#endif