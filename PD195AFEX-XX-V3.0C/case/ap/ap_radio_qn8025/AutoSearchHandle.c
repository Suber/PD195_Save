/*
 *******************************************************************************
 *                ACTOS AP
 *        main ap of sdk 3.0
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *        History Record:
 1.去掉刷新背景显示，消除搜台时的界面闪烁感, by Mars,2008-04-15
 *******************************************************************************
 */

#include "ap_radio.h"

#pragma name(AP_AUTOSEARCH)

extern uint16 Freq_bak;
extern    slider_t     slider; 
extern    comval_t    F_comval;
extern uint16 FMStationTab[60];  // 前30个是US, 后30个是JP.
extern fm_play_status_t FMStatus;
extern FMRADIO_param_t radio_param;
extern uint8 Freqtab[10];
extern uint8 fmconnect[2];
extern uint8 FMFreNumOld; //上次显示频率数字的位数
extern uint8    Updateflag;
extern uint8 Prs_nor;
extern uint16 FMAutoTab[30]; // 全自动收台,预存台.
//extern bool Need_refresh;
extern void SequenceFreqlist(uint16* buf, uint16 curfreq, uint8 totalnum);
extern void DealFadeIn(uint8 Volume);
extern void ShowFreqNum(void);
extern void ShowSignalST(void);
void SaveStation(void);

/*
 ********************************************************************************
 *             void ui_show_chdata(uint8 *buf, uint8 x, uint8 y)
 *
 * Description : 显示电台号数字
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
/*void ui_show_chdata(uint8 *buf, uint8 x, uint8 y) //zxs add for show CH number
{
    while (1)
    {
        if (*buf != 0)
        {
            ResShowPic(MNUM0 + *buf - 0x30, x, y);
        }
        else
        {
            break;
        }
        ClearWatchDog();
        buf++;
        x += (uint8) (PIC_X_CHNUM1 - PIC_X_CHNUM0);
    }
}*/	   
/*
 ********************************************************************************
 *             void ui_show_FMdata(uint8 *buf, uint8 x, uint8 y)
 *
 * Description : 显示电台频率
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
/*void ui_show_FMdata(uint8 *buf, uint8 x, uint8 y)
{
    uint8 FMFreNum = (uint8) strlen(buf);
    if (FMFreNum != FMFreNumOld)
    {
        ResShowPic(PIC_ID_FMNUMBG,PIC_X_FMNUMBG,PIC_Y_FMNUMBG);
        FMFreNumOld = FMFreNum;
    }
    while (1)
    {
        if (*buf != 0)
        {
            ResShowPic(FMNUM0 + *buf - 0x30, x, y);
        }
        else
        {
            break;
        }
        ClearWatchDog();
        buf++;
        x += (uint8) (PIC_X_F2RATE - PIC_X_F1RATE);
    }
}*/
/*
 ********************************************************************************
 *             void ShowStationCount(uint16 count)
 *
 * Description : 显示电台号.
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void ShowStationCount(uint8 count)
{
		uint8 buf[4]; 
    
    if(count > 0)        //显示电台号
    { 
        //*itoa_2(count,buf);
        buf[0]=count/10;
        buf[1]=count%10;
     	ResShowPic(SYSFONT_SMALL_0+buf[0],0,32);
        ResShowPic(SYSFONT_SMALL_0+buf[1],0+6,32);
		ResShowPic(SYSFONT_SMALL_SLASH,0+12,32);
		ResShowPic(SYSFONT_SMALL_0+buf[0],0+18,32);
		ResShowPic(SYSFONT_SMALL_0+buf[1],0+24,32);
    }
}
/*
 ********************************************************************************
 *             void CleanStationCount(void)
 *
 * Description : 清除电台号
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void CleanStationCount(void)
{
//    ResShowPic(PIC_ID_FMCHBG,PIC_X_FMCHBG,PIC_Y_FMCHBG);
}

/*
 ********************************************************************************
 *             void ShowFreqInFM2()
 *
 * Description : 显示全自动搜台界面
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void ShowFreqInFM2(void)
{
//    uint8 buf[4];
//    uint8 FMTab[5];
     ClearScreen(NULL);             
    if(FMStatus.stereo_status==0)        
    {                
        ResShowPic(IM_RADIO_STEREO,40,16);        //显示立体声
    }
    else
    {
        ResShowPic(IM_RADIO_MONO,40,16);
    }
	ResShowPic(IM_RADIO_PRESET,0,16);	
	if((FMStatus.station_count>0)&&(FMStatus.station>0))        //显示电台号
    {
		ShowStationCount(FMStatus.station);

	}
	ShowFreqNum();
    ui_show_batt(GetBattery());
	   
    ShowSignalST();         
	Prs_nor = 1;
    UpdateScreen(NULL);                             //刷新 
    ClearWatchDog(); //清看门狗
}

/*
 ********************************************************************************
 *             bool Search_Show2(FM_direct_t Direct)
 *
 * Description : 搜索下一个在电台表中没有的新电台.
 *
 * Arguments   : Direct,Next或Last搜索.
 *
 * Returns     : TRUE:搜到下一个新电台,FALSE:没有搜到.
 *
 * Notes       :
 *
 ********************************************************************************
 */
