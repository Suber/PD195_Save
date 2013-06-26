
#include <actos.h> 

EXTRN ECODE(sfselGetNextFile??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public fselGetNextFile??
fselGetNextFile??:
MOV	WR8,#WORD0(sfselGetNextFile??)
MOV	R10, #BYTE2(sfselGetNextFile??) 
LJMP    RSTMIDAPI 

END