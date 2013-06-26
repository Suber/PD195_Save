#include <actos.h> 

EXTRN ECODE(sfselGetCurFNameL??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public fselGetCurFNameL??
fselGetCurFNameL??:
MOV	WR8,#WORD0(sfselGetCurFNameL??)
MOV	R10, #BYTE2(sfselGetCurFNameL??) 
LJMP    RSTMIDAPI 

END