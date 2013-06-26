#include <actos.h> 

EXTRN ECODE(sfselGetCurGroupL??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public fselGetCurGroupL??
fselGetCurGroupL??:
MOV	WR8,#WORD0(sfselGetCurGroupL??)
MOV	R10, #BYTE2(sfselGetCurGroupL??) 
LJMP    RSTMIDAPI 

END