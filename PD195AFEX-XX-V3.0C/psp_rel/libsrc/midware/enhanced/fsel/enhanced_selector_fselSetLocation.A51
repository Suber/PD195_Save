#include <actos.h> 

EXTRN ECODE(sfselSetLocation??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public fselSetLocation??
fselSetLocation??:
MOV	WR8,#WORD0(sfselSetLocation??)
MOV	R10, #BYTE2(sfselSetLocation??) 
LJMP    RSTMIDAPI 

END