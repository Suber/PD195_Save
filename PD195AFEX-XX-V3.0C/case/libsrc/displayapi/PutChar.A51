$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************PutChar µÄAPIÈë¿Ú******************************/
PUBLIC PutChar{RPrefix}  
RSEG   API30

PutChar{RPrefix}:
	MOV		WR8, #API_PutChar
	PUSH    #WORD0(RSTBankAPI)
	EJMP	RSTFARAPI
END

