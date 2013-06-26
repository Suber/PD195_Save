#include <actos.h> 

EXTRN ECODE(sfselGetNo??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public fselGetNo??
fselGetNo??:
MOV	WR8,#WORD0(sfselGetNo??)
MOV	R10, #BYTE2(sfselGetNo??) 
LJMP    RSTMIDAPI 

END