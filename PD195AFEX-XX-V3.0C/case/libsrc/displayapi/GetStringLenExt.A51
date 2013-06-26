$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************SetLanguage µÄAPIÈë¿Ú******************************/
PUBLIC GetStringLengthExt{RPrefix}  
RSEG   API30

GetStringLengthExt{RPrefix}:
		MOV		WR8, #API_GetStringLengthExt
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI
END

