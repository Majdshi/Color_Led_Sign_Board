#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "i2c.h"
#include "ds1307.h"
U16 GetSec(){
	int x=0,y=0;
	U16 sec[2],sel,i=0;
	Init_Device();
		initSYS();
		LCD_drawButton(0,15,10,80,70,5,RED,WHITE,"0",4);
		LCD_drawButton(1,109,10,80,70,5,RED,WHITE,"1",4);
		LCD_drawButton(2,203,10,80,70,5,RED,WHITE,"2",4);
		LCD_drawButton(3,297,10,80,70,5,RED,WHITE,"3",4);
		LCD_drawButton(4,391,10,80,70,5,RED,WHITE,"4",4);
		LCD_drawButton(5,15,100,80,70,5,RED,WHITE,"5",4);
		LCD_drawButton(6,109,100,80,70,5,RED,WHITE,"6",4);
		LCD_drawButton(10,10,180,220,30,5,GREEN,WHITE,"Send",3);
		LCD_drawButton(11,240,180,220,30,5,BLUE,WHITE,"Clear",3);
		LCD_drawButton(12,240,180,220,30,5,BLUE,WHITE,"Back",3);
		LCD_fillRoundRect(10,250,460,50,5,RED);
		LCD_printC(10,220,"Second:",3,WHITE);
while(sell!=10&&sell!=12&&sell!=11)
	{		
	for(i=0;i<2;i++)
		{
			while(T_IRQ);
			delay_ms(10);
			x=ReadTouchX();
			y=ReadTouchY();
			sel=ButtonTouch(x,y);
			while(!T_IRQ);
		  delay_ms(20);
			sec[i]=sel;
			sec0=
		}
	}
	if(sel==10){
		
	}
			
				
			
		
}
	
	
	