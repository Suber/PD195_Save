#pragma name(MW_EH_TPCHK_WAV)

#pragma renamecode(MW_EH_TPCHK_WAV)

#include <actos.h>
#include <enhanced.h>
#include <stringH.h>
//#include "decC.h"


#define TWO_MS 2
//#define BUFFER_ADDRESS (0x0600)
extern BYTE *databuf;
extern int EndPageLen;//最后一个PAGE的字节数
extern long m_Filetotalpages;
extern HANDLE FileHandle;

long far curpage;
static BYTE far tempstr[20];
static int8 far m_checkTimer;
static BYTE far wav_time_count;

static const char wavTag[][4] =
{ "fmt ", "data" };

#pragma ROM(large)
void wav_count_fun(void);
#pragma ROM(huge)

BOOL m_readstring(char* readbuf, int16* bufoffset, int16 str_len)
{
    bool result;
    ClearWatchDog();
    if ((*bufoffset + str_len) < 512)
    {
        memcpy(readbuf, databuf + *bufoffset, (uint16)(str_len));
    }
    else
    {
        memcpy(readbuf, databuf + *bufoffset, ((uint16)(512 - (*bufoffset))));

        result = FS_FRead(databuf, 1, FileHandle);
        if (FALSE == result)
        {
            return FALSE;
        }
        curpage++;
        memcpy(readbuf + 512 - *bufoffset, databuf, ((uint16)((str_len + *bufoffset) - 512)));
    }
    *bufoffset = (str_len + *bufoffset) & 0x1ff;

    return TRUE;
}

