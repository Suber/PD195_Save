/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: sub functions 4 of music
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"
#pragma name(AP_SUB_FUN4)
uint8 temp_buffer[9] ={ 0 };


void SetStopToPlayInterfaceForPlay(void)
{
    ui_auto_update = FALSE;
    UpDisplayFlag = UpDisplayFlag/* | up_musictypeflag | up_lyricflag | up_musicbitrate | up_musicloopmode
            | up_musicsequence | up_musiceqmode */| up_musicbattery;
}

/*�����Ź����еĴ���*/
void Deal_Error_Playing(void)
{
    uint8 key;
    do
    {
        key = ap_get_message();
        if (key == AP_MSG_SD_OUT)
        {
            break;
        }
    } while (key != NULL);
    if ((key == AP_MSG_SD_OUT) && (g_music_vars.location.disk == 'H'))
    {
        DisplayErrMsg(CARDPULL);
        g_music_vars.location.disk = 'C';
    }
    else if ((g_music_vars.location.disk == 'H') && (DRV_DetectUD(1) != 0x20))
    {
        g_music_vars.location.disk = 'C';
        ap_sleep(2);
    }
    else
    {
        if(g_music_vars.location.disk == 'H')
        {
            int8 rtc_count = 0;
            do
            {
                key = ap_get_message();
                if ((key == AP_MSG_RTC) || (key == AP_MSG_CHARGING))
                {
                    rtc_count++;
                }
                
                //check time out
                if (rtc_count >= 3)
                {
                    break;
                }
            }while (key != AP_MSG_SD_OUT);
            // ����Ƿ񿨰γ�������
            if(key == AP_MSG_SD_OUT)
            {
                DisplayErrMsg(CARDPULL);
                g_music_vars.location.disk = 'C';
            }
            else
            {

	
                DisplayErrMsg(MFMTERR);//��ʾ��ʽ��

		
            }
        }
        else
        {
            DisplayErrMsg(MFMTERR);//��ʾ��ʽ��
        }
    }

    musicpSendCommand(MC_STOP, NULL);
    musicpSendCommand(MC_CLOSE, NULL);
    SetFullInterface();
    need_draw = TRUE;
}

//��ѡ��ǰ����
void SelectCurMusic(void)
{
    musicpSendCommand(MC_STOP, NULL);
    musicpSendCommand(MC_CLOSE, NULL);
    ChangeMusic(Music_Cur, Play_play); //���ŵ�ǰ����
    GetInformation(); //ȡ�ļ���Ϣ
    if (g_Openparam.typeerror == 0)
    {
        musicpSendCommand(MC_OPEN, (void*) &g_Openparam);
        musicpSendCommand(MC_LOAD, NULL);
        musicpSendCommand(MC_PLAY, NULL);
    }
}
//========================================================

/*���˵��ļ�ͷ��ʾ*/
void DisInFBHead(uint8 DisMod)
{
    if (DisMod == 0)
    {//��ͨ��ʾģʽ
       // if (g_currentMusicType != Music_Wav)
            temp_buffer[0] = '0';
        temp_buffer[1] = '0';
        temp_buffer[2] = ':';
        temp_buffer[3] = '0';
        temp_buffer[4] = '0';
        temp_buffer[5] = ':';
        temp_buffer[6] = '0';
        temp_buffer[7] = '0';

       // SetTextPos(POSITION_F, 0);
      //  SetAsciiFont(FONT_TYPE_LARGE);
        //PutS(temp_buffer, -1); //��ͷ��ʾʱ��Ϊ0
//        if (g_currentMusicType != Music_Wav)
            ui_show_f(temp_buffer);
//       else
//            ui_show_ef(temp_buffer);
   //     SetAsciiFont(FONT_TYPE_DEFAULT);
        UpdateScreen(NULL);
    }
    else
    {//�����ʾģʽ
        region_t region;
        region.x = LyricIcon_Pos_X;
        region.y = LyricIcon_Pos_Y;
        region.width = 12;
        region.height = 8;
//        ResShowPic(LYRIC_R, LyricIcon_Pos_X, LyricIcon_Pos_Y);
        UpdateScreen(&region);
    }
    while (ap_get_message() != (AP_KEY_PREV | AP_KEY_UP))
    {
        ;//do nothing
    }
    //ChangeMusic(Music_Prev, Play_key);
    SelectCurMusic();
}

