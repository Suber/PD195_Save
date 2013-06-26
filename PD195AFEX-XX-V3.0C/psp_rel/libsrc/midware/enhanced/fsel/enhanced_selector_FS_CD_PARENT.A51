#include <actos.h> 

EXTRN ECODE(sFS_CD_DIR_PARENT??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public FS_CD_DIR_PARENT??
FS_CD_DIR_PARENT??:
MOV	WR8,#WORD0(sFS_CD_DIR_PARENT??)
MOV	R10, #BYTE2(sFS_CD_DIR_PARENT??) 
LJMP    RSTMIDAPI 

END