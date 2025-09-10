#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "i2c.h"
#include "ds1307.h"



extern unsigned char hour;
extern unsigned char min;
extern unsigned char sec;


extern unsigned char rem_min ;
extern unsigned char rem_hour;

unsigned char rem_min_setup(void);
int rem_hour_setup(void);

void reminder_page_display(void){

LCD_clearButton();
LCD_fillScreen(BLACK);
LCD_print2C(80,20,"reminder Screen",4,WHITE,BLACK);
LCD_drawButton(6,10,100,210,90,5,RED,WHITE,"hour",4);
LCD_drawButton(7,240,100,230,90,5,RED,WHITE,"min",4);
LCD_drawButton(9,10,200,470,90,5,GREEN,WHITE,"BACK",4);


}


int reminder_page(void){
int sel=-1,x,y;

	reminder_page_display();

while(sel != 9){

  while(T_IRQ);
  x=ReadTouchX();
	y=ReadTouchY();
	sel=ButtonTouch(x,y);
	
	switch(sel){	

			  case 6:
    					rem_hour=rem_hour_setup();
						    reminder_page_display();

						break;
		case 7:LCD_clearButton();
           LCD_fillScreen(BLACK);
					 rem_min=rem_min_setup();
	        	reminder_page_display();

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