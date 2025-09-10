#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "ds1307.h"

extern unsigned char day;
extern unsigned char date;
extern unsigned char month;
extern unsigned char year;

void day_setup(void);
void date_setup(void);
void month_setup(void);
void year_setup(void);

void date_page_display(void){

LCD_clearButton();
LCD_fillScreen(BLACK);
LCD_print2C(130,20,"date Screen",4,WHITE,BLACK);
LCD_drawButton(6,10,100,90,90,5,RED,WHITE,"day",3);
LCD_drawButton(7,110,100,90,90,5,RED,WHITE,"date",3);
LCD_drawButton(8,210,100,110,90,5,RED,WHITE,"month",3);
LCD_drawButton(9,330,100,140,90,5,RED,WHITE,"year",3);
LCD_drawButton(11,10,200,470,90,5,GREEN,WHITE,"BACK",4);
}


int date_page(void){
int sel=-1,x,y;
	
date_page_display();
	
do{

	while(T_IRQ);
	x=ReadTouchX();
	y=ReadTouchY();
	sel=ButtonTouch(x,y);	
	while(T_IRQ);

	switch(sel){
		case 6: 
						LCD_clearButton();
						LCD_fillScreen(BLACK);
						day_setup();
						date_page_display();
						break;
		case 7:
						LCD_clearButton();
						LCD_fillScreen(BLACK);
						date_setup();
						date_page_display();
						break;
		case 8:
						LCD_clearButton();
						LCD_fillScreen(BLACK);
						month_setup();
						date_page_display();
						break;
		case 9:
					  LCD_clearButton();
						LCD_fillScreen(BLACK);
						year_setup();
						date_page_display();
						break;
	}




}while(sel!=11);	

setupdaydate(day,date,month,year); // seting start date
	day = readDs1307(0x03);  // real day update
  date = readDs1307(0x04); // real date update
  month = readDs1307(0x05); // real month update
	year = readDs1307(0x06); // real year update
setupdaydate(day,date,month,year); // seting start date

LCD_clearButton();
LCD_fillScreen(BLACK);

return 1;





}