/*������ļ�β��ʾ*/
void DisInFFEnd(uint8 DisMod)
{
	if(!Pedo_ui_draw)
    {  
    if (DisMod == 0)
    {//��ͨ��ʾģʽ
        // wma_play_status_t status_buf;
       // musicpSendCommand(MC_GETSTATUS, (int) (&status_buf));
       // itoa2(TimeRateData_sav.TotalTime_sav[0], temp_buffer);
       // if (g_currentMusicType != Music_Wav)
        //    temp_buffer[0] = ' ';
     //   temp_buffer[2] = ':';
        itoa2(TimeRateData_sav.TotalTime_sav[1], &temp_buffer[0]);
        temp_buffer[2] = ':';
        itoa2(TimeRateData_sav.TotalTime_sav[2], &temp_buffer[3]);
        temp_buffer[5] = 0x00;
       // SetTextPos(POSITION_F, 0);
       // SetAsciiFont(FONT_TYPE_LARGE);
        //PutS(temp_buffer, -1); //��ͷ��βʱ��ʾ��ʱ��
//        if (g_currentMusicType != Music_Wav)
            ui_show_f(temp_buffer);
//        else
//            ui_show_ef(temp_buffer, 1);
      //  SetAsciiFont(FONT_TYPE_DEFAULT);
        UpdateScreen(NULL);
    }
    else
    {//�����ʾģʽ
        region_t region;
        region.x = LyricIcon_Pos_X;
        region.y = LyricIcon_Pos_Y;
        region.width = 12;
        region.height = 8;
//        ResShowPic(LYRIC_F, LyricIcon_Pos_X, LyricIcon_Pos_Y);
        UpdateScreen(&region);
	    }
    }
}

/*��������*/
void FadeOut(bool DisMod)
{
    music_play_status_t status_buf;
    DisMod = DisMod;
    while (1)
    {
        bool result = musicpSendCommand(MC_FADEINOUT, FADEOUT_DEF);
        if (result != 0)
        {
            break;
        }

        musicpSendCommand(MC_GETSTATUS, (void*) (&status_buf));
        if ((status_buf.status == PLAYING_REACH_END) || (status_buf.status == PLAYING_REACH_HEAD) || (status_buf.status
                == PLAYING_ERROR))
        {
            break;
        }
        if ((status_buf.status == PLAYING_WAIT_A) || (status_buf.signal == SIGNAL_REACH_B) || (status_buf.signal
                == SIGNAL_REACH_V))
        {
            musicpSendCommand(MC_SETA_B, ClrABPoint);
            break;
        }
    }
}

void CheckVolumeLimit(void)
{
    if (g_music_vars.volume > SOFTVOL_MAX)
    {
        g_music_vars.volume = SOFTVOL_MAX;
    }

    if (g_music_vars.volume < 0)
    {
        g_music_vars.volume = 0;
    }
}

void CheckPlayrateLimit(void)
{
    if (g_music_vars.play_rate > 8)
    {
        g_music_vars.play_rate = 8;
    }
    if (g_music_vars.play_rate < -8)
    {
        g_music_vars.play_rate = -8;
    }
}

void CheckReplaygapLimit(void)
{
    if (g_music_vars.replaygap < 1)
    {
        g_music_vars.replaygap = 1;
    }
    if (g_music_vars.replaygap > 10)
    {
        g_music_vars.replaygap = 10;
    }
}

