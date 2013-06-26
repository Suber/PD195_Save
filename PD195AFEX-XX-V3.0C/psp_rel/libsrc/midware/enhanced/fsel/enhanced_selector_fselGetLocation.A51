#include <actos.h> 

EXTRN ECODE(sfselGetLocation??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public fselGetLocation??
fselGetLocation??:
MOV	WR8,#WORD0(sfselGetLocation??)
MOV	R10, #BYTE2(sfselGetLocation??) 
LJMP    RSTMIDAPI 

END