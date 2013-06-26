#include "actos.h"
#include "key.h"
#include "string.h"

#define NO_KEY 0xff
#pragma renamecode(?PR?KY_COM)

#define GPIOA_BITS  0x1b
#define GPIOA_MASK  0xe4
#define GPIOE_BITS  0xe1
#define GPIOE_MASK  0x1e

extern void sKY_CheckCardStatus(void);
extern void sKY_CheckUsbStatus(void);

uint8 newkey = NO_KEY;
uint8 oldkey = NO_KEY;
uint8 tmpkey = NO_KEY;
uint8 tmp_count, hold_count;
uint8 s_timer;
uint8 wait_keyup = 0;
uint8 WPCardStatue = 0;
uint8 cardpowerflag = 0;
uint8 cardoutflag = 0;
uint8 batDetectCnt = 0;
bool HOLD_FLAG=FALSE;
uint8 earInFlag = 0;
uint8 earOutFlag = 1;
uint8 const key_map[16];

uint8 gsensor_type=0;
uint32 step_counter=0;		//20110524	已运动步数
uint32 step_second=0;
uint32 step_counter_swim=0;		//20110524	已运动步数
uint32 step_second_swim=0;
uint16 gsensor_xadc1=0;
uint16 gsensor_yadc1=0;
uint16 gsensor_zadc1=0;
uint8 gsensor_period=0;		//20110524 扫描Gsensor的时间间隔计数，每20MS计数一次
uint8 gsensor_workflag=0;	//20110524	1:扫描	0:不扫描	
uint8 gsensor_workflag_swim=0;
uint16 gsensor_gap=0;
uint8 gsensor_got=0;


extern uint8 earphoneinflag;
extern uint8 sysverflag;


