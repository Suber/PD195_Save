
$include (actos.inc)
#include "driver.h"

/************************* Segment Define ********************/
DRV_INFO SEGMENT CODE
/************************* public info ***********************/
/************************* EXTRN symbol **********************/
EXTRN HCONST(_PR_KY_COM_E_)
EXTRN HCONST(_PR_KY_CARD_E_)
 
/************************* code begin ***********************/

rseg    DRV_INFO
DRV_FileType:             	  DB    'D'
DRV_Type:                 	  DB    DRV_KY
DRV_RcodeStartAddr:       	  DW    0x0200
DRV_RcodeLength:          	  DW    _PR_KY_COM_E_ - 0x0200 +1
DRV_Rcode2StartAddr:      	  DW    0x6c00
DRV_Rcode2Length:         	  DW    _PR_KY_CARD_E_ - 0x6c00 +1
DRV_RdataStartAddr:       	  DW    0x14b0
DRV_RdataLength:          	  DW    0x0050
DRV_InitAPI:              	  DW    0x0000 + API_KY_BaseNoA
DRV_ExitAPI:              	  DW    0x1000 + API_KY_BaseNoA
DRV_BnkAFileAddr:             DS    4
DRV_BnkBFileAddr:    	      DS    4
DRV_BnkADataFileAddr:         DS    4
DRV_BnkBDataFileAddr:         DS    4
org     500
ICVer:                   DB    21h,15h
org     510
DW      0x55aa


end