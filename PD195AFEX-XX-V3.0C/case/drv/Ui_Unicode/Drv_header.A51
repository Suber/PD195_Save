#include "display.h"
#include "Drv_S7565.h"


		NAME   DRV_HEADER

		UI_INFO SEGMENT CODE
		RSEG   UI_INFO
//定义驱动头信息 
DRV_FileType:            DB    'D'
DRV_Type:                DB    DRV_UI
DRV_RcodeStartAddr:      DW    0x3400
DRV_RcodeLength:         DW    0x0800
DRV_Rcode2StartAddr:     DW    0x0000
DRV_Rcode2Length:        DW    0x0000
DRV_RdataStartAddr:      DW    0x1400
DRV_RdataLength:         DW    0x00Af
DRV_InitAPI:             DW    0x0000 + API_UI_BaseNoB
DRV_ExitAPI:             DW    0x1000 + API_UI_BaseNoB
DRV_BnkAFileAddr:        DS    4
DRV_BnkBFileAddr:        DS    4
DRV_BnkADataFileAddr:    DS    4
DRV_BnkBDataFileAddr:    DS    4
ORG     500		  
ICVer:                   DB    21h,15h
ORG     510
DW      0x55aa

END