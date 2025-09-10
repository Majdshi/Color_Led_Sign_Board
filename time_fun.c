#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "ds1307.h"

void hour_setup(void);
void min_setup(void);
void sec_setup(void);


extern unsigned char hour;
extern unsigned char min;
extern unsigned char sec;

void time_page_display(void){

LCD_clearButton();
LCD_fillScreen(BLACK);
LCD_print2C(130,20,"time Screen",4,WHITE,BLACK);
LCD_drawButton(6,10,100,110,90,5,RED,WHITE,"hour",4);
LCD_drawButton(7,130,100,110,90,5,RED,WHITE,"min",4);
LCD_drawButton(8,250,100,220,90,5,RED,WHITE,"sec",4);
LCD_drawButton(9,10,200,470,90,5,GREEN,WHITE,"BACK",4);


}


int time_page(void){
int sel3=-1,x3,y3;

	time_page_display();

while(sel3 != 9){

  while(T_IRQ);
  x3=ReadTouchX();
	y3=ReadTouchY();
	sel3=ButtonTouch(x3,y3);
	
	switch(sel3){	

			  case 6:
    					hour_setup();
						  time_page_display();

						break;
		case 7:LCD_clearButton();
           LCD_fillScreen(BLACK);
					 min_setup();
	        	time_page_display();

						break;
		case 8:
						LCD_clearButton();
           LCD_fillScreen(BLACK);
					 sec_setup();
		       time_page_display();

						break;
	
	
	
	}				 
	
	

}

	hour = readDs1307(0x02);  // real hour update
  min = readDs1307(0x01); // real min update
  sec = readDs1307(0x00); // real second update
setupTime(hour,min,sec);

LCD_clearButton();
LCD_fillScreen(BLACK);
	return 1;






}