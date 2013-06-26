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
* \file     usbdisk_.c
* \brief    
* \author   wanghaijing
* \par      GENERAL DESCRIPTION:
*               这里对文件进行描述
* \par      EXTERNALIZED FUNCTIONS:
*               这里描述调用到外面的模块
*					/********************************************************************************
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
* \file     usbdisk_enter2exit.c
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
* \date  2010-3-27
*******************************************************************************/
#include "Udisk.h"
extern uint8 UdiskStatus;
extern comval_t g_comval; //全局变量数据结构 
extern uint8 HoldDispState;

extern void ReadBattPicInRam(void);
//extern void ui_show_batt(uint8 value);
void FlashFrequenceSet(uint8 Frequence);//EMI
void  FlashPadDrvSet(uint8 PadDrv);
#pragma name(AP_ENTEREXIT)
bool UDiskInitAll(int para)
{      
	bool ret;     
//    /*低电模式下进行充电*/
//    if(GetBattery() < 0x13)
//    {
//       sKY_ChargeSet(1);    
//       sKY_ChargeGetandSet();    
//    } 

	//将ram的clk均切换到mcu clk,避免应用退出到u盘,clk不对导致使用问题.
	RamToMcu();	

	//安装ud驱动
	res_fp = ResOpen("ui30.res");

	ResShowPic(IM_USB_BG, 0, 0); //显示空闲画面	

	//电池图片缓存在内存中,避免连写写flash时有去频繁读flash，影响写速度. add by wanghaijing,2010-5-27
	ReadBattPicInRam();
	
	if (DRV_DetectUD(0x00) == 0xff)
    {
		return 0;
    }
	else
    {
		DRV_ActiveUD(0x00, MODE_FAST); //快速模式ud驱动	
    }
    #ifdef USB_EMI
    FlashFrequenceSet(16);//EMI
    FlashPadDrvSet(0);   //EMI
    #endif
    
	//read comval
	VMRead(&g_comval, VM_SYSTEM, sizeof(g_comval)); //读VRAM
	//check if valid
	if (g_comval.magic != MAGIC_COMVAL) //取联机模式设定
	{
		GetDefaultComval(&g_comval);
	}

	//==================================================================================
	//**********************以下函数调用次序不能改动***************************************
	//==================================================================================
	UDiskSetDiskNum(g_comval.OnlineMode);       //设置联机模式

	AlwaysShowCardDisk(g_comval.SuppCard);      //设置是否显示多盘符  //reagan modify 2009-1-6 13:39	
								
	//U盘初始终化,输入参数:callback函数，属性...  
	UDiskInit(0,g_comval.udisk_setting);
	
	if (sKY_HoldCheck()) //HOLD表示需要写保护
	{		
        int i,j;
        for(i=0;i<20;i++)
        {
            if(sKY_HoldCheck() == 0)
            {
               break;
            }
            /*delay*/
            for(j=0;j++;j<1000)
            {
                ClearWatchDog();
            }
        }
        if(i==20) 
        {
		UDiskSetWP('C');
		HoldDispState = 1;
		//ResShowPic(UDLOCK_S, 115, 16);
//		ResShowPic(UDLOCK_S, 65, 49);
		}
	}
			  
    ret = UDiskRun(para); //启动USB引擎
    return ret;   
}
extern void          _nop_     (void);
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
	/*spi norbase在退出u盘后,检测u盘插入时DPDM的状态时,需要有usbclk*/
	//MCSR3 &= 0xbf;
	MCSR5 &= 0xe7;

    SFR_BANK = sfr_bak;
}

