/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: deal the play status of music playing
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */

#include "ap_music.h"
#pragma name(AP_SUB_PLY)

uint16 DealFilePlayEnd(void);
uint16 DealKeyResult(uint8 key);
//extern void ui_show_loop(uint16 id);
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
uint16 DealKeyForPlayStatus(uint8 key)
{
    switch (key)
    {
        case AP_MSG_STANDBY: /*����״̬����Ӧ�Զ��ػ�*/
            if  (g_comval.playpowerofflag)
                break;
            else
                return RESULT_STANDBY;
     
        case AP_KEY_NEXT | AP_KEY_LONG: //���
        ui_run_realtime = FALSE;
        if(Pedo_ui_draw)
                break;
            else
            {
        	    g_result = ChangeVolume();			
        		if (g_result == RESULT_UDISK)
                {
                    return g_result;
                }
        		volume_callback(g_music_vars.vol_display);
        		SetFullInterface();
        		need_draw = TRUE;
        		return_to_play = 1;
    		}
        break;

        case AP_KEY_PREV | AP_KEY_LONG: //����
        ui_run_realtime = FALSE;
        if(Pedo_ui_draw)
                break;
            else
            {
        	    g_result = ChangeVolume();			
        		if (g_result == RESULT_UDISK)
                {
                    return g_result;
                }
        		volume_callback(g_music_vars.vol_display);
        		SetFullInterface();
        		need_draw = TRUE;
        		return_to_play = 1;
    		}
        break;

        case AP_KEY_PREV | AP_KEY_UP:
        ui_run_realtime = FALSE;
        return_to_play = 1;
       if((PlayStatus == PlaySta) && ((g_music_vars.BreakPTDisTime.hour > 0) || (g_music_vars.BreakPTDisTime.minute> 0) || (g_music_vars.BreakPTDisTime.second > 10)))
		{
			SelectCurMusic();
        	need_draw = TRUE;
        	while (ap_get_message() != NULL)
        	{
            	; //clear rtc message.
        	}
			break;
		}
		else
		{
            g_result = play_key_prev();
            need_draw = TRUE;
            while (ap_get_message() != NULL)
            {
                ; //clear rtc message.
            }
            return g_result;
        }     

        case AP_KEY_NEXT | AP_KEY_UP:
        ui_run_realtime = FALSE;
        return_to_play = 1;
      	g_result = (uint16) play_key_next(key);
        need_draw = TRUE;
       	while (ap_get_message() != NULL)
        {
            ; //clear rtc message.
        }
        return g_result;

        case AP_KEY_MODE | AP_KEY_UP: //�̰� MODE +
        if (RePlayStatus == NormalPlay)
        {
            return_to_play = 1;
            //ui_auto_select = TRUE;
            g_result = ui_play_menu(FALSE); //���Ų˵�
            //ui_auto_select = FALSE;
            SetFullInterface();
            if (g_result == RESULT_RECORD)//����ӦAB��
            {
                UpDisplayFlag = UpDisplayFlag & clr_musicbitrate;
                return play_key_next(AP_KEY_AB | AP_KEY_UP);
            }
            else
            {  
               return g_result;
            }

        }
        else
        {
            musicpSendCommand(MC_SETA_B, ClrABPoint); //clear SET_A_B
            RePlayStatus = NormalPlay;
            UpDisplayFlag = UpDisplayFlag | up_MPRLmode | up_musictypeflag;
            g_FCexitflag = FALSE;
//            if (g_currentMusicType != Music_Wav)
                ui_show_loop(IM_MUSIC_PLAYTIME_BG);
//            else
//                ui_show_d_pic(IM_MUSIC_REPEATAB_CLEAR, 1);
			SetFullInterface();
			need_draw = TRUE;
        }

        break;

        /*
        case AP_KEY_MODE | AP_KEY_LONG://����MODE ��������ʾ����
        #ifdef OPT_Sup_LRC_fun//֧�ָ����ʾ����
        if (!g_lyricflag)
        {
            return RESULT_MAIN; //û��ʾͲ�����
        }
        if (RePlayStatus != NormalPlay)
        {
            return RESULT_MAIN;
        }
        return_to_play = 1;
        g_result = ui_lyric();
        //�ָ�����ʱ��
        VMRead(& g_comval, VM_SYSTEM, sizeof(g_comval));
        ap_message_init(& g_comval);
        if( g_result != 0 ) //������ȼ�,��Ҫ����.

        {
            SetFullInterface();
            need_draw = TRUE;
        }
        return g_result;
        #endif
        break;
        */

        case AP_KEY_PLAY | AP_KEY_UP:
        return DealPlayKey();

        case AP_KEY_VOLSUB:
        case AP_KEY_VOLADD:
            if(Pedo_ui_draw)
                break;
            else
            {
        	    g_result = ChangeVolume();			
        		if (g_result == RESULT_UDISK)
                {
                    return g_result;
                }
        		volume_callback(g_music_vars.vol_display);
        		SetFullInterface();
        		need_draw = TRUE;
        		return_to_play = 1;
    		}
            break;
        /*
		case AP_KEY_AB | AP_KEY_UP:
        g_result = ChangeVolume();            
        if (g_result == RESULT_RECORD)//����ӦAB��
        {
            g_result = play_key_next(AP_KEY_AB | AP_KEY_UP);
            if (g_result == RESULT_UI_STOP)
            {
                return 0;
            }
            if (g_result != 0)
            {
                return g_result;
            }
            SetFullInterface();
            need_draw = TRUE;
        }
        else
        {
            if (DealFilePlayEnd()) //�����ŵ��ļ�β�����
                return g_result;
            if (g_result != 0)
            {
                return g_result;
            }
        }
		SetFullInterface();
        need_draw = TRUE;
        break;
               
		case AP_KEY_AB | AP_KEY_LONG: //����¼��
        	return RESULT_REC_NOSTART;
		*/
		
        default:
        key = ap_handle_hotkey(key);
        if (key == RESULT_REDRAW)
        {
            SetFullInterface();
            need_draw = TRUE;
            return_to_play = 1;
        }
        else if (key == RESULT_AUTO_BACK)
        {
            return NULL;
        }
        else if (key != 0)
        {
            /*����������ʱ����USB����Ӧ������*/
            if ((PlayStatus == FFPlaySta) || (PlayStatus == FBPlaySta))
            {
                musicpSendCommand(MC_STEP, 0);
                PlayStatus = PlaySta;
                UpDisplayFlag = UpDisplayFlag | up_musicbattery;
                need_draw = TRUE;
            }
            return key;
        }
        else
        {
        }
        break;
    }
    return NULL;

}

