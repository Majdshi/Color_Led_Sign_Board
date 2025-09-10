//===================== uart1.c ====================
#include "initsys.h"
#include "uart1.h"

void PUTCHAR1(U8 tav){
	SBUF1=tav;
	while(TI1==0); 
	clrTI1;//SCON1=SCON1&(~2);//TI1 = 0;  
}

U8 GETCHAR1(void){
   while(RI1==0); 
   clrRI1;//SCON1=SCON1&0Xfe;//RI1=0;
   return(SBUF1);
}

//=================================================
