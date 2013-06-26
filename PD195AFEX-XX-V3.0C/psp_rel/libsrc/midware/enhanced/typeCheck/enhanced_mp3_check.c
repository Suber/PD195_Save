/********************************************************************************
 *                              USDK 5102
 *                            Module: POWER MANAGER
 *                 Copyright(c) 2001-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    wuyueqian     2009-9-07 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * file
 * brief
 * Author   wuyueqian
 * par
 *
 *      Copyright(c) 2001-2007 Actions Semiconductor, All Rights Reserved.
 *
 * version 1.0
 * date  2008/6/12
 *******************************************************************************/

#include <actos.h>
#include <enhanced.h>
#include <stringH.h>

#pragma name(MW_EH_TPCHK_MP3)

#pragma renamecode(MW_EH_TPCHK_MP3)

#define TWO_MS 2
#define TIME_COUNT_MAX 150

BYTE *databuf;
WORD bitrate;
WORD samplerate;
BYTE Byterate;
DWORD tvalue;
int EndPageLen;//���һ��PAGE���ֽ���
long m_Filetotalpages;
HANDLE FileHandle;
uint16 time_count;

int32 far curpos;
uint8 far m_checkTimer;
WORD far mp3_readpagescount;// =0;
BYTE far ID3_TAGFlag;// =0;
DWORD far Sync_HeadValue[Sync_Head_MaxCount];
BYTE far Sync_HeadCount;// =0;
WORD far FrameSize[Sync_Head_MaxCount];
DWORD far FramePos[Sync_Head_MaxCount];
BYTE far FrameByterate[Sync_Head_MaxCount];//֡��Ӧ��������ռ�ֽ�����bitrate>>3

char far Sync_HeadFindFlag;// =0;
DWORD far curframehead;

uchar far dataBuferTwo[2];

UCHAR far ID3Header[10];

int16 m_getpage(void);
#pragma ROM(large)
void time_count_fun(void);
#pragma ROM(huge)
extern int mp3GetTotaltime(uint32 *pMp3frameoffset, time_t *pTotaltime);
extern WORD mp3Getframesize(DWORD curframehead);

/*
 ********************************************************************************
 *             m_getpage
 *
 * Description :  �Ӹ����ļ��л��һ�����������ݵ�databuf;
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
int16 m_getpage(void)
{
    long curfilepos;
    BOOL result;

    curfilepos = FS_FTell(FileHandle);
    result = FS_FRead(GET_REAL_ADDR(databuf), 1, FileHandle);

    if (result == 0)
    {
        return 0;
    }
    if ((curfilepos + 1) == m_Filetotalpages)
    {
        return EndPageLen;
    }
    return BSPAGESize;
}

/*
 ********************************************************************************
 *             mp3CheckType
 *
 * Description :  �������飬���ø����Ƿ�����Ч��
 *
 * Arguments   :  �������Ч�����ͣ�pMp3frameoffset��ŵ�һ֡��ʵĵ�ַ��pTotaltime�Ǹ�������ʱ�䣬��ȷ��
 curpos�Ǿ�ȷ���ļ���дָ�룬bufindex������buffer�Ķ�дָ��
 *
 * Returns     : ��ȷ����0�����󷵻�1
 *
 * Notes       :
 *
 ********************************************************************************
 */
