 

$include (reg251s.inc)

		PUBLIC	?C_STARTUP?
		PUBLIC	?C?STARTUP?

NAME	?C_START??


?C_C51STARTUP	SEGMENT     CODE
RSEG	?C_C51STARTUP
?C?STARTUP?:
?C_STARTUP?:
		eret   
		END
