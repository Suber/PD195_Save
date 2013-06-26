$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************PutUnS µÄAPIÈë¿Ú******************************/
PUBLIC PutUnS{RPrefix}  
RSEG   API30

PutUnS{RPrefix}:
	MOV		WR8, #API_PutUnS
	PUSH    #WORD0(RSTBankAPI)
	EJMP	RSTFARAPI
END

