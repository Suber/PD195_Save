$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************FontCodeMBToUni ��API���******************************/
PUBLIC FontCodeMBToUni{RPrefix}  
RSEG   API30

FontCodeMBToUni{RPrefix}:
		MOV		WR8, #API_FontCodeMBToUni
		PUSH    #WORD0(RSTDisplayAPI)
		EJMP	RSTFARAPI
END

