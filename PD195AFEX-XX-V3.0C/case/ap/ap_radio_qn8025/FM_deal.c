/*
 *******************************************************************************
 *                ACTOS AP
 *        main ap of sdk 3.0
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *       History Record:
 *      1.解决护耳模式开启时音量变化不能保存的问题，Mars，2008-04-15
 *******************************************************************************
 */
#include "ap_radio.h"

#pragma name(FM_DEAL)

extern fm_play_status_t m_FMPlaystatus;
extern uint8 EnterFlag;
extern void ui_show_volfm(BYTE volume);
extern uint16 g_CNT;//2hz 中断计数器(0.5秒加一)
extern uint8 To_FMRec_standby; //FM录音从standby起来
extern FMRADIO_param_t radio_param;
extern function_menu_t menu; //菜单结构体
extern comval_t g_comval;
extern uint8 Updateflag; //刷新标志
extern uint8 value;
extern bool volmuteflag; //静音标志
extern uint8 AutoTabNum;
extern uint8 Prs_nor;//1:prs 换台模式  0:nor 微调模式
//电台频率对照表(注:频率范围87M-108M（US）或76M-90M（Japan）,以K为精度)
extern uint16 FMStationTab[60];
extern fm_play_status_t FMStatus; //当前电台信息,8 bytes
extern fm_play_status_t FMStatusUS; //US电台信息暂存,8 bytes
extern fm_play_status_t FMStatusJP; //Japan电台信息暂存,8bytes
extern uint8 fmdataflag[3]; //第一次进FM标志，3 bytes
extern uint8 Freqtab[10];
extern uint16 active; //进入菜单的选中号
extern BOOL fm_VolOpFlag;
extern uint8 nopresetflag;
extern uint8 fmconnect[2];
extern slider_t slider;

extern uint16 FMAutoTab[30]; // 全自动收台,预存台.
//bool Need_refresh = 1;
extern uint8 FMFreNumOld; //上次显示频率数字的位数
//#define  MENU_COUNT  8

const uint16 string_nor[] =
{ FMENU1, FMENU16}; //FMENU15:电台录音 by ccm
const uint16 string_pre[]=
{ FMENU2, FMENU16};

const uint16 string_nor_pedo[] =   //pedometer
{ FMENU1, FMENU16, PEDO, SWIMFIT}; //FMENU15:电台录音 by ccm
const uint16 string_pre_pedo[]=
{ FMENU2, FMENU16, PEDO, SWIMFIT};

//函数定义
extern uint16 SearchHandle(uint8 Direct); //自动搜索处理，Direct=10，NEXT，=20，LAST
extern uint16 AutoSearchHandle(uint8 Direct);
extern void ShowFreqInFM(void); //FM主界面显示
extern uint16 ChangeVolume(void);
extern bool Search_Show(FM_direct_t Direct); //自动搜台时的显示
extern void DealFadeIn(uint8 Volume);
extern void DealFadeInOut(void);
extern void read_var(void);
extern void SetMfpToNand(void);
extern void FM_Wait1ms(uint16 time);
extern void ShowFreqInFM2(void);
extern void ShowSignalST(void);

uint16 FMdeal(void)
{
    uint16 result = 0;
    uint8 key;
	uint8 modekeystore = NULL;
    //uint8 sfr_bak;
//    uint8 buf[4] =
//    { "FM" };
    //标志：need_draw，重画 BandSwitch，频段切换 FirstWait，刚进AP
    bool need_draw=TRUE/*,need_draw_all=TRUE*/,BandSwitch=TRUE,FirstWait=TRUE;  
//    uint16 k;
    // uint8 strvolume = 0; //czh bit7:flash enable flag, bit 6:flash flag, other bits: count time
    uint8 count = 0;
	uint8 nopresetnum=0;
//	uint8 buf[2];

	//ResShowPic(IM_WAITING, 0, 0);
    /*显示请稍后*/
    show_waiting();
    
    read_var();
    g_comval.KeyTone=0;
    //clear the message pool  清除“初始化...”过程中按下的键!
    while (ap_get_message() != AP_KEY_NULL)
    {
        ; //清空消息池
    }
    
    while (1)
    {
#ifndef PC
//////////////////////////////////////////////////////////////
            if (FMStatus.mode == 0)
            {
            memcpy(&FMStatusUS, &FMStatus, sizeof(FMStatus));
            memcpy(&FMStatus, &FMStatusJP, sizeof(FMStatus));
            BandSwitch = TRUE;
            }
/////////////////////////////////////////////////////////////////
        if (BandSwitch == TRUE) //初始化
        {
            {
                radio_param.param1 = FMStationTab;
            }
            (fm_play_status_t *) radio_param.param2 = &FMStatus;

            fmSendCommand(MC_RESTORE, (uint16) & radio_param);
            fmSendCommand(MC_SETVOLUME, 0);
            if (!fmSendCommand(MC_OPEN, SEARCHLEVEL)) //硬件错误提示
            {
//                SetPenColor(Color_WHITE_def);
//                SetBackgdColor(Color_BULE_def);
                ClearScreen(NULL);
                ResShowMultiString(FMERROR, 255, 8);
                strcpy(fmdataflag, "  ");
                ap_sleep(3);
                return RESULT_MAIN;
            }
            fmSendCommand(MC_MUTE, SetMUTE);
            fmSendCommand(MC_PLAY, 0);
            BandSwitch = FALSE;
            FirstWait = TRUE;
        }
#endif   
		/*if(need_draw_all)
        {
            ResShowPic(IM_RADIO_BG,0,0);
            need_draw_all=FALSE;
            need_draw=TRUE;
        }*/
        
        if ((need_draw == TRUE) && (To_FMRec_standby == FALSE)) //屏幕重画
        {
            if(Pedo_ui_draw)
            {
                Pedo_RefreshMainScreen(Pedo_Swim_flag);
            }    
            else 
            {
                ShowFreqInFM();
            }
            need_draw = FALSE;
        }
        
        if (FirstWait == TRUE) //初次进入AP等待1.5S释放MUTE(静音)
        {
            FirstWait = FALSE;
            ap_sleep(3);
			FMStatus.volume = FMStatus.vol_display;
            fmSendCommand(MC_SETVOLUME, FMStatus.volume);
            fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
            if (FMStatus.volume != 0)
            {
                fmSendCommand(MC_MUTE, releaseMUTE);
            }
        }
        
        if (To_FMRec_standby == TRUE)
        {
            To_FMRec_standby = FALSE;
            return RESULT_FMREC_NOSTART; //返回到FM录音
        }
       
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
        
        key = ap_get_message();
        switch (key)

        {
            case AP_MSG_STANDBY:
            break;
            
            case AP_MSG_RTC: //2HZ中断到来，屏幕重画，为了电池显示更新
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
				else
				{
                    count++;
                    if (count == 4)
                    {
                        count = 0;
                        fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
                        if(FMStatus.stereo_status==0)        
                		{                
                    		ResShowPic(IM_RADIO_STEREO,40,16);        //显示立体声
                		}
                		else
                		{
                    		ResShowPic(IM_RADIO_MONO,40,16);
                		}
                		ShowSignalST();  
                    }
                    
                    ui_show_batt(GetBattery()); //电池电量显示
             	
    			    if(nopresetflag == 1)
                    {
    				    nopresetnum++;
    				    if(nopresetnum >= 4)
    				    {
    					    need_draw = TRUE;
    					    nopresetflag = 0;
    					    nopresetnum = 0;
    				    }
                    }
				}
      		    break;
				
          case AP_KEY_HOLD | AP_KEY_NEXT:
		  #if(MACHINE_MODEL == 2)
                if(Pedo_ui_draw)
        		{
        			break;
        		}  
        		else
        		{
				    ChangeVolume();
			        need_draw = TRUE;
			    }
		  #elif(MACHINE_MODEL == 1)
		       if(Pedo_ui_draw) break;
                if(Prs_nor == 0)
        		{
                   	result = SearchHandle(10);
        			fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
        		}
		  #endif
        		while( ap_get_message() != AP_KEY_NULL )
                {
                    ; //清空消息池
                }
                break;

            case AP_KEY_HOLD | AP_KEY_PREV:
			#if(MACHINE_MODEL == 2)
                if(Pedo_ui_draw)
        		{
        			break;
        		}  
        		else
        		{
				    ChangeVolume();
			        need_draw = TRUE;
			    }
			#elif(MACHINE_MODEL == 1)
			    if(Pedo_ui_draw) break;
                if(Prs_nor == 0)
        		{
                   	result = SearchHandle(20);
                   	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
        	  	}
			#endif
    	  		while( ap_get_message() != AP_KEY_NULL );
                {
                    ; //清空消息池
                }
                break;

            case AP_KEY_NEXT | AP_KEY_UP:
                if(Pedo_ui_draw) 
                {
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
    			}
    			
                if(Prs_nor == 1)
    		   	{
                	fmSendCommand(MC_MUTE, SetMUTE);
                	SetPAVolume(0);
                	fmSendCommand(MC_STEP, UP);
                	if (FMStatus.volume != 0)
                	{
                    	fmSendCommand(MC_MUTE, releaseMUTE);
                	}
                	DealFadeIn(FMStatus.volume);
                	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
    //            	Updateflag = 1;
    		   	}
    			else
    			{
    				/*itoa2(m_FMPlaystatus.station, buf);
    				ResShowPic(SYSFONT_SMALL_0 + buf[0] - 0x30, 15, 0);
    				ResShowPic(SYSFONT_SMALL_0 + buf[1] - 0x30, 15 + 8, 0);*/
    				
    				fmSendCommand(MC_STEPGRID, UP); //电台频率微调，以100KHZ为步进		
                	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
    			}
    //            Updateflag = 1;
                need_draw = TRUE;
                break;

            case AP_KEY_PREV | AP_KEY_UP:	
                if(Pedo_ui_draw) 
                {
                    Pedo_ui_draw = FALSE;
                	need_draw = TRUE;
                	break;
                }
                if(Prs_nor == 1)
    		   	{
                	fmSendCommand(MC_MUTE, SetMUTE);
                	SetPAVolume(0);
                	fmSendCommand(MC_STEP, DOWN);
                	if (FMStatus.volume != 0)
                	{
                    	fmSendCommand(MC_MUTE, releaseMUTE);
                	}
                	DealFadeIn(FMStatus.volume);
                	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
    //            	Updateflag = 1;
    		   	}
    			else
    			{
    				fmSendCommand(MC_STEPGRID, DOWN);
                	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
    			}
                need_draw = TRUE;
                break;

			case AP_KEY_AB | AP_KEY_UP:
			case AP_KEY_VOLSUB:
            case AP_KEY_VOLADD:
			    if(Pedo_ui_draw)
        		{
        			break;
        		}  
        		else
        		{
				    ChangeVolume();
//			        need_draw_all = TRUE;
			        need_draw = TRUE;
			    }
			    break;
			    
			case AP_KEY_AB | AP_KEY_LONG:
				return RESULT_FMREC_START;
			
            case AP_KEY_MODE:
                modekeystore = key;
                break;

            case AP_KEY_MODE | AP_KEY_UP:
                if (modekeystore != AP_KEY_MODE)
                { //要先按了AP_KEY_MODE　键后up才算有效
                    modekeystore = NULL;
                    break;
                }
                if(Pedo_ui_draw)
        		{
        		    result = Pedo_menu(Pedo_Swim_flag);
                    need_draw = TRUE;
                    if(result == RESULT_RETURN_TO_MAIN)
                    {
                        //sKY_SetGsensor(0,0,0);
                	    //Pedo_work_flag = FALSE;
                	    Pedo_ui_draw = FALSE;
                	    need_draw = TRUE;
                	    result = 0;
                	    break;
                    }
                    if ((result == RESULT_IGNORE) || (result == RESULT_AUTO_BACK))
                	{
                    	result = 0;
                    	break;
                	}
        			if(result!= NULL)
			        {
			   	        return  result;
			        }
        		}  
        		
    			if(Prs_nor == 1)
    		   	{
                	if(g_comval.PedoFlag)
                	{
                        menu.total = sizeof(string_pre_pedo) / 2; //MENU_COUNT; 
                        menu.string = string_pre_pedo;
                	}
                    else
                    {
                        menu.total = sizeof(string_pre) / 2; //MENU_COUNT; 
                        menu.string = string_pre;
                	}
                	menu.active = 0;
    				menu.menuhead= NULL; //RADIO;
    				menu.string_id=g_comval.langid;
                	//ui_auto_select = TRUE;
                	result = ui_function_menu(&menu, NULL);
                	//ui_auto_select = FALSE;
    				need_draw = TRUE;
    //				need_draw_all=TRUE;
                //按PLAY键和8s超时返回FM收音界面,不执行当前激活的菜单!
                	if ((result == RESULT_IGNORE) || (result == RESULT_AUTO_BACK))
                	{
                    	result = 0;
                    	break;
                	}
                	if (result != 0)
                	{
                    	return result;
                	}
                	active = menu.active;
                	if (menu.active == 0) //删除电台
                	{
    					if (fmSendCommand(MC_DELONE, 0) != FALSE)
                    	{
                        	fmSendCommand(MC_BACKUP, (uint16) & radio_param);
                    	}
                    	fmSendCommand(MC_STEP, UP);
                    	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
                	}
    				else  if(menu.active == 1)
                	{
                		//ShowFreqInFM2();
    					fmSendCommand(MC_DELALL, 0);
                    	fmSendCommand(MC_BACKUP, (uint16) & radio_param);
                    	fmSendCommand(MC_MUTE, SetMUTE);
                    	result = AutoSearchHandle(10);
                    	if (result == RESULT_IGNORE)
                    	{
                        	result = 0;
                    	}
                    	if (result != 0)
                    	{
                    		return result;
                    	}
                    	if (FMStatus.volume != 0)
                    	{
                        	fmSendCommand(MC_MUTE, releaseMUTE);
                    	}
                    	fmSendCommand(MC_SETVOLUME, FMStatus.volume);
                    	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
    //					need_draw_all=FALSE;         
    				}
    				else if(menu.active == 2)    //pedometer
    				{
                        Pedo_ui_draw = TRUE;
            			Pedo_Swim_flag = FALSE;
            			if(Swim_work_flag)  Swim_work_flag = FALSE;
                        need_draw = TRUE;
    				}
    				else //if(menu.active == 3)    //swim fitness
    				{
                        Pedo_ui_draw = TRUE;	  
            			Pedo_Swim_flag = TRUE;
            			if(Pedo_work_flag)  Pedo_work_flag = FALSE;
                        need_draw = TRUE;
    				}
    				    
    			}
    			else
    			{		
    			    if(g_comval.PedoFlag)
                	{
                        menu.total = sizeof(string_nor_pedo) / 2; //MENU_COUNT; 
                        menu.string = string_nor_pedo;
                	}
                    else
                    {
                        menu.total = sizeof(string_nor) / 2; //MENU_COUNT; 
                        menu.string = string_nor;
                	}
                	menu.active = 0;
                	menu.menuhead= NULL; //RADIO;
                	menu.string_id = g_comval.langid;           	
                	//ui_auto_select = TRUE
                	result = ui_function_menu(&menu, NULL);
                	//ui_auto_select = FALSE;
                	need_draw = TRUE;
    //				need_draw_all=TRUE;
                //按PLAY键和8s超时返回FM收音界面,不执行当前激活的菜单!
                	if ((result == RESULT_IGNORE) || (result == RESULT_AUTO_BACK))
                	{
                    	result = 0;
                    	break;
                	}
                	if (result != 0)
                	{
                    	return result;
                	}
                	active = menu.active;
                	if (menu.active == 0) //保存电台
                	{
    					if (fmSendCommand(MC_SAVESTATION, 0) != FALSE)
                    	{
    						Prs_nor=1;
    						fmSendCommand(MC_BACKUP, (uint16) & radio_param);
    					}
    				}
                	else if(menu.active == 1)
                	{
                		fmSendCommand(MC_DELALL, 0);
                    	fmSendCommand(MC_BACKUP, (uint16) & radio_param);
                    	fmSendCommand(MC_MUTE, SetMUTE);
                    	result = AutoSearchHandle(10);
                    	if (result == RESULT_IGNORE)
                    	{
                        	result = 0;
                    	}
                    	if (result != 0)
                    	{               		
                        	return result;
                    	}
                    	if (FMStatus.volume != 0)
                    	{
                        	fmSendCommand(MC_MUTE, releaseMUTE);
                    	}
                    	fmSendCommand(MC_SETVOLUME, FMStatus.volume);
                    	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);       
    //					need_draw_all=FALSE; 
    				}   
    				else if(menu.active == 2)    //pedometer
    				{
                        Pedo_ui_draw = TRUE;
            			Pedo_Swim_flag = FALSE;
            			if(Swim_work_flag)  Swim_work_flag = FALSE;
                        need_draw = TRUE;
    				}
    				else //if(menu.active == 3)    //swim fitness
    				{
                        Pedo_ui_draw = TRUE;	  
            			Pedo_Swim_flag = TRUE;
            			if(Pedo_work_flag)  Pedo_work_flag = FALSE;
                        need_draw = TRUE;
    				}
    				    
    			}
                /*else if (list.active == 5) //选择欧美频段
                {
                    if (FMStatus.mode == 1)
                    {
                        memcpy(&FMStatusJP, &FMStatus, sizeof(FMStatus));
                        memcpy(&FMStatus, &FMStatusUS, sizeof(FMStatus));
                        BandSwitch = TRUE;
                    }
                }
                else if (list.active == 6) //选择日本频段
                {
                    if (FMStatus.mode == 0)
                    {
                        memcpy(&FMStatusUS, &FMStatus, sizeof(FMStatus));
                        memcpy(&FMStatus, &FMStatusJP, sizeof(FMStatus));
                        BandSwitch = TRUE;
                    }
                }

                else
                {

                }  */
                break;
            
            case AP_KEY_PLAY | AP_KEY_UP: //跳到电台表的下一个
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
        		else
        		{
                    if(Prs_nor)
		   	            Prs_nor = 0;
    		        else
    		        {
    		   	        Prs_nor = 1;
    		   	        fmSendCommand(MC_MUTE, SetMUTE);
                	    SetPAVolume(0);
                	    fmSendCommand(MC_STEP, UP);
                	    if (FMStatus.volume != 0)
                	    {
                    	    fmSendCommand(MC_MUTE, releaseMUTE);
                	    }
                	    DealFadeIn(FMStatus.volume);
                	    fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
    		   	    }
		   	    }
                need_draw = TRUE;
                break;
	    
            default: //热键处理
                key = ap_handle_hotkey(key);
                if (key == RESULT_REDRAW)
                {
    //                  need_draw_all=TRUE;
                      need_draw = TRUE;
    //                  Need_refresh=0;	//matt for FM hold display
                }
                else if (key == RESULT_RECORD)
                {
                    return RESULT_FMREC_START;
                }
                else
                {
                    if ((key != 0) || (result != 0))
                    {
                        if (key != 0)
                        {
                            return key;
                        }
                        else
                        {
                            return result;
                        }
                    }
                }
        }
    }
    return key;
}