uint16 play_key_next(uint16 key)
{
    wma_play_status_t status_buf;
    if ((PlayStatus == FFPlaySta) || (PlayStatus == FBPlaySta))
    {
        musicpSendCommand(MC_STEP, 0);
	    UpDisplayFlag = UpDisplayFlag | up_musicbattery;
        PlayStatus = PlaySta;
    }
    else
    {
        switch (RePlayStatus)
        {
            case NormalPlay:
            if (key == (AP_KEY_AB | AP_KEY_UP)) //ab key
            {
                musicpSendCommand(MC_SETA_B, SetAPoint); //SET_A
            }
            else //next key
            {
#ifdef OPT_Sup_FadeOut_fun
                FadeOut(TRUE);
#endif
                g_result = ToNextMusic(Play_key);
                if (g_result != 0)
                {
                    return g_result;
                }
            }
            break;

            case WaitSetAPointForAB:
            musicpSendCommand(MC_SETA_B, SetAPoint); //SET_A
            break;

            case WaitSetBPointForAB:
            if ((g_ABMinTimeCNT == 0) && (musicpSendCommand(MC_SETA_B, SetBPoint) != 0)) //Ҫ����AB��Сʱ��
            {
                while (1)
                {
                    musicpSendCommand(MC_GETSTATUS, (void*) (&status_buf));
                    if (status_buf.signal == SIGNAL_REACH_B)
                    {
                        musicpSendCommand(MC_SETA_B, (void*) StopBPlay);
                        musicpSendCommand(MC_SETA_B, (void*) ReturnApoint);
                        break;
                    }
                }

            }
            break;

            case ABLoopPlay:
#ifdef OPT_Sup_follow_fun//֧�ָ����Աȹ���,APE/MPC��ʽ�ļ���֧�ֶԱȶ���������
            {
                musicpSendCommand(MC_GETSTATUS, (void*)(& status_buf));
                if (status_buf.status == PLAYING_REACH_A)
                {
                    //musicpSendCommand(MC_CALTIME, NULL);
                    musicpSendCommand(MC_PLAY, NULL);
                }
                g_FCexitflag = FALSE;
                //ui_auto_select=TRUE;
                g_result = ui_follow_compare();
                FS_FRemove( FollowReadTempFileName );// ɾ��¼����ʱ�ļ���֮ǰ��music ap�˳�ʱɾ����
                //ui_auto_select=FALSE;
                g_music_vars.MusicOpenMod = Nor_Break_open;//�����Զϵ㷽ʽ�˳���
                g_ReplayTimeCNT = 0;
                if (g_result != 0)
                {
                    return g_result;
                }
            }
#else    
            musicpSendCommand(MC_SETA_B, ClrABPoint);
            RePlayStatus = WaitSetAPointForAB;
#endif

            SetFullInterface();
            if (RePlayStatus != NormalPlay)
            {
               // UpDisplayFlag = UpDisplayFlag & clr_musicbitrate;
            }
            break;

            default:
            break;
        }
    }
    need_draw = TRUE;
    return NULL;
}