static short m_getpage(void)
{
    long curfilepos;
    BOOL result;

    ClearWatchDog();

    curfilepos = FS_FTell(FileHandle);
    #ifdef PC
    result = FS_FRead(GET_REAL_ADDR(databuf), 1, FileHandle);
    #else
    result = FS_FRead(databuf, 1, FileHandle);
    #endif
    curpage++;

    if (result == 0)
    {
        return 0;
    }
    if ((curfilepos + 1) == m_Filetotalpages)
    {
        return EndPageLen;
    }
    return 512;
}
#ifndef PC
BYTE swavCheckType(const uint8 *filename, uint32 *pWavframeoffset, time_t *pTotaltime, audio_format_t *formatParameter)
#else
BYTE wavCheckType(const uint8 *filename, uint32 *pWavframeoffset, time_t *pTotaltime, audio_format_t *formatParameter)
#endif
{
    //init
    int bufindex = 0, datalen;
    BYTE Tagindex = 0, j = 0;
    uint8 k;
    WORD wavtype, BitsPerSample;
    DWORD bytepersec, wavdatalen, totalsecs;
    uint32 bitRate;
    bool read_result;
    #ifdef PC
    databuf = (BYTE*) (GET_REAL_ADDR(BUFFER_ADDRESS));
    #else
    databuf = (BYTE*) BUFFER_ADDRESS;
    #endif
    curpage = -1;

    ClearWatchDog();

    FileHandle = NULL;
    FileHandle = FS_FOpen(filename, FS_OPEN_NORMAL);
    if (FileHandle == NULL)
    {
        return 1;
    }
    EndPageLen = FS_GetUnalignedLen(FileHandle);//取最后一个PAGE的BYTE数
    m_Filetotalpages = (long) FS_GetFileLen(FileHandle);

    m_checkTimer = (int8)TM_SetTimer((int) wav_count_fun, TWO_MS);
    datalen = m_getpage();

    if (datalen <= 12)
    {
        FS_FClose(FileHandle);
        TM_KillTimer(m_checkTimer);
        return CHECK_NOPASS;
    }

    while (wav_time_count < 150)
    {
        ClearWatchDog();
        while ((bufindex < datalen) && (wav_time_count < 150))
        {
            ClearWatchDog();
            if (j >= 4)
            {
                //tagIndex = 0时查找fmt格式段
                if (Tagindex == 0)
                {
                    read_result = m_readstring(tempstr, &bufindex, 20);
                    if (FALSE == read_result)
                    {

                        FS_FClose(FileHandle);
                        TM_KillTimer(m_checkTimer);
                        return CHECK_NOPASS;
                    }

                    //获得wav格式 PCM:0x01; ADPCM:0x11
                    //WAV是小端格式，读出来的数据是大端
                    wavtype = *((WORD*) &tempstr[4]);//压缩类型

                    bytepersec = 0;
                    for (k = 0; k < 4; k++)
                    {
                        bytepersec = (uint32) (bytepersec << 8);
                        bytepersec += (uint32) (tempstr[15 - k]);
                    }

                    //5101只支持16bit
                    BitsPerSample = *((WORD*) &tempstr[18]);//量化比特数
                    //wavtype = 1 表示无压缩， 0x11表示使用INTEL IMA-ADPCM压缩算法(4:1)
                    if (((wavtype != 0x0100) && (wavtype != 0x1100)) 
                            || ((BitsPerSample != 0x1000) && (wavtype == 0x0100))
                            || ((BitsPerSample != 0x0400) && (wavtype == 0x1100)))
                    {

                        FS_FClose(FileHandle);
                        TM_KillTimer(m_checkTimer);
                        return CHECK_NOPASS;
                    }

                    Tagindex = 1;
                    j = 0;
                    continue;
                }
                //tagIndex = 1时查找data格式段
                else
                {
                    //从data chunk 的第八位开始是文件的第一个数据帧 data chunk
                    *pWavframeoffset = (uint32)((curpage << 9) + bufindex + 4);
                    read_result = m_readstring(tempstr, &bufindex, 4);
                    if (FALSE == read_result)
                    {

                        FS_FClose(FileHandle);
                        TM_KillTimer(m_checkTimer);
                        return CHECK_NOPASS;
                    }

                    //wavdatalen = *((DWORD*)&tempstr[0]);//获得所有数据段的长度
                    wavdatalen = 0;
                    for (k = 0; k < 4; k++)
                    {
                        wavdatalen = (uint32) (wavdatalen << 8);
                        wavdatalen += (uint32) (tempstr[3 - k]);
                    }

                    bitRate = ((uint32) bytepersec) << 3;
                    bitRate /= 1000;

                    formatParameter->bit_rate[0] = ((uint8)(bitRate / 100)) + '0';
                    bitRate %= 100;
                    formatParameter->bit_rate[1] = ((uint8)(bitRate / 10)) + '0';
                    bitRate %= 10;
                    formatParameter->bit_rate[2] = ((uint8)(bitRate)) + '0';
                    formatParameter->bit_rate[3] = 0;

                    totalsecs = wavdatalen / bytepersec;
                    if (totalsecs == 0)
                    {
                        totalsecs++;//解决不到一秒录音文件总时间显示为0问题
                    }
                    pTotaltime->hour = (uchar) (totalsecs / 3600);
                    totalsecs %= 3600;
                    pTotaltime->minute = (uchar) (totalsecs / 60);
                    totalsecs %= 60;
                    pTotaltime->second = (uchar) totalsecs;

                    FS_FClose(FileHandle);
                    TM_KillTimer(m_checkTimer);
                    return CHECK_PASS;
                }
            }
            else if (databuf[bufindex] == wavTag[Tagindex][j])
            {
                j++;
            }
            else
            {
                j = 0;
            }
            bufindex++;
        }
        //继续查找，直到300毫秒耗尽
        if (bufindex >= datalen)
        {
            bufindex -= datalen;
            datalen = m_getpage();
        }
    }

    FS_FClose(FileHandle);
    TM_KillTimer(m_checkTimer);
    return CHECK_NOPASS;
}

#pragma ROM(large)
void wav_count_fun(void)
{
    wav_time_count++;
}
#pragma ROM(huge)
