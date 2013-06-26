/*
 *******************************************************************************
 *                ACTOS AP
 *        ap common lib message file
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *  $Id: ap_message2.c,v 1.1.1.1 2006/05/22 03:23:59 selina Exp $
 *******************************************************************************
 */

#include "ap_common.h"

#pragma name(COM_MSG2)
extern bool blnInPlayLong; //在play long处理中
extern bool g_lockflag;
extern bool g_lockflag2;
extern uint8 g_lock_time;  //软件锁
uint8 ap_handle_hotkey_core(uint8 key);
extern void ui_show_CardOut(void);

void keylight_check(void)
{
/*
	uint8 sfr_bak;
    if(!g_keylight_flag)
    {  
        sfr_bak = SFR_BANK;
        SFR_BANK = BANK_GPIO;
		GPIOCOUTEN |= 1;
        GPIOCDAT |= 1;
        SFR_BANK = sfr_bak;
        g_keylight_flag = TRUE;
    }
*/
}   
     

void Lightmodeset(bool OnOff)
{
    //if (g_light_mode == 1)
    {
        /*if (OnOff == TRUE)
        {
            SetPLL(PLL_24MHZ);
            SetVDD(VDDSET_1_6V);
		}*/
        StandbyScreen(OnOff);
        if (OnOff == TRUE)
        {
           // SetPLL(PLL_24MHZ);
            //SetVDD(VDDSET_1_6V);
            SetContrast(g_contrast_num); //zhangxs added for Contrast adjust
        }
        /*else
        {
            SetPLL(PLL_6MHZ);
            SetVDD(VDDSET_1_4V);
        }*/
    }
    /*
    else
    {
        if (!OnOff)
        {
            SetContrast(0);
        }
        else
        {
            SetContrast(g_contrast_num);
        }
    }
    */
}


//打开按键音
void Open_KeyTone(uint8 keytone)
{
    if (keytone != 0)
    {
        sKY_Beep(keytone);
    }
}

/*
 ********************************************************************************
 * Description : 处理系统消息, 返回按键消息, 同时负责开/关背光
 *
 * Arguments   : key, 系统消息
 *
 * Returns     : 返回ap能够识别的消息, 如果没有系统消息, 返回 AP_KEY_NULL
 *
 * Notes       :

 * 按键时序:
 *  0.....1.2....1.5........... (s)
 *  key    long   hold    up    (>1.2s)
 *  key  up            (<1.2s)
 *
 * key rate: 0.3s
 *
 ********************************************************************************
 */
