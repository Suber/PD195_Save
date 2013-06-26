#include <actos.h> 

EXTRN ECODE(slyricLoad??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public lyricLoad??
lyricLoad??:
MOV	WR8,#WORD0(slyricLoad??)
MOV	R10, #BYTE2(slyricLoad??) 
LJMP    RSTMIDAPI 

END