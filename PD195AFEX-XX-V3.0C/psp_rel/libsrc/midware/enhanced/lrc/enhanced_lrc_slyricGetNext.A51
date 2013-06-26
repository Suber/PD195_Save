#include <actos.h> 

EXTRN ECODE(slyricGetNext??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public lyricGetNext??
lyricGetNext??:
MOV	WR8,#WORD0(slyricGetNext??)
MOV	R10, #BYTE2(slyricGetNext??) 
LJMP    RSTMIDAPI 

END