$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************GetFontPoint µÄAPIÈë¿Ú******************************/
PUBLIC GetFontPoint{RPrefix}  
RSEG   API30

GetFontPoint{RPrefix}:
	    MOV		WR8, #API_GetFontPoint
		PUSH    #WORD0(RSTDisplayAPI)
        EJMP    RSTFARAPI
END

