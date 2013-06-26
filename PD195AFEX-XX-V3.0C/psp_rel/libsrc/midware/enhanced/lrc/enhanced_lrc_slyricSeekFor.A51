#include <actos.h> 

EXTRN ECODE(slyricSeekFor??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public lyricSeekFor??
lyricSeekFor??:
MOV	WR8,#WORD0(slyricSeekFor??)
MOV	R10, #BYTE2(slyricSeekFor??) 
LJMP    RSTMIDAPI 

END