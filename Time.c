#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "i2c.h"
#include "ds1307.h"
	U16 GetSec();
	void Time (){
		int x=0,y=0;
		U16 sel=0,second[],minute,hour;
		Init_Device();
		initSYS();
		LCD_fillScreen(BLACK);
		LCD_drawButton(1,160,50,150,70,5,RED,BLACK,"Second",3);
		LCD_drawButton(2,160,135,150,70,5,GREEN,BLACK,"Minute",3);
		LCD_drawButton(3,160,220,150,70,5,BLUE,BLACK,"Hour",3);
			while(T_IRQ);
			delay_ms(10);
			x=ReadTouchX();
			y=ReadTouchY();
			sel=ButtonTouch(x,y);
			while(!T_IRQ);
		  delay_ms(20);
		if(sel==1){
		second[]=GetSec();
		}
		
	

	}