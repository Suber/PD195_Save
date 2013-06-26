#include "actos.h"
#include "key.h"
#include "time.h"


//#define IO_DRIVE_HIGH 	
//#define IO_DRIVE_MID 	
#define IO_DRIVE_LOW  

extern void _nop_(void);
extern void key_scan(void);
extern void ctc_isr(void);
extern void cld_overflow_isr(void);

void delay(void);
extern uint8 earphoneinflag;

extern uint8 sysverflag;

extern uint8 gsensor_workflag;
extern uint8 gsensor_workflag_swim;

#pragma renamecode(?PR?BAC_IE0)

bool key_init(void)
{
    uint8 sfr_bak;
	
	/**************flash driver ability modify************/
	SFR_BANK = 0x06;                 //切到bank06

#ifdef IO_DRIVE_HIGH
	PADDRV0 |= 1;
	PADDRV0 &= ~(1<<1);
	PADDRV0 |= 1<<2;
	PADDRV0 &= ~(1<<4);
	PADDRV0 |= 1<<5;

	PADDRV1 &= ~(1<<2);
	PADDRV1 |= 1<<3;
	PADDRV1 &= ~(1<<4);
	PADDRV1 |= 1<<5;
#endif

#ifdef IO_DRIVE_MID
	PADDRV0 |= 1;
	PADDRV0 |= 1<<1;
	PADDRV0 &= ~(1<<2);
	PADDRV0 |= 1<<4;
	PADDRV0 &= ~(1<<5);

	PADDRV1 |= 1<<2;
	PADDRV1 &= ~(1<<3);
	PADDRV1 |= 1<<4;
	PADDRV1 &= ~(1<<5);
#endif

#ifdef IO_DRIVE_LOW
   	PADDRV0 &= ~1;
	PADDRV0 &= ~(1<<1);
	PADDRV0 &= ~(1<<2);
	PADDRV0 &= ~(1<<4);
	PADDRV0 &= ~(1<<5);

	PADDRV1 &= ~(1<<2);
	PADDRV1 &= ~(1<<3);
	PADDRV1 &= ~(1<<4);
	PADDRV1 &= ~(1<<5);
#endif
    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_PMU;
    PMUADC_CTL |= 0xa0; //enable LRADC1,ADC FREQUENCY 128Hz
    _nop_();
    delay(); //enable LRADC1后需要等待最长10ms才能读出正确的数据，否则可能是全0
    SYSTEM_CTL_RTCVDD &= 0x3f; //设置play按键时间=2S

    TM_SetTimer((uint16) key_scan, 0x02); //设置RTC中断函数入口

    //ctc set
    SFR_BANK = BANK_RTC;
    CTCCtl |= 1 << 6; //ctc prescale set
    CTCCtl &= ~(1 << 5);
    CTCCtl &= ~(1 << 4);
    CTCCtl |= 1 << 3;

    CTCCNTH = 0x5f; //ctc counter set
    CTCCNTL = 0xff;

    IRQ_Intercept((uint16) ctc_isr, IRQ_CTC); //设置CTC中断函数入口
    CTCCtl |= 1 << 7; //enable ctc
    IE0 |= 1 << 1; //enable IE.1

    //demo直驱与CLD的切换
    SFR_BANK = BANK_GPIO;
    GPIOAOUTEN &= ~1;
    GPIOAINEN |= 1;
    earphoneinflag = 0;

	/*加速度传感器，RCODE空间不够，故直接用IIC写来此处*/
	sKY_GsensorCheck();   //pedometer
	sKY_GsensorOff();
	gsensor_workflag = 0;	//扫描加速度传感
	gsensor_workflag_swim = 0;
	

    //classD过流保护
    if (sysverflag != 0)
    {
        //使能classCTL的过流保护
        SFR_BANK = BANK_AUIP;
        CLD_CTL3 |= 1 << 7;
        IRQ_Intercept((uint16) cld_overflow_isr, IRQ_ADC);
    }

    SFR_BANK = sfr_bak;
    return 1;
}


void delay(void)
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
