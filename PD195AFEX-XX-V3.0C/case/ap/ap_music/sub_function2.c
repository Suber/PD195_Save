/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: sub functions 2 of music
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"
#define MusicExtNum 6

extern bool Mp3Get_ldb_Load(const uint8 * strFile, uint8 * strID3);
extern bool WMAGet_ldb_Load(const uint8 * strFile, uint8 * strID3);
extern void Deal_UniID3Infor(bool mp3flag, bool longnameflag, bool uni_flag);
extern void MakeShortName(uint8 *str);

#pragma name(AP_SUB_FUN2)

const uint8 ExtMusicstr[MusicExtNum][4] =
{ "MP1", "MP2", "MP3", "WMA", /*"ASF",*/"WMV", "WAV" };

uint8 far LyricID3Buf[92];
void DealTheFirstSpace(uint8 *String, uint8 Size);
/*
 ******************************************************************************
 *             Music_type_t CheckMusicType(uint8   *filename)
 *
 * Description : 检测当前是哪种类型的MUSIC文件
 *
 * Arguments   : uint8   *filename  文件名
 *
 * Returns     : Music_type_t 文件类型
 *
 * Notes       :
 *
 ********************************************************************************
 */
Music_type_t CheckMusicType(uint8 * filename)
{
    uint8 cnt;
    uint8 ext[4];
    Music_type_t ret;
    memcpy(ext, filename + 8, 3);
    ext[0] = ((ext[0] >= 'a') && (ext[0] <= 'z')) ? (ext[0] - 'a' + 'A') : ext[0];
    ext[1] = ((ext[1] >= 'a') && (ext[1] <= 'z')) ? (ext[1] - 'a' + 'A') : ext[1];
    ext[2] = ((ext[2] >= 'a') && (ext[2] <= 'z')) ? (ext[2] - 'a' + 'A') : ext[2];
    ext[3] = 0;

    for (cnt = 0; cnt < Music_TypeMax; cnt++)
    {
        if (strcmp(ext, ExtMusicstr[cnt]) == 0)
        {
            ret = Music_NON + cnt + 1;
            if ((ret == Music_MP1) || (ret == Music_MP2))
            {
                return Music_MP3;
            }
            else
            {
                return ret;
            }
        }
    }
    return Music_NON;
}

/*在ID3/Tag信息字符串资源中插入空格字符*/
void DealTagDataBuffer(void)
{
    strcpy(g_TagInfoBuf, &g_TagInfoBuf[0]);
    strcat(g_TagInfoBuf, " ");
    strcat(g_TagInfoBuf, &g_TagInfoBuf[30]);
    strcat(g_TagInfoBuf, " ");
    strcat(g_TagInfoBuf, &g_TagInfoBuf[60]);
    strcat(g_TagInfoBuf, " ");
}
/*
 ********************************************************************************
 *             uint16 GetInformation(void)
 *
 * Description : 读取歌曲信息,歌词
 *
 * Arguments   : void
 *
 * Returns     : int
 *
 * Notes       :
 *
 ********************************************************************************
 */
