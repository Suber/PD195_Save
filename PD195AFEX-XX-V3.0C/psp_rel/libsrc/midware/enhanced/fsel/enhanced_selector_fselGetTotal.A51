#include <actos.h> 

EXTRN ECODE(sfselGetTotal??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public fselGetTotal??
fselGetTotal??:
MOV	WR8,#WORD0(sfselGetTotal??)
MOV	R10, #BYTE2(sfselGetTotal??) 
LJMP    RSTMIDAPI 

END