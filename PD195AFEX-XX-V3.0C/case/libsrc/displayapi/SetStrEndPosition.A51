$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************SetStrEndPosition µÄAPIÈë¿Ú******************************/
PUBLIC SetStrEndPosition{RPrefix}  
RSEG   API30

SetStrEndPosition{RPrefix}:
		MOV		WR8, #API_SetStrEndPosition
		PUSH    #WORD0(RSTDisplayAPI)
        EJMP    RSTFARAPI
END

