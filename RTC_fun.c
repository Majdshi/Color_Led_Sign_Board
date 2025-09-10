//RTC_display

#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "ds1307.h"



extern unsigned char hour;
extern unsigned char min;
extern unsigned char sec;

int time_page(void);
int date_page(void);
int reminder_page(void);

int RTC(void){
int sel2=0,x2,y2;
LCD_print2C(130,20,"RTC Screen",4,WHITE,BLACK);
LCD_drawButton(2,10,100,110,90,5,RED,WHITE,"TIME",4);
LCD_drawButton(3,130,100,110,90,5,RED,WHITE,"DATE",4);
LCD_drawButton(4,250,100,220,90,5,RED,WHITE,"REMINDER",4);
LCD_drawButton(5,10,200,470,90,5,GREEN,WHITE,"BACK",4);

do{
  while(T_IRQ);
	x2=ReadTouchX();
	y2=ReadTouchY();
	sel2=ButtonTouch(x2,y2);
	delay_ms(10);
	switch(sel2){
		case 2: sel2= time_page();
						break;
    case 3: sel2=date_page();
						break;
   	case 4: sel2=reminder_page();
						break;
}
	
	

}while(sel2!=1 &&  sel2!=5);


	hour = readDs1307(0x02);  // real hour update
  min = readDs1307(0x01); // real min update
  sec = readDs1307(0x00); // real second update
setupTime(hour,min,sec);

LCD_clearButton();
LCD_fillScreen(BLACK);
if(sel2==1){
	return 1; // return value 1 will relode the RTC PAGE
}else{
	return 0;  // return value 0 will relode the MAIN_DISPLAY PAGE
} 

}