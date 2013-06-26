
$include (actos.inc)
#include "key.h"

		NAME KEY_DRV_FUN_INTERNAL
		API30_KEY_DRV SEGMENT CODE

		public	BAPI_sKY_GsensorOn1{RPrefix}
		public	BAPI_sKY_GsensorOff1{RPrefix}
		public	BAPI_sKY_ReadGSensor1{RPrefix}
		public	BAPI_sKY_GsensorOn2{RPrefix}
		public	BAPI_sKY_GsensorOff2{RPrefix}
		public	BAPI_sKY_ReadGSensor2{RPrefix}
		public	BAPI_sKY_GsensorOn3{RPrefix}
		public	BAPI_sKY_GsensorOff3{RPrefix}
		public	BAPI_sKY_ReadGSensor3{RPrefix}
		 
        RSEG    API30_KEY_DRV

/************************** EXTRN info ***********************/  
/************************* code begin ***********************/
//BANK APIÌø×ª 
BAPI_sKY_GsensorOn1{RPrefix}:
		MOV		WR8, #API_GsensorOn1
		LJMP    RSTBankAPI
BAPI_sKY_GsensorOff1{RPrefix}:
		MOV		WR8, #API_GsensorOff1
		LJMP    RSTBankAPI	
BAPI_sKY_ReadGSensor1{RPrefix}:
		MOV		WR8, #API_ReadGSensor1
		LJMP    RSTBankAPI		

BAPI_sKY_GsensorOn2{RPrefix}:
		MOV		WR8, #API_GsensorOn2
		LJMP    RSTBankAPI	
BAPI_sKY_GsensorOff2{RPrefix}:
		MOV		WR8, #API_GsensorOff2
		LJMP    RSTBankAPI
BAPI_sKY_ReadGSensor2{RPrefix}:
		MOV		WR8, #API_ReadGSensor2
		LJMP    RSTBankAPI	

BAPI_sKY_GsensorOn3{RPrefix}:
		MOV		WR8, #API_GsensorOn3
		LJMP    RSTBankAPI	
BAPI_sKY_GsensorOff3{RPrefix}:
		MOV		WR8, #API_GsensorOff3
		LJMP    RSTBankAPI	
BAPI_sKY_ReadGSensor3{RPrefix}:
		MOV		WR8, #API_ReadGSensor3
		LJMP    RSTBankAPI	

END
