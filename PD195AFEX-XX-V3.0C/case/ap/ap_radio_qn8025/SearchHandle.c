/*
 *******************************************************************************
 *                ACTOS AP
 *        main ap of sdk 3.0
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *       History Record:
 *         1.解决自动搜台时数字闪烁的问题，by Mars，2008-04-15
 *******************************************************************************
 */

#include "ap_radio.h"

#pragma name(AP_SEARCH)

uint8 SearchCount = 0; //记录手动搜台的次数
uint8 StationFlag = 1; //是否是在正常频段范围内的电台,1为真,0为假
uint16 Freq_bak; //开始搜索频点的备份值fm_play_status_t FMStatus; //当前电台信息,8 bytes
extern fm_play_status_t FMStatusUS; //US电台信息暂存,8 bytes
extern fm_play_status_t FMStatusJP; //Japan电台信息暂存,8bytes
extern fm_play_status_t FMStatus;
extern FMRADIO_param_t radio_param;
extern uint8 Freqtab[10];
extern uint8 Updateflag;
extern uint8 FMFreNumOld; //上次显示频率数字的位数
extern fm_play_status_t m_FMPlaystatus;
extern BOOL fm_VolOpFlag; 
extern uint8 Prs_nor;
extern uint8 fmconnect[2];
extern slider_t slider;
extern uint8 QND_GetRSSI(); 
extern uint16 FMStationTab[60];
extern uint8 fmdataflag[3];
extern uint8 nopresetflag;
void ShowFreqInFM(void); //FM主界面显示
bool Search_Show(FM_direct_t Direct); //自动搜台时的显示
const uint8 nopreset[]="No Preset";
/*
 ********************************************************************************
 *             void ui_show_volfm(uint8 volume)
 *
 * Description : 显示音量信息
 *
 * Arguments   :    当前音量值
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void read_var(void)
{
	uint16 k;
    uint8 buf[4] = { "FM" };

	if (memcmp(fmdataflag, buf, 2) != 0)
    {
        for (k = 0; k < 30; k++)
        {
            FMStationTab[k] = 0;
        }
        memcpy(fmdataflag, buf, 3); //FM BUFFER标志赋初值
        //US和JP暂存区赋初值
        FMStatusUS.freq=875;
        FMStatusUS.mode = 0;
        
        FMStatusUS.vol_display = g_comval.VolumeDef;
        FMStatusJP.vol_display = g_comval.VolumeDef;
        if (g_comval.VolumeMax != 0)
        {
            FMStatusUS.volume =  (FMStatusUS.vol_display * MAXVOLUME) / g_comval.VolumeMax;
            FMStatusJP.volume =  (FMStatusJP.vol_display * MAXVOLUME) / g_comval.VolumeMax;
        }
        else
        {
            FMStatusUS.volume = FMStatusUS.vol_display;
            FMStatusJP.volume = FMStatusJP.vol_display;
        }
        FMStatusUS.station = 0;
        FMStatusUS.station_count = 0;
        FMStatusUS.stereo_status = 1;
        FMStatusJP.freq=760;
        FMStatusJP.mode = 1;
        FMStatusJP.station = 0;
        FMStatusJP.station_count = 0;
        FMStatusJP.stereo_status = 1;
        memcpy(&FMStatus, &FMStatusUS, sizeof(FMStatus));
        // sfr_bak = SFR_BANK;
        // SFR_BANK = BANK_AUIP;
        //DAC_ANALOG |= 0xc0; //使能DAC和PA，设置初始音量为24
        // DAC_ANALOG |= 0x18;
        // SFR_BANK = sfr_bak;
    }
}

/*void ui_show_volfm(uint8 volume) //显示音量信息      zxs add for show fm vol
{
//     uint8 temp[3];
   	 uint16 i,nCnt;
//   itoa_2(volume, temp);   //matt for volume
//   ResShowPic(PIC_ID_VOL1+temp[0]-0x30,PIC_X_VOL1,PIC_Y_VOL1);
//   ResShowPic(PIC_ID_VOL1+temp[1]-0x30,PIC_X_VOL2,PIC_Y_VOL2);

	 nCnt =((uint16)volume*34)/g_comval.VolumeMax;
   
   for(i = 0; i < nCnt; i++)
   {
       ResShowPic(MUVOLFG,PIC_X_F_VOLFG + i,PIC_Y_F_VOLFG);
   }
	 for(i = nCnt; i < 34; i++)
   {
       ResShowPic(MUVOLBG,PIC_X_F_VOLFG + i,PIC_Y_F_VOLFG);
   }
}*/
/*
 ********************************************************************************
 *             uint16 SearchHandle(uint8 Direct)
 *
 * Description : 手动搜台
 *
 * Arguments   :    Direct,Next或Last搜索.
 *
 * Returns     : 	如果有可识别的热键消息,  返回结果消息
 如果没有可识别的热键消息,  返回0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 SearchHandle(uint8 Direct)
{
    uint8 key;
    uint8 count = 0; //RTC消息计熟
    bool need_draw=TRUE,need_draw_all=FALSE;
    uint8 Searchflag = 1;
    StationFlag = 1;
    //SearchCount = 0; //硬件搜台次数
    Freq_bak = FMStatus.freq;
    fmSendCommand(MC_OPEN, SEARCHLEVEL);
    while (1)
    {
        if (Direct != 0)
        {
            if (Direct == 10) //NEXT搜索处理
            {
                if ((FMStatus.freq + 1 > 1080) && (FMStatus.mode == 0))
                {
                    FMStatus.freq = 875;
                }
                else if ((FMStatus.freq + 1  > 900) && (FMStatus.mode == 1))
                {
                    FMStatus.freq = 760;
                }
                else
                {
                    FMStatus.freq += 1;
                }
				if(FMStatus.freq == Freq_bak)
                {
                    fmSendCommand(MC_SETFREQ, FMStatus.freq); 
                    if (FMStatus.volume != 0)
                    {
					    fmSendCommand(MC_MUTE,releaseMUTE);
                    }
                    fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
					ShowFreqInFM();
                    return 0;    // searched total band & no station searched
                }
                if (Search_Show(UP) == 1)
                {
                    return 0;
                }
                else
                    ShowFreqInFM();
            }
            else if (Direct == 20) //LAST搜索处理
            {
				if ((FMStatus.freq - 1  < 875) && (FMStatus.mode == 0))
                {
                    FMStatus.freq = 1080;
                }
                else if ((FMStatus.freq - 1  < 760) && (FMStatus.mode == 1))
                {
                    FMStatus.freq = 900;
                }
                else
                    {
                   FMStatus.freq -= 1; 
                }
                if(FMStatus.freq == Freq_bak)
                {
                    fmSendCommand(MC_SETFREQ, FMStatus.freq); //若两次都搜不到台,则将频点设置到之前开始搜索的频点
                    if (FMStatus.volume != 0)
                    {
                        fmSendCommand(MC_MUTE, releaseMUTE);
                    }
                    fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
					ShowFreqInFM();
                    return 0;    // searched total band & no station searched
                }
                if (Search_Show(DOWN) == 1)
                {
                    return 0;
                }
                else
                    ShowFreqInFM();
            }
            else
            {
                if (Direct == 50)
                {
                    Direct = 0;
                    return 0;
                }
            }
        }  
        
        /*if(need_draw)                //显示
        {          
            ShowFreqInFM();                        
            need_draw=FALSE;           
        }*/
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
            case AP_KEY_AB | AP_KEY_UP: //搜索时不响应REC
            //do nothing
            break;
            case AP_MSG_RTC: //2HZ中断到来，屏幕重画，为了电池显示更新
            count++;
            //if (count == 2)
            {
                ui_show_batt(GetBattery()); //电池电量显示
                count = 0;
            }
            break;
