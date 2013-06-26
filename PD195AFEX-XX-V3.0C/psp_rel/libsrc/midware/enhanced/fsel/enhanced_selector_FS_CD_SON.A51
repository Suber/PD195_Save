#include <actos.h> 

EXTRN ECODE(sFS_CD_DIR_SON??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public FS_CD_DIR_SON??
FS_CD_DIR_SON??:
MOV	WR8,#WORD0(sFS_CD_DIR_SON??)
MOV	R10, #BYTE2(sFS_CD_DIR_SON??) 
LJMP    RSTMIDAPI 

END