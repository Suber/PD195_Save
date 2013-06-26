$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************GetTextPos µÄAPIÈë¿Ú******************************/
PUBLIC GetTextPos{RPrefix}  
RSEG   API30

GetTextPos{RPrefix}:
		MOV		WR8, #API_GetTextPos
		PUSH    #WORD0(RSTBankAPI)
        EJMP    RSTFARAPI
END