bool Search_Show2(FM_direct_t Direct)
{
    bool Search_Flag;
    uint16 i;
    uint16 temp;

    FMStatus.stereo_status = 1;
    FMStatus.station = 0;
//    fmSendCommand(MC_MUTE,SetMUTE);
//    fmSendCommand(MC_SETVOLUME,FMStatus.volume);
    fmSendCommand(MC_RESTORE, (uint16) & radio_param);
    Search_Flag = fmSendCommand(MC_SEARCH, Direct);
    if (Search_Flag == 1) //搜台成功后，MUTE释放
    {
    	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
        fmSendCommand(MC_GETFREQ, (uint16) & temp);
        for (i = 0; i < AutoTabNum; i++) // 检查是否在电台表中
        {
            if (FMAutoTab[i] == temp)
            {
                FMStatus.stereo_status = 1;
                Search_Flag = FALSE;
                break;
            }
        }
//		fmSendCommand(MC_GETSTATUS,(int)&FMStatus);
        /*if ((FMStatus.mode == 1) && (FMStatus.freq > 140))
        {
            Search_Flag = FALSE;
        }*/
        ShowFreqInFM2();
    }
/*#ifdef HARDWARE_SEEK
    if(Search_Flag==1)
    {
        CleanStationCount();
        ShowFreqInFM2();
        ShowStationCount(AutoTabNum + 1);
        g_CNT=0;
        while(g_CNT==0)
        {
            ; //等待0.5秒
        }
    }
#else*/
    
    if (Search_Flag == 1)
    {
        ShowStationCount(AutoTabNum + 1);
        g_CNT = 0;
        while (g_CNT < 2)
        {
            ; //等待1秒
        }
        CleanStationCount();
    }
    else
    {
    	for(i=0;i<0x100;i++)
    	{
//         	ClearWatchDog(); //清看门狗
    	}
	}
//#endif
    return Search_Flag;
}

/*
 ********************************************************************************
 *             bool RestartSearch(uint8 level)
 *
 * Description : 设置门限值,重新开始搜索.
 *
 * Arguments   : 门限值
 *
 * Returns     : TRUE:继续搜索,FALSE:中断搜索,正常结束.
 *
 * Notes       :
 *
 ********************************************************************************
 */
