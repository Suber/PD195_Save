$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************PutS ��API���******************************/
PUBLIC PutS{RPrefix}  
RSEG   API30

PutS{RPrefix}:
    MOV		WR8, #API_PutS
	PUSH    #WORD0(RSTBankAPI)
	EJMP	RSTFARAPI
END