uint8 ap_get_message_core(uint8 key)
{
    #ifdef EAR_PROTECT
    bool counter_flag = FALSE;
    #endif
    uint8 sfr_bak;

	sfr_bak = SFR_BANK;
    SFR_BANK = BANK_PMU;

    if (blnInPlayLong)
    {
        g_rtc_counter = 0;
        if ((PRESS_DISPLAY & 0x40) == 0)
        {
            key = AP_KEY_PLAY | AP_KEY_UP; //发送长按play up
            blnInPlayLong = FALSE;
            if (!g_light_flag)
            {
                Lightmodeset(TRUE);//开背光
                g_light_flag = TRUE;
                g_rtc_counter = 0;
			    SFR_BANK = sfr_bak;
                return NULL;
            }
        }

        SFR_BANK = sfr_bak;

//#ifdef lock_key
//        key = AP_MSG_LOCK;
//#endif
        return key;
    }

/*PC 模拟器不需要单独处理play键的消息*/    
#ifndef PC
    if (key == Msg_KeyPlayPause) //PLAY键特殊处理
    {
        g_rtc_counter = 0;
        if ((PRESS_DISPLAY & 0x80) != 0) //短按play
        {
            PRESS_DISPLAY = (uint8) (PRESS_DISPLAY | 0x80); //清press_display bit7
            
            #if(MACHINE_MODEL==1)
          	keylight_check();
        	#endif
        	
#ifdef lock_key
		    if(g_lockflag)
        	{
				key = AP_MSG_LOCK;
        		Lightmodeset(TRUE);//开背光
        		g_light_flag = TRUE;
                g_rtc_counter = 0;
        		key_count = 1;
        		key_value = key;
        	}
			else
#endif
		    {    		
	            if (!g_light_flag)
	            {
	                Lightmodeset(TRUE);//开背光
	                g_light_flag = TRUE;
                    g_rtc_counter = 0;
				    SFR_BANK = sfr_bak;
					return NULL;
	            }
	            key = (uint8) (key | AP_KEY_UP); //发送短按play
			    Open_KeyTone(g_comval.KeyTone);
	        }
        }
        else if ((PRESS_DISPLAY & 0x40) != 0) //长按play
        {
            PRESS_DISPLAY = (uint8) (PRESS_DISPLAY | 0x40); //清press_display bit6
            
            #if(MACHINE_MODEL==1)
        	keylight_check();
            #endif

#ifdef lock_key
		    if(g_lockflag)
        	{
				key = AP_MSG_LOCK;
        		Lightmodeset(TRUE);//开背光
        		g_light_flag = TRUE;
                g_rtc_counter = 0;
        		key_count = 1;
        		key_value = key;
        		blnInPlayLong = FALSE;
        	}
			else
#endif
            {
                if (!g_light_flag)
                {
                    key_count = 0;
    				Lightmodeset(TRUE);//开背光
                    g_light_flag = TRUE;
                    g_rtc_counter = 0;
                    SFR_BANK = sfr_bak;
                    return NULL;
                }
                blnInPlayLong = TRUE;
                key = (uint8) (key | AP_KEY_LONG); //发送长按play up
                Open_KeyTone(g_comval.KeyTone);
            }
        }
        else
        {

        }
        SFR_BANK = sfr_bak;
        return key;

    } 
#endif

    /*按键消息和上次的一样时的处理*/
    
    if(g_lockflag2)
    {
        if((key&0x1f)==AP_KEY_MODE)
        {
            key_count = 0;
            return NULL;
        }
        if(key==Msg_KeyShortUp)
        {
            key_count = 0;
            g_lockflag2 = FALSE;
            return NULL;
        }
    }
    
    if((key==Msg_KeyShortUp) || ((key&0x1f)==AP_KEY_VOLADD) || 
                                 ((key&0x1f)==AP_KEY_VOLSUB) ||
                                 ((key&0x1f)==AP_KEY_NEXT) || 
                                 ((key&0x1f)==AP_KEY_PREV) )
    {
        if(g_lockflag)
            key = Msg_KeyHold;
    }    
    
    if (key == key_value)
    {
        /*背光没开时，等抬起键开背光，忽略按键*/
        g_rtc_counter = 0;
        key_count++;

        #if(MACHINE_MODEL==1)
        keylight_check();
        #endif
        
        if (!g_light_flag)
        {
            Lightmodeset(TRUE);//开背光
            g_light_flag = TRUE;
            key_count = 0;
            g_rtc_counter = 0;
            return NULL;
        }

/*#ifdef lock_key
		if(g_lockflag)
        {
					if(key != Msg_KeyHold)
					{
        		key = AP_MSG_LOCK;
        	}
			else
			{
				key = key_value = Msg_KeyNull;
				key_count = 0;
			}
        }
#endif*/

        /*按住按键时，320ms发送一次相同按键消息*/
        if (key_count == 4)
        {
            key = (uint8) (key | AP_KEY_LONG); //=1.2s,发长按键消息
        }
        else if (key_count > 4)
        {
            key = (uint8) (key | AP_KEY_HOLD); //》1.2s,发Key hold消息
        }
        else
        {

        }
        /*
        if(key == (AP_KEY_MODE | AP_KEY_HOLD))
        {
            g_lockflag2 = TRUE;
            if(!g_lockflag)
                key = Msg_KeyHold;
            else
                key = Msg_KeyUnHold;
        }
        else*/
        {
            return key;
        }
        
    }

//#ifdef ALARM_CLOCK
//    /*处理Alarm 中断写alarmtimerflag的消息*/
//    if (alarmtimerflag == HAVE_MESSSAGE_FLAG)
//    {
//        alarmtimerflag = NO_MESSSAGE_FLAG;
////        #ifdef KEYLIGHT
////        if(!g_keylight_flag)
////        {
////            SFR_BANK = BANK_GPIO;
////						GPIOFOUTEN |=(1<<3);
////            GPIOFDAT |=(1<<3);
////            g_keylight_flag = TRUE;
////            SFR_BANK = sfr_bak;
////        }
////        #endif
//        if (!g_light_flag)
//        {
//            Lightmodeset(TRUE);//开关背光（TRUE: 开, FALSE:关）
//            g_light_flag = TRUE;
//            return NULL;
//        }
//        key = Msg_RtcAlarm;
//    }
//#endif

    /*处理USB插入消息*/
    #if(MACHINE_MODEL==0)
    if (usbplugflag == MSG_USB_STICK_FLAG)
    #elif((MACHINE_MODEL==1)||(MACHINE_MODEL==2))
    if (/*(usbplugflag == MSG_USB_STICK_FLAG) &&*/ check_5v())
    #endif
    {
        key = AP_MSG_USB_PLUGIN;
        usbplugflag = AP_MSG_USB_PLUGIN_FLAG;
        //    g_rtc_counter = 0;
        //    key_count = 1;
        //开始充电
        //sKY_ChargeSet(1);
        //sKY_ChargeGetandSet();
        if (g_light_flag == 0)
        {
            Lightmodeset(TRUE);//开关背光（TRUE: 开, FALSE:关）
            g_light_flag = TRUE;
            g_rtc_counter = 0;
        }
    
        #if(MACHINE_MODEL==1)
        keylight_check();
        #endif
        
        return key;
    }

    
    switch (key)
    {
        case Msg_KeyShortUp:
        key_count = 0;
        g_rtc_counter = 0;
        /*key_value的置需改变*/
        key = key_value | AP_KEY_UP;
        key_value = key;

        #if(MACHINE_MODEL==1)
        keylight_check();
        #endif
        
        if (!g_light_flag)
        {
            Lightmodeset(TRUE);//开背光
            g_light_flag = TRUE;
            g_rtc_counter = 0;
            /*输出按键音*/
            Open_KeyTone(g_comval.KeyTone);
            return NULL;
        }
        break;

        case MSG_RTC2HZ:
            #ifndef STANDBY_AP   //pedometer
            #if(PETOMETER_USE)
                if(g_comval.PedoFlag)
                {
                    if(Pedo_work_flag)
    			    {
        			    Pedo_time_cnt++;
        				if(Pedo_time_cnt>1)
        			    {
        				    Pedo_total_sec++;
        				    Pedo_time_cnt = 0;
        				}
        				sKY_SetGsensor(7,0,Pedo_total_sec,Pedo_Swim_flag);
        			}
        			else if(Swim_work_flag)
    			    {
        			    Pedo_time_cnt++;
        				if(Pedo_time_cnt>1)
        			    {
        				    Swim_total_sec++;
        				    Pedo_time_cnt = 0;
        				}
        				sKY_SetGsensor(7,0,Swim_total_sec,Pedo_Swim_flag);
        			}
 				}
        	#endif
        	#endif
        			
        g_rtc_counter++;

/*#ifdef EAR_PROTECT
        if(g_EarProtect_flag == 1)
        {
            counter_flag = ear_protect_callback();
            if(counter_flag==TRUE)
            {
                g_decrease_time++;
            }
            else
            {
                g_decrease_time = 0;
            }

            g_half_hour_counter++;
            g_autoswitch_counter++;
        }
#endif*/
    
        /*2HZ计时关背光*/
        if ((g_light_time != 0) && (g_rtc_counter >= g_light_time) && (g_light_flag != 0))
        {
            Lightmodeset(FALSE); //开关背光（true: 开, false:关）       //zxs add for test
            g_light_flag = FALSE;
        }

        /*
	 			if((g_light_time <= 30) && (g_light_time != 0))
	 					g_keylight_time = g_light_time;
	 			else
	 					g_keylight_time = 30;
	 	*/				
	 	#if 0//(MACHINE_MODEL==1)				
        if( (g_rtc_counter >= g_keylight_time) && (g_keylight_flag != 0))
        {        	 
                    sfr_bak = SFR_BANK;
            		SFR_BANK = BANK_GPIO;
            		GPIOCOUTEN |= 1;
            		GPIOCDAT &=~ 1;
            		SFR_BANK = sfr_bak;
            		g_keylight_flag = FALSE;
			 			
        }
        #endif

        //软件锁
        if ((g_lockflag == FALSE) && (g_lock_time != 0) && (g_rtc_counter >= g_lock_time))
        {
            g_lockflag = TRUE;
            goto key_hold;
        }
        
        ///*充电不關機 pedometer*/
        #if(MACHINE_MODEL==1)
        if ((GetUsbCableStatus() == 0) && !Pedo_work_flag && !Swim_work_flag)
        #elif(MACHINE_MODEL==2)
        if ((!check_5v()) && !Pedo_work_flag && !Swim_work_flag)
        #endif
        {
        /*2HZ计时进入睡眠模式*/
        //if ((g_sleep_time != 0) && (g_rtc_counter == g_sleep_time)) //睡眠
        //{
        //    return AP_MSG_FORCE_STANDBY;
        //}

        /*2HZ计时进入省电模式*/
        if ((g_standby_time != 0) && (g_rtc_counter == g_standby_time)) //自动关机
        {
            return AP_MSG_STANDBY;
        }
        }

        /*2HZ计时无操作返回*/
        if ((g_rtc_counter % 16) == 0)
        {
            if (ui_auto_select == TRUE)
            {
                /*
                if (!g_light_flag)
                {
                    Lightmodeset(TRUE);//开背光
                    g_light_flag = TRUE;
                    g_rtc_counter = 0;
                }
                
                #if(MACHINE_MODEL==1)
        		keylight_check();
        		#endif
        		*/
        			
                return AP_MSG_WAIT_TIMEOUT;//RESULT_AUTO_BACK;
            }
        }

        if (key == MSG_RTC2HZ)
        {
            key = AP_MSG_RTC;
        }

        //            /*充电检测2HZ计时，每30s监测一次充电状态*/
        #if(MACHINE_MODEL==1)
        if (GetUsbCableStatus() != 0)
        #elif(MACHINE_MODEL==2)
        if (check_5v())
        #endif
        {
            if (g_charge_counter == 60)
            {
                sKY_ChargeGetandSet();
                g_charge_counter = 0;
            }
            else
            {
                g_charge_counter++;
            }
        }
        break;

        case MSG_LOW_POWER:
        /*系统低电消息处理为强制关机*/
        lowPower_counter++;
        if (lowPower_counter >= 3)
        {
            key = AP_MSG_FORCE_STANDBY;

        }
        else
        {
            key = NULL;
        }
        break;

        case MSG_USB_UNSTICK:
        /*usb拔出时一定要检测一次充电状态才会立即停止充电*/
        sKY_ChargeSet(2);
        sKY_ChargeGetandSet();
        key = AP_KEY_NULL;
        break;

        case Msg_KeyUnHold:
//#ifdef lock_key
		g_lockflag = FALSE;
        key = Msg_KeyUnHold;
        key_count = 0;
        key_value = key;
//#endif
        break;

        case Msg_KeyHold:
key_hold:
//#ifdef lock_key
		if(!g_lockflag)
		{
			g_lockflag = TRUE;
      		key = AP_MSG_LOCK;
      		g_rtc_counter = 0;
      		key_count = 1;
      		key_value = key;
		}
//#endif
		 if (!g_light_flag)
         {
              Lightmodeset(TRUE);//开背光
              g_light_flag = TRUE;
             // g_rtc_counter = 0;
         }

            #if(MACHINE_MODEL==1)
        	keylight_check();
        	#endif
        	
        	g_rtc_counter = 0;
        	key = AP_MSG_LOCK;     
        	key_count = 1;
        	key_value = key;

        break;

        case MSG_SD_IN:/*插卡状态*/
        key = AP_MSG_SD_IN;
        break;

        case MSG_SD_OUT:/*拨卡状态*/
        if ((bGlobal_Critical & 0x02) == 2)
        {
            key = AP_MSG_SD_OUT;
        }
        else
        {
            key = 0;
        }
        break;

#ifdef ALARM_CLOCK
        case Msg_RtcAlarm:
        result = set_fmrecord_or_alarm_msg();
        if(result == RESULT_AUTORECORD)
        {
            key = AP_MSG_FMRECORD;
        }
#ifdef AUTO_FMREC
        else if(result==RESULT_ALARM)
        {
            key = AP_MSG_ALARM;
        }
#endif
        else
        {
            key=0;
        }
        break;
#endif

        default: /*新按键处理*/
        g_rtc_counter = 0;
		key_value = key;
/*#ifdef lock_key
		if(g_lockflag)
		{
			key = AP_MSG_LOCK;
			Lightmodeset(TRUE);//开背光
			g_light_flag = TRUE;        
		}
#endif*/
		key_count = 1;
        if (!g_light_flag)
        {
            /*如背灯没开，不处理消息*/
            key_count = 0;
            //   g_rtc_counter = 0;
            return NULL;
        }

        Open_KeyTone(g_comval.KeyTone);
    } /*end of switch*/

    return key;
}

/*
 ********************************************************************************
 * Description : 处理热键消息, 返回结果消息
 *
 * Arguments   : key, 按键消息
 *
 * Returns     : 如果有可识别的热键消息,  返回结果消息
 如果没有可识别的热键消息,  返回0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint8 ap_handle_hotkey_core(uint8 key)
{
    if (usbplugflag == AP_MSG_USB_PLUGIN_FLAG)
    {
        return RESULT_UDISK;
    }
    switch (key)
    {
        case AP_KEY_PLAY | AP_KEY_LONG: //长按play发standby
        case AP_MSG_STANDBY: //自动关机
        case AP_MSG_FORCE_STANDBY: //使用上两个会报label duplicate
        return RESULT_STANDBY;

        case AP_KEY_MODE | AP_KEY_LONG: //长按menu进入main
            if(g_lockflag)
        	{
				Lightmodeset(TRUE);//开背光
        		g_light_flag = TRUE;
                g_rtc_counter = 0;
                g_lockflag = FALSE;
                g_lockflag2 = TRUE;
                key_count = 0;
                if (ui_run_realtime == TRUE) //实时模式不显示lock
                {
                    return 0;
                }
                else
                {
                    ui_show_lock(0); //这里不处理热键,防止嵌套
//                  ap_sleep(4);
                    return RESULT_REDRAW;
        	    }
        	}
        	else
        	    return RESULT_MAIN;

        case RESULT_AUTO_BACK:
            return AP_MSG_WAIT_TIMEOUT;
            //return RESULT_AUTO_BACK;
       
        /*case AP_KEY_AB | AP_KEY_LONG: //进入录音
        return RESULT_RECORD;*/

		
        case AP_MSG_SD_OUT:
            ui_show_CardOut();
            return RESULT_SD_OUT;
        
        case Msg_KeyUnHold:
//		  if(ui_run_realtime)    //maya090512#2 for hold clear
//        {
//            #if(!(MAIN_AP||AMV_AP||VOICE_AP||PHOTO_AP))	//matt for hold
//            SetBackgdColor(Color_BLACK_def);
//            ClearScreen(&region);
//            #endif
//        }
		//return RESULT_REDRAW;
		if (ui_run_realtime == TRUE) //实时模式不显示lock
        {
            return 0;
        }
        else
        {
            ui_show_lock(0); //这里不处理热键,防止嵌套
//            ap_sleep(4);
            return RESULT_REDRAW;
        }
        case AP_MSG_LOCK: //locked
        if (ui_run_realtime == TRUE) //实时模式不显示lock
        {
            return 0;
        }
        else
        {
            ui_show_lock(1); //这里不处理热键,防止嵌套
//            ap_sleep(4);
            return RESULT_REDRAW;
        }
#ifdef ALARM_CLOCK
        case AP_MSG_ALARM:
        return RESULT_ALARM;
#endif

#ifdef  AUTO_FMREC
        case AP_MSG_FMRECORD:
        return RESULT_AUTORECORD;
#endif

        default:
        //here just drop the key
        return 0;
    }
}




