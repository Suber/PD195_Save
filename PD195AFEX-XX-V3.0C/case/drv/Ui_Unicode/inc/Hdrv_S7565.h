#ifndef _HDRV_S7565_H
#define _HDRV_S7565_H

#include "sysdef.h"
#include "Drv_S7565.h"

#ifdef __C251__
#include "ap_cfg.h"

extern void _nop_(void);

extern void Hdrv_SetWindow(region_t *winreg);

extern void Hdrv_BuffDataTrans(uint8* pbuf, uint16 len);

extern void Hdrv_HardInit(void);
extern void Hdrv_LCDInit(void);
extern void Hdrv_LcdReset(void);
extern void Hdrv_LcdInitBuf(void);

extern void StoreCE(void);
extern void RestoreCE(void);
extern void Write_Command(uint8 cmd);

extern void Write_Data(uint8* mydata,uint16 len);

/*****************Ä£ÄâÆ÷Ë¢ÆÁ******************/
/**********************************************/

/*************EXTÑ¡Ôñ******************/
extern void Hdrv_SetMfpToEmif(void);
//extern void Hdrv_SetMfpToLcd(void);
/**********************************************/

extern void Hdrv_SetContrast(uint8 ContrastValue);

//extern void Hdrv_StandbyScreen(bool bStandby);
extern void Hdrv_LightOnOff(bool OnOff);
void Hdrv_UpdateScreen(region_t *updateregion);
extern void Delay_NMS(uint8 delay_ms);
extern void Delay_200us(void);
extern void Delay_ms(uint8 delay_ms);
#endif

#ifdef __A251__

#endif  /*end of A251*/
#endif

