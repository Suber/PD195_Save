

//******************************************************************************
//*     show logo api
//******************************************************************************   
#define		showLogoFuncEntry	0x0860
 
public  sGetLogoData?_ 
public  sGetBuffInfo?_
public  sGetLogoSize?_
public  sAdfuEnter?_
//public  mcu_delay?_

;------------------------------------------------------------------------------
; Put segment and variable declarations here.
;------------------------------------------------------------------------------	
CODEDISPSTART	SEGMENT  ECODE 	 
//CODEDISPSTART	SEGMENT  CODE 	 
        rseg    CODEDISPSTART


sGetLogoData?_:
        
		MOV		R10,#0
		CALL	showLogoFuncEntry
		RET	  

sGetBuffInfo?_:
        
		MOV		R10,#1
		CALL	showLogoFuncEntry
		RET	  

		
sGetLogoSize?_:
        
		MOV		R10,#2
		CALL	showLogoFuncEntry
		RET	
		
sAdfuEnter?_:
       	EJMP	0xfe0180		  // jmp to adfu entry
	    JMP $   
        RET 
;----------------------------delay subroutine------------------------------
/*mcu_delay?_:							;delay subroutine
	    MOV   R2,#0ffh	
loop:	
        NOP
        NOP 
        NOP
        NOP						
    	NOP
    	NOP
    	DJNZ R2,loop
	    RET*/
end