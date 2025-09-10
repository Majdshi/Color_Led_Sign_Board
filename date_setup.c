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

void date_setup(void){

int sel6,x6,y6;
unsigned char i=0,num,date_arry[2]={' ',' '};
	
 input_key_bord();


do{

		if(i==0){
			for( i=0;i<=1;i++){
				while(T_IRQ);
				x6=ReadTouchX();
				y6=ReadTouchY();
				sel6=ButtonTouch(x6,y6);
				while(!T_IRQ);
			 
			if(0<=sel6 && sel6<=9){
					date_arry[i]=sel6;
					LCD_setCursor(10+i*20,250);
					printf("%d",(int)date_arry[i]);		
			}
			else{
			    i--;
			}
			
			
			if(sel6==11){
						i=0;
						date_arry[2]=' ',' ';
					  LCD_fillRoundRect(10,250,220,50,5,RED);
						LCD_printC(290,260,"CLEARD",3,BLACK);
						delay_ms(500);
						LCD_fillRoundRect(240,250,220,50,5,WHITE);
						LCD_setText2Color(WHITE,RED);
	}
			
			
			if(num >31 && sel6==10){	
						LCD_printC(290,260,"error",3,BLACK);
						delay_ms(500);
						LCD_fillRoundRect(240,250,220,50,5,WHITE);
						LCD_setText2Color(WHITE,RED);
	}
			if(sel6==12){
			
			 i=2;
				
			 day = readDs1307(0x03);  // real day update
			 date = readDs1307(0x04); // real date update
			 month = readDs1307(0x05); // real month update
			 year = readDs1307(0x06); // real year update
				
			}
			
		}
		
	}else{
	
		while(T_IRQ);
				x6=ReadTouchX();
				y6=ReadTouchY();
				sel6=ButtonTouch(x6,y6);
				while(!T_IRQ);
	
	}
	
	num=date_arry[0]*10+date_arry[1];
	
	if(sel6==11){
		i=0;
		date_arry[2]=' ',' ';
	  LCD_fillRoundRect(10,250,220,50,5,RED);
		LCD_printC(290,260,"CLEARD",3,BLACK);
		delay_ms(500);
  	LCD_fillRoundRect(240,250,220,50,5,WHITE);
		LCD_setText2Color(WHITE,RED);
	}
	
	
	if(1<=num && num<=31 && sel6==10){
	
		LCD_printC(290,260,"SEND",3,BLACK);
		delay_ms(500);
		LCD_fillRoundRect(240,250,220,50,5,WHITE);
		LCD_setText2Color(WHITE,RED);
		date=num;
		setupdaydate(day,date,month,year); // seting start date
		day = readDs1307(0x03);  // real day update
		date = readDs1307(0x04); // real date update
		month = readDs1307(0x05); // real month update
		year = readDs1307(0x06); // real year update
		
	}else if(num >31 && sel6==10){	
	  LCD_printC(290,260,"error",3,BLACK);
		delay_ms(500);
		LCD_fillRoundRect(240,250,220,50,5,WHITE);
		LCD_setText2Color(WHITE,RED);
	}
		
  



}while(sel6 != 12);


setupdaydate(day,date,month,year); // seting start date
	day = readDs1307(0x03);  // real day update
  date = readDs1307(0x04); // real date update
  month = readDs1307(0x05); // real month update
	year = readDs1307(0x06); // real year update
setupdaydate(day,date,month,year); // seting start date

LCD_clearButton();
LCD_fillScreen(BLACK);
}