int UdiskExitAll(void)
{
	int ret;
	UDiskExit();
    sKY_ChargeGetandSet(); /*再次检测充电，拔线时处理充电流程*/	                            
    ResClose(res_fp); 	                      
   
    DRV_ActiveUD(0x00, MODE_SLOW);
	/*
	//按Mode退出  
    if(UdiskStatus == 0x00)                       
    {  
        #if(MACHINE_MODEL==1)
        ret = RESULT_MAIN; 
        #else
        ret = 0;    
        #endif
    }
	//升级退出 
    else 
    */
    if(UdiskStatus == 0x50) 
    {                
        ret = RESULT_UPGRADE;
    }
    else
    {    
	    //拔线,弹出  
        //ret = 0;    
        ret = RESULT_STANDBY;  
    }
	power_control();
	return ret;
}


void RamToMcu(void)
{
    uint8 sfr_bak;
    sfr_bak = SFR_BANK;

    SFR_BANK = BANK_CMU_RMU;
    MCSR11 = (uint8) (MCSR11 & 0x04);
    MCSR5 = (uint8) ((MCSR5 & 0xe1) | 0x34); //JRAM3Clk:MCU clk; PCMRAM:MCU clk;
    MCSR6 = (uint8) ((MCSR6 & 0xf0) | 0x0a); //MURAM2:MCU clk; JRAM1Clk:MCU clk;
	MCSR7 = 0x2a; //B1 Clk:MCU clk; MURAM1HClk:MCU clk; JRAM2Clk:MCU clk;
    MCSR8 = (uint8) ((MCSR8 & 0x03) | 0x48); //MURAM1MClk:MCU clk; MURAM1LClk:MCU clk;
	MCSR9 = 0x4a; //URAMClk:MCU clk; B2 Clk:MCU clk; DRAM251BClk:MCU clk; 
	MCSR11 = MCSR11 | 0xfb;

    SFR_BANK = sfr_bak;

}	                 
#ifdef USB_EMI
void FlashFrequenceSet(uint8 Frequence)   //EMI
{
    uint8 pll,sfr_bak;
    if(Frequence <= 16)
    {
        sfr_bak =  SFR_BANK;
        SFR_BANK = BANK_CMU_RMU;
        pll = MCSR1;
        pll &= 0xfc;
        pll |= 0x02;
        MCSR1 = pll;
        SFR_BANK = sfr_bak;
        uprintf("Frequence: %x\n",Frequence);
        sfr_bak =  SFR_BANK;
        SFR_BANK = 8;
        pll = NAND_CTL;
        if(Frequence == 16)
        {
            pll &= 0xf3;
        }
        else
        {
            pll &= 0xf3;
            pll |= 0x08;
        }
        NAND_CTL = pll;
        SFR_BANK = sfr_bak;
    }
    else
    {
        SetPLL(Frequence/2);
        sfr_bak =  SFR_BANK;
        SFR_BANK = BANK_CMU_RMU;
        pll = MCUPLL_CTL;
        SFR_BANK = sfr_bak;
        pll >>= 1;
        pll = pll & 0x1f;
        pll <<= 2;
        uprintf("pll %x\n",pll);
        sfr_bak =  SFR_BANK;
        SFR_BANK = 8;
        pll = NAND_CTL;
        pll &= 0xf3;
        NAND_CTL = pll;
        SFR_BANK = sfr_bak;
    }
}
void  FlashPadDrvSet(uint8 PadDrv)
{
    uint8 padrv0,padrv1;
    uint8 sfr_bak = SFR_BANK;
    SFR_BANK = 6;
    padrv0 = PADDRV0;
    padrv1 = PADDRV1;
    padrv0 &= 0xc8;
    padrv1 &= 0xc3;
    if(PadDrv == 0)
    {
        padrv0 |= 0;
        padrv1 |= 0;
    }
    else if (PadDrv == 1)
    {
        padrv0 |= 0x13;
        padrv1 |= 0x14;
    }
    else
    {
        padrv0 |= 0x25;
        padrv1 |= 0x28;
    }
    PADDRV0 = padrv0;
    PADDRV1 = padrv1;
    SFR_BANK = sfr_bak;
}
#endif
