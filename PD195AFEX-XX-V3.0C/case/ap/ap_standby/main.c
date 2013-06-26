/*
 ********************************************************************************
 *                       ACTOS
 *               standby ap of sdk 3.0
 *
 *                (c) Copyright 2002-2003, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File   : main.c
 * By     : Gongee
 * Version: 1> v1.00     first version     1/12/2004 11:10PM
 ********************************************************************************
 * $Id: main.c,v 1.1.1.1 2006/05/22 03:23:59 selina Exp $
 ********************************************************************************
 */
#include "ap_common.h"
#include "actos.h"
#include "Filesys.h"
#include "uhdrv.h"

#pragma name(AP_STANDBY)

#define DISK_C 0
#define DISK_H 1

#define SPACE_LOW           60L    //剩余扇区数 

#define PEDO_RECORD_MAX     8
#define PEDO_RECORD_SAVE_MAX     8
#define PEDO_RECORD_MAGIC   0x1721

typedef unsigned int    UInt;



//UInt g_peim_save_handle_test;

uint8 far GPIOBuf[20];
uint8 far intBuf[2];
uint8   far PMUBuf[2];
uint8   far RTCBuf[3];
uint8   far ClockBuf[13];
uint8   far NANDBuf[4];
uint8   far USBBuf[2];
uint8   far RESETBuf[3];
uint8   far PWMBuf;
uint8   far DIODEBuf;
uint8   far temp_sfrbank;

pedo_save_vars_t far pedo_save_list[PEDO_RECORD_SAVE_MAX];
pedo_save_vars_head_t far pedo_save_head;
char far buff[512];


extern UInt g_peim_save_handle; 

uint8 draw_standby_logo(void);
#ifndef PC
void Enter_HardStandby(void);
void Enter_SoftStandby(void);
#endif
 
extern uint8 StandbyCharge(void);
void ui_standby_msg(uint16 msgID);
void time_delay(uint8 time);
uint8 time_sleep(uint16 half_sec);
uint8 Get_PowerON_Message(void);
void RecoverPara(void);
extern void _nop_(void);
void MCU_delay(void);
void LOSC_delay(void);
void SetParaToStandby(void);
void MyDI(void);
void MyEI(void);
void SetRESET(void);
void RecoverRESET(void);
void SetRTC(void);
void RecoverRTC(void);
void SetPMU(void);
void RecoverPMU(void);
void SetNAND(void);
void RecoverNAND(void);
void SetPWM(void);
void RecoverPWM(void);
void SetGPIOReg(void);
void RecoverGPIOReg(void);
void SetUSBEfuse(void);
void RecoverUSBEfuse(void);
void SetAllClock(void);
void RecoverAllClock(void);
void SetDIODE(void);
void RecoverDIODE(void);
void SetToLOSC(void);
void RecoverHOSC(void);
void read_var(void);

extern uint8 dir(uint8 mode);
extern uint8 pedo_save(uint8 mode);
uint8 pedo_save_disk(uint8 num, uint8 *filename, uint8 mode);



comval_t g_comval;
extern STG_INFO StorageInfor;
extern uint8 sysverflag;