//======��������ʱ��PREV������========
uint16 play_key_prev(void)
{
    if (RePlayStatus != NormalPlay)
    {
        musicpSendCommand(MC_SETA_B, ClrABPoint); //clear AB
        RePlayStatus = WaitSetAPointForAB;
        UpDisplayFlag = UpDisplayFlag | up_MPRLmode ;
        need_draw = TRUE;
        return NULL;
    }
    switch (PlayStatus)
    {
        case FBPlaySta:
        case FFPlaySta:
        musicpSendCommand(MC_STEP, 0);
        UpDisplayFlag = UpDisplayFlag | up_musicbattery;
        PlayStatus = PlaySta;
        break;

        case PlaySta:
#ifdef OPT_Sup_FadeOut_fun
        FadeOut(TRUE);
#endif
        g_result = PrevMusicInPlay();
        if (g_result != 0)
        {
            return g_result;
        }
        break;

        default:
        break;
    }
    return NULL;
}

uint16 PrevMusicInPlay(void)
{
    uint8 key;
    musicpSendCommand(MC_STOP, NULL);
    musicpSendCommand(MC_CLOSE, NULL);

    findfileagain: while ((key = ap_get_message()) != NULL)
    {
        switch (key)
        {
            case AP_KEY_PLAY | AP_KEY_UP:
            return RESULT_UI_STOP;
            break;
            default:
            break;
        }
    }
    ClearWatchDog();

    g_result = ChangeMusic(Music_Prev, Play_key);

    if (g_result == 0)
    {
        return RESULT_UI_STOP;//�ص�ֹͣ����
    }

    GetInformation(); //ȡ�ļ���Ϣ

    if (g_Openparam.typeerror == 1)
    {
        DisplayErrMsg(MFMTERR);//��ʾ��ʽ��
        SetFullInterface();
        need_draw = 1;
        RefreshMainScreen();
        {
            goto findfileagain;
        }
    }

    g_music_vars.MusicOpenMod = Nor_open;
    musicpSendCommand(MC_OPEN, (void*) &g_Openparam);
    g_result = musicpSendCommand(MC_LOAD, NULL);

    if (g_result == 0)
    {
        DisplayErrMsg(MFMTERR);//��ʾ��ʽ��
        SetFullInterface();
        need_draw = 1;
        RefreshMainScreen();
        {
            goto findfileagain;
        }

    }

    SetFullInterface();

    if (return_to_play == 0)
    {
    //    UpDisplayFlag = UpDisplayFlag & clr_musicBackGDPic; //by zxs for not refresh the bg
    //    RefreshMainScreen();//�������һ��������ʾֹͣͼ��Ȼ������ʾ����ͼ�������
    }
    musicpSendCommand(MC_PLAY, NULL);
    return NULL;
}

