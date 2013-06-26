#ifndef _IRQ_H
#define _IRQ_H

#include "actapi.h"

#define IRQ_INT0   0				 
#define IRQ_CTC    1			 
#define IRQ_INT1   2	   			 
#define IRQ_RTC	   3			 
#define IRQ_UART   4				 
#define IRQ_I2C	   5		 
#define IRQ_SPI	   6		 
#define IRQ_NMI	   7		 
#define IRQ_USB	   8		 
#define IRQ_DMA5   9			 
#define IRQ_DMA4   10				 
#define IRQ_lCD	   11			 
#define IRQ_ADC	   12			 
#define IRQ_DSP	   13			 
#define IRQ_DMA012 14
#define IRQ_MAX	   14			 
#ifdef __C251__


uint16 IRQ_Intercept(uint16 newisrptr,uint8 IRQType);
void IRQ_Release(uint16 oldisrptr,uint8 IRQType);
//#define enable_interrupt()  EA=1;
//#define disable_interrupt() EA=0;
#ifndef PC
#define enable_interrupt()  {EA=1;}//{(*((char data*)0x007f))--;if(!(*((char data*)0x007f))) EA=1;}
#define disable_interrupt() {EA=0;}//{EA=0; (*((char data*)0x007f))++;}
#else
#define enable_interrupt()         
#define disable_interrupt()      
#endif
#ifndef PC
#define DRAM251B_SetMCU_CLK(sfr_bak,mcsr9_bak){sfr_bak=SFR_BANK;SFR_BANK=BANK_CMU_RMU;mcsr9_bak=MCSR9; MCSR9=(unsigned char)((MCSR9&0xf8)|0x02);SFR_BANK=sfr_bak;}
#define DRAM251B_Restore_CLK(sfr_bak,mcsr9_bak){sfr_bak=SFR_BANK;SFR_BANK=BANK_CMU_RMU;MCSR9=mcsr9_bak;SFR_BANK=sfr_bak;}
#else
#define DRAM251B_SetMCU_CLK(sfr_bak,mcsr9_bak)
#define DRAM251B_Restore_CLK(sfr_bak,mcsr9_bak)
#endif

#ifndef PC
void ClearWatchDog(void);
#define SYS_ClearWatchDog(tmpval) tmpval=SFR_BANK;SFR_BANK=BANK_RTC;WDCtl |=0x08;SFR_BANK=tmpval;
#else
#define ClearWatchDog			/##/
#endif
#endif /*__C251__ */

#ifdef __A251__

#define API_IRQ_Intercept  0x0000+API_IRQ_BaseNoA
#define API_IRQ_Release    0x1000+API_IRQ_BaseNoA


mIRQ_Intercept  macro
          MOV      WR8,#API_IRQ_Intercept
		  LCALL	 RSTBankAPI
endm

mIRQ_Release    macro
          MOV      WR8,#API_IRQ_Release
		  LCALL	 RSTBankAPI
          endm

mCLRWD	  macro
		  PUSH SFR_BANK
		  MOV  SFR_BANK,#BANK_RTC
		  SETB WDCtl.3
		  POP  SFR_BANK
		  endm
		  			
mDI       macro
		  CLR	EA
		  INC	0x7F
          endm

mEI       macro
          local  _interrupt_disabled
		  DEC	0x7F
 		  PUSH  R11
		  MOV   A,0x7F
		  JNZ   _interrupt_disabled	
		  SETB  EA
_interrupt_disabled:
		  POP R11
        endm       

#endif /* __A251__ */

#endif /*_IRQ_H*/