int main(int param)
{
    BOOL disk_ok = FALSE;
    //BOOL status;
    uint8 result;
    uint8 retval;
    uint8 bat_value;
    uint8 rtc_count = 0;
    uint8 key = 0;
    uint8 num;
    uint8 pedoname[12] = "PEDO    DAT";
    uint8 swimname[12] = "SWIM    DAT";
    if (param > 0)
    {
    }
	/*
    VMRead(&g_comval, VM_SYSTEM, sizeof(g_comval));
    g_comval.SleepTime = 0; //此变量要清0，使睡眠模式设置一次仅一次有效。
    VMWrite(&g_comval, VM_SYSTEM);
    */
    
    if(!DRV_ActiveUD(DISK_C, MODE_SLOW))
    {
        return 0;
    }
    
    read_var();
 
    if (!DRV_ActiveDrive('C', MODE_SLOW))
    {
        return 0;
    }

    
    FS_CD(":");
    
    if(dir(0))     
    {
        num = pedo_save(0);
    }

    pedo_save_disk(num, pedoname, 0);
    
    FS_CD(":");   
    if(dir(1))     
    {
        num = pedo_save(1);
    }

    
    pedo_save_disk(num, swimname, 1);
    
    DRV_DetectDrive('C');  
    DRV_DetectUD(DISK_C);

     
    
    


 
    
    #if(MACHINE_MODEL==0)
    //检测是否接上充电器或USB，retval=0XC0或0x40为USB，retval=0x80为充电器
    retval = GetUsbCableStatus();
    #elif((MACHINE_MODEL==1)||(MACHINE_MODEL==2))
    if (check_5v()) retval = 0x40;
    else    retval = 0;
    #endif
    //充电检测
    //开始充电
//    sKY_ChargeSet(1);
//    sKY_ChargeGetandSet();
    if ((retval & 0xc0) != 0)
    {
        if ((retval & 0x40) != 0)
        {
            return RESULT_UDISK;
        }
        if (sKY_ChargeSet(0) != 0x0)
        {
            res_fp = ResOpen("ui30.res");
            result = StandbyCharge(); //显示充电界面
            ResClose(res_fp);
            if (result != RESULT_IGNORE)
            {
                return result;
            }
        }
        else
        {
            if ((retval & 0x40) != 0)
            {
                return RESULT_UDISK;
            }
            else
            {
                return RESULT_MAIN;
            }
        }

    }
    else
    {
        bat_value = GetBattery();
        if (bat_value <= 0x54)
        {
            res_fp = ResOpen("ui30.res");
//            ui_standby_msg(LOWPOWER);//显示低电界面
            ResClose(res_fp);
        }
    }
    draw_standby_logo();//显示关机LOGO

    

    ClearScreen(NULL); //清屏
    StandbyScreen(0); //关背光和屏
    UpdateScreen(NULL);
    
    time_delay(2);    //wait VCC off
    

#ifndef PC
 //   if (sysverflag != 0) //进入软开关模式
    {
        Enter_SoftStandby();
    }
  //  else //进入硬开关模式
    //长按键抬起后才真正进入低频关机状态
    {
        do
        {

            key = ap_get_message();
            if (key == AP_KEY_NULL)
            {
            }
            else if ((key == AP_MSG_RTC) || (key == AP_MSG_CHARGING))
            {
                rtc_count++;
            }
            else
            {
                rtc_count = 0;
            }

            //check time out
            if (rtc_count >= 2)
            {
                break;
            }
        } while ((key & AP_KEY_UP) != AP_KEY_UP);
        Enter_HardStandby(); //进入硬开关模式，函数跑的是LOSC低频运行
        while (1)
        {
            ClearWatchDog();
            result = Get_PowerON_Message();//获取长按PLAY键消息或者插入USB线，充电器时的消息
            switch (result)
            {
                case RESULT_POWERON:
                case RESULT_UDISK:
                RecoverPara();//恢复高频，然后开机
                //DMA3Init(0); //重新初始化DMA3，否刚屏亮不起来
                StandbyScreen(1);//开背光，亮屏
                SFR_BANK = temp_sfrbank;
                break;
                default:
                break;
            }
            if (result != 0)
            {
                break;
            }
        }

    }
    return result;
#else
    RefreshDisplayPC();
    while(1)
    {
        SIM_Sleep(200);
    }
#endif

}
/********************************************************************************
 * Description : 进入软开关既S3模式，这时VCC，VDD全部断开
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void Enter_SoftStandby(void)
{
    //PMU RTCVDD域寄存器有问题,需要外部低频或者反复的写这一位，;
    //disable VCC/VDD
    uint8 count = 0;
    SFR_BANK = 0x01;
    //关闭单PIN低频，可以省8，9个UA的功耗。
    LOSC1_CTL = (uint8 ) (LOSC1_CTL & 0xfe);
    LOSC2_CTL = (uint8 ) (LOSC2_CTL & 0xfe);
    while (count < 4)
    {
        SFR_BANK = 0x05;
        SYSTEM_CTL_RTCVDD = (uint8 ) (SYSTEM_CTL_RTCVDD & 0xfe);
        time_sleep(6);
        count++;
    }

}
/********************************************************************************
 * Description : 进入硬开关，既S2模式，系统跑的是低频32K
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void Enter_HardStandby(void)
{
    temp_sfrbank = SFR_BANK;
    SetParaToStandby();
    SetToLOSC();
}
/********************************************************************************
 * Description : 显示关机LOGO
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
uint8 draw_standby_logo(void)
{
    uint16 i;
    uint16 result;

    #if(MACHINE_MODEL == 1)
    VMRead(&g_comval, VM_SYSTEM, sizeof(g_comval));
    res_fp = ResOpen("ui30.res");
    SetLanguage(g_comval.langid);
    ClearScreen(NULL);//清屏
    SetAsciiFont(FONT_TYPE_LARGE);
    ResShowMultiString(SHUTDOWN, 255, 16);
    UpdateScreen(NULL);//清屏
    ResClose(res_fp);
    #endif
    
    res_fp = ResOpen("end.res");
    for (i = 1; i <= 5; i++)
    {
        #if(MACHINE_MODEL == 1)
        result = ResShowPic(i, 0, 40);
        #else
        result = ResShowPic(i, 0, 0);
        #endif
        ClearWatchDog();
        if (result == 0)
        {
            ResClose(res_fp);
            return FALSE; //format error
        }
        //software timer
#ifdef PC
        RefreshDisplayPC();
        SIM_Sleep(200);
#else
        time_delay(1);
#endif

    }

    ResClose(res_fp);
    return TRUE;
}

void time_delay(uint8 time)
{
    uint16 i;
    uint32 timer;
    for (i = 0; i < 10 * time; i++)
    {
        timer = 8000;
        ClearWatchDog();
        while (timer > 0)
        {
            --timer;
        }
    }

}

uint8 time_sleep(uint16 half_sec)
{
    uint8 n = 0; //rtc counter
    uint8 key;
    while (1)
    {
        key = ap_get_message();
        if (key == AP_MSG_RTC)
        {
            n++;
            if (n >= (uint8) half_sec)
            {
                break;
            }
        }
    }
    return 0;
}
void ui_standby_msg(uint16 msgID)
{


    ClearScreen(NULL);
    ResShowMultiString(msgID, 255, 80);
	UpdateScreen(NULL);
    time_delay(8);

}

/********************************************************************************
 * Description : 恢复高频，恢复其他设置的一些参数
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void RecoverPara(void)
{

    RecoverHOSC();
    MyEI();
    RecoverRESET();
    RecoverUSBEfuse();
    RecoverRTC();
    RecoverNAND();
    //    RecoverPWM();
    RecoverGPIOReg();
    RecoverPMU();
    RecoverAllClock();
    RecoverDIODE();
    //恢复高频24M，VDD1.6V
    //主要是防止黑屏自动关机时，PLL设成6M，VDD为1.4V，开机起来没有恢复。
    SetPLL(PLL_24MHZ);
    SetVDD(VDDSET_1_6V);
}
/********************************************************************************
 * Description : 进入STANDBY之前，设置相关寄存器，降低功耗
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void SetParaToStandby(void)
{
    DisablePA();
    DisableDAC();
    DisableAIN();
    SetRESET();
    SetUSBEfuse();
    SetRTC();
    SetNAND();
    //    SetPWM();
    SetGPIOReg();
    SetPMU();
    SetAllClock();
    SetDIODE();
    MyDI();
}
void SetDIODE(void)
{
    SFR_BANK = 0x05;
    DIODEBuf = DIODE_CTL;
    DIODE_CTL = (uint8 ) (DIODE_CTL & 0x7f);
}
void RecoverDIODE(void)
{
    SFR_BANK = 0x05;
    DIODE_CTL = DIODEBuf;
    DIODE_CTL = (uint8 ) (DIODE_CTL | 0x80);
    MCU_delay();
}

/********************************************************************************
 * Description : 低频32K下的延时，大概为3MS
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/

void LOSC_delay(void)
{
    uint8 time = 0x10;
    while (1)
    {
        time--;
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        ClearWatchDog();
        if (time == 0)
        {
            break;
        }

    }
}
/********************************************************************************
 * Description : MCU24M时的延时
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void MCU_delay(void)
{
    uint8 time = 0xff;
    while (1)
    {
        time--;
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        ClearWatchDog();
        if (time == 0)
        {
            break;
        }
    }
}
void SetRESET(void)
{
    SFR_BANK = 0x01;
    RESETBuf[0] = MRCR1;
    RESETBuf[1] = MRCR3;
    RESETBuf[2] = MRCR2;
#ifndef PC
    if(StorageInfor.ce_info[0].type != 3) 
	{
	     //bit4 set to 1:nand flash controller,DMA5 controller&file system accelerator reset to normal
	    MRCR1 = 0x10;
	    //bit3 set to 1,this bit should be reset before USB reset bit is reset,bit2 set to 1,PWM controller reset to normal
	    MRCR3 = 0x0e;
	    //bit3 set to 1,USB&DMA6 reset to normal
	    MRCR2 = 0x08;
	}
	else  //card base
	{
	    //bit5 set to 1: SD card
	    MRCR1 = 0x20;
	    //bit3 set to 1,this bit should be reset before USB reset bit is reset,bit2 set to 1,PWM controller reset to normal
	    MRCR3 = 0x0e;
	    //bit3 set to 1,USB&DMA6 bit 2: DMA4
	    MRCR2 = 0x0C;
	}
#endif

    _nop_();
    _nop_();
    _nop_();
}
void RecoverRESET(void)
{
    SFR_BANK = 0x01;
    MRCR1 = RESETBuf[0];
    MRCR3 = RESETBuf[1];
    MRCR2 = RESETBuf[2];
    _nop_();
    _nop_();
    _nop_();
}
void SetUSBEfuse(void)
{
    SFR_BANK = 0x07;
//    USBBuf[0] = USB_Efuse_Ref;
    USBBuf[1] = USB_PHYCTRL;
//    USB_Efuse_Ref = (uint8 ) (USB_Efuse_Ref | 0x40);//bit6 set to 1, efuse is standby,and its standby current is 5UA max
    USB_PHYCTRL = (uint8 ) (USB_PHYCTRL | 0x80);//enable phy_PLLEn
    USB_PHYCTRL = (uint8 ) (USB_PHYCTRL | 0x40);//enable phy_dallallen
    _nop_();
    _nop_();
}
void RecoverUSBEfuse(void)
{
    SFR_BANK = 0x07;
//    USB_Efuse_Ref = USBBuf[0];
    USB_PHYCTRL = USBBuf[1];
    _nop_();
    _nop_();
}
void SetRTC(void)
{
    SFR_BANK = 0x0c;
    RTCBuf[0] = RTC_CTL0;
    RTCBuf[1] = WDCtl;
    RTCBuf[2] = RTC_CTL1;
    RTC_CTL0 = (uint8 ) (RTC_CTL0 & 0x7f);
    WDCtl = (uint8 ) (WDCtl | 0x08);
    WDCtl = (uint8 ) (WDCtl & 0x7f);
    RTC_CTL1 = (uint8 ) (RTC_CTL1 & 0xbf);
    RTC_CTL1 = (uint8 ) (RTC_CTL1 & 0x7f);
    _nop_();
    _nop_();
}
void RecoverRTC(void)
{
    SFR_BANK = 0x0c;
    RTC_CTL0 = RTCBuf[0];
    WDCtl = RTCBuf[1];
    RTC_CTL1 = RTCBuf[2];
    _nop_();
    _nop_();
}
void SetNAND(void)
{
    SFR_BANK = 0x06;
    NANDBuf[0] = PADDRV1;
    NANDBuf[1] = PADDRV0;
    NANDBuf[2] = PADPUPD0;
    NANDBuf[3] = PADPUPD1;
    PADDRV1 = 0x0;
    PADDRV0 = 0x0;
    PADPUPD0 = (uint8 ) (PADPUPD0 & 0xfb);
    PADPUPD0 = (uint8 ) (PADPUPD0 & 0xf7);
    PADPUPD1 = (uint8 ) (PADPUPD1 & 0x7f);
    _nop_();
    _nop_();
}
void RecoverNAND(void)
{
    SFR_BANK = 0x06;
    PADDRV1 = NANDBuf[0];
    PADDRV0 = NANDBuf[1];
    PADPUPD0 = NANDBuf[2];
    PADPUPD1 = NANDBuf[3];
    _nop_();
    _nop_();
}
/*--------------------------------PWM---------------------------------------
 //PWM进Standby时，有两种情况
 //1.PWM并不是作背光控制，只需要将clock源切到LOSC，并且将模块reset写"0"
 //2.假如此时用PWM控制屏的背光，则进入Standby时，PWM选择LOSC，PWMDUTY寄存器写成0x00，
 //  PWM便可以输出“0",背光一直被关闭，关闭的时机参考LCD进standby步骤
 */
