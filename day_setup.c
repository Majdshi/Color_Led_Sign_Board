#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "ds1307.h"

extern unsigned char day;
extern unsigned char date;
extern unsigned char month;
extern unsigned char year;

void input_key_bord(void);

void day_setup(void){
int sel=1,x,y;
unsigned char num;


LCD_clearButton();
LCD_fillScreen(BLACK);	


LCD_drawButton(1,10,10,100,70,5,RED,WHITE,"Sun",4);
LCD_drawButton(2,120,10,100,70,5,RED,WHITE,"Mon",4);
LCD_drawButton(3,230,10,100,70,5,RED,WHITE,"Tue",4);
LCD_drawButton(4,340,10,100,70,5,RED,WHITE,"Wed",4);
LCD_drawButton(5,10,100,100,70,5,RED,WHITE,"Thu",4);
LCD_drawButton(6,120,100,100,70,5,RED,WHITE,"Fri",4);
LCD_drawButton(7,230,100,100,70,5,RED,WHITE,"Sat",4);
	
	
LCD_drawButton(1000,340,100,110,70,5,RED,WHITE,"(^_-)",4);
LCD_drawButton(10,10,180,220,30,5,GREEN,WHITE,"Send",3);
LCD_drawButton(11,240,180,220,30,5,BLUE,WHITE,"Clear",3);
LCD_drawButton(12,240,215,220,30,5,BLUE,WHITE,"back",3);
LCD_fillRoundRect(10,250,220,50,5,RED);
LCD_fillRoundRect(240,250,220,50,5,WHITE);
LCD_printC(10,220,"Text box:",3,WHITE);


do{

		
			
				while(T_IRQ);
				x=ReadTouchX();
				y=ReadTouchY();
				sel=ButtonTouch(x,y);
				while(!T_IRQ);
			 
			
					switch(sel){
						case 1:	LCD_print2C(10,250,"Sunday",4,WHITE,RED);
										num=sel;
										break;
						case 2:	LCD_print2C(10,250,"Monday",4,WHITE,RED);
										num=sel;
										break;
						case 3:	LCD_print2C(10,250,"Tuesday",4,WHITE,RED);
										num=sel;
										break;
						case 4:	LCD_print2C(10,250,"Wednesday",4,WHITE,RED);
										num=sel;
										break;
						case 5:	LCD_print2C(10,250,"Thursday",4,WHITE,RED);
										num=sel;
										break;
						case 6:	LCD_print2C(10,250,"Friday",4,WHITE,RED);
										num=sel;
										break;
						case 7:	LCD_print2C(10,250,"Saturday",4,WHITE,RED);
										num=sel;
										break;
					}
						
			

	
	if(sel==11){
		num=day;
	  LCD_fillRoundRect(10,250,220,50,5,RED);
		LCD_printC(290,260,"CLEARD",3,BLACK);
		delay_ms(500);
  	LCD_fillRoundRect(240,250,220,50,5,WHITE);
		LCD_setText2Color(WHITE,RED);
	}
	
	
	if(1<=num && num<=7 && sel==10){
	
		LCD_printC(290,260,"SEND",3,BLACK);
		delay_ms(500);
		LCD_fillRoundRect(240,250,220,50,5,WHITE);
		LCD_setText2Color(WHITE,RED);
		day=num;
		setupdaydate(day,date,month,year); // seting start date
		day = readDs1307(0x03);  // real day update
		date = readDs1307(0x04); // real date update
		month = readDs1307(0x05); // real month update
		year = readDs1307(0x06); // real year update
		
	}else if(num < 1 || num > 7 && sel==10){	
	  LCD_printC(290,260,"error",3,BLACK);
		delay_ms(500);
		LCD_fillRoundRect(240,250,220,50,5,WHITE);
		LCD_setText2Color(WHITE,RED);
	}
		
  



}while(sel != 12);

setupdaydate(day,date,month,year); // seting start date
	day = readDs1307(0x03);  // real day update
  date = readDs1307(0x04); // real date update
  month = readDs1307(0x05); // real month update
	year = readDs1307(0x06); // real year update
setupdaydate(day,date,month,year); // seting start date

LCD_clearButton();
LCD_fillScreen(BLACK);
}





