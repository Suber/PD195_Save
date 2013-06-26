#include "ak211x.h"

KCFG_C SEGMENT ECODE
public IsNotPowerDown??

rseg KCFG_C
IsNotPowerDown??:     			//返回  0：掉电   非0：没掉电 
	PUSH  	SFR_BANK
	MOV	 	SFR_BANK,#BANK_RTC
	MOV	  	R11,#01H
 	JB     RTC_CTL0.7,NotPowerDown
	MOV		R11,#00H 
NotPowerDown:
	POP		SFR_BANK
    eret                          
    end  