void key_scan(void)
{
    uint8 sfr_bak;
    uint8 key_val, adcdat;
	

    sfr_bak = (uint8) SFR_BANK;

    //demo直驱与CLD的切换
    /*SFR_BANK = BANK_GPIO;
    if (earphoneinflag != 0) //运行cld自动切换
    {
        if ((GPIOADAT & 0x01) == 1) //无耳机插入，高电平
        {
            if (!earOutFlag) //首次耳机拔出检测
            {
                EnableCLD();
                earOutFlag = 1;
            }
            earInFlag = 0;
        }

        if ((GPIOADAT & 0x01) == 0) //有耳机插入，低电平
        {
            if (!earInFlag) //首次耳机插入检测
            {
                DisableCLD();
                earInFlag = 1;
            }
            earOutFlag = 0;
        }
    }
    else
    {
        earInFlag = 0;
        earOutFlag = 0;
    }*/

#if 0 //20110424	因为触摸按键代码长，故屏蔽卡的检测先
    sKY_CheckCardStatus();
#endif

    if (!sysverflag)
    {
        sKY_CheckUsbStatus();
    }


#if (PETOMETER_USE)
	if(gsensor_workflag	|| gsensor_workflag_swim)			//是否要扫描
	{
	    gsensor_gap ++;
		gsensor_period++;	
		if (gsensor_period >= 1)			//30ms扫描一次，	扫描周期(Unit:*30ms) 
		{
			//sKY_ReadGSensor();
			gsensor_period=0;
			gsensor_got = 1;
		}	
	}
#endif


    SFR_BANK = BANK_PMU;
    adcdat = LRADC1_DATA & 0x3f;

    if (adcdat >= 56) //white110121#1 for hold adc
    {
			//if(!HOLD_FLAG)  //SC903
    	  	//{
    	  	//  HOLD_FLAG=TRUE;
    	  	//  PutSysMsg(Msg_KeyHold);//锁键消息
    	  	//}
        	key_val = NO_KEY;
    } 
    else if(adcdat > 35)
    {
    	 if(HOLD_FLAG)
    	 {
    	   PutSysMsg(Msg_KeyUnHold);//解锁
    	   HOLD_FLAG=FALSE;
    	 }
    	 else
    	    key_val = NO_KEY;	    	 
    }
    //else if ((adcdat >=0) && (adcdat <= 5)) //0
	//{
    //    key_val = 2;
	//}
    else if ((adcdat>=5) && (adcdat <= 15)) //14
	{
        key_val = 6;
    }
	//else if ((adcdat>15) && (adcdat <= 25)) //27
    //{
	//    	key_val = 4;
    //}
	//else if ((adcdat>25) && (adcdat <= 35)) //40
    //{
     //   key_val = 5;
    //} 
//	else if ((adcdat > 44) && (adcdat <= 48)) //47
//    {
//        key_val = 6;
//    }
    else
    {
        key_val = NO_KEY; //未扫描出有按键按下
    }
   
    //加入play键处理
    if (PRESS_DISPLAY & 0xc0) //bit6，7不为0，长按/短按play，统一发play，转到AP去处理
    {
        key_val = 1;
        if(HOLD_FLAG)
    	  {
    	   		PutSysMsg(Msg_KeyHold);//解锁
            PRESS_DISPLAY = (uint8) (PRESS_DISPLAY | 0xc0);//清掉PRESS_DIAPLAY  bit7和bit6
    	  }
    	  else 
    	  {
        		PutSysMsg(key_map[key_val]);
      	}
        SFR_BANK = sfr_bak;
        return;
    }
    
    ///////////////////////////////////////////////////////////////
    //Next,Menu键处理
	SFR_BANK = BANK_GPIO;
	GPIOCINEN |= 0x03;
	if ((GPIOCDAT & 0x02) == 0) //gpioc0 
	    key_val = 5;
	else if ((GPIOCDAT & 0x01) == 0)
	    key_val = 4;
	GPIOCINEN &= ~0x03;
	GPIOCOUTEN &= ~0x03;
    SFR_BANK = sfr_bak; 
    ////////////////////////////////////////////////////////////////
    
    if (key_val != oldkey) //如果本次扫描值不等于上次按键值
    {
        if (key_val == tmpkey) //如果本次键值等于前次扫描值
        {
            tmp_count++; //累加扫描次数

            if (tmp_count > 3)
            {
                tmp_count = 3;
            }

            if (tmp_count == 3) //扫描三次，是同一键值，确定有按键按下
            {
                if (key_val != NO_KEY) //如果本次扫描捕获键值不为空
                {
                    oldkey = tmpkey;
                    wait_keyup = 1;
        								if(HOLD_FLAG)
										{
											 PutSysMsg(Msg_KeyHold);
										}
										else
										{                   		
                    		PutSysMsg(key_map[oldkey]);
                  	}
                }
                else //本次键值为NO_KEY
                {
                    hold_count = 0;
                    oldkey = NO_KEY;
                    tmpkey = NO_KEY;

                    if (wait_keyup) //等待KEYUP
                    {
                        wait_keyup = 0;
                        tmp_count = 0; //清扫描次数
                        PutSysMsg(Msg_KeyShortUp); //发keyup消息
                    }
                }
            }
        }
        else //如果本次扫描值不等于前次扫描值，则重置键值，重新计数
        {
            tmpkey = key_val;
            if (!wait_keyup)
            {
                tmp_count = 0;
            }
        }
    }
    else //如果本次键值等于上次按键值,累计16次(320ms)，发一次按键消息
    {
        if (key_val != NO_KEY) //
        {
            hold_count++;

            if (hold_count == 16)
            {
                hold_count = 0;
        					if(HOLD_FLAG)       		 
                   PutSysMsg(Msg_KeyHold);//锁键锁上           		   
                else                                   
                PutSysMsg(key_map[oldkey]); //发键值消息
            }
        }
        else
        {
            tmpkey = key_val;
            hold_count = 0;
            if (!wait_keyup)
            {
                tmp_count = 0;
            }
        }
    }

    SFR_BANK = sfr_bak;
}


//cts isr, send	2hz msg
void ctc_isr(void)
{
    //send 2-hz msg
    uint8 sfr_bak;

    PutSysMsg(Msg_HalfSecond); //send 2-hz msg
    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_RTC;

    CTCCtl |= 1 << 7; //enable ctc
    SFR_BANK = sfr_bak;
}

