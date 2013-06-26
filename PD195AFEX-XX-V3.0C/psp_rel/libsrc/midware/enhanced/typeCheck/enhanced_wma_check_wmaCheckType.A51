#include <actos.h> 

EXTRN ECODE(swmaCheckType??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public wmaCheckType??
wmaCheckType??:
MOV	WR8,#WORD0(swmaCheckType??)
MOV	R10, #BYTE2(swmaCheckType??) 
LJMP    RSTMIDAPI 

END