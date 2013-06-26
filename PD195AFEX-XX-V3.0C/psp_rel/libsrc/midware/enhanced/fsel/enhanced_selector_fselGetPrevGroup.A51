#include <actos.h> 

EXTRN ECODE(sfselGetPrevGroup??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public fselGetPrevGroup??
fselGetPrevGroup??:
MOV	WR8,#WORD0(sfselGetPrevGroup??)
MOV	R10, #BYTE2(sfselGetPrevGroup??) 
LJMP    RSTMIDAPI 

END