#include <actos.h> 

EXTRN ECODE(sfselInit??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public fselInit??
fselInit??:
MOV	WR8,#WORD0(sfselInit??)
MOV	R10, #BYTE2(sfselInit??) 
LJMP    RSTMIDAPI 

END