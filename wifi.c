//=========================== Example2_1.c ============================
#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"


void PUTCHAR0(char tav);
char GETCHAR0(void);
void clear_arry(void);
extern unsigned char arry[30];


unsigned char wifi(void){
unsigned char i=0,tav=0,sel=-1,x,y;
	LCD_drawButton(2,40,210,100,70,5,GREEN,WHITE,"back",4);
	
	do{
		  LCD_setTextSize(3);
		  x=ReadTouchX();
		  y=ReadTouchY();
		  sel=ButtonTouch(x,y);
			delay_ms(10);
		  tav=1;
		 	PUTCHAR0(tav);
		  GETCHAR0();  // Receives user input into 'arry'
			LCD_setCursor(0,i);
			printf("%s", arry); 
			tav=2;
			PUTCHAR0(tav);
		
			//clear_arry();
			while(tav!='3'){
		   tav=GETCHAR0();
			 LCD_setCursor(0,40);
			 printf("%s", arry); 
			}
	}while(sel!=2);
	LCD_clearButton();
	LCD_fillScreen(BLACK);

return 0;
}
