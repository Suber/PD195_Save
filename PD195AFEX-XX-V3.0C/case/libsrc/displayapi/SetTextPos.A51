$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************SetTextPos ��API���******************************/
PUBLIC SetTextPos{RPrefix}  
RSEG   API30

SetTextPos{RPrefix}:
        MOV		WR8, #API_SetTextPos
		PUSH    #WORD0(RSTBankAPI)
        EJMP    RSTFARAPI
END

