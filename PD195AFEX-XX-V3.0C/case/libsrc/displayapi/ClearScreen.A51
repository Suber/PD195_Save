$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************ClearScreen µÄAPIÈë¿Ú******************************/
PUBLIC ClearScreen{RPrefix}  
RSEG   API30

ClearScreen{RPrefix}:
		MOV		WR8, #API_ClearScreen
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI
END

