#include <actos.h> 

EXTRN ECODE(sfselGetPrevFile??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public fselGetPrevFile??
fselGetPrevFile??:
MOV	WR8,#WORD0(sfselGetPrevFile??)
MOV	R10, #BYTE2(sfselGetPrevFile??) 
LJMP    RSTMIDAPI 

END