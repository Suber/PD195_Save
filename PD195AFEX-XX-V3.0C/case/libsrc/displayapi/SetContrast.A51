$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************SetContrast µÄAPIÈë¿Ú******************************/
PUBLIC SetContrast{RPrefix}  
RSEG   API30

SetContrast{RPrefix}:
		MOV		WR8, #API_SetContrast
		PUSH    #WORD0(RSTDisplayAPI)
		EJMP	RSTFARAPI
END

