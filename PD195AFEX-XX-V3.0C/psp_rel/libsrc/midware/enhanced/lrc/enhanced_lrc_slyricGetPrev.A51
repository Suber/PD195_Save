#include <actos.h> 

EXTRN ECODE(slyricGetPrev??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public lyricGetPrev??
lyricGetPrev??:
MOV	WR8,#WORD0(slyricGetPrev??)
MOV	R10, #BYTE2(slyricGetPrev??) 
LJMP    RSTMIDAPI 

END