void SetPWM(void)
{
    SFR_BANK = 0x06;
    PWMBuf = PWMDUTY;
    PWMDUTY = 0x0;
}
void RecoverPWM(void)
{
    SFR_BANK = 0x06;
    PWMDUTY = PWMBuf;
}
void SetGPIOReg(void)
{

    uint8 i = 0;
    SFR_BANK = 0x06;
    //GPIP_A
    GPIOBuf[i] = GPIOAOUTEN;
    i++;
    GPIOAOUTEN = 0xff;

    GPIOBuf[i] = GPIOAINEN;
    i++;
    GPIOAINEN = 0x0;

    GPIOBuf[i] = GPIOADAT;
    i++;
    GPIOADAT = 0xff;
    MCU_delay();

    //GPIP_B
    GPIOBuf[i] = GPIOBOUTEN;
    i++;
    GPIOBOUTEN = 0xff;

    GPIOBuf[i] = GPIOBINEN;
    i++;
    GPIOBINEN = 0x0;

    GPIOBuf[i] = GPIOBDAT;
    i++;
    GPIOBDAT = 0xff;

#if 0   //由于GPIOC 做为开关背光用，所以这里不能对其进行操作，否则屏会亮一下
    MCU_delay();
    GPIOBuf[i] = GPIOCOUTEN;
    i++;
    GPIOCOUTEN = 0xff;
    GPIOBuf[i] = GPIOCDAT;
    i++;
    GPIOCDAT = (uint8 ) (GPIOCDAT | 0xfd);
    GPIOCDAT = (uint8 ) (GPIOCDAT & 0xfd);
#endif
    MCU_delay();

    //GPIP_D
    GPIOBuf[i] = GPIODOUTEN;
    i++;
    GPIODOUTEN = 0xff;

    GPIOBuf[i] = GPIODINEN;
    i++;
    GPIODINEN = 0x0;

    GPIOBuf[i] = GPIODDAT;
    i++;
    GPIODDAT = 0xff;
    MCU_delay();

    //GPIP_E
    GPIOBuf[i] = GPIOEOUTEN;
    i++;
    //GPIOE4为卡检测，进standby时必须将E4输出口disable,
    //否则插上卡时，硬件上会将E4口直接下拉到地，
    //从而导致STANDBY电流过高
    GPIOEOUTEN = 0xef;

    GPIOBuf[i] = GPIOEINEN;
    i++;
    GPIOEINEN = 0x0;
    GPIOBuf[i] = GPIOEDAT;
    i++;
    //  GPIOEDAT |= 0xe8;
    //GPIOEDAT &= 0xe8;
    GPIOEDAT = 0xff;
    MCU_delay();

    //GPIP_F
    GPIOBuf[i] = GPIOFOUTEN;
    i++;
    GPIOFOUTEN = 0xff;

    GPIOBuf[i] = GPIOFINEN;
    i++;
    GPIOFINEN = 0x0;

    GPIOBuf[i] = GPIOFDAT;
    i++;
    GPIOFDAT = 0xff;
    MCU_delay();

    //GPIP_G
    MFPSEL6 = (uint8 ) (MFPSEL6 & 0xfe);
    GPIOBuf[i] = GPIOGOUTEN;
    i++;
    GPIOGOUTEN = 0xff;

    GPIOBuf[i] = GPIOGINEN;
    i++;
    GPIOGINEN = 0x0;

    GPIOBuf[i] = GPIOGDAT;
    GPIOGDAT = 0xff;
    MCU_delay();
}
void RecoverGPIOReg(void)
{

    uint8 i = 0;
    SFR_BANK = 0x06;
    GPIOAOUTEN = GPIOBuf[i];
    i++;
    GPIOAINEN = GPIOBuf[i];
    i++;
    GPIOADAT = GPIOBuf[i];
    i++;
    MCU_delay();

    GPIOBOUTEN = GPIOBuf[i];
    i++;
    GPIOBINEN = GPIOBuf[i];
    i++;
    GPIOBDAT = GPIOBuf[i];
    i++;
    MCU_delay();
#if 0
    GPIOCOUTEN = GPIOBuf[i];
    i++;
    GPIOCDAT = GPIOBuf[i];
    i++;
#endif
    MCU_delay();

    GPIODOUTEN = GPIOBuf[i];
    i++;
    GPIODINEN = GPIOBuf[i];
    i++;
    GPIODDAT = GPIOBuf[i];
    i++;
    MCU_delay();

    GPIOEOUTEN = GPIOBuf[i];
    i++;
    GPIOEINEN = GPIOBuf[i];
    i++;
    GPIOEDAT = GPIOBuf[i];
    i++;
    MCU_delay();

    GPIOFOUTEN = GPIOBuf[i];
    i++;
    GPIOFINEN = GPIOBuf[i];
    i++;
    GPIOFDAT = GPIOBuf[i];
    i++;
    MCU_delay();

    GPIOGOUTEN = GPIOBuf[i];
    i++;
    GPIOGINEN = GPIOBuf[i];
    i++;
    GPIOGDAT = GPIOBuf[i];
    MCU_delay();
}
void SetPMU(void)
{
/*
    uint8 bdg_ctl;
    SFR_BANK = 0x05;
    PMUBuf[0] = BDG_CTL;
    PMUBuf[1] = EFUSE_CTL;
    bdg_ctl = BDG_CTL;
    BDG_CTL = bdg_ctl;
    BDG_CTL = (uint8 ) (BDG_CTL | 0x40);
    BDG_CTL = (uint8 ) (BDG_CTL & 0xE0);
    MCU_delay();
    EFUSE_CTL = (uint8 ) (EFUSE_CTL | 0x40);
*/}
void RecoverPMU(void)
{
/*
    SFR_BANK = 0x05;
    BDG_CTL = PMUBuf[0];
    MCU_delay();
    EFUSE_CTL = PMUBuf[1];
*/}

