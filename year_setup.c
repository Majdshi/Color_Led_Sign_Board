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

void year_setup(void){

int sel8,x8,y8;
unsigned char i=0,num,year_arry[2]={' ',' '};
	
 input_key_bord();


do{

		if(i==0){
			for( i=0;i<=1;i++){
				while(T_IRQ);
				x8=ReadTouchX();
				y8=ReadTouchY();
				sel8=ButtonTouch(x8,y8);
				while(!T_IRQ);
			 
			if(0<=sel8 && sel8<=9){
					year_arry[i]=sel8;
					LCD_setCursor(10+i*20,250);
					printf("%d",(int)year_arry[i]);		
			}
			else{
			    i--;
			}
			
			
			if(sel8==11){
						i=0;
						year_arry[2]=' ',' ';
					  LCD_fillRoundRect(10,250,220,50,5,RED);
						LCD_printC(290,260,"CLEARD",3,BLACK);
						delay_ms(500);
						LCD_fillRoundRect(240,250,220,50,5,WHITE);
						LCD_setText2Color(WHITE,RED);
	}
			
			

			if(sel8==12){
			
			 i=2;
				
			 day = readDs1307(0x03);  // real day update
			 date = readDs1307(0x04); // real date update
			 month = readDs1307(0x05); // real month update
			 year = readDs1307(0x06); // real year update
				
			}
			
		}
		
	}else{
	
		while(T_IRQ);
				x8=ReadTouchX();
				y8=ReadTouchY();
				sel8=ButtonTouch(x8,y8);
				while(!T_IRQ);
	
	}
	
	num=year_arry[0]*10+year_arry[1];
	
	if(sel8==11){
		i=0;
		year_arry[2]=' ',' ';
	  LCD_fillRoundRect(10,250,220,50,5,RED);
		LCD_printC(290,260,"CLEARD",3,BLACK);
		delay_ms(500);
  	LCD_fillRoundRect(240,250,220,50,5,WHITE);
		LCD_setText2Color(WHITE,RED);
	}
	
	
	if(sel8==10){
	
		LCD_printC(290,260,"SEND",3,BLACK);
		delay_ms(500);
		LCD_fillRoundRect(240,250,220,50,5,WHITE);
		LCD_setText2Color(WHITE,RED);
		year=num;
		setupdaydate(day,date,month,year); // seting start date
		day = readDs1307(0x03);  // real day update
		date = readDs1307(0x04); // real date update
		month = readDs1307(0x05); // real month update
		year = readDs1307(0x06); // real year update
		}

		

}while(sel8 != 12);


setupdaydate(day,date,month,year); // seting start date
	day = readDs1307(0x03);  // real day update
  date = readDs1307(0x04); // real date update
  month = readDs1307(0x05); // real month update
	year = readDs1307(0x06); // real year update
setupdaydate(day,date,month,year); // seting start date

LCD_clearButton();
LCD_fillScreen(BLACK);
}