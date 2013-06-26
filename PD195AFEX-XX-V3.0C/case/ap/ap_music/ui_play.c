/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: play status module
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"
#define IntCntValDef 10

extern uint16 Pedo_DealKeyForPlayStatus(uint8 key);   //pedometer

//extern bool g_light_flag;
extern uint8 VolChangeFlag;


#pragma name(AP_UI_PLY)
//比特率的小图标(由于比标准ASCII小，所以要另出一套图标)
const uint16 NUM_PIC[10] =
{IM_MUSIC_MP3_NUMBER_0,IM_MUSIC_MP3_NUMBER_1,IM_MUSIC_MP3_NUMBER_2,IM_MUSIC_MP3_NUMBER_3,IM_MUSIC_MP3_NUMBER_4,
 IM_MUSIC_MP3_NUMBER_5,IM_MUSIC_MP3_NUMBER_6,IM_MUSIC_MP3_NUMBER_7,IM_MUSIC_MP3_NUMBER_8,IM_MUSIC_MP3_NUMBER_9};



/*
 ********************************************************************************
 *             uint16 ui_play( void )
 *
 * Description : 显示时间，滚屏,AB闪
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 ui_play(void)
{
    uint8 key,time_to_step = 0;
	//uint8 time_wate;
	uint16 scrollcount=0;
    music_play_status_t status_buf;
    PlayStatus = PlaySta;
    RePlayStatus = NormalPlay;/* 播放状态 */
    need_draw = TRUE;/* 需要画screen */
    SetStopToPlayInterfaceForPlay();
#ifdef Show_Album_picture
    pic_order=0;