void SetAllClock(void)
{
    SFR_BANK = 0x01;
    ClockBuf[0] = MCSR1;
    ClockBuf[1] = MCSR2;
    ClockBuf[2] = AUDIOPLL_CTL;
    ClockBuf[3] = MCSR3;
    ClockBuf[4] = MCSR4;
    ClockBuf[5] = MCSR5;
    ClockBuf[6] = MCSR6;
    ClockBuf[7] = MCSR7;
    ClockBuf[8] = MCSR8;
    ClockBuf[9] = MCSR9;
    ClockBuf[10] = MCSR10;
    ClockBuf[11] = MCSR11;
    ClockBuf[12] = MCSR12;

    MCSR1 = 0x0;
    MCSR2 = 0x0;
    AUDIOPLL_CTL = (uint8 ) (AUDIOPLL_CTL & 0xfe);
    MCSR3 = 0x0;
    MCSR4 = (uint8 ) (MCSR4 & 0x03);
    MCSR5 = (uint8 ) (MCSR5 & 0x20);
    MCSR6 = 0x0;
    MCSR7 = 0x0;
    MCSR8 = 0x0;
    MCSR9 = 0x0;
    MCSR10 = 0x0;
    MCSR11 = 0x0;
    MCSR12 = 0x0;
    _nop_();
    _nop_();
    _nop_();
}
void RecoverAllClock(void)
{
    SFR_BANK = 0x01;
    MCSR1 = ClockBuf[0];
    MCSR2 = ClockBuf[1];
    AUDIOPLL_CTL = ClockBuf[2];
    MCSR3 = ClockBuf[3];
    MCSR4 = ClockBuf[4];
    MCSR5 = ClockBuf[5];
    MCSR6 = ClockBuf[6];
    MCSR7 = ClockBuf[7];
    MCSR8 = ClockBuf[8];
    MCSR9 = ClockBuf[9];
    MCSR10 = ClockBuf[10];
    MCSR11 = ClockBuf[11];
    MCSR12 = ClockBuf[12];
}
void SetToLOSC(void)
{
    SFR_BANK = 0x01;
    LOSC1_CTL = (uint8 ) (LOSC1_CTL & 0xfe);
    LOSC2_CTL = (uint8 ) (LOSC2_CTL & 0xfe);
    MCSR4 = (uint8 ) (MCSR4 & 0xfc);
    MCSR4 = (uint8 ) (MCSR4 | 0x00);
    _nop_();
    _nop_();
    _nop_();
    ClearWatchDog();
    //   SetAllClock();
    LOSC_delay();
    HOSC_CTL = (uint8 ) (HOSC_CTL & 0xf7);
    HOSC_CTL = (uint8 ) (HOSC_CTL & 0xfe);
}
void RecoverHOSC(void)
{
    uint8 mcsr_val;
    SFR_BANK = 0x01;
    //    RecoverAllClock();
    HOSC_CTL = (uint8 ) (HOSC_CTL | 0x01); //HOSC Enable
    LOSC_delay();
    _nop_();
    _nop_();
    mcsr_val = (uint8 ) (MCSR4 & 0xfe);//切换HOSC模式
    mcsr_val = (uint8 ) (mcsr_val | 0x02); //MCU clock=24M
    MCSR4 = mcsr_val;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    LOSC1_CTL = (uint8 ) (LOSC1_CTL & 0xfe); //disable LOSC
    _nop_();
    _nop_();   
}
//#endif
/********************************************************************************
 * Description : 获取USB或充电器的状态，
 *
 * Arguments  :
 *
 *
 * Returns     :dpdm_status=0X40表示插入的是USB线，dpdm_status=0X80为充电器
 *            无
 * Notes       :
 *
 ********************************************************************************/
