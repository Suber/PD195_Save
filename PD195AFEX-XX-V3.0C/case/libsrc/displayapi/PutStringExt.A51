$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************SetLanguage µÄAPIÈë¿Ú******************************/
PUBLIC PutStringExt{RPrefix}  
RSEG   API30

PutStringExt{RPrefix}:
		MOV		WR8, #API_PutStringExt
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI
END

