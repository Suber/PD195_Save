$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************RegionCopy ��API���******************************/
PUBLIC RegionCopy{RPrefix}  
RSEG   API30

RegionCopy{RPrefix}:
		MOV		WR8, #API_RegionCopy
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI
END

