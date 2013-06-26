#include <actos.h> 

EXTRN ECODE(sfselGetNextGroup??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public fselGetNextGroup??
fselGetNextGroup??:
MOV	WR8,#WORD0(sfselGetNextGroup??)
MOV	R10, #BYTE2(sfselGetNextGroup??) 
LJMP    RSTMIDAPI 

END