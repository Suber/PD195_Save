/*
 ********************************************************************************
 *                       ACTOS
 *                 show logo when boot
 *
 *                (c) Copyright 2003-2009, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File   : welcome.c
 * By     : fiona.yang
 ********************************************************************************
 */
#include "ak211x.h"
//#include <intrins.h>
#include "typeext.h"
//#include "lcd.h"
#include "string.h"
#include "display.h"

#define USB_EFUSE_VALID
//#define USB_EFUSE_INVALID

//#define DEMO_EFUSE

extern uint32 sGetBuffInfo(void);
extern void sAdfuEnter(void);
//extern void mcu_delay(void);
//EXTRN 		CODE		(sGetBuffInfo?_)
extern void sGetLogoData(uint8 cnt, uint16 addr);

extern void Hdrv_LCDInit(void);
extern void Show_welcome_logo();
//from ui_drvier
extern void Hdrv_SetWindow(region_t *pic);
extern void Hdrv_SetDrawMode(uint8 mode);
extern void Hdrv_BuffDataTrans(uint8* pbuf, uint16 len);
extern void Hdrv_LightOnOff(bool blctl);
extern void Delay_NMS(uint8 delay_ms);

extern void _nop_(void);
void usb_efuse_done(void);
void mcu_enter_sleep(void);
void mcu_enter_losc(void);

#pragma name(CODEDISPSTART)

void DispStarting(void)
{
    uint8 sfrbak;
    #if(MACHINE_MODEL==0)
    uint8 dpdm_status;
    #endif
	//for low power
	uint8 batval;
	uint8 loopcnt=0;
	uint8 keyval;
	//====================
	uint8 sysversion;
    uint8 *buffer_addr = (uint8*) 0x3000;
    uint16 sec_cnt = 0;
 
#if DEMO_EFUSE == 1
    usb_efuse_done();
#endif

    #if((MACHINE_MODEL==1)||(MACHINE_MODEL==2))
    	sfrbak = SFR_BANK;
		SFR_BANK = BANK_GPIO;
		GPIOCOUTEN |= 0x10; //Disable DP/DM 音频信号 
		#if(EARPHONE_ON)
		GPIOCDAT &= ~(0x10); //earphone_on(FALSE);
		#else
        GPIOCDAT |= (0x10);
		#endif
		GPIOAINEN |= 0x03; 	//
		GPIOADAT |= 0x03; 	//
		SFR_BANK = sfrbak;
	#endif
		
    
    #if(MACHINE_MODEL==0)
    sfrbak = SFR_BANK;
    SFR_BANK = BANK_USB;
    dpdm_status = DPDMCTRL & 0x40;
	SFR_BANK = BANK_PMU;
    dpdm_status |= (SYSTEM_VOL & 0x80);	
	batval = BATADC_DATA & 0x7f;
	SFR_BANK = BANK_LCD;
	sysversion = ChipVersion & 0x0f;
	SFR_BANK = sfrbak;
    //for low power and andfu check
    if ((dpdm_status & 0xc0) == 0)
    #elif((MACHINE_MODEL==1)||(MACHINE_MODEL==2))
    sfrbak=SFR_BANK; 
    SFR_BANK = BANK_PMU;
	if((SYSTEM_VOL & 0x80)||(CHG_CTL & 0x80))	
		keyval=1;
	else	
		keyval=0;
	batval = BATADC_DATA & 0x7f;
	SFR_BANK = BANK_LCD;
	sysversion = ChipVersion & 0x0f;
	SFR_BANK=sfrbak;
	if(keyval)
	#endif
    {
        if (batval < 0x51)
        {
			if (sysversion!= 0) //非A版IC时，进入软开关模式
			{
              mcu_enter_sleep();
			}
			else
			{
			  mcu_enter_losc();
			}
        }
    }

    #if(MACHINE_MODEL==0)
    SFR_BANK = BANK_USB;
    if ((DPDMCTRL & 0x40) != 0x00)
    #elif((MACHINE_MODEL==1)||(MACHINE_MODEL==2))
    sfrbak=SFR_BANK; 
    SFR_BANK = BANK_PMU;
	if((SYSTEM_VOL & 0x80)||(CHG_CTL & 0x80))	
		keyval=1;
	else	
		keyval=0;
	SFR_BANK=sfrbak;
	if(keyval)
	#endif
    {
        //check  adfu enter option
        SFR_BANK = BANK_PMU;
        PMUADC_CTL |= 0xa0;
        Delay_NMS(10);
        //check key value
        loopcnt = 3;
        while (loopcnt!=0)
        {
            keyval = LRADC1_DATA & 0x3f;
            #if(MACHINE_MODEL==2)
            if ((keyval > 15) || (keyval< 5))
            #elif(MACHINE_MODEL==1)
            if ((keyval > 25) || (keyval< 15))
            #endif
            {
                goto show_logo_start;
            }
            Delay_NMS(1);
            loopcnt--;
        }

        /*
        #if(MACHINE_MODEL==2)
        sfrbak = SFR_BANK;
		SFR_BANK = BANK_GPIO;
		GPIOCOUTEN |= 0x10; //Disable DP/DM 音频信号 
		#if(EARPHONE_ON)
		GPIOCDAT &= ~0x10; //clr GPIOC0.0 
		#else
        GPIOCDAT |= 0x10;
		#endif
		GPIOCINEN |= 0x01; 	//
		GPIOCDAT |= 0x01; 	//
		SFR_BANK = sfrbak;
		#endif
		*/
		
        //disable all intrupt
        IE0 &= 0x7f;
        SFR_BANK = BANK_RTC;
        WDCtl |= 0x08;
        // jmp to adfu entry
        sAdfuEnter();
    }

show_logo_start:

    #if((MACHINE_MODEL==1)||(MACHINE_MODEL==2))
    sfrbak = SFR_BANK;
	SFR_BANK = BANK_GPIO;
	GPIOCOUTEN |= 0x10; //Disable DP/DM 音频信号 
	#if(EARPHONE_ON)
	GPIOCDAT &= ~(0x10); //earphone_on(FALSE);
	#else
	GPIOCDAT |= (0x10); //clr GPIOC0.0 
    #endif
	GPIOCINEN |= 0x03; 	//
	GPIOCDAT |= 0x03; 	//
	SFR_BANK = sfrbak;
	#endif
	
    SFR_BANK = sfrbak;
	Hdrv_LightOnOff(FALSE); // 关背光
    Hdrv_LCDInit();
    
    Show_welcome_logo();
	//==================================
    sfrbak = SFR_BANK;
    SFR_BANK = BANK_PMU;
	DIODE_CTL |= 0x80;
	SFR_BANK = sfrbak;
	//===================================
	Delay_NMS(60);
    Hdrv_LightOnOff(TRUE); //开背光

}



