$include (actos.inc)
#include "display.h"

API30 SEGMENT 'ECODE_APISEG'

/**************************SetLanguage ��API���******************************/
PUBLIC SetLanguage{RPrefix}  
RSEG   API30

SetLanguage{RPrefix}:
		MOV		WR8, #API_LanguageSet
		PUSH    #WORD0(RSTBankAPI)
		EJMP	RSTFARAPI
END