/*            case AP_KEY_NEXT | AP_KEY_UP: //NEXT搜索时按NEXT键，停止搜索
            if (Searchflag == 1)
            {
                Searchflag = 0;
//            		ResShowPic(MNUM0+WFreq/1000,0+8,50);
//								ResShowPic(MNUM0+WFreq/100%10,0+18,50);
//								ResShowPic(MNUM0+WFreq/10%10,0+28,50);
//								ResShowPic(MNUM0+WFreq%10,0+38,50);
            }
            else
            {
                if (Direct == 10)
                {
                    goto Direct50;
                   	if(FMStatus.volume)
                    fmSendCommand(MC_MUTE,releaseMUTE);                    
                }
            }

            break;
            case AP_KEY_PREV | AP_KEY_UP: //LAST搜索时按LAST键，停止搜索
            if (Searchflag == 1)
            {
                Searchflag = 0;
            }
            else
            {
                if ((Direct==20)||(Direct==10))
                {
                    Direct50: Direct = 50;
                    if (FMStatus.volume != 0)
                    {
                        fmSendCommand(MC_MUTE, releaseMUTE);
                    }
                }
            }
            break;
*/
            default: //热键处理
            key = ap_handle_hotkey(key);
            if (key == RESULT_REDRAW)
            {
                  need_draw_all=TRUE;
                  need_draw = TRUE;
//                  Need_refresh=0;	//matt for FM hold display
            }
            else
            {
                if (key != 0)
                {
                    return key;
                }
            }
        }
    }
    return key;
}