#endif
    while (1)
    {

        if (need_draw != 0)
        {
            if(Pedo_ui_draw == 0)
                RefreshMainScreen();
			else
			{
				Pedo_RefreshMainScreen(Pedo_Swim_flag);
			}
            need_draw = FALSE;
        }    
        /*
        if((ScrollFlag) && (Pedo_ui_draw==0))
   	    {
           if(scrollcount++>20) //matt120306#2
           {
             scrollcount = 0;
		     SetAsciiFont(FONT_TYPE_LARGE);
		     ScrollString(scroll_buf, FALSE, tag_is_unicode, &region_scroll);
		     ScrollString2(scroll_buf2, FALSE, tag_is_unicode,&region_scroll2);
             UpdateScreen(NULL);
			 SetAsciiFont(FONT_TYPE_DEFAULT);
           }
   	    } 
   	    */
        key = 1;
        /*解决快进快退高比特率和大文件时出现up键丢失的问题*/
        do
        {
            key = ap_get_message(); //读键值
            if (key == AP_MSG_USB_PLUGIN)
            {
                break;
            }
            //快进快退状态，任意键抬起都退回到正常播放，避免组合键
            if ((key & AP_KEY_UP) != 0)
            {
                if (PlayStatus == FBPlaySta)
                {
                    key = AP_KEY_PREV | AP_KEY_UP;
                }
                else if (PlayStatus == FFPlaySta)
                {
                    key = AP_KEY_NEXT | AP_KEY_UP;
                }
                else
                {
                }

            }
        } while (((PlayStatus == FFPlaySta) || (PlayStatus == FBPlaySta)) && (key != 0) && (key != (AP_KEY_PREV
                | AP_KEY_UP)) && (key != (AP_KEY_NEXT | AP_KEY_UP)));
       
        #if((MACHINE_MODEL==1)||(MACHINE_MODEL==2))
        if(earphone_in()==1)  //liuf 110323
      	{
      	    //earphone_on(FALSE);
      	    
      	    earphone_on(TRUE);
      	}	
      	else
      	{
      	    earphone_on(FALSE);
      	}
      	/*
      	if(check_5v())
        {
            g_light_flag = FALSE;
        	return RESULT_UDISK;
        }
        */
        #endif
        
        switch (key)
        {
            case AP_MSG_RTC:
			if(Pedo_ui_draw)
			{
			            if((Pedo_work_flag)||(Swim_work_flag))
    			        {
        			        Pedo_ui_num++;
        					if(Pedo_ui_num == 9)		
        					{			
        						Pedo_ui_num = 0;	
        					}
        					Pedo_RefreshDataScreen(Pedo_Swim_flag);
			            }
			}
			break;

            case AP_MSG_WAIT_TIMEOUT:
            case AP_KEY_PLAY://当按下PLAY键时先不处理,可减少停止时的杂音
            case AP_KEY_NEXT://当按下NEXT键时先不处理,可减少选下一首时的杂音
            case AP_KEY_PREV://当按下PREV键时先不处理,可减少选上一首时的杂音
            key = ap_handle_hotkey(key);
            if (key == RESULT_REDRAW)
            {
                SetFullInterface();
                need_draw = TRUE; //liuf
            }
            else if (key != 0)
            {
                return key;
            }
            else
            {            	
            }
            break;
            
            case AP_KEY_NULL://没按键
            case AP_KEY_MODE:
            break;

            default:
            //调用其他按键处理,只有对应的键按了后才进入（此函数在别的BANK）
			if(Pedo_ui_draw)
				key = Pedo_DealKeyForPlayStatus(key);
			else
                key = DealKeyForPlayStatus(key);
            if (key != NULL)
            {
                g_result = DealKeyResult(key);
                if (g_result == RESULT_UI_STOP)
                {
                    return RESULT_UI_STOP;    //NULL    //matt120405#1 for play stop
                }
                else if (g_result != NULL)
                {
                    return g_result;
                }
                else
                {
                }

            }
            break;
        }

        musicpSendCommand(MC_GETSTATUS, (void*) (&status_buf));
        DealForABGap(&status_buf);
        if (status_buf.status == PLAYING_ERROR)
        {
						//sKY_SetGsensor(0,0,0);       //pedometer
						SetFullInterface();
						Pedo_ui_draw = FALSE;
						need_draw = TRUE; //liuf
            Deal_Error_Playing();
            return NULL;
        }
        else if (status_buf.status == PLAYING_REACH_END)
        {
						//savenum = 0;						
            /*播放到文件尾时需重新刷新一下当前时间和进度条*/ 
            if (!return_to_play)
            {
           		if(!Pedo_ui_draw)
				      ShowNowTime();
            }
            g_result = DealMusicFileEnd(0);
            if (g_result != NULL)
            {
                return g_result;
            }
        }
        //注， 下面是为解决一种特殊情况而做出的特殊处理，在一般情况下可不分到头，到尾
        //     标志，统一用 PLAYING_REACH_END 标志就可以了，因为可通过 PlayStatus
        //     这个标志判断是快进还是快退，就可知道是到了文件头还是文件尾，但如果在
        //     快退到差不多到头时停止快退，这时已发停止命令，但由于模块层在接收这个
        //     命令后到真正停止快退要有一个时间差，如在这个时间差中到文件头，如还是
        //     用 PLAYING_REACH_END 标志那就分不清是到头还是到尾，所以要加一个标志
        //     PLAYING_REACH_HEAD 表示是到头。
        else if (status_buf.status == PLAYING_REACH_HEAD)
        {
            //savenum = 0;
            if (PlayStatus == PlaySta)
            {
                DisInFBHead(0);//到头显示
                SelectCurMusic();
            }
            else
            {
                g_result = DealMusicFileEnd(0); //解决进度条刷新不完全的问题
                ClearTimePara();
                if (g_result != NULL)
                {
                    return g_result;
                }
            }
        }
        else
        {
        }

        if (RePlayStatus == WaitSetBPointForAB)
        {
            if (g_comval.ReplayMode != 0)
            {
                if ((status_buf.signal == SIGNAL_REACH_V) && (g_ABMinTimeCNT == 0))//要大于AB最小时间
                {
                    //if (musicpSendCommand(MC_SETA_B, SetBPoint) != 0) //set_v
                    {
                        //g_AVSetFlag = TRUE;//标志是用AV的方式设置B点的
                    }
                }
            }
        }
        /*从菜单返回时需先刷新背景，再刷新滚屏信息*/
        if (return_to_play == 0)
        {
            g_result = (uint16) RealDisplay();

        }
        else
        {
            SetFullInterface();
            need_draw = TRUE;
            return_to_play = 0;
        }
        if (g_result == RESULT_UI_STOP)
        {
            return g_result;    //0    //matt120405#1 for play stop
        }
    }/* end of while(1) */
    return 0;

}
 
/*
 ********************************************************************************
 *             void RealDisplay( void )
 *
 * Description : 显示时间，滚屏,AB闪
 *
 * Arguments   : void
 *
 * Returns     : int
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 RealDisplay(void)
{
//    uint8 RANDOM_DATA;
//    region_t region_Scroll;
     
    BackupCurrentTime();
    if (g_music_vars.BreakPTDisTime.second != g_OldSecTempBackup)
    {
        g_OldSecTempBackup = g_music_vars.BreakPTDisTime.second;
        if (g_music_vars.repeat_mode == Intro)
        {
            g_introCNT++;
            if ((g_introCntMode && (g_music_vars.BreakPTDisTime.second > IntCntValDef)) || (g_introCNT > IntCntValDef))

            { //从头开始的情况
                if ((PlayStatus != FFPlaySta) && (PlayStatus != FBPlaySta))
                {
#ifdef OPT_Sup_FadeOut_fun
                    FadeOut(FALSE);
#endif
                }
                return DealMusicFileEnd(0);
            }
        }

        //matt120315#3
        /*背景灯开的时候才刷屏*/
        if (!g_light_flag)
        {
            return 0;
        } 

        if (RePlayStatus == NormalPlay)
        {
		    if(!Pedo_ui_draw)
                Show_Bitrate();
        }
		if(!Pedo_ui_draw)
            ShowNowTime();
         
    }

    //matt120315#3
    /*背景灯开的时候才刷屏*/
    if (!g_light_flag)
    {
        return 0;
    } 
    
    //matt120306#2
	if(!Pedo_ui_draw)
	{
        //滚屏时的处理
        if ((g_ScrollFlag) && (PlayStatus == PlaySta))
        {
            g_ScrollFlag = FALSE;
            SetAsciiFont(FONT_TYPE_LARGE);
            if(ScrollFlag)
                ScrollString(scroll_buf, FALSE, tag_is_unicode, &region_scroll);
            if(ScrollFlag2)
                ScrollString2(scroll_buf2, FALSE, tag_is_unicode, &region_scroll2);
            UpdateScreen(NULL);
			SetAsciiFont(FONT_TYPE_DEFAULT);
			 /*
            SetAsciiFont(FONT_TYPE_LARGE);
    		region_Scroll.x = 0;
            region_Scroll.y = 24;
            region_Scroll.width = 96;
            region_Scroll.height = 16;
            memcpy(scroll_buf,g_TagInfoBuf,92);//by cailizhen
            ScrollString(scroll_buf, FALSE,tag_is_unicode,&region_Scroll);
            SetAsciiFont(FONT_TYPE_SMALL);
            */
            if ((PlayStatus != FBPlaySta) && (PlayStatus != FFPlaySta))
            {

                Show_Bitrate();
            }
            ui_show_batt(GetBattery());
            //UpdateScreen(NULL);//gongee 2004-3-18 8:48

        }
	}
	
    if (g_2HZFlag && (PlayStatus == PlaySta) && (g_currentMusicType != Music_Wav))
    {
      //  ResShowPic( ((uint16)rand())%16 + IMAGE_05, 60, 48); //显示声音的频普
    }
    //复读A闪，B闪时的处理
    if (RePlayStatus == WaitSetAPointForAB)
    {
       /* if (g_2HZFlag)
        {
     //       if (g_currentMusicType != Music_Wav)
        //        ui_show_d_pic(MRPLAB, 0);
       //     else
       //         ui_show_d_pic(MRPLAB, 1);
        }
        else
        {
        //    if (g_currentMusicType != Music_Wav)
         //       ui_show_d_pic(MRPLNOA, 0);
        //    else
          //      ui_show_d_pic(MRPLNOA, 1);
        }*/
        ui_show_loop(IM_MUSIC_REPEATA);
    }
    else if (RePlayStatus == WaitSetBPointForAB)
    {
       /* if (g_2HZFlag)
        {
      //      if (g_currentMusicType != Music_Wav)
         //       ui_show_d_pic(MRPLAB, 0);
         //   else
        //        ui_show_d_pic(MRPLAB, 1);
        }
        else
        {
          //  if (g_currentMusicType != Music_Wav)
          //      ui_show_d_pic(MRPLNOB, 0);
         //   else
          //      ui_show_d_pic(MRPLNOB, 1);
        }  //
        */
        ui_show_loop(IM_MUSIC_REPEATAB);
    }
	else if (RePlayStatus == ABLoopPlay)
	{
		if(g_2HZFlag)
		{
			ui_show_loop(IM_MUSIC_REPEATAB);
		}
		else
		{
			ui_show_loop(IM_MUSIC_PLAYTIME_BG);
//			ResShowPic(IM_MUSIC_PLAYTIME_BG, 59, 16);
		}
	}
	return NULL;
}
/*
 ********************************************************************************
 *             bool musicpSendCommand( uint8 cmd, uint16 param )
 *
 * Description : 根据不同的文件类型来选择发的命令（对MODULE）
 *
 * Arguments   : cmd 命令 param 参数
 *
 * Returns     : success/fail
 *
 * Notes       :
 *
 ********************************************************************************
 */
bool musicpSendCommand(uint8 cmd, void* param)
{
    switch (cmd)
    {
        case MC_GETTIME:
        case MC_GETSAPLE:
        case MC_GETSTATUS:
        case MC_GETAUDIOFMT:
        case MC_FADEINOUT:
        return wavpSendCommand(cmd, param);
        default:
        return musicpSendCommand_bank(cmd, param);
    }
    return FALSE;
}

/*
 ********************************************************************************
 *             void Show_Bitrate( void )
 *
 * Description : 显示比特率
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
void BackupCurrentTime(void)
{
    Full_time_t time_buf;
    if (musicpSendCommand(MC_GETTIME, (void*) &time_buf) != 0)
    {
        g_music_vars.BreakPTDisTime.hour = time_buf.TimeHour;
        g_music_vars.BreakPTDisTime.minute = time_buf.TimeMinute;
        g_music_vars.BreakPTDisTime.second = time_buf.TimeSecond;
    }
}

uint16 time_ResShowPic(uint16 id, uint8 x, uint8 y)
{
    res_infor_t  res_infor;
    SD_FSeek(res_fp, SEEK_SET, pic_entry_offset+(long)id * RESHEADITEM);
    SD_FRead(res_fp, &res_infor, sizeof(res_infor_t));
    /*判断res类型的合法性*/
    if(res_infor.bType != 0x02) 
    {
        return 0; 
    }  
    /*设窗参数设置*/
    res_region.x = x;
    res_region.y = y; 
    res_region.width = (char)res_infor.wWidth;
    res_region.height = (char)res_infor.wHeight; 
    /*读取图片数据并刷屏*/
    PutSDImage(&res_region, res_fp->startaddr + res_infor.dwOffset );                    
   
    return 1;
}

/*显示当前播放时间和进度条信息*/
void ShowNowTime(void)
{
    uint32 nCnt;
    uint16 i,num;;
/*
	music_play_status_t status_buf;
	if (ifSelBuffToltalTimeFlag || (!CaltimeFlag))     
	{         //用 buffer 里的总时间数据         
		status_buf.total_time.hour = TimeRateData_sav.TotalTime_sav[0];         
		status_buf.total_time.minute = TimeRateData_sav.TotalTime_sav[1];         
		status_buf.total_time.second = TimeRateData_sav.TotalTime_sav[2];     
	}     
	else     
	{         
		musicpSendCommand(MC_GETSTATUS, (void*)(& status_buf));         
		//暂存现在的总时间数据到 buffer         
		TimeRateData_sav.TotalTime_sav[0] = status_buf.total_time.hour;         
		TimeRateData_sav.TotalTime_sav[1] = status_buf.total_time.minute;         
		TimeRateData_sav.TotalTime_sav[2] = status_buf.total_time.second;         
		ifSelBuffToltalTimeFlag = 1;     
	}
	*/
/*	itoa1(g_music_vars.BreakPTDisTime.hour,&temp_buffer[0]);
	temp_buffer[1] = ':';
	itoa2(g_music_vars.BreakPTDisTime.minute, &temp_buffer[2]);
    temp_buffer[4] = ':';
    itoa2(g_music_vars.BreakPTDisTime.second, &temp_buffer[5]);
    temp_buffer[7] = 0x00;*/
    
    itoa_1(g_music_vars.BreakPTDisTime.hour, temp_buffer);     
	itoa_2(g_music_vars.BreakPTDisTime.minute,  & temp_buffer[1]);     
	itoa_2(g_music_vars.BreakPTDisTime.second, & temp_buffer[3]);
    ui_show_f(temp_buffer);

    //itoa_1(TimeRateData_sav.TotalTime_sav[0], temp_buffer);       
	//itoa_2(TimeRateData_sav.TotalTime_sav[1],  & temp_buffer[1]);       
    //itoa_2(TimeRateData_sav.TotalTime_sav[2], & temp_buffer[3]);       
    //ui_show_ef(temp_buffer);

    if(((uint32)TimeRateData_sav.TotalTime_sav[0]*3600 + TimeRateData_sav.TotalTime_sav[1]*60 + TimeRateData_sav.TotalTime_sav[2]) == 0)
        nCnt = 0;
    else
        nCnt =((uint32)126*((uint32)g_music_vars.BreakPTDisTime.hour*3600 + g_music_vars.BreakPTDisTime.minute*60 + g_music_vars.BreakPTDisTime.second))
            / ((uint32)TimeRateData_sav.TotalTime_sav[0]*3600 + TimeRateData_sav.TotalTime_sav[1]*60 + TimeRateData_sav.TotalTime_sav[2]);
    num=126;
    time_ResShowPic(IM_MUSIC_TOTALTIME_BG,0 ,56);
    for(i = 0; i < nCnt; i++)
    {
        time_ResShowPic(IM_SBRA,1 + i ,56);
    }   
    for(i = nCnt; i < num; i++)
    {
        time_ResShowPic(IM_SBAR_CLEAR,1 + i,56);
    }
    res_region.x = 0;
    res_region.y = 56; 
    res_region.width = 128;
    res_region.height = 8; 
    UpdateScreen(&res_region);
    
/*
	itoa_1(status_buf.total_time.hour, temp_buffer);       
	itoa_2(status_buf.total_time.minute,  & temp_buffer[1]);       
    itoa_2(status_buf.total_time.second, & temp_buffer[3]);       
    ui_show_ef(temp_buffer);
*/
}

/*显示比特率信息*/
void Show_Bitrate(void)
{
    audio_format_t pFormat;    
    if (PlayStatus == PauseSta)
    {
        //首次进入时和停止时，通过checktype获取比特率，
    }
    else if (PlayStatus != StopSta)
    {   
        musicpSendCommand(MC_GETAUDIOFMT, (void*) &pFormat);   
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
    ResShowPic(NUM_PIC[TimeRateData_sav.Bitrate_sav[0] - 0x30], POSITION_B, 8);
    ResShowPic(NUM_PIC[TimeRateData_sav.Bitrate_sav[1] - 0x30], POSITION_B+5, 8);
    ResShowPic(NUM_PIC[TimeRateData_sav.Bitrate_sav[2] - 0x30], POSITION_B+10, 8);
}

//===================[处理AB复读状态]====================
void DealForABGap(music_play_status_t *status)
{
    if (status->signal == SIGNAL_REACH_B)
    {
        g_ReplayTimeCNT++;
        g_2HZ_CNT = 0;

        if (g_ReplayTimeCNT < g_music_vars.replaytimes)
        {
            musicpSendCommand(MC_SETA_B, (void*) StopBPlay);
        }
        else
        {

            musicpSendCommand(MC_SETA_B, (void*) ABFinished);
            musicpSendCommand(MC_SETA_B, ClrABPoint);
            RePlayStatus = WaitSetAPointForAB;
        }
    }

    if (status->status == PLAYING_WAIT_A)
    {
        if (g_music_vars.replaygap * 2 >= g_2HZ_CNT) //PAUSE值
        {
            return;
        }
        musicpSendCommand(MC_SETA_B, (void*) ReturnApoint);
        RePlayStatus = ABLoopPlay;
    }

}

