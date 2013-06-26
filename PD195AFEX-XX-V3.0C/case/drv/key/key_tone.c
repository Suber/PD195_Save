#include "actos.h"
#include "irq.h"
#include "key.h"

#pragma renamecode(?PR?BAC_KT)
void Delay10ms(void); //24Mhz，延时500ms
extern void _nop_(void);


bool sKY_Beep(uint8 value)
{
    uint8 sfr_bak;
    uint8 paVolBak;
    uint8 i;

    sfr_bak = SFR_BANK;
    paVolBak = GetPAVolume();
    if (value < 6)
    {
        value = 6;
    }
    SetPAVolume(value);

    SFR_BANK = BANK_AUIP;
    KT_VAL = 0xf; //set kt vol
    KT_CTL = 0x11; //set kt period& sq wave num
    KT_CTL |= 1 << 7; //ENABLE KT

    while ((KT_CTL & 0x80))
    {
        //ClearWatchDog(); //check kt finish or not
        for (i = 0; i < 20; i++)
        {
            Delay10ms();
            if ((KT_CTL & 0x80) == 0) //等到KT FINISH
            {
                goto exit;
                //直接退出
            }
        }
        //200ms延迟后，仍等不到kt finish，直接退出
        break; //退出
    }

    exit: SetPAVolume(paVolBak);
    SFR_BANK = sfr_bak;
    return TRUE;
}


void Delay10ms(void)
{
    uint8 i, j;
    for (i = 0; i < 255; i++)
    {
        for (j = 0; j < 255; j++)
        {
            _nop_();
            _nop_();
        }
    }
}
