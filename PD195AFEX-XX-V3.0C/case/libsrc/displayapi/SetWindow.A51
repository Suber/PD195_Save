$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************SetWindow µÄAPIÈë¿Ú******************************/
PUBLIC SetWindow{RPrefix}  
RSEG   API30

SetWindow{RPrefix}:
		MOV		WR8, #API_SetWindow
		PUSH    #WORD0(RSTDisplayAPI)
		EJMP	RSTFARAPI
END

