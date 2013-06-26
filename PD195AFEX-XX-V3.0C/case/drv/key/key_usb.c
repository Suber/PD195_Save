#include "actos.h"
#include "key.h"
#include "ap_common.h"
#pragma ROM(medium)
extern uint8 usbplugflag;
extern uint8 low_battery_value;
extern uint8 batDetectCnt;
extern uint8 sysverflag;

#pragma renamecode(?PR?KY_USB)
uint8 data usb_plugin _at_ 0x38;
uint8 data usb_unstickcount _at_ 0x39;
uint8 data usb_stickcount _at_ 0x3a;

void LowBatDetect(void);

void sKY_CheckUsbStatus(void)
{
    uint8 dpdm_status;
    uint8 sys_vol;
    uint8 usb_plugin_new;
    uint8 sfr_bak = SFR_BANK;

    SFR_BANK = BANK_USB;
    dpdm_status = DPDMCTRL & 0x40;
    SFR_BANK = BANK_PMU;
    //#if(MACHINE_MODEL==1)
    //sys_vol = SYSTEM_VOL & 0x80;
    //usb_plugin_new = dpdm_status | sys_vol;
    //#elif(MACHINE_MODEL==2)
    sys_vol = (SYSTEM_VOL & 0x80) | (CHG_CTL & 0x80);
    usb_plugin_new = sys_vol;
    //#endif

    if (usb_plugin_new != 0)
    {
        usb_plugin_new = 1;
    }
    else
    {
        batDetectCnt++;
        if (batDetectCnt >= 10)
        {
            batDetectCnt = 0;
            LowBatDetect(); //调用低电检测
        }
    }

    if ((usbplugflag == 1) && (usb_plugin & usb_plugin_new))
    {
        usbplugflag = MSG_USB_STICK_FLAG;
    }

    if (usb_plugin_new != usb_plugin)
    {
        if (usb_plugin_new != 0)
        {
            usb_stickcount++;
            if (usb_stickcount > 3)
            {
                usb_plugin = usb_plugin_new;
                usb_unstickcount = 0;
                usbplugflag = MSG_USB_STICK_FLAG;//PutSysMsg(MSG_USB_STICK);
            }
        }
        else
        {
            usb_unstickcount++;
            if (usb_unstickcount > 3)
            {
                usb_plugin = usb_plugin_new;
                usb_unstickcount = 0;
                PutSysMsg(MSG_USB_UNSTICK);
            }
        }
    }

    SFR_BANK = sfr_bak;
}

void LowBatDetect(void)
{
    uint8 sfr_bak;
    uint8 bat_vol;

    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_PMU;
    bat_vol = BATADC_DATA & 0x7f;

    if (bat_vol <= low_battery_value)
    {
        PutSysMsg(MSG_LOW_POWER);
    }
    SFR_BANK = sfr_bak;
}

void cld_overflow_isr(void)
{
    uint8 sfr_bak;

    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_AUIP;

    if ((CLD_CTL1 >> 2) & 0x01)
    {
        //短路
        DisableCLD();
    }

    if ((CLD_CTL1 >> 3) & 0x01)
    {
        //过温
        DisableCLD();
    }

    SFR_BANK = sfr_bak;
}

