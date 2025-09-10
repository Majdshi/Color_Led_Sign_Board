//===================== Example11_3.c ==============
#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "ds1307.h"

extern unsigned char day;
extern unsigned char date;
extern unsigned char month;
extern unsigned char year;


void input_key_bord(void);

void month_setup(void){

int sel7,x7,y7;
unsigned char i=0,num,month_arry[2]={' ',' '};
	
 input_key_bord();


do{

		if(i==0){
			for( i=0;i<=1;i++){
				while(T_IRQ);
				x7=ReadTouchX();
				y7=ReadTouchY();
				sel7=ButtonTouch(x7,y7);
				while(!T_IRQ);
			 
			if(0<=sel7 && sel7<=9){
					month_arry[i]=sel7;
					LCD_setCursor(10+i*20,250);
					printf("%d",(int)month_arry[i]);		
			}
			else{
			    i--;
			}
			
			
			if(sel7==11){
						i=0;
						month_arry[2]=' ',' ';
					  LCD_fillRoundRect(10,250,220,50,5,RED);
						LCD_printC(290,260,"CLEARD",3,BLACK);
						delay_ms(500);
						LCD_fillRoundRect(240,250,220,50,5,WHITE);
						LCD_setText2Color(WHITE,RED);
	}
			
			
			if(num >12 && sel7==10){	
						LCD_printC(290,260,"error",3,BLACK);
						delay_ms(500);
						LCD_fillRoundRect(240,250,220,50,5,WHITE);
						LCD_setText2Color(WHITE,RED);
	}
			if(sel7==12){
			
			 i=2;
				
			 day = readDs1307(0x03);  // real day update
			 date = readDs1307(0x04); // real date update
			 month = readDs1307(0x05); // real month update
			 year = readDs1307(0x06); // real year update
				
			}
			
		}
		
	}else{
	
		while(T_IRQ);
				x7=ReadTouchX();
				y7=ReadTouchY();
				sel7=ButtonTouch(x7,y7);
				while(!T_IRQ);
	
	}
	
	num=month_arry[0]*10+month_arry[1];
	
	if(sel7==11){
		i=0;
		month_arry[2]=' ',' ';
	  LCD_fillRoundRect(10,250,220,50,5,RED);
		LCD_printC(290,260,"CLEARD",3,BLACK);
		delay_ms(500);
  	LCD_fillRoundRect(240,250,220,50,5,WHITE);
		LCD_setText2Color(WHITE,RED);
	}
	
	
	if(1<=num && num<=12 && sel7==10){
	
		LCD_printC(290,260,"SEND",3,BLACK);
		delay_ms(500);
		LCD_fillRoundRect(240,250,220,50,5,WHITE);
		LCD_setText2Color(WHITE,RED);
		month=num;
		setupdaydate(day,date,month,year); // seting start date
		day = readDs1307(0x03);  // real day update
		date = readDs1307(0x04); // real date update
		month = readDs1307(0x05); // real month update
		year = readDs1307(0x06); // real year update
		
	}else if(num >12 && sel7==10){	
	  LCD_printC(290,260,"error",3,BLACK);
		delay_ms(500);
		LCD_fillRoundRect(240,250,220,50,5,WHITE);
		LCD_setText2Color(WHITE,RED);
	}
		
  



}while(sel7 != 12);


setupdaydate(day,date,month,year); // seting start date
	day = readDs1307(0x03);  // real day update
  date = readDs1307(0x04); // real date update
  month = readDs1307(0x05); // real month update
	year = readDs1307(0x06); // real year update
setupdaydate(day,date,month,year); // seting start date

LCD_clearButton();
LCD_fillScreen(BLACK);
}