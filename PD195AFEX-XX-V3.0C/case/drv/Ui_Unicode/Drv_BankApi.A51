/*
********************************************************************************
*                       ACTOS
*                  driver demo
*
*                (c) Copyright 2002-2003, Actions Co,Ld.
*                        All Right Reserved
*
* File   : Drv_BankAPI.msa
* Version: 1> v1.00     first version     date
********************************************************************************
*/
#include "ak211x.h"
$include (actos.inc)
#include "display.h"

		NAME UI_DRV_FUN_INTERNAL
		API30_UI_DRV SEGMENT CODE

        public  BAPI_PutS{RPrefix}
		public  BAPI_ClearScreen{RPrefix}
		public  BAPI_GetFontPoint{RPrefix}
		public  BAPI_UnicodeTOArabic{RPrefix}
		public  BAPI_GUI_BIDI_IsNSM{RPrefix}
		public  BAPI_CopyStr{RPrefix}
		public  BAPI_TurnArabicToUn{RPrefix}
		public  BAPI_ThaiOverlap{RPrefix}
		public  BAPI_FontCodeMBToUni{RPrefix}
		 
		 
        RSEG    API30_UI_DRV


BAPI_PutS{RPrefix}:
        MOV     WR8,  #API_PutS
        LJMP    RSTBankAPI

BAPI_ClearScreen{RPrefix}:
        MOV     WR8,  #API_ClearScreen
        LJMP    RSTBankAPI

BAPI_GetFontPoint{RPrefix}:
        MOV     WR8,  #API_GetFontPoint
        LJMP    RSTBankAPI

BAPI_UnicodeTOArabic{RPrefix}:
        MOV     WR8,  #API_UnicodeTOArabic
        LJMP    RSTBankAPI


BAPI_GUI_BIDI_IsNSM{RPrefix}:
        MOV     WR8,  #API_GUI_BIDI_IsNSM
        LJMP    RSTBankAPI

BAPI_CopyStr{RPrefix}:
        MOV     WR8,  #API_CopyStr
        LJMP    RSTBankAPI

BAPI_TurnArabicToUn{RPrefix}:
        MOV     WR8,  #API_TurnArabicToUn
		LJMP    RSTBankAPI

BAPI_ThaiOverlap{RPrefix}:
        MOV     WR8,  #API_ThaiOverlap
		LJMP    RSTBankAPI

BAPI_FontCodeMBToUni{RPrefix}:
	    MOV     WR8,  #API_FontCodeMBToUni
		LJMP    RSTBankAPI
 
end