void GetInformation(void)
{
    bool unicode_flag = FALSE;
	
	#ifdef OPT_Sup_LRC_fun
    uint32 lEntry;
	#endif
	
    bool mp3_unicdoe = FALSE;
    bool long_name_flag = FALSE;
    uint8 result;
	
	#ifdef OPT_Sup_LRC_fun
    uint8 g_lyric_mod;
	#endif
	
    ID3Info_t ID3;
    audio_format_t pFormat;
    g_currentMusicType = CheckMusicType(g_music_vars.location.filename);
    memset(g_TagInfoBuf, 0, 92);
    ID3.TIT2_length = 30;
    ID3.TPE1_length = 30;
    ID3.TALB_length = 30;
	  ID3.GENRE_length = 0;
    ID3.TrackNum_length = 0;
	  ID3.TENC_length = 0;
    ID3.TIT2_buffer = &g_TagInfoBuf[0];
    ID3.TPE1_buffer = &g_TagInfoBuf[30];
    ID3.TALB_buffer = &g_TagInfoBuf[60];
	  ID3.GENRE_buffer = &g_TagInfoBuf[90];
    ID3.TrackNum_buffer = &g_TagInfoBuf[90];	
    ID3.TENC_buffer = &g_TagInfoBuf[90];
    g_Openparam.typeerror = 0;
    switch (g_currentMusicType)
    {
        case Music_WMA:
        g_Openparam.typeerror = wmaCheckType(/*g_music_vars.location.filename*/NULL, &g_Openparam.musicframeoffset,
                &g_Openparam.musictotaltime, &pFormat, &ID3);

        if ((g_Openparam.typeerror == 0)/*&&(wmaGetTAGInfo(g_music_vars.location.filename, &ID3))*/) //正常文件才操作
        {
            if ((*(uint16 *) g_TagInfoBuf != 0x0000) || (*(uint16 *) (g_TagInfoBuf + 30) != 0x0000)
                    || (*(uint16 *) (g_TagInfoBuf + 60) != 0x0000))
            {
                unicode_flag = TRUE;
            }
        }
        else
        {
            g_TagInfoBuf[0] = 0x0;
        }
/*#ifdef LDB_LRC//支持LDB歌词显示功能
        if(WMAGet_ldb_Load(g_music_vars.location.filename,g_TagInfoBuf))
        {
            g_LDBFlag |=(1<<1);
        }
        else
        {
            g_LDBFlag &=~(1<<1);
        }
#endif*/
        break;


        case Music_Wav:
        g_Openparam.typeerror = wavCheckType(/*g_music_vars.location.filename*/NULL, &g_Openparam.musicframeoffset,
                &g_Openparam.musictotaltime, &pFormat);
        break;

        
        case Music_MP3:
        g_Openparam.typeerror = mp3CheckType(/*g_music_vars.location.filename*/NULL, &g_Openparam.musicframeoffset,
                &g_Openparam.musictotaltime, &pFormat);
        BankFlush(FLUSH_BANK_BASAL | FLUSH_BANK_CODEC);
        if ((g_Openparam.typeerror == 0) && (mp3GetID3Info(/*g_music_vars.location.filename*/NULL, &ID3,
                g_comval.langid)))
        {

            if (((g_TagInfoBuf[0] == 0xff) && (g_TagInfoBuf[1] == 0xfe)) || ((g_TagInfoBuf[30] == 0xff)
                    && (g_TagInfoBuf[31] == 0xfe)) || ((g_TagInfoBuf[60] == 0xff) && (g_TagInfoBuf[61] == 0xfe))) // is unicode ?
            {
                mp3_unicdoe = TRUE;
                unicode_flag = TRUE;
            }
            else
            {
                //DealTagDataBuffer();
            }
        }
        else
        {
            g_TagInfoBuf[0] = 0x0;
        }

/*#ifdef LDB_LRC//支持LDB歌词显示功能
        if(Mp3Get_ldb_Load(g_music_vars.location.filename,g_TagInfoBuf))
        {
            g_LDBFlag |=(1<<1);
        }
        else
        {
            g_LDBFlag &=~(1<<1);
        }
#endif*/
        break;

        default:
        g_TagInfoBuf[0] = 0x0;
        break;
    }

    if (g_Openparam.typeerror == 0)
    {
        memcpy(TimeRateData_sav.TotalTime_sav, &g_Openparam.musictotaltime, sizeof(time_t));
        if ((pFormat.bit_rate[0] < '0') || (pFormat.bit_rate[0] > '9') || (pFormat.bit_rate[1] < '0')
                || (pFormat.bit_rate[1] > '9') || (pFormat.bit_rate[2] < '0') || (pFormat.bit_rate[2] > '9'))
        {
            pFormat.bit_rate[0] = '9';
            pFormat.bit_rate[1] = '9';
            pFormat.bit_rate[2] = '9';
        }
        TimeRateData_sav.Bitrate_sav[0] = (uint8) pFormat.bit_rate[0]; //保存用于跟读对比时显示
        TimeRateData_sav.Bitrate_sav[1] = (uint8) pFormat.bit_rate[1];
        TimeRateData_sav.Bitrate_sav[2] = (uint8) pFormat.bit_rate[2];
    }
    else
    {
        memset(&TimeRateData_sav.TotalTime_sav, 0, sizeof(time_t));
        TimeRateData_sav.Bitrate_sav[0] = '0'; //保存用于跟读对比时显示
        TimeRateData_sav.Bitrate_sav[1] = '0';
        TimeRateData_sav.Bitrate_sav[2] = '0';

    }

    DealTheFirstSpace(g_TagInfoBuf, sizeof(g_TagInfoBuf));
    //---处理没有TAG信息的情况
    if (!unicode_flag)
    {
        if (g_TagInfoBuf[0] == 0)
        {
            memset(g_TagInfoBuf, 0, 92);
            memcpy(g_TagInfoBuf, g_music_vars.location.filename, 12);
            result = (uint8) FS_GetName(g_TagInfoBuf, 15); //取出长名   //30
            if (result != 0)
            {
                long_name_flag = TRUE;
                unicode_flag = TRUE;
            }
            else
            {
                memcpy(g_TagInfoBuf, g_music_vars.location.filename, 12);
                MakeShortName(g_TagInfoBuf);
            }
        }

    }
    /*
    if (unicode_flag != 0)
    {
        Deal_UniID3Infor(mp3_unicdoe, long_name_flag, unicode_flag);
    }
    else
    {
        for (result = 0; result < sizeof(g_TagInfoBuf); result++)
        {
            if (g_TagInfoBuf[result] == 0)
            {
                g_TagInfoBuf[result] = ' ';
                g_TagInfoBuf[result + 1] = 0;
                break;
            }
        }

    }
    */
    if(unicode_flag )
    {
        
        tag_is_unicode = UNICODELANGUAGE;
    }
    else
    {
        tag_is_unicode = 0;
    }

#ifdef OPT_Sup_LRC_fun//支持歌词显示功能
    if(/*(!(g_LDBFlag&(1<<1)))&&*/(g_Openparam.typeerror == 0))
    {
        lEntry = FS_GetCurDirEntry();//文件项在当前目录项中的偏移
        g_lyricflag = lyricQueryExist(g_music_vars.location.filename);
        if (g_lyricflag)
        {
            if((g_TagInfoBuf[0] == 0xff) && (g_TagInfoBuf[1] == 0xfe))
            {
                memcpy(LyricID3Buf,&g_TagInfoBuf[2],sizeof(LyricID3Buf)-2);
                unitochar(LyricID3Buf, sizeof(LyricID3Buf)-2, g_comval.langid);
            }
            else
            {
                memcpy(LyricID3Buf,g_TagInfoBuf,sizeof(LyricID3Buf));
            }
            g_lyric_mod = lyricLoad(g_music_vars.location.filename, LyricID3Buf);
        }
        if (g_lyric_mod == 0)
        {
            g_lyricflag = FALSE;
        }
        FS_SetCurDirEntry(lEntry);
    }
    else if(g_Openparam.typeerror == 0) /*存在LDB格式歌词*/
    {
        g_lyricflag = TRUE;
    }
    else
    {
        g_lyricflag = FALSE;
    }
#endif //OPT_Sup_LRC_fun
}

uint8 get_endposition(uint8 *string_pt, uint8 string_len, uint8 type)
{
    uint8 i = 0;
    type = type;
    while (i < (string_len - 1))
    {
        if ((string_pt[i] == 0) && (string_pt[i + 1] == 0))
        {
            string_pt[i] = 0x20;
            string_pt[i + 1] = 0x00;
            break;
        }
        else
        {
            i += 2;
        }
    }
    if (i > (string_len - 1))
    {
        string_pt[i - 2] = 0x20;
        string_pt[i - 1] = 0x00;
        return (i - 2);
    }
    return i;
}

void endchar_add(uint8 end_position)
{

    if (end_position >= 88)
    {
        g_TagInfoBuf[end_position - 2] = 0x20;
        g_TagInfoBuf[end_position - 1] = 0x00;
    }
    else
    {
        end_position += 2;
    }
    g_TagInfoBuf[end_position] = 0x00;
    g_TagInfoBuf[end_position + 1] = 0x00;
}

/*
 ********************************************************************************
 *             void Deal_UniID3Infor(void)
 *
 * Description : 处理unicode的歌曲信息,歌词
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
void Deal_UniID3Infor(bool mp3flag, bool longnameflag, bool uni_flag)
{

    uint8 end_pos, end_pos_last;
    uint8 temp_buf[30];
    if (mp3flag != 0)
    {
        if ((g_TagInfoBuf[0] != 0xff) || (g_TagInfoBuf[1] != 0xfe))
        {
            end_pos = get_endposition(g_TagInfoBuf, 30, uni_flag);
            memcpy(temp_buf, g_TagInfoBuf, 30);
            memcpy(&g_TagInfoBuf[2], temp_buf, 28);
            g_TagInfoBuf[0] = 0xff;
            g_TagInfoBuf[1] = 0xfe;
            end_pos += 2;
        }
        else
        {
            end_pos = get_endposition(&g_TagInfoBuf[2], 28, uni_flag);
            end_pos += 4;
        }
        memcpy(g_TagInfoBuf + end_pos, &g_TagInfoBuf[32], 28);
        end_pos_last = end_pos;
        end_pos = get_endposition(&g_TagInfoBuf[end_pos_last], 28, uni_flag);
        end_pos += end_pos_last;
        end_pos_last = end_pos + 2;
        memcpy(g_TagInfoBuf + end_pos_last, &g_TagInfoBuf[62], 28);
        end_pos = get_endposition(&g_TagInfoBuf[end_pos_last], 28, uni_flag);
        end_pos += end_pos_last;
        endchar_add(end_pos);
    }
    else
    {

        if (longnameflag != 0)
        {
            end_pos = get_endposition(g_TagInfoBuf, 64, uni_flag);
            end_pos += 4;
            g_TagInfoBuf[end_pos] = 0x00;
            g_TagInfoBuf[end_pos + 1] = 0x00;
            end_pos -= 2;
            while (end_pos >= 2)
            {
                g_TagInfoBuf[end_pos] = g_TagInfoBuf[end_pos - 2];
                end_pos--;
            }
        }
        else
        {
            memcpy(temp_buf, g_TagInfoBuf, 30);
            memcpy(&g_TagInfoBuf[2], temp_buf, 28);
            end_pos = get_endposition(&g_TagInfoBuf[2], 28, uni_flag);
        }

        g_TagInfoBuf[0] = 0xff;
        g_TagInfoBuf[1] = 0xfe;
        if (!longnameflag)
        {
            end_pos += 4;
            memcpy(g_TagInfoBuf + end_pos, &g_TagInfoBuf[30], 30);
            end_pos_last = end_pos;
            end_pos = get_endposition(&g_TagInfoBuf[end_pos_last], 30, uni_flag);
            end_pos += end_pos_last;
            end_pos_last = end_pos + 2;
            memcpy(g_TagInfoBuf + end_pos_last, &g_TagInfoBuf[60], 30);
            end_pos = get_endposition(&g_TagInfoBuf[end_pos_last], 30, uni_flag);
            end_pos += end_pos_last;
            endchar_add(end_pos);
        }

    }
}