#ifndef PC
BYTE smp3CheckType(const uint8 *filename, uint32 *pMp3frameoffset, time_t *pTotaltime, audio_format_t *formatParameter)
#else
BYTE mp3CheckType(const uint8 *filename, uint32 *pMp3frameoffset, time_t *pTotaltime, audio_format_t *formatParameter)
#endif
{
    int16 i, bufindex = 0, datalen;
    uint16 bitRate;
    uchar flagData;
    BOOL result;
    BOOL crossPage = FALSE;
    DWORD ID3_datasize, curframepos;

    BankFlush(FLUSH_BANK_CODEC);
    mp3_readpagescount = 0;
    Sync_HeadCount = 0;
    ID3_TAGFlag = 0;//�ҵ�ID3����1
    Sync_HeadFindFlag = 0;
    databuf = (BYTE*) (GET_REAL_ADDR(BUFFER_ADDRESS));
    curpos = 0;
    time_count = 0;

    FileHandle = NULL;
    FileHandle = FS_FOpen(filename, FS_OPEN_NORMAL);//֧������seek����һҳ������seekһ������
    if (FileHandle == NULL)
    {
        return 1;
    }

    EndPageLen = FS_GetUnalignedLen(FileHandle);//ȡ���һ��PAGE��BYTE��
    m_Filetotalpages = (long) FS_GetFileLen(FileHandle);

    for (i = 0; i < Sync_Head_MaxCount; i++)
    {
        Sync_HeadValue[i] = 0xFFFFFF;
        FrameSize[i] = 0;
        FramePos[i] = 0;
    }

    m_checkTimer = TM_SetTimer((int) time_count_fun, TWO_MS);
    datalen = m_getpage();

    if (datalen <= ID3V2_HEADER_LEN)
    {
        FS_FClose(FileHandle);
        TM_KillTimer(m_checkTimer);
        return CHECK_NOPASS;
    }

    //����ID3�����������:1û��ID3V2��2��һ��ID3V2,3�кܶ��ID3V2
    while ((!ID3_TAGFlag))
    {
        ClearWatchDog();
        crossPage = FALSE;
        i = datalen - bufindex;
        if (i < ID3V2_HEADER_LEN)
        {
            memcpy(ID3Header, databuf + bufindex, (uint16)i);
            datalen = m_getpage();
            if (datalen == 0)
            {
                break;
            }
            crossPage = TRUE;
            memcpy(ID3Header + i, databuf, ((uint16)(ID3V2_HEADER_LEN - i)));
        }
        else
        {
            memcpy(ID3Header, databuf + bufindex, ID3V2_HEADER_LEN);
        }

        tvalue = ((DWORD) ID3Header[0] << 16) + ((DWORD) ID3Header[1] << 8) + (DWORD) ID3Header[2];
        if (ID3tag == tvalue)//ǰ�������ַ���"id3",��һ�����:����ID3V2
        {
            //֡���Ƿ�Ҫ����֡ͷ��10�ֽ�?
            ID3_datasize = (((DWORD) ID3Header[6] << 21) + ((DWORD) ID3Header[7] << 14) + ((DWORD) ID3Header[8] << 7)
                    + (DWORD) ID3Header[9]) + 10;//id3len+ id3 struct size
            curpos += ((int32)ID3_datasize);//risk, if too big, would exit when wav_count>150

            if (ID3_datasize >= ((uint32)(BSPAGESize - bufindex)))
            {
                ID3_datasize = (ID3_datasize + ((uint32)bufindex)) >> 9;
                ID3_datasize--; //fileSystem seek 1 page after fread
                if (ID3_datasize >= 1)
                {
                    result = FS_FSeek(((int32)ID3_datasize), 1, FileHandle);
                    if (result == 0)
                    {
                        FS_FClose(FileHandle);
                        TM_KillTimer(m_checkTimer);

                        return CHECK_NOPASS;
                    }
                    datalen = m_getpage();
                    if (datalen == 0)
                    {
                        break;
                    }
                }
                else if (FALSE == crossPage)//((ID3_datasize == 0) && (!crossPage))
                {
                    datalen = m_getpage();
                    if (datalen == 0)
                    {
                        break;
                    }
                }
                else
                {
                }

            }

            bufindex = curpos & 0x1ff;
        }
        else
        {
            ID3_TAGFlag = 1;//����ID3�������Ѿ�����������һҳ
        }
    }

    if (FALSE != crossPage)
    {
        result = FS_FSeek(2, 2, FileHandle);
        if (result == 0)
        {

            FS_FClose(FileHandle);
            TM_KillTimer(m_checkTimer);

            return CHECK_NOPASS;
        }
        datalen = m_getpage();
        if (datalen == 0)
        {
            FS_FClose(FileHandle);
            TM_KillTimer(m_checkTimer);
            return CHECK_NOPASS;
        }
    }
    //�Ѿ������һ������֡
    while (time_count < TIME_COUNT_MAX)
    {
        //sync head
        ClearWatchDog();//bufindex��0��ʼ��0-511  datalen = 512
        while ((bufindex < datalen) && (time_count < TIME_COUNT_MAX))
        {
            ClearWatchDog();
            if (bufindex < 0)
            {
                flagData = dataBuferTwo[2 + bufindex];
            }
            else
            {
                flagData = databuf[bufindex];
            }
            if (flagData != 0xFF)//ͬ���ź�
            {
                bufindex++;//bufindex����һֱ�ӵ�����datalen����˻�������ҳ��
                curpos++;
                continue;
            }
            bufindex++;
            curpos++;

            i = datalen - bufindex;
            if (i < MP3_FRAME_HEAD)
            {
                if (i == 1)
                {
                    dataBuferTwo[1] = databuf[bufindex];
                }
                else if (i == 2)
                {
                    dataBuferTwo[0] = databuf[bufindex];
                    dataBuferTwo[1] = databuf[bufindex + 1];
                }
                else
                {
                }
                bufindex = -i;
                datalen = m_getpage();
                if (datalen == 0)
                {
                    FS_FClose(FileHandle);
                    TM_KillTimer(m_checkTimer);
                    return CHECK_NOPASS;
                }
            }

            if (bufindex >= 0)
            {
                curframehead = ((DWORD) databuf[bufindex] << 16) 
                               + ((DWORD) databuf[bufindex + 1] << 8)
                               + ((DWORD) databuf[bufindex + 2]);//24bit head
            }
            else if (bufindex == -1)
            {
                curframehead = ((DWORD) dataBuferTwo[1] << 16) 
                               + ((DWORD) databuf[0] << 8) 
                               + (DWORD) databuf[1];//24bit head
            }
            else if (bufindex == -2)
            {
                curframehead = ((DWORD) dataBuferTwo[0] << 16) 
                               + ((DWORD) dataBuferTwo[1] << 8) 
                               + (DWORD) databuf[0];//24bit head
            }
            else
            {
            }

            //curframehead=((DWORD)frameheader[0]<<16)+((DWORD)frameheader[1]<<8)+(DWORD)frameheader[2];//24bit head

            if ((curframehead & 0xE00000) != 0xE00000)//sync
            {
                continue;
            }
                
            if ((curframehead & 0x180000) == 0x080000)//ver reserved
            {
                continue;
            }
            if ((curframehead & 0x060000) == 0x000000)//layer reserved
            {
                continue;
            }
            if ((curframehead & 0x00F000) == 0x00F000)//bitrate reserved
            {
                continue;
            }            
            if ((curframehead & 0x000C00) == 0x000C00)//samplerate reserved
            {
                continue;
            }

            tvalue = curframehead & MP3HeadORa;//���������֡ͷ�����������һ����
            curframepos = (uint32)curpos;
            for (i = Sync_HeadFindFlag - 1; i >= 0; i--)
            {
                //improve_wuyueqian
                //�����ǿ���������ǰ���������Ϣ�ģ����Լ�¼������Ϊֹ����ȷ֡ͷ
                if (tvalue == Sync_HeadValue[i])
                {
                    if ((curframepos - FramePos[i]) != FrameSize[i])
                    {
                        continue;//�п����м���Щ��������֡ͷ���������ݡ����Ҫ��ȥ
                    }
                        
                    Sync_HeadCount++;
                    Byterate += FrameByterate[i];
                    curframepos = FramePos[i];

                    if (Sync_HeadCount == Sync_Head_MaxSameCount)
                    {
                        *pMp3frameoffset = FramePos[i] - 1;//������֡�ĵڶ����ֽڼ�ȥ1����ָʾ����֡�Ŀ�ʼ
                        Byterate /= Sync_HeadCount;

                        bitRate = ((uint16) Byterate) << 3;
                        formatParameter->bit_rate[0] = ((uint8)(bitRate / 100)) + '0';
                        bitRate %= 100;
                        formatParameter->bit_rate[1] = ((uint8)(bitRate / 10)) + '0';
                        bitRate %= 10;
                        formatParameter->bit_rate[2] = ((uint8)(bitRate)) + '0';
                        formatParameter->bit_rate[3] = 0;

                        mp3GetTotaltime(pMp3frameoffset, pTotaltime);

                        TM_KillTimer(m_checkTimer);

                        if (!FS_FClose(FileHandle))
                        {
                            FileHandle = NULL;
                            return CHECK_NOPASS;
                        }
                        FileHandle = NULL;
                        return CHECK_PASS;
                    }
                }
            }

            Sync_HeadCount = 0;//����4֡�Ծ���Ϊ����ȷ��
            Byterate = 0;
            if (Sync_HeadFindFlag < Sync_Head_MaxCount)
            {
                Sync_HeadValue[Sync_HeadFindFlag] = tvalue;
                FramePos[Sync_HeadFindFlag] = (uint32)curpos;//curpos��ʾ��λ����0xff����һ���ֽڣ�������֡�ĵڶ����ֽ�
                FrameSize[Sync_HeadFindFlag] = mp3Getframesize(curframehead);
                FrameByterate[Sync_HeadFindFlag] = (uint8)(bitrate >> 3);
                Sync_HeadFindFlag++;
                continue;
            }
            else
            {
                //improve_wuyueqian
                //���Խ��мӴ�������1-32  5-36 ����������������Ҫ�޸�
                //��ֻ�Ǻ�С�Ŀ����ԡ���˲��ٸĶ���������Ҫ�ٸ�
                Sync_HeadFindFlag = 0;
                continue;
            }

        }//while sync head

        //�������ҳ����δ�ﵽ300���룬������һҳ����������
        datalen = m_getpage();
        if (datalen <= 0)
        {
            break; //����ҳβֱ���˳�
        }
            
        bufindex = 0;
        mp3_readpagescount++;
        if (mp3_readpagescount > BSPAGEMaxErrorNum)
        {
            FS_FClose(FileHandle);
            TM_KillTimer(m_checkTimer);

            return CHECK_NOPASS;
        }
    }//while(1)

    //����300���룬���ٽ��м�飬ֱ������Ϊ��Ч�ļ�
    FS_FClose(FileHandle);
    TM_KillTimer(m_checkTimer);

    return CHECK_NOPASS;
}

#pragma ROM(large)
void time_count_fun(void)
{
    time_count++;
}
#pragma ROM(huge)

