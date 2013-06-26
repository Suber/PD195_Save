$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************PutSDImage µÄAPIÈë¿Ú******************************/
PUBLIC PutSDImage{RPrefix}  
RSEG   API30

PutSDImage{RPrefix}:
		MOV		WR8, #API_PutSDImage
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI
END

