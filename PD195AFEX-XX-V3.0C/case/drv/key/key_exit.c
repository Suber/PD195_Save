#include "actos.h"
#include "key.h"
#include "time.h"

extern void ctc_isr(void);
extern void cld_overflow_isr(void);

extern uint8 sysverflag;

#pragma renamecode(?PR?BAC_IE1)

void key_exit(void)
{
		/*加速度传感器，RCODE空间不够，故直接用IIC写来此处*/
		sKY_GsensorOff();   //pedometer
		
	TM_KillTimer(0x02); //关闭RTC
    IE0 &= ~(1 << 1); //disable IE.1
    IRQ_Release((uint16) ctc_isr, IRQ_CTC);

    if (sysverflag != 0)
    {
        IRQ_Release((uint16) cld_overflow_isr, IRQ_ADC);
    }
}