/*
 ********************************************************************************
 *             bool Search_Show(FM_direct_t Direct)
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
bool Search_Show(FM_direct_t Direct)
{
    bool Search_Flag;
    uint16 i;
    FMStatus.stereo_status = 1;
    FMStatus.station = 0;
    fmSendCommand(MC_MUTE, SetMUTE);
    fmSendCommand(MC_SETVOLUME, FMStatus.volume);
    fmSendCommand(MC_RESTORE, (uint16) & radio_param);
    Search_Flag = fmSendCommand(MC_SEARCH, Direct);
    if (Search_Flag == 1) //搜台成功后，MUTE释放
    {
    	
/*        if ((FMStatus.mode == 1) && (FMStatus.freq > 140)) //若搜到的频点大于90MHZ，则搜索下一个电台
        {
            Search_Flag = FALSE;
            StationFlag = 0;
            Search_Flag = fmSendCommand(MC_SEARCH, Direct);
        }
        if (Search_Flag == 1)
        {
            StationFlag = 1;
            if (FMStatus.volume != 0)
            {*/
                fmSendCommand(MC_MUTE, releaseMUTE);
//            }
            fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
            ShowFreqInFM();
        }
					
	
//    Updateflag = 1;

    
    for (i = 0; i < 3999; i++)
    {

    }
