#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "i2c.h"
#include "ds1307.h"
extern unsigned char hour;
extern unsigned char min;
extern unsigned char sec;
void input_key_bord(void);
void input_12_key_bord(void);



void hour_setup(void){
int sel4,x4,y4;
unsigned char i=0,num,hour_arry[2]={' ',' '};
	
 input_key_bord();


do{

		if(i==0){
			for( i=0;i<=1;i++){
				while(T_IRQ);
				x4=ReadTouchX();
				y4=ReadTouchY();
				sel4=ButtonTouch(x4,y4);
				while(!T_IRQ);
			 
			if(0<=sel4 && sel4<=9){
					hour_arry[i]=sel4;
					LCD_setCursor(10+i*20,250);
					printf("%d",(int)hour_arry[i]);		
			}
			else{
			    i--;
			}
			
			
			if(sel4==11){
						i=0;
						hour_arry[2]=' ',' ';
					  LCD_fillRoundRect(10,250,220,50,5,RED);
						LCD_printC(290,260,"CLEARD",3,BLACK);
						delay_ms(500);
						LCD_fillRoundRect(240,250,220,50,5,WHITE);
						LCD_setText2Color(WHITE,RED);
	}
			
			
			if(num >24 && sel4==10){	
						LCD_printC(290,260,"error",3,BLACK);
						delay_ms(500);
						LCD_fillRoundRect(240,250,220,50,5,WHITE);
						LCD_setText2Color(WHITE,RED);
	}
			if(sel4==12){
			
			 i=2;
				hour = readDs1307(0x02);  // real hour update
        min = readDs1307(0x01); // real min update
        sec = readDs1307(0x00); // real second update
	
			}
			
		}
		
	}else{
	
		while(T_IRQ);
				x4=ReadTouchX();
				y4=ReadTouchY();
				sel4=ButtonTouch(x4,y4);
				while(!T_IRQ);
	
	}
	
	num=hour_arry[0]*10+hour_arry[1];
	
	if(sel4==11){
		i=0;
		hour_arry[2]=' ',' ';
	  LCD_fillRoundRect(10,250,220,50,5,RED);
		LCD_printC(290,260,"CLEARD",3,BLACK);
		delay_ms(500);
  	LCD_fillRoundRect(240,250,220,50,5,WHITE);
		LCD_setText2Color(WHITE,RED);
	}
	
	
	if(0<=num && num<=23 && sel4==10){
	
		LCD_printC(290,260,"SEND",3,BLACK);
		delay_ms(500);
		LCD_fillRoundRect(240,250,220,50,5,WHITE);
		LCD_setText2Color(WHITE,RED);
		hour=num;
		setupTime(hour,min,sec);
		hour = readDs1307(0x02);  // real hour update
		min = readDs1307(0x01); // real min update
		sec = readDs1307(0x00); // real second update

		
	}else if(num >24 && sel4==10){	
	  LCD_printC(290,260,"error",3,BLACK);
		delay_ms(500);
		LCD_fillRoundRect(240,250,220,50,5,WHITE);
		LCD_setText2Color(WHITE,RED);
	}
		
  



}while(sel4 != 12);

setupTime(hour,min,sec);
	hour = readDs1307(0x02);  // real hour update
  min = readDs1307(0x01); // real min update
  sec = readDs1307(0x00); // real second update
setupTime(hour,min,sec);

LCD_clearButton();
LCD_fillScreen(BLACK);
}