$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************FillRect µÄAPIÈë¿Ú******************************/
PUBLIC FillRect{RPrefix}
RSEG   API30

FillRect{RPrefix}:
		MOV		WR8, #API_FillRect
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI
END

