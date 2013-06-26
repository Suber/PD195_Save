#include <actos.h> 

EXTRN ECODE(sfselGetErrID??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public fselGetErrID??
fselGetErrID??:
MOV	WR8,#WORD0(sfselGetErrID??)
MOV	R10, #BYTE2(sfselGetErrID??) 
LJMP    RSTMIDAPI 

END