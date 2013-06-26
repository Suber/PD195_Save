/*
********************************************************************************
*                       ACTOS
*                  driver demo
*
*                (c) Copyright 2002-2003, Actions Co,Ld.
*                        All Right Reserved
*
* File   : ui_S6B33B0A_bnkif.A51
* Version: 1> v1.00     first version     date
********************************************************************************
*/
#include "ak211x.h"
$include (actos.inc)
#include "display.h"
/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		bank A/B在code ram内存分布示意： 
		      bank  header----------BAH/BBH
			  bank  const var-------BAS/BBS
			  bank  code------------BAC/BBC
		    在data ram内存分布:
			  bank  data------------BAD/BBD
		Bank Header定义： 
	        // Bank的头信息段 
	        rseg  BBH_Func
	
	        // Bank的有效长度，必须小于系统给定的bank最大长度 
	        defw  code_end - bank_start_addr
	
	        // bank内函数入口地址表，最多8个函数入口 
	        defs  16
	
			//data len，one byte
			db    BBD_L_
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

        NAME     UI_BNKIF


		BBH_IE   SEGMENT  CODE
		BBH_DW3  SEGMENT  CODE
		BBH_PUTS SEGMENT  CODE
		BBH_GSL  SEGMENT  CODE
		BBH_ARABIC SEGMENT  CODE
		BBH_SubArabic SEGMENT  CODE
		BBH_GSLExt SEGMENT  CODE
		BBH_Thai SEGMENT  CODE 
     
		 
		BAH_CLRS SEGMENT  CODE	
		BAH_PSDI  SEGMENT  CODE
		BAH_DW1	 SEGMENT  CODE
		BAH_IS_NSM SEGMENT  CODE
		
		//BAH_Thai SEGMENT  CODE
		BAH_TurnArabicToUn SEGMENT  CODE
		//BAH_MSG SEGMENT  CODE
		BBH_HFUN SEGMENT  CODE
		
//-----------------------------------------------------------------
//----------------------------  BANK_B ----------------------------
//-----------------------------------------------------------------
  


		EXTRN   HCONST(_PR_BBC_IE_E_)
		EXTRN   HCONST(_ED_BBD_IE_SDRV_INIT_L_)

        EXTRN   CODE(Sdrv_Init{RPrefix})
        EXTRN   CODE(Sdrv_Exit{RPrefix})
        EXTRN   CODE(Sdrv_LanguageSet{RPrefix})
		EXTRN   CODE(Hdrv_HardInit{RPrefix})
        
		RSEG    BBH_IE
        DW      WORD0(_PR_BBC_IE_E_ - BNKB_SRAM_ADDR + 1)
        DW      WORD0(Sdrv_Init{RPrefix})
        DW      WORD0(Sdrv_Exit{RPrefix})
        DW      WORD0(Sdrv_LanguageSet{RPrefix})
		DW      WORD0(Hdrv_HardInit{RPrefix})
        DS      8 
        DB      0

//-----------------------------------------------------------------

 		EXTRN   HCONST(_PR_BBC_DW3_E_)

        EXTRN   CODE(Sdrv_RegionCopy{RPrefix})
		EXTRN   CODE(Sdrv_CharOutRect{RPrefix})
		 

		RSEG    BBH_DW3
        DW      WORD0(_PR_BBC_DW3_E_ - BNKB_SRAM_ADDR + 1)
        DW      WORD0(Sdrv_RegionCopy{RPrefix})
		DW      WORD0(Sdrv_CharOutRect{RPrefix})
        DS      12 
        DB      0

//-----------------------------------------------------------------
 


		EXTRN    HCONST(_PR_BBC_PUTS_E_)  


        EXTRN    CODE(Sdrv_PutChar{RPrefix})
        EXTRN    CODE(Sdrv_PutS{RPrefix})
        //EXTRN    CODE(Sdrv_CharOutRect{RPrefix})
	    EXTRN    CODE(Sdrv_PutUnS{RPrefix})
        EXTRN    CODE(Sdrv_GetFontPoint{RPrefix})
		EXTRN    CODE(Sdrv_PutStringExt{RPrefix})	
		EXTRN    CODE(Sdrv_SetTextPos{RPrefix})		
        EXTRN    CODE(Sdrv_GetTextPos{RPrefix})	
		RSEG     BBH_PUTS
        DW       WORD0(_PR_BBC_PUTS_E_ - BNKB_SRAM_ADDR + 1)
        DW       WORD0(Sdrv_PutChar{RPrefix})
        DW       WORD0(Sdrv_PutS{RPrefix}) 	
        DW       WORD0(Sdrv_PutUnS{RPrefix})	
        DW       WORD0(Sdrv_GetFontPoint{RPrefix})        
		DW       WORD0(Sdrv_PutStringExt{RPrefix})		    
		DW       WORD0(Sdrv_SetTextPos{RPrefix})
		DW       WORD0(Sdrv_GetTextPos{RPrefix})   
        DS       2
		DB       0 
		
		
//-----------------------------------------------------------------
 

        EXTRN    HCONST(_PR_BBC_HFUN_E_)

		EXTRN    CODE(Sdrv_Invert{RPrefix})
		

		RSEG     BBH_HFUN
        DW       WORD0(_PR_BBC_HFUN_E_ - BNKB_SRAM_ADDR + 1)
		DW       WORD0(Sdrv_Invert{RPrefix})
		DS       14
        DB       0 		
		
		 
//-----------------------------------------------------------------
 

        EXTRN    HCONST(_PR_BBC_GSL_E_)

		EXTRN    CODE(Sdrv_GetStringLength{RPrefix})
		

		RSEG     BBH_GSL
        DW       WORD0(_PR_BBC_GSL_E_ - BNKB_SRAM_ADDR + 1)
		DW       WORD0(Sdrv_GetStringLength{RPrefix})
		DS       14
        DB       0 

//-----------------------------------------------------------------

        EXTRN    HCONST(_PR_BBC_ARABIC_E_)

		EXTRN    CODE(UnicodeTOArabic{RPrefix})
		

		RSEG     BBH_ARABIC
        DW       WORD0(_PR_BBC_ARABIC_E_ - BNKB_SRAM_ADDR + 1)
		DW       WORD0(UnicodeTOArabic{RPrefix})
		DS       14
        DB       0 


		 
        
//-----------------------------------------------------------------
       
      
        
        EXTRN    HCONST(_PR_BBC_SubArabic_E_)

		EXTRN    CODE(CopyStr{RPrefix})
		

		RSEG     BBH_SubArabic
        DW       WORD0(_PR_BBC_SubArabic_E_ - BNKB_SRAM_ADDR + 1)
		DW       WORD0(CopyStr{RPrefix})
		DS       14
        DB       0 
        
             
//-----------------------------------------------------------------
        
        EXTRN    HCONST(_PR_BBC_GSLExt_E_)

		EXTRN    CODE(Sdrv_GetStringLengthExt{RPrefix})
		

		RSEG     BBH_GSLExt
        DW       WORD0(_PR_BBC_GSLExt_E_ - BNKB_SRAM_ADDR + 1)
		DW       WORD0(Sdrv_GetStringLengthExt{RPrefix})
		DS       14
        DB       0 
        
//-----------------------------------------------------------------
        
        EXTRN   HCONST(_PR_BBC_Thai_E_)

        EXTRN   CODE(Sdrv_ThaiOverlap{RPrefix})
        EXTRN   CODE(Sdrv_FontCodeMBToUni{RPrefix})
		RSEG    BBH_Thai
        DW      WORD0(_PR_BBC_Thai_E_ - BNKB_SRAM_ADDR + 1)
        DW      WORD0(Sdrv_ThaiOverlap{RPrefix})
        DW      WORD0(Sdrv_FontCodeMBToUni{RPrefix})
        DS      12
        DB      0 
         
  

//------------------------------------------------------------------


//----------------------------  BANK_A ----------------------------
//-----------------------------------------------------------------  

 
   		
		EXTRN    HCONST(_PR_BAC_CLRS_E_)

        EXTRN    CODE(Sdrv_ClearScreen{RPrefix})

		RSEG     BAH_CLRS
        DW       WORD0(_PR_BAC_CLRS_E_ - BNKA_SRAM_ADDR + 1)
        DW       WORD0(Sdrv_ClearScreen{RPrefix})
        DS       14 
        DB       0   	
				       
//----------------------------------------------------------------- 

 


		EXTRN   HCONST(_PR_BAC_PSDI_E_)

        EXTRN   CODE(Sdrv_PutSDImage{RPrefix})
		 
		RSEG    BAH_PSDI
        DW      WORD0(_PR_BAC_PSDI_E_ - BNKA_SRAM_ADDR + 1)
        DW      WORD0(Sdrv_PutSDImage{RPrefix})
		
        DS      14 
        DB      0 

//----------------------------------------------------------------- 
      
 
		EXTRN   HCONST(_PR_BAC_DW1_E_)

        EXTRN   CODE(Sdrv_FillRect{RPrefix})

		RSEG    BAH_DW1
        DW      WORD0(_PR_BAC_DW1_E_ - BNKA_SRAM_ADDR + 1)
        DW      WORD0(Sdrv_FillRect{RPrefix})
        DS      14 
        DB      0 
//--------------------------------------------------------------------
 
      
      
		EXTRN   HCONST(_PR_IS_NSM_E_)

        EXTRN   CODE(GUI_BIDI_IsNSM{RPrefix})

		RSEG    BAH_IS_NSM
        DW      WORD0(_PR_IS_NSM_E_ - BNKA_SRAM_ADDR + 1)
        DW      WORD0(GUI_BIDI_IsNSM{RPrefix})
        DS      14 
        DB      0 
 
//--------------------------------------------------------------------
        
        
/*        
        EXTRN   HCONST(_PR_BAC_Thai_E_)

        EXTRN   CODE(Sdrv_ThaiOverlap{RPrefix})
        EXTRN   CODE(Sdrv_FontCodeMBToUni{RPrefix})
		RSEG    BAH_Thai
        DW      WORD0(_PR_BAC_Thai_E_ - BNKA_SRAM_ADDR + 1)
        DW      WORD0(Sdrv_ThaiOverlap{RPrefix})
        DW      WORD0(Sdrv_FontCodeMBToUni{RPrefix})
        DS      12
        DB      0 
*/        
//--------------------------------------------------------------------
 
        EXTRN   HCONST(_PR_BAC_TurnArabicToUn_E_)

        EXTRN   CODE(TurnArabicToUn{RPrefix})
         
		RSEG    BAH_TurnArabicToUn
        DW      WORD0(_PR_BAC_TurnArabicToUn_E_ - BNKA_SRAM_ADDR + 1)
         
        DW      WORD0(TurnArabicToUn{RPrefix})
        DS      14
        DB      0 

 

//--------------------------------------------------------------------
 
  /*      EXTRN   HCONST(_PR_BAC_MSG_E_)

        EXTRN   CODE(Sdrv_MsgBox{RPrefix})
         
		RSEG    BAH_MSG
        DW      WORD0(_PR_BAC_MSG_E_ - BNKA_SRAM_ADDR + 1)
         
        DW      WORD0(Sdrv_MsgBox{RPrefix})
        DS      14
        DB      0 
        */

/*       
********************************************************************************
*  the end of file
********************************************************************************
*/
END

	