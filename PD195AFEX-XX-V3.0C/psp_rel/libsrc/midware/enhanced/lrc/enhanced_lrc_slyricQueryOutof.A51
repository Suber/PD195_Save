#include <actos.h> 

EXTRN ECODE(slyricQueryOutof??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public lyricQueryOutof??
lyricQueryOutof??:
MOV	WR8,#WORD0(slyricQueryOutof??)
MOV	R10, #BYTE2(slyricQueryOutof??) 
LJMP    RSTMIDAPI 

END