bool RestartSearch(uint8 level)
{
    if (level <= 2)
    {
        return FALSE;
    }
    //fmSendCommand(MC_OPEN, level); //重新设置搜索门限值，开始重新搜索
    fmSendCommand(MC_OPEN, level); //重新设置搜索门限值，开始重新搜索

#ifndef HARDWARE_SEEK
    fmSendCommand(MC_RESTORE, (uint16) & radio_param);
#endif
    return TRUE;
}
/*
 ********************************************************************************
 *             uint16 AutoSearchHandle(uint8 Direct)
 *
 * Description : 全自动搜索
 *
 * Arguments   : Direct,Next或Last搜索.
 *
 * Returns     : 如果有可识别的热键消息,  返回结果消息
 如果没有可识别的热键消息,  返回0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 AutoSearchHandle(uint8 Direct)
{
    uint8 key = 0;
    uint8 count = 0; //RTC消息次数计数
    bool need_draw=TRUE,need_draw_all=TRUE;
    uint8 Search_Level = SEARCHLEVEL;
    bool loop = TRUE;
    uint8 Search_Loop = 1; //搜台的次数
    uint8 temp;

//    Need_refresh = 1;
    AutoTabNum = 0; // 全自动搜索,从极值开始.
    Freq_bak = FMStatus.freq;
    memset(FMAutoTab, 0, 30);        //AutoSearch 所用数组初始化
    fmSendCommand(MC_BACKUP, (uint16) & radio_param);
    fmSendCommand(MC_OPEN, SEARCHLEVEL);
    if (FMStatus.mode == 0)
    {
        if (Direct == 10) // US频段
        {
            FMStatus.freq = 875;
        }
        else
        {
            FMStatus.freq = 1080;
        }
    }
    else
    { // JP频段
        if (Direct == 10)
        {
            FMStatus.freq = 760;
        }
        else
        {
            FMStatus.freq = 900;
        }
    }
//#ifdef HARDWARE_SEEK
//    fmSendCommand(MC_SETFREQ,FMStatus.freq);
//#endif
    temp = FMStatus.station_count;
    while (loop == TRUE)
    {
        if(need_draw_all)
        {
            ResShowPic(IM_RADIO_BG,0,0);     
            need_draw_all=FALSE;
            need_draw=TRUE;            
        }
        if(need_draw)                //显示
         {
         		ShowFreqInFM2();
         		need_draw=FALSE;
         } 
        if (Direct != 0)
        {
            if (Direct == 10) //NEXT搜索处理
            {
/*#ifdef HARDWARE_SEEK			    //硬件SEEK搜索
                if(loop==TRUE)
                {
                    if(Search_Show2(UP))
                    {
                        SaveStation();
                        need_draw = TRUE;
                    }
                    else
                    {
                        FMStatus.freq=0;
                        Search_Level-= 3; // 降低搜索门限，以不同门限搜索.
                        Search_Loop--;
                        if(Search_Loop==0)
                        {
                            loop = FALSE;
                            key = 0;
                        }
                        else
                        {
                            loop = RestartSearch(Search_Level);
                            if(loop)
                            {
                                fmSendCommand(MC_SETFREQ,FMStatus.freq);
                            }
                        }
                    }
                }

#else*/
                if (((FMStatus.freq + 1) > 1080) && (FMStatus.mode == 0)) //软件搜索
                {
                    FMStatus.freq = 875;
                    Search_Level -= 3; // 降低搜索门限，以不同门限搜索.
                    Search_Loop--;
                    if (Search_Loop == 0)
                    {
                        loop = FALSE;
                        key = 0;
                    }
                    else
                    {
                        loop = RestartSearch(Search_Level);
                    }
                }
                else if(((FMStatus.freq+1)>900)&&(FMStatus.mode==1))
                    {
                        FMStatus.freq=760;
                    Search_Level -= 3; // 降低搜索门限，以不同门限搜索.
                    Search_Loop--;
                    if (Search_Loop == 0)
                    {
                        loop = FALSE;
                        key = 0;
                    }
                    else
                    {
                        loop = RestartSearch(Search_Level);
                    }
                }
                else
                {
                    FMStatus.freq += 1;
                    need_draw = TRUE;
                }
                if ((loop) && (Search_Show2(UP)))
                {
					SaveStation();
					DealFadeIn(FMStatus.volume);
                    need_draw = TRUE;
                }
				/*else //搜索无电台
				{
					if (Search_Loop == 1)
					{
						loop = FALSE;
                        key = 0;
					}
					else if (Search_Loop == 2) //似乎无作用，因暂无此需求，故先不管这个动作
					{
						if (FMStatus.mode == 0)
						{
							FMStatus.freq = 875;
						}
						else
						{
							FMStatus.freq = 760;
						}
					}
				}*/
//#endif
            }
            else if (Direct == 20) //LAST搜索处理
            {
/*#ifdef HARDWARE_SEEK						  //硬件SEEK搜索
                if(loop==TRUE)
                {
                    if(Search_Show2(DOWN))
                    {
                        SaveStation();
                        need_draw = TRUE;
                    }
                    else
                    {
                        if(FMStatus.mode == 0)
                        {
                            FMStatus.freq=875;
                        }
                        else
                        {
                            FMStatus.freq=760;
                        }
                        Search_Level-=3; // 降低搜索门限，以不同门限搜索.
                        Search_Loop--;
                        if(Search_Loop==0)
                        {
                            loop = FALSE;
                            key = 0;
                        }
                        else
                        {
                            loop = RestartSearch(Search_Level);
                            if(loop)
                            {
                                fmSendCommand(MC_SETFREQ,FMStatus.freq);
                            }
                        }
                    }
                }
#else*/
                if (((FMStatus.freq-1)<875) && (FMStatus.mode == 0)) //软件搜索
                {
                    FMStatus.freq = 1080;
                    Search_Level -= 3; // 降低搜索门限，以不同门限搜索.
                    Search_Loop--;
                    if (Search_Loop == 0)
                    {
                        loop = FALSE;
                        key = 0;
                    }
                    else
                    {
                        loop = RestartSearch(Search_Level);
                    }
                }
                else if (((FMStatus.freq-1)<760) && (FMStatus.mode == 1))
                {
                    FMStatus.freq = 900;
                    Search_Level -= 3; // 降低搜索门限，以不同门限搜索.
                    Search_Loop--;
                    if (Search_Loop == 0)
                    {
                        loop = FALSE;
                        key = 0;
                    }
                    else
                    {
                        loop = RestartSearch(Search_Level);
                    }
                }
                else
                {
                    FMStatus.freq -= 1;
                    need_draw = TRUE;
                }
                if ((loop) && (Search_Show2(DOWN)))
                {									
                    SaveStation();
					DealFadeIn(FMStatus.volume);
                    need_draw = TRUE;
                }
//#endif
            }
            else
            {
                if (Direct == 50)
                {
                    Direct = 0;
                    loop = 0;
                    key = 0;
                }
            }
        }

         if(loop && need_draw)                //显示
         {
         		ShowFreqInFM2();
         		need_draw=FALSE;
         }  

        if (AutoTabNum >= 30) // 已搜到30个电台,正常退出.
        {
            loop = FALSE;
            key = 0;
        }
        if (loop == TRUE)
        {
/*#ifdef HARDWARE_SEEK
            do
            {
                key = ap_get_message();
            }while (key == AP_MSG_RTC);
#else*/
            key = ap_get_message();
//#endif
            switch (key)
            {
                case AP_MSG_STANDBY:
                //do nothing
                break;
                
                case AP_MSG_RTC: //2HZ中断到来，屏幕重画，为了电池显示更新
                if(g_rtc_counter <= g_light_time - 1) 
                {
                	g_rtc_counter=0;
                }
                /*
				if(FMStatus.stereo_status==0)        
    			{                
        			ResShowPic(IM_RADIO_STEREO,40,8);        //显示立体声
    			}
    			else
    			{
        			ResShowPic(IM_RADIO_MONO,40,8);
    			}	
                count++;
                if (count == 2)*/
                {
                    ui_show_batt(GetBattery()); //电池电量显示
                    count = 0;
                }
                break;
                
                case AP_KEY_PLAY | AP_KEY_UP:
                case AP_KEY_MODE | AP_KEY_UP: // 保存电台, 返回播放界面.
                loop = FALSE;
                key = RESULT_IGNORE;
                break;
                
                default: //热键处理
                key = ap_handle_hotkey(key);
                if (key == RESULT_REDRAW)
                {
                      need_draw_all=TRUE;
                    	need_draw = TRUE;
//                    	Need_refresh=1;	//matt for FM hold display
                }
                else
                {
                    if (key != 0)
                    {
                        loop = FALSE;
                    }
                }
				break;
            } // end switch(key)
        } // end if(loop)
		ClearWatchDog(); //清看门狗
    } // end while(loop)
    if (AutoTabNum > 0)
    {

        memcpy(radio_param.param1, FMAutoTab, AutoTabNum * 2);
        if (temp < AutoTabNum) // 覆盖所有电台
        {
            FMStatus.station_count = AutoTabNum;
        }
        else
        {
            if (key == 0)
            {
                FMStatus.station_count = AutoTabNum;
            }
            if (key == RESULT_IGNORE) // 覆盖部分电台
            {
                if (temp != 0)
                {
                    FMStatus.station_count = temp;
                }
            }
        }
        FMStatus.freq = FMAutoTab[0];
        FMStatus.station = 0;
        fmSendCommand(MC_RESTORE, (uint16) & radio_param);
    }

    if (key == RESULT_IGNORE)
    {
        key = 0;
    }

    if (FMStatus.station_count > 0x00) // 没有搜到电台退出的情况.
    {
        fmSendCommand(MC_STEP, UP);
        for (temp = 0; temp < 0xff; temp++) // 延迟
        {
        }
        fmSendCommand(MC_BACKUP, (uint16) & radio_param);
    }
/*	else
	{
		FMStatus.freq = Freq_bak;
        fmSendCommand(MC_SETFREQ, FMStatus.freq); //若两次都搜不到台,则将频点设置到之前开始搜索的频点
        fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
        //ShowFreqInFM2();
        Updateflag = 1;
	}*/
    return key;
}
/*
 ********************************************************************************
 *             void SaveStation(void)
 *
 * Description : 保存搜索到的电台
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void SaveStation(void)
{
    uint8 i;
    uint16 k;
    fmSendCommand(MC_GETFREQ, (uint16) & k); // 保存电台
    //FMAutoTab[AutoTabNum] = k;
    AutoTabNum++;
    SequenceFreqlist(FMAutoTab, k, AutoTabNum);	
    for (i = AutoTabNum; i < 30; i++)
    {
        if (radio_param.param1[i] == k) // 删除重复的电台.
        {
            radio_param.param1[i] = 0;
        }
    }

}