//#endif
    return Search_Flag;
}
void ShowFreqNum(void)
{
	uint8 buf[4];
	uint32 i;
    uint32 k;
    uint8 h;
	//频率换算及转换成ASCII码
    i=FMStatus.freq;
    i=i*100;
    if((i%100)>49)
    {
        i=i/100+1;
    }
    else
        i=i/100;
    k=i/10;
    h=i%10;
    ResShowPic(IM_RADIO_CLEAR_FRENUM,0,40);
    //显示电台频率
    if(k<100)
    {
        itoa_3(k,buf);
        itoa_1(h,&buf[3]);
//        ResShowPic(PIC_ID_FMNUMBG,PIC_X_FMNUMBG,PIC_Y_FMNUMBG);
    }
    else
    {
        itoa_3(k,buf);
        itoa_1(h,&buf[3]);
        ResShowPic(IM_RADIO_NUMBER_0+buf[0]-0x30,26,40);
    }
    ResShowPic(IM_RADIO_NUMBER_0+buf[1]-0x30,26+9,40);
    ResShowPic(IM_RADIO_NUMBER_0+buf[2]-0x30,26+18,40);
    ResShowPic(IM_RADIO_NUMBER_FULLSTOP,26+27,40);
    ResShowPic(IM_RADIO_NUMBER_0+buf[3]-0x30,26+36,40);
	ResShowPic(IM_FM_MHZ,26+48,40);
}
void ShowSignalST(void)
{
	uint8 strength=0;
	strength=QND_GetRSSI();
	if(strength >= 40)
	{
		strength -= 40;
	}
    else
        strength = 0;
	if(strength <= 5)
	{
		ResShowPic(ICON_MY_RSSI6,64,16);
	}
	else if(strength <= 10)
	{
		ResShowPic(ICON_MY_RSSI5,64,16);
	}
	else if(strength <= 15)
	{
		ResShowPic(ICON_MY_RSSI4,64,16);
	}
	else if(strength <= 20)
	{
		ResShowPic(ICON_MY_RSSI3,64,16);
	}
	else if(strength <= 25)
	{
		ResShowPic(ICON_MY_RSSI2,64,16);
	}
	else if(strength <= 30)
	{
		ResShowPic(ICON_MY_RSSI1,64,16);
	}
	else
	{
		ResShowPic(ICON_MY_RSSI0,60,16);
	}
}
/*
 ********************************************************************************
 *             void ShowFreqInFM()
 *
 * Description : 显示电台收听界面
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void ShowFreqInFM(void)
{
    uint8 buf[2];

    ui_auto_update = FALSE;
    
    ClearScreen(NULL);

    ui_show_lock_1();
    
	if(Prs_nor)
	{
		ResShowPic(IM_RADIO_PRESET,0,16);		
		if(FMStatus.station_count > 0)        //显示电台号
    	{
			if(FMStatus.station != 0)
			{
				ShowStationCount(FMStatus.station);
				ResShowPic(SYSFONT_SMALL_SLASH,0+12,32);
				itoa2(FMStatus.station_count,buf);
				ResShowPic(SYSFONT_SMALL_0+buf[0]-0x30,0+18,32);
				ResShowPic(SYSFONT_SMALL_0+buf[1]-0x30,0+24,32);
				ShowFreqNum();
			}
			ShowFreqNum();
		}
    	else
    	{
			SetAsciiFont(FONT_TYPE_LARGE);
			ResShowMultiString(SFNONUM, 255, 40);
			Prs_nor = 0;
			nopresetflag = 1;
    	}
	}
	else
	{    
		ResShowPic(IM_RADIO_MESSAGE_BK, 0, 40);
		ResShowPic(IM_RADIO_NORMAL,0,16);
		ShowFreqNum();
	}
	if(FMStatus.stereo_status==0)        
    {                
        ResShowPic(IM_RADIO_STEREO,40,16);        //显示立体声
    }
    else
    {
        ResShowPic(IM_RADIO_MONO,40,16);
    }
    ui_show_batt(GetBattery());
	      
    ShowSignalST(); 
	UpdateScreen(NULL); 

    ui_auto_update = TRUE;
    ClearWatchDog(); //清看门狗
}
/*
 ********************************************************************************
 *             void    volume_callback(uint16 Vol)
 *
 * Description : 音量设置的CALLBACK函数
 *
 * Arguments   :    当前需设置的音量值
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void volume_callback(uint16 Vol)
{
    //实时改变音量的值
    FMStatus.vol_display = (uint8) Vol;
    if (g_comval.VolumeMax != 0)
    {
        FMStatus.volume = FMStatus.vol_display * MAXVOLUME / g_comval.VolumeMax;
    }
    fmSendCommand(MC_SETVOLUME, FMStatus.volume);
    if (FMStatus.volume == 0)
    {
        fmSendCommand(MC_MUTE, SetMUTE);
    }
    else
    {
        fmSendCommand(MC_MUTE, releaseMUTE);
    }
    m_FMPlaystatus.vol_display = FMStatus.vol_display;
}
/*
 ********************************************************************************
 *             uint16 ChangeVolume(void)
 *
 * Description : 音量调节显示函数
 *
 * Arguments   :
 *
 * Returns     : 	如果有可识别的热键消息,  返回结果消息
 如果没有可识别的热键消息,  返回0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 ChangeVolume(void)
{
	uint8 g_result;
    //init slider
   	ClearScreen(NULL);
    UpdateScreen(NULL);
    //init slider
    //ResShowPic(T_FM, PIC_X_FMTITLE, PIC_Y_FMTITLE);
    slider.max = g_comval.VolumeMax;
    slider.min = 0;
    slider.step = 1;
    slider.title = NULL; //VOLCHANGE;
    slider.value = (int16) FMStatus.vol_display;
    g_result = ui_slider(&slider, volume_callback);
    FMStatus.vol_display = slider.value;
	FMStatus.volume = FMStatus.vol_display;//*28/g_comval.VolumeMax;	//matt091130#2	for volume 
	fmSendCommand(MC_SETVOLUME, FMStatus.volume);
	//slider.datas = NULL;
    //slider.total = 0;
	//slider.wUnitID = NULL;
    //return ui_slider(&slider, volume_callback);
    return NULL;
}
/*
 ********************************************************************************
 *             void DealFadeIn(uint8 Volume)
 *
 * Description : 音量淡入处理函数
 *
 * Arguments   :    当前音量
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void DealFadeIn(uint8 Volume)
{
    uint8 i, j;
    uint16 nop = 0;
    for (i = 0; i <= Volume; i++)
    {
        for (j = 0; j < 100; j++)
        {
            nop++;
        }
        SetPAVolume(i);
    }
    ClearWatchDog();
}
/*
 ********************************************************************************
 *             void delay1(void)
 *
 * Description : 延时函数
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void delay1(void)
{
    uint16 i;
    for (i = 0; i < 1000; i++)
    {
        ; //淡出时短暂延时
    }

}
/*
 ********************************************************************************
 *             void DealFadeInOut(void)
 *
 * Description : 退出FM时，淡出处理函数
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void DealFadeInOut(void)
{
    uint8 vol_cnt;
    vol_cnt = FMStatus.volume;
    while (vol_cnt > 0)
    {
        fmSendCommand(MC_SETVOLUME, vol_cnt);
        fmSendCommand(MC_MUTE, releaseMUTE);
        vol_cnt--;
        delay1();
    }

}
