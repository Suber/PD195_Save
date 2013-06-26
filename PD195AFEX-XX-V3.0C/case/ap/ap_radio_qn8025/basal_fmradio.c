/*
 *******************************************************************************
 *                ACTOS AP
 *        FM tunner basal module implementation
 *
 *        (c) Copyright Actions Co,Ld.
 *
 *******************************************************************************
 */
#include "ap_radio.h"

#pragma name(MOD_BS_FMRADIO)

//-----basal module ----

uint16 m_FMStationTab[30]; //电台频率对照表(注:频率范围87M-108M 以K为精度).
fm_play_status_t m_FMPlaystatus; //当前电台信息
FMStatus_Tab_t d_FMStatus; //从FM硬件读取的当前频率和声道信息
extern fm_play_status_t FMStatus; //当前电台信息,8 bytes


/*********************************************************
 *input:  buf---ptr point to buffer which is wait to deal
 *          curfreq----current freq which will be inserted to buffer
 *          totalnum---current total freq num in freq list
 *output: no
 *Description:  insert current freq into freq list
 *note:  freq list from small to large
 **********************************************************
 */
void SequenceFreqlist(uint16* buf, uint16 curfreq, uint8 totalnum)
{
    uint8 i, cur_pos;
    uint16* dealbuf;
    int8 j;

    dealbuf = buf;
    cur_pos = 0;

    for (i = 0; i < 30; i++)
    {
        if (curfreq < (*dealbuf))
        {
            cur_pos = i;
            if (cur_pos == 29)
            {
                *dealbuf = curfreq;
                m_FMPlaystatus.station = i + 1;
                break;
            }

            for (j = 28; j >= (int8) cur_pos; j--)
            {
                *(buf + j + 1) = *(buf + j);
            }
            *dealbuf = curfreq;
            m_FMPlaystatus.station = i + 1;

            break;
        }
        else if (curfreq == (*dealbuf))
        {
            break; //有重复，不更新表
        }
        else
        {
            if ((*dealbuf == 0) && (i < totalnum))
            {
                *dealbuf = curfreq;
                m_FMPlaystatus.station = i + 1;
                break;
            }
            dealbuf++;
        }
    }
    return;
}

/*
 ********************************************************************************
 *             BOOL fmSendCommand(uint8 cmd, uint16 param)
 *
 * Description : FM命令处理
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint8 fmSendCommand(uint8 cmd, uint16 param)
{
    uint8 i;
    uint8 k;
	
    uint16 Tempfreq;
    switch (cmd)
    {
        case MC_OPEN:
        return FM_Init(); //FM初始化
		//return FM_Init(m_FMPlaystatus.mode, (uint8) param); //FM初始化
        case MC_PLAY:
        Tempfreq = m_FMPlaystatus.freq ;
        FM_SetFreq(Tempfreq); //设置电台频点
        break;

        case MC_STEP:
        if (m_FMPlaystatus.station_count == 0)
        {
            return FALSE;
        }
        k = 0;
        switch (param)
        {
            case UP: //找到下一电台频率
/*			Tempfreq = m_FMPlaystatus.freq;
			while(flag)
			{
				m_FMPlaystatus.freq ++;
				if(m_FMPlaystatus.station != 0)
				{
					flag = 0;
				}
			}*/
            if (m_FMPlaystatus.station >= m_FMPlaystatus.station_count)
            {
                m_FMPlaystatus.station = 0;
            }
            m_FMPlaystatus.station++;
            for (i = 0; i < 30; i++)
            {
                if (m_FMStationTab[i] != 0)
                {
                    k++;
                    if (k == m_FMPlaystatus.station)
                    {
                        break;
                    }
                }

            }
            if (i >= 30) // 如没有找到电台,重新设置电台数
            {
                m_FMPlaystatus.station_count = k - 1;
                m_FMPlaystatus.station = 1;
            }
            break;
            case DOWN: //找到上一电台频率
            if (m_FMPlaystatus.station == 1)
            {
                m_FMPlaystatus.station = m_FMPlaystatus.station_count + 1;
            }
            m_FMPlaystatus.station--;
            for (i = 0; i < 30; i++)
            {
                if (m_FMStationTab[i] != 0)
                {
                    k++;
                    if (k == m_FMPlaystatus.station)
                    {
                        break;
                    }
                }
            }
            if (i >= 30) // 如没有找到电台,重新设置电台数
            {
                m_FMPlaystatus.station = m_FMPlaystatus.station_count = k + 1;
            }
            break;
            default:
            break;
        }
        if (i < 30)
        {
            m_FMPlaystatus.freq = m_FMStationTab[i];
        }
        Tempfreq = m_FMPlaystatus.freq  ;
        FM_SetFreq(Tempfreq);
        return TRUE;

        case MC_STEPGRID:
        switch (param)
        {
            case UP:
            if (((m_FMPlaystatus.freq + 1) > 1080) && (m_FMPlaystatus.mode == 0))
            {
                m_FMPlaystatus.freq = 875;
            }
            else if (((m_FMPlaystatus.freq + 1) > 900) && (m_FMPlaystatus.mode == 1))
            {
                m_FMPlaystatus.freq = 760;
            }
            else
            {
                m_FMPlaystatus.freq += 1;
            }
            break;
            case DOWN:
            if (((m_FMPlaystatus.freq - 1 ) < 875) && (m_FMPlaystatus.mode == 0))
            {
                m_FMPlaystatus.freq = 1080;
            }
            else if (((m_FMPlaystatus.freq - 1 ) < 760) && (m_FMPlaystatus.mode == 1))
            {
                m_FMPlaystatus.freq = 900;
            }
            else
            {
                m_FMPlaystatus.freq -= 1;
            }
            break;
            default:
            break;
        }
        Tempfreq = m_FMPlaystatus.freq ;
		FM_Mute(SetMUTE);
        FM_SetFreq(Tempfreq);
		FM_Mute(releaseMUTE);
        return TRUE;

        case MC_SEARCH:
        Tempfreq = m_FMPlaystatus.freq ;
        //S_QND.ctrl = param;
        return FM_Search(&FMStatus, Tempfreq, (uint8) param);

        case MC_SAVESTATION:
        if (m_FMPlaystatus.station_count > 29)
            return FALSE;
        if (m_FMPlaystatus.station_count != 0) //当前电台在电台表中有保存的情况
        {
            for (i = 0; i < m_FMPlaystatus.station_count; i++)
            {
                if(m_FMStationTab[i] == m_FMPlaystatus.freq)
                {
                    m_FMPlaystatus.station = i+1;
                    return TRUE;
                }
            }
            m_FMStationTab[m_FMPlaystatus.station_count] = m_FMPlaystatus.freq;
            m_FMPlaystatus.station_count++;
            m_FMPlaystatus.station = m_FMPlaystatus.station_count;
            
        }
        else //新的电台频率保存到电台表
        {
            m_FMStationTab[m_FMPlaystatus.station_count] = m_FMPlaystatus.freq;
            m_FMPlaystatus.station_count++;
            m_FMPlaystatus.station = m_FMPlaystatus.station_count;
        }
        return TRUE;

        case MC_SETMODE:
        m_FMPlaystatus.mode = (Station_bank_t) param;
        break;

        case MC_SETVOLUME:
        FM_VolumeSet((uint8) param);
        m_FMPlaystatus.volume = (uint8) param;
        break;

        case MC_BACKUP:
#ifndef PC
        memcpy(((FMRADIO_param_t *) param)->param1, m_FMStationTab, 60);
        memcpy(((FMRADIO_param_t *) param)->param2, &m_FMPlaystatus, sizeof(fm_play_status_t));
#else
        memcpy((void *)param,m_FMStationTab, 60);
        memcpy((void *)(param+2),&m_FMPlaystatus, sizeof(fm_play_status_t));
#endif
        break;

        case MC_RESTORE:
#ifndef PC
        memcpy(m_FMStationTab, ((FMRADIO_param_t *) param)->param1, 60);
        memcpy(&m_FMPlaystatus, ((FMRADIO_param_t *) param)->param2, sizeof(fm_play_status_t));
#else
        memcpy(m_FMStationTab, (void *)param, 60);
        memcpy(&m_FMPlaystatus,(void *)(param+2), sizeof(fm_play_status_t));
#endif
        break;

        case MC_GETSTATUS:
        FM_GetStatus(&d_FMStatus);
        m_FMPlaystatus.freq = d_FMStatus.FM_CurrentFreq;
        m_FMPlaystatus.stereo_status = d_FMStatus.FM_Stereo_Status; //0: stereo;   1: mono
        k = 0;
//        //当前电台频率值和电台表中某一频率差距小于150KHZ，此电台和此电台表对应
        /*
        for (i = 0; i < 30; i++)
        {
            m_FMPlaystatus.station = 0;
            if (m_FMStationTab[i] != 0)
            {
                k++;
                if ((m_FMPlaystatus.freq < (m_FMStationTab[i] + 1)) && (m_FMPlaystatus.freq > (m_FMStationTab[i] - 1)))
                {
                    if (k <= m_FMPlaystatus.station_count)
                    {
                        m_FMPlaystatus.station = k;
                    }
                    break;
                }
            }
        }
        */
        memcpy((fm_play_status_t *) param, &m_FMPlaystatus, sizeof(fm_play_status_t));
        break;

        case MC_GETTABLE:
        memcpy((uint16 *) param, &m_FMStationTab[0], 60);
        break;

        case MC_DELONE:
        if (m_FMPlaystatus.station == 0)
        {
            return FALSE;
        }
        //m_FMStationTab[m_FMPlaystatus.station-1]=0;
        //for (k = m_FMPlaystatus.station; k < (m_FMPlaystatus.station_count + 1); k++)
        for (k = m_FMPlaystatus.station; k < (m_FMPlaystatus.station_count); k++)
        {
            m_FMStationTab[k - 1] = m_FMStationTab[k];
        }
        m_FMStationTab[m_FMPlaystatus.station_count - 1] = 0;
        m_FMPlaystatus.station_count--;
        m_FMPlaystatus.station = 0;
        return TRUE;

        case MC_DELALL:
        for (i = 0; i < 30; i++)
        {
            m_FMStationTab[i] = 0;
        }
        m_FMPlaystatus.station = 0;
        m_FMPlaystatus.station_count = 0;
        break;

        case MC_MUTE: //静音
#if 1
		FM_Mute(param);
#else
        i = 0x10;
        while ((!FM_Mute(param)) && (i > 0))
        {
            i--;
            ClearWatchDog();
            FM_GetStatus(&d_FMStatus);
        }
#endif
        break;

        case MC_CLOSE:
        FM_Standby();
        FM_Exit();
        break;

        case MC_GETFREQ:
        *(uint16 *) param = FMStatus.freq;
        break;

        case MC_SETFREQ:
        m_FMPlaystatus.freq = param;
        Tempfreq = m_FMPlaystatus.freq ;
        FM_SetFreq(Tempfreq);
        break;

        default:
        break;
    }

    return TRUE;

}

