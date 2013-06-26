$NOCOND
$include (actos.inc)
NAME    ?BANK?SWITCHING

 
?BANK?SELECT    SEGMENT  'CODE_BANKF'

PUBLIC  ?B_NBANKS, ?B_MODE 
?B_NBANKS       EQU     64        
?B_MODE         EQU     2     
 
SELECT   MACRO   N
PUBLIC  ?B_BANK&N
?B_BANK&N:      
ENDM 

rseg  ?BANK?SELECT
public ?C?INITEDATA
?C?INITEDATA:
	  CNT   SET     0
      REPT    ?B_NBANKS
      SELECT  %CNT
	  CNT   SET     CNT+1
      ENDM  
      ret
END