void CheckReplaytimesLimit(void)
{
    if (g_music_vars.replaytimes < 1)
    {
        g_music_vars.replaytimes = 1;
    }
    if (g_music_vars.replaytimes > 10)
    {
        g_music_vars.replaytimes = 10;
    }
}
/*
 ********************************************************************************
 * Description :  �����ڲ���״̬�¶԰������жϴ�����Ϊui_play.c�ռ�̫�����ˣ�
 *
 * input : ��ֵ key
 *
 * output: ����ֵ == 0: ��������
 *                != 0: �����������أ����ȼ���
 * Notes       :
 ********************************************************************************
 */
uint16 Pedo_DealKeyForPlayStatus(uint8 key)   //pedometer
{
    uint16 result;
    
    switch (key)
    {
        case AP_MSG_STANDBY: /*����״̬����Ӧ�Զ��ػ�*/
            if  (g_comval.playpowerofflag)
                break;
            else
                return RESULT_STANDBY;
        
        case AP_KEY_MODE | AP_KEY_UP: //�̰� MODE +	
            result = Pedo_menu(Pedo_Swim_flag);
            need_draw = TRUE;
            return_to_play = 1;
            if(result == RESULT_RETURN_TO_MAIN)
            {
                SetFullInterface();
        		/*if(Pedo_work_flag == TRUE)
        			sKY_SetGsensor(1,0,0);
        		else*/
        		//sKY_SetGsensor(0,0,0);
        		//Pedo_work_flag = FALSE;
        		Pedo_ui_draw = FALSE;
                break;
            }
            if(result!=NULL)
                return result;
            break;
    			    
        case AP_KEY_PREV | AP_KEY_UP: 
            SetFullInterface();
    		/*if(Pedo_work_flag == TRUE)
    			sKY_SetGsensor(1,0,0);
    		else*/
    		//sKY_SetGsensor(0,0,0);
    		//Pedo_work_flag = FALSE;
    		Pedo_ui_draw = FALSE;
            need_draw = TRUE;
            return_to_play = 1;
    		break;
		
		case AP_KEY_PLAY | AP_KEY_UP:
		            if(Pedo_ui_draw)
        			{
        				if(Pedo_work_flag)
        				{
        					sKY_SetGsensor(0,0,0,Pedo_Swim_flag);
        					Pedo_work_flag = FALSE;
        				}
        				else if(Swim_work_flag)
        				{
        					sKY_SetGsensor(0,0,0,Pedo_Swim_flag);
        					Swim_work_flag = FALSE;
        				}
        				else
        				{
        					if(Pedo_Swim_flag)
        					{
        					    Swim_work_flag = TRUE;
        					    Pedo_work_flag = FALSE;
        					    sKY_SetGsensor(0,0,0,0);
        					    sKY_SetGsensor(1,0,0,Pedo_Swim_flag);
        					}
        					else
        					{
                                Pedo_work_flag = TRUE;
        					    Swim_work_flag = FALSE;
        					    sKY_SetGsensor(0,0,0,1);
        					    sKY_SetGsensor(1,0,0,Pedo_Swim_flag);
        					}
        				}
        				Pedo_RefreshDataScreen(Pedo_Swim_flag);
                    }
		    break;	
		
		//case AP_KEY_AB | AP_KEY_UP:
		case AP_KEY_NEXT | AP_KEY_UP:
		            sKY_SetGsensor(2,0,0,Pedo_Swim_flag);
	                if(Pedo_Swim_flag)
                    {
    	                Swim_total_sec = 0;
    		            Swim_total_step = 0;
    		        } 
    				else
    				{
    	                Pedo_total_sec = 0;
    		            Pedo_total_step = 0;
    		        }    
		//test_up = 0;
		//test_down = 0;
		need_draw = TRUE;
		break;

        default:
        key = ap_handle_hotkey(key);
        if (key == RESULT_REDRAW)
        {
           // SetFullInterface();
            need_draw = TRUE;
           // ui_auto_update = 0;
           // return_to_play = 1;
        }
        else if (key == RESULT_AUTO_BACK)
        {
            return NULL;
        }
        else if (key != 0)
        {
			//sKY_SetGsensor(0,0,0);
            return key;
        }
        else
        {
        }
        break;
    }
    return NULL;

}

