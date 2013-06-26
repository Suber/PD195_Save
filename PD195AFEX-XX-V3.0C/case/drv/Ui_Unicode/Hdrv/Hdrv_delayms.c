#include "actos.h"
#include "ak211x.h"
#include "Hdrv_S7565.h"
#pragma renamecode(DELAY_MS)
void Delay_ms(uint8 delay_ms)
{
    uint8 i, j;
    while (delay_ms != 0)
    {
        for (i = 0; i < 10; i++)
        {
            for (j = 0; j < 255; j++)
            {
                _nop_();
            }
        }
        delay_ms--;
    }
}

//24Mhz 20msºÁÃëµÄÕûÊý±¶
void Delay_NMS(uint8 delay_ms)
{
    Delay_ms(delay_ms);
}
void Delay_200us(void)
{
    //delay_ms = delay_ms;
    uint8 i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 255; j++)
        {
            _nop_();
        }
    }
}
