#include <actos.h> 

EXTRN ECODE(swavCheckType??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public wavCheckType??
wavCheckType??:
MOV	WR8,#WORD0(swavCheckType??)
MOV	R10, #BYTE2(swavCheckType??) 
LJMP    RSTMIDAPI 

END