$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************ClearScreen µÄAPIÈë¿Ú******************************/
PUBLIC UpdateScreen{RPrefix}  
RSEG   API30

UpdateScreen{RPrefix}:
		MOV		WR8, #API_UpdateScreen
		PUSH    #WORD0(RSTDisplayAPI)
		EJMP	RSTFARAPI
END

