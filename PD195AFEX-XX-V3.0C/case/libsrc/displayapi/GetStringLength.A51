$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************GetStringLength µÄAPIÈë¿Ú******************************/
PUBLIC GetStringLength{RPrefix}  
RSEG   API30

GetStringLength{RPrefix}:
		MOV		WR8, #API_GetStringLength
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI
END

