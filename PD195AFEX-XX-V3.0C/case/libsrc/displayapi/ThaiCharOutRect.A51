$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************SetLanguage µÄAPIÈë¿Ú******************************/
PUBLIC ThaiCharOutRect{RPrefix}  
RSEG   API30

ThaiCharOutRect{RPrefix}:
		MOV		WR8, #API_ThaiCharOutRect
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI
END

