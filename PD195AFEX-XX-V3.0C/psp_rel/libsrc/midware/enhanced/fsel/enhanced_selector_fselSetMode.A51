#include <actos.h> 

EXTRN ECODE(sfselSetMode??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public fselSetMode??
fselSetMode??:
MOV	WR8,#WORD0(sfselSetMode??)
MOV	R10, #BYTE2(sfselSetMode??) 
LJMP    RSTMIDAPI 

END