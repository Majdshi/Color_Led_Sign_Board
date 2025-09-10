//===================== Example11_3.c ==============
#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "i2c.h"
#include "ds1307.h"
void Init_Device(void);
void main(void){	
	U8 second, minute, hour;
	Init_Device();
	initSYS();

	LCD_setTextSize(4);
	LCD_setText2Color(YELLOW, BLACK);	
	LCD_setCursor (50, 20);
	printf("Test DS1307 RTC");
	LCD_setText2Color(WHITE, BLACK);
   	setupTime(11,59, 56);
	while (1){
	     hour = readDs1307(0x02);
		minute = readDs1307(0x01);
		second = readDs1307(0x00);
	     printTime(hour,minute,second);
	}
}
