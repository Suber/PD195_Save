$include (actos.inc)


/************************* Segment Define ************************************/
 		BAH_IE	 	SEGMENT  	ECODE 
		BAH_CHARGE	SEGMENT		ECODE
		BAH_STATE	SEGMENT		ECODE
		BAH_GSENSOR1	SEGMENT		ECODE
		BAH_GSENSOR2	SEGMENT		ECODE
		BAH_GSENSOR3	SEGMENT		ECODE
		//BAH_KT		SEGMENT		ECODE	 
/************************* public info **************************************/
	 
/************************* EXTRN symbol *************************************/
        EXTRN  			CODE(key_init{RPrefix})
        EXTRN  			CODE(key_exit{RPrefix})     	 
		EXTRN 			CODE(sKY_CardPowerOn{RPrefix})
		EXTRN 			CODE(sKY_ChargeSet{RPrefix})
		EXTRN 			CODE(sKY_ChargeGetandSet{RPrefix})
		EXTRN			CODE(sKY_GetKeyTabAddress{RPrefix})
		EXTRN			CODE(sKY_Beep{RPrefix})
		EXTRN			CODE(sKY_HoldCheck{RPrefix})
		EXTRN			CODE(sKY_GetEarState{RPrefix})	
		EXTRN			CODE(sKY_SetGsensor{RPrefix})
		EXTRN			CODE(sKY_BankGsensorGetStep{RPrefix})
		EXTRN			CODE(sKY_BankGsensorGetADC{RPrefix})

		EXTRN			CODE(sKY_GsensorOn1{RPrefix})
		EXTRN			CODE(sKY_GsensorOff1{RPrefix})
		EXTRN			CODE(sKY_ReadGSensor1{RPrefix})
		EXTRN			CODE(sKY_GsensorOn2{RPrefix})
		EXTRN			CODE(sKY_GsensorOff2{RPrefix})
		EXTRN			CODE(sKY_ReadGSensor2{RPrefix})
		EXTRN			CODE(sKY_GsensorOn3{RPrefix})
		EXTRN			CODE(sKY_GsensorOff3{RPrefix})
		EXTRN			CODE(sKY_ReadGSensor3{RPrefix})
	
		
/************************* code begin   *************************************/

 
/************************* bank head define  *************************************/
//-------------------------------------------------------------------------------
 		EXTRN   HCONST(_PR_BAC_IE1_E_)

        rseg    BAH_IE
  		DW	    WORD0(_PR_BAC_IE1_E_ - BNKA_SRAM_ADDR + 1) 
        DW    	WORD0(key_init{RPrefix})
        DW    	WORD0(key_exit{RPrefix})
		DW		0000H
		DW		0000H
		DW		0000H
		DW	    WORD0(sKY_CardPowerOn{RPrefix})
		DW		0000H
        DS    	2
		DB		00H 
//-------------------------------------------------------------------------------
		EXTRN   HCONST(_PR_BAC_CHARGE_E_)

        rseg    BAH_CHARGE
  		DW	    WORD0(_PR_BAC_CHARGE_E_ - BNKA_SRAM_ADDR + 1) 
        DW    	WORD0(sKY_ChargeSet{RPrefix})
        DW    	WORD0(sKY_ChargeGetandSet{RPrefix})
        DS    	12
		DB		00H 
//------------------------------------------------------------------------------- 
		EXTRN   HCONST(_PR_BAC_STATE_E_)

        rseg    BAH_STATE
  		DW	    WORD0(_PR_BAC_STATE_E_ - BNKA_SRAM_ADDR + 1) 
        DW    	WORD0(sKY_GetKeyTabAddress{RPrefix})
		DW    	WORD0(sKY_Beep{RPrefix})
		DW    	WORD0(sKY_HoldCheck{RPrefix})		
		DW    	WORD0(sKY_GetEarState{RPrefix})
		DW		WORD0(sKY_SetGsensor{RPrefix})
		DW		WORD0(sKY_BankGsensorGetStep{RPrefix})
		DW		WORD0(sKY_BankGsensorGetADC{RPrefix})
        DS    	2
		DB		00H 
//------------------------------------------------------------------------------- 
		//EXTRN   HCONST(_PR_BAC_KT_E_)

        //rseg    BAH_KT
  		//DW	    WORD0(_PR_BAC_KT_E_ - BNKA_SRAM_ADDR + 1) 
        //DW    	WORD0(sKY_Beep{RPrefix})
        //DS    	14
		//DB		00H 
//------------------------------------------------------------------------------- 
		
		EXTRN   HCONST(_PR_KY_GSENSOR1_E_)
        rseg    BAH_GSENSOR1
  		DW	    WORD0(_PR_KY_GSENSOR1_E_ - BNKA_SRAM_ADDR + 1) 
        DW    	WORD0(sKY_GsensorOn1{RPrefix})
        DW    	WORD0(sKY_GsensorOff1{RPrefix})
        DW    	WORD0(sKY_ReadGSensor1{RPrefix})
        DS    	10
		DB		00H 

		EXTRN   HCONST(_PR_KY_GSENSOR2_E_)
        rseg    BAH_GSENSOR2
  		DW	    WORD0(_PR_KY_GSENSOR2_E_ - BNKA_SRAM_ADDR + 1) 
        DW    	WORD0(sKY_GsensorOn2{RPrefix})
        DW    	WORD0(sKY_GsensorOff2{RPrefix})
        DW    	WORD0(sKY_ReadGSensor2{RPrefix})
        DS    	10
		DB		00H 

		EXTRN   HCONST(_PR_KY_GSENSOR3_E_)
        rseg    BAH_GSENSOR3
  		DW	    WORD0(_PR_KY_GSENSOR3_E_ - BNKA_SRAM_ADDR + 1) 
        DW    	WORD0(sKY_GsensorOn3{RPrefix})
        DW    	WORD0(sKY_GsensorOff3{RPrefix})
        DW    	WORD0(sKY_ReadGSensor3{RPrefix})
        DS    	10
		DB		00H 
		
		
        END