#include <actos.h> 

EXTRN ECODE(smp3CheckType??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public mp3CheckType??
mp3CheckType??:
MOV	WR8,#WORD0(smp3CheckType??)
MOV	R10, #BYTE2(smp3CheckType??) 
LJMP    RSTMIDAPI 

END