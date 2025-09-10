#include "compiler_defs.h"
#include "C8051F380_defs.h"
void PUTCHAR0(char tav)
	{
		SBUF0 = tav ;
		while (TI0 == 0) ;
		TI0 = 0  ;
	}
char GETCHAR0 (void) 
	{
		while (RI0 == 0) ;
		RI0 = 0 ; 
		return SBUF0 ;
	}