$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************StandbyScreen ��API���******************************/
PUBLIC StandbyScreen{RPrefix}  
RSEG   API30

StandbyScreen{RPrefix}:
		MOV		WR8, #API_StandbyScreen
		PUSH    #WORD0(RSTDisplayAPI)
		EJMP	RSTFARAPI
END

