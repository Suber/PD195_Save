#include "kapcfg.h"
#include "stringH.h"


#pragma name(KCFG_M)
extern unsigned long systemtime;
extern unsigned long gwtime;
void power_control(void);
extern void          _nop_     (void);

uint8 InitDateTime;
uint32 rtctime;
date_t date;
time_t time;
kval_t kval;

int16 main(int16 param)
{  
	uint8 sfr_bak;
      param=param; 
   	ClearWatchDog();
    if( !DRV_Install("vm_nanf.drv",0) )
    {
        while(1) {;}
    }

	VMRead(&kval,VM_KERNEL,sizeof(kval_t));
    if (kval.magic == MAGIC_KVAL)
    {
        //是否掉电判别
        if (IsNotPowerDown() == 0) //返回  0：掉电   非0：没掉电
        {
            InitDateTime = 1; //需要初始化
        }
        else
        {
            InitDateTime = 0; //不需要初始化
        }
    }
    else //升级完第一次开机
    {
        InitDateTime = 1; //需要初始化
    }
    if (InitDateTime == 1)
    {         
        kval.rtcresetflag = 0x55; //设置掉电标志
        date.year = 2010;
        date.month = 10;
        date.day = 29;
        time.hour = 1;
        time.minute = 0;
        time.second = 0;
        TM_SetGWDate(&date, 0); //会修改gwtime  systemtime和gwtime采用系统初始值
        TM_SetGWTime(&time); //会修改gwtime  systemtime和gwtime采用系统初始值

        date.year = 2010;
        TM_SetDate(&date, 0); //会修改systemtime   systemtime和gwtime采用系统初始值
        TM_SetTime(&time); //会修改systemtime   systemtime和gwtime采用系统初始值
        //将systemtime 和 gwtime写回到VRAM中
        kval.magic = MAGIC_KVAL;
        kval.systemtime = systemtime; //在TM_SetDate被修改
        kval.gwtime = gwtime; //在TM_SetGWDate被修改
        VMWrite(&kval, VM_KERNEL);
		sfr_bak = SFR_BANK;
		SFR_BANK = BANK_RTC;
		RTC_CTL0|=0x80;
		RTCFlush();
		SFR_BANK = sfr_bak;
   		ClearWatchDog();
    }
    else
    {
  
    }
   		ClearWatchDog();

    //从VRAM中取出systemtime 和 gwtime
    systemtime = kval.systemtime;
    gwtime = kval.gwtime;

	//power control
	SetVDD(VDDSET_1_6V);
	SetPLL(PLL_24MHZ);
	power_control();
    enable_interrupt();
    return 0xffff;
 
}


void power_control(void)
{
    uint8 sfr_bak;
    sfr_bak = SFR_BANK;

 	SFR_BANK = BANK_CMU_RMU;
	MRCR3|=0x08;
	_nop_();
	_nop_();
 	_nop_();
 	_nop_();
	MRCR2|=0x08;
	_nop_();
	_nop_();

	SFR_BANK = BANK_PMU;
	VDD_DCDC_CTL = (VDD_DCDC_CTL & 0xC1)|0x16;
	BDG_CTL|=0x40;
	EFUSE_CTL|=0x40;
	SFR_BANK = BANK_USB;
	USB_PHYCTRL|=0xC0;
	USB_Efuse_Ref|= 0x40;
 	SFR_BANK = BANK_CMU_RMU;
	MRCR1 &= 0xf7;
	MCSR2 &= 0xe0;
	MCSR3 &= 0xbf;
	MCSR5 &= 0xe7;

    SFR_BANK = sfr_bak;
}

 