//mcu enter low frequent
void mcu_enter_sleep(void)
{
    SFR_BANK = 0x05;
    SYSTEM_CTL_RTCVDD &= 0xfe;
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
        //ClearWatchDog();
		WDCtl |= 0x08;
        if (time == 0)
        {
            break;
        }

    }
}
void SetToLOSC(void)
{
    SFR_BANK = 0x01;
    LOSC1_CTL = (char) (LOSC1_CTL & 0xfe);
    LOSC2_CTL = (char) (LOSC2_CTL & 0xfe);
    MCSR4 = (char) (MCSR4 & 0xfc);
    MCSR4 = (char) (MCSR4 | 0x00);
    _nop_();
    _nop_();
    _nop_();
    //ClearWatchDog();
	WDCtl |= 0x08;
    //   SetAllClock();
    LOSC_delay();
    HOSC_CTL = (char) (HOSC_CTL & 0xf7);
    HOSC_CTL = (char) (HOSC_CTL & 0xfe);
}
void RecoverHOSC(void)
{
    char mcsr_val;
    SFR_BANK = 0x01;
    //    RecoverAllClock();
    HOSC_CTL = (char) (HOSC_CTL | 0x01); //HOSC Enable
    LOSC_delay();
    _nop_();
    _nop_();
    mcsr_val = (char) (MCSR4 & 0xfe);//切换HOSC模式
    mcsr_val = (char) (mcsr_val | 0x02); //MCU clock=24M
    MCSR4 = mcsr_val;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    LOSC1_CTL = (char) (LOSC1_CTL & 0xfe); //disable LOSC
    _nop_();
    _nop_();
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
void mcu_enter_losc(void)
{
    uint8 sfrbank;
	uint8 usbcable_status;
	sfrbank = SFR_BANK;
    SetToLOSC();
	SFR_BANK = BANK_USB;
	while(1)
	{	   
       usbcable_status = DPDMCTRL & 0x40;   
       usbcable_status |= (SYSTEM_VOL & 0x80);
	   if(usbcable_status & 0xc0)
	   {
	   	 break;
	   }
	}
	RecoverHOSC();
	SFR_BANK = sfrbank;
}

//delay for efuse
void delay_func(uint8 cnt)
{
    while (cnt)
    {
        _nop_();
        cnt--;
    }
}
void usb_efuse_done(void)
{
    uint8 sfrbak;
    uint8 bdg_ctl;
    sfrbak = SFR_BANK;
    SFR_BANK = BANK_CMU_RMU;
    MCSR3 |= 0x40;
    delay_func(10);
    MRCR3 |= 0x08;
    delay_func(10);
    SFR_BANK = BANK_USB;
#ifdef USB_EFUSE_INVALID
    USB_Efuse_Ref = 0xda;
#endif
#ifdef USB_EFUSE_VALID
    USB_Efuse_Ref &= 0x3f;
    delay_func(10);
    USB_Efuse_Ref |= 0xc0;
#endif

    SFR_BANK = BANK_PMU;
    bdg_ctl = BDG_CTL;
    BDG_CTL = bdg_ctl;

    delay_func(10);
	BDG_CTL |= 1<<6;
    delay_func(10);
	EFUSE_CTL |= 1<<6;   //CLOSE EFUSE
    SFR_BANK = sfrbak;
}