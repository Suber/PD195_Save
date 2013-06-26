$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************CharOutRect µÄAPIÈë¿Ú******************************/
PUBLIC CharOutRect{RPrefix}  
RSEG   API30

CharOutRect{RPrefix}:
		MOV		WR8, #API_CharOutRect
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI
END

