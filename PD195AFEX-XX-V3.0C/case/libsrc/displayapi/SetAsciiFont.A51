$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************SetAsciiFont ��API���******************************/
PUBLIC SetAsciiFont{RPrefix}  
RSEG   API30

SetAsciiFont{RPrefix}:
		MOV		WR8, #API_SetAsciiFont
		PUSH    #WORD0(RSTDisplayAPI)
        EJMP    RSTFARAPI
END

