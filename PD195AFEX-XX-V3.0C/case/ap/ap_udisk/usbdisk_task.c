/*
********************************************************************************
*                       ACTOS
*                      usbfunctions
*
*                (c) Copyright 2002-2009, Actions Co,Ld.
*                        All Right Reserved
*
* File   : usbdisk_2.c
* By     : Mars
* Version: v1.00     first version     2009-11-10 14:05  by Mars 
********************************************************************************
*/
#include <reg251s.h>
#include "Udisk.h"


#pragma name(AP_TASK)
//void ui_show_batt();
void ReadBattPicInRam(void);
extern uint8 full_flag; 
extern uint8 charging_flag;
extern uint8 bat_value;
extern uint8 UdiskStatus;
extern uint8 MenuKeyPress;
extern uint8 IsCardOut;
extern uint8 ChargeCheckCounter;
extern uint8 gRtcCounter; 
extern comval_t g_comval;
extern uint8 HoldDispState;
//extern res_infor_t res_infor[5];
extern uint8 ChargeState;
extern uint8 init_flag;

#pragma renamecode(?PR?AP_TASK)
void MsgManager(uint8 dispIndi)
{
    uint8 i;
    uint8 sfr_bak;

    	sfr_bak = SFR_BANK;
    SFR_BANK = BANK_PMU;
    if(MenuKeyPress==1)
    {    
        return;
    }
    
	i = GetSysMsg();
    if( i == Msg_KeyMenu ) //收到Menu键
    {
    #if(MACHINE_MODEL==2)
    #ifdef  BACKLIGHTCTL
        if(UdiskStatus==0 && g_light_flag)
    #else
        if(UdiskStatus==0)
    #endif
		{
          	MenuKeyPress=1;
		}
	#endif
        return;
    }
    else if ( i == MSG_SD_OUT )
    {
        IsCardOut = 1;
        return;
    }
	
#ifdef  BACKLIGHTCTL  
    else if (i == Msg_KeyPlayPause) //PLAY键特殊处理
    {
        g_rtc_counter = 0;
        if ((PRESS_DISPLAY & 0x80) != 0) //短按play
        {
            i = (uint8) (i | AP_KEY_UP); //发送短按play
            PRESS_DISPLAY = (uint8) (PRESS_DISPLAY | 0x80); //清press_display bit7
            //Open_KeyTone(g_comval.KeyTone);
            if (!g_light_flag)
            {
                LightOnOff(TRUE);//开背光
                g_light_flag = TRUE;
			    SFR_BANK = sfr_bak;
                return;
            }
        }
        else if ((PRESS_DISPLAY & 0x40) != 0) //长按play
        {
            //blnInPlayLong = TRUE;
            if (!g_light_flag)
            {
                key_count = 0;
                SFR_BANK = sfr_bak;
                return;
            }
            i = (uint8) (i | AP_KEY_LONG); //发送长按play up
            PRESS_DISPLAY = (uint8) (PRESS_DISPLAY | 0x40); //清press_display bit6
            //Open_KeyTone(g_comval.KeyTone);
        }
        else
        {

        }
        SFR_BANK = sfr_bak;
        return;

    } 
    else if((i==Msg_KeyLongUp)||(i==Msg_KeyShortUp))
    {
        g_rtc_counter = 0;
        if (!g_light_flag)
        {
            LightOnOff(TRUE);//开关背光（TRUE: 开, FALSE:关)
            g_light_flag = TRUE;
        }
    }
    else if (i == Msg_KeyPlayPause) //PLAY键特殊处理
    {
        if ((PRESS_DISPLAY & 0x80) != 0) //短按play
        {
           
            PRESS_DISPLAY = (uint8) (PRESS_DISPLAY | 0x80); //清press_display bit7
           
        }
        else if ((PRESS_DISPLAY & 0x40) != 0) //长按play
        {
            PRESS_DISPLAY = (uint8) (PRESS_DISPLAY | 0x40); //清press_display bit6
        }
     }
#endif  

    else if((i == MSG_RTC2HZ) && (dispIndi<0x10)) //dispIndi>=0x10，升级状态,不能调用涉及到读flashSD区的操做.
    {
		g_rtc_counter++;
#ifdef  BACKLIGHTCTL  
        if(g_rtc_counter >= 120) //自动关背光
        {
            //关背光
            g_rtc_counter=130;
            LightOnOff(FALSE); //开关背光（true: 开, false:关)
            g_light_flag = FALSE;
        }
#endif         
        if(ChargeCheckCounter >= 60)
        {     	
			ChargeCheckCounter = 0;
        	sKY_ChargeGetandSet(); 
			
        	return;
		}
		ChargeCheckCounter++;
			ChargeState = sKY_ChargeSet(0);
            if (ChargeState == 0x01)
            {
                if ((full_flag == TRUE) && (charging_flag == FALSE))
                {
                    charging_flag = TRUE;
                    full_flag = FALSE;
                }
                if (bat_value < 4)
            	{
                	bat_value++;
            	}
            	else
            	{
                	bat_value = 0;
            	}
            	//if(init_flag)
                ResShowPic(IM_USB_BATTERY_01+bat_value, POSITION_BATT,0); 
            	//else
            	    //ResShowPic(IM_USB_POWER1+bat_value, 0,0); 
            }
            else
            {
                if ((ChargeState == 0x02) && (full_flag == FALSE))
                {
                    full_flag = TRUE;
                    charging_flag = FALSE;
                    //if(init_flag)
                    {
                    ResShowPic(IM_USB_BATTERY_05, POSITION_BATT,0); 
                        //ResShowPic(IM_USB_FULL, 0, 48);
                    }
                    //else
            	        //ResShowPic(IM_USB_POWER5, 0,0); 
                    //ResShowPic(IM_USB_FULL, 0, 48);
                }
                else if (ChargeState == 0x0)
                {
                    //if(init_flag)
                    ResShowPic(IM_USB_BATTERY_01, POSITION_BATT, 0);
                    //else
            	        //ResShowPic(IM_USB_POWER1, 0,0); 
                    //bat_value = 0x00;
                }
                else
                {
                }
             }		
		
    }
}
/********************************************************************************
* Description : 根据获取到的电压值显示相应的电池图标
*
* Arguments  : 
*            value：当前获取到的电压值
*		
* Returns     :
*            无 
* Notes       :  
*
********************************************************************************/
/*void ui_show_batt(void)
{	
#if 1    
    uint8 value,SFRBak;

    SFRBak = SFR_BANK;
	SFR_BANK = BANK_PMU;
    value = BATADC_DATA & 0x7f;
	SFR_BANK = SFRBak;

	if (ChargeState == 0x0) //当有接充电器或USB，而没有电池时，电池图标显示空
    {
        ResShowPic(IM_USB_BATTERY_05, 0, 0);//显示满格
		cur_value = 0x00;
    }
    else
    {
        if ( value <= 0x76)
        {
            if (cur_value < 4)
            {
                cur_value++;
            }
            else
            {
                cur_value = 0;
            }
        }
        else
        {
            for (cur_value = 0; cur_value < 5; cur_value++)
            {
                if (value <= batvalue[cur_value])
                {
                    break;
                }
            }
            if (cur_value >= 5)
            {
                cur_value = 4;
            }
        }
	}
    ResShowPic(IM_USB_BATTERY_01+cur_value, 0, 0);   
#endif    	       
} */

/********************************************************************************
* Description : 读battery 图片到缓存中
*
* Arguments  : 	ramaddr
*           
*		
* Returns     :
*            无 
* Notes       :  
*
********************************************************************************/
void ReadBattPicInRam(void)
{
#if 0    
	uint8 i;
	
	//read battery picture:total 5
	//res_infor[i].dwLength = res_infor[i].wWidth * res_infor[i].wHeight * 2 
	for (i=0x00;i<=0x04;i++)
	{
		SD_FSeek(res_fp, SEEK_SET, pic_entry_offset + (uint32) (BATTLVL1+i) * RESHEADITEM);
    	SD_FRead(res_fp, &res_infor[i], sizeof(res_infor_t));	
		SD_FSeek(res_fp, SEEK_SET, res_infor[i].dwOffset);
		SD_FRead(res_fp, (uint8*)(BATTPICBUFFER + i*BATTPICLENGTH), res_infor[i].dwLength);
	}	
#endif							
}


void Display(uint8 dispIndi)
{
#if 1    
	uint8 disptemp = dispIndi & 0x0f;
	uint8 IE0Bak;

	IE0Bak = IE0;	
	IE0 = IE0 & 0x7f; //显示时不开中断，避免低频跑中断服务（下面代码会降频），降低效率	

    if(dispIndi<0x10)
    {
        if(dispIndi==0x00) //显示任务指示 0:无显示动作  01:上传  02:下传  03:传输结束
        {
            if (full_flag == TRUE) 
                ResShowPic(IM_USB_FULL, 0, 48);
            else
			    ResShowPic(IM_USB_CONNECT,0,48);
//			ui_show_batt();
        }
	    
        if(dispIndi==0x01)
        {
			ResShowPic(IM_USB_TRANS,0,48);
//			ResShowPic(UDISKUPL, 60, 76);
        }
		
        if(dispIndi==0x02)
        {
			ResShowPic(IM_USB_TRANS,0,48);
			//ResShowPic(RW_GUAGE1+Process, 32, 40);
        } 
        
    }
    else
    {
    }

	IE0 = IE0Bak; //恢复中断
#endif	
}


void FlashLed(void)
{
}