uint16 DealPlayKey(void)
{
    if (g_FCexitflag != 0)
    {
        musicpSendCommand(MC_SETVOLUME, 0);
        OutPutVolume(0);
        g_FCexitflag = FALSE;
        musicpSendCommand(MC_FADEINOUT, FADEOUT2_DEF);//���ÿ���FADEOUT
    }
    if (RePlayStatus != NormalPlay)
    {
        musicpSendCommand(MC_SETA_B, ClrABPoint);
        RePlayStatus = NormalPlay;
    }
    g_FCexitflag = FALSE;
    musicpSendCommand(MC_STOP, NULL);

    /*���ŵ��ļ�βʱ������ˢ��һ�µ�ǰʱ��ͽ�����*/
    BackupCurrentTime();
	//ClearTimePara();
    musicpSendCommand(MC_BACKUP, NULL);
    musicpSendCommand(MC_CLOSE, NULL);
    g_music_vars.MusicOpenMod = Nor_Break_open;
    ifSelBuffToltalTimeFlag = TRUE;
    PlayStatus = PauseSta; // zxs add for not refresh the background
     UpDisplayFlag = UpDisplayFlag | up_musicloopmode;
    if ((g_music_vars.location.disk == 'H') && (DRV_DetectUD(1) != 0x20))
    {
        g_music_vars.location.disk = 'C';
        PlayStatus = StopSta;
    }
    return RESULT_UI_STOP;//�˻ص�ֹͣ״̬
}

uint16 DealFilePlayEnd(void)
{
    uint16 g_result_bak;
    music_play_status_t status_buf;
    g_result_bak = g_result;
    if (g_result == RESULT_MUSIC_PLAY)
    {
        return 0;
    }
    musicpSendCommand(MC_GETSTATUS, (void*) (&status_buf));
    if (status_buf.status == PLAYING_REACH_END)
    {
        g_result = DealMusicFileEnd(0);
        if (g_result != NULL)
        {
            if (g_result_bak != 0) //g_result_bak ����0����������Ϣ
            {
                g_result = g_result_bak;
                return g_result;
            }
            else
            {
                return g_result;
            }
        }
    }
    g_result = g_result_bak;
    return 0;
}

uint16 DealKeyResult(uint8 key)
{
    uint16 result;
    if (RESULT_SD_OUT == g_result)
    {
        g_music_vars.location.disk = 'C';
        return RESULT_UI_STOP2;
    }
    else if ((key == RESULT_UI_STOP) || (key == RESULT_LIST)) //test
    {
        if (g_Openparam.typeerror != 0)
        {
            musicpSendCommand(MC_CLOSE, NULL);
        }
        return RESULT_UI_STOP;
    }

    else if (((key == RESULT_IGNORE)||(key == RESULT_AUTO_BACK))!=0)
    {
        SetFullInterface();
        if (PlayStatus != PlaySta)
        {
            return RESULT_UI_STOP;
        }
        return NULL;
    }
    else if (key == RESULT_MAIN)
    {
        result = (int16) key;
        return result;
    }
    else
    {
        return key;
    }
}
uint16 ChangeVolume(void)
{
	slider_t slider;
    ClearScreen(NULL);
//    ResShowMultiString(CHANGEVO, 0, 0);
    UpdateScreen(NULL);
    slider.min = 0;
    slider.max = g_comval.VolumeMax;
    slider.value = g_music_vars.vol_display;
    slider.step = 1;
    //ui_auto_update = FALSE;
    g_result = ui_slider(&slider, volume_callback);
    g_music_vars.vol_display = slider.value;
    //ui_auto_update = TRUE;
    StartIntro(FALSE);
    if (g_result != 0)
    {   
      if(g_result==RESULT_IGNORE)
	  	g_result=0;
        return g_result;
    }
    return 0;
}
 

void volume_callback(uint16 Vol)
{
    g_music_vars.vol_display = (uint8) Vol;
    g_music_vars.volume = (uint8) Vol ;//* SOFTVOL_MAX / g_comval.VolumeMax;
    musicpSendCommand(MC_SETVOLUME, (void*) g_music_vars.volume);
}

