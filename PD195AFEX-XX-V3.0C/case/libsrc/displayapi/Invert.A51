$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************Invert µÄAPIÈë¿Ú******************************/
PUBLIC InvertRegion{RPrefix}  
RSEG   API30

InvertRegion{RPrefix}:
		MOV		WR8, #API_InvertRegion
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI
END

