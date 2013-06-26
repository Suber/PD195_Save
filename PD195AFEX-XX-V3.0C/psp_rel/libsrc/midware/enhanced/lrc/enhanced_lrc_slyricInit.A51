#include <actos.h> 

EXTRN ECODE(slyricInit??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public lyricInit??
lyricInit??:
MOV	WR8,#WORD0(slyricInit??)
MOV	R10, #BYTE2(slyricInit??) 
LJMP    RSTMIDAPI 

END