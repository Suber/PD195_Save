#include "actos.h"
#include "enhanced.h"
#include "decC.h"

#pragma name(MW_EH_TPCHK_MP31)

#pragma renamecode(MW_EH_TPCHK_MP31)

//#include "debug.h"


//#define BSPAGESize 512
//#define Sync_Head_MaxCount 32

extern BYTE *databuf;
extern WORD bitrate;
extern WORD samplerate;
extern BYTE Byterate;
extern DWORD tvalue;
extern int EndPageLen;//���һ��PAGE���ֽ���
extern long m_Filetotalpages;
extern HANDLE FileHandle;
extern uint16 time_count;
const WORD table_bitrate[2][3][15] =
{
    {
        { 0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256 },
        { 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160 },
        { 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160 } 
    },
    {
        { 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448 },
        { 0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384 },
        { 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320 } 
    }
};
const WORD samplerate_table[3] =
{ 44100, 48000, 32000 };
const WORD samples_per_frame[2][3] =
{
/* Layer   I    II   III */
    { 384, 1152, 1152 }, /* MPEG-1     */
    { 384, 1152, 576 } /* MPEG-2(.5) */
};
const char Xingtag[] = "Xing";

/*
 ********************************************************************************
 *             mp3Getframesize
 *
 * Description :  �������֡�ĳ��ȣ���ʵ�ɱ�ġ��й�ʽ���㡣�����ÿ��ĳ��Ȳ��ܻ����һ�������ͷ
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
WORD mp3Getframesize(DWORD curframehead)
{
    WORD curframesize, t;
    BYTE mpeg_lsf, mpeg_ver, mpeglayer, padding;
    /* 1: MPEG-1, 0: MPEG-2 LSF*/

    mpeg_ver = (BYTE) (curframehead >> 19) & 0x3;
    mpeg_lsf = (mpeg_ver & 0x1);
    if (mpeg_ver == 0)
    {
        mpeg_ver = 2;	
    }        
    else
    {
        mpeg_ver = 3 - mpeg_ver;
    }        
    mpeglayer = 4 - ((BYTE) (curframehead >> 17) & 0x3);
    t = (BYTE) (curframehead >> 10) & 0x3;
    samplerate = samplerate_table[t];
    samplerate = samplerate >> mpeg_ver;
    t = (BYTE) (curframehead >> 12) & 0xF;
    bitrate = table_bitrate[mpeg_lsf][mpeglayer - 1][t];
    padding = (BYTE) (curframehead >> 9) & 0x1;

    if (mpeglayer == 3)
    {
        curframesize = (WORD)((144000 * ((DWORD) bitrate)) / ((DWORD) samplerate));
        if (mpeg_lsf == 0)
        {
            curframesize = curframesize >> 1;	
        }            
        curframesize = curframesize + padding;
    }
    else if (mpeglayer == 2)
    {
        curframesize = (WORD)((144000 * ((DWORD) bitrate)) / ((DWORD) samplerate));
        curframesize = curframesize + padding;
    }
    else //lay1
    {
        curframesize = (WORD)((12000 * ((DWORD) bitrate)) / ((DWORD) samplerate));
        curframesize = curframesize + padding;
        curframesize = curframesize << 2;
    }
    return curframesize;
}
/*
 ********************************************************************************
 *             mp3GetTotaltime
 *
 * Description :  ����ǿɱ����ʵĸ������ڵ�һ֡����֡�л��м�¼�����Ϣ������¼���е�֡���ģ������������������ʱ��
 *                 ��"Xing"���
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
int mp3GetTotaltime(uint32 *pMp3frameoffset, time_t *pTotaltime)
{
    uint32 TotalFileLen, totalsecs;
    uint32 pagestart, framecount;
    int16 bufoffset, testcount = 0;
    BYTE *pframecount;
    BYTE mpeg_lsf, mpeg_ver, mpeglayer, t, j = 0;

    WORD framesamples;//��һ֡���ܲ�������

    mpeg_ver = (BYTE) (tvalue >> 19) & 0x3;
    mpeg_lsf = (mpeg_ver & 0x1);
    if (mpeg_ver == 0)
    {
        mpeg_ver = 2;//MPEG 2.5
    }        
    else
    {
        mpeg_ver = 3 - mpeg_ver;// 0: MPEG-1, 1: MPEG-2
    }        
    mpeglayer = 4 - ((BYTE) (tvalue >> 17) & 0x3);

    t = (BYTE) (tvalue >> 10) & 0x3;
    samplerate = samplerate_table[t];
    samplerate = samplerate >> mpeg_ver;

    t = (BYTE) (mpeg_ver != 0);//MPEG-2,MPEG 2.5 framesamples ��ͬ
    framesamples = samples_per_frame[t][mpeglayer - 1];

    TotalFileLen = (uint32) m_Filetotalpages * 512;

    //�����ļ�β������PAGE������
    if (EndPageLen != 0)
    {
        TotalFileLen = TotalFileLen - 512 + (uint32)EndPageLen;
    }

    pagestart = *pMp3frameoffset >> 9;
    bufoffset = (int16)((*pMp3frameoffset) & 0x1ff);

    ClearWatchDog();
    FS_FSeek((int32)pagestart, 0, FileHandle);
    #ifdef PC
    FS_FRead(GET_REAL_ADDR(databuf), 1, FileHandle);
    #else
    FS_FRead(databuf, 1, FileHandle);
    #endif

    pframecount = (BYTE*) &framecount;
    while (testcount < 512)//�������֡�ĳ��������156
    {
        while ((bufoffset < 512) && (j <= 11))
        {
            if ((j >= 4) && (j < 8))
            {
                j++;
            }
            else if (j >= 8)
            {
                //should change to big endian
                pframecount[j - 8] = databuf[bufoffset];
                j++;
            }
            else if (databuf[bufoffset] == Xingtag[j])
            {
                j++;
            }
            else
            {
                j = 0;
            }
            bufoffset++;
            testcount++;
        }
        if (j > 11)
        {
            break;
        }
        else
        {
            ClearWatchDog();
            #ifdef PC
            FS_FRead(GET_REAL_ADDR(databuf), 1, FileHandle);//֮ǰ�Ѿ�seekһҳ��
            #else
            FS_FRead(databuf, 1, FileHandle);//֮ǰ�Ѿ�seekһҳ��
            #endif
            bufoffset = 0;
        }
    }

    if ((j > 11) && (framecount != 0))//�ҵ�Xingtag
    {
        //��ʱ�� = ����֡�� * ��֡����������/ ������
        totalsecs = (framecount * framesamples) / samplerate;
    }
    else
    {
        totalsecs = (TotalFileLen - *pMp3frameoffset) / Byterate / 1000;
    }

    pTotaltime->hour = (uchar) (totalsecs / 3600);
    totalsecs %= 3600;
    pTotaltime->minute = (uchar) (totalsecs / 60);
    totalsecs %= 60;
    pTotaltime->second = (uchar) totalsecs;
    return 0;
}
