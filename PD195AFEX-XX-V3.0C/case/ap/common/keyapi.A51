$include (actos.inc)
#include "key.h"

/************************* Segment Define ********************/
 		API30	SEGMENT	 'ECODE_APISEG'
/************************* public info ***********************/ 
//bank API 
public sKY_HoldCheck{RPrefix}
public sKY_ChargeSet{RPrefix}
public sKY_Beep{RPrefix}
public sKY_ChargeGetandSet{RPrefix}
public sKY_GetKeyTabAddress{RPrefix}
public sKY_GetEarState{RPrefix}
public sKY_SetGsensor{RPrefix}
public sKY_BankGsensorGetStep{RPrefix}
public sKY_BankGsensorGetADC{RPrefix}
/************************** EXTRN info ***********************/  
/************************* code begin ***********************/
rseg API30
//BANK APIÌø×ª 
sKY_HoldCheck{RPrefix}:
		MOV		WR8, #API_HoldCheck
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI
sKY_ChargeSet{RPrefix}:
		MOV		WR8, #API_ChargeSet
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI
sKY_Beep{RPrefix}:   
		MOV		WR8, #API_Beep
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI
sKY_ChargeGetandSet{RPrefix}:
	  	MOV		WR8, #API_ChargeGetandSet
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI	
sKY_GetKeyTabAddress{RPrefix}:  
	  	MOV		WR8, #API_GetKeyTabAddress
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI  
sKY_GetEarState{RPrefix}:  
	  	MOV		WR8, #API_GetEarState
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI  
sKY_SetGsensor{RPrefix}:
		MOV		WR8, #API_SetGsensor
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI
sKY_BankGsensorGetStep{RPrefix}:
		MOV		WR8, #API_BankGsensorGetStep
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI	
sKY_BankGsensorGetADC{RPrefix}:
		MOV		WR8, #API_BankGsensorGetADC
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI	
END