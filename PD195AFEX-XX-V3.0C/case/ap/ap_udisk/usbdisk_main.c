/********************************************************************************
*                              AS211A
*                            Module: USB MAIN
*                 Copyright(c) 2001-2010 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*      wanghaijing 2010-3-10         1.0                build this file 
********************************************************************************/
/*!
* \file     usbdisk.c
* \brief    
* \author   wanghaijing
* \par      GENERAL DESCRIPTION:
*               这里对文件进行描述
* \par      EXTERNALIZED FUNCTIONS:
*               这里描述调用到外面的模块
*
*      Copyright(c) 2001-2010 Actions Semiconductor, All Rights Reserved.
*
* \version 1.0
* \date  2010-3-10
*******************************************************************************/

#include "Udisk.h"

#pragma name(AP_MAIN)

uint8 DisplayIndi,UdiskStatus;
uint8 MenuKeyPress;
uint8 IsCardOut; 
uint8 ChargeCheckCounter;
uint8 gRtcCounter; 
comval_t g_comval; //全局变量数据结构 
uint8 HoldDispState;
//res_infor_t res_infor[5];
uint8 Precess_cnt=0;
uint8 ChargeState;
uint8 full_flag = FALSE, charging_flag = TRUE;
uint8 bat_value;
uint8 init_flag=1;
#pragma renamecode(?PR?AP_MAIN)

int main(void)
{
    uint8 para = 0x01;	
	uint8 dispIndibak=0xff;
    uint8 rii=0;
    uint8  cnt=0;
	uint8 sfr_bak;
	sfr_bak = SFR_BANK;
    DisplayIndi=0x00; 	                         
    ChargeCheckCounter = 0;
    gRtcCounter = 0;
	     
	//sKY_ChargeSet(1);    //充电          
    //sKY_ChargeGetandSet(); 

	//note:在udisk的升级过程中,注意不要调用涉及到读flashSD区的操做.
	ChargeState = sKY_ChargeSet(0);
//	Lightmodeset(1);

    #if(MACHINE_MODEL==2)
      sfr_bak = SFR_BANK;
	  SFR_BANK = BANK_GPIO;
	  GPIOCOUTEN |= 1;
      GPIOCDAT |= 1;     
      SFR_BANK = sfr_bak;
    #endif
      
	/*AP初始化*/ 	
	if(!UDiskInitAll(para))                     
    {
		//提前拔线退出
		#if(MACHINE_MODEL==1)
		UdiskStatus = 0x40;
		return  UdiskExitAll();
		#elif(MACHINE_MODEL==2)
        init_flag = 0;
		#endif
    }       
    while(1)
    {
        ClearWatchDog(); 
		/*deal message */
        MsgManager(DisplayIndi);
                           
        if((IsCardOut == 1) && init_flag)
        {            
            UDiskPullCardOut();
            IsCardOut = 0;
        }

				//调用状态机执行命令 
        if(init_flag)	
        	UDiskHandle();		
				//获取USB状态                  
        //0:空闲  10h:上传  20h:下传  30h:弹出要求退出  40h:拔线要求退出  50h:切换到韧件升级应用 
        //60h:分区处理状态  70h:出错要求退出  80h:进入Suspend状态  90h:退出Suspend状态 
        if(!init_flag)
        	UdiskStatus = 0;
        else
        	UdiskStatus = UDiskGetStatus();   

        if((DisplayIndi==0xff) && ( UdiskStatus !=0x80))
        {   
           //StandbyScreen(1);
           SetContrast(g_comval.DisplayContrast);      
        }

        #if(MACHINE_MODEL==1)
        if((UdiskStatus==0x30) || (UdiskStatus==0x40) || (UdiskStatus==0x50) || (UdiskStatus==0x70))
        {
            break;     
        }
        #elif(MACHINE_MODEL==2)
        if((UdiskStatus==0x30) || (UdiskStatus==0x40) || /*(UdiskStatus==0x50) ||*/ (UdiskStatus==0x70)|| (UdiskStatus==0x80))
        {
            init_flag = 0;     
        }
        if(UdiskStatus==0x50)
        {
            break;     
        }
        #endif
        
        sfr_bak=SFR_BANK;
   		SFR_BANK = BANK_PMU;
        if(!((SYSTEM_VOL & 0x80)||(CHG_CTL & 0x80)))
        {
        	return UdiskExitAll();
        }
     	SFR_BANK = sfr_bak;   
        

        switch(UdiskStatus)
        {
            case 0:      //空闲 
                if(DisplayIndi != 0x00)
                {   
                    cnt=0;
					Precess_cnt=0;
                    DisplayIndi=0x00;
                    Display(DisplayIndi);    
                }
				/*
				if (ChgMsgFlag)
                {
                       //充电消息来，要刷新电池电量显示                            
                    ChgMsgFlag = FALSE;
                    ui_show_battt();
                } 
				*/  
                break;
    
            case 0x10:	 //上传 
                if(DisplayIndi!=0x01)
                {
                    DisplayIndi=0x01;
                    Display(DisplayIndi);   
                }
                break;
                
            case 0x11://显示Commu
            case 0x21://显示DownLoad
            case 0x31://显示UpLoad
            case 0x41://显示Succ
            case 0x61://显示Succ，退出 
                DisplayIndi=UdiskStatus>>4;
                if (UdiskStatus==0x61)
                {
                    DisplayIndi -= 1;       
                }
                DisplayIndi+=0x10;				
                if(dispIndibak != DisplayIndi)    
                {
                    dispIndibak=DisplayIndi;
                    Display(DisplayIndi);
                }
                break;
                
            case 0x51:      //切换到adfu
                //关键盘和RTC中断 
				IE0 = IE0 & 0xf7;  
                DisplayIndi=0x10;
				//保证只显示一次 
                if(dispIndibak != DisplayIndi)       
                {
                    dispIndibak=DisplayIndi;
                    Display(DisplayIndi);
                }
                break;
                
            case 0x20:	   //下传 
               // if(DisplayIndi!=0x02)
                if(cnt++>200)
                {   
                    cnt=0;
					Precess_cnt++;
					if(Precess_cnt==8)
						Precess_cnt=0;
                    DisplayIndi=0x02;
                    Display(DisplayIndi);   
                }
                break;
                
            case 0x80:	  //进入suspend
                DisplayIndi=0xff;       
                break;
                
            case 0x90:    //退出Suspend状态
                //StandbyScreen(1);				
                //SetContrast(g_comval.DisplayContrast);      
                break;
                
            default:
                break;
        }    

        if(((MenuKeyPress==1) && (UdiskStatus==0))||(UdiskStatus==0x90))
        {   
            break;       
        }
        else
        {   
            MenuKeyPress = 0;           
        }               
    }/*end of while loop*/  
	
#ifdef  BACKLIGHTCTL 	
	LightOnOff(1);
#endif

	return  UdiskExitAll();              
}
