$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************LightOnOff µÄAPIÈë¿Ú******************************/
PUBLIC LightOnOff{RPrefix}  
RSEG   API30

LightOnOff{RPrefix}:
	MOV		WR8, #API_BackLightOnOff
	PUSH    #WORD0(RSTDisplayAPI)
	EJMP	RSTFARAPI
END