uint8 getUSBStatus(void)
{
    uint8 sfr_bak;
    uint8 dpdm_status;
    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_USB;
    dpdm_status = (uint8 ) (DPDMCTRL & 0x40);
    SFR_BANK = BANK_PMU;
    dpdm_status = (uint8 ) (dpdm_status | (SYSTEM_VOL & 0x80));
    SFR_BANK = sfr_bak;
    return dpdm_status;
}
/********************************************************************************
 * Description : 获取开机的消息
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            RESULT_POWERON：长按PLAY键开机
 RESULT_UDISK：插入USB线开机
 RESULT_MAIN：插入充电器开机
 * Notes       :
 *
 ********************************************************************************/
uint8 Get_PowerON_Message(void)
{
    uint8 temp;
    uint8 USBStatus = 0;
    USBStatus = getUSBStatus();//获取USB或充电器消息
    //初始化PLAY键寄存器
    SFR_BANK = BANK_PMU;
    temp = SYSTEM_CTL_RTCVDD;
    temp = temp & 0x3f;
    SYSTEM_CTL_RTCVDD = temp;
    if ((PRESS_DISPLAY & 0x40) > 0) //检测长按PLAY键消息
    {

        SFR_BANK = BANK_PMU;
        if ((BATADC_DATA & 0x7f) < 0x51) //检测到低电时，不开机
        {
            return 0;
        }
        else
        {
            return RESULT_POWERON;
        }
    }
    else if (USBStatus > 0)
    {
        return RESULT_UDISK;
    }
    else
    {
        return 0;
    }
}
/********************************************************************************
 * Description : 关闭所有的中断
 *
 * Arguments  :
 *
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************/
void MyDI(void)
{
    intBuf[0] = IE0;
    intBuf[1] = AIE;
    IE0 = 0x0;
    AIE = 0x0;
}
/********************************************************************************
 * Description : 恢复中断
 *
 * Arguments  :
 *
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************/
void MyEI(void)
{
    IE0 = intBuf[0];
    AIE = intBuf[1];
}



void read_var(void)
{
    int16 result;

    result = VMRead(&g_comval, VM_SYSTEM, sizeof(g_comval));
    if (g_comval.magic != MAGIC_COMVAL)
    {
        GetDefaultComval(&g_comval);
    }
    ap_message_init(&g_comval);
}

/*
 ********************************************************************************
 * Description : 设置PED缺省目录\RecordX         //set default path:\RecordX
 *
 * Arguments   :  返回FALSE的情况：1 F16 根目录， 2 磁盘满了 3 文件系统BUG
 *
 * Returns     :
 *
 * Notes       : 把\RecordX\做为当前目录//set "\RecordX\" as current directory
 *
 ********************************************************************************
 */
	//if(isFmRecord)
	//	strcpy(g_record_vars.location.path, g_dir_rec_fm[1]);
	//else


    	//if(isFmRecord)
		//	status = FS_CD(g_dir_rec_fm[i]);
		//else

			//if(isFmRecord)
			//	status = FS_MakeDir(g_dir_rec_fm[i]);
		//	else


			//if(isFmRecord)
			//{
				//if (0 == memcmp(file, g_dir_rec_fm[i], 8))
	          //  {
	          //      break;
	         //   }
		//	}
		//	else
