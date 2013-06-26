#include <actos.h> 

EXTRN ECODE(sfselGetByNO??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public fselGetByNO??
fselGetByNO??:
MOV	WR8,#WORD0(sfselGetByNO??)
MOV	R10, #BYTE2(sfselGetByNO??) 
LJMP    RSTMIDAPI 

END