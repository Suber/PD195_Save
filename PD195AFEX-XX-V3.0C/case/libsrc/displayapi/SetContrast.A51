$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************SetContrast ��API���******************************/
PUBLIC SetContrast{RPrefix}  
RSEG   API30

SetContrast{RPrefix}:
		MOV		WR8, #API_SetContrast
		PUSH    #WORD0(RSTDisplayAPI)
		EJMP	RSTFARAPI
END

