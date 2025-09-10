#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "i2c.h"
#include "ds1307.h"

extern unsigned char hour;
extern unsigned char min;
extern unsigned char sec;

extern unsigned char rem_min ;

void input_key_bord(void);
	
unsigned char rem_min_setup(void){
int sel,x,y,i=0;
unsigned char num,m=-1,rem_min_arry[2]={' ',' '};

 input_key_bord();

do{

		if(i==0){
			for( i=0;i<=1;i++){
				while(T_IRQ);
				x=ReadTouchX();
				y=ReadTouchY();
				sel=ButtonTouch(x,y);
				while(!T_IRQ);
			 
			if(0<=sel && sel<=9){
					rem_min_arry[i]=sel;
					LCD_setCursor(10+i*20,250);
					printf("%d",(int)rem_min_arry[i]);		
			}
			else{
			    i--;
			}
			
			
			if(sel==11){
						i=0;
						rem_min_arry[2]=' ',' ';
					  LCD_fillRoundRect(10,250,220,50,5,RED);
						LCD_printC(290,260,"CLEARD",3,BLACK);
						delay_ms(500);
						LCD_fillRoundRect(240,250,220,50,5,WHITE);
						LCD_setText2Color(WHITE,RED);
	}
			
			
			if(num >59 && sel==10){	
						LCD_printC(290,260,"error",3,BLACK);
						delay_ms(500);
						LCD_fillRoundRect(240,250,220,50,5,WHITE);
						LCD_setText2Color(WHITE,RED);
	}
			if(sel==12){
			
			 i=2;
				hour = readDs1307(0x02);  // real hour update
        min = readDs1307(0x01); // real min update
        sec = readDs1307(0x00); // real second update
				setupTime(hour,min,sec);
			}
			
		}
		
	}else{
	
		while(T_IRQ);
				x=ReadTouchX();
				y=ReadTouchY();
				sel=ButtonTouch(x,y);
				while(!T_IRQ);
	
	}
	
	num=rem_min_arry[0]*10+rem_min_arry[1];
	
	if(sel==11){
		i=0;
		rem_min_arry[2]=' ',' ';
	  LCD_fillRoundRect(10,250,220,50,5,RED);
		LCD_printC(290,260,"CLEARD",3,BLACK);
		delay_ms(500);
  	LCD_fillRoundRect(240,250,220,50,5,WHITE);
		LCD_setText2Color(WHITE,RED);
	}
	
	
	if(0<=num && num<=59 && sel==10){
	
		LCD_printC(290,260,"SEND",3,BLACK);
		delay_ms(500);
		LCD_fillRoundRect(240,250,220,50,5,WHITE);
		LCD_setText2Color(WHITE,RED);
		m=num;


		
	}else if(num >59 && sel==10){	
	  LCD_printC(290,260,"error",3,BLACK);
		delay_ms(500);
		LCD_fillRoundRect(240,250,220,50,5,WHITE);
		LCD_setText2Color(WHITE,RED);
	}
		
  



}while(sel != 12);


	hour = readDs1307(0x02);  // real hour update
  min = readDs1307(0x01); // real min update
  sec = readDs1307(0x00); // real second update
setupTime(hour,min,sec);

LCD_clearButton();
LCD_fillScreen(BLACK);